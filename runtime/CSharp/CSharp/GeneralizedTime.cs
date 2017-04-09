using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class GeneralizedTime : ASN
    {
        private String m_str;
        static readonly Tag m_Tag = new Tag (TagClass.Universal, 28, TagType.Implicit);

        //
        //  Various initializers
        //

        public GeneralizedTime ()
        {
        }

        public GeneralizedTime (string str)
        {
            m_str = str;
        }

        public GeneralizedTime (GeneralizedTime val)
        {
            m_str = val.m_str;
        }

        public GeneralizedTime (DateTime date)
        {
            m_str = date.ToShortDateString();
        }

        static public GeneralizedTime Create () { return new GeneralizedTime (); }

        //
        //  Get/Set functions
        //

        public String Value { get { return m_str; } set { m_str = value; } }

        //
        //  Overrider functions
        //


        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, m_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {

            if (m_str == null) {
                throw new Exception ("Invalid State");
            }

            Encoding enc = new UTF8Encoding();
            byte[] rgb = enc.GetBytes (m_str);

            if (tag == null) throw new  InvalidState();
                stm.WriteTag (tag, false);

            stm.WriteLength (rgb.Length);

            stm.WriteData (rgb);
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, m_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tagChild, ParserStream stm)
        {
            OctetString os = new OctetString ();
            if (tagChild == null) throw new InvalidState ();
            os.__Decode(flags, fDecodeAsDer, ctxt, new Tag[]{tagChild},  stm);

            Encoding enc = new UTF8Encoding ();
            m_str = enc.GetString (os.Value);
        }
    }
}
