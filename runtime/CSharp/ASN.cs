using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public enum A2C_FLAGS { MORE_DATA = 1, STRICT=2 }

    /// <summary>
    /// ASN is the base class for all ASN.1 encoding/decoding types
    /// </summary>
    public abstract class ASN
    {
        private class Frame : ContextFrame
        {
            byte[] m_data;

            public Frame (byte[] data) { m_data = data; }

            public byte[] data { get { return m_data; } }
        }

        public void EncodeBER(A2C_FLAGS flags, Context ctxt, Stream stm)
        {
            _Encode(flags, false, ctxt, null, stm);
        }


        public void EncodeDER(A2C_FLAGS flags, Context ctxt, Stream stm)
        {
            _Encode(flags, true, ctxt, null, stm);
        }

        public void DecodeBER(A2C_FLAGS flags, Context ctxt, byte[] data)
        {
            _DecodeCommon(false, flags, ctxt, data);
        }

        public void DecodeDER(A2C_FLAGS flags, Context ctxt, byte[] data)
        {
            _DecodeCommon(true, flags, ctxt, data);
        }

        //
        //  Functions that all classes derived from this class will be required to derive
        //

        protected abstract void _Encode(A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm);
        protected abstract void _Decode(A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm);

        protected virtual void _Print(int iDepth, TextStream stm)
        {
            stm.Write("Print Not Implemented for " + this.GetType().FullName);
        }
        protected virtual bool _Validate (TextStream stm)
        {
            return true;
        }

        internal void __Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tags, Stream stm)
        { 
            _Encode(flags, fEncodeAsDer, ctxt, tags, stm);
        }

        internal void __Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagsChild, ParserStream stm)
        {
            _Decode (flags, fDecodeAsDer, ctxt, tagsChild, stm);
        }

        internal abstract void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm);
        internal abstract void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tag, ParserStream stm);

        protected void _EncodeTags (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tags, Stream stm)
        {
            if ((tags == null) || (tags.Length == 0)) _EncodePrimative (flags, fEncodeAsDer, ctxt, null, stm);
            else {
                EncodeTagsRecurse (flags, fEncodeAsDer, ctxt, tags[0], tags, 0, stm);
            }
        }

        private void EncodeTagsRecurse(A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tagNext, Tag[] tags, int iTag, Stream stm)
        {
            while (iTag < tags.Length) {
                if (tags[iTag].TagType == TagType.Explicit) {

                    MemoryStream stmMemory = new MemoryStream ();
                    Stream stm2 = stmMemory;

                    stm.WriteTag (tagNext, true);
                    if (fEncodeAsDer) {
                    }
                    else {
                        stm.WriteLength (-1);
                        stm2 = stm;
                    }

                    EncodeTagsRecurse (flags, fEncodeAsDer, ctxt, tags[iTag + 1], tags, iTag + 1, stm2);

                    if (fEncodeAsDer) {
                        byte[] rgb = stmMemory.data;

                        stm.WriteLength (rgb.Length);
                        stm.WriteData (rgb);
                    }
                    else {
                        stm.WriteTag (new Tag (TagClass.Universal, 0, TagType.Implicit), false);
                        stm.WriteLength (0);
                    }

                    return;
                }
                iTag += 1;
            }
            _EncodePrimative (flags, fEncodeAsDer, ctxt, tagNext, stm);
        }

        protected void _DecodeTags (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tags, ParserStream stm)
        {
            if (tags == null) _DecodePrimative (flags, fDecodeAsDer, ctxt, null, stm);
            else if (tags.Length == 1) _DecodePrimative (flags, fDecodeAsDer, ctxt, tags[0], stm);
            else {
                DecodeTagsRecurse (flags, fDecodeAsDer, ctxt, tags, tags[0], 0, stm);
            }
        }

        private void DecodeTagsRecurse (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tags, Tag topTag, int iTag, ParserStream stmIn)
        {
            while (iTag < tags.Length) {
                if (tags[iTag].TagType == TagType.Explicit) {
                    ParserStream stmT;
                    int cbLength;
                    Tag tagFound;
                    bool fConstructed;
                    int cbTL;

                    if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                        cbLength = 0;
                    }
                    else {

                        //  Parse the explict tag at this point

                        stmIn.PeekTagAndLength (out tagFound, out fConstructed, out cbLength, out cbTL);
                        if (tagFound != topTag) throw new TagMismatchException ("Explicit tag not found");

                        if (cbLength == -1) {
                            if (fDecodeAsDer) throw new MalformedEncodingException ("Not DER encoding");
                        }

                        stmIn.Advance (cbTL);
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

                    DecodeTagsRecurse (flags, fDecodeAsDer, ctxt, tags, tags[iTag + 1], iTag + 1, stmIn);

                    if (cbLength == -1) {
                        stmIn.PeekTagAndLength (out tagFound, out fConstructed, out cbLength, out cbTL);

                        if (tagFound != new Tag (TagClass.Universal, 0, TagType.Implicit)) throw new TagMismatchException ("Missing close tag");
                        stmIn.Advance (cbTL);
                    }

                    return;
                }
                iTag += 1;
            }

            //  Emit now
            _DecodePrimative (flags, fDecodeAsDer, ctxt, topTag, stmIn);
        }

        //
        //  Private routines
        //

        void _DecodeCommon(bool fDER, A2C_FLAGS flags, Context ctxt, byte[] data)
        {
            ParserStream stm;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                Frame frame = ctxt.Frames.Pop() as ASN.Frame;
                if (frame == null) throw new InvalidState ();

                byte[] data2 = frame.data;
                int cbCache = frame.data.Length;

                Array.Resize(ref data2, frame.data.Length + data.Length);
                Array.Copy (data, 0, data2, cbCache, data.Length);

                if (ctxt.Frames.Count == 0) {
                    flags &= ~A2C_FLAGS.MORE_DATA;
                }

                /*
                 *  Setup the stream buffer to point to ALL of the data
                 */

                stm = new ParserStream(data2);
            }
            else {
            
                //
                //  Setup the buffer object with the passed in data
                //

                stm = new ParserStream(data);
            }


            //
            //  Setup the decode roots array and initialize it
            //

            try {
                //
                //  Now do the actual decode operation
                //

                _Decode (flags, fDER, ctxt, null, stm);
            }
            catch (NeedMoreDataException e) {
                byte[] dataSaved = stm.Read (stm.Length - stm.Current);

                e.context.Frames.Push (new Frame (dataSaved));

                throw e;
            }
        }
    }


    /// <summary>
    /// Context is used by the encoder and decoder functions to keep information about where things are to do
    /// incremental encode/decode operations.
    /// </summary>
    public class Context
    {
        Stack<ContextFrame> m_FrameList = new Stack<ContextFrame> ();

        public Context () { }

        public Stack<ContextFrame> Frames { get { return m_FrameList; } }
    }

    public class ContextFrame
    {
    }



    public enum TagClass { Universal = 0, Application = 1, Context=2, Private=3 }
    public enum TagType { Implicit = 1, Explicit = 2, Automatic = 3 };

    public class Tag
    {
        //  A tag consists of a class and a value.

        TagClass m_class;
        int m_iTagValue;
        TagType m_tagType;

        public Tag(TagClass classVal, int iVal, TagType tagType)
        {
            m_class = classVal;
            m_iTagValue = iVal;
            m_tagType = tagType;
        }

        public TagClass Class
        {
            get { return m_class; }
        }

        public int Value
        {
            get { return m_iTagValue; }
        }

        public TagType TagType { get { return m_tagType; } }

        public override bool Equals (object obj)
        {
            return this.Equals (obj as Tag);
        }

        public bool Equals (Tag val)
        {
            if (val == null) return false;
            if (Object.ReferenceEquals (this, val)) return true;
            if (this.GetType () != val.GetType ()) return false;
            return (m_iTagValue == val.m_iTagValue) && (m_class == val.m_class);
        }

        public static bool operator == (Tag lhs, Tag rhs)
        {
            if (Object.ReferenceEquals(lhs, null)) {
                return Object.ReferenceEquals(rhs, null);
            }
            return lhs.Equals(rhs);
        }

        public static bool operator != (Tag lhs, Tag rhs)
        {
            return !(lhs==rhs);
        }

        public override int GetHashCode ()
        {
            return (int) m_class * 4096 + m_iTagValue;
        }


        public byte[] ToBytes(bool fConstructed)
        {
            //
            //  Allocate the buffer
            //

            byte[] rgb;

            //
            //  Copy bytes to the buffer.
            //
            //  Deal with the simple case - encoded into a single byte

            if (m_iTagValue < 31) {
                rgb = new byte[1];
                rgb[0] = (byte) ((((int)m_class) << 6) | (fConstructed ? 0x20 : 0) | m_iTagValue);

                return rgb;
            }

            //
            //  Now deal with the potential multiple byte encodings.
            //

            byte[] rgbTmp = new byte[6];
            byte b = 0;
            int tag = m_iTagValue;
            int i;

            for (i = 5; i >= 0; i--) {
                //
                //  Save the current value
                //

                rgbTmp[i] =  (byte)((tag & 0x7f) | b);

                //
                //  Do the shift
                //

                tag = tag >> 7;

                //
                //  If zero, then we are done.
                //

                if (tag == 0) break;

                //
                //  Mark as having a continutaion
                //

                b = 0x80;
            }

            //
            //  Or in the class and construction flags
            //

            rgbTmp[i - 1] = (byte) ((((int) m_class) << 6) | (fConstructed ? 0x20 : 0) | 31);

            byte[] rgbT = new byte[6 - i];
            rgbT.CopyTo(rgbTmp, i - 1);
            return rgbT;
        }

        public static Tag[] Append (Tag[] rgTags, Tag tag)
        {
            Tag[] tags;

            if (tag == null) return rgTags;
            if (rgTags == null) return new Tag[] { tag };

            tags = new Tag[rgTags.Length + 1];
            rgTags.CopyTo (tags, 0);
            tags[rgTags.Length] = tag;
            return tags;
        }

        public static Tag[] Append (Tag[] tagsIn, Tag[] tagsAdd)
        {
            Tag[] tagsOut;

            if (tagsAdd == null) return tagsIn;
            if (tagsIn == null) return tagsAdd;

            tagsOut = new Tag[tagsIn.Length + tagsAdd.Length];
            tagsIn.CopyTo (tagsOut, 0);
            tagsAdd.CopyTo (tagsOut, tagsIn.Length);

            return tagsOut;
        }

    }
}
