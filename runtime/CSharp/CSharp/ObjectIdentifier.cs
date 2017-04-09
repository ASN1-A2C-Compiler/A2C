using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class ObjectIdentifier : ASN
    {
        static readonly Tag s_Tag = new Tag (TagClass.Universal, 6, TagType.Implicit);
        private string m_Oid;

        public ObjectIdentifier () { m_Oid = null; }
        public ObjectIdentifier (string oidValue) { m_Oid = oidValue; }
        public ObjectIdentifier (ObjectIdentifier val) { m_Oid = val.m_Oid; }

        public String Value { get { return m_Oid; } set { m_Oid = value; } }

        static public ObjectIdentifier Create () { return new ObjectIdentifier (); }

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

            stm.PeekTagAndLength(out tag, out fConstructed, out cbLength, out cbTL);

            if (tagChild != null) {
                if (tag != tagChild) throw new TagMismatchException("Expected Tag " + tagChild.ToString());
            }
            else {
                if (tag != s_Tag) {
                    throw new TagMismatchException("Expected Tag [UNIVERSIAL OID]");
                }
            }

            //  We don't do either constructed or indefinite length encoding for this type

            if (fConstructed || (cbLength == -1)) throw new MalformedEncodingException();

            //  See if we have enough data

            if (stm.Length - stm.Current < cbLength + cbTL) throw new NeedMoreDataException();

            //  Skip over the tag and length

            stm.Advance(cbTL);

            //  Allocate a bufer and read in the encoded bytes

            byte[] rgb = new byte[cbLength];
            rgb = stm.Read(cbLength);

            //  Convert from byte array to int to string

            int iItem = 0;
            int ui = 0;
            int i;
            string sz = "";

            for (i = 0; i < cbLength; i++) {
                ui = (ui << 7) + (rgb[i] & 0x7f);
                if ((rgb[i] & 0x80) == 0) {
                    if (iItem == 0) {
                        sz += (ui / 40).ToString();
                        ui = ui % 40;
                        iItem += 1;
                    }
                    sz += ".";
                    sz += ui.ToString();

                    ui = 0;
                }
            }

            m_Oid = sz;
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tags, Stream stm)
        {
            Tag[] tagsAll = Tag.Append (tags, s_Tag);
            _EncodeTags (flags, fEncodeAsDer, cctxt, tagsAll, stm);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fEncodeAsDer, Context ctxt, Tag tag, Stream stm)
        {
            if (tag == null) throw new InvalidState ();

            stm.WriteTag (tag, false);

            string[] rgTags = m_Oid.Split ('.');
            Int64[] rgInts = new Int64[rgTags.Length];
            int i;
            int j;

            for (i = 0; i < rgTags.Length; i++) {
                rgInts[i] = Convert.ToInt64 (rgTags[i]);
            }

            if (rgInts.Length == 1) throw new Exception ("Invalid Object Identifier value");

            rgInts[1] = rgInts[0] * 40 + rgInts[1];

            byte[] rgb = new byte[6];
            List<byte> rgb2 = new List<byte>();

            for (i=1; i<rgInts.Length; i++) {
                for (j=5; (j>=0) && (rgInts[i] != 0); j--) {
                    rgb[j] = (byte) (rgInts[i] & 0x7f);
                    rgInts[i] >>= 7;
                    if (j != 5) rgb[j] |= 0x80;
                }

                if (rgInts[i] != 0) throw new Exception("Invalid OID value");

                for (j=j+1; j<6; j++) rgb2.Add(rgb[j]);
            }

            stm.WriteLength(rgb2.Count);

            stm.WriteData(rgb2.ToArray());
        }
    }
}
