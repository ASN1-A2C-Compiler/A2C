using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class OctetString : ASN
    {
        class Frame : ContextFrame
        {
            public MemoryStream stm;

            public Frame (MemoryStream stm1) { stm = stm1; }
        }

        static readonly Tag s_Tag = new Tag(TagClass.Universal, 4, TagType.Implicit);

        byte[] m_rgb= null;

        public OctetString() {}

        public OctetString(byte[] rgb) { m_rgb = rgb; }

        public OctetString (OctetString val) { m_rgb = (byte[]) val.m_rgb.Clone (); }

        public byte[] Value { get { return m_rgb; } set { m_rgb = value; } }

        public static OctetString Create () { return new OctetString (); }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tags, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tags, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tagChild, ParserStream stm)
        {
            if (fDecodeAsDer) {
                _DecodeDER (flags, ctxt, tagChild, stm);
                return;
            }

            int cbLength;
            int cbTL;
            bool fConstructed;
            Tag tagLocal;
            int iBits = 0;

            MemoryStream stmDst = null;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                Frame frame = ctxt.Frames.Pop () as OctetString.Frame;
                if (frame == null) throw new InvalidState ("Bad context frame");

                stmDst = frame.stm;

                if (ctxt.Frames.Count == 0) flags &= ~A2C_FLAGS.MORE_DATA;

            }
            else {
                stm.PeekTagAndLength (out tagLocal, out fConstructed, out cbLength, out cbTL);

                if (tagChild == null) throw new InvalidState ();
                if (tagLocal != tagChild) throw new TagMismatchException ("Octet String");

                stmDst = new MemoryStream ();
            }

            try {
                Binary.RecurseGetLength (stm, stmDst, flags, ctxt, s_Tag, false, ref iBits);

                m_rgb = stmDst.data;
            }
            catch (NeedMoreDataException e) {
                e.context.Frames.Push (new Frame (stmDst));
                throw;
            }
        }

        void _DecodeDER (A2C_FLAGS flags, Context ctxt, Tag tagChild, ParserStream stm)
        {
            Tag tagLocal;
            bool fConstructed;
            int cbData;
            int cbTL;

            stm.GetTagAndLength (out tagLocal, out fConstructed, out cbData, out cbTL);

            if (tagChild == null) tagChild = s_Tag;
            if (tagChild != tagLocal) throw new A2C.TagMismatchException ("Octet String");

            if (fConstructed || cbData == -1) throw new A2C.MalformedEncodingException ("Malformed DER");

            if (stm.Length < cbData) throw new A2C.NeedMoreDataException ("Need more Data");

            stm.Advance(cbTL);

            m_rgb = stm.Read(cbData);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tags, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tags, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative(A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
{
            if (tag == null) throw new InvalidState();

            stm.WriteTag (tag, false);

            stm.WriteLength (m_rgb.Length);

            stm.WriteData (m_rgb);
        }
    }
}
