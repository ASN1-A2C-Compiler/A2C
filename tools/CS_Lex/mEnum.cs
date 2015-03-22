using System;
using System.Collections;

namespace TUVienna.CS_Lex
{
    /// <summary>
    /// Summary description for mEnum.
    /// </summary>
    public class mEnum: IEnumerator
    {
        int pos=-1;
        SparseBitSet mSet;

        public mEnum(SparseBitSet parent)
        {
            mSet = parent;
        }
        public  bool MoveNext()
        {
            pos++;
            while (pos<mSet.size()&& !mSet.Get(pos))
                   pos++;
            return pos<mSet.size();
        }
        public    object Current
        {
            get
            {
                return pos;
            }
        }
        public void Reset()
        {
            pos=-1;
        }
    }

}
