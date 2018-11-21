using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class IA5String : GenericString
    {   

    static readonly Tag m_Tag = new Tag (TagClass.Universal, 28, TagType.Implicit);

        //
        //  Various initializers
        //

        public IA5String ()
        {
        }

        public IA5String (string str)
        {
            m_str = str;
        }

        public IA5String (IA5String val)
        {
            m_str = val.m_str;
        }

        //
        //  Get/Set functions
        //

        //
        //  Overrider functions
        //

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, m_Tag);
            _EncodeTags (flags, fEncodeAsDer, ctxt, tagsAll, stm);
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, m_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        public override bool CheckCharacterSet ()
        {
            return true;
        }


        //

        public static IA5String Create () { return new IA5String (); }
    }

}
