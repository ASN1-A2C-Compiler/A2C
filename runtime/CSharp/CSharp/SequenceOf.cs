using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class SequenceOf : ASN, IEnumerable
    {
        static readonly Tag s_Tag = new Tag (TagClass.Universal, 16, TagType.Implicit);
        internal protected ASNTable m_tableX;
        protected List<ASN> m_lst;

        protected SequenceOf (ASNTable table)
        {
            m_tableX = table;
            m_lst = new List<ASN> ();
        }

        protected SequenceOf (SequenceOf rhs)
        {
            m_tableX = rhs.m_tableX;
            m_lst = new List<ASN> ();
            foreach (ASN obj in rhs.m_lst) m_lst.Add (obj);
        }

        public void Add(ASN node) { m_lst.Add(node); }
        public int Count { get { return m_lst.Count; } }
        public ASN this[int i]
        {
            get { return m_lst[i]; }
            set { m_lst[i] = value; }
        }
        public ASN GetIndex (int i) { return m_lst[i]; }
        public void SetIndex (int i, ASN value) { m_lst[i] = value; }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tag, ParserStream stm)
        {
            if (tag == null) throw new InvalidState ();
            DecodeCommon (fDecodeAsDer, flags, tag, ctxt, stm);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tags, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tags, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if (fEncodeAsDer) {
                _EncodeDER (flags, ctxt, stm, tag);
                return;
            }

            if (tag == null) throw new InvalidState ();
            stm.WriteTag (tag, true);

            //  Write out a length
            stm.WriteLength (-1);

            //  Do we have an override chid tag?
            Tag[] localTag = m_tableX.tags;

            foreach (ASN field in m_lst) {
                field.__Encode (flags, false, ctxt, localTag, stm );
            }

            //  Write out terminator tag
            stm.WriteTag (new Tag (TagClass.Universal, 0, TagType.Implicit), false);
            stm.WriteLength (0);
        }

        private void _EncodeDER (A2C_FLAGS flags, Context cctxt, Stream stm, Tag tag)
        {
            if (tag == null) throw new InvalidState ();

            stm.WriteTag (tag, true);

            //  Is there an overide tag in the table?

            Tag[] localTag = m_tableX.tags;

            A2C.MemoryStream stmLocal = new MemoryStream ();

            foreach (ASN field in m_lst) {
                field.__Encode (flags, true, cctxt, localTag, stmLocal);
            }

            byte[] rgb = stmLocal.data;

            stm.WriteLength (rgb.Length);
            stm.WriteData (rgb);
        }

        private void DecodeCommon (bool fDer, A2C_FLAGS flags, Tag tagChild, Context cctxt, ParserStream stm)
        {
            Tag tagLocal;
            bool fConstructed;
            int cbData;
            int cbTL;
            Tag[] tagsChild;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                throw new Exception ("NYI");
            }
            else {
                stm.GetTagAndLength (out tagLocal, out fConstructed, out cbData, out cbTL);

                if (tagChild != tagLocal) throw new TagMismatchException ("Incorrect Tag");

                if (fDer && (cbData == -1)) throw new A2C.MalformedEncodingException ("not DER encoded");

                stm.Advance (cbTL);
            }

            ParserStream stm2;

            if (cbData == -1) stm2 = stm;
            else {
                stm2 = new ParserStream (stm, Math.Min (cbData, stm.Length - stm.Current));
            }

            tagsChild = m_tableX.tags;

            for (; (cbData == -1) || (cbData - stm2.Current > 0); ) {
                if (cbData == -1) {
                    int cbLength;
                    Tag tag2;

                    stm2.GetTagAndLength (out tag2, out fConstructed, out cbLength, out cbTL);

                    if ((tag2.Class == TagClass.Universal) && (tag2.Value == 0)) {

                        stm.Advance (cbTL);
                        break;
                    }
                }

                ASN node = m_tableX.ASNType.Create ();

                node.__Decode (flags, fDer, cctxt, tagsChild, stm2);
                m_lst.Add (node);

                flags &= ~A2C_FLAGS.MORE_DATA;

            }

            if (stm != stm2) {
                stm.Advance (cbData);
            }
        }

        public IEnumerator GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }
    }
}
