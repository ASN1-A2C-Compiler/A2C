using System;
using System.Collections;

namespace TUVienna.CS_Lex
{
	/// <summary>
	/// Summary description for CDfa.
	/// </summary>
    /***************************************************************
  Class: CDfa
  **************************************************************/
    public class CDfa 
    {
        /***************************************************************
          Member Variables
          ***********************************************************/
        int m_group;
       public  bool m_mark;
       public  CAccept m_accept;
        public int m_anchor;
        public Vector m_nfa_set;
        public SparseBitSet m_nfa_bit;
       public  int m_label;

        /***************************************************************
          Function: CDfa
          **************************************************************/
       public  CDfa
            (
            int label
            )
        {
            m_group = 0;
            m_mark = false;

            m_accept = null;
            m_anchor = CSpec.NONE;

            m_nfa_set = null;
            m_nfa_bit = null;

            m_label = label;
        }
    }

}
