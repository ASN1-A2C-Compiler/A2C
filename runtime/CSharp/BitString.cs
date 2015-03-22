using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class BitString : ASN
    {
        //  Data storage

        byte[] m_rgb;
        int m_cbits;
        static readonly Tag s_tagBitString = new Tag (TagClass.Universal, 3, TagType.Implicit);

        /// <summary>
        /// Create an empty bitstring object
        /// </summary>
        public BitString ()
        {
            m_cbits = 0;
            m_rgb = new byte[0]; ;
        }

        /// <summary>
        /// Create a bitstring object
        /// </summary>
        /// <param name="rgb">Initial set of bits to be set</param>
        public BitString (byte[] rgb)
        {
            m_rgb = new byte[rgb.Length];
            rgb.CopyTo(m_rgb, 0);
            m_cbits = m_rgb.Length * 8;
        }

        /// <summary>
        /// Create a bitstring object
        /// </summary>
        /// <param name="cbits">Inital count of bits to be allocated</param>
        public BitString (int cbits)
        {
            m_rgb = new byte[(cbits + 7) / 8];
            m_cbits = cbits;
        }

        /// <summary>
        /// Create a bitstring object
        /// </summary>
        /// <param name="rgb">Initial set of bits to be used</param>
        /// <param name="cbits">Count of bits to be used</param>
        public BitString (byte[] rgb, int cbits)
        {
            if ((cbits < (rgb.Length - 1) * 8) || (rgb.Length * 8 < cbits)) throw new ArgumentException ();
            m_rgb = new byte[rgb.Length];
            rgb.CopyTo (m_rgb, 0);
            m_cbits = cbits;
            
            //  Mask out top bits
            if (m_cbits % 8 != 0) {
                m_rgb[m_cbits / 8] &= (byte) (0xff & (0xff00 >> (m_cbits % 8)));
            }
        }

        /// <summary>
        /// Create a bitstring object
        /// </summary>
        /// <param name="val">Copy values of existing object</param>
        public BitString (BitString val)
        {
            m_rgb = new byte[val.m_rgb.Length];
            val.m_rgb.CopyTo(m_rgb, 0);
            m_cbits = val.m_cbits;
        }


        //  Accessor routines to get and set values

        public void ClearBit (int iBit) 
        {
            SetBit (iBit, false);
        }

        public bool GetBit (int iBit)
        {
            if (iBit <= 0) throw new ArgumentOutOfRangeException();
            if (iBit >= m_cbits) return false;

            return (m_rgb[iBit / 8] & (0x80 >> (iBit % 8))) != 0;
        }

        public void SetBit (int iBit) { SetBit (iBit, true); }

        void SetBit (int iBit, bool val)
        {
            if (iBit < 0)  throw new ArgumentOutOfRangeException ();

            //
            //  If the bit is to be set, and the array is not long enough - then extend it
            //

            if (val && (iBit / 8 >= m_rgb.Length)) {
                Array.Resize (ref m_rgb, (iBit / 8)+1);
            }

            if (val) {
                m_rgb[iBit / 8] |= (byte) (0x80 >> (iBit % 8));
                if (m_cbits <= iBit) m_cbits = iBit + 1;
            }
            else {
                m_rgb[iBit / 8] &= (byte) ((0xff7f >> (iBit % 8)) & 0xff);
            }
        }

        public bool TestBit (int iBit)
        {
            if (iBit < 0) throw new ArgumentOutOfRangeException ();
            if (iBit >= m_cbits) return false;

            return (m_rgb[iBit / 8] & (0x80 >> (iBit % 8))) != 0;
        }

        public byte[] Value { 
            get { return m_rgb; } 
            set {
                int len = m_rgb.Length * 8;
                m_rgb = value;
                if (!((len - 8 < m_cbits) && (m_cbits <= len))) m_cbits = len;
            }
        }
        public int BitsUsed { 
            get { return m_cbits; } 
            set {
                if (m_cbits == value) return;
                if (m_rgb.Length != (value + 7) / 8) {
                    Array.Resize (ref m_rgb, (value + 7) / 8);
                }
                byte mask = (byte) ((0xff00 >> (value % 8)) & 0xff);
                m_rgb[m_rgb.Length - 1] &= mask;
                m_cbits = value; 
            } 
        }

        public bool this[int iBit]
        {
            get { return GetBit (iBit); }
            set { SetBit (iBit, value); }
        }

        //  Creator function

        public static BitString Create () { return new BitString (); }

        //  Comparison Routines

        public override bool  Equals (object rhsObj)
        {
            BitString rhs = rhsObj as BitString;
            if (rhs == null) return false;

            if (!base.Equals (rhsObj)) return false;

            if (m_cbits != rhs.m_cbits) return false;
            for (int i = 0; i < m_rgb.Length; i++) {
                if (m_rgb[i] != rhs.m_rgb[i]) return false;
            }
            return true;
        }

        public override int GetHashCode ()
        {
            int hashValue = 0;
            for (int i = 0; i < m_rgb.Length; i++) {
                hashValue = hashValue * 10 + m_rgb[i];
            }
            return hashValue;
        }

        //  Print function

        protected override void _Print (int iDepth, TextStream stm)
        {
            stm.Write ("BITSTRING");
        }

        //  Encoder/Decoder routines

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            Tag[] tags = Tag.Append (tagChild, s_tagBitString);
            _DecodeTags (flags, fDecodeAsDer, ctxt, tags, stm);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            Tag[] tags = Tag.Append (tag, s_tagBitString);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tags, stm);
        }

        private void _DecodePrimativeBER (A2C_FLAGS flags, Context ctxt, Tag tagChild, ParserStream stm)
        {
            int cbLength;
            int cbTL;
            bool fConstructed;
            int iUnused = 0;
            Tag tagValue;
            MemoryStream stmDst;

            //
            //  Check if we are doing a restart
            //

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {

                //  Deal with context problems

                throw new NotImplementedException ();
            }
            else {
                //  Our first time here

                stm.PeekTagAndLength (out tagValue, out fConstructed, out cbLength, out cbTL);

                //
                //  Check that the tag matches the expected value
                //

                if (tagChild == null) tagChild = s_tagBitString;
                if (tagChild != tagValue) throw new A2C.TagMismatchException ("Tag mismatch for BitString");

                //
                //  Create the destination stream
                //

                stmDst = new MemoryStream ();

                //
                //  Setup the restart world
                //

                //
                //  Start with 0 unused bits in the last block
                //

                iUnused = 0;
            }

            Binary.RecurseGetLength (stm, stmDst, flags, ctxt, s_tagBitString, true, ref iUnused);

            //
            //  Grab the data from the stream and put it into the object
            //

            m_rgb = stmDst.data;
            m_cbits = m_rgb.Length * 8 - iUnused;
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDER, Context ctxt, Tag tagChild, ParserStream stm)
        {
            //
            //  If this is BER - then use the BER decoder
            //

            if (!fDER) {
                _DecodePrimativeBER (flags, ctxt, tagChild, stm);
                return;
            }

            //
            //  Decode as DER
            //

            int cbData;
            int cbTL;
            bool fConstructed;
            Tag tagLocal;

            //
            //  Grab the type and length of the next item.
            //  If not enough data exists, then no bytes will be consumed
            //

            stm.GetTagAndLength (out tagLocal, out fConstructed, out cbData, out cbTL);

            //  Check that the tag is correct.  If not then error

            if (tagChild == null) tagChild = s_tagBitString;
            if (tagChild != tagLocal) throw new A2C.MalformedEncodingException ("Tag mismatch decoding BitString");

            //  Can't be constructed and be a BitString

            if (fConstructed || (cbData == -1)) throw new A2C.MalformedEncodingException ("Malformed DER encoding");

            //
            //  Check to see if there are enough bytes in the stream to get the entire object
            //

            if (stm.Length < cbData) throw new A2C.NeedMoreDataException ();

            //
            //  Skip over the tag and length bytes
            //

            stm.Advance (cbTL);

            //
            //  Get the count of unused bits from the encoding
            //

            int cbitsUnused = stm.Read (1)[0];

            //
            //  Now get the bytes for the encoding
            //

            m_rgb = stm.Read (cbData - 1);

            //
            //  Turn unused bit count into total bits present
            //

            m_cbits = (cbData - 2) * 8 + (8 - cbitsUnused);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            byte b;
            byte cbitsUnused;
            int cbData;

            //
            //  Output is as follows:
            //  tag
            //  length
            //  # of unused bits in the last byte
            //  bit array
            //
            //  Figure out the number of unused bits and make the buffer as small as possible
            //

            cbData = 1 + (m_cbits + 7) / 8;
            cbitsUnused = (byte) (m_cbits % 8);

            //
            //  Mask the last byte so it has the correct number of bits showing
            //  DER says
            //    if there are unused bits - then 

            if ((cbData > 1) && (cbitsUnused > 0)) {
                b = m_rgb[cbData - 2];
                b &= (byte) (0xff00 >> cbitsUnused);
                cbitsUnused = (byte) (8 - cbitsUnused);
                m_rgb[cbData - 2] = b;
            }

            //
            //  Write out the tag - either the one provided or the default
            //

            if (tag == null) tag = s_tagBitString;
            stm.WriteTag (tag, false);

            //
            //  Write out the length
            //

            stm.WriteLength (cbData);

            //  Write out # of unused bits

            stm.WriteData (new byte[] { cbitsUnused });

            //  Write out the data bytes

            stm.WriteData (m_rgb);
        }
    }

    public class NamedBitString : BitString
    {
        public NamedBitString () { }
        public NamedBitString (byte[] rgb) : base (rgb) { }
        public NamedBitString (byte[] rgb, int cbits) : base (rgb, cbits) { }
        public NamedBitString (NamedBitString val) : base (val) { }

        protected override void _Encode (A2C_FLAGS flags, bool fDER, Context cctxt, Tag[] tags, Stream stm)
        {
            TrimValue ();
            base._Encode (flags, fDER, cctxt, tags, stm);
        }

        protected override void _Print (int iDepth, TextStream stm)
        {
            bool fFirst = true;

            for (int iBit = 0; iBit < BitsUsed; iBit++) {
                if (GetBit (iBit)) {
                    // M00BUG - get the names of the strings

                    if (!fFirst) {
                        stm.Write (" | ");
                        fFirst = false;
                    }

                    stm.Write (iBit.ToString ());
                }
            }
        }

        private void TrimValue ()
        {
            int iBit = BitsUsed;

            while ((iBit != 0) && !TestBit (iBit-1)) iBit -= 1;

            BitsUsed = iBit;
        }
    }
}
