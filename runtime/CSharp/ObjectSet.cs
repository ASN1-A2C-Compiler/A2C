using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class ObjectSet : ASN
    {
        ASN[] m_rgItems;

        public ObjectSet ()
        {
            m_rgItems = null;
        }

        public ObjectSet (ASN[] rgItems)
        {
            m_rgItems = rgItems;
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
