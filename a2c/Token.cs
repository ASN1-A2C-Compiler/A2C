using System;
using System.Collections.Generic;
using System.Text;

namespace asn_compile_cs
{

    enum TknType { empty = 1, A_L_Identifier, A_U_Identifier, ADDTOOBJECTSET, And, AtSign, AtDot, Bang, BINARY,
        Carat, CMDSTART, CMDEND, Colon, Comma, Dash, Definition, Dot, DotDot, DotDotDot, 
        DQuote, Equal, HUGEENUMERATED, HUGEINTEGER, LAngle, LBrace, LineBreak, LParen, LSqr, LSqrLSqr, NAMESPACE, NATIVEENUMERATED, NATIVEINTEGER,
        POINTER, RAngle, RBrace, RParen, RSqr, RSqrRSqr, SemiColon, Slash, SQuote, Star, UScore, USEFUNCTION, VLine, VIEW,
        L_Identifier = 100, U_Identifier, Number, Real, Reduced, HexString, BinaryString, cstring, mstring,
        ABSENT = 1000, ABSTRACT_SYNTAX, ALL, APPLICATION, AUTOMATIC,
        BEGIN, BIT, BMPString, BOOLEAN, BY,
        CHARACTER, CHOICE, CLASS, COMPONENT, COMPONENTS, CONSTRAINED, CONTAINING,
        DEFAULT, DEFINITIONS, DATE, DATE_TIME, DURATION,
        EMBEDDED, EMITSTRING, ENCODED, ENCODING, ENCODING_CONTROL, END, ENUMERATED, EXCEPT, EXPLICIT, EXPORTS, EXTENSIBILITY, EXTERNAL,
        FALSE, FROM, 
        GeneralizedTime, GeneralString, GraphicString,
        IA5String, IDENTIFIER, IMPLICIT, IMPLIED, IMPORTS, INCLUDES, INSTANCE, INSTRUCTIONS, INTEGER, INTERSECTION, ISO646String,
        MAX, MIN, MINUS_INFINITY,
        NewLine, NOT_A_NUMBER, NULL, NumericString,
        OBJECT, ObjectDescriptor, OCTET, OID_IRI, OF, OPTIONAL,
        PATTERN, PDV, PLUS_INFINITY, PRESENT, PrintableString, PRIVATE,
        REAL, RELATIVE_OID, RELATIVE_OID_IRI,
        SEQUENCE, SET, SETTINGS, SIZE, STRING, SYNTAX,
        T61String, TAGS, TeletexString, TIME, TIME_OF_DAY, TRUE, TYPE_IDENTIFIER,
        UNION, UNIQUE, UNIVERSAL, UniversalString, USETYPE, UTCTime, UTF8String,
        VideotexString, VisibleString, WITH, ANY
    };

    class Token
    {
        String m_strName;
        long m_iNumber;
        double m_real;
        TknType m_tknType;
        RuleNo m_ruleNo;
        Results[] m_rgrv = new Results[10];
        int m_iPass;

        String m_strFileName;
        bool m_fLeadingWhitespace;   // Leading white space 
        int m_iLine;        // Line # for the token
        int m_iChar;        // Char # in the line for the token
        int m_iIndex;       // index of the token;

        Symbol m_sym;

        //
        //  Constructor for simple token
        //

        public Token(TknType tkn, bool fNoWhitespace, int iLine, int iChar)
        {
            m_tknType = tkn;
            m_iLine = iLine;
            m_iChar = iChar;
            m_fLeadingWhitespace = !fNoWhitespace;
        }

        public Token(TknType tkn, String str, int iLine, int iChar)
        {
            m_tknType = tkn;
            m_strName = str;
            m_iLine = iLine;
            m_iChar = iChar;
            m_fLeadingWhitespace = false;
        }

        //
        //  Constuctor for integer tokens
        //

        public Token(long iValue, int iLine, int iChar)
        {
            m_tknType = TknType.Number;
            m_iNumber = iValue;
            m_iLine = iLine;
            m_iChar = iChar;
            m_fLeadingWhitespace = true;
        }

        //
        //  Constructor for floating number token
        //

        public Token(double dbl, int iLine, int iChar)
        {
            m_tknType = TknType.Real;
            m_real = dbl;
            m_iLine = iLine;
            m_iChar = iChar;
            m_fLeadingWhitespace = true;
        }

        public Token(RuleNo action, int iLine, int iChar)
        {
            m_tknType = TknType.Reduced;
            m_ruleNo = action;
            m_iLine = iLine;
            m_iChar = iChar;
            m_fLeadingWhitespace = true;
        }

        public String fileName
        {
            get { return m_strFileName; }
            set { m_strFileName = value; }
        }

        public int index
        {
            get { return m_iIndex; }
            set { m_iIndex = value; }
        }

        public double real
        {
            get { return m_real; }
        }

        public int charNo { get { return m_iChar; } }
        public int line { get { return m_iLine; } }

        public long number
        {
            get { return m_iNumber; }
        }

        public string name 
        { 
            get { return m_strName; } 
        }

        public RuleNo ruleNo
        {
            get { return m_ruleNo; }
        }

        public Symbol sym
        {
            get { return m_sym; }
            set { m_sym = value; }
        }

        public TknType tknType
        {
            get { return m_tknType; }
        }

        public bool LeadingWhiteSpace { get { return m_fLeadingWhitespace; } }

        public static string ToText(TknType tknType)
        {
            switch (tknType) {
            case TknType.CMDSTART: return "'--#'";
            case TknType.CMDEND: return "'#--'";
            }
            return tknType.ToString();
        }

        public override string ToString()
        {
            String str;
            str = m_tknType.ToString();

            switch (m_tknType) {
            case TknType.L_Identifier: str += " " + m_strName; break;
            case TknType.U_Identifier: str += " " + m_strName; break;
            case TknType.Number: str += " " + m_iNumber.ToString(); break;
            case TknType.Reduced: str += " " + m_ruleNo.ToString(); break;
            }

            return str;
        }

        public Results GetResult(RuleNo ruleNo, int iPass)
        {
            if ((iPass != m_iPass) && (m_iPass != -1)) {
                m_iPass = iPass;
                for (int i = 0; i < m_rgrv.Length; i++) {
                    if ((m_rgrv[i] != null) && (m_rgrv[i].mr == MatchResult.Fail)) m_rgrv[i] = null;
                }
            }

            switch (ruleNo) {
            case RuleNo.ComponentType: return m_rgrv[0];
//            case RuleNo.PrimitiveType: return m_rgrv[1];
//            case RuleNo.Type: return m_rgrv[2];
            case RuleNo.ObjectIdentifierValue: return m_rgrv[3];
            }
            return null;
        }

        public void SetResult(RuleNo ruleNo, int iPass, Results r)
        {
            if ((iPass != m_iPass) && (m_iPass != -1)) {
                m_iPass = iPass;
                //  Clear out any saved false results
                for (int i = 0; i < m_rgrv.Length; i++) {
                    if ((m_rgrv[i] != null) && (m_rgrv[i].mr == MatchResult.Fail)) m_rgrv[i] = null;
                }
            }

            switch (ruleNo) {
            case RuleNo.ComponentType: if (m_iPass == -1) { m_rgrv[0] = r; } break;
//            case RuleNo.Value: m_rgrv[0] = r; break;
//            case RuleNo.PrimitiveType: m_rgrv[1] = r; break;
//            case RuleNo.Type: m_rgrv[2] = r; break;
            case RuleNo.ObjectIdentifierValue: m_rgrv[3] = r; break;
            }

            return;
        }
    }

    class TokenList : IEnumerable<Token>
    {
        List<Token> m_lst;


        public TokenList()
        {
            m_lst = new List<Token>();
        }

        public TokenList(Token tkn)
        {
            m_lst = new List<Token>();
            m_lst.Add(tkn);
        }

        public TokenList(List<Token> lst)
        {
            m_lst = lst;
        }

        public Token this[int i]
        {
            get { return m_lst[i]; }
        }

        public int Count { get { return m_lst.Count; } }

        public List<Token> list
        {
            get { return m_lst; }
        }

        public void Add(Token tkn)
        {
            m_lst.Add(tkn);
        }

        public void AddRange(TokenList lst)
        {
            m_lst.AddRange(lst.m_lst);
        }

        public void Insert(int i, Token tkn)
        {
            m_lst.Insert(0, tkn);
        }

        public string ToDotString()
        {
            string szOut = "";

            foreach (Token tkn in m_lst) {
                szOut = tkn.name + ".";
            }

            return szOut.Substring(0, szOut.Length - 1);
        }

        public IEnumerator<Token> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

    }

    class Results
    {
        MatchResult m_mr;
        RuleValue m_rv;
        int m_cTokensUsed;

        public Results(MatchResult mr, RuleValue rv, int cTokensUsed)
        {
            m_mr = mr;
            m_rv = rv;
            m_cTokensUsed = cTokensUsed;
        }

        public MatchResult mr { get { return m_mr; } }
        public RuleValue rv { get { return m_rv; } }
        public int cTokensUsed { get { return m_cTokensUsed; } }
    }

}
