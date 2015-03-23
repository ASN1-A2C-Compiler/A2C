using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    public class ASNType
    {
        readonly System.Type m_type;

        public ASNType (String strTypeName)
        {
            m_type =  System.Type.GetType (strTypeName, false);

        }

        public ASNType (System.Type type)
        {
            m_type = type;
        }

        public ASN Create ()
        {
            System.Reflection.ConstructorInfo f = m_type.GetConstructor (System.Type.EmptyTypes);

            return (ASN) f.Invoke (new Object[0]);
        }
    }
}
