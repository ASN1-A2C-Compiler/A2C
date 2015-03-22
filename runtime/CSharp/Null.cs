using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class Null : ASN
    {
        static readonly Tag s_Tag = new Tag(TagClass.Universal, 5, TagType.Implicit);

        //
        //  Various initializers
        //

        public Null ()
        {
        }

        public Null (Null val)
        {
        }

        static public Null Create () { return new Null (); }

        //  Comparision routines

        public override bool Equals (object obj)
        {
            Null rhs = obj as Null;
            if (rhs == null) return false;

            return base.Equals (obj);
        }

        public override int GetHashCode ()
        {
            return 1;
        }

        //  Print functions

        protected override void _Print (int iDepth, TextStream stm)
        {
            stm.Write ("NULL");
        }

        public override string ToString ()
        {
            return "NULL";
        }

        //
        //  Overrider functions
        //

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if (tag == null) throw new InvalidState ();
                    stm.WriteTag (tag, false);

            stm.WriteLength (0);
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tagChild, ParserStream stm)
        {
            Tag tagLocal;
            int cbLength;
            int cbTL;
            bool fConstructed;

            stm.PeekTagAndLength (out tagLocal, out fConstructed, out cbLength, out cbTL);

            if (tagChild == null) throw new InvalidState ();

            if (tagChild != tagLocal) throw new TagMismatchException ("NULL");

            stm.Advance (cbTL);
        }
    }
}
