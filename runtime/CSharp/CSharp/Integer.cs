using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class Integer : ASN
    {
        //
        //  The internal data may be stored in two different formats depending on
        //  the actual current value and what was last set.  It will either be an integer,
        //  if it can be stored in the native format, or a length and byte array.
        //

        Int64? m_i64Value;
        byte[] m_rgbValue;
        static readonly Tag s_Tag = new Tag (TagClass.Universal, 2, TagType.Implicit);

        //
        //  Support various constructors that vary based on the passed in value.
        //  Note that all constructors clone the passed in value rather than just point to
        //  the passed in value
        //

        public Integer()
        {
        }

        public Integer(Integer src)
        {
            m_i64Value = src.m_i64Value;
            if (src.m_rgbValue != null) {
                m_rgbValue = (byte[])src.m_rgbValue.Clone();
            }
        }

        public Integer(Int64 val)
        {
            m_i64Value = val;
        }

        public Integer(byte[] val)
        {
            m_rgbValue = (byte[])val.Clone();
        }

        //
        //  Routines to get and set values
        //

        public Int64 Value
        {
            get
            {
                if (m_i64Value != null) return (Int64) m_i64Value;
                if (m_rgbValue == null) {
                    throw new InvalidOperationException();
                }

                if (m_rgbValue.Length > 8) {
                    throw new ValueOutOfRangeException("fred");
                }

                byte[] rgb;

                if (m_rgbValue.Length == 8) {
                    rgb = (byte[]) m_rgbValue.Clone();
                }
                else {
                    if ((m_rgbValue[0] & 0x80) != 0) {
                        rgb = new byte[8] { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
                    }
                    else {
                        rgb = new byte[8];
                    }
                    m_rgbValue.CopyTo (rgb, 8 - m_rgbValue.Length);
                }

                if (BitConverter.IsLittleEndian) Array.Reverse(rgb);

                m_i64Value = BitConverter.ToInt64(rgb, 0);

                return (Int64) m_i64Value;


#if false
                   // If the system architecture is little-endian (that is, little end first),
// reverse the byte array.
if (BitConverter.IsLittleEndian)
    Array.Reverse(bytes);

int i = BitConverter.ToInt32(bytes, 0);
Console.WriteLine("int: {0}", i);
// Output: int: 25


                   byte[] bytes = BitConverter.GetBytes(201805978);
Console.WriteLine("byte array: " + BitConverter.ToString(bytes));
// Output: byte array: 9A-50-07-0C

#endif
            }

            set
            {
                m_rgbValue = null;
                m_i64Value = value;
            }
        }

        public byte[] ValueBytes
        {
            get
            {
                if (m_rgbValue != null) return m_rgbValue;
                if (m_i64Value == null) {
                    throw new InvalidOperationException();
                }

                byte[] rgbTmp = BitConverter.GetBytes((Int64) m_i64Value);
                if (!BitConverter.IsLittleEndian) Array.Reverse(rgbTmp);
                return rgbTmp;
            }
            set { m_rgbValue = value; m_i64Value = null; }
        }

        //  Comparison Routines

        public override bool Equals (object obj)
        {
            A2C.Integer rhs = obj as A2C.Integer;
            if (rhs == null) return false;

            // if (!base.Equals (obj)) return false;

            if ((m_i64Value != null) && (rhs.m_i64Value != null)) return m_i64Value == rhs.m_i64Value;
            
            byte[] rhsBytes = rhs.ValueBytes;
            byte[] lhsBytes = ValueBytes;

            if (m_rgbValue.Length != rhsBytes.Length) return false;

            for (int i = 0; i < m_rgbValue.Length; i++) {
                if (m_rgbValue[i] != rhsBytes[i]) return false;
            }

            return true;
            
        }

        public override int GetHashCode ()
        {
            if (m_i64Value != null) return (int) (m_i64Value & 0xffffffff);
            
            int iMax = 4;
            if (m_rgbValue.Length < iMax) iMax = m_rgbValue.Length;

            int iHash = 0;
            for (int i = 0; i < iMax; i++) {
                iHash = iHash * 256 + m_rgbValue[i];
            }

            return iHash;
        }

        //
        //  Routines that need to be over-ridden
        //

        protected override void _Print (int iDepth, TextStream stm)
        {
            if (m_i64Value != null) {
                stm.Write (String.Format("{0} {0:x}", m_i64Value));
                return;
            }

            stm.Write ("lONG INTEGER - NOT IMPLEMENTED");
        }

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

            byte[] rgb = m_rgbValue;

            if (rgb == null) {
                rgb = BitConverter.GetBytes((Int64) m_i64Value);        // Will throw an exception of m_i64Value is not set.
                if (BitConverter.IsLittleEndian) Array.Reverse(rgb);
            }

            //
            //  Per class 8.3.2 - trim the output string as necessary.
            //

            if ((rgb.Length > 2) && (rgb[0] == 0xff) && ((rgb[1] & 0x80) == 0x80)) {
                int i = 0;
                while ((rgb.Length - i >= 2) && (rgb[i] == 0xff) && ((rgb[i + 1] & 0x80) == 0x80)) {
                    i += 1;
                }
                if (i > 0) {
                    byte[] rgbT = new byte[rgb.Length - i];
                    for (int i1 = 0; i < rgb.Length; i++, i1++) rgbT[i1] = rgb[i];
                    rgb = rgbT;
                }
            }

            if ((rgb.Length > 2) && (rgb[0] == 0) && ((rgb[1] & 0x80) == 0)) {
                int i = 0;
                while ((rgb.Length - i >= 2) && (rgb[i] == 0) && ((rgb[i + 1] & 0x80) == 0)) {
                    i += 1;
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

            if (tag == null) {
                stm.WriteTag(s_Tag, false);
            }
            else {
                stm.WriteTag(tag, false);
            }

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

            if (tagChild != null) {
                if (tag != tagChild) {
                    throw new TagMismatchException("Expected Tag " + tagChild.ToString());
                }
            }
            else {
                if (tag != s_Tag) {
                    throw new TagMismatchException("Expected Tag [UNIVERSIAL 2]");
                }
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
            m_rgbValue = stm.Read(cbLength);
        }

        public static ASN Create()
        {
            return new Integer();
        }
    }
}
