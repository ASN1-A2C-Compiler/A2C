using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class Choice : ASN
    {
        ASNTable[] m_table;
        protected ASN m_data;
        protected int m_choice;

        public Choice (ASNTable[] table)
        {
            m_table = table;
            m_data = null;
        }

        public Choice (ASNTable[] table, int which, ASN obj)
        {
            m_table = table;
            m_choice = which;
            m_data = obj;
            // M00BUG check that the type is correct
        }

        public Choice (Choice rhs)
        {
            m_table = rhs.m_table;
            m_choice = rhs.m_choice;
            m_data = rhs.m_data;
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            if (m_data == null) throw new A2C_Exception ("Choice does not have value");
            Tag[] tags = Tag.Append(tag, m_table[m_choice].tags);
            m_data.__Encode (flags, fEncodeAsDer, cctxt, tags, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fDER, Context cctxt, Tag tag, Stream stm)
        {
            throw new InvalidState ();
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            _DecodeTags(flags, fDecodeAsDer, ctxt, tagChild, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDER, Context ctxt, Tag tagChild, ParserStream stm)
        {
            bool fConstructed;
            int cbLength;
            int cbTL;
            ASNTable itemType = null;


            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                throw new Exception ("Not yet implemented");
            }
            else {
                Tag tagLocal;
                int i = 0;

                stm.GetTagAndLength (out tagLocal, out fConstructed, out cbLength, out cbTL);

                foreach (ASNTable item in m_table) {
                    if (item.tags[0] == tagLocal) {
                        itemType = item;
                        m_choice = i;
                        break;
                    }
                    i += 1;
                }

                if (itemType == null) throw new A2C.MalformedEncodingException ("Choice element not found");

                m_data = itemType.ASNType.Create ();
            }

            m_data.__Decode (flags, fDER, ctxt, itemType.tags, stm);
        }
    }
}
