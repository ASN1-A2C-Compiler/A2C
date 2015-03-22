using System;

namespace TUVienna.CS_Lex
{
	/// <summary>
	/// Summary description for CAccept.
	/// </summary>
    /***************************************************************
      Class: CAccept
     **************************************************************/
    public class CAccept
    {
        /***************************************************************
          Member Variables
          **************************************************************/
        public char[] m_action;
        public int m_action_read;
        public int m_line_number;

        /***************************************************************
          Function: CAccept
          **************************************************************/
        public CAccept
            (
            char[] action,
            int action_read,
            int line_number
            )
        {
            int elem;

            m_action_read = action_read;

            m_action = new char[m_action_read];
            for (elem = 0; elem < m_action_read; ++elem)
            {
                m_action[elem] = action[elem];
            }

            m_line_number = line_number;
        }

        /***************************************************************
          Function: CAccept
          **************************************************************/
       public  CAccept
            (
            CAccept accept
            )
        {
            int elem;

            m_action_read = accept.m_action_read;
	
            m_action = new char[m_action_read];
            for (elem = 0; elem < m_action_read; ++elem)
            {
                m_action[elem] = accept.m_action[elem];
            }

            m_line_number = accept.m_line_number;
        }

        /***************************************************************
          Function: mimic
          **************************************************************/
        public void mimic
            (
            CAccept accept
            )
        {
            int elem;

            m_action_read = accept.m_action_read;
	
            m_action = new char[m_action_read];
            for (elem = 0; elem < m_action_read; ++elem)
            {
                m_action[elem] = accept.m_action[elem];
            }
        }
    }

}
