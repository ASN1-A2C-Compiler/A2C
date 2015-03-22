using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Tools;

namespace Recurser
{
    public enum TokenType { Literal=1, Identifier, Code, Plus, Star, Optional, Choice, Empty,
        Token, Optional1, Plus1, Plus2, Star1, Star2, Bang, BSlash, UScore, TokenList }

    class Program
    {
        static public Dictionary<String, TypeDesc> TypeMap = new Dictionary<string,TypeDesc>();
        static public string FileName;
        static public string OutputFile;
        static public int CErrors = 0;
        static public int CWarnings = 0;
        static public bool FQuiet = false;

        static void Main(string[] args)
        {
            int c = 1;

            if (args[0] == "-quiet") {
                FQuiet = true;
                c = 2;    
            }

            if (args.Length !=c) {
                Console.WriteLine("usage:  rcc <input file>");
                Environment.Exit(-1);
            }

            FileName = args[c-1];
            OutputFile = FileName + ".cs";

            Parser p = new parser(new lex());
            // p.m_debug = true;
            StreamReader s = new StreamReader(FileName);
            if (p.Parse(s) != null) {
                Console.WriteLine("Success");
            }

            Console.WriteLine("Count Errors = {0},  Count Warnings = {1}", CErrors, CWarnings);

            Environment.Exit(-CErrors);
        }


        static public void ProcessAll(String strHeader, RuleList lst, String strFooter)
        {
            StreamWriter stream = new StreamWriter(OutputFile);
            Dictionary<string, SingleRule> dict = new Dictionary<string, SingleRule>();
            int i;

            if (strHeader.Length > 0) {
                stream.Write(strHeader);
            }

            //
            //  Walk the list putting everything into an array for later checking and emit the enumeration
            //

            stream.Write("namespace asn_compile_cs\n{\n");
            stream.Write("\tpublic enum RuleNo { RuleIsMissing, \n");
            stream.Write("\t\t");
            i = 0;
            foreach (SingleRule rule in lst.list) {
                stream.Write(rule.RuleName);
                stream.Write(", ");
                i += 1;
                if (i % 5 == 0) stream.Write("\n\t\t");

                try {
                    dict.Add(rule.RuleName, rule);
                }
                catch {
                    Console.WriteLine("Duplicate rule name {0}", rule.RuleName);
                    CErrors += 1;
                }
                rule.m_iRuleNo = i;
                rule.Normalize();
            }

            stream.Write("\n\tLastRule }\n\n");                // End of enum

            //
            //  Look for items w/o typing information
            //



            //
            //  Write out the RuleStep data
            //

            stream.WriteLine("\tpartial class Rules");
            stream.WriteLine("\t{");
            stream.WriteLine("\t\tpublic static Rule[] m_rules = new Rule[] {");
            foreach(SingleRule rule in lst.list) {
                rule.WriteRule(stream);
            }
            stream.WriteLine("\t\t};");
            stream.WriteLine("\t};");


            //
            //  Write Out Action data
            //

            WriteActionHeader(stream);
            foreach(SingleRule rule in lst.list) {
                rule.WriteAction(stream);
            }
            WriteActionFooter(stream);

            stream.Write("}\n");                    // End of namespace
            if (strFooter.Length > 0) {
                stream.WriteLine(strFooter);
            }

            stream.Close();
        }

        static void WriteActionHeader(StreamWriter stream)
        {
            stream.WriteLine("\tpartial class AsnFile");
            stream.WriteLine("\t{");
            stream.WriteLine("\t\tMatchResult X680_Actions(RuleNo rule, RuleType action, out int cTokensUsed, HistoryStack history, out RuleValue rvOut)");
            stream.WriteLine("\t\t{");
            stream.WriteLine("\t\t\tint myChoice;");
            stream.WriteLine("\t\t\t//");
            stream.WriteLine("\t\t\t//  Add up the number of tokens used.");
            stream.WriteLine("\t\t\t//");
            stream.WriteLine();
            stream.WriteLine("\t\t\tcTokensUsed = CountTokensUsed(history);");
            stream.WriteLine();
            stream.WriteLine("\t\t\tmyChoice = (((int)rule)<<8) | (((int)(action-RuleType.Action0))<< 5);");
            stream.WriteLine("\t\t\tif ((RuleType.Choice0 <= history[0].choice) && (history[0].choice <RuleType.ChoiceLast)) myChoice |= history[0].choice - RuleType.Choice0;");
            stream.WriteLine("\t\t\tif ((RuleType.ExChoice0 <= history[0].choice) && (history[0].choice <RuleType.ExChoiceLast)) myChoice |= history[0].choice - RuleType.ExChoice0;");
            stream.WriteLine("\t\t\tswitch (myChoice) {");
        }

        static void WriteActionFooter(StreamWriter steam)
        {
            steam.WriteLine("\t\t\tdefault:");
            steam.WriteLine("\t\t\t\trvOut = null;");
            steam.WriteLine("\t\t\t\treturn MatchResult.Fail;");
            steam.WriteLine("\t\t\t}");
            steam.WriteLine("\t\t\treturn MatchResult.Success;");
            steam.WriteLine("\t\t}");
            steam.WriteLine("\t}");
        }
    }

    public class TypeDesc
    {
        public String fieldName;
        public String typeName;

        public TypeDesc(String typ, String fld)
        {
            fieldName = fld;
            typeName = typ;
        }
    }

    public class RuleElement
    {
        TokenType m_tknType;
        String m_val;
        String m_id;
        int m_iVal;

        public RuleElement(TokenType tknType)
        {
            m_tknType = tknType;
        }

        public RuleElement(TokenType tknType, String val)
        {
            m_tknType = tknType;
            m_val = val;
        }

        public RuleElement(TokenType tknType, String val, String id)
        {
            m_tknType = tknType;
            m_val = val;
            m_id = "$" + id;
        }

        public String id
        {
            get { return m_id; }
        }

        public int iVal
        {
            get { return m_iVal; }
            set { m_iVal = value; }
        }

        public string strVal { get { return m_val; } }

        public TokenType tokenType { 
            get { return m_tknType; }
            set { m_tknType = value; }
        }

        public void Normalize()
        {
            int i;

            switch (m_tknType) {
            case TokenType.Identifier:
                //
                //  If it is all upper case - change it to a Token
                //

                for (i = 0; i < m_val.Length; i++) {
                    if (!Char.IsUpper(m_val[i])) break;
                }
                if (i == m_val.Length) m_tknType = TokenType.Token;
                break;
            }
        }

        public void WriteRule(StreamWriter stream)
        {
            switch (m_tknType) {
            default:
                stream.Write("NYI:" + m_tknType.ToString() + ", ");
                break;

            case TokenType.Bang:
                stream.Write("new RuleStep(RuleType.Not), ");
                break;

            case TokenType.Empty:
                stream.Write("new RuleStep(RuleType.Empty), ");
                break;

            case TokenType.Identifier:
                stream.Write("new RuleStep(RuleNo." + m_val + "), ");
                break;

            case TokenType.Token:
                stream.Write("new RuleStep(TknType." + m_val + "), ");
                break;

            case TokenType.Optional:
                stream.Write("NYI:OPTIONAL, ");
                break;

            case TokenType.Optional1:
                stream.Write("new RuleStep(RuleType.Optional1), ");
                break;

            case TokenType.Plus1:
                stream.Write("new RuleStep(RuleType.Plus1), ");
                break;

            case TokenType.Plus2:
                stream.Write("new RuleStep(RuleType.Plus2), ");
                break;

            case TokenType.Star1:
                stream.Write("new RuleStep(RuleType.Star1), ");
                break;

            case TokenType.Star2:
                stream.Write("new RuleStep(RuleType.Star2), ");
                break;

            case TokenType.Choice:
                stream.Write("new RuleStep(RuleType.Choice"+m_iVal.ToString()+"), ");
                break;

            case TokenType.BSlash:
                stream.Write("new RuleStep(RuleType.ExChoice" + m_iVal.ToString() + "), ");
                break;

            case TokenType.Code:
                if (iVal < 7) {
                    stream.Write("new RuleStep(RuleType.Action" + m_iVal.ToString() + "), ");
                }
                break;

            case TokenType.UScore:
                stream.Write("new RuleStep(RuleType.UScore), ");
                break;

            case TokenType.TokenList:
                stream.Write("new RuleStep(RuleType.TokenList), ");
                break;

            case TokenType.Literal:
                stream.Write("new RuleStep(" + m_val + "), ");
                break;

            }
        }
    }

    public class ElementList
    {
        List<RuleElement> m_lst = new List<RuleElement>();

        public List<RuleElement> list { get { return m_lst; } }

        public void Add(RuleElement re) { m_lst.Add(re); }
    }

    public class RuleList
    {
        List<SingleRule> m_lst = new List<SingleRule>();

        public void Add(SingleRule re) { m_lst.Add(re); }

        public List<SingleRule> list { get { return m_lst; } }
    }

    public class SingleRule
    {
        String m_lhs;
        ElementList m_rhs;
        public int m_iRuleNo;
        int m_iLine;

        public SingleRule(int iLine, String lhs, ElementList rhs)
        {
            m_lhs = lhs;
            m_rhs = rhs;
            m_iLine = iLine;
        }

        public string RuleName { get { return m_lhs; } }

        public void Normalize()
        {
            int i;
            int iChoice = 0;
            int iChoiceType = 0;
            int iAction = 0;
            RuleElement re2;

            foreach (RuleElement re in m_rhs.list) {
                re.Normalize();
            }

            //
            //  We walk the list and look for some special things that we need to fix.
            //
            //  Can't use the foreach since we are going to mess up the order of items.
            //

            for (i = 0; i < m_rhs.list.Count; i++) {
                re2 = m_rhs.list[i];
                switch (re2.tokenType) {
                case TokenType.Optional:
                    if (i == 0) {
                        Console.WriteLine("Optional Cannot be first token in a rule (Rule=" + m_lhs + ")");
                        Program.CErrors += 1;
                    }
                    if (m_rhs.list[i - 1].tokenType == TokenType.Code) {
                        if (!Program.FQuiet) {
                            Console.WriteLine("Optional follows code for " + m_lhs + " - Feature NYI");
                        }
                        Program.CWarnings += 1;
                    }
                    m_rhs.list.RemoveAt(i);
                    m_rhs.list.Insert(i - 1, re2);
                    re2.tokenType = TokenType.Optional1;
                    break;

                case TokenType.Choice:
                    if (iChoiceType == 2) {
                        Console.WriteLine("Cannot mix Choice and Exclusive Choice in a single rule (Rule= " + m_lhs + ")");
                        Program.CErrors += 1;
                    }
                    iChoiceType = 1;
                    iChoice += 1;
                    re2.iVal = iChoice;
                    if (iChoice == 1) {
                        m_rhs.list.Insert(0, new RuleElement(TokenType.Choice));
                        i += 1;
                    }
                    iAction = 0;
                    break;

                case TokenType.BSlash:
                    if (iChoiceType == 1) {
                        Console.WriteLine("Cannot mix Choice and Exclusive Choice in a single rule (Rule= " + m_lhs + ")");
                        Program.CErrors += 1;
                    }
                    iChoiceType = 2;
                    iChoice += 1;
                    re2.iVal = iChoice;
                    if (iChoice == 1) {
                        m_rhs.list.Insert(0, new RuleElement(TokenType.BSlash));
                        i += 1;
                    }
                    iAction = 0;
                    break;

                case TokenType.Plus:
                    if (i < 2) {
                        Console.WriteLine("Plus cannot be first or second token in a rule  (Rule: " + m_lhs + ")");
                        Program.CErrors += 1;
                    }
                    m_rhs.list.RemoveAt(i);
                    m_rhs.list.Insert(i - 2, re2);
                    if (m_rhs.list[i].tokenType == TokenType.Empty) {
                        re2.tokenType = TokenType.Plus1;
                        m_rhs.list.RemoveAt(i);
                        i -= 1;
                    }
                    else {
                        re2.tokenType = TokenType.Plus2;
                    }
                    break;

                case TokenType.Star:
                    if (i < 2) {
                        Console.WriteLine("Star cannot be first or second token in a rule (Rule: " + m_lhs + ")");
                        Program.CErrors += 1;
                    }
                    m_rhs.list.RemoveAt(i);
                    m_rhs.list.Insert(i - 2, re2);
                    if (m_rhs.list[i].tokenType == TokenType.Empty) {
                        re2.tokenType = TokenType.Star1;
                        m_rhs.list.RemoveAt(i);
                        i -= 1;
                    }
                    else {
                        re2.tokenType = TokenType.Star2;
                    }
                    break;

                case TokenType.Code:
                    if ((i == m_rhs.list.Count - 1) || (m_rhs.list[i + 1].tokenType == TokenType.Choice) ||
                        (m_rhs.list[i+1].tokenType == TokenType.BSlash)) {
                        re2.iVal = 7;
                    }
                    else {
                        re2.iVal = iAction;
                        iAction += 1;
                    }
                    break;
                }
            }
        }

        public void WriteAction(StreamWriter stream)
        {
            int i = 0;
            int i1 = 0;
            int iCaseNo;
            int iChoice = 0;
            int iChoiceIndex = 0;
            RuleElement re;
            String[,] rgStr = new String[50, 3];
            String strPrefix = null;

            for (i = 0; i < m_rhs.list.Count; i++, i1++) {
                re = m_rhs.list[i];
                switch (re.tokenType) {
                case TokenType.Choice:
                case TokenType.BSlash:
                    if (i == 0) break;

                    if (m_rhs.list[i - 1].tokenType != TokenType.Code) {
                        iCaseNo = (m_iRuleNo << 8) + (7 << 5) + (iChoice << 0);
                        stream.WriteLine("\t\t\t\tcase " + iCaseNo.ToString() + ":    // Rule: " + m_lhs + "  Choice: " + iChoice.ToString() + " Action: 7");
                        stream.WriteLine("\t\t\t\t\trvOut = history[1].rv;");
                        stream.WriteLine("\t\t\t\t\tbreak;");
                        stream.WriteLine();

                        CheckTypeMatch(m_iLine, m_lhs, m_rhs.list[iChoiceIndex + 1].strVal);

                    }
                    iChoice = re.iVal;
                    iChoiceIndex = i;

                    //  Reset the index variable array to it's start;
                    rgStr = new String[50, 3];
                    i1 = 0;
                    break;

                case TokenType.Code:
                    iCaseNo = (m_iRuleNo << 8) + (re.iVal << 5) | iChoice;
                    stream.WriteLine("case " + iCaseNo.ToString() + ":       // Rule: " + m_lhs + " Choice: " + iChoice.ToString() + " Action: " + re.iVal.ToString());
                    String str = re.strVal;
                    if (strPrefix != null) {
                        stream.WriteLine(strPrefix);
                    }

                    for (int j = 0; j < 30; j++) {
                        if (rgStr[j, 0] != null) {
                            str = str.Replace(rgStr[j, 0] + ".result", rgStr[j, 2] + ".result");
                            str = str.Replace(rgStr[j, 0] + ".rv", rgStr[j, 2] + ".rv");
                            str = str.Replace(rgStr[j, 0] + ".iTokenLast", rgStr[j, 2] + ".iTokenLast");
                            str = str.Replace(rgStr[j, 0] + ".iToken", rgStr[j, 2] + ".iToken");
                            str = str.Replace(rgStr[j, 0] + ".cTokens", rgStr[j, 2] + ".cTokens");
                            if (rgStr[j, 1] == null) {
                                str = str.Replace(rgStr[j, 0], rgStr[j, 2]);
                            }
                            else {
                                try {
                                    TypeDesc td = Program.TypeMap[rgStr[j, 1]];
                                    str = str.Replace(rgStr[j, 0], rgStr[j, 2] + ".rv." + td.fieldName);
                                }
                                catch {
                                    str = str.Replace(rgStr[j, 0], rgStr[j, 2] + ".rv");
                                }
                            }
                        }
                    }

                    try {
                        TypeDesc td = Program.TypeMap[m_lhs];
                        str = str.Replace("$$.", "rvOut." + td.fieldName + ".");
                        str = str.Replace("$$[", "rvOut." + td.fieldName + "[");
                    }
                    catch {
                    }

                    str = str.Replace("$$", "rvOut");
                    str = str.Replace("FAIL;", "{rvOut=null; return MatchResult.Fail; }");
                    stream.Write(str);

                    if (re.iVal == 7) {
                        try {
                            TypeDesc td = Program.TypeMap[m_lhs];

                            stream.WriteLine("Debug.Assert(rvOut.Is" + td.typeName + "());");
                        }
                        catch {
                            ;
                        }
                    }


                    if (strPrefix != null) {
                        stream.WriteLine("}");
                        strPrefix = null;
                    }
                    stream.WriteLine("\t\t\t\t\treturn MatchResult.Success;");
                    break;

                case TokenType.Identifier:
                case TokenType.Token:
                    rgStr[i1, 0] = re.id;
                    rgStr[i1, 1] = re.strVal;
                    rgStr[i1, 2] = String.Format("history[{0}]", i1.ToString());
                    break;

                case TokenType.Plus1:
                case TokenType.Star1:
                    if ((m_rhs.list[i + 1].tokenType == TokenType.Identifier) && (m_rhs.list[i + 1].id != null)) {


                        try {
                            TypeDesc td = Program.TypeMap[m_rhs.list[i + 1].strVal];

                            rgStr[i1 + 1, 0] = m_rhs.list[i + 1].id;
                            rgStr[i1 + 1, 1] = null;
                            rgStr[i1 + 1, 2] = "__" + m_rhs.list[i + 1].id.Substring(1);

                            strPrefix = String.Format("{{ List<{0}> {1} = new List<{0}>();", td.typeName, rgStr[i1 + 1, 2]);
                            strPrefix += String.Format("for (int __index = 1; __index<history[{0}].cList; __index+= 1)", i1.ToString());
                            strPrefix += String.Format("    {0}.Add(history[__index+{1}].rv.{2});", rgStr[i1 + 1, 2], i.ToString(), td.fieldName);

                            i1++;
                            i++;
                        }
                        catch (Exception e) {
                        }
                    }
                    break;

                case TokenType.Plus2:
                case TokenType.Star2:
                    strPrefix = null;
                    if (((m_rhs.list[i + 1].tokenType == TokenType.Identifier) ||
                         (m_rhs.list[i + 1].tokenType == TokenType.Token)) && (m_rhs.list[i + 1].id != null)) {
                        try {
                            TypeDesc td = Program.TypeMap[m_rhs.list[i + 1].strVal];

                            i1++;
                            i++;

                            rgStr[i1, 0] = m_rhs.list[i].id;
                            rgStr[i1, 1] = null;
                            rgStr[i1, 2] = "__" + m_rhs.list[i].id.Substring(1);

                            strPrefix = String.Format("{{ List<{0}> {1} = new List<{0}>();", td.typeName, rgStr[i1, 2]);
                            strPrefix += String.Format("for (int __index = 1; __index<history[{0}].cList; __index += 2)", (i1 - 1).ToString());
                            strPrefix += String.Format(" {0}.Add(history[__index+{1}].rv.{2});", rgStr[i1, 2], (i - 1).ToString(), td.fieldName);
                        }
                        catch {
                        }
                    }

                    if (((m_rhs.list[i + 2].tokenType == TokenType.Identifier) ||
                         (m_rhs.list[i + 2].tokenType == TokenType.Token)) && (m_rhs.list[i + 2].id != null)) {
                        try {
                            TypeDesc td = Program.TypeMap[m_rhs.list[i + 2].strVal];

                            i1++;
                            i++;

                            rgStr[i1, 0] = m_rhs.list[i+1].id;
                            rgStr[i1, 1] = null;
                            rgStr[i1, 2] = "__" + m_rhs.list[i+1].id.Substring(1);

                            strPrefix = String.Format("{{ List<{0}> {1} = new List<{0}>();", td.typeName, rgStr[i1, 2]);
                            strPrefix += String.Format("for (int __index = 1; __index<history[{0}].cList; __index += 2)", (i1 - 1).ToString());
                            strPrefix += String.Format(" {0}.Add(history[__index+{1}].rv.{2});", rgStr[i1, 2], (i - 1).ToString(), td.fieldName);
                        }
                        catch {
                        }
                    }
                    break;

                }
            }

            if (m_rhs.list[i - 1].tokenType != TokenType.Code) {
                iCaseNo = (m_iRuleNo << 8) + (7 << 5) + (iChoice << 0);
                stream.WriteLine("\t\t\t\tcase " + iCaseNo.ToString() + ":    // Rule: " + m_lhs + "  Choice: " + iChoice.ToString() + " Action: 7");
                if ((m_rhs.list[0].tokenType == TokenType.Choice) || (m_rhs.list[0].tokenType == TokenType.BSlash)) {
                    stream.WriteLine("\t\t\t\t\trvOut = history[1].rv;");
                    CheckTypeMatch(m_iLine, m_lhs, m_rhs.list[iChoiceIndex + 1].strVal);
                }
                else {
                    stream.WriteLine("\t\t\t\t\trvOut = history[0].rv;");
                    CheckTypeMatch(m_iLine, m_lhs, m_rhs.list[iChoiceIndex].strVal);
                }
                try {
                    TypeDesc td = Program.TypeMap[m_lhs];

                    stream.WriteLine("Debug.Assert(rvOut.Is" + td.typeName + "());");
                }
                catch {
                    ;
                }
                stream.WriteLine("\t\t\t\t\tbreak;");
            }

        }

        public void CheckTypeMatch(int iLine, string lhs, string rhs)
        {
            //
            //  Lookup both the left and right hand sides and see if they are of the same type.  If not then warn
            //

            TypeDesc tdRhs = null;
            TypeDesc tdLhs = null;

            try {
                tdLhs = Program.TypeMap[lhs];
            }
            catch {
                ;
            }

            try {
                tdRhs = Program.TypeMap[rhs];
            }
            catch {
                ;
            }

            if (tdLhs == null) {
                if (tdRhs == null) {
                    // Neither item is typed - this is currently ok.
                    //  Could potentially not do the copy in this case
                }
                else {
                    if (!Program.FQuiet) {
                        Console.WriteLine("Warning: {0}({1}): Copying from typed {3} to untyped {2} ", Program.FileName, iLine, lhs, rhs);
                    }
                    Program.CWarnings += 1;
                }
            }
            else if (tdRhs == null) {
                if (!Program.FQuiet) {
                    Console.WriteLine("Warning: {0}({1}): Coping from untyped {3} to typed {2}", Program.FileName, iLine, lhs, rhs);
                }
                Program.CWarnings += 1;
            }
            else {
                if (tdLhs.typeName != tdRhs.typeName) {
                    if (!Program.FQuiet) {
                        Console.WriteLine("Warning: {0}({1}): Copying from {3} to {2} but types are different {2}, {3}", Program.FileName, iLine, lhs, rhs, tdLhs.typeName, tdRhs.typeName);
                    }
                    Program.CWarnings += 1;
                }
            }
        }

        public void WriteRule(StreamWriter stream)
        {
            int i = 0;

            stream.Write("\t\t\tnew Rule(RuleNo." + m_lhs + ", new RuleStep[] { ");
            foreach (RuleElement re in m_rhs.list) {
                if ((i%5) == 0) stream.Write("\n\t\t\t\t");
                i += 1;
                re.WriteRule(stream);
            }
            stream.Write(" new RuleStep(RuleType.ActionFinal)");
            stream.WriteLine("}),");
        }
    }
}

/*
 *  Copyright 2008 Jim Schaad and Paul Hoffman. All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  
 *  1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  
 *  2. Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *  
 *  This software is provided by Jim Schaad and Paul Hoffman "as is" and
 *  any express or implied warranties, including, but not limited to, the
 *  implied warranties of merchantability and fitness for a particular
 *  purpose are disclaimed. In no event shall Jim Schaad and Paul Hoffman
 *  or contributors be liable for any direct, indirect, incidental,
 *  special, exemplary, or consequential damages (including, but not
 *  limited to, procurement of substitute goods or services; loss of use,
 *  data, or profits; or business interruption) however caused and on any
 *  theory of liability, whether in contract, strict liability, or tort
 *  (including negligence or otherwise) arising in any way out of the use
 *  of this software, even if advised of the possibility of such damage.
 *  
 */
