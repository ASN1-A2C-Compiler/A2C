using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class Set : Sequence
    {
        class Frame : ContextFrame
        {
            public bool[] m_rgFound;
            public int m_cbLeft;
            public int m_iField;

            public Frame (int cbLeft, int iField, bool[] rgFound)
            {
                m_rgFound = rgFound;
                m_cbLeft = cbLeft;
                m_iField = iField;
            }
        }

        private static Tag s_Tag = new Tag (TagClass.Universal, 17, TagType.Implicit);

        protected Set (ASNTable[] table) : base (table) { }

        protected Set (Set var) : base (var) { }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if (tag == null) throw new InvalidState ();

            EncodeCommon (flags, ctxt, stm, tag, fEncodeAsDer, true);
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

        internal void DecodeCommon (A2C_FLAGS flags, Context ctxt, Tag tagIn, ParserStream stmIn, bool fDecodeAsDer)
        {
            int cbLength;
            int cbTL;
            Tag tag;
            int iField;
            bool fConstructed;
            bool[] rgFound;

            ParserStream stmT;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                Frame frame = ctxt.Frames.Pop () as Set.Frame;

                cbLength = frame.m_cbLeft;
                iField = frame.m_iField;
                rgFound = frame.m_rgFound;
            }
            else {
                //
                //  Grab the type and length of the item
                //

                stmIn.PeekTagAndLength (out tag, out fConstructed, out cbLength, out cbTL);

                if (!fConstructed) {
                    throw new MalformedEncodingException ("Set is not constructed");
                }

                //
                //  Is the tag corrct?
                //

                if (tagIn == null) throw new InvalidState ();

                if (tag != tagIn) throw new TagMismatchException ("Expected tag " + tagIn.ToString ());

                //  Allocate the we found it buffer

                rgFound = new bool[m_tableX.Length];

                //  If doing DER - can't be indefinite length

                if (fDecodeAsDer && (cbLength == -1)) {
                    throw new MalformedEncodingException ();
                }

                //  Move forward over the TL bytes

                stmIn.Advance (cbTL);
            }

            //  Setup the buffer we are going to parse from which limits to the lenght of this object

            if (cbLength == -1) {
                stmT = stmIn;
            }
            else {
                stmT = new ParserStream (stmIn, Math.Min (cbLength, stmIn.Length - stmIn.Current));
            }

            //  We run until we have no more bytes to process

            while (cbLength != 0) {
                try {
                    if (cbLength == -1) {
                        int cbData;
                        stmT.PeekTagAndLength (out tag, out fConstructed, out cbData, out cbTL);
                        if (tag == new Tag (TagClass.Universal, 0, TagType.Implicit)) {
                            //
                            //  We found the end of indefinite marker - so we are finished with the loop
                            //

                            break;
                        }
                    }
                    else {
                        if (stmT.Length -stmT.Current== 0) {
                            //
                            //  We are out of data so break from the loop
                            //

                            break;
                        }
                    }

                    //
                    //  Try and decode it with any and all items
                    //

                    for (iField = 0; iField < m_tableX.Length; iField++) {
                        //  Have we already found this field?
                        if (m_data[iField] != null) continue;

                        Tag[] tagsChild = m_tableX[iField].tags;
                        ASN asnObj = m_tableX[iField].ASNType.Create ();

                        try {
                            asnObj.__Decode (flags, fDecodeAsDer, ctxt, tagsChild, stmT);
                            m_data[iField] = asnObj;
                            break;
                        }
                        catch (TagMismatchException e) {
                            //  Not really a problem
                            //  Just try the next field in the table
                        }
                        catch (NeedMoreDataException e) {
                            if (cbLength == -1) throw;
                            if (cbLength < stmT.Length) throw;

                            throw new MalformedEncodingException ("Invalid Set encoding");
                        }
                    }

                    if (iField == m_tableX.Length) {
                        throw new TagMismatchException ("Set tag does not match");
                    }

                }
                catch (NeedMoreDataException e) {
                    throw;
                }
            }

            //  We successfully fell out of the loop -
            //
            //  Check that what we found and what the table has match for manditory requirements
            //

            for (iField = 0; iField < m_tableX.Length; iField++) {
                //
                //  If we found and filled in the data - all is good
                //

                if (m_data[iField] != null) continue;

                //
                //  If the field has a default - fill in the default
                //

                if (m_tableX[iField].Default != null) {
                    m_data[iField] = m_tableX[iField].Default;
                    continue;
                }

                //
                //  If the field is optional - then it is fine to be absent
                //

                if (m_tableX[iField].IsOptional) continue;

                //
                //  If we get here - then the world is bad for this decoding
                //

                throw new MalformedEncodingException ("Missing field in set");
            }
        }
    }
}
