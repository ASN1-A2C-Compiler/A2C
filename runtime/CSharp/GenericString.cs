using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public  abstract class GenericString : ASN
    {
        protected String m_str;

        //
        //  Various initializers
        //

        protected GenericString ()
        {
        }

        protected GenericString (string str)
        {
            m_str = str;
#if STRICT_STRINGS
            if (!CheckCharacterSet()) throw new Exception("Invalid String");
#endif
        }

        protected GenericString (GenericString rhs)
        {
            m_str = rhs.m_str;
        }

        //
        //  Get/Set functions
        //

        public String Value { 
            get { return m_str; } 
            set { 
                m_str = value;
#if STRICT_STRINGS
                if (!CheckCharacterSet()) throw new Exception("Invalid String");
#endif
            } 
        }

        //  Check the set of characters in the string

        public abstract bool CheckCharacterSet();

        //  Real encode/decode functions

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if ((flags & A2C_FLAGS.STRICT) != 0) {
                if (!CheckCharacterSet ()) {
                    throw new Exception ("Invalid character in string");
                }
            }

            if (m_str == null) {
                throw new Exception ("Invalid State");
            }

            Encoding enc = new UTF8Encoding ();
            byte[] rgb = enc.GetBytes (m_str);

            if (tag == null) throw new InvalidState ();
            stm.WriteTag (tag, false);

            stm.WriteLength (rgb.Length);

            stm.WriteData (rgb);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tagChild, ParserStream stm)
        {
            OctetString os = new OctetString ();
            if (tagChild == null) throw new InvalidState ();

            os._DecodePrimative(flags, fDecodeAsDer, ctxt, tagChild,  stm);

            Encoding enc = new UTF8Encoding ();
            m_str = enc.GetString (os.Value);

            if ((flags & A2C_FLAGS.STRICT) != 0) {
                if (!CheckCharacterSet ()) {
                    throw new Exception ("Invalid character in string");
                }
            }
        }
    }
}
