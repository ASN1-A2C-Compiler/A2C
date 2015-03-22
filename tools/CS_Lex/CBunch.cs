using System;
using System.Collections;

namespace TUVienna.CS_Lex
{
	/// <summary>
	/// Summary description for CBunch.
	/// </summary>
    public class CBunch
    {
        /***************************************************************
          Member Variables
          **************************************************************/
        public Vector m_nfa_set; /* Vector of CNfa states in dfa state. */
       public  SparseBitSet m_nfa_bit; /* BitSet representation of CNfa labels. */
        public CAccept m_accept; /* Accepting actions, or null if nonaccepting state. */
        public int m_anchor; /* Anchors on regular expression. */
        public int m_accept_index; /* CNfa index corresponding to accepting actions. */

        /***************************************************************
          Function: CBunch
          Description: Constructor.
          **************************************************************/
        public CBunch
            (
            )
        {
            m_nfa_set = null;
            m_nfa_bit = null;
            m_accept = null;
            m_anchor = CSpec.NONE;
            m_accept_index = -1;
        }
    }

}
