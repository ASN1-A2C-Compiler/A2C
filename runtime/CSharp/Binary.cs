using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    class Binary : ASN
    {
        private class Frame : ContextFrame
        {
            public MemoryStream m_stm;
            public int m_cbLeft;

            public Frame (MemoryStream stm) { m_stm = stm; }
            public Frame (int cbLeft) { m_cbLeft = cbLeft; }

        }

        private byte[] m_rgb;

        protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)
        {
            _DecodeTags (flags, fDecodeAsDer, ctxt, tagChild, stm);
        }

        protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)
        {
            _EncodeTags (flags, fEncodeAsDer, cctxt, tag, stm);
        } 

        private void _DecodeBER (A2C_FLAGS flags, Context ctxt, Tag tagChild, ParserStream stmIn)
        {
            int iBits = 0;
            MemoryStream stmData;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                Frame frame = ctxt.Frames.Pop () as Binary.Frame;
                if (frame == null) throw new InvalidState ();

                stmData = frame.m_stm;

                if (ctxt.Frames.Count == 0) flags &= ~A2C_FLAGS.MORE_DATA;

            }
            else {
                stmData = new MemoryStream ();

                //  Setup for restart
            }

            try {

                RecurseGetLength (stmIn, stmData, flags, ctxt, null, false, ref iBits);

                m_rgb = stmData.data;
            }
            catch (NeedMoreDataException e) {
                e.context.Frames.Push(new Frame(stmData));
                throw e;
            }
        }

        internal override void _DecodePrimative (A2C_FLAGS flags, bool fDER, Context ctxt, Tag tagChild, ParserStream stm)
        {
            if (!fDER) {
                _DecodeBER (flags, ctxt, tagChild, stm);
                return;
            }

            Tag tagLocal;
            bool fConstructed;
            int cbData;
            int cbTL;

            stm.PeekTagAndLength (out tagLocal, out fConstructed, out cbData, out cbTL);

            if (stm.Length < (cbTL + cbData)) throw new NeedMoreDataException();

            m_rgb = stm.Read(cbTL + cbData);
        }

        internal override void _EncodePrimative (A2C_FLAGS flags, bool fDER, Context cctxt, Tag tag, Stream stm)
        {
            if (tag != null) throw new InvalidState ();
            stm.WriteData (m_rgb);
        }

        static public void RecurseGetLength(ParserStream stmIn, MemoryStream stmOut, A2C_FLAGS flags, Context ctxt, Tag tagIn, bool fBitString, ref int iBits)
        {
                Tag tagLocal;
                bool fConstructed;
                int cbTL;
                int cbData;
                bool fSkipTagCheck = false;

            if ((flags & A2C_FLAGS.MORE_DATA) != 0) {
                Frame frame = ctxt.Frames.Pop () as Binary.Frame;
                if (frame == null) throw new InvalidState ();

                cbData = frame.m_cbLeft;

                if (ctxt.Frames.Count == 0) {
                    flags &= ~A2C_FLAGS.MORE_DATA;
                }
                else fSkipTagCheck = true;
            }
            else {

                //  Find the type and length of this value

                stmIn.PeekTagAndLength(out tagLocal, out fConstructed, out cbData, out cbTL);

                //
                //  If this is definitive length and
                //      either 1) we are keeping all of the tags or
                //              2) it is not constructed
                //
                //  we can just copy everything over with out problems
                //

                if ((cbData != -1) && ((tagIn == null) || !fConstructed)) {
                    //
                    //  if not enough data left - return that
                    //

                    if (stmIn.Length< cbTL + cbData) throw new NeedMoreDataException();

                    if (tagIn == null) {
                        stmOut.WriteData(stmIn.Read(cbTL + cbData));
                    }
                    else {
                        stmIn.Advance(cbTL);

                        if (fBitString) {
                            if (iBits != 0) throw new MalformedEncodingException();
                            if (cbData == 0) throw new MalformedEncodingException();

                            byte[] rgb1 = stmIn.Read(1);
                            cbData -= 1;

                            iBits = rgb1[0];
                        }

                        stmOut.WriteData(stmIn.Read(cbData));
                    }
                    return;
                }

                //  Copy over tag and length

                if (tagIn == null) {
                    stmOut.WriteData(stmIn.Read(cbTL));
                }
                else {
                    stmIn.Advance(cbTL);
                }

                //  Setup for restart operatoin
            }

            //  Loop until finished

            while (cbData != 0) {
                int cbData2;
                int cbOffset;

                try {
                    if (!fSkipTagCheck) {
                        //  Ad the end of the loop?

                        stmIn.PeekTagAndLength (out tagLocal, out fConstructed, out cbData2, out cbTL);

                        //  Look for end of object marker

                        if ((tagLocal.Class == TagClass.Universal) && (tagLocal.Value == 0)) {
                            if (tagIn == null) {
                                stmOut.WriteData (stmIn.Read (cbTL));
                            }
                            else {
                                stmIn.Advance (cbTL);
                            }
                            break;
                        }
                    }
                    else fSkipTagCheck = false;

                    // Grab the current pointer

                    cbOffset = stmIn.Current;

                    RecurseGetLength (stmIn, stmOut, flags, ctxt, tagIn, fBitString, ref iBits);

                    //  Adjust the # of bytes consumed as needed

                    if (cbData != -1) {
                        cbData -= (stmIn.Current - cbOffset);
                    }
                }
                catch (NeedMoreDataException e) {
                    e.context.Frames.Push (new Frame (cbData));
                    throw e;
                }
            }
        }
    }
}
