using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    abstract public class ClassObject : ASN
    {
        private readonly ASNTable[] m_table;
        protected readonly ASN[] m_data;

        protected ClassObject (ASNTable[] table)
        {
            m_table = table;
            m_data = new ASN[table.Length];
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            throw new NotImplementedException ();
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tag, ParserStream stm)
        {
            throw new NotImplementedException ();
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            throw new NotImplementedException ();
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            throw new NotImplementedException ();
        }
    }
}
