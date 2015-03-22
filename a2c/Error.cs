using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Collections;

namespace asn_compile_cs
{
    enum ErrorNumber
    {
        //  Internal compiler errors

        ICE = 1000,

        //  Command line Errors
        FileNotFound = 1200,

        //  Internal Language Errors
        DuplicateParameter = 1300,
        UndefinedDummyGovernor,

        //  Parsing Level 2 Errors
        ParseSemanticError = 1700,
        SymbolNotFound,
        SymbolNotType,
        SymbolNotValue,
        SymbolNotObjectSet,
        SymbolNotObject,
        SkippedToken,

        //  Parsing Level 1 Errors
        ParseSyntaxError = 1900,
        ExpectedToken,
        BadCharacter, 
        ParseOtherError = 1999,

        //
        HugeIntegerBadType = 2000,
        FieldNotFound,
        NotChoiceSetSeq,
        OverrideTypeOfAssignment,
        SeqSetValueMissingField,
        MustBeField,
        EnumNameTwice,
        EnumValueTwice,
        EnumExtValue,
        MustBeType,
        DuplicateImportModuleName,
        ImportModuleNotFound,
        UseFunctionTag,
        NumberFormNotInt,
        OIDStartDefinedValue,
        OIDSecondDefinedValue,
        OIDThirdDefinedValue,
        OIDNotARelativeOID,
        OIDLength,
        OIDVal1TooLarge,
        OIDVal2TooLarge,
        OIDFirstOnlyOID,
        OIDBadSymbolType,
        OIDNumberNonNegative,
        OIDNameFormMissing,
        OIDDuplicateName,
        InvalidEmitString,

        //  Warnings
        WarningBase = 4000,
        HugeIntegerTwice,
        OverflowNativeInt,
        OIDWrongName,
    }

    /// <summary>
    /// Summary description for Class1
    /// </summary>
    class Error
    {
        static public int CErrors = 0;
        static public int CWarnings = 0;

        ErrorNumber m_errno;
        List<Object> m_lst = new List<Object>();
        Token m_token;

        public Error(ErrorNumber errno)
        {
            m_errno = errno;
        }

        public Error(ErrorNumber errno, Token tkn)
        {
            m_errno = errno;
            m_token = tkn;
        }

        public Error(ErrorNumber errno, Token tkn, Object obj)
        {
            m_errno = errno;
            m_token = tkn;
            m_lst.Add(obj);
        }

        public ErrorNumber errno
        {
            get { return m_errno; }
        }

        public Token token { get { return m_token; } }

        public void Print()
        {
            Console.Error.WriteLine(this);
            if (m_errno >= ErrorNumber.WarningBase) {
                CWarnings += 1;
            }
            else {
                CErrors += 1;
            }
        }

        override public String ToString()
        {
            String str = GetMessage(m_errno);

            if (m_token == null) {
                str = String.Format(str, m_lst.ToArray());
                str = String.Format("{0}: {1} {2}", "FileName", m_errno >= ErrorNumber.WarningBase ? "Warning" : "Error", str);
            }
            else {

                switch (m_token.tknType) {
                case TknType.L_Identifier:
                case TknType.U_Identifier:
                    m_lst.Insert(0, m_token.name);
                    break;

                default:
                    m_lst.Insert(0, m_token);
                    break;
                }
                str = String.Format(str, m_lst.ToArray());
                str = String.Format("{0} ({1}, {4}, {3}): {5} {2}", m_token.fileName, m_token.line, str, m_token.index, m_token.charNo,
                    m_errno >= ErrorNumber.WarningBase ? "Warning" : "Error");

                m_lst.RemoveAt(0);
            }

            return str;
        }

        public void AddObject(Object obj)
        {
            m_lst.Add(obj);
        }

        String GetMessage(ErrorNumber errno)
        {
            switch (errno) {
            case ErrorNumber.ICE: return "Internal Compiler Error";
            case ErrorNumber.ExpectedToken: return "Found Token {0}, Expected Token {1}";
            case ErrorNumber.FileNotFound: return "File Not Found {0}";
            case ErrorNumber.DuplicateParameter: return "Parameter {1} is defined twice";
            case ErrorNumber.UndefinedDummyGovernor: return "Parameter uses {1} as a governor, but it is not itself a parameter";
            case ErrorNumber.SymbolNotFound: return "Symbol {0} not defined.";
            case ErrorNumber.SymbolNotType: return "Symbol {0} is not valid for a type expression";
            case ErrorNumber.HugeIntegerBadType: return "{1}: Type or field is not an INTEGER.";
            case ErrorNumber.HugeIntegerTwice: return "Only one {0} command is used.";
            case ErrorNumber.FieldNotFound: return "{1}: Field '{0}' not found in the sequence, set or choice.";
            case ErrorNumber.NotChoiceSetSeq: return "Type is not a CHOICE, SET or SEQUENCE";
            case ErrorNumber.OverrideTypeOfAssignment: return "NYI: {1} overriding a non-base type.";
            case ErrorNumber.SeqSetValueMissingField: return "Field {1} is not specified in set or sequence value.";
            case ErrorNumber.MustBeField: return "Target of command must be a SET/SEQUENCE/CHOICE field.";
            case ErrorNumber.EnumNameTwice: return "Enumeration name {1} is used twice.";
            case ErrorNumber.EnumValueTwice: return "Enumeration value {1} is used twice.";
            case ErrorNumber.EnumExtValue: return "Enumeration values in extension set must be in increasing order.";
            case ErrorNumber.MustBeType: return "Command must refer to a type.";
            case ErrorNumber.DuplicateImportModuleName: return "Module name {1} is used more than once.";
            case ErrorNumber.ImportModuleNotFound: return "Import module name {1} not found.";
            case ErrorNumber.UseFunctionTag: return "Unrecognized UseFunction tag.";
            case ErrorNumber.NumberFormNotInt: return "{1} must be an integer value";
            case ErrorNumber.OverflowNativeInt: return "{0} overflows optional field.  Use pointer command.";
            case ErrorNumber.SkippedToken: return "Skipped token {0}";
            case ErrorNumber.SymbolNotValue: return "{0} is not a value";
            case ErrorNumber.SymbolNotObject: return "{0} is not an object";
            case ErrorNumber.SymbolNotObjectSet: return "{0} is not an object set";
            case ErrorNumber.OIDStartDefinedValue: return "OID must start with an OID or an integer (symbol {1})";
            case ErrorNumber.OIDSecondDefinedValue: return "Symbol {1} must be an integer";
            case ErrorNumber.OIDThirdDefinedValue: return "Symbol {1} must be an integer or a relative object identifier";
            case ErrorNumber.OIDNotARelativeOID: return "Must be a relative OID value";
            case ErrorNumber.OIDLength: return "An OID must be at least two numbers long";
            case ErrorNumber.OIDVal1TooLarge: return "First number in an oid must not exceed 5";
            case ErrorNumber.OIDVal2TooLarge: return "Second number in an oid must not exceed 39";
            case ErrorNumber.OIDWrongName: return "Arc node has two names {1} and {2}";
            case ErrorNumber.OIDBadSymbolType: return "Symbol {1} is not a legal type for this location";
            case ErrorNumber.OIDFirstOnlyOID: return "An object identifier symbol {1} must be the first element in an OID";
            case ErrorNumber.OIDNumberNonNegative: return "Integer value for object identifier must be non-negative";
            case ErrorNumber.OIDNameFormMissing: return "Symbol {1} is not a pre-defined name for an object identifier";
            case ErrorNumber.OIDDuplicateName: return "The string {1} is used for the number {2} and {3}.";
            case ErrorNumber.InvalidEmitString: return "EmitString is incorrect.";
            case ErrorNumber.BadCharacter: return "Illegal character {1:x} found at line {0}";
            }
            return String.Format("Unknown Error {0} {{0}}", errno);
        }

        public override bool Equals(object obj)
        {
            if (obj == null) return false;      // Can't be null

            // If parameter cannot be cast to Point return false.
            Error err = obj as Error;
            if ((System.Object)err == null) {
                return false;
            }

            if ((err.m_token != m_token) || (err.m_errno != m_errno)) return false;

            /*  Is this object on the list of the other (or versa vica)  M00BUG
             */

            if (err.m_lst.Count != m_lst.Count) return false;
            for (int i = 0; i < m_lst.Count; i++) {
                if (!err.m_lst[i].Equals(m_lst[i])) return false;
            }
            return true;
        }
    }

    class ErrorList : IEnumerable<Error>
    {
        List<Error> m_lst = new List<Error>();

        public int Count { get { return m_lst.Count; } }

        public void Add(Error errNew)
        {
            //
            //  M00TODO this is where disabled warnings should be checked
            //

            // if (errNew.errno == ErrorNumber.OIDWrongName) return;

            //
            //  Now add it to the list
            //

            if (m_lst.Count > 0) {
                //
                //  If our token number is less than the list - ignore it
                //
                //  M00QUEST - is that true for warnings?
                //

                if (errNew.token.index < m_lst[0].token.index) {
                    return;
                }

                //
                //  If our token number is greater than the list - empty the list
                //

                if (errNew.token.index > m_lst[0].token.index) {
                    m_lst.Clear();
                    m_lst.Add(errNew);
                    return;
                }

                //
                //  Insert logic dealing with error levels.
                //
                //  Three different types of errors
                //
                //  If new is a syntax error - don't add if there is a semantic error
                //  If new is a semantic error - clear all syntax errors from the list
                //  If new is not in either range - just add the error
                //

                if ((ErrorNumber.ParseSemanticError <= errNew.errno) && (errNew.errno < ErrorNumber.ParseSyntaxError)) {
                    for (int i = 0; i < m_lst.Count; i++) {
                        if ((ErrorNumber.ParseSyntaxError <= m_lst[i].errno) && (m_lst[i].errno < ErrorNumber.ParseOtherError)) {
                            m_lst.RemoveAt(i);
                            i -= 1;
                        }
                    }
                }
                else if ((ErrorNumber.ParseSyntaxError <= errNew.errno) && (errNew.errno < ErrorNumber.ParseOtherError)) {
                    for (int i=0; i<m_lst.Count; i++) {
                        if ((ErrorNumber.ParseSemanticError <= m_lst[i].errno) && (m_lst[i].errno < ErrorNumber.ParseSyntaxError)) {
                            return;
                        }
                    }
                }
            }

            //
            //  Don't add duplicates

            foreach (Error errOld in m_lst) {
                if (errOld.Equals(errNew)) {
                    return;
                }
            }
            m_lst.Add(errNew);
        }

        public void Add(ErrorList errlstNew)
        {
            foreach (Error errNew in errlstNew) {
                Add(errNew);
            }
        }

        public void Print()
        {
            foreach (Error err in m_lst) {
                err.Print();
            }
        }

        public override string ToString()
        {
            String str = "";

            foreach (Error err in m_lst) {
                str += err.ToString() + "\r\n";
            }
            return str;
        }

        public IEnumerator<Error> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

    }

    class ObjectList : IEnumerable<Object>
    {
        List<Object> m_lst = new List<Object>();

        public void Add(Object objNew)
        {
            m_lst.Add(objNew);
        }

        public void Insert(int i, Object objNew)
        {
            m_lst.Insert(i, objNew);
        }

        public IEnumerator<Object> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        public override bool Equals(object obj)
        {
             if (obj == null) return false;      // Can't be null

            // If parameter cannot be cast to Point return false.
            ObjectList err = obj as ObjectList;
            if ((asn_compile_cs.ObjectList)err == null) {
                return false;
            }

            if (m_lst.Count != err.m_lst.Count) return false;

            for (int i=0; i<m_lst.Count; i++) {
                if (!m_lst[i].Equals(err.m_lst[i])) return false;
            }
            return true; 
        }

        public Object[] ToArray()
        {
            return m_lst.ToArray();
        }
    }
}
