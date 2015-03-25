using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public delegate ASN ASNCreator();

    public enum ASNTableFlags { Optional = 4, Unique=8 }
    public class ASNTable
    {
        readonly string  m_szFieldName;              // Name of the field
        readonly Tag[] m_tags;                        // Expected/required tag for the field
        readonly ASNType m_type;                     // Pointer to type class
        readonly ASN m_defaultValue;                 // Default value if one exists
        readonly ASNTableFlags m_flags;              // Controling flags
        readonly int m_iVersion;                     // Version # for the field

        public ASNTable(string szFieldName, Tag[] tags, ASNType type, ASN asnDefault, ASNTableFlags flags, int iVersion)
        {
            m_szFieldName = szFieldName;
            m_tags = tags;
            m_type = type;
            m_defaultValue = asnDefault;
            m_flags = flags;
            m_iVersion = iVersion;
        }


        public ASNType ASNType { get { return m_type; } }
        public ASN Default { get { return m_defaultValue; } }
        public bool HasDefault { get { return m_defaultValue != null; } }
        public bool IsOptional { get { return (m_flags & ASNTableFlags.Optional) != 0; } }
        public string name { get { return m_szFieldName; } }
        public Tag[] tags { get { return m_tags; } }
    }

    abstract public class Sequence : ASN
    {
        class Frame : ContextFrame
        {
            public int m_cbLeft;
            public int m_iField;

            public Frame (int cbLeft, int iField)
            {
                m_cbLeft = cbLeft;
                m_iField = iField;
            }
        }

        class ByteArray : IComparable
        {
            byte[] m_rgb;

            public ByteArray (byte[] rgb) { m_rgb = rgb; }

            public byte[] Data { get { return m_rgb; } set { m_rgb = value; } }
            public int Length { get { return m_rgb.Length; } }

            public int CompareTo (Object rhsObj)
            {
                ByteArray rhs = rhsObj as ByteArray;
                if (rhs == null) throw new ArgumentException ("Not a ByteArray");

                int max = Math.Min (m_rgb.Length, rhs.m_rgb.Length);
                for (int i = 0; i < max; i++) {
                    if (m_rgb[i] != rhs.m_rgb[i]) return m_rgb[i] - rhs.m_rgb[i];
                }
                return m_rgb.Length - rhs.m_rgb.Length;
            }

            public static void Sort (ByteArray[] rgItems)
            {
                int i;
                int j;

                for (i = 0; i < rgItems.Length; i++) {
                    if (rgItems[i] == null) continue;
                    for (j = i + 1; j < rgItems.Length; j++) {
                        if (rgItems[j] == null) {
                            continue;
                        }
                        else {
                            if (rgItems[i].CompareTo (rgItems[j]) > 0) {
                                ByteArray t = rgItems[i];
                                rgItems[i] = rgItems[j];
                                rgItems[j] = t;
                            }
                        }
                    }
                }
            }
        }


        private static Tag s_Tag = new Tag (TagClass.Universal, 16, TagType.Implicit);
        protected readonly ASNTable[] m_tableX;
        protected readonly ASN[] m_data;

        protected Sequence (ASNTable[] table, ASN[] data)
        {
            m_tableX = table;
            m_data = data;
        }

        protected Sequence (ASNTable[] table)
        {
            m_tableX = table;
            m_data = new ASN[table.Length];
        }

        protected Sequence (Sequence rhs)
        {
            m_tableX = rhs.m_tableX;
            m_data = new ASN[m_tableX.Length];
            for (int i = 0; i < m_data.Length; i++) {
                m_data[i] = rhs.m_data[i];
            }
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if (tag == null) throw new InvalidState ();

            EncodeCommon(flags, ctxt, stm, tag, fEncodeAsDer, false);
        }

        private void DecodeCommon(A2C_FLAGS flags, Context ctxt, Tag tagIn, ParserStream stmIn, bool fDER)
        {
            int iField;             // Index into the fields we are looking at

            int cbLength;
            int cbTL;
            Tag tag;
            bool fConstructed;

            ParserStream stmT;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                Frame frame = ctxt.Frames.Pop() as Sequence.Frame;

                cbLength = frame.m_cbLeft;
                iField = frame.m_iField;
            }
            else {
                //
                //   Grab the type and length of the item
                //

                stmIn.PeekTagAndLength(out tag, out fConstructed, out cbLength, out cbTL);

                if (!fConstructed) {
                    throw new MalformedEncodingException("Sequence is not constructed");
                }

                //
                //  Do we have the correct tag?
                //

                if (tagIn == null) throw new InvalidState ();

                if (tag != tagIn) {
                    throw new TagMismatchException("Expected the tag " + tagIn.ToString());
                }

                //
                //  Start with field #0
                //

                iField = 0;

                //
                //  If DER - then we can't do indefinite length encoding.
                //

                if (fDER && (cbLength == -1)) {
                    throw new MalformedEncodingException();
                }

                //
                //  Skip over the tag and length bytes
                //

                stmIn.Advance(cbTL);
            }

            //
            //  Limit the children decode to the bytes in the current object
            //

            if (cbLength == -1) {
                stmT = stmIn;
            }
            else {
                stmT = new ParserStream (stmIn, Math.Min (cbLength, stmIn.Length - stmIn.Current));
            }

            /*
             *  Start walking the fields to get the parsing done
             */

            for (; iField < m_tableX.Length; iField++) {
                try {
                    //
                    //  Allocate the item we are going to try and parse
                    //

                    ASN asnObj;

                    if (m_tableX[iField].ASNType == null) {
                        //  We don't know what the type is - it is based on other data.
                        asnObj = new A2C.Binary();
                    }
                    else {
                        asnObj = m_tableX[iField].ASNType.Create();
                    }
                    Tag[] tagsChild;

                    /*
                     *  Check if we have an implicit tag that is to be used in the decode. 
                     *      If so then make sure that we use it and not the default tag value.
                     */

                    tagsChild = m_tableX[iField].tags;

                    //  Check to see if more bytes exist

                    if (cbLength != -1) {
                        if (stmT.Current == stmT.Length) {
                            //  We have no more bytes - this needs to be an optional or default element
                            if (m_tableX[iField].IsOptional) continue;
                            if (m_tableX[iField].Default != null) {
                                m_data[iField] = m_tableX[iField].Default;
                                continue;
                            }

                            throw new MalformedEncodingException ("Manditory field " + m_tableX[iField].name + " not present");
                        }
                    }

                    /*
                     *  Perform the actual decode
                     */

                    try {
                            asnObj.__Decode (flags, fDER, ctxt, tagsChild, stmT);

                        m_data[iField] = asnObj;
                    }
                    catch (TagMismatchException e) {
                        if (m_tableX[iField].IsOptional) {
                            ;  //  This is fine - we just skip this item
                        }
                        else if (m_tableX[iField].Default != null) {
                            m_data[iField] = m_tableX[iField].Default; // Should be a clone
                        }
                        else throw;
                    }
                    catch (NeedMoreDataException e) {
                        //
                        //  We need more data in order to go forward with processing.  Under some circumstances this means that we
                        //  are finished and under others it means that we need more data.
                        //
                        //  If we are doing decoding of indefinite length, then we need more data.  In this case the ending conditions are
                        //      going to be a mis-matched tag (expected something and got 0) or we have run out of fields to check.
                        //
                        //  If we are doing a definite length encoding, then we know how many bytes are needed and if we have not consumed 
                        //      all of them then we need more data
                        //

                        if (cbLength == -1) throw;
                        if (cbLength < stmT.Length) throw;

                        //
                        //  We aren't really short of bytes - we had a potential decoding issue.
                        //

                        if (m_tableX[iField].IsOptional) {
                            ;  // This a good condition
                        }
                        else if (m_tableX[iField].Default != null) {
                            m_data[iField] = m_tableX[iField].Default; //  Should clone it
                        }
                        else throw new MalformedEncodingException ("Invalid input");
                    }
                }
                catch (NeedMoreDataException e) {
                    //  Adjust cbLength for any data consumed
                    if (cbLength != -1) {
                        cbLength -= stmT.Current;
                        stmIn.Advance(stmT.Current);
                    }
                    e.context.Frames.Push(new Frame(cbLength, iField));
                    throw;
                }

                /*
                 *  Clear recursion flag
                 */

                flags &= ~A2C_FLAGS.MORE_DATA;
            }

            /*
             *  If we did not use the entire blob - fail
             */

            if (cbLength != -1) {
                if (cbLength != stmT.Current) {
                    throw new MalformedEncodingException();
                }
            }

            /*
             *  Advance called in buffer by correct # of bytes
             */

            if (cbLength != -1) {
                stmIn.Advance(cbLength);
            }

            /*
             *  Look for the end of octets if needed
             */

            if (cbLength == -1) {
                stmIn.PeekTagAndLength(out tag, out fConstructed, out cbLength, out cbTL);

                /*
                 *  Should be tag=0, length=0
                 */

                if ((tag.Class != TagClass.Universal) && (tag.Value != 0) && (cbLength != 0)) {
                    throw new TagMismatchException("sequence end tag");
                }

                /*
                 *  Skip tag and length
                 */

                stmIn.Advance(cbTL);
            }
        }


        internal void EncodeCommon(A2C_FLAGS flags, Context ctxt, Stream stm, Tag tag, bool fDER, bool fSet)
        {
            Tag[] tagsChild;
            ByteArray[] rgrgb = null;
 
            // 
            //  We are always told the tag to be written out
            //

            stm.WriteTag(tag, true);

            //
            //  If we are encoding to a DER Set, then we need to create an array to hold the temp
            //  results in order to sort them
            //

            if (fDER && fSet) {
                rgrgb = new ByteArray[m_tableX.Length];
            }

            //
            //  If we are doing a BER encoding, then use indefinite length encoding.
            //

            if (!fDER) {
                stm.WriteLength(-1);
            }

            MemoryStream stmM = null;
            Stream stmT;

            if (fDER) {
                stmT = stmM = new MemoryStream ();
            }
            else stmT = stm;

            int cbSet = 0;

            for (int iItem = 0; iItem < m_tableX.Length; iItem++) {
                //
                //  If the field is not marked as optional and the field is not defined,
                //      throw an exception that the field is missing.
                //

                if (m_data[iItem] == null) {
                    if (!m_tableX[iItem].IsOptional) {
                        throw new MissingFieldException (m_tableX[iItem].name);
                    }
                    continue;
                }

                //
                //  If the field has a default, don't encode the value if it matches the default value.
                //

                if (m_tableX[iItem].HasDefault) {
                    if (m_data[iItem].Equals( m_tableX[iItem].Default)) {
                        continue;
                    }
                }

                //  Handle explicit and implicit tagging on the field
                //

                tagsChild = m_tableX[iItem].tags;

                if (m_data[iItem] == null) {
                    if (m_tableX[iItem].IsOptional) continue;
                    throw new MissingMemberException ("Sequence field " + iItem + " is missing");
                }

                //
                //   Call the encoder on the element
                //

                    m_data[iItem].__Encode (flags, fDER, ctxt, tagsChild, stmT);

            ErrorChecks:
                if (fDER && fSet) {
                    //
                    //  Stash the just encoded bytes for later sorting
                    //

                    rgrgb[iItem] = new ByteArray(stmM.data);
                    stmM.Clear();

                    /*
                     *  Running total # of bytes
                     */

                    cbSet += rgrgb[iItem].Length;
                }
            }


            //
            //  Now write out any cached results
            //

            if (fDER) {
                if (fSet) {
                    //
                    //  Write out the length of the data
                    //

                    stm.WriteLength(cbSet);

                    //
                    //  Now sort the data
                    //

                    ByteArray.Sort (rgrgb);

                    //
                    //  Now write out the data
                    //

                    foreach (ByteArray rgb in rgrgb) {
                        if (rgb != null) stm.WriteData(rgb.Data);
                    }
                }
                else {
                    //
                    //  Write out the length of the data
                    //

                    stm.WriteLength(stmM.Length);

                    //
                    //  Write out the data
                    //

                    stm.WriteData(stmM.data);
                }
            }
            else {
                //
                //  We are doing indefinite encoding - write out the end of data marker
                //

                stm.WriteTag(new Tag(TagClass.Universal, 0, TagType.Implicit), false);
                stm.WriteLength (0);
            }
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tag, ParserStream stm)
        {
            if (tag == null) throw new InvalidState ();

            DecodeCommon (flags, ctxt, tag, stm, fDecodeAsDer);
        }

    }
}
