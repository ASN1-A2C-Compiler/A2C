using System;
using System.Collections;

namespace TUVienna.CS_Lex
{
    /// <summary>
    /// Summary description for Vector.
    /// </summary>
    public class Vector
    {
        ArrayList parent;
        public Vector()
        {
            parent=new ArrayList();
            //
            // TODO: Add constructor logic here
            //
        }
        public Vector(ArrayList mPar)
        {
            parent=mPar;
        }
    
        public int size()
        {
            return parent.Count;
        }

        public int Count
        {
            get
            {
                return parent.Count;
            }
        }

        public object elementAt(int i)
        {
            return parent[i];
        }
        public IEnumerator elements()
        {
            return parent.GetEnumerator();
        }

        public int indexOf(object elem)
        {
            return parent.IndexOf(elem);
        }
        public void addElement(object elem)
        {
            parent.Add(elem);
        }

        public void removeElement(object elem)
        {
            parent.Remove(elem);
        }
        public void removeElementAt(int i)
        {
            parent.RemoveAt(i);
        }
        public void setElementAt(object obj,int pos)
        {
            parent[pos]=obj;
        }

        public bool contains(object elem)
        {
            return parent.Contains(elem);
        }

        public void setSize(int i)
        {
            if (parent.Count<=i)
            {
                parent.Capacity=i;
            }
            else
            {
                if (parent.Count>i)
                    parent.RemoveRange(i,parent.Count-i);
                parent.Capacity=i;
            }
        }

        public  object Clone()
        {
            return new Vector((ArrayList)parent.Clone());
        }

    }
}
