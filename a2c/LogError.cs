using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace asn_compile_cs
{
    class LogError
    {
        static public void Error(String str1, String str2)
        {
            return;
        }

        static public void ICE()
        {
            Debug.Assert(false, "ICE");
            Environment.Exit(-1);
        }
    }
}
