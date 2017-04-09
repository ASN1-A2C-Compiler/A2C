using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    abstract public class Stream
    {
        abstract public void WriteData (byte[] rgb);

        public void WriteLength (int cb)
        {
            byte[] rgb;

            //
            //  Is this an indefinite length encode?
            //

            if (cb == -1) {
                rgb = new byte[] { 0x80 };
                WriteData (rgb);
                return;
            }

            //
            //  Can be simple if small enough
            //

            if (cb < 0x80) {
                rgb = new byte[] { (byte) cb };
                WriteData (rgb);
                return;
            }

            //
            //  Now lets look at long items.
            //
            //  Compute the length needed.
            //

            int cbT = cb;
            int i;

            for (i = 1; cbT != 0; i++, cbT = cbT >> 8) ;

            rgb = new byte[i];
            rgb[0] = (byte) (0x80 + i - 1);
            for (i--, cbT = cb; cbT != 0; i--) {
                rgb[i] = (byte) (cbT & 0xff);
                cbT = cbT >> 8;
            }

            WriteData(rgb);
        }

        public void WriteTag(Tag tag, bool fConstructed)
        {
            WriteData (tag.ToBytes (fConstructed));
        }
    }


    public class MemoryStream : Stream
    {
        byte[] m_rgb;
        int m_ib;

        public MemoryStream ()
        {
            m_rgb = new byte[128];
            m_ib = 0;
        }

        public MemoryStream (int cb)
        {
            m_rgb = new byte[cb];
            m_ib = 0;
        }

        public override void WriteData (byte[] rgb)
        {
            if (m_ib + rgb.Length > m_rgb.Length) {
                Array.Resize (ref m_rgb, m_ib + rgb.Length + 256);
            }

            rgb.CopyTo (m_rgb, m_ib);

            m_ib += rgb.Length;
        }

        public void Clear ()
        {
            m_ib = 0;
        }

        public byte[] data
        {
            get
            {
                byte[] rgbT = (byte[]) m_rgb.Clone ();
                Array.Resize (ref rgbT, m_ib);
                return rgbT;
            }
        }

        public int Length
        {
            get { return m_ib; }
        }
    }

    public class ParserStream
    {
        byte[] m_rgbData;
        int m_ibCurrent;

        public ParserStream (byte[] rgb)
        {
            m_ibCurrent = 0;
            m_rgbData = rgb;
        }

        public ParserStream (ParserStream stmIn, int cbIn)
        {
            m_rgbData = new byte[cbIn];
            Array.Copy (stmIn.m_rgbData, stmIn.m_ibCurrent, m_rgbData, 0, cbIn);
        }

        public int Length
        {
            get { return m_rgbData.Length; }
        }

        public int Current
        {
            get { return m_ibCurrent; }
        }

        public void Advance (int cb)
        {
            if (m_ibCurrent + cb > m_rgbData.Length) {
                throw new IndexOutOfRangeException ();
            }
            m_ibCurrent += cb;
        }

        public void GetTagAndLength (out Tag tag, out bool fConstructed, out int cbData, out int cbTL)
        {
            PeekTagAndLength (out tag, out fConstructed, out cbData, out cbTL);
            // Advance (cbTL);
        }

        public void PeekTagAndLength (out Tag tag, out bool fConstructed, out int cbData, out int cbTL)
        {
            TagClass tagClass;
            int tagValue;

            int ib = m_ibCurrent;
            if (m_rgbData.Length == m_ibCurrent) {
                throw new NeedMoreDataException ("PeekTagAndLength");
            }

            //
            //  Grab the class to return
            //

            tagClass = (TagClass) (m_rgbData[ib] >> 6);

            //
            //  Grab the constructed flag
            //

            fConstructed = ((m_rgbData[ib] >> 5) & 0x1) != 0;

            //
            //  Is this a long or short form?
            //

            if ((m_rgbData[ib] & 0x1f) < 0x1f) {
                //
                //  Short form - set the tag value
                //

                tagValue = (m_rgbData[ib] & 0x1f);
            }
            else {
                Int32 tagT;

                //
                //  Long form - need to build out the actual tag value
                //

                tagT = 0;
                do {
                    ib += 1;
                    if (ib == m_rgbData.Length) {
                        throw new NeedMoreDataException ();
                    }

                    /*
                     *  Check for overflow
                     *
                     *  sizeof(tag)     Mask
                     *     1           0xfe<<0          0xfe
                     *     2           0xfe<<8          0xfe00
                     *     4           0xfe<<24         0xfe000000
                     */

                    if ((tagT & (0x00fe << (4 /* sizeof(tagT) */ * 8 - 8))) != 0) {
                        throw new OverflowException ();
                    }

                    tagT <<= 7;
                    tagT += (m_rgbData[ib]) & 0x7f;
                } while (((m_rgbData[ib]) & 0x80) != 0);

                tagValue = tagT;
            }

            ib++;
            if (ib == m_rgbData.Length) {
                throw new NeedMoreDataException ();
            }

            //
            //  Check if this is a long or short version
            //

            if ((m_rgbData[ib] & 0x80) != 0) {
                byte b;
                byte[] rgb = new byte[4];

                if (m_rgbData[ib] == 0x80) {
                    cbData = -1;
                    ib++;
                }
                else {

                    /*
                     *  Long version
                     */

                    b = (byte) ((m_rgbData[ib]) & 0x7f);

                    if (b > sizeof (int)) {
                        throw new OverflowException ("Data length is more than 4 bytes");
                    }

                    ib++;
                    if (ib + b >= m_rgbData.Length) {
                        throw new NeedMoreDataException ();
                    }

                    Array.Copy(m_rgbData, ib, rgb, 4-b, b);
                  //  m_rgbData.CopyTo (rgb, ib, b);

                    if (BitConverter.IsLittleEndian) Array.Reverse (rgb);
                    cbData = BitConverter.ToInt32(rgb, 0);

                    ib += b;
                }
            }
            else {
                /*
                 *  Short Version
                 */

                cbData = m_rgbData[ib];
                ib++;
            }

            cbTL = ib - m_ibCurrent;

            tag = new Tag (tagClass, tagValue, TagType.Implicit);

        }

        public byte[] Read (int cbRead)
        {
            byte[] rgb = new byte[cbRead];

            for (int i = 0; i < cbRead; i++) {
                rgb[i] = m_rgbData[m_ibCurrent + i];
            }

            m_ibCurrent += cbRead;

            return rgb;
        }
    }

}
