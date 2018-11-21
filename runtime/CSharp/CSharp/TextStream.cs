using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public abstract class TextStream
    {
            abstract public void Write (String str);
    }

    public class MemoryTextStream : TextStream
    {
                char[] m_rgch;
        int m_ib;

        public MemoryTextStream()
        {
            m_rgch = new char[128];
            m_ib = 0;
        }

        public MemoryTextStream(int cb)
        {
            m_rgch = new char[cb];
            m_ib = 0;
        }

        public override void Write(String str)
        {
            if (m_ib + str.Length > m_rgch.Length) {
                Array.Resize(ref m_rgch, m_ib + m_rgch.Length + 256);
            }

            str.ToCharArray().CopyTo(m_rgch, m_ib);

            m_ib += str.Length;
        }

        public void Clear()
        {
            m_ib = 0;
        }

        public char[] data
        {
            get
            {
                char[] rgbT = (char[]) m_rgch.Clone();
                Array.Resize(ref rgbT, m_ib);
                return rgbT;
            }
        }

        public String Text
        {
            get { return new String( m_rgch); }
        }

        public int Length
        {
            get { return m_ib; }
        }

    }
}
