using System;

namespace TUVienna.CS_Lex
{
	/// <summary>
	/// Summary description for CNfaPair.
	/// </summary>
    /***************************************************************
      Class: CNfaPair
      **************************************************************/
    public class CNfaPair
    {
        /***************************************************************
          Member Variables
          **************************************************************/
        public CNfa m_start;
        public CNfa m_end;
  
        /***************************************************************
          Function: CNfaPair
          **************************************************************/
        public CNfaPair
            (
            )
        {
            m_start = null;
            m_end = null;
        }
    }

}
