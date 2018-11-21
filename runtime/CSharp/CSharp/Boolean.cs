using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class Boolean : ASN
    {
        static Tag s_Tag = new Tag (TagClass.Universal, 1, TagType.Implicit);
        //
        //  The internal data may be stored in two different formats depending on
        //  the actual current value and what was last set.  It will either be an integer,
        //  if it can be stored in the native format, or a length and byte array.
        //

        int m_Value;

        //
        //  Support various constructors that vary based on the passed in value.
        //  Note that all constructors clone the passed in value rather than just point to
        //  the passed in value
        //

        public Boolean()
        {
        }

        public Boolean(Boolean src)
        {
            m_Value = src.m_Value;
        }

        public Boolean(bool b)
        {
            m_Value = b ? 1 : 0;
        }

 
        //
        //  Routines to get and set values
        //

        public bool Value
        {
            get
            {
                return m_Value != 0;
            }

            set
            {
                m_Value = value ? 1 : 0;
            }
        }

        //
        //  Comparison routines
        //

        public override bool Equals (object obj)
        {
            A2C.Boolean rhs = obj as A2C.Boolean;
            if (rhs == null) return false;

            if (!base.Equals (obj)) return false;

            return m_Value == rhs.m_Value;
        }

        public override int GetHashCode ()
        {
            return m_Value == 0 ? 0 : 1;
        }


        //
        //  Utility Functions
        //

        protected override void _Print (int iDepth, TextStream stm)
        {
            if (m_Value != 0) stm.Write ("TRUE");
            else stm.Write ("FALSE");

            return;
        }

        //
        //  Routines that need to be over-ridden
        //

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        override internal void _EncodePrimative (A2C_FLAGS flags, bool fDER, Context cctxt, Tag tag, Stream stm)
        {
            //
            //  Either write out the tag that was passed in, or out normal default tag
            //

            if (tag == null) throw new InvalidState ();
                stm.WriteTag(tag, false);

            //
            //  Write out the length of the data
            //

            stm.WriteLength(1);

            //
            //  Write out the actual bytes
            //

            byte[] b = new byte[1]{(byte) ((m_Value != 0) ? 0xff : 0)};
            stm.WriteData(b);
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, s_Tag);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        } 


        internal override void _DecodePrimative(A2C_FLAGS flags, bool fDER, Context ctxt, Tag tagChild, ParserStream stm)
        {
            int cbLength;
            int cbTL;
            bool fConstructed;
            Tag tag;

            /*
             *  Grab the type and length of this item.  This function will without advancing
             *  the pointer if there are not sufficent bytes for the value in the buffer.
             */

            stm.PeekTagAndLength(out tag, out fConstructed, out cbLength, out cbTL);

            /*
             *  Compare the acutal and expected tags.  It may be passed in or uses the default ones.
             */

            if (tagChild == null) throw new InvalidState ();
                if (tag != tagChild) {
                    throw new TagMismatchException("Expected Tag " + tagChild.ToString());
                }

            /*
             *  We don't do either construted or indefinite lenght encoding for this type
             */

            if (fConstructed || (cbLength == -1)) {
                throw new MalformedEncodingException();
            }

            if (cbLength != 1) {
                throw new MalformedEncodingException ();
            }

            /*
             *  See if we have enougth data
             */

            if (stm.Length - stm.Current < cbLength + cbTL) {
                throw new NeedMoreDataException();
            }

            /*
             *  Skip over the tag and length
             */

            stm.Advance(cbTL);

            /*
             *  Allocate a buffer to read the result into
             */

            byte[] rgb;
            rgb = stm.Read(cbLength);

            //  Strict DER check - byte must be 0 or 0xff

            if ((flags & A2C_FLAGS.STRICT) != 0) {
                if ((rgb[0] != 0) && (rgb[0] != 0xff)) throw new MalformedEncodingException ("Boolean value fails strict check");
            }

            m_Value = rgb[0];
        }

        public static ASN Create()
        {
            return new Boolean();
        }
    }

    public class Boolean__Set : ObjectSet
    {
        public Boolean__Set () { }
        public Boolean__Set (Boolean[] rg) : base (rg) { }
    }
}
