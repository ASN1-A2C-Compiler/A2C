using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class BMPString : GenericString
    {
        static Tag s_tag = new Tag (TagClass.Universal, 99, TagType.Implicit);

        //
        //  Various initializers
        //

        public BMPString ()
        {
        }

        public BMPString (string str) : base(str)
        {
        }

        public BMPString (BMPString val) : base(val)
        {
        }

        static public BMPString Create () { return new BMPString (); }

        //
        //  Overrider functions
        //

        protected override void _Encode (A2C_FLAGS flags, bool fDER, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_tag);
            _EncodeTags(flags, fDER, cctxt, tagsAll, stm);
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDER, Context ctxt, Tag[] tagsChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagsChild, s_tag);
            _DecodeTags (flags, fDER, ctxt, tagsAll, stm);
        }

        public override bool CheckCharacterSet ()
        {
            return true;
        }
    }
}
