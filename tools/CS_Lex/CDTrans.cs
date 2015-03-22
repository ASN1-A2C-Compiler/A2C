using System;

namespace TUVienna.CS_Lex
{
	/// <summary>
	/// Summary description for CDTrans.
	/// </summary>
    /***************************************************************
      Class: CDTrans
      **************************************************************/
    public class CDTrans
    {
        /*************************************************************
          Member Variables
          ***********************************************************/
        public int[] m_dtrans;
        public CAccept m_accept;
        public int m_anchor;
        public int m_label;

        /*************************************************************
          Constants
          ***********************************************************/
        public const int F = -1;

        /*************************************************************
          Function: CTrans
          ***********************************************************/
        public CDTrans
            (
            int label,
            CSpec spec
            )
        {
            m_dtrans = new int[spec.m_dtrans_ncols];
            m_accept = null;
            m_anchor = CSpec.NONE;
            m_label = label;
        }
    }

}
