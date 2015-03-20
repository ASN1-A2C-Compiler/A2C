using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace asn_compile_cs
{
    enum ConstraintType
    {
        //  Elements with no children
        Value_Min = 1, Value_Max, Extension, UserDefined, Contents,
        //  Leaf Elements to build a constraint from
        Value, LB_Value, UB_Value, Table, Contained,
        //  Constraints which have a single child
        Size, From, With,
        //  Constraints which have two children
        Range, Except, Relation,
        //  Constraints which have multiple children
        Intersection, Union, List
    }

    class Constraint
    {
        ConstraintType m_ct;
        Value m_val;
        List<Constraint> m_list;
        SymbolList m_symlst;
        String m_str;
        Type m_type;                // Type of the embedded object for Contents constraint.

        /// <summary>
        /// Contraint constructor - Private
        /// </summary>
        Constraint()
        {
        }

        public Constraint(ConstraintType ct)
        {
            m_ct = ct;
        }

        public Constraint(ConstraintType ct, Value val)
        {
            Debug.Assert((ct == ConstraintType.Value) || (ct == ConstraintType.LB_Value) || (ct == ConstraintType.UB_Value) ||
                (ct == ConstraintType.Value_Max) || (ct == ConstraintType.Table) || (ct == ConstraintType.Relation));
            m_ct = ct;
            m_val = val;
        }

        public Constraint(ConstraintType ct, Constraint val1)
        {
            Debug.Assert((ct == ConstraintType.Size) || (ct == ConstraintType.Intersection) ||
                         (ct == ConstraintType.Union) || (ct == ConstraintType.From));
            m_ct = ct;
            m_list = new List<Constraint>();
            m_list.Add(val1);
        }

        public Constraint(ConstraintType ct, Constraint val1, Constraint val2)
        {
            Debug.Assert((ct == ConstraintType.Range) || (ct == ConstraintType.Union));
            m_ct = ct;
            m_list = new List<Constraint>();
            m_list.Add(val1);
            m_list.Add(val2);
        }

        public Constraint(ConstraintType ct, List<Constraint> lst)
        {
            Debug.Assert((ct == ConstraintType.Intersection) || (ct == ConstraintType.Union));
            m_ct = ct;
            m_list = lst;
        }

        //
        //  Create a contents constraint - contents constraint have
        //  type - the type of the field that is contained within this object
        //  valEncoding - an OID representing the encoding type - MUST be an OID
        //
        public Constraint(ConstraintType ct, Type type, Value valEncoding)
        {
            Debug.Assert(ct == ConstraintType.Contents);
            m_ct = ct;
            m_type = type;
            m_val = valEncoding;
        }

        //

        public Type type
        {
            get { return m_type; }
        }

        public SymbolList symlst
        {
            get { return m_symlst; }
            set { m_symlst = value; }
        }

        public ConstraintType ct
        {
            get { return m_ct; }
        }

        public String text
        {
            get { return m_str; }
            set { m_str = value; }
        }

        public Value value
        {
            get { return m_val; }
            set { m_val = value; }
        }

        //

        public void Add(Constraint val)
        {
            if (m_list == null) m_list = new List<Constraint>();
            m_list.Add(val);
        }

        public void ChangeType(ConstraintType ct)
        {
            m_ct = ct;
        }

        public void Collapse()
        {
            if (m_val != null) m_val.Collapse();
            if (m_list != null) {
                foreach (Constraint cst in m_list) cst.Collapse();
            }
        }

        public void Dump(String tab)
        {
            Console.Error.WriteLine(tab + m_ct.ToString());
            if (m_val != null) m_val.Dump(tab + "    ");
            if (m_list != null) {
                foreach (Constraint cst in m_list) cst.Dump(tab + "    ");
            }
            if (m_symlst != null) {
                foreach (Symbol sym in m_symlst) sym.Dump(tab + "    ");
            }
        }

        /// <summary>
        /// Return a new constraint object with the parameters replaced
        /// </summary>
        /// <param name="paramList">List of parameters to replace</param>
        /// <returns>new constraint object</returns>
        public Constraint Instance(SymbolList paramList)
        {
            Constraint newCst = new Constraint();

            newCst.m_ct = m_ct;
            if (m_list != null) {
                newCst.m_list = new List<Constraint>();
                foreach (Constraint cst in m_list) {
                    newCst.m_list.Add(cst.Instance(paramList));
                }
            }
            if (m_val != null) {
                newCst.m_val = m_val.Instance(paramList);
            }
            if (m_symlst != null) {
                newCst.m_symlst = new SymbolList();
                foreach (Symbol sym in m_symlst) {
                    newCst.m_symlst.Add(sym.Instance(paramList));
                }
            }
            if (m_type != null) {
                newCst.m_type = m_type.Instance(paramList);
            }

            newCst.m_str = m_str;
            return newCst;
        }

        /// <summary>
        /// Size computes the number of items required to hold the permitted set of values
        /// </summary>
        /// <returns>Count of items or -1 if the number of items is unbounded</returns>
        public int Size()
        {
            //
            //  Handle each of the different items
            //
            switch (m_ct) {
            //
            //  A size constraint is the easiest.  It actually gives the count of items
            //      for the minimum and maximum size of the field.  We can simply grab the
            //      maximum value and go home
            //

            case ConstraintType.Size:
                Debug.Assert(m_list.Count == 1);
                return 0; // m00bug

            default:
                Debug.Assert(false);
                return -1;
            }
        }

        public Value ToValue()
        {
            ValueList vlst = this.ToValueList();

            Debug.Assert(vlst.Count > 0);

            Value val;

            val = new Value(vlst, ValueType.SetOf);

            if (val.valueList.Count > 0) {
                val.type = val.valueList[0].type;
            }

            return val;
        }


        public ValueList ToValueList()
        {
            ValueList vlst = new ValueList();
            ValueList vlst2;

            switch (m_ct) {
            case ConstraintType.Union:
                foreach (Constraint cst in m_list) {
                    vlst2 = cst.ToValueList();
                    // M00BUG Check for uniqunes on the add operation
                    vlst.Add(vlst2);
                }
                break;

            case ConstraintType.Value:
                vlst.Add(m_val);
                break;

            default:
                Debug.Assert(false, "NYI");
                break;
            }

            return vlst;
        }
    }

    class ConstraintList : IEnumerable<Constraint>
    {
        List<Constraint> m_lst;


        public ConstraintList()
        {
            m_lst = new List<Constraint>();
        }

        public ConstraintList(Constraint sym)
        {
            m_lst = new List<Constraint>();
            m_lst.Add(sym);
        }

        public ConstraintList(List<Constraint> lst)
        {
            m_lst = lst;
        }

        public ConstraintList(ConstraintList lst)
        {
            m_lst = new List<Constraint>();
            foreach (Constraint cst in lst.m_lst) {
                m_lst.Add(cst);
            }
        }

        public Constraint this[int i]
        {
            get { return m_lst[i]; }
        }

        public int Count { get { return m_lst.Count; } }

        public List<Constraint> list
        {
            get { return m_lst; }
        }

        public void Add(Constraint sym)
        {
            m_lst.Add(sym);
        }

        //

        public Constraint BuildConstraintFromList()
        {
            Constraint cstExcept = null;
            Constraint cstIntersect = null;
            Constraint cstValue = null;
            Constraint cstUnion = null;

            //
            //  Walk the list of all elements and then build up the final constraint that is to be returned
            //  by the entire routine.  We walk the list from left to right and impose the needed
            //  precedence rules now.
            //  

            foreach (Constraint cst in m_lst) {
                switch (cst.ct) {
                case ConstraintType.Contained:
                case ConstraintType.Range:
                case ConstraintType.Size:
                case ConstraintType.Value:
                case ConstraintType.With:
                    //
                    //  We have a value - save it for the next item to come along.
                    //  at this point we should NEVER have another value sitting on 
                    //  the hot spot
                    //

                    Debug.Assert(cstValue == null);
                    cstValue = cst;
                    break;

                case ConstraintType.Except:
                    //
                    //  We should always have a lhs at this point.
                    //

                    Debug.Assert(cstValue != null);

                    //
                    //  If we don't have an except list -- create one
                    //  If we do have one - then complain
                    //

                    if (cstExcept != null) {
                        //  Generate an error to be returned -- M00BUG
                    }
                    else {
                        cstExcept = new Constraint(ConstraintType.Except);
                    }

                    cstExcept.Add(cstValue);
                    cstValue = null;
                    break;

                case ConstraintType.Intersection:
                    //
                    //  We should always have a lhs at this point.
                    //

                    Debug.Assert(cstValue != null);

                    //
                    //  If we don't have a intersection list - create one
                    //

                    if (cstIntersect == null) cstIntersect = new Constraint(ConstraintType.Intersection);

                    if (cstExcept != null) {
                        cstExcept.Add(cstValue);
                        cstValue = cstExcept;
                        cstExcept = null;
                    }

                    //
                    //  Add the current value to the constraint
                    //

                    cstIntersect.Add(cstValue);
                    cstValue = null;
                    break;


                case ConstraintType.Union:
                    //
                    //  We should always have a lhs at this point.
                    //

                    Debug.Assert(cstValue != null);

                    //
                    //  If we don't have a union list - create one
                    //

                    if (cstUnion == null) cstUnion = new Constraint(ConstraintType.Union);

                    if (cstExcept != null) {
                        cstExcept.Add(cstValue);
                        cstValue = cstExcept;
                        cstExcept = null;
                    }

                    if (cstIntersect != null) {
                        cstIntersect.Add(cstValue);
                        cstValue = cstIntersect;
                        cstIntersect = null;
                    }

                    //
                    //  Add the current value to the constraint
                    //

                    cstUnion.Add(cstValue);
                    cstValue = null;
                    break;

                default:
                    Debug.Assert(false, "Need to deal with intersection and except when doing constraint processing " + cst.ct);

                    //
                    //  We have a value - save it for the next item to come along.
                    //  at this point we should NEVER have another value sitting on 
                    //  the hot spot
                    //

                    Debug.Assert(cstValue == null);
                    cstValue = cst;
                    break;
                }
            }

            //
            //  When we finish - we should always have a value
            //

            Debug.Assert(cstValue != null);

            //
            //  Is there an open except clause?
            //
            //  If yes - then set the value onto the except clause.
            //

            if (cstExcept != null) {
                cstExcept.Add(cstValue);
                cstValue = cstExcept;
            }

            //
            //  Is there an open Intersection clause?
            //  If yes - then set the value onto the intersection clause.
            //

            if (cstIntersect != null) {
                cstIntersect.Add(cstValue);
                cstValue = cstIntersect;
            }

            //
            //  If we have a union - add it to the union
            //

            if (cstUnion != null) {
                cstUnion.Add(cstValue);
                cstValue = cstUnion;
            }

            //
            //  return the value
            //

            return cstValue;
        }

        public void Insert(int i, Constraint cst)
        {
            m_lst.Insert(i, cst);
        }

        public void AddRange(ConstraintList lst)
        {
            m_lst.AddRange(lst.m_lst);
        }

        public void Dump(String tab)
        {
            foreach (Constraint sym in m_lst) sym.Dump(tab);
        }

        public IEnumerator<Constraint> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }
    }
}
