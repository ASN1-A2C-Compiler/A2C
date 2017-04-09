using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class UtcTime : GenericString
    {
        static readonly Tag s_Tag = new Tag (TagClass.Universal, 23, TagType.Implicit);


        //
        //  Various initializers
        //

        public UtcTime ()
        {
        }

        public UtcTime (string str)
        {
            m_str = str;
        }

        public UtcTime (UtcTime val)
        {
            m_str = val.m_str;
        }

        static public UtcTime Create () { return new UtcTime (); }

        //
        //  Get/Set functions
        //


        //
        //  Overrider functions
        //

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append(tagChild, s_Tag);
            _DecodeTags(flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append(tag, s_Tag);
            _EncodeTags(flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative(A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {

            if (m_str == null) {
                throw new Exception("Invalid State");
            }

            Encoding enc = new UTF8Encoding();
            byte[] rgb = enc.GetBytes(m_str);

            if (tag == null) throw new InvalidState();
            stm.WriteTag(tag, false);

            stm.WriteLength(rgb.Length);

            stm.WriteData(rgb);
        }

        public override bool CheckCharacterSet ()
        {
            throw new NotImplementedException ();
        }
    }
}
