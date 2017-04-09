using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class UTF8String : GenericString
    {
        static readonly Tag s_Tag = new Tag (TagClass.Universal, 12, TagType.Implicit);

        //
        //  Various initializers
        //

        public UTF8String ()
        {
        }

        public UTF8String(UTF8String str) : base(str) { }
        public UTF8String (string str) : base(str) {}

        static public UTF8String Create () { return new UTF8String (); }
 
        //
        //  Overrider functions
        //

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, ctxt, tagsAll, stm);
        }

        public override bool CheckCharacterSet ()
        {
            return true;
        }
    }
}
