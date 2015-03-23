using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class Enumerated : ASN
    {
        //
        //  The internal data may be stored in two different formats depending on
        //  the actual current value and what was last set.  It will either be an Enumerated,
        //  if it can be stored in the native format, or a length and byte array.
        //

        Int64 m_i64Value;

        readonly static Tag s_Tag = new Tag (TagClass.Universal, 2, TagType.Implicit);

        //
        //  Support various constructors that vary based on the passed in value.
        //  Note that all constructors clone the passed in value rather than just point to
        //  the passed in value
        //

        protected Enumerated()
        {
        }

        protected Enumerated(Enumerated src)
        {
            m_i64Value = src.m_i64Value;
        }

        protected Enumerated(Int64 val)
        {
            m_i64Value = val;
        }

        //
        //  Routines to get and set values
        //

        protected Int64 Value
        {
            get
            {
                return m_i64Value;
            }

            set
            {
                m_i64Value = value;
            }
        }

        //
        //  Routines that need to be over-ridden
        //

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tag, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            //
            //  Get the value we will be emitting
            //

            byte[] rgb = null;

            if (rgb == null) {
                rgb = BitConverter.GetBytes((Int64) m_i64Value);        // Will throw an exception of m_i64Value is not set.
                if (BitConverter.IsLittleEndian) Array.Reverse(rgb);
            }

            //
            //  Per class 8.3.2 - trim the output string as necessary.
            //

            if ((rgb.Length > 2) && (rgb[0] == 0xff) && ((rgb[1] & 0x80) == 0x80)) {
                int i;
                for (i = 0; i < rgb.Length - 2; i++) {
                    if ((rgb.Length > 2) && (rgb[0] == 0xff) && ((rgb[1] & 0x80) == 0x80)) {
                    }
                    else {
                        break;
                    }
                }
                if (i > 0) {
                    byte[] rgbT = new byte[rgb.Length - i];
                    for (int i1 = 0; i < rgb.Length; i++, i1++) rgbT[i1] = rgb[i];
                    rgb = rgbT;
                }
            }

            if ((rgb.Length > 2) && (rgb[0] == 0) && ((rgb[1] & 0x80) == 0)) {
                int i;
                for (i = 0; i < rgb.Length - 2; i++) {
                    if ((rgb.Length > 2) && (rgb[0] == 0) && ((rgb[1] & 0x80) == 0)) {
                    }
                    else {
                        break;
                    }
                }
                if (i > 0) {
                    byte[] rgbT = new byte[rgb.Length - i];
                    for (int i1 = 0; i < rgb.Length; i++, i1++) rgbT[i1] = rgb[i];
                    rgb = rgbT;
                }
            }

            //
            //  Either write out the tag that was passed in, or out normal default tag
            //

            if (tag == null) throw new InvalidState();
                stm.WriteTag(tag, false);

            //
            //  Write out the length of the data
            //

            stm.WriteLength(rgb.Length);

            //
            //  Write out the actual bytes
            //

            stm.WriteData(rgb);
        }

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tagsAll = Tag.Append (tagChild, s_Tag);

            _DecodeTags (flags, fDecodeAsDer, ctxt, tagsAll, stm);
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag tagChild, ParserStream stm)
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

            if (tagChild == null) throw new InvalidState();
            if (tag != tagChild) {
                throw new TagMismatchException ("Expected Tag [UNIVERSIAL 2]");
            }

            /*
             *  We don't do either construted or indefinite lenght encoding for this type
             */

            if (fConstructed || (cbLength == -1)) {
                throw new MalformedEncodingException();
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

            byte[] rgb = new byte[cbLength];
            if (cbLength > 8) throw new Exception("Internal Unsupported");
        }

        public static ASN Create()
        {
            return new Enumerated();
        }
    }
}
