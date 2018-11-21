using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    abstract public class SetOf : SequenceOf
    {
        static readonly Tag s_Tag = new Tag (TagClass.Universal, 17, TagType.Implicit);

        protected SetOf (ASNTable entry) : base (entry) { }

        protected SetOf (SetOf var) : base (var) { }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tags, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tags, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tags, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tags, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if (tag == null) throw new InvalidState ();

            if (!fEncodeAsDer) {
                base._EncodePrimative (flags, fEncodeAsDer, ctxt, tag, stm);
                return;
            }

            //  Get the correct tag and write it out

            if (tag == null) tag = s_Tag;
            stm.WriteTag (tag, true);

            //  What is the tag for the embeded items?

            Tag[] tagLocal = m_tableX.tags;
            

            //  where we encode to

            MemoryStream stmLocal = new MemoryStream();
            List<byte[]> lstChildren = new List<byte[]>();
            int cbSet = 0;

            //  Now encode each of the child nodes

            foreach (ASN item in m_lst) {
                //

                item.__Encode (0, fEncodeAsDer, ctxt, tagLocal, stmLocal);

                lstChildren.Add (stmLocal.data);
                cbSet += lstChildren[lstChildren.Count-1].GetLength(0);

                stmLocal.Clear ();
            }

            //  Write out our length

            stm.WriteLength (cbSet);

            //  Sort the list


            lstChildren.Sort (CompareByteArrays);

            //  Write the data out

            foreach (byte[] rgb in lstChildren) {
                stm.WriteData (rgb);
            }
        }

        private static int CompareByteArrays (byte[] lhs, byte[] rhs)
        {
            if (lhs == null) {
                if (rhs == null) return 0;
                return -1;
            }
            else {
                if (rhs == null) return 1;

                int c = Math.Min (lhs.GetLength (0), rhs.GetLength (0));

                for (int i = 0; i < c; i++) {
                    if (lhs[i] != rhs[i]) return lhs[i] - rhs[i];
                }

                return lhs.GetLength (0) - rhs.GetLength (0);
            }
        }
    }
}
