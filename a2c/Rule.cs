



using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace asn_compile_cs
{
   
    enum RuleType { Rule=1, Token, Body, Empty, Plus1, Plus2, Optional1, Symbol, String, Star1, Star2, Not, UScore, TokenList,
                    Choice0=100, Choice1, Choice2, Choice3, Choice4, Choice5, Choice6, Choice7, Choice8, Choice9, Choice10,
                        Choice11, Choice12, Choice13, Choice14, Choice15, Choice16, Choice17, Choice18, Choice19, Choice20, Choice21, ChoiceLast,
                    ExChoice0 = 150, ExChoice1, ExChoice2, ExChoice3, ExChoice4, ExChoice5, ExChoice6, ExChoice7, ExChoice8, ExChoice9, ExChoice10,
                        ExChoice11, ExChoice12, ExChoice13, ExChoice14, ExChoice15, ExChoice16, ExChoice17, ExChoice18, ExChoice19, ExChoice20, ExChoice21,
                        ExChoice22, ExChoice23, ExChoice24, ExChoice25, ExChoice26, ExChoice27, ExChoiceLast,
                    Action0 = 200, Action1, Action2, Action3, Action4, Action5, Action6, ActionFinal
                    }

    class RuleValue
    {
        Constraint m_constraint;
        ConstraintList m_constraintList;
        Symbol m_sym;
        Type m_type;
        Token m_token;
        FieldFlags m_fieldflags;
        SymbolList m_symbolList;
        RuleStep m_rs;
        Value m_val;
        ValueList m_valList;
        Parameter m_param;
        TokenList m_tknlst;
        String m_str;
        Error m_err;
        ErrorList m_errlst;
        ErrorList m_warnlst;
        AsnFile m_af;
        RuleStepList m_rsList;
        SymbolListList m_symListList;
        int m_iCount;

        //
        //  Constructors
        //

        public RuleValue() { }
        public RuleValue(AsnFile af) { m_af = af; }
        public RuleValue(Constraint constraint) { m_constraint = constraint; }
        public RuleValue(ConstraintList lst) { m_constraintList = lst; }
        public RuleValue(SymbolList lst) { m_symbolList = lst; }
        public RuleValue(FieldFlags flags) { m_fieldflags = flags; }
        public RuleValue(Parameter param) { m_param = param; }
        public RuleValue(RuleStep rs) { m_rs = rs; }
        public RuleValue(RuleStepList lst) { m_rsList = lst; }
        public RuleValue(String str) { m_str = str; }
        public RuleValue(Symbol sym)
        {
            m_sym = sym;
        }
        public RuleValue(SymbolListList lst) { m_symListList = lst; }

        public RuleValue(Token tkn) { m_token = tkn; }
        public RuleValue(TokenList lst) { m_tknlst = lst; }

        public RuleValue(Type typ)
        {
            m_type = typ;
        }

        public RuleValue(Value val)
        {
            m_val = val;
        }

        public RuleValue(ValueList lst)
        {
            m_valList = lst;
        }
        public RuleValue(Error err)
        {
            if (err.errno >= ErrorNumber.WarningBase) {
                m_warnlst = new ErrorList();
                m_warnlst.Add(err);
            }
            else {
                m_err = err;
            }
        }
        public RuleValue(ErrorList errlst) { m_errlst = errlst; }

        //
        //  Properties - there are no set routines because we never change
        //  the value of a RuleValue, we just build a new one.

        public AsnFile af { get { return m_af; }}
        public Constraint constraint { get { return m_constraint; }}
        public ConstraintList constraintList { get { return m_constraintList; } }
        public int tokenCount { get { return m_iCount; } set { m_iCount = value; } }
        public Error error { get { return m_err; } }
        public ErrorList errorList { get { return m_errlst; } }
        public SymbolList symList { get { return m_symbolList; } }
        public FieldFlags fieldFlags { get { return m_fieldflags; } }
        public Parameter param { get { return m_param; } }
        public SymbolListList paramlsts { get { return m_symListList; } set { m_symListList = value; } }
        public RuleStep rs
        {
            get { return m_rs; }
        }
        public RuleStepList rsList { get { return m_rsList; } }
        public String str { get { return m_str; } }
        public Symbol sym
        {
            get { return m_sym; }
        }
        public Type type
        {
            get { return m_type; }
        }
        public Token token { get { return m_token; } }
        public TokenList tokenList { get { return m_tknlst; } }
        public Value value
        {
            get { return m_val; }
        }
        public ValueList valueList
        {
            get { return m_valList; }
        }
        public ErrorList warnList
        {
            get { return m_warnlst; }
            set { m_warnlst = value; }
        }

        public void AddField(Symbol sym)
        {
            Debug.Assert(IsSymbol() || IsSymbolList());
            if (m_symbolList == null) {
                m_symbolList = new SymbolList();
                if (m_sym != null) {
                    m_symbolList.Add(m_sym);
                    m_sym = null;
                }
            }

            m_symbolList.Add(sym);
        }

        public void AddField(SymbolList lst)
        {
            Debug.Assert(IsSymbol() || IsSymbolList());
            if (m_symbolList == null) {
                Debug.Assert(m_sym != null);
                m_symbolList = new SymbolList();
                if (m_sym != null) {
                    m_symbolList.Add(m_sym);
                    m_sym = null;
                }
            }
            m_symbolList.AddRange(lst);
        }

        public void AddWarning(Error err)
        {
            if (m_warnlst == null) m_warnlst = new ErrorList();
            m_warnlst.Add(err);
        }
        public void AddWarning(ErrorList errs)
        {
            if (m_warnlst == null) m_warnlst = new ErrorList();
            m_warnlst.Add(errs);
        }

        public Boolean IsAsnFile() { return m_af != null; }
        public Boolean IsConstraint() { return m_constraint != null; }
        public Boolean IsConstraintList() { return m_constraintList != null; }
        public Boolean IsError() { return m_err != null; }
        public Boolean IsErrorList() { return m_errlst != null; }
        public Boolean IsField() { return (m_sym != null) && m_sym.IsType(SymbolType.Component); }
        public Boolean IsFieldList() { return (m_symbolList != null) && m_symbolList[0].IsType(SymbolType.Component); }

        public Boolean IsFieldFlags()
        {
            return m_fieldflags != null;
        }

        public Boolean IsParameter() { return m_param != null; }
        public Boolean IsParameterList() 
        { 
            return (m_symbolList != null) && 
                ((m_symbolList[0].GetSymbolType() == SymbolType.Dummy) ||
                 (m_symbolList[0].GetSymbolType() == SymbolType.Governor));
        }

        public Boolean IsRuleStep()
        {
            return m_rs != null;
        }
        public Boolean IsRuleStepList() { return m_rsList != null; }

        public Boolean IsString() { return m_str != null; }

        public Boolean IsSymbol()
        {
            return m_sym != null;
        }

        public Boolean IsSymbolList() { return m_symbolList != null; }

        public Boolean IsToken()
        {
            return m_token != null;
        }
        public Boolean IsTokenList() { return m_tknlst != null; }
        public Boolean IsType()
        {
            return m_type != null;
        }

        public  Boolean IsValue()
        {
            return m_val != null;
        }

        public Boolean IsValueList()
        {
            return m_valList != null;
        }
    }

    class RuleStep
    {
        RuleType m_rt;
        RuleNo m_iRule;
        TknType m_tknType;
        Symbol m_sym;
        String m_str;

        public RuleStep(RuleNo iRule)
        {
            m_rt = RuleType.Rule;
            m_iRule = iRule;
        }

        public RuleStep(TknType tknType)
        {
            m_rt = RuleType.Token;
            m_tknType = tknType;
        }

        public RuleStep(RuleType rt) { m_rt = rt; }

        public RuleStep(String str)
        {
            m_rt = RuleType.String;
            m_str = str;
        }

        public RuleStep(Symbol sym)
        {
            m_rt = RuleType.Symbol;
            m_sym = sym;
        }

        public RuleNo ruleNo { get { return m_iRule; } }
        public RuleType ruleType { get { return m_rt; } }
        public String str { get { return m_str; } }
        public Symbol sym { get { return m_sym; } }
        public TknType tknType { get { return m_tknType; } }

        public override bool Equals(object obj)
        {
            if (obj == null) return false;      // Can't be null

            // If parameter cannot be cast to Point return false.
            RuleStep rs = obj as RuleStep;
            if ((System.Object)rs == null) {
                return false;
            }

            if (m_rt != rs.m_rt) return false;
            switch (m_rt) {
            case RuleType.Token:
                return m_tknType == rs.m_tknType;

            }

            Debug.Assert(false);
            return false;
        }

        public Boolean IsChoice()
        {
            return ((RuleType.Choice0 <= m_rt) && (m_rt < RuleType.ChoiceLast)) || ((RuleType.ExChoice0 <= m_rt) && (m_rt < RuleType.ExChoiceLast));
        }
        public override string ToString()
        {
            String str;

            switch (m_rt) {
            case RuleType.Rule: str = m_rt + " r: " + m_iRule; break;
            case RuleType.Token: str = Token.ToText(m_tknType); break;
            case RuleType.Body: str = m_rt + " body "; break;
            case RuleType.Empty:
            case RuleType.Plus1:
            case RuleType.Plus2:
            case RuleType.Star1:
            case RuleType.Star2:
            case RuleType.Optional1:
                str = m_rt.ToString();
                break;
            case RuleType.String: str = m_rt + " s: " + m_str; break;
            case RuleType.Symbol: str = m_rt + ": " + m_sym.name; break;
            case RuleType.Not: str = "! "; break;

            default:
                if (IsChoice()) {
                    str = m_rt.ToString();
                }
                else if ((RuleType.Action0 <= m_rt) && (m_rt <= RuleType.ActionFinal)) {
                    str = m_rt.ToString();
                }
                else {
                    str = "";
                }
                break;
            }

            return str;
        }
    }

    class RuleStepList
    {
        List<RuleStep> m_lst = new List<RuleStep>();

        public RuleStepList(RuleStep rs)
        {
            m_lst.Add(rs);
        }

        public RuleStepList(RuleStep rs1, RuleStep rs2)
        {
            m_lst.Add(rs1);
            m_lst.Add(rs2);
        }

        public void InsertAt(int i, RuleStep rs)
        {
            m_lst.Insert(i, rs);
        }

        public void Add(RuleStep rs)
        {
            m_lst.Add(rs);
        }

        public RuleStep[] ToArray()
        {
            return m_lst.ToArray();
        }
    }

    class Rule
    {
        RuleNo m_iRule;
        RuleStep[] m_rgRules;


        public Rule(RuleNo iRule, RuleStep[] rgRules)
        {
            m_iRule = iRule;
            m_rgRules = rgRules;


        }

        public Rule(RuleNo iRule, RuleStepList lst)
        {
            m_iRule = iRule;
            m_rgRules = lst.ToArray();

        }


        public RuleNo ruleNo { get { return m_iRule; } }

        public RuleStep[] rgRules { get { return m_rgRules; } }

        public override string ToString()
        {
            String str;

            str = m_iRule.ToString();
            for (int ii = 0; ii < m_rgRules.Length; ii++) {
                str += " " + m_rgRules[ii].ToString();
            }

            return str;
        }
    }

    partial class Rules
    {
        static RuleNo m_lastRule = RuleNo.LastRule;

        public static RuleNo NextRuleNo()
        {
            m_lastRule += 1;
            return m_lastRule;
        }

        public static RuleNo LastRuleNo()
        {
            return m_lastRule;
        }

        public static List<Rule> m_rgRules2 = new List<Rule>();



    }

    class Parameter
    {
        public Symbol m_governor;
        public Symbol m_reference;

        public Parameter(Symbol sym1, Symbol sym2)
        {
            m_governor = sym1;
            m_reference = sym2;
        }
    }


}
