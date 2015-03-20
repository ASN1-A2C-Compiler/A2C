using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;
using System.Diagnostics;

namespace asn_compile_cs
{
    [Flags] public enum AF_FLAGS { None = 0, PrintErrors = 1 };

    /// <summary>
    /// MatchResult is the return code for attempting to match a single rule.
    /// <list type="table">
    /// <item>Unknown</item><description>Result has not been determined.</description>
    /// <item>Success</item><description>Rule was successfully matched.</description>
    /// <item>Fail</item><description>Rule was not matched.</description>
    /// <item>ChoiceLeft</item><description>Rule was successfully matched, but there are other possible matches left.</description>
    /// <item>ChoiceEnd</item><description>Used internally and converts to ChoiceEnd</description>
    /// <item>Progress</item><description>Returned from AssignmentList if some, but not all, items were reduced</description>
    /// </list>
    /// </summary>
    public enum MatchResult { Unknown, Success, Fail, ChoiceLeft, ChoiceEnd, Progress, ExChoiceEnd, NoProgress, Finished, Reduced };

    /// <summary>
    /// <para>A successful parsing history is kept as a tree of history items, 
    /// each history item matching one item in every rule matched.  During the parsing the tree is expanded and
    /// contracted as a search for a successful match is done.</para>
    /// </summary>
    class History
    {
        RuleNo m_iRule;
        MatchResult m_mr;
        RuleValue m_rv;

        /// <summary>
        /// m_children is the set of history elements for the current item
        /// </summary>
        /// 
        HistoryStack m_children;
        int m_iToken;
        int m_cTokens;
        int m_iItem;
        RuleType m_choice;
        int m_cHistory;                     // number of following history items related to this item.

        /// <summary>
        /// Create a History item
        /// </summary>
        /// <param name="rule">the rule number being processed</param>
        /// <param name="iItem">index into the rule for the item being matched</param>
        public History(RuleNo rule, int iItem /*, int iLevel, int iToken */)
        {
            m_iRule = rule;
            m_mr = MatchResult.Unknown;
            m_iItem = iItem;
            m_cHistory = 0;
            m_rv = null;
        }

        /// <summary>
        /// children returns the number of history elements.  Only relevent if current item is rule
        /// </summary>
        public HistoryStack children
        {
            get { return m_children; }
            set { m_children = value; }
        }

        /// <summary>
        /// contains the RuleType for this item
        /// </summary>
        public RuleType choice
        {
            get { return m_choice; }
            set { m_choice = value; }
        }

        /// <summary>
        /// returns the number of following history items assiocated with this item.
        /// </summary>
        public int cList
        {
            get { return m_cHistory; }
            set { m_cHistory = value; }
        }

        /// <summary>
        /// Contains the number of tokens consumed by this item
        /// </summary>
        public int cTokens
        {
            get { return m_cTokens; }
            set { m_cTokens = value; }
        }

        /// <summary>
        /// contains the index of the item in the parent rule
        /// </summary>
        public int iItem
        {
            get { return m_iItem; }
        }

        /// <summary>
        /// contains the starting token matched by this rule
        /// </summary>
        public int iToken
        {
            get { return m_iToken; }
            set { m_iToken = value; }
        }

        public int iTokenLast { get { return m_iToken + m_cTokens - 1; } }

        /// <summary>
        /// contains the matching result for this item
        /// </summary>
        public MatchResult result
        {
            get { return m_mr; }
            set { m_mr = value; }
        }

        /// <summary>
        /// contains the rule number for this item
        /// </summary>
        public RuleNo rule 
        {
            get { return m_iRule; }
        }

        /// <summary>
        /// contains the resulting value on a successful match
        /// </summary>
        public RuleValue rv
        {
            get { return m_rv; }
            set { m_rv = value; }
        }

        /// <summary>
        /// Textize the item
        /// </summary>
        /// <returns>A text version with the rule number and result.</returns>
        public override string ToString()
        {
            return m_iRule.ToString() + " r:" + m_mr;
        }
    }

    class HistoryStack
    {
        List<History> m_rgHistory = new List<History>();
        Type m_type;
        SymbolList m_fldList;
        SymbolList m_paramList;
        Symbol m_sym;
        int m_index;

        public History this[int i]
        {
            get { return m_rgHistory[i]; }
        }

        public int index { get { return m_index; } set { m_index = value; } }
        public int Count { get { return m_rgHistory.Count; } }

        public SymbolList fieldList
        {
            get { return m_fldList; }
            set { m_fldList = value; }
        }

        public SymbolList paramList
        {
            get { return m_paramList; }
            set { m_paramList = value; }
        }

        public Symbol sym
        {
            get { return m_sym; }
            set { m_sym = value; }
        }

        public Type type
        {
            get { return m_type; }
            set { m_type = value; }
        }

        public int Push(RuleNo rule, int iItem/*, int iLevel, int iItem, MatchResult mr, int iToken*/)
        {
            //  Set variables

            m_rgHistory.Add(new History(rule, iItem/*, iLevel, iItem, mr, iToken*/));

            return m_rgHistory.Count - 1;
        }

        public void Pop()
        {
            m_rgHistory.RemoveAt(m_rgHistory.Count - 1);
        }

        public void PopTo(int idx)
        {
            m_rgHistory.RemoveRange(idx, m_rgHistory.Count - idx);
        }

        public void Remove(int idx, int c)
        {
            m_rgHistory.RemoveRange(idx, c);
        }

        public void SetResult(int index, MatchResult mr)
        {
            m_rgHistory[index].result = mr;
        }

  /*      public void TokensUsed(int index, int cToken)
        {
            m_rgHistory[index].m_cTokensUsed = cToken;
        } */

        public override string ToString()
        {
            if (m_rgHistory.Count == 0) {
                return "Unknown";
            }
            return m_rgHistory[0].rule.ToString() + " " + m_rgHistory.Count.ToString();
        }

    }

    class Context : IEnumerable<HistoryStack>
    {
        Stack<HistoryStack> m_rgHistories = new Stack<HistoryStack>();

        public Symbol FindSymbol(String strName)
        {
            bool f = true;
            //
            //  Walk the context list looking for symbols
            //
            //  Only look at one set of fields
            //  
            //  Assume that the walk from from the top to the bottom of the stack
            //

            foreach (HistoryStack h in m_rgHistories) {
                //
                //

                if (h.sym != null) {
                    if (h.sym.name == strName) return h.sym;
                }

                if (f && (h.fieldList != null)) {
                    foreach (Symbol sym in h.fieldList.list) {
                        if (sym.name == strName) return sym;
                    }
                    f = false;
                }

                if (h.paramList != null) {
                    foreach (Symbol sym in h.paramList.list) {
                        if (sym.name == strName) return sym;
                    }
                }

                if (f && (h.type != null)) {
                    Type type = h.type;
                    bool f1 = true;
                    f = false;

                    //
                    //  Walk down looking for a named list
                    //

                    while (f1) {
                        switch (type.baseType) {
                        case BaseType.Assignment:
                            if (type.subType == null) {
                                f1 = false;
                            }
                            else {
                                type = type.subType;
                            }
                            break;

                        case BaseType.Tagged:
                            type = type.subType;
                            break;

                        default:
                            f1 = false;
                            break;
                        }
                    }
                    
                    if (type.fieldList != null) {
                        foreach (Symbol sym in type.fieldList) {
                            if (sym.name == strName) return sym;
                        }
                    }
                }
            }

            return null;
        }

        public Symbol GetTopSymbol()
        {
            foreach (HistoryStack h in m_rgHistories) {
                if (h.sym != null) {
                    return h.sym;
                }
            }

            return null;
        }

        public Type GetTopType()
        {
            foreach (HistoryStack h in m_rgHistories) {
                if (h.type != null) {
                    return h.type;
                }
            }

            return null;
        }

        public void Push(HistoryStack history)
        {
            m_rgHistories.Push(history);
        }

        public void Pop()
        {
            m_rgHistories.Pop();
        }

        /// <summary>
        /// GetEnumerator returns an IEnumerator&lt;HistoryStack&gt; interface on the history stack in the context.  
        /// Allows for the use of foreach on Contexts.
        /// </summary>
        /// <returns>IEnumerator&lt;HistoryStack&gt; interface</returns>
        public IEnumerator<HistoryStack> GetEnumerator()
        {
            return m_rgHistories.GetEnumerator();
        }

        /// <summary>
        /// GetEnumerator returns an IEnumerator interface on the history stack in the context.  
        /// Allows for the use of foreach on Contexts.
        /// </summary>
        /// <returns>IEnumerator interface</returns>
        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_rgHistories.GetEnumerator();
        }

    }
    
    partial class AsnFile
    {
        //
        //  Statics
        //

        static List<AsnFile> m_lstModules = new List<AsnFile>();               // List of all modules created

        //
        //  Per class items
        //
        
        List<Token> m_rgTokens = new List<Token>();
        String m_strFileName;
        SymbolTable m_symbols = new SymbolTable();
        TagType m_tagTypeDefault = TagType.Explicit;    // Default tagging is explict per X.680 Clause 12.2
        Boolean m_fUseAutomaticTagging = false;         // by default - don't auto tag.
        Boolean m_fExtensibilityImplied = false;        // By default extensibility is not impled per X.680 Clause 12.4
        Context m_context = new Context();
        SymbolList m_symlstImports;
        RuleValue m_rvImports = null;                   // Append errors for imports to this item
        Token m_tknImports = null;                      // Token for all import errors
        SymbolList m_symlstExports;                     // If list is non-empty, then set of exports is restricted
        int iPass = 0;
        String m_strModuleName;
        Value m_valModuleOID;
        AF_FLAGS m_afFlags = AF_FLAGS.None;
        String m_strNamespace = null;
        Boolean m_fEmitThisFile = true;
        Boolean m_fHasImports = false;
        Dictionary<string, string> m_mpszszEmitData;

        //
        //  Setup some default types for this Module
        //

        Type m_DefaultIntegerType;
        Type m_DefaultEnumeratedType;

        //
        //  Constructor for snFile
        //

        public AsnFile(String str, bool fEmit)
        {
            m_DefaultIntegerType = new Type(BaseType.Leaf, new Tagging(TagClass.Universal, TagType.Specified, 2), DataType.Integer, null, "A2C.Integer");
            m_DefaultEnumeratedType = new Type(BaseType.Leaf, new Tagging(TagClass.Universal, TagType.Specified, 10), DataType.Enumerated, null, "A2C.Enumerated");

            m_lstModules.Add(this);

            m_symbols.m_module = this;
            m_strFileName = str;
            m_fEmitThisFile = fEmit;
        }

        //
        //  

        public String fileName { get { return m_strFileName; } }
        public String moduleName { get { return m_strModuleName; } }
        public String nameSpace { get { return m_strNamespace; } set { m_strNamespace = value; } }
        public Boolean NoEmission { get { return !m_fEmitThisFile; } }
        public Boolean EmitSymbols { get { return m_fEmitThisFile; } }
        public Dictionary<string, string> EmitStrings { get { return m_mpszszEmitData; } }


        public bool ToTokens(Lexer lex)
        {
            bool fRet = false;
            int iToken = 0;
            Token tkn;
            bool fSeenBegin = false;

            while ((tkn = lex.yylex()) != null) {
                //
                //  We do some special processing for new line tokens
                //

                if (tkn.tknType == TknType.NewLine) {
                    //
                    //  If not doing EOL hints - then skip this token
                    //

                    if (!Program.args.fEOLHint) continue;

                    //
                    //  If we have not yet seen the BEGIN token - then we ignore it
                    //

                    if (!fSeenBegin) continue;
                }

                //
                //  We need to know when we start seeing the BEGIN tag
                //

                if (tkn.tknType == TknType.BEGIN) fSeenBegin = true;

                tkn.index = iToken;
                tkn.fileName = fileName;
                iToken += 1;
                m_rgTokens.Add(tkn);
                fRet = true;
                if (tkn.tknType == TknType.END) break;
            }

            return fRet;
        }

        //
        //  We are copying forward items if necessary
        //

        public void CopyForward(AsnFile afPrevious)
        {
            //
            //  If we have not set a namespace in this file, then copy it forward
            //  from the previous module.
            //

            if (m_strNamespace == null) {
                if (afPrevious == null) m_strNamespace = "";
                else m_strNamespace = afPrevious.m_strNamespace;
            }

            //
            //  Copy forward the default sizing of integers if not set
            //

            if (m_DefaultIntegerType.emitName == null) {
                if (afPrevious == null) m_DefaultIntegerType.emitName = "A2C_INTEGER_NATIVE";
                else m_DefaultIntegerType.emitName = afPrevious.m_DefaultIntegerType.emitName;
            }

            if (m_DefaultEnumeratedType.emitName == null) {
                if (afPrevious == null) m_DefaultEnumeratedType.emitName = "A2C_ENUMERATED_NATIVE";
                else m_DefaultEnumeratedType.emitName = afPrevious.m_DefaultEnumeratedType.emitName;
            }
        }

        public void Dump()
        {
            m_symbols.Dump();
        }

        public void Print()
        {
            m_symbols.Print();
        }

        public void Print(int count)
        {
            if (count > m_rgTokens.Count) count = m_rgTokens.Count;
            for (int ii = 0; ii < count; ii++) {
                Debug.Print(m_rgTokens[ii].ToString());
            }
        }
        
        public MatchResult Reduce(AF_FLAGS flags)
        {
            int iToken = 0;
            int c = 0;
            MatchResult mr;
            HistoryStack history = new HistoryStack();
            RuleValue rv;

            if (m_rgTokens[0].ruleNo == RuleNo.ModuleDefinition) {
                return MatchResult.Finished;
            }

            //
            //  Stash the flags for later reference
            //

            m_afFlags = flags;

            //
            //  Check any imports that are to be processed
            //

            ProcessImportList();

            //
            //  Try and reduce the module
            //

            mr = MatchRules(RuleNo.ModuleDefinition, 0, iToken, ref history, out c, out rv);

            //
            //  If we succeeded, or we failed for the last time 
            //          - print out all warnings
            //          - print out all errors
            //

            if ((mr == MatchResult.ChoiceLeft) || (mr == MatchResult.Success) ||
                (mr == MatchResult.Fail) && ((m_afFlags & AF_FLAGS.PrintErrors) > 0)) {
                if (rv != null) {
                    if (rv.IsError()) {
                        rv.error.Print();
                    }
                    else if (rv.IsErrorList()) {
                        rv.errorList.Print();
                    }
                    if ((rv.warnList != null) && (rv.warnList.Count > 0)) {
                        rv.warnList.Print();
                    }
                }
            }

            return mr;
        }

        MatchResult MatchRules(RuleNo rule, int iLevel, int iTokenIn, ref HistoryStack history, out int cTokensConsumed, out RuleValue rvOut)
        {
            int cItemsUsed;
            ErrorList errlst = null;
            int iHistory = 0;
            int iItem;
            int iToken = iTokenIn;
            MatchResult mr;
            MatchResult mrResult = MatchResult.Success;
            RuleStep ruleCurrent;
            Rule rules = FindRule(rule);
            // ErrorList errlstForBackTrack = null;

            MatchResult mr2;
            int cTokensUsed;
            Results results;

            cTokensConsumed = 0;

            //   Kill this if the rule is not found

            if (rules == null) {
                cTokensConsumed = 0;
                rvOut = null;
                return MatchResult.Fail;
            }

            //
            //  Check for overrrun
            //

            if (iTokenIn >= m_rgTokens.Count) {
                rvOut = null;
                cTokensConsumed = 0;
                return MatchResult.Fail;
            }

            //
            //  Check for a cached answer
            //

            results = m_rgTokens[iTokenIn].GetResult(rule, iPass);
            if (results != null) {
                rvOut = results.rv;
                cTokensConsumed = results.cTokensUsed;
                return results.mr;
            }

            // Debug.Print("Match Rule: {0}, iLevel: {1}  iTokenIn: {2}", rule, iLevel, iTokenIn);

            rvOut = null;

            //
            //  If it has already been reduced down, then we succeed if it is the correct rule
            //

            if (m_rgTokens[iToken].tknType == TknType.Reduced) {
                cTokensConsumed = 1;
                if (m_rgTokens[iToken].ruleNo == rule) {
                    if (rule == RuleNo.Imports) {
                        rvOut = m_rvImports;
                    }
                    else {
                        rvOut = null;
                    }
                    return MatchResult.Success;
                }
            }

            //
            //  Keep context around in a way that is easy to find.
            //

            m_context.Push(history);

            //
            //  Some rules get special handling, locate those rules and deal them here
            //  Note:  for some it is only pre-processing, while for others we replace all
            //  actions at this point
            //

            switch (rule) {
            case RuleNo.ActualParameterList1:
                mrResult = ActualParameterList1(iTokenIn, iLevel, ref history, out cTokensConsumed, out rvOut);
                m_context.Pop();
                return mrResult;

            case RuleNo.AssignmentList:
                //
                //  Process the rules - if we think it succeeded, then we want to try doing the reduction here
                //
                //  else - return the result up one level.
                //

                mrResult = AssignmentList(iTokenIn);
                if (mrResult != MatchResult.Success) {
                    cTokensConsumed = 1;
                    rvOut = null;
                    m_context.Pop();
                    return mrResult;
                }
                break;

            case RuleNo.BuiltinValue:
                mr = BuiltinValue(iTokenIn, iLevel, ref history, out cTokensConsumed, out rvOut);
                m_context.Pop();
                return mr;

            case RuleNo.word:
                mr = Word(iTokenIn, iLevel, ref history, out cTokensConsumed, out rvOut);
                m_context.Pop();
                return mr;

            case RuleNo.ObjectDefn:
                mr = ObjectDefn(iTokenIn, iLevel, ref history, out cTokensConsumed, out rvOut);
                m_context.Pop();
                return mr;

            case RuleNo.SubtypeElements:
                mr = SubtypeElements(iTokenIn, iLevel, ref history, out cTokensConsumed, out rvOut);
                m_context.Pop();
                return mr;
            }


            //
            //  If First - then setup all of the variables
            //

            if (history.Count == 0) {
                iItem = 0;
            }
            else {
                if (BackTrack(rules, history, ref errlst, out iHistory, out iItem, out iToken) == MatchResult.Fail) {
                    cTokensConsumed = 0;
                    m_context.Pop();
                    return MatchResult.Fail;
                }

                for (int i = 0; i < iHistory; i++) {
                    if (history[i].result == MatchResult.ChoiceLeft) {
                        mrResult = MatchResult.ChoiceLeft;
                        break;
                    }
                }
            }

            //
            //  Loop over all items in this rule
            //

            while (iItem < rules.rgRules.Length) {
                ruleCurrent = rules.rgRules[iItem];

                //
                //  Process the step information
                //

                mr = DoOneStep (rules, iLevel, iItem, iToken, iHistory, ref history, out cTokensUsed, out cItemsUsed);
                cTokensConsumed = cTokensUsed;

                //
                //  Based on the result do something intelegent
                //

                switch (mr) {
                case MatchResult.Success:
                    break;

                case MatchResult.Reduced:
                    rvOut = null;  // We should never care what this is since the rule just got reduced
                    history.Pop ();
                    m_context.Pop ();
                    return MatchResult.Success;

                case MatchResult.ChoiceLeft:
                    mrResult = mr;
                    break;

                case MatchResult.Progress:
                case MatchResult.NoProgress:
                    cTokensConsumed = cTokensUsed;
                    rvOut = history[history.Count - 1].rv;
                    history.Pop ();
                    m_context.Pop ();
                    return mr;

                //
                //  We have finished processing a choice on this level
                //
                //  We have already done the end of rule processing for this rule.
                //
                case MatchResult.ChoiceEnd:
                    cTokensConsumed = cTokensUsed;
                    rvOut = history[history.Count - 1].rv;
                    history.Pop ();
                    m_context.Pop ();
                    return MatchResult.ChoiceLeft;

                case MatchResult.ExChoiceEnd:
                    cTokensConsumed = cTokensUsed;
                    rvOut = history[history.Count - 1].rv;
                    history.Pop ();
                    m_context.Pop ();
                    m_rgTokens[iTokenIn].SetResult (rule, iPass, new Results (MatchResult.Success, rvOut, cTokensConsumed));
                    return MatchResult.Success;

                case MatchResult.Fail:
                    //
                    //  On Failure -
                    //     See if we have someplace to backtrack to.  If there is then
                    //     we continue again from that point.  After there return from BackTrack
                    //     iHistory, iItem and iToken will have the correct values so we skip the
                    //     adjustment code at the bottom of this loop.
                    //
                    //     If there is no place to backtrack to - then we take the error list returned
                    //      and pop it back up.
                    //

                    if (BackTrack (rules, history, ref errlst, out iHistory, out iItem, out iToken) == MatchResult.Fail) {
                        cTokensConsumed = 0;
                        if (errlst != null) {
                            rvOut = new RuleValue (errlst);
                        }
                        m_context.Pop ();
                        m_rgTokens[iTokenIn].SetResult (rule, iPass, new Results (mr, rvOut, cTokensConsumed));
                        return mr;
                    }
                    continue;
                }

                iToken += cTokensUsed;
                iItem += cItemsUsed;
                iHistory = history.Count;
            }

            //
            //  Perform action?
            //


            ///  Need to get cTokensConsumed
            ///  Need to build errlst
            ///  

            if (mrResult == MatchResult.Fail) {
                // M00BUG - need to merge in errors from the Action as well
                if (errlst != null) {
                    rvOut = new RuleValue(errlst);
                }
                cTokensConsumed = 0;
            }
            else {
                rvOut = history[history.Count - 1].rv;
            }

            //
            //  Based on the results of all of the items we have found, return a result
            //

#if false
            if (mrResult == MatchResult.Success) {
                m_rgTokens[iTokenIn].SetResult(rule, iPass, new Results(mrResult, rvOut, cTokensConsumed));
            }
#endif

            m_context.Pop();
            return mrResult;
        }

        MatchResult DoOneStep(Rule rules, int iLevel, int iItem, int iToken, int iHistory, ref HistoryStack history, out int cTokensUsed, out int cItemsUsed)
        {
            int c;
            int c1;
            int c2;
            Boolean fReEnter = false;
            HistoryStack history2;
            MatchResult mr;
            MatchResult mr2;
            MatchResult mr3;
            RuleNo rn;
            RuleStep ruleCurrent;
            RuleValue rv;
            int iHistory2;

            //
            //  Get the details on the current step based on the item number
            //

            ruleCurrent = rules.rgRules[iItem];
            cItemsUsed = 1;

            //
            //  If necessary - push an item onto the history stack for what we are matching
            //      - the only reason not to do it is if we have backtracked and are going back
            //      down into the history.
            //
            //      This could be a bit fun for repeaters.
            //

            if (iHistory == history.Count) {
                history.Push(rules.ruleNo, iItem);
                history[iHistory].choice = ruleCurrent.ruleType;
                history[iHistory].iToken = iToken;
            }
            else {
                fReEnter = true;
            }

            //
            //  Based on the content of the rule, do the correct processing.
            //

            switch (ruleCurrent.ruleType) {
            case RuleType.Rule:
                if (history[iHistory].children == null) {
                    history2 = new HistoryStack();
                    history[iHistory].children = history2;
                }
                else {
                    history2 = history[iHistory].children;
                }
                mr = MatchRules(ruleCurrent.ruleNo, iLevel + 1, iToken, ref history2, out cTokensUsed, out rv);
                break;

                //
                //  We are trying to match an explicit token.  
                //  On success - return the token
                //  On Failure - return the error.
                //
            case RuleType.Token:
                if (iToken >= m_rgTokens.Count) {
                    mr = MatchResult.Fail;
                    cTokensUsed = 0;
                    rv = null;
                }
                else if (ruleCurrent.tknType == TknType.ANY) {
                    mr = MatchResult.Success;
                    cTokensUsed = 1;
                    rv = new RuleValue(m_rgTokens[iToken]);
                }
                else if (m_rgTokens[iToken].tknType == ruleCurrent.tknType) {
                    mr = MatchResult.Success;
                    cTokensUsed = 1;
                    rv = new RuleValue(m_rgTokens[iToken]);
                }
                else {
                    Error err = new Error(ErrorNumber.ExpectedToken, m_rgTokens[iToken]);
                    err.AddObject(ruleCurrent);
                    mr = MatchResult.Fail;
                    cTokensUsed = 0;
                    rv = new RuleValue(err);
                }
                break;

            case RuleType.Choice0:
            case RuleType.Choice1:
            case RuleType.Choice2:
            case RuleType.Choice3:
            case RuleType.Choice4:
            case RuleType.Choice5:
            case RuleType.Choice6:
            case RuleType.Choice7:
            case RuleType.Choice8:
            case RuleType.Choice9:
            case RuleType.Choice10:
            case RuleType.Choice11:
            case RuleType.Choice12:
            case RuleType.Choice13:
            case RuleType.Choice14:
            case RuleType.Choice15:
            case RuleType.Choice16:
            case RuleType.Choice17:
            case RuleType.Choice18:
            case RuleType.Choice19:
            case RuleType.Choice20:
            case RuleType.Choice21:
                cTokensUsed = 0;

                if (history.Count == 1) {
                    //
                    //  First choice on the list we just pass over.
                    //

                    mr = MatchResult.Success;
                    rv = null;
                }
                else {
                    //
                    //  We have actually finished a choice, so check if we successfully do the
                    //  evaluation on it.  If this fails, then return that.  If it succeeds,
                    //  then we return the ChoiceLeft option.
                    //

                    mr = PerformAction(rules.ruleNo, RuleType.ActionFinal, out cTokensUsed, history, out rv);
                    if (mr != MatchResult.Fail) {
                        mr = MatchResult.ChoiceEnd;
                    }
                }
                break;

            case RuleType.ExChoice0:
            case RuleType.ExChoice1:
            case RuleType.ExChoice2:
            case RuleType.ExChoice3:
            case RuleType.ExChoice4:
            case RuleType.ExChoice5:
            case RuleType.ExChoice6:
            case RuleType.ExChoice7:
            case RuleType.ExChoice8:
            case RuleType.ExChoice9:
            case RuleType.ExChoice10:
            case RuleType.ExChoice11:
            case RuleType.ExChoice12:
            case RuleType.ExChoice13:
            case RuleType.ExChoice14:
            case RuleType.ExChoice15:
            case RuleType.ExChoice16:
            case RuleType.ExChoice17:
            case RuleType.ExChoice18:
            case RuleType.ExChoice19:
            case RuleType.ExChoice20:
            case RuleType.ExChoice21:
            case RuleType.ExChoice22:
            case RuleType.ExChoice23:
            case RuleType.ExChoice24:
            case RuleType.ExChoice25:
            case RuleType.ExChoice26:
            case RuleType.ExChoice27:
                cTokensUsed = 0;

                if (history.Count == 1) {
                    //
                    //  First excluive choice on the list we just pass over.
                    //

                    mr = MatchResult.Success;
                    rv = null;
                }
                else {
                    //
                    //  We have actually finished a choice, so check if we successfully do the
                    //  evaluation on it.  If this fails, then return that.  If it succeeds,
                    //  then we return the Success.
                    //

                    mr = PerformAction(rules.ruleNo, RuleType.ActionFinal, out cTokensUsed, history, out rv);
                    if (mr != MatchResult.Fail) {
                        mr = MatchResult.ExChoiceEnd;
                    }
                }
                break;

            case RuleType.Action0:
            case RuleType.Action1:
            case RuleType.Action2:
            case RuleType.Action3:
            case RuleType.Action4:
            case RuleType.Action5:
            case RuleType.Action6:
                history[iHistory].choice = ruleCurrent.ruleType;
 
                mr = PerformAction(rules.ruleNo, ruleCurrent.ruleType, out cTokensUsed, history, out rv);

                cTokensUsed = 0; 
                break;

            case RuleType.Empty:
                cTokensUsed = 0;
                rv = null;
                mr = MatchResult.Success;
                break;

                //
                //  Repeat the one item following. There must be at least one occurence for success.
                //

            case RuleType.Plus1:
                if (fReEnter) Debug.Assert(false, "Re-enter Plus1");
                iHistory2 = iHistory + 1;
                mr = DoOneStep(rules, iLevel, iItem + 1, iToken, iHistory2, ref history, out cTokensUsed, out c2);
                Debug.Assert(c2 == 1);
                if (mr != MatchResult.Fail) {
                    //
                    //  Loop until failure occurs
                    //

                    while (true) {
                        iHistory2 += 1;
                        mr2 = DoOneStep(rules, iLevel, iItem + 1, iToken + cTokensUsed, iHistory2, ref history, out c, out c2);
                        Debug.Assert(c2 == 1);
                        if (mr2 == MatchResult.Fail) {
                            history.Pop();
                            break;
                        }
                        else if (mr2 == MatchResult.ChoiceLeft) {
                            mr = mr2;
                        }
                        cTokensUsed += c;
                    }
                }
                rv = null;
                cItemsUsed = 2;
                break;

                //
                //  Repeat the two items following.
                //          rule = A B +
                //      First item (A) must be present
                //      If second item (B) is present, it must be followed by the first item again
                //
                //   i.e. Legal matches are A, A B A, A B A B A, ...
                //
            case RuleType.Plus2:
                if (fReEnter) {
                    int ii;
                    //
                    //  We are in the middle of a backtrack operation.  Need to determine where we are re-starting at.
                    //

                    Debug.Assert(history[iHistory].choice == RuleType.Plus2);


                    //
                    //  How many tokens have we consumed
                    //

                    for (cTokensUsed = 0, ii = iHistory+1, mr = MatchResult.Success; ii < history.Count-1; ii++) {
                        cTokensUsed += history[ii].cTokens;
                        if (history[ii].result == MatchResult.ChoiceLeft) {
                            mr = MatchResult.ChoiceLeft;
                        }
                    }
                    Debug.Assert(history[ii].result == MatchResult.ChoiceLeft);
                    iHistory2 = history.Count - 1;
                    iToken = history[iHistory2].iToken;

                    //  Is this the odd number item?
                    //      difference between iHistory and history.count == even
                    //          then start with a B check
                    //      difference is odd
                    //          then start with an A check
                    //

                    if ((history.Count - iHistory) % 2 == 0) {
                        mr2 = DoOneStep(rules, iLevel, iItem + 1, iToken, iHistory2, ref history, out c1, out c2);
                        Debug.Assert(c2 == 1);
                        if (mr2 == MatchResult.Fail) {
                            if (iHistory2 - iHistory == 1) {
                                mr = MatchResult.Fail;
                            }
                            else {
                                history.Pop();
                                history.Pop();
                            }
                            goto plus1Skip;
                        }
                        cTokensUsed += c1;
                        if (mr2 == MatchResult.ChoiceLeft) {
                            mr = MatchResult.ChoiceLeft;
                        }

                        iHistory2 += 1;
                    }

                    while (true) {
                        mr2 = DoOneStep(rules, iLevel, iItem + 2, iToken, iHistory2, ref history, out c, out c2);
                        Debug.Assert(c2 == 1);
                        if (mr2 == MatchResult.Fail) {
                            
                            history.Pop();  // mr2
                            break;
                        }

                        iHistory2 += 1;
                        mr3 = DoOneStep(rules, iLevel, iItem + 1, iToken + cTokensUsed + c, iHistory2, ref history, out c1, out c2);
                        if (mr3 == MatchResult.Fail) {
                            history.Pop();  // mr3
                            history.Pop();  // mr2
                            mr = mr3;
                            break;
                        }

                        cTokensUsed += c + c1;
                        if ((mr2 == MatchResult.ChoiceLeft) || (mr3 == MatchResult.ChoiceLeft)) {
                            mr = MatchResult.ChoiceLeft;
                        }
                    }

                plus1Skip:
                    cItemsUsed = 3;
                    rv = null;
                }
                else {
                    iHistory2 = iHistory + 1;
                    mr = DoOneStep(rules, iLevel, iItem + 1, iToken, iHistory2, ref history, out cTokensUsed, out c2);
                    Debug.Assert(c2 == 1);
                    if (mr != MatchResult.Fail) {
                        //
                        //  Loop until failure occurs
                        //

                        while (true) {
                            iHistory2 += 1;
                            mr2 = DoOneStep(rules, iLevel, iItem + 2, iToken + cTokensUsed, iHistory2, ref history, out c, out c2);
                            Debug.Assert(c2 == 1);
                            if (mr2 == MatchResult.Fail) {
                                history.Pop();
                                break;
                            }

                            iHistory2 += 1;
                            mr3 = DoOneStep(rules, iLevel, iItem + 1, iToken + cTokensUsed + c, iHistory2, ref history, out c1, out c2);
                            Debug.Assert(c2 == 1);
                            if (mr3 == MatchResult.Fail) {
                                history.Pop();  // mee
                                history.Pop();  // last one as well
                                // mr = mr3;
                                break;
                            }

                            cTokensUsed += c + c1;
                            if ((mr2 == MatchResult.ChoiceLeft) || (mr3 == MatchResult.ChoiceLeft)) {
                                mr = MatchResult.ChoiceLeft;
                            }
                        }
                    }
                    cItemsUsed = 3;
                    rv = null;
                }
                break;

            case RuleType.Star1:
                iHistory2 = iHistory;
                cTokensUsed = 0;

                //
                //  Loop until failure occurs
                //

                mr = MatchResult.Success;

                while (true) {
                    iHistory2 += 1;

                    mr2 = DoOneStep(rules, iLevel, iItem + 1, iToken + cTokensUsed, iHistory2, ref history, out c, out c2);
                    Debug.Assert(c2 == 1);
                    if (mr2 == MatchResult.Fail) {
                        history.Pop();
                        break;
                    }
                    else if (mr2 == MatchResult.ChoiceLeft) {
                        mr = mr2;
                    }
                    cTokensUsed += c;
                }

                rv = null;
                cItemsUsed = 2;
                break;

                //
                //  Next token in line is an optional one
                //

            case RuleType.Optional1:
                if (fReEnter) {
                    //Debug.Assert(false, "Re-enter Optional1");
                    if (history.Count == iHistory+1) {
                        mr = MatchResult.Success;
                        cTokensUsed = 0;
                        history.Push(rules.ruleNo, iItem + 1);
                        history[iHistory + 1].result = MatchResult.Fail;
                    }
                    else {
                        mr = DoOneStep(rules, iLevel, iItem + 1, iToken, iHistory + 1, ref history, out cTokensUsed, out c2);
                        Debug.Assert(c2 == 1);
                        if (mr == MatchResult.Fail) {
                            mr = MatchResult.Success;
                            cTokensUsed = 0;
                            history[iHistory + 1].children = null;
                        }
                        else {
                            if ((mr != MatchResult.Progress) && (mr != MatchResult.NoProgress)) {
                                //
                                //  Could do without the optional - so say so.
                                //
                                mr = MatchResult.ChoiceLeft;
                            }
                        }
                    }
                    rv = null;              // No return value
                    cItemsUsed = 2;
                }
                else {
                    mr = DoOneStep(rules, iLevel, iItem + 1, iToken, iHistory + 1, ref history, out cTokensUsed, out c2);
                    Debug.Assert(c2 == 1);
                    if (mr == MatchResult.Fail) {
                        //
                        //  The Optional will succeed, but the next item fails.
                        //

                        mr = MatchResult.Success;
                        cTokensUsed = 0;
                        history[iHistory + 1].children = null;
                    }
                    else {
                        if ((mr != MatchResult.Progress) && (mr != MatchResult.NoProgress)) {
                            //
                            //  Could do without the optional - so say so.
                            //
                            mr = MatchResult.ChoiceLeft;
                        }
                    }
                    rv = null;              // No return value
                    cItemsUsed = 2;
                }
                break;

                //
                //  We only return success/failure for this item
                //  It does not advance the pointer for tokens
                //  It does not save history for what is beyond it.
                //

            case RuleType.Not:
                history2 = new HistoryStack();

                mr = DoOneStep(rules, iLevel, iItem + 1, iToken, 0, ref history2, out cTokensUsed, out c2);
                Debug.Assert(c2 == 1);
                if (mr == MatchResult.Fail) {
                    mr = MatchResult.Success;
                }
                else {
                    mr = MatchResult.Fail;
                }
                //
                //  Push a dummy element onto the stack for the !a
                //

                history.Push(rules.ruleNo, iItem);
                history[iHistory + 1].choice = ruleCurrent.ruleType;
                history[iHistory + 1].iToken = iToken;
                history[iHistory + 1].result = MatchResult.Success;

                //
                //  Setup the return
                //

                rv = null;
                cItemsUsed = 2;
                cTokensUsed = 0;
                break;

            case RuleType.String:
                if (((m_rgTokens[iToken].tknType == TknType.L_Identifier) || (m_rgTokens[iToken].tknType == TknType.U_Identifier)) &&
                    (m_rgTokens[iToken].name == ruleCurrent.str)) {
                    mr = MatchResult.Success;
                    cTokensUsed = 1;
                    rv = new RuleValue(m_rgTokens[iToken]);
                }
                else {
                    rv = null;
                    mr = MatchResult.Fail;
                    cTokensUsed = 0;
                }
                break;      
          
            case RuleType.Symbol:
                //
                if (history[iHistory].children == null) {
                    history2 = new HistoryStack();
                    history[iHistory].children = history2;
                }
                else {
                    history2 = history[iHistory].children;
                }
                if (ruleCurrent.sym.IsType(SymbolType.ValueField)) {
                    rn = RuleNo.Value;
                    history.type = ruleCurrent.sym.type;
                }
                else if (ruleCurrent.sym.IsType(SymbolType.TypeField)) {
                    rn = RuleNo.Type;
                }
                else if (ruleCurrent.sym.symType == SymbolType.ObjectField) {
                    rn = RuleNo.Object;
                    history.type = ruleCurrent.sym.type;
                }
                else if (ruleCurrent.sym.symType == SymbolType.ObjectSetField) {
                    rn = RuleNo.ObjectSet;
                    history.type = ruleCurrent.sym.type;
                }
                else {
                    rv = null;
                    cTokensUsed = 0;
                    return MatchResult.Fail;
                }

                mr = MatchRules(rn, iLevel + 1, iToken, ref history2, out cTokensUsed, out rv);
                break;

            case RuleType.UScore:
                if (m_rgTokens[iToken].LeadingWhiteSpace) {
                    rv = null;
                    mr = MatchResult.Fail;
                    cTokensUsed = 0;
                }
                else {
                    rv = null;
                    mr = MatchResult.Success;
                    cTokensUsed = 0;
                }
                break;

            case RuleType.ActionFinal:
                mr = PerformAction(rules.ruleNo, RuleType.ActionFinal, out cTokensUsed, history, out rv);
                break;


            default:
                cTokensUsed = 0;
                rv = null;
                mr = MatchResult.Fail;
                break;
            }

            //
            //  Save the results into the history stack
            //

            history[iHistory].result = mr;
            history[iHistory].rv = rv;
            history[iHistory].cTokens = cTokensUsed;
            history[iHistory].cList = history.Count - iHistory;

            return mr;
        }

        Rule FindRule(RuleNo ruleNo)
        {
            for (int ii=0; ii < Rules.m_rules.Length; ii++) {
                if (Rules.m_rules[ii].ruleNo == ruleNo) return Rules.m_rules[ii];
            }

            for (int ii = 0; ii < Rules.m_rgRules2.Count; ii++) {
                if (Rules.m_rgRules2[ii].ruleNo == ruleNo) return Rules.m_rgRules2[ii];
            }
            
            Debug.Print("Rule: " + ruleNo.ToString() + " not found");
            return null;
        }

        Symbol FindSymbol(String symName)
        {
            Symbol sym;

            //
            //  Look over the contexts looking for the symbol
            //

            sym = m_context.FindSymbol(symName);
            if (sym == null) {
                //
                //  Look for symbol in symbol table
                //

                sym = m_symbols.Find(symName);

                //
                // Symbol was not found
                //

                if (sym == null) return sym;
            }

            //
            //  In some cases we might want to look for a different symbol.
            //

            //
            //  Follow links in the symbol table if this is an imported symbol
            //

            while ((sym.symType & SymbolType.Imported) != 0) {
                if (sym.m_sym == null) return sym;
                sym = sym.m_sym;
            }

            return sym;
        }

        Symbol GetExternalReference(String szModuleName, String szSymbolName)
        {
            //
            //  We need to find the module we are looking at.  
            //  If IMPORTS are provided then look into the import list 
            //  If IMPORTS are not provided - look in the global list
            //

            if (m_fHasImports) {
                Symbol symModule = FindSymbol(szModuleName);

                //
                //  If we don't find the symbol or it is not a module, then return an error.
                //

                if ((symModule == null) || 
                    ((symModule.symType != SymbolType.Module) && (symModule.symType != (SymbolType.Module|SymbolType.Imported)))) {
                    return null;
                }

                //
                //  We found the module.  See if the symbol we are looking for is on the list
                //  of imports.
                //

                foreach (Symbol sym in symModule.symList) {
                    if (sym.name == szSymbolName) {
                        //if ((sym.symType & SymbolType.BaseMask) == SymbolType.Unknown) {
                            //
                            //  Symbol is not yet sufficently defined to be of use
                            //

                        //    return null;
                        //}

                        //
                        //  Jump to the real symbol
                        //

                        Symbol sym2 = sym;
                        while ((sym2.symType & SymbolType.Imported) != 0) {
                            sym2 = sym2.m_sym;
                            if (sym2 == null) return null;
                        }

                        return sym2;
                    }
                }

                //
                //  Symbol is not found - this is an error.
                //
            }
            else {
                //
                //  Look for a matching module name in the list of all modules we know about
                //

                foreach (AsnFile af in m_lstModules) {
                    if (af.m_strModuleName == szModuleName) {
                        //
                        //  We found a module name, see if the symbol exists.
                        //

                        Symbol sym = af.FindSymbol(szSymbolName);
                        if ((sym == null) || ((sym.symType & SymbolType.BaseMask) == SymbolType.Unknown)) {
                            return null;
                        }

                        //
                        //  Was the symbol marked as for export?
                        //

                        if (af.m_symlstExports != null) {
                            foreach (Symbol symExport in af.m_symlstExports) {
                                if (symExport.name == sym.name) {
                                    // M00BUG - skip past imports?
                                    return sym;
                                }
                            }
                            //
                            //  Symbol is not marked for export!
                            //

                            return null;
                        }

                        //
                        //  Jump past any imports
                        //

                        while ((sym.symType & SymbolType.Imported) != 0) {
                            sym = sym.m_sym;
                        }

                        return sym;
                    }
                }

                //
                //  We did not find a module with the correct name, this is an erro
                //
            }

            return null;
        }

        void ReplaceTokens(int iTokenBase, int cTokens, Token tknNew)
        {
            int i;

            //  Skip over the first set of line breaks

            while (m_rgTokens[iTokenBase].tknType == TknType.LineBreak) {
                iTokenBase += 1;
            }

            //  Insert the new token here

            m_rgTokens.Insert(iTokenBase, tknNew);
            iTokenBase += 1;

            //  Now delete the next cTokens items

            for (i = 0; i < cTokens; i++) {
                while (m_rgTokens[iTokenBase].tknType == TknType.LineBreak) {
                    iTokenBase += 1;
                }
                m_rgTokens.RemoveAt(iTokenBase);
            }
        }

        MatchResult ActualParameterList1(int iTokenIn, int iLevel, ref HistoryStack history, out int cTokensConsumed, out RuleValue rvOut)
        {
            int i;
            int iParen;
            int iParam;
            SymbolList paramList = null;
            int[,] rgi;
            
            //
            //  Get the list of parameters to be matched
            //
            
            foreach(HistoryStack history1 in m_context) {
                if (history1.paramList != null) {
                    paramList = history1.paramList;
                    break;
                }
            }

            //
            //  Start by getting a token count for each parameter we are going to be parsing
            //

            rgi = new int[paramList.Count, 2];
            iParam = 0;
            rgi[0, 0] = iTokenIn;

            for (i = iTokenIn, iParen = 0; i < m_rgTokens.Count; i++) {
                if ((m_rgTokens[i].tknType == TknType.Comma) && (iParen == 0)) {
                    if (iParam + 1 == paramList.Count) {
                        cTokensConsumed = 0;
                        rvOut = null;
                        // M00ERROR - # parameters is incorrect
                        return MatchResult.Fail;
                    }
                    rgi[iParam + 1, 0] = i + 1;
                    rgi[iParam, 1] = i - rgi[iParam, 0];
                    iParam += 1;
                }
                else if ((m_rgTokens[i].tknType == TknType.LBrace) || (m_rgTokens[i].tknType == TknType.LParen)) {
                    iParen += 1;
                }
                else if ((m_rgTokens[i].tknType == TknType.RBrace) || (m_rgTokens[i].tknType == TknType.RParen)) {
                    iParen -= 1;
                    if (iParen == -1) {
                        rgi[iParam, 1] = i - rgi[iParam, 0];
                        iParam += 1;
                        break;
                    }
                }
            }
            
            //
            //  Compute number of tokens consumed on success
            //

            cTokensConsumed = i - iTokenIn;

            //
            //  Walk the parameters in order attempting to evaluate them.
            //  M00BUG - This order needs to be update in order to deal with cases
            //      where the type is defined in the parameter list AFTER the value!!!
            //

            int cTokens;
            HistoryStack history2;
            MatchResult mr;
            RuleValue rv;
            Symbol sym;
            SymbolList symList = new SymbolList();


            for (i = 0; i < paramList.Count; i++) {
                history.Push(RuleNo.ActualParameterList1, i);
                while (true) {
                    if (history[0].children == null) {
                        history2 = new HistoryStack();
                        history[0].children = history2;
                    }
                    else {
                        history2 = history[0].children;
                    }

                    switch (paramList[i].symType) {
                    case SymbolType.ValueParameter:
                        history2.type = paramList[i].type;

                        mr = MatchRules(RuleNo.Value, iLevel + 1, rgi[i, 0], ref history2, out cTokens, out rv);

                        if (mr != MatchResult.Fail) {
                            sym = new Symbol(paramList[i].name, SymbolType.Value);
                            sym.value = rv.value;
                            sym.Collapse();
                        }
                        else {
                            sym = null;
                        }
                        break;

                    case SymbolType.TypeParameter:

                        mr = MatchRules(RuleNo.Type, iLevel + 1, rgi[i, 0], ref history2, out cTokens, out rv);

                        if (mr != MatchResult.Fail) {
                            sym = new Symbol(paramList[i].name, SymbolType.Type);
                            sym.type = rv.type;
                            sym.Collapse();
                        }
                        else {
                            sym = null;
                        }
                        break;

                    case SymbolType.ObjectClassParameter:

                        mr = MatchRules(RuleNo.DefinedObjectClass, iLevel + 1, rgi[i, 0], ref history2, out cTokens, out rv);

                        if (mr != MatchResult.Fail) {
                            sym = new Symbol(paramList[i].name, SymbolType.ObjectClass);
                            sym.type = rv.type;
                            sym.Collapse();
                        }
                        else {
                            sym = null;
                        }
                        break;


                    case SymbolType.ObjectSetParameter:

                        history2.type = paramList[i].type;

                        if ((history2.type.baseType == BaseType.Symbol) &&
                            (history2.type.sym.symType == SymbolType.ObjectClassParameter)) {
                            foreach (Symbol symT in symList) {
                                if (symT.name == history2.type.sym.name) {
                                    history2.type = symT.type;
                                    break;
                                }
                            }
                        }
                        
                        mr = MatchRules(RuleNo.ObjectSet, iLevel + 1, rgi[i, 0], ref history2, out cTokens, out rv);

                        if (mr != MatchResult.Fail) {
                            sym = new Symbol(paramList[i].name, SymbolType.ObjectSet);
                            sym.value = rv.value;
                        }
                        else {
                            sym = null;
                        }
                        break;

                    default:
                        Debug.Assert(false);
                        cTokens = 0;
                        sym = null;
                        mr = MatchResult.Fail;
                        break;
                    }

                    //
                    //  I need to get here with the following:
                    //      1. a success code and
                    //      2. all of the tokens are consumed (no more, no less)
                    //
                    //   Recycle on a ChoiceLeft if not all tokens are consumed.

                    if (mr == MatchResult.Fail) {
                        history.Pop();
                        rvOut = null;
                        return mr;
                    }
                    if (cTokens == rgi[i, 1]) {
                        symList.Add(sym);
                        break;
                    }
                    if (mr == MatchResult.Success) {
                        //  The parser succeeded, but did not use all of the tokens - this
                        //  maps to a big failure.
                        history.Pop();
                        rvOut = null;
                        return MatchResult.Fail;
                    }
                }
                history.Pop();
            }
  

            rvOut = new RuleValue(symList);

            return MatchResult.Success;
        }

        MatchResult AssignmentList(int iTokenIn)
        {
            int c;
            int iToken = iTokenIn;
            int iTokenStart;
            Boolean fProgressInPass = true;
            Boolean fProgressInBody = false;
            Boolean fUnusedDefineFound = false;
            MatchResult mr;
            RuleValue rv;
            HistoryStack history;
            int iPassCount = 0;

            //
            //  Process the Assignment List rules
            //

            iTokenStart = iToken;
            history = new HistoryStack();

            while (fProgressInPass && (iPassCount < 3)) {
                iPassCount++;

                fProgressInPass = false;
                fUnusedDefineFound = false;
                iPass += 1;

                //
                //  Walk the list and start doing reductions
                //

                for (iToken = iTokenStart; iToken < m_rgTokens.Count; iToken++) {
                    //
                    //  If token is already reduced, then no work to be done
                    //

                    if (m_rgTokens[iToken].tknType == TknType.Reduced) {
                        continue;
                    }
                    if (m_rgTokens[iToken].tknType == TknType.END) continue;

                    //
                    //  Check if we are using the EOL hint for parsing.
                    //
                    if (Program.args.fEOLHint) {
                        //
                        //  If it is not a newline - then continue to skip forward
                        //

                        if (m_rgTokens[iToken].tknType != TknType.NewLine) continue;

                        //
                        //  If the next token is reduced - delete this token
                        //

                        if (m_rgTokens[iToken + 1].tknType == TknType.Reduced) {
                            m_rgTokens.RemoveAt(iToken);
                            iToken -= 1;
                            continue;
                        }

                        //
                        //  Statement should start on the token following the EOL marker
                        //

                        iToken += 1;
                    }

                    //
                    //  Reduce this statement
                    //

                    history.PopTo(0);
                    
                    mr = MatchRules(RuleNo.Assignment, 0, iToken, ref history, out c, out rv);
                    if (mr != MatchResult.Fail) {
                        fProgressInPass = true;
                        fProgressInBody = true;
                        iToken += c - 1;

                        if ((rv != null) && (rv.warnList.Count > 0)) {
                            rv.warnList.Print();
                        }

                        if (m_rgTokens[iToken - 1].tknType == TknType.NewLine) {
                            m_rgTokens.RemoveAt(iToken - 1);
                            iToken -= 1;
                        }
                        continue;
                    }

                    //

                    if ((rv != null) && ((m_afFlags & AF_FLAGS.PrintErrors) > 0)) {
                        if (rv.IsError()) {
                            rv.error.Print();
                        }
                        else if (rv.IsErrorList()) {
                            rv.errorList.Print();
                        }
                    }


                    //
                    //  We failed to reduce that statement.
                    //
                    //  Try and jump forward over this possible statement and then continue
                    //
                    //  We have two ways of jumping over to the next line. The easy way is if end of line hinting is turned
                    //  on - in that case we can just skip to the next new line marker and continue.
                    //  The other way is to try and parse forward to guess where the item ends -- which may or may not
                    //  be a correct guess
                    //

                    if (Program.args.fEOLHint) {
                        //
                        //  The code at the top of the loop will automatically will skip to the next EOL marker
                        //

                        if ((m_afFlags & AF_FLAGS.PrintErrors) != 0) {
                            int iToken1 = iToken-1;

                            while ((m_rgTokens[iToken].tknType != TknType.NewLine) &&
                                    (m_rgTokens[iToken].tknType != TknType.Reduced) &&
                                    (m_rgTokens[iToken].tknType != TknType.END)) {
                                iToken++;
                            }
                            iToken -= 1;
                            ReplaceTokens(iToken1, iToken - iToken1 + 1, new Token(RuleNo.TypeAssignment, 0, 0));
                            iToken = iToken1;
                        }
                        else {
                            fUnusedDefineFound = true;
                        }
                        continue;
                    }

                    //
                    //  1.  Jump over a command if that is what is present
                    //

                    if (m_rgTokens[iToken].tknType == TknType.CMDSTART) {
                        int iToken1 = iToken;
                        
                        while (m_rgTokens[iToken].tknType != TknType.CMDEND) iToken++;

                        if ((m_afFlags & AF_FLAGS.PrintErrors) != 0) {
                            ReplaceTokens(iToken1, iToken - iToken1 + 1, new Token(RuleNo.TypeAssignment, 0, 0));
                            iToken = iToken1;
                        }
                        else {
                            fUnusedDefineFound = true;
                        }
                        continue;
                    }

                    history.PopTo(0);
                    mr = MatchRules(RuleNo.DnnAssignment, 0, iToken, ref history, out c, out rv);
                    if (mr != MatchResult.Fail) {
                        if ((m_afFlags & AF_FLAGS.PrintErrors) != 0) {
                            ReplaceTokens(iToken, c, new Token(RuleNo.TypeAssignment, 0, 0));
                        }
                        else {
                            iToken += c - 1;
                            fUnusedDefineFound = true;
                        }
                        continue;
                    }

                    //  Fatal Error
                    // Debug.Assert(false);
                    Error err = new Error(ErrorNumber.SkippedToken, m_rgTokens[iToken]);
                    err.Print();

                    ReplaceTokens(iToken, 1, new Token(RuleNo.TypeAssignment, 0, 0));

                }
            }

            //
            //  Now figure out what the state of the world is.
            //
            //  We have some unprocessed rules, but we made progress - this is good news, say so.
            //

            if (fUnusedDefineFound && fProgressInBody) {
                return MatchResult.Progress;
            }
            //
            //  We have unprocessed rules, but no progress was made
            //

            if (fUnusedDefineFound) {
                return MatchResult.NoProgress;
            }

            //
            //  We found no unprocessed rules
            //

            return MatchResult.Success;
        }

        MatchResult BuiltinValue(int iTokenIn, int iLevel, ref HistoryStack history, out int cTokensUsed, out RuleValue rvOut)
        {
            HistoryStack history2;
            MatchResult mr;
            Type type = null;
            Type rootType;

            //
            //  We need to do some special coding at this point, specifically we only want to match the
            //  single rule that corresponds to the type at the top of the context stack.
            //

            //
            //  Find out the current type we are looking for.
            //

            rootType = m_context.GetTopType();
            type = rootType.SkipFluff();

            //
            //  If no type is found - oops...
            //  We can get to this point if we have decided that a symbol is a type, but we have
            //  not yet reduced the type to get full details about it.
            //

            if (type == null) {
                rvOut = null;
                cTokensUsed = 0;
                return MatchResult.Fail;
            }

            if (history.Count != 2) {
                history.Push(RuleNo.BuiltinValue, 0);
                history[0].result = MatchResult.Success;
                history.Push(RuleNo.BuiltinValue, 1);
            }

            history2 = history[1].children;
            if (history2 == null) history2 = new HistoryStack();

            //
            //  Based on the base type, use the correct rule
            //

            switch (type.baseType) {
            case BaseType.Leaf:
                switch (type.dataType) {
                case DataType.BitString:
                    mr = MatchRules(RuleNo.BitStringValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.Boolean:
                    mr = MatchRules(RuleNo.BooleanValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.Enumerated:
                    mr = MatchRules(RuleNo.EnumeratedValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.Integer:
                    mr = MatchRules(RuleNo.IntegerValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.Null:
                    mr = MatchRules(RuleNo.NullValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.ObjectIdentifier:
                    mr = MatchRules(RuleNo.ObjectIdentifierValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.OctetString:
                    mr = MatchRules(RuleNo.OctetStringValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                case DataType.String:
                    mr = MatchRules(RuleNo.CharacterStringValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    break;

                default:
                    rvOut = null;
                    cTokensUsed = 0;
                    mr = MatchResult.Fail;
                    Debug.Assert(false, "Unknown built-in type");
                    break;
                }
                break;

            case BaseType.Class:
               // mr = MatchResult(RuleNo.ClassValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                rvOut = null;
                cTokensUsed = 0;
                mr = MatchResult.Fail;
                break;

            case BaseType.RestrictedCharacter:
                mr = MatchRules(RuleNo.RestrictedCharacterStringValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                break;

            case BaseType.Sequence:
                mr = MatchRules(RuleNo.SequenceValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                break;

            case BaseType.SequenceOf:
                mr = MatchRules(RuleNo.SequenceOfValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                break;

            case BaseType.Set:
                mr = MatchRules(RuleNo.SetValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                break;

            case BaseType.SetOf:
                mr = MatchRules(RuleNo.SetOfValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                break;

            case BaseType.Tagged:
                mr = MatchRules(RuleNo.Value, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                break;

            case BaseType.ObjectClassField:
                Symbol symT = type.GetObjectClassField();
                if (symT == null) {
                    Debug.Assert(false);
                    mr = MatchResult.Fail;
                    rvOut = null;
                    cTokensUsed = 0;
                    break;
                }

                if (symT.GetSymbolType() == SymbolType.ValueField) {
                    history.type = symT.type;
                    mr = MatchRules(RuleNo.BuiltinValue, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                }
                else {
                    Debug.Assert(symT.GetSymbolType() == SymbolType.TypeField);
                    /// ValueField.   M00HERE
                    mr = MatchResult.Fail;
                    rvOut = null;
                    cTokensUsed = 0;
                }
                break;

            case BaseType.Symbol:
                //  
                //  If we get here, the only thing that we can accept is a parameter which has this 
                //  symbol as it's type.
                //

                Debug.Assert((type.sym.symType & SymbolType.Parameter) != 0);
                Symbol symX = FindSymbol(m_rgTokens[iTokenIn].name);
                if ((symX.symType & SymbolType.Parameter) != 0) {
                    if ((symX.type.baseType == BaseType.Symbol) &&
                        (symX.type.sym == type.sym)) {
                        rvOut = new RuleValue(new Value(symX));
                        cTokensUsed = 1;
                        mr = MatchResult.Success;
                    }
                    else {
                        rvOut = null;
                        cTokensUsed = 0;
                        return MatchResult.Fail;
                    }
                }
                else {
                    rvOut = null;
                    cTokensUsed = 0;
                    return MatchResult.Fail;

                }
                break;

            default:
                Debug.Assert(false, "Unknown built-in type");
                rvOut = null;
                cTokensUsed = 0;
                return MatchResult.Fail;
            }

            history[1].children = history2;
            history[1].rv = rvOut;
            history[1].cTokens = cTokensUsed;

 
            if (mr != MatchResult.Fail) {
                rvOut.value.type = rootType;        // We needed the base type to parse a value, but we want the real type for emission.
                mr = PerformAction (RuleNo.BuiltinValue, RuleType.ActionFinal, out cTokensUsed, history, out rvOut);
            }

            history[1].result = mr;
            return mr;
        }

        MatchResult SubtypeElements(int iTokenIn, int iLevel, ref HistoryStack history, out int cTokensUsed, out RuleValue rvOut)
        {
            BaseType baseType;
            RuleType choice;
            DataType dataType;
            Boolean f;
            HistoryStack history2;
            MatchResult mr;
            RuleNo ruleNo;
            Type typeTop = m_context.GetTopType();
            
            //
            //  It should be impossible to get to this point without having placed a type on the stack.
            //

            Debug.Assert(typeTop != null);

            typeTop = typeTop.SkipFluff();

            baseType = typeTop.baseType;
            dataType = typeTop.dataType;

            //
            //  Setup for re-tries
            //

            if (history.Count != 2) {
                history.Push(RuleNo.SubtypeElements, 0);
                history.Push(RuleNo.SubtypeElements, 1);
                choice = RuleType.Choice0;
            }
            else {
                choice = history[0].choice + 1;
            }

            history2 = history[1].children;
            if (history2 == null) {
                history2 = new HistoryStack();
                history[1].children = history2;
            }

            //
            //  If this is an ObjectClassField, then we need to update the type for matching
            //  values to that of the field itself.  Note that this can be an open type!
            //

            if (baseType == BaseType.ObjectClassField) {
                Symbol symT = typeTop.GetObjectClassField();
                if (symT == null) {
                    //  Two possible problems:
                    //  1.  We can't resolve because the field name does not exist, or
                    //  2.  We can't resolve because typeTop is a parameter.
                    cTokensUsed = 0;
                    rvOut = null;
                    return MatchResult.Fail;
                }
                 history.type = symT.type;
            }

            //
            //  We need to control the set of allowable constraints based on the type of the object we are constrainting.
            //

            for (; choice < RuleType.Choice8; choice += 1) {
                f = false;
                ruleNo = RuleNo.ModuleBody;         // Should never get this

                switch (choice) {
                case RuleType.Choice2:
                    //
                    //  Single Value - all types are allowed
                    //
                    //

                    if (baseType != BaseType.Class) {
                        ruleNo = RuleNo.SingleValue;
                        f = true;
                    }
                    break;

                case RuleType.Choice3:
                    //
                    // ContainedSubtype
                    //

                    if (((baseType == BaseType.Leaf) && ((dataType == DataType.BitString) || (dataType == DataType.Boolean) ||
                                                         (dataType == DataType.Enumerated) || (dataType == DataType.Integer) ||
                                                         (dataType == DataType.ObjectIdentifier) || (dataType == DataType.OctetString) ||
                                                         (dataType == DataType.String))) ||
                        (baseType == BaseType.Choice) || 
                        (baseType == BaseType.InstanceOf) ||
                        (baseType == BaseType.Null) || (baseType == BaseType.ObjectClassField) ||
                        (baseType == BaseType.ObjectDescriptor) ||
                        (baseType == BaseType.Real) ||
                        (baseType == BaseType.RelativeObjectIdentifier) || 
                        (baseType == BaseType.Sequence) || (baseType == BaseType.SequenceOf) ||
                        (baseType == BaseType.Set) || (baseType == BaseType.SetOf)) {
                        f = true;
                        ruleNo = RuleNo.ContainedSubtype;
                    }
                    break;

                case RuleType.Choice1:
                    //
                    // ValueRange
                    //
                    //  (b) Allowed only within PermittedAlphabet for ....
                    //

                    if ((baseType == BaseType.Leaf) && ((dataType == DataType.Integer) || (dataType == DataType.Real))) {
                        f = true;
                        ruleNo = RuleNo.ValueRange;
                    }
                    break;

                case RuleType.Choice0:
                    //
                    // Size Constraint
                    //

                    if (((baseType == BaseType.Leaf) && 
                        ((dataType == DataType.BitString) || (dataType == DataType.OctetString) ||
                         (dataType == DataType.String))) || 
                        (baseType == BaseType.ObjectDescriptor) ||
                        (baseType == BaseType.RestrictedCharacter) ||
                        (baseType == BaseType.SequenceOf) || (baseType == BaseType.SetOf) ||
                        (baseType == BaseType.UnrestrictedCharacter)) {
                        f = true;
                        ruleNo = RuleNo.SizeConstraint;
                    }
                    break;

                case RuleType.Choice4:
                    //
                    //  Permitted Alphabet
                    //

                    if ((baseType == BaseType.ObjectDescriptor) || 
                        ((baseType == BaseType.Leaf) && (dataType == DataType.String))) {  // M00BUG and ! Unrestricted String
                        f = true;
                        ruleNo = RuleNo.PermittedAlphabet;
                    }
                    break;

                case RuleType.Choice5:
                    //
                    //  Type Constraint
                    //

                    if (baseType == BaseType.OpenType) {
                        f = true;
                        ruleNo = RuleNo.TypeConstraint;
                    }
                    break;

                case RuleType.Choice6:
                    //
                    //  Inner subtyping
                    //

                    if ((baseType == BaseType.Choice) || (baseType == BaseType.Embedded_PDV) ||
                        (baseType == BaseType.External) || (baseType == BaseType.InstanceOf) ||
                        (baseType == BaseType.Real) || (baseType == BaseType.Sequence) ||
                        (baseType == BaseType.SequenceOf) || (baseType == BaseType.Set) ||
                        (baseType == BaseType.SetOf) || (baseType == BaseType.UnrestrictedCharacter)) {
                        f = true;
                        ruleNo = RuleNo.InnerTypeConstraints;
                    }
                    break;

                case RuleType.Choice7:
                    //
                    //  Pattern constraint
                    //

                    if (baseType == BaseType.RestrictedCharacter) {
                        f = true;
                        ruleNo = RuleNo.PatternConstraint;
                    }
                    break;
                }

                //
                //  If permitted constraint - then attempt to parse the file
                //

                if (f) {
                    history2.PopTo(0);
                    mr = MatchRules(ruleNo, iLevel + 1, iTokenIn, ref history2, out cTokensUsed, out rvOut);
                    if (mr != MatchResult.Fail) {
                        if (mr == MatchResult.Success) {
                            mr = MatchResult.ChoiceLeft;
                        }

                        history[0].choice = choice;
                        history[1].result = mr;
                        return mr;
                    }
                }
            }

            cTokensUsed = 0;
            rvOut = null;

            return MatchResult.Fail;
        }


        MatchResult Word(int iTokenIn, int iLevel, ref HistoryStack history, out int cTokensUsed, out RuleValue rvOut)
        {
            int i;

            //
            //  Choices at this point is a number of tokens or an identifier with some restraints.
            //

            switch (m_rgTokens[iTokenIn].tknType) {
            case TknType.U_Identifier:
                //  Only uppercase letters and '-' allowed

                for (i = 0; i < m_rgTokens[iTokenIn].name.Length; i++) {
                    if (!char.IsUpper(m_rgTokens[iTokenIn].name[i]) && (m_rgTokens[iTokenIn].name[i] != '-')) {
                        cTokensUsed = 0;
                        rvOut = null;
                        return MatchResult.Fail;
                    }
                }

                history.Push(RuleNo.word, 0);
                cTokensUsed = 1;
                rvOut = new RuleValue(new RuleStep(m_rgTokens[iTokenIn].name));
                break;

            case TknType.L_Identifier:
            case TknType.A_L_Identifier:
            case TknType.A_U_Identifier:
            case TknType.BIT:
            case TknType.BOOLEAN:
            case TknType.CHARACTER:
            case TknType.CHOICE:
            case TknType.EMBEDDED:
            case TknType.END:
            case TknType.ENUMERATED:
            case TknType.EXTERNAL:
            case TknType.FALSE:
            case TknType.INSTANCE:
            case TknType.INTEGER:
            case TknType.INTERSECTION:
            case TknType.MINUS_INFINITY:
            case TknType.NULL:
            case TknType.OBJECT:
            case TknType.OCTET:
            case TknType.PLUS_INFINITY:
            case TknType.REAL:
            case TknType.RELATIVE_OID:
            case TknType.SEQUENCE:
            case TknType.SET:
            case TknType.TRUE:
            case TknType.UNION:
                cTokensUsed = 0;
                rvOut = null;
                return MatchResult.Fail;

            default:
                if (m_rgTokens[iTokenIn].tknType >= TknType.ABSENT) {

                    history.Push(RuleNo.word, 0);
                    cTokensUsed = 1;
                    rvOut = new RuleValue(new RuleStep(m_rgTokens[iTokenIn].tknType));
                }
                else {
                    cTokensUsed = 0;
                    rvOut = null;
                    return MatchResult.Fail;
                }
                break;
            }
            return MatchResult.Success;
        }

        MatchResult BackTrack(Rule rules, HistoryStack history, ref ErrorList errlst, out int iHistory, out int iItem, out int iToken)
        {
            int i;

            //
            //  Walk backwards through the set of history results looking for a previous
            //      path that has a choice left.
            //

            for (i = history.Count-1; i >= 0; i--) {
                if (history[i].result == MatchResult.ChoiceLeft) {
                    //
                    //  Set the default return values
                    //

                    iHistory = i;
                    iToken = history[i].iToken;
                    iItem = history[i].iItem;
                    history.PopTo(iHistory + 1);

                    //
                    //  Need to check if this was part of a repeat.  
                    //

                    for (; i >= 0; i--) {
                        switch (history[i].choice) {
                        case RuleType.Plus1:
                            if (history[i].iItem == history[iHistory].iItem - 1) {
                                iItem = history[i].iItem;
                                iHistory = i;
                            }
                            return MatchResult.Success;

                        case RuleType.Plus2:
                            if ((history[i].iItem == history[iHistory].iItem - 1) ||
                                (history[i].iItem == history[iHistory].iItem - 2)) {
                                iItem = history[i].iItem;
                                iHistory = i;
                            }
                            return MatchResult.Success;

                        case RuleType.Optional1:
                            if (history[i].iItem == history[iHistory].iItem - 1) {
                                iItem = history[i].iItem;
                                iHistory = i;
                            }
                            return MatchResult.Success;

                        default:
                            break;
                        }
                    }

                    return MatchResult.Success;
                }
                else if (history[i].result == MatchResult.Fail) {
                    if ((history[i].rv != null) && (history[i].rv.IsError())) {
                        if (errlst == null) {
                            errlst = new ErrorList();
                        }
                        errlst.Add(history[i].rv.error);
                    }
                    else if ((history[i].rv != null) && (history[i].rv.IsErrorList())) {
                        if (errlst == null) {
                            errlst = new ErrorList();
                        }
                        errlst.Add(history[i].rv.errorList);
                    }
                }
            }

            //
            //  If we found no choices left in the tree, see if this rule
            //      was built from choices and thus might have a choice left.
            //

            if (rules.rgRules[0].IsChoice()) {
                for (iItem = history[history.Count-1].iItem; iItem < rules.rgRules.Length; iItem++) {
                    if (rules.rgRules[iItem].IsChoice()) {
                        iHistory = 0;
                        iToken = history[0].iToken;
                        history.PopTo(0);
                        return MatchResult.Success;
                    }
                }
            }

            // Fail - it has no choices left

            iHistory = 0;
            iToken = 0;
            iItem = 0;
            return MatchResult.Fail;
        }

        MatchResult PerformAction(RuleNo rule, RuleType action, out int cTokensUsed, HistoryStack history, out RuleValue rvOut)
        {
            MatchResult err;
            ErrorList warnlst;

            //
            //  Start by seeing of this was a pre-defined rule.  If so then it hash it's own special
            //  set of actions that need to be taken.
            //

            if ((RuleNo.RuleIsMissing < rule) && (rule <= RuleNo.LastRule)) {
                err = X680_Actions(rule, action, out cTokensUsed, history, out rvOut);
            }

            //
            //  Now check to see if this rule was defined by an object defintion.  If so then we have
            //  a single final action we perform for it.
            //

            else if (rule <= Rules.LastRuleNo()) {
                err = PerformObjectDefinition(rule, action, out cTokensUsed, history, out rvOut);
            }

            //
            //  We found and processed a rule that does not exist?
            //

            else {
                Debug.Assert(false, "Rule # " + rule + " is out of range in PerformAction");

                cTokensUsed = 0;
                rvOut = null;

                return MatchResult.Fail;
            }

            if (err == MatchResult.Success) {
                //
                //  Process and gather warnings if any exist
                //

                if ((rvOut == null) || (rvOut.warnList == null)) {
                    warnlst = new ErrorList();
                }
                else {
                    warnlst = rvOut.warnList;
                }

                for (int i = 0; i < history.Count; i++) {
                    if ((history[i].rv != null) && (history[i].rv.warnList != null)) {
                        warnlst.Add(history[i].rv.warnList);
                    }
                }

                if (warnlst.Count > 0) {
                    if (rvOut == null) {
                        rvOut = new RuleValue();
                    }
                    rvOut.warnList = warnlst;
                }
            }

            return err;
        }

        public MatchResult PerformObjectDefinition(RuleNo rule, RuleType action, out int cTokensUsed, HistoryStack history, out RuleValue rvOut)
        {
            int i;
            Symbol sym;
            Rule rules = FindRule(rule);
            RuleStep ruleCurrent;

            //
            //  Grab the values and assoicate them with the correct field
            //  names in the object.
            //

            SymbolList list = new SymbolList();

            for (i = 0; i < history.Count; i++) {
                if (history[i].choice == RuleType.Symbol) {
                    ruleCurrent = rules.rgRules[history[i].iItem];

                    switch (ruleCurrent.sym.GetSymbolType()) {
                    case SymbolType.TypeField:

                        sym = new Symbol(ruleCurrent.sym.name, SymbolType.Type);
                        sym.type = history[i].rv.type;
                        list.Add(sym);
                        break;

                    case SymbolType.ValueField:
                        sym = new Symbol(ruleCurrent.sym.name, SymbolType.Value);
                        sym.type = ruleCurrent.sym.type;
                        sym.flags = ruleCurrent.sym.flags;
                        sym.value = history[i].rv.value;
                        list.Add(sym);
                        break;

                    case SymbolType.ObjectField:
                        sym = new Symbol(ruleCurrent.sym.name, SymbolType.Object);
                        sym.type = ruleCurrent.sym.type;
                        sym.value = history[i].rv.value;
                        list.Add(sym);
                        break;

                    case SymbolType.ObjectSetField:
                        sym = new Symbol(ruleCurrent.sym.name, SymbolType.ObjectSet);
                        sym.type = ruleCurrent.sym.type;
                        sym.value = history[i].rv.value;
                        list.Add(sym);
                        break;


                    default:
                        Debug.Assert(false, "NYI: " + ruleCurrent.sym.GetSymbolType());
                        break;
                    }
                }
                else if (history[i].choice == RuleType.Rule) {
                    //  This was reduced already - has a list of symbols to add here
                    foreach (Symbol sym2 in history[i].rv.value.symList) {
                        list.Add(sym2);
                    }
                }
                else if (history[i].choice == RuleType.Optional1) {
                    //
                    //  If an optional failed - skip the next item as it should not be processed
                    //
                    if (history[i+1].result == MatchResult.Fail) {
                        i += 1;
                    }
                }
            }

            cTokensUsed = CountTokensUsed(history);
            rvOut = new RuleValue(new Value(list));

            return MatchResult.Success;
        }

        MatchResult PreParseParameterList(HistoryStack history)
        {
#if false
            int iToken = history[0].iToken;      // First token in the parameter list!
            int state = 0;
            Symbol[] rgsym = new Symbol[30];
            int [,] rgitkn = new int[30, 2];
            int iParam = 0;
            SymbolType st;

            //  
            //  First pass - look for everything that is a simple identifier
            //
            //  Run state machine
            //
            //

            while (state != 4) {
                switch (state) {
                case 0:
                    if (m_rgTokens[iToken].tknType == TknType.LBrace) {
                        state = 1;
                    }
                    else {
                        Debug.Assert(false);
                        return MatchResult.Fail;
                    }
                    break;

                case 1:
                    switch (m_rgTokens[iToken].tknType) {
                    case TknType.U_Identifier:
                        state = 2;
                        rgitkn[iParam, 0] = iToken;
                        break;

                    case TknType.Definition:
                        return MatchResult.Fail;

                    default:
                        state = 5;
                        rgitkn[iParam, 0] = iToken;
                        break;
                    }
                    break;

                case 2:
                    switch (m_rgTokens[iToken].tknType) {
                        case TknType.Colon:
                        state = 3;
                        rgitkn[iParam, 1] = iToken-1;
                        break;

                    case TknType.RBrace:
                        state = 4;

                        DefineParamType:
                        //
                        //  need to know if this is a type or a type/class -- any lower case letters?
                        //

                        st = SymbolType.TypeOrClassParameter;
                        foreach (char ch in m_rgTokens[iToken - 1].name) {
                            if (char.IsLower(ch)) {
                                st = SymbolType.TypeParameter;
                                break;
                            }
                        }
                        rgsym[iParam] = new Symbol(m_rgTokens[iToken - 1].name, st);
                        iParam += 1;
                        break;

                    case TknType.Comma:
                        state = 1;
                        goto DefineParamType;

                    case TknType.Definition:
                        return MatchResult.Fail;

                    default:
                        state = 5;
                        break;
                    }
                    break;

                case 3:
                    switch (m_rgTokens[iToken].tknType) {
                    case TknType.U_Identifier:
                    case TknType.L_Identifier:
                        state = 6;
                        break;

                    default:
                        return MatchResult.Fail;
                    }
                    break;

                case 5:
                    switch (m_rgTokens[iToken].tknType) {
                    case TknType.Colon:
                        state = 3;
                        rgitkn[iParam, 1] = iToken-1;
                        break;

                    case TknType.Definition:
                        return MatchResult.Fail;

                    default:
                        state = 5;
                        break;
                    }
                    break;

                case 6:
                    switch (m_rgTokens[iToken].tknType) {
                    case TknType.RBrace:
                        state = 4;

                        DefineValueOrObject:
                        if (char.IsLower(m_rgTokens[iToken-1].name[0])) {
                            rgsym[iParam] = new Symbol(m_rgTokens[iToken-1].name, SymbolType.ValueOrObjectParameter);
                        }
                        else {
                            rgsym[iParam] = new Symbol(m_rgTokens[iToken-1].name, SymbolType.ValueSetOrObjectSetParameter);
                        }
                        iParam += 1;
                        break;

                    case TknType.Comma:
                        state = 1;
                        goto DefineValueOrObject;

                    default:
                        return MatchResult.Fail;
                    }
                    break;

                default:
                    return MatchResult.Fail;
                }

                iToken += 1;
            }


#endif
            return MatchResult.Success;
        }

        public MatchResult PreEmit(CodeEmitter emitter)
        {
            return m_symbols.PreEmit(emitter);
        }

        public MatchResult Emit(CodeEmitter emit)
        {
            MatchResult mr;
            mr = m_symbols.Emit(emit );
            return mr;
        }

        int CountTokensUsed(HistoryStack history)
        {
            int i;
            int cTokensUsed;

            for (i = 0, cTokensUsed = 0; i < history.Count; i++) {
                if (history[i].choice == RuleType.Plus1) continue;
                if (history[i].choice == RuleType.Plus2) continue;
                if (history[i].choice == RuleType.Star1) continue;
                if (history[i].choice == RuleType.Star2) continue;
                if (history[i].choice == RuleType.Optional1) continue;
                cTokensUsed += history[i].cTokens;
            }

            return cTokensUsed;
        }

        public void Collapse()
        {
            m_symbols.Collapse();
        }

        void ProcessExportList()
        {
        }

        ///
        /// After we have finished processing here, we know the following:
        /// 1.  The list of all of the modules that we can import from
        /// 2.  The list of all symbols that are uniquely imported are in the symbol table
        /// 3.  The state of imported symbols is not necessarily complete!
        ///
        MatchResult ProcessImportListPassOne(SymbolList symlstImports, Token tkn, out RuleValue rvOut)
        {
            AsnFile af;
            Symbol sym1;
            Symbol sym3;
            MatchResult mr = MatchResult.Success;
            SymbolList lst = new SymbolList();

            Debug.Assert(symlstImports != null);

            /*
             *  Create our results element.  The only thing that will happen is that
             *  errors will be added to it either now or on a later pass.
             */
            
            rvOut = new RuleValue();
            m_rvImports = rvOut;
            m_tknImports = tkn;

            /*
             *  Walk each item and process the imports for that module.
             *  This is not the final pass for doing so, unless by chance we
             *  get lucky and finish everything.
             *
             *  Items we check for at this point:
             *     1.  The supplied module name is not in the symbol table
             *     2.
             */

            foreach (Symbol symModule in symlstImports) {
                /*
                 *  See if the module name is already in 
                 *      the symbol table.
                 */

                sym1 = m_symbols.Find(symModule.name);
                if (sym1 != null) {
                    /*
                     *  Yes it is, this is a problem
                     */

                    rvOut.AddWarning(new Error(ErrorNumber.DuplicateImportModuleName, tkn, symModule.name));

                    /*
                     *  Kill the parser until this gets fixed
                     */
                    
                    // mr = MatchResult.Fail;
                }
                else {
                    /*
                     *  Save to add this as a symbol
                     */
                    
                    m_symbols.Add(symModule);
                }

                /*
                 *  Try and locate the module associated with this name
                 *
                 *  Module name look up is by
                 *      OID if the oid is present,
                 *      name if it is not present.
                 */

                af = FindModule(symModule);

                /*
                 *  M00TODO - 
                 *      Print warning if OID is not present or absent in both locations
                 */
                        

                /*
                 * Process  all of the symbols associated with the module
                 */

                foreach (Symbol sym in symModule.symList) {
                    /*
                     *  Does symbol already exist in OUR symbol table?
                     *     - If no, add it.
                     *     - If yes, set it as a conflict so that nobody finds and uses it.
                     */

                    sym3 = m_symbols.Find(sym.name);
                    if (sym3 == null) {
                        m_symbols.Add(sym);
                    }
                    else {
                        sym3.SetConflict();
                    }

                    /*
                     *  Now try and locate the symbol in THEIR symbol table
                     */

                    if (af != null) {
                        //
                        //  M00BUG - Need to do the following:
                        //      Restrict search to just the exported symbols.
                        //      Don't search any contexts
                        
                        sym1 = af.FindSymbol(sym.name);
                    }
                    else sym1 = null;

                    /*
                     *  If we found it AND it is defined...
                     *     Tell our symbol about their symbol
                     */

                    if ((sym1 != null) && ((sym1.symType & SymbolType.BaseMask) != SymbolType.Unknown)) {
                        /*
                         * Point to the real result
                         */

                        sym.SetImport(sym1);
                    }
                    else {
                        /*
                         * Add to the list of items still to be processed
                         */

                        lst.Add(sym);
                    }

                    /*
                     *  Tag the symbol with the module information (or what we know of it
                     */

                    sym.SetImportSource(symModule, af);
                }
            }
            

            /*
             *  Assuming that there are still symbols to be imported - save them
             *  for later processing
             */

            if (lst.Count > 0) {
                m_symlstImports = lst;
            }

            return mr;
        }

        /// <summary>
        /// Do second and later processing on the import symbol list
        /// </summary>
        /// <returns>void</returns>

        void ProcessImportList()
        {
            AsnFile af;
            Symbol sym1;
            Symbol symMod = null;
            SymbolList symlst = new SymbolList();       // List of symbols not successfully processed

            /*
             *  Do we have any import symbols still to be processed?
             */

            if (m_symlstImports == null) return;
            Debug.Assert(m_symlstImports.Count > 0);

            foreach (Symbol sym in m_symlstImports) {
                //
                //  If the symbol does know know the module it came from -
                //  try and look it up again.  
                //

                if (sym.m_af == null) {
                    /*
                     *  If we have already said this module is not found, then skip the symbol
                     */
                    
                    if (sym.m_symModule == symMod) {
                        //
                        //  We did not successfully process, but it can never be processed so
                        //      don't bother putting it on the to be processed later list
                        //
                        // symlst.Add(sym);
                        continue;
                    }

                    /*
                     *  Try and find the module for this symbol
                     */
                    
                    sym.m_af = FindModule(sym.m_symModule);

                    /*
                     *  We failed to find the module - this is a fatal error
                     *  so emit an error messages and forget about processing
                     *  this symbol anymore
                     */
                    
                    if (sym.m_af == null) {
                        //
                        //  We did not successfully process, but it can never be processed so
                        //      don't bother putting it on the to be processed later list
                        //
                        // symlst.Add(sym);

                        m_rvImports.AddWarning(new Error(ErrorNumber.ImportModuleNotFound, m_tknImports, sym.m_symModule.name));
                        continue;
                    }
                }
                
                af = sym.m_af;

                //
                //  See if the symbol exists in the original module - and it is defined.
                //  M00QUEST - does it just have to not be of SymbolType.Unknown?
                //  M00BUG - Restrict search to those symbols that are exported
                //  M00BUG - Don't use context for this search.
                //

                sym1 = af.FindSymbol(sym.name);
                if ((sym1 != null) && ((sym1.symType & SymbolType.BaseMask) != SymbolType.Unknown)) {
                    sym.SetImport(sym1);
                }
                else {
                    symlst.Add(sym);
                }
            }

            /*
             *  See if the "to be processed" list is empty - if not save it
             */

            if (symlst.Count > 0) {
                m_symlstImports = symlst;
            }
            else {
                m_symlstImports = null;
            }
        }

        /// <summary>
        /// Find a given module in the currently defined list of globals modules
        /// <para>When looking for a module use the following set of steps:</para>
        /// <para>If passed reference has an Object Identifier, match the passed in OID againist the module OID.</para>
        /// <para>Else use the name to match againist the name in the module list</para>
        /// </summary>
        /// <param id="sym">module to be searched for.  If an OID exists it is the value of the symbol</param>
        /// <returns>Module object if found, else null</returns>

        static AsnFile FindModule(Symbol sym) 
        {
            Debug.Assert((sym.GetSymbolType() == SymbolType.Module) || (sym.GetSymbolType() == (SymbolType.Module | SymbolType.Imported)));

            if ((sym.value != null) && (sym.value.valueType == ValueType.Symbol)) {
                //  M00BUG - check for external reference symbol

                //  Look for the symbol in this symbol table
                Symbol sym2 = sym.module.FindSymbol (sym.value.sym.name);
                if (sym2 == null) return null;

                sym.value = sym2.value;
            }


            foreach (AsnFile af in m_lstModules) {
                if (sym.value != null) {
                    if (sym.value.Equals(af.m_valModuleOID)) {
                        return af;
                    }
                }
                else {
                    if (sym.name == af.moduleName) {
                        return af;
                    }
                }
                
            }
            return null;
        }


        private SymbolList m_symlstRewrites = new SymbolList();

        public void Rewrite()
        {
            foreach (Symbol sym in m_symbols) {
                RewriteSymbol(sym);
            }

            while (m_symlstRewrites.Count > 0) {
                SymbolList lst = m_symlstRewrites;
                m_symlstRewrites = new SymbolList();

                foreach (Symbol sym in lst) {
                    m_symbols.Add(sym);
                    RewriteSymbol(sym);
                }
            }
        }

        private void RewriteSymbol(Symbol sym)
        {
            List<String> lst = new List<String>();
            Stack<Symbol> stksym;

            if (Program.DebugLevel > 3) Console.Error.WriteLine("Rewrite Symbol '{0}'", sym.name);

            lst.Add("");
            lst.Add(sym.name);

            switch (sym.GetSymbolType()) {
            case SymbolType.Module:
                break;

            case SymbolType.Object:
                Value valT = sym.value;
                RewriteObject (valT, sym.value.type, lst);
                sym.value = valT;
                break;

            case SymbolType.ObjectClass:
                RewriteClass(sym, lst);
                break;

            case SymbolType.ObjectSet:
                stksym = new Stack<Symbol> ();
                stksym.Push (sym);
                RewriteObjectSet (sym.value, sym.type, lst, stksym);
            //    RewriteObjectSet(sym, lst);
                break;

            case SymbolType.Type:
                stksym = new Stack<Symbol>();
                stksym.Push(sym);
                RewriteType(sym.type, lst, true, new Stack<Type>(), stksym);
                Debug.Assert(stksym.Count == 1);
                break;

            case SymbolType.Value:
                stksym = new Stack<Symbol>();
                stksym.Push(sym);
                RewriteValue(sym.value, sym.value.type, lst, stksym);
                Debug.Assert(stksym.Count == 1);
                break;

            default:
                //
                //  Ignore imported and parameterized symbols
                //

                if ((sym.symType & (SymbolType.Parameterized | SymbolType.Imported)) == 0) {
                    Debug.Assert(false);
                }
                break;
            }
        }

        /// <summary>
        /// RewriteClass - Look at all of the value and value set fields to see if we need to
        /// place a new type into the symbol table.
        /// </summary>
        /// <param name="sym"></param>
        /// <param name="lst"></param>
        private void RewriteClass(Symbol sym, List<String> lst)
        {
            // If we are not pointing to a class definition, then we had better be pointing to
            //    an assignment - which would be to a class definition.  If we are pointing to an
            //    assigment - we can defer the re-write until that symbol is processed.
            //

            if (sym.type.subType.baseType != BaseType.Class) {
                Debug.Assert(sym.type.subType.baseType == BaseType.Assignment);
                return;
            }

            foreach (Symbol symField in sym.type.subType.fieldList) {
                if ((symField.symType == SymbolType.ValueField) || (symField.symType == SymbolType.ValueSetField)) {
                    if (symField.type.baseType == BaseType.Assignment) {
                        Symbol symT = symField.type.owner;

                        //
                        //  Symbol is an assignment - The name should start with an '&', and it should not be in a symbol table.
                        //  We are going to rewrite the name to include the list of items above it, and then plop it into the symbol table

                        if (symT.name[0] == '&') {
                            if (Program.args.iEmitter == Emitters.CSharp) {
                                symT.parent = sym;
                            }
                            else {
                                symT.name = C_CodeEmitter.UseUScore (lst) + "__" + symT.name;
                                m_symlstRewrites.Add (symT);
                            }
                        }
                    }
                }
            }
        }

        private void RewriteObject (Value valRoot, Type valType, List<String> lst)
        {
            bool f;
            SymbolList symlstNew = new SymbolList ();
            Type type = valType;
            Stack<Symbol> stksym = new Stack<Symbol> ();

            f = true;

            while (type.baseType != BaseType.Class) {
                if (type.baseType == BaseType.Assignment) {
                    type = type.subType;
                }
                else {
                    Debug.Assert (false);
                    return;
                }
            }

            //
            //  The first thing we need to do is to change the order of items so that 
            //  they match the order in the class definition and not the object defintion syntax.
            //  We may also need to insert items for defaults and tag in optional values as well.
            //

            foreach (Symbol symDef in type.fieldList) {
                f = false;

                //  See if the class definition field has a value in the object for it

                foreach (Symbol symField in valRoot.symList) {
                    if (symDef.name == symField.name) {
                        symlstNew.Add (symField);
                        valRoot.symList.Remove (symField);
                        f = true;
                        break;
                    }
                }

                //  If we did not find any thing - see if there is a default value

                if (!f && symDef.flags.Default) {
                    Symbol sym2 = new Symbol (symDef.name, SymbolType.Value);
                    sym2.type = symDef.type;
                    sym2.value = symDef.flags.valDefault;
                    sym2.flags.Pointer = symDef.flags.Pointer;
                    symlstNew.Add (sym2);
                    f = true;
                }

                //  If the flag is optional - then put in a dummy field

                if (!f && symDef.flags.Optional) {
                    Symbol sym2 = new Symbol (symDef.name, SymbolType.Dummy);
                    symlstNew.Add (sym2);
                    f = true;
                }

                //  If we did not make a match - this is an error and needs to be reported

                Debug.Assert (f);
            }

            //
            //  We should have consumed all of the values in the original value list
            //

            Debug.Assert (valRoot.symList.Count == 0);

            //
            //  Setup the new Value
            //

            Value valT = new Value(symlstNew);
            valT.type = valType;
            valRoot.ChangeTo (valT);

            //
            //  Do rewrites on all of the child values.  This can be done by
            //  creating new symbols or just doing the re-write
            //

            foreach (Symbol symField in valRoot.symList) {
                lst.Add (symField.name);
                if (Program.args.iEmitter == Emitters.CSharp) {
                    switch (symField.GetSymbolType ()) {
                    case SymbolType.Type:
                       // Debug.Assert (false);
                        break;

                    case SymbolType.Value:
                        RewriteValue (symField.value, symField.value.type, lst, stksym);
                        break;

                    case SymbolType.Object:
                        if (symField.value.valueType != ValueType.Symbol) {
                            RewriteObject (symField.value, symField.value.type, lst);
                        }
                        break;

                    case SymbolType.ObjectSet:
                        RewriteObjectSet (symField.value, symField.value.type, lst, stksym);
                        break;

                    case SymbolType.Dummy:
                        break;

                    default:
                        Debug.Assert (false);
                        break;
                    }

                }
                else {
                    switch (symField.GetSymbolType ()) {
                    case SymbolType.Type:
                        switch (symField.type.baseType) {
                        case BaseType.Leaf:
                        case BaseType.Assignment:
                            break;

                        case BaseType.Choice:
                        case BaseType.Set:
                        case BaseType.SetOf:
                        case BaseType.Sequence:
                        case BaseType.SequenceOf:
                        case BaseType.Param_Assignment:
                        case BaseType.Constraint:
                            Type typeNew = new Type (BaseType.Assignment);
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Type);
                            symNew.type = typeNew;
                            typeNew.owner = symNew;

                            typeNew.subType = symField.type;
                            symField.type = typeNew;

                            m_symlstRewrites.Add (symNew);
                            // RewriteSymbol(symNew);
                            break;

                        default:
                            Debug.Assert (false);
                            break;
                        }
                        break;

                    case SymbolType.Value:
                        if ((symField.value.valueType != ValueType.Symbol) ||
                            ((symField.value.sym.symType & SymbolType.InList) != 0)) {
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Value);

                            symNew.value = symField.value;
                            symNew.type = symField.type;

                            symField.value = new Value (symNew);

                            m_symlstRewrites.Add (symNew);
                            // RewriteSymbol(symNew);
                        }
                        break;

                    case SymbolType.Object:
                        if (symField.value.valueType != ValueType.Symbol) {
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Object);

                            symNew.value = symField.value;
                            symNew.type = symField.type;

                            symField.value = new Value (symNew);

                            m_symlstRewrites.Add (symNew);
                        }
                        break;

                    case SymbolType.ObjectSet:
                        if (symField.value.valueType != ValueType.Symbol) {
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.ObjectSet);

                            symNew.value = symField.value;
                            symNew.type = symField.type;

                            symField.value = new Value (symNew);

                            m_symlstRewrites.Add (symNew);
                        }
                        break;

                    case SymbolType.Dummy:
                        break;

                    default:
                        Debug.Assert (false);
                        break;
                    }

                }
                lst.RemoveAt (lst.Count - 1);
            }
        }


#if false
        /// <summary>
        /// RewriteObject performs a set of changes to an object.  These are necessary to correctly
        /// emit the code.
        /// </summary>
        /// <param name="sym"></param>
        /// <param name="lst"></param>
        private void RewriteObject(Symbol sym, List<String> lst)
        {
            bool f;
            SymbolList symlstNew = new SymbolList();
            Type type = sym.type;

            f = true;

            while (type.baseType != BaseType.Class) {
                if (type.baseType == BaseType.Assignment) {
                    type = type.subType;
                }
                else {
                    Debug.Assert(false);
                    return;
                }
            }

            //
            //  The first thing we need to do is to change the order of items so that
            //  they match the order in the definition.  We may also need to insert items
            //  for defaults and tag in optional values as well.
            //

            foreach (Symbol symDef in type.fieldList) {
                f = false;
                foreach (Symbol symField in sym.value.symList) {
                    if (symDef.name == symField.name) {
                        symlstNew.Add(symField);
                        sym.value.symList.Remove(symField);
                        f = true;
                        break;
                    }
                }

                //  There is a default value - plug that in here

                if (!f && symDef.flags.Default) {
                    Symbol sym2 = new Symbol(symDef.name, SymbolType.Value);
                    sym2.type = symDef.type;
                    sym2.value = symDef.flags.valDefault;
                    sym2.flags.Pointer = symDef.flags.Pointer;
                    symlstNew.Add(sym2);
                    f = true;
                }

                if (!f && symDef.flags.Optional) {
                    Symbol sym2 = new Symbol(symDef.name, SymbolType.Dummy);
                    symlstNew.Add(sym2);
                    f = true;
                }

                Debug.Assert(f);
            }

            Debug.Assert(sym.value.symList.Count == 0);

            //
            //  Setup the new sorted symbol list
            //

            Type typeT = sym.value.type;
            sym.value = new Value(symlstNew);
            sym.value.type = typeT;

            //
            //  Do rewrites to turn everything into it's own variable - this is not
            //  needed for C#
            //

            if (Program.args.iEmitter != Emitters.CSharp) {

                foreach (Symbol symField in sym.value.symList) {
                    lst.Add (symField.name);
                    switch (symField.GetSymbolType ()) {
                    case SymbolType.Type:
                        switch (symField.type.baseType) {
                        case BaseType.Leaf:
                        case BaseType.Assignment:
                            break;

                        case BaseType.Choice:
                        case BaseType.Set:
                        case BaseType.SetOf:
                        case BaseType.Sequence:
                        case BaseType.SequenceOf:
                        case BaseType.Param_Assignment:
                        case BaseType.Constraint:
                            Type typeNew = new Type (BaseType.Assignment);
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Type);
                            symNew.type = typeNew;
                            typeNew.owner = symNew;

                            typeNew.subType = symField.type;
                            symField.type = typeNew;

                            m_symlstRewrites.Add (symNew);
                            // RewriteSymbol(symNew);
                            break;

                        default:
                            Debug.Assert (false);
                            break;
                        }
                        break;

                    case SymbolType.Value:
                        if ((symField.value.valueType != ValueType.Symbol) ||
                            ((symField.value.sym.symType & SymbolType.InList) != 0)) {
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Value);

                            symNew.value = symField.value;
                            symNew.type = symField.type;

                            symField.value = new Value (symNew);

                            m_symlstRewrites.Add (symNew);
                            // RewriteSymbol(symNew);
                        }
                        break;

                    case SymbolType.Object:
                        if (symField.value.valueType != ValueType.Symbol) {
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Object);

                            symNew.value = symField.value;
                            symNew.type = symField.type;

                            symField.value = new Value (symNew);

                            m_symlstRewrites.Add (symNew);
                        }
                        break;

                    case SymbolType.ObjectSet:
                        if (symField.value.valueType != ValueType.Symbol) {
                            Symbol symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.ObjectSet);

                            symNew.value = symField.value;
                            symNew.type = symField.type;

                            symField.value = new Value (symNew);

                            m_symlstRewrites.Add (symNew);
                        }
                        break;

                    case SymbolType.Dummy:
                        break;

                    default:
                        Debug.Assert (false);
                        break;
                    }
                    lst.RemoveAt (lst.Count - 1);
                }
            }
        }
#endif

        private void RewriteField(Symbol symField, List<String> lst, Stack<Type> stk, Stack<Symbol> stksym)
        {

            if (symField.GetSymbolType() == SymbolType.Extension) return;
            if (symField.GetSymbolType() == SymbolType.ExtensionEnd) return;
            if (symField.GetSymbolType() == SymbolType.ExtensionGroup) {
                foreach (Symbol sym in symField.symList) {
                    if (Program.DebugLevel > 4) Console.Error.WriteLine("   RewriteField/E: '{0}'", sym.name);
                    lst.Add(sym.name);
                    stksym.Push(sym);
                    RewriteField(sym, lst, stk, stksym);
                    stksym.Pop();
                    lst.RemoveAt(lst.Count - 1);
                }
                return;
            }
            if (symField.GetSymbolType() == SymbolType.Exception) return;

            if (Program.DebugLevel > 4) Console.Error.WriteLine("   RewriteField: '{0}'", symField.name);

            lst.Add(symField.name);
            stksym.Push(symField);
            RewriteType(symField.type, lst, false, stk, stksym);
            stksym.Pop();

            if (symField.flags.Default) {
                //  Am I a simple?
                Type typeReal = symField.type.SkipFluff();

                if ((typeReal.baseType == BaseType.Leaf) &&
                    ((typeReal.dataType == DataType.Boolean) ||
                     ((typeReal.dataType == DataType.Integer) && (typeReal.emitName == "A2C_INTEGER_NATIVE")) ||
                     ((typeReal.dataType == DataType.Enumerated) && (typeReal.emitName == "A2C_ENUMERATED_NATIVE")))) {
                    //  Yes I am a simple type - no changes required?
                }
                else {
                    if (symField.flags.valDefault.valueType != ValueType.Symbol) {
                        Symbol symNew;

                        //  No - this needs to be a symbol name for the value
                        lst.Add("__default");

                        symNew = new Symbol(C_CodeEmitter.UseUScore(lst), SymbolType.Value);
                        symNew.type = symField.flags.valDefault.type;
                        symNew.value = symField.flags.valDefault;

                        symField.flags.valDefault = new Value(symNew);

                        lst.RemoveAt(lst.Count - 1);

                        m_symlstRewrites.Add(symNew);
                    }
                }
            }

            lst.RemoveAt(lst.Count - 1);
        }

        private void RewriteObjectSet (Value valRoot, Type valType, List<String> lst, Stack<Symbol> stkSym)
        {
            int cLst = lst.Count;
            int cItem = 1;
            lst.Add ("__1");

            ValueList lstCurrent;
            ValueList lstToDo = valRoot.valueList;
            valRoot.valueList = new ValueList ();

            //
            //  Process all of the symbols in the list
            //  For symplicity we grab the old list and put everything on a new list as we process it.
            //
            //  Since we are re-writing an object list - the only things we should find to process are objects.
            //
            //  M00BUG - we don't keep the order of the elements in the object set correctly.  This means that things will wander
            //  back and forth across the extension marker point.
            //

            while (lstToDo.Count > 0) {
                lstCurrent = lstToDo;
                lstToDo = new ValueList ();

                foreach (Value val in lstCurrent) {
                    switch (val.valueType) {
                    case ValueType.SymbolList:
                        //
                        //  This is not a reference to an object (i.e. an object name) but an
                        //  object being defined - we therefore need to re-write the object in place
                        //  or make it into a symbol and let that symbol get re-written
                        //

                        if (Program.args.iEmitter == Emitters.CSharp) {
                            RewriteObject (val, val.type, lst);
                        }
                        else {
                            Symbol symNew;
                            Value valNew;

                            lst[cLst] = String.Format ("__{0}", cItem);
                            cItem += 1;

                            symNew = new Symbol (C_CodeEmitter.UseUScore (lst), SymbolType.Object);
                            symNew.type = valRoot.type;
                            symNew.value = val;

                            valNew = new Value (symNew);
                            valNew.type = symNew.type.subType;
                            m_symlstRewrites.Add (symNew);

                            symNew.value.valueList = new ValueList();
                            symNew.value.valueList.Add (valNew);
                        }
                        break;

                    case ValueType.Symbol:
                        //  Nested object sets need to be flattened

                        if (val.sym.symType == SymbolType.ObjectSet) {
                            foreach (Value val2 in val.sym.value.valueList) {
                                lstToDo.Add (val2);
                            }
                        }
                        else {
                            valRoot.valueList.Add (val);
                        }
                        break;

                    case ValueType.Extension:
                        valRoot.valueList.Add (val);
                        break;

                    case ValueType.List:
                        lstToDo.Add (val.valueList);
                        break;

                    case ValueType.ExtractFromObject:
                    case ValueType.ExtractFromObjectSet:
                        //  M00BUG - this are not processed yet
                        //Debug.Assert (false);
                        break;

                    default:
                        Debug.Assert (false);
                        break;
                    }
                }
            }
            lst.RemoveAt (cLst);
        }

#if false
        private void RewriteObjectSet(Symbol sym, List<String> lst)
        {
            int cLst = lst.Count;
            int cItem=1;
            lst.Add("__1");

            ValueList lstCurrent;
            ValueList lstTodo = sym.value.valueList;
            sym.value.valueList = new ValueList();

            while (lstTodo.Count > 0) {
                lstCurrent = lstTodo;
                lstTodo = new ValueList();

                foreach (Value val in lstCurrent) {
                    switch (val.valueType) {
                    case ValueType.SymbolList:
                        Symbol symNew;
                        Value valNew;

                        //
                        //  We have an object defintion rather than an object reference.
                        //  We therefore need to turn the object into a reference.
                        //

                        lst[cLst] = String.Format("__{0}", cItem);
                        cItem += 1;

                        symNew = new Symbol(C_CodeEmitter.UseUScore(lst), SymbolType.Object);
                        symNew.type = sym.type;
                        symNew.value = val;

                        valNew = new Value(symNew);
                        valNew.type = sym.type.subType;
                        m_symlstRewrites.Add(symNew);

                        sym.value.valueList.Add(valNew);
                        break;

                    case ValueType.Symbol:
                        //
                        //  Nested object sets need to be flattened
                        //

                        if (val.sym.symType == SymbolType.ObjectSet) {
                            foreach (Value val2 in val.sym.value.valueList) {
                                lstTodo.Add(val2);
                            }
                        }
                        else {
                            sym.value.valueList.Add(val);
                        }
                        break;

                    case ValueType.Extension:
                        sym.value.valueList.Add(val);
                        break;

                    case ValueType.List:
                        lstTodo.Add(val.valueList);
                        break;

                    case ValueType.ExtractFromObjectSet:
                    case ValueType.ExtractFromObject:
                        // M00BUG - need to do somthing here
                        break;

                    default:
                        break;
                    }
                }
            }

            lst.RemoveAt(cLst);
        }
#endif

        private void RewriteType(Type type, List<String> lst, Boolean fTop, Stack<Type> stk, Stack<Symbol> stkSym)
        {
            Symbol sym;
            Type typeNew;
            Symbol symNew;


            Debug.Assert(!fTop || (type.baseType == BaseType.Assignment));

            if (Program.DebugLevel > 2) Console.Error.WriteLine("   RewriteType: '{0}'", type.baseType);

            switch (type.baseType) {
            case BaseType.Assignment:
                if (!fTop && (type.owner != null)) return;
                stk.Push(type);
                RewriteType(type.subType, lst, false, stk, stkSym);
                stk.Pop();
                break;

            case BaseType.Param_Assignment:
                stk.Push(type);
                RewriteType(type.subType, lst, false, stk, stkSym);
                stk.Pop();
                break;

            case BaseType.Constraint:
                RewriteType(type.subType, lst, fTop, stk, stkSym);
                /*
                 *
                 *
                 */

                foreach (Constraint cst in type.constraintList) {
                    switch (cst.ct) {
                    case ConstraintType.Relation:
                        stk.Push(type);
                        ConstraintLocateSymbols(cst, stk, lst, stkSym);
                        stk.Pop();
                        break;

                    case ConstraintType.Table:
                        RewriteValue(cst.value, cst.value.type, lst, stkSym);
                        if (cst.value.valueType != ValueType.Symbol) {
                            lst.Add("_Table");
                            Symbol symNew2 = new Symbol(C_CodeEmitter.UseUScore(lst), SymbolType.ObjectSet);
                            symNew2.type = cst.value.type;
                            symNew2.value = cst.value;

                            cst.value = new Value(symNew2);

                            m_symlstRewrites.Add(symNew2);
                            lst.RemoveAt(lst.Count - 1);
                        }
                        break;

                    case ConstraintType.Contents:
                        //  We must be on an OCTET STRING or on a BIT STRING
                        //  Change the base type to an OPEN TYPE
                        //  
                        //  Process the child constriants of me.
                        //

                        //
                        //  Change the base type we are working with - to an open version of
                        //  what was there before.
                        //  M00BUG - we loose any constraints on the base type when we do this
                        //

                        Type typeBase = type.SkipFluff();

                        if (typeBase == Type.OctetString) {
                            type.subType = Type.OpenOctetString;
                        }
                        else if (typeBase == Type.BitStringType) {
                            type.subType = Type.OpenBitString;
                        }
                        else {
                            // M00BUG - this is an error
                        }
                        lst.Add("_Contents");
                        RewriteType(cst.type, lst, fTop, stk, stkSym);
                        lst.RemoveAt(lst.Count - 1);
                        break;
                    }
                }

                return;
                
            case BaseType.Tagged:

                //
                //  Change from Automatic Tag to Implicit Tag
                //
                
                if (type.tagged.tagType == TagType.Automatic) {
                    type.tagged.tagType = TagType.Implicit;
                }

                //
                // M00TODO - If IMPLICIT tag, change to EXPLICIT if wrapping a CHOICE, Open Type or Dummy Ref
                //

                if (type.tagged.tagType == TagType.Implicit) {
                    Type typeT = type.subType;
                    bool f = true;

                    while (f) {
                        switch (typeT.baseType) {
                        case BaseType.Choice:
                        case BaseType.OpenType:
                            f = false;
                            type.tagged.tagType = TagType.Explicit;
                            break;

                        case BaseType.Assignment:
                        case BaseType.Constraint:
                        case BaseType.NamedList:
                        case BaseType.Param_Assignment:
                            typeT = typeT.subType;
                            break;

                        case BaseType.Symbol:
                            typeT = typeT.sym.type;
                            break;

                        case BaseType.ObjectClassField:
                            Symbol symField = typeT.GetObjectClassField();
                            if (symField.symType == SymbolType.TypeField) {
                                f = false;
                                type.tagged.tagType = TagType.Explicit;
                            }
                            else {
                                typeT = symField.type;
                            }
                            break;

                        case BaseType.Leaf:
                        case BaseType.Sequence:
                        case BaseType.SequenceOf:
                        case BaseType.Set:
                        case BaseType.SetOf:
                        case BaseType.Tagged:
                            f = false;
                            break;

                        default:
                            Debug.Assert(false);
                            f = false;
                            break;
                        }
                    }
                }

                RewriteType(type.subType, lst, fTop, stk, stkSym);
                break;

            case BaseType.NamedList:
                RewriteType(type.subType, lst, fTop, stk, stkSym);

                //
                //  If the top symbol on the stack points to an assignment, then we are ok.
                //  Otherwise we need to make it point to an assigment and we meed to make all
                //  of the values of the named list point to that value -- for integer and enumerated.
                //  Note that we need to behaive sligthly differently for bitstring as the named items
                //  still need to be native integer values.
                //

                if (stkSym.Peek().type.baseType != BaseType.Assignment) {
                    bool f = false;
                    Type typeOld = stkSym.Peek().type;
                    typeNew = new Type(BaseType.Assignment);

                    typeNew.subType = typeOld;
                    symNew = new Symbol(SymbolType.Type);
                    symNew.name = stkSym.Peek().name + "__Type";
                    symNew.type = typeNew;
                    typeNew.owner = symNew;

                    symNew.parent = stkSym.ToArray()[1];

                    stkSym.Peek().type = typeNew;

                    foreach (Symbol symX in stkSym) {
                        switch (symX.symType) {
                        case SymbolType.Type:
                            switch (symX.type.SkipFluff().baseType) {
                            case BaseType.Choice:
                            case BaseType.Sequence:
                            case BaseType.Set:
                                symX.symlstParts.Add(symNew);
                                f = true;
                                break;
                            }
                            break;
                        }
                        if (f) break;
                    }
                    Debug.Assert(f);
                }

                typeNew = stkSym.Peek().type;

                //
                //  Need to make the type on the values match with the root type.
                //  Specifically native vs huge.
                //

                if ((typeNew.SkipFluff().baseType == BaseType.Leaf) &&
                    (typeNew.SkipFluff().dataType == DataType.Enumerated ||
                    typeNew.SkipFluff().dataType == DataType.Integer)) {
                    foreach (Symbol symT2 in type.fieldList) {
                        symT2.type = typeNew;
                    }
                }
                return;

            case BaseType.Leaf:
            case BaseType.Symbol:
            case BaseType.OpenType:
                return;

            case BaseType.Choice:
            case BaseType.Sequence:
            case BaseType.Set:
                stk.Push(type);
                if ((type.baseType == BaseType.Choice) &&
                    (!Program.args.fAnonUnion)) {
                    lst.Add("u");
                }
                foreach (Symbol symI in type.fieldList) {
                    RewriteField(symI, lst, stk, stkSym);
                }
                if ((type.baseType == BaseType.Choice) &&
                    (!Program.args.fAnonUnion)) {
                    lst.RemoveAt(lst.Count - 1);
                }
                stk.Pop();
                break;


            case BaseType.ObjectClassField:
                Symbol symT = type.GetObjectClassField();
                switch (symT.GetSymbolType()) {
                case SymbolType.ValueField:
                    RewriteType(symT.type, lst, fTop, stk, stkSym);
                    return;

                case SymbolType.TypeField:
                    return;
                }
                Debug.Assert(false);
                return;

            case BaseType.SequenceOf:
            case BaseType.SetOf:
                //
                //  If this is a symbol or a leaf, we can ignore it
                //

                Debug.Assert(type.fieldList.Count == 1);
                sym = type.fieldList[0];

                Type type2 = sym.type;

                //
                //  Walk down this type until we get to good or bad news
                //

                while (type2 != null) {
                    switch (type2.baseType) {
                    case BaseType.Assignment:
                        if (type2.owner != null) return;
                        type2 = null;
                        break;

                    case BaseType.Leaf:
                        return;

                    case BaseType.Tagged:
                        type2 = null;
                        break;

                    case BaseType.Constraint:
                        /*
                         */

                        foreach (Constraint cst in type2.constraintList) {
                            switch (cst.ct) {
                            case ConstraintType.Relation:
                                stk.Push(type);
                                ConstraintLocateSymbols(cst, stk, lst, stkSym);
                                stk.Pop();
                                break;
                            }
                        }
                        type2 = type2.subType;
                        break;

                    case BaseType.ObjectClassField:
                        type2 = type2.subType;
                        break;

                    case BaseType.Symbol:
                        return;

                    default:
                        type2 = null;
                        break;
                    }
                }

                lst.Add(sym.name);
                typeNew = new Type(BaseType.Assignment);
                symNew = new Symbol(C_CodeEmitter.UseUScore(lst), SymbolType.Type);
                symNew.type = typeNew;
                typeNew.owner = symNew;
                typeNew.subType = sym.type;

                m_symlstRewrites.Add(symNew);
                sym.type = new Type(symNew);

                // RewriteSymbol(symNew);

                lst.RemoveAt(lst.Count - 1);
                break;

            default:
                Debug.Assert(false);
                break;
            }
        }

        private void RewriteValue(Value val, Type typField, List<String> lst, Stack<Symbol> stksym)
        {
            //
            //  When looking at values we need to do two different types of
            //  re-write operations.
            //  1.  In some cases we need to turn sub-trees of a value into a seperate
            //      value symbol.  One such case is for open types.
            //  2.  A second thing, is that some values them selfs need to be correct to
            //      remove strangeness left over from the parser and the language definition.
            //      for example object sets need to be flattened out into a single list of values.
            //

            if (val.valueType == ValueType.Extension) return;

            //
            //  Walk down the type structure looking for specific events
            //

            bool f = true;
            do {
                switch (typField.baseType) {
                case BaseType.Leaf:
                case BaseType.Sequence:
                case BaseType.SequenceOf:
                case BaseType.Set:
                case BaseType.SetOf:
                case BaseType.Choice:
                case BaseType.Class:
                    f = false;
                    break;

                case BaseType.Assignment:
                case BaseType.Param_Assignment:
                case BaseType.Constraint:
                case BaseType.Tagged:
                    typField = typField.subType;
                    break;

                case BaseType.ObjectClassField:
                    Symbol symT = typField.GetObjectClassField();

                    switch (symT.symType) {
                    case SymbolType.ValueField:
                        typField = symT.type;
                        break;

                    case SymbolType.TypeField:
                        if (val.valueType == ValueType.Symbol) {
                            // No op - nothing to do?
//                            Debug.Assert(false);
                        }
                        else if (Program.args.iEmitter == Emitters.C) {
                            Symbol symNew = new Symbol(C_CodeEmitter.UseUScore(lst), SymbolType.Value);
                            Value valNew = new Value(false);
                            valNew.ChangeTo(val);

                            symNew.value = valNew;
                            symNew.type = valNew.type;
                            symNew.m_symTable = stksym.Peek().m_symTable;

                            val.ChangeTo(new Value(symNew));
                            val.name = valNew.name;

                            stksym.Peek().symlstParts.Add(symNew);
                            stksym.Push(symNew);
                            RewriteValue(symNew.value, symNew.value.type, lst, stksym);
                            stksym.Pop();
                        }
                        f = false;
                        break;

                    default:
                        Debug.Assert(false);
                        f = false;
                        break;
                    }
                    break;

                case BaseType.NamedList:    // M00BUG
                    f = false;
                    break;

                default:
                    Debug.Assert(false);

                    f = false;
                    break;
                }
            } while (f);

            //
            //  Now do the value structure re-writes.
            //

            switch (val.valueType) {
            case ValueType.SetOf:
                ValueList lstRemove = new ValueList();

                foreach (Value valChild in val.valueList) {
                    RewriteValue(valChild, valChild.type, lst, stksym); // M00BUG.
                    if (valChild.valueType == ValueType.SetOf) {
                        lstRemove.Add(valChild);
                    }
                }

                foreach (Value valChild in lstRemove) {
                    val.valueList.Remove(valChild);
                    val.valueList.Add(valChild.valueList);
                }

                if (val.valueList.Count == 1) {
                    //  Rewrite down again?

                    if ((val.valueList[0].valueType == ValueType.Symbol) &&
                         ((val.valueList[0].sym.GetSymbolType() == SymbolType.ObjectSet) ||
                          (val.valueList[0].sym.GetSymbolType() == SymbolType.ObjectSetParameter))) {
                        val.sym = val.valueList[0].sym;
                        val.valueList = null;
                    }
                }
                break;

            case ValueType.List:
                switch (typField.baseType) {
                case BaseType.Sequence:
                    int iChild = 0;
                    foreach (Symbol typChild in typField.fieldList) {
                        if ((iChild < val.valueList.Count) && (typChild.name == val.valueList[iChild].name)) {
                            lst.Add(typChild.name);
                            RewriteValue(val.valueList[iChild], typChild.type, lst, stksym);
                            lst.RemoveAt(lst.Count - 1);
                            iChild += 1;
                        }
                    }
                    Debug.Assert(iChild == val.valueList.Count);
                    break;

                default:
                    Debug.Assert(false);
                    break;

                case BaseType.ObjectClassField:
                    break;

                case BaseType.Leaf:
                case BaseType.Class:        // May not be correct.
                case BaseType.Set:
                case BaseType.SequenceOf:
                case BaseType.SetOf:
                    foreach (Value valChild in val.valueList) {
                        RewriteValue(valChild, valChild.type, lst, stksym);
                    }
                    break;
                }
                break;

            case ValueType.ExtractFromObject:
                //  Use 1. the value in the value list 
                //      2. The token list  - to get and change this value to that.
                //  -- check that the type is correct.
                //

                Value valX = val.valueList[0];
                string str = val.name;
                Debug.Assert(val.valueList.Count == 1);

                RewriteValue(valX, valX.type, lst, stksym);

                foreach (Token tkn in val.tknlst) {
                    f = false;

                    while (!f && (valX != null)) {
                        switch (valX.valueType) {
                        case ValueType.Symbol:
                            Symbol symX = valX.sym;
                            if (symX.symType == SymbolType.Object) {
                                valX = symX.value;
                            }
                            else {
                                //
                                //  I don't know what else could legally appear here.
                                //

                                Debug.Assert(false);
                            }
                            break;

                        case ValueType.SymbolList:
                            foreach (Symbol sym1 in valX.symList) {
                                if (sym1.name == tkn.name) {
                                    f = true;
                                    switch (sym1.symType) {
                                    case SymbolType.Value:
                                        valX = sym1.value;
                                        break;

                                    default:
                                        valX = sym1.value;
                                        break;
                                    }
                                }
                            }
                            break;

                        default:
                            Debug.Assert(false);
                            break;
                        }
                    }

                }

                val.ChangeTo(valX);
                val.name = str;

                break;
            }
        }

        private void ConstraintLocateSymbols(Constraint cst, Stack<Type> stk, List<String> lst, Stack<Symbol> stkSym)
        {
            Type[] rg = stk.ToArray();

            //
            //  Re-write the table value half
            //

            RewriteValue(cst.value, cst.value.type, new List<String>(), stkSym);
            if (cst.value.valueType != ValueType.Symbol) {
                lst.Add("_Table");
                Symbol symNew2 = new Symbol(C_CodeEmitter.UseUScore(lst), SymbolType.ObjectSet);
                symNew2.type = cst.value.type;
                symNew2.value = cst.value;

                cst.value = new Value(symNew2);

                m_symlstRewrites.Add(symNew2);
                lst.RemoveAt(lst.Count - 1);
            }


            //
            //  Walk each of the constraints looking for it's symbol.
            //

            foreach (Symbol symConstraint in cst.symlst) {
                //
                //  Process this symbol.  We have the stack of types and we have
                //  the list of pointers - start playing together.
                //

                if (symConstraint.symList[0].name == "@.") {
                    int iType = 0;
                    Type typ = null;
                    int iDepth = 0;
                    Symbol sym2 = null;

                    foreach (Symbol symT in symConstraint.symList) {

                        if ((symT.name == "@.") || (symT.name == ".")) {
                            //
                            //  Walk up the stack looking for items we can be members of.
                            //

                            iType += 1;
                            typ = null;

                            for (; (iType < rg.Length) && (typ == null); iType++) {
                                switch (rg[iType].baseType) {
                                case BaseType.Set:
                                case BaseType.Sequence:
                                case BaseType.Choice:
                                    typ = rg[iType];
                                    break;

                                case BaseType.SetOf:
                                case BaseType.SequenceOf:
                                    iDepth += 1;
                                    break;

                                case BaseType.Assignment:
                                case BaseType.Param_Assignment:
                                    Debug.Assert(false);
                                    break;
                                }
                            }
                            if (typ == null) {
                                Debug.Assert(false, "Overran the top of the stack - ERROR");
                                // M00TODO - Generate an error message
                                return;
                            }
                        }
                        else {
                            Debug.Assert(typ != null);
                            Debug.Assert((typ.baseType == BaseType.Choice) || (typ.baseType == BaseType.Sequence) || (typ.baseType == BaseType.Set));


                            sym2 = null;
                            foreach (Symbol symT2 in typ.fieldList) {
                                if (symT2.name == symT.name) {
                                    sym2 = symT2;
                                    break;
                                }
                            }

                            Debug.Assert(sym2 != null);
                            typ = sym2.type;
                        }
                    }
                    symConstraint.m_sym = sym2;
                    symConstraint.symList = null;
                    sym2.depth = iDepth;
                }
                else {
                    //
                    //  Walk up the stack looking for the first Assignment type and start there.
                    //  
                    //

                    Type typeNode = rg[rg.Length - 1];
                    Symbol sym2;
                    Symbol sym3 = null;
                    int iDepth = 0;

                    for (int i = 0; i < rg.Length; i++) {
                        if ((rg[i].baseType == BaseType.Assignment) || (rg[i].baseType == BaseType.Param_Assignment)) {
                            typeNode = rg[i - 1];
                            break;
                        }
                        if ((rg[i].baseType == BaseType.SetOf) ||
                            (rg[i].baseType == BaseType.SequenceOf)) {
                            iDepth += 1;
                        }
                    }


                    foreach (Symbol symId in symConstraint.symList) {
                        switch (typeNode.baseType) {
                        case BaseType.Set:
                        case BaseType.Choice:
                        case BaseType.Sequence:
                            sym2 = null;
                            foreach (Symbol symField in typeNode.fieldList) {
                                if (symId.name == symField.name) {
                                    sym2 = symField;
                                    break;
                                }
                            }
                            if (sym2 == null) Debug.Assert(false);
                            typeNode = sym2.type;
                            break;

                        default:
                            sym2 = null;
                            Debug.Assert(false);
                            break;
                        }
                        sym3 = sym2;
                    }

                    symConstraint.m_sym = sym3;
                    symConstraint.symList = null;
                    sym3.depth = iDepth;
                }

            }
        }

        /// <summary>
        /// CheckTagsForSet applies the rules for a completed set.
        /// </summary>
        /// <param name="rv">RuleValue containing the set definition</param>
        void CheckTagsForSet(Type typ)
        {
            Debug.Assert(typ.baseType == BaseType.Set);

            //
            //  What we are checking for:
            //  1.  
        }

        MatchResult StdTypeReplacement(Symbol symRoot, int iTokenLast, Type typeDefault, Type typeUse, Type typeNeg, String strCmd, ref RuleValue rvOut)
        {
            //
            //  Two possiblities as this point:
            //      the target is a Type name (i.e. CertificteNumber) or
            //      the target is a field name (i.e. Certificate.certificateNumber)
            //

            Boolean fDone = false;
            Boolean fAssignmentOK = true;
            Type type;

            if (symRoot.GetSymbolType() == SymbolType.Type) {
                Debug.Assert(symRoot.type.baseType == BaseType.Assignment);
                type = symRoot.type;
            }
            else if (symRoot.GetSymbolType() == SymbolType.Component) {
                if (symRoot.type == typeDefault) {
                    symRoot.type = typeUse;
                    return MatchResult.Success;
                }
                else if (symRoot.type == typeUse) {
                    //  Dump a warning
                    return MatchResult.Success;
                }
                else if (symRoot.type == typeNeg) {
                    //  Dump an error
                    return MatchResult.Success;
                }
                else if (symRoot.type.baseType == BaseType.Assignment) {
                    //  Clone the type and put out a warning
                    //  I need to lose the assignment as part of the clone -

                    rvOut = new RuleValue(new Error(ErrorNumber.OverrideTypeOfAssignment, m_rgTokens[iTokenLast], strCmd));
                    return MatchResult.Fail;
                }
                else {
                    type = symRoot.type;
                }
            }
            else {
                rvOut = new RuleValue(new Error(ErrorNumber.HugeIntegerBadType, m_rgTokens[iTokenLast], strCmd));
                return MatchResult.Fail;
            }


            //
            //  Common loop code to drill into the item
            //

            while (!fDone) {
                switch (type.baseType) {
                case BaseType.Assignment:
                    if (fAssignmentOK) {
                        fAssignmentOK = false;
                        goto CheckSubType;
                    }

                    Debug.Assert(false);

                    rvOut = new RuleValue(new Error(ErrorNumber.ICE, m_rgTokens[iTokenLast]));
                    return MatchResult.Fail;

                //  M00BUG -- put in rest of items
                case BaseType.Constraint:
                case BaseType.NamedList:
                case BaseType.Tagged:
                CheckSubType:
                    if (type.subType == typeDefault) {
                        type.subType = typeUse;
                        fDone = true;
                    }
                    else if (type.subType == typeUse) {
                        //  Dump a warning
                        fDone = true;

                    }
                    else if (type.subType == typeNeg) {
                        //  Dump an error
                        fDone = true;
                    }
                    else if (type.subType.baseType == BaseType.Assignment) {
                        //  Clone the type and put out a warning
                        //  I need to lose the assignment as part of the clone -

                        rvOut = new RuleValue(new Error(ErrorNumber.OverrideTypeOfAssignment, m_rgTokens[iTokenLast], strCmd));
                        return MatchResult.Fail;
                    }
                    type = type.subType;
                    break;

                // M00BUG - put in rest of items
                case BaseType.Leaf:
                    //
                    //  If we get here - then we say it's the wrong type and error
                    //

                    rvOut = new RuleValue(new Error(ErrorNumber.HugeIntegerBadType, m_rgTokens[iTokenLast], strCmd));
                    return MatchResult.Fail;

                default:
                    Debug.Assert(false, "Unknown baseType in HugeInteger");
                    fDone = true;
                    break;
                }
            }
            return MatchResult.Success;
        }


        RuleValue CheckSequenceFields(ValueList lst, Token tkn)
        {
            Boolean f;
            Type  type = m_context.GetTopType();
            ValueList lstFinal = new ValueList();
            RuleValue ret;
            ErrorList errlst = new ErrorList();

            f = true;
            while (f) {
                switch (type.baseType) {
                case BaseType.Assignment:
                case BaseType.Param_Assignment:
                case BaseType.Tagged:
                    type = type.subType;
                    break;

                case BaseType.Sequence:
                case BaseType.Set:
                    f = false;
                    break;

                default:
                    Debug.Assert(false);
                    f = false;
                    break;
                }
            }
            //
            //  Process each of the different fields
            //
            
            foreach (Symbol sym in type.fieldList) {
                //
                //  Is field in the passed in list?
                //

                f = false;
                foreach (Value val in lst) {
                    if (val.name == sym.name) {
                        lstFinal.Add(val);
                        f = true;
                        break;
                    }
                }

                //
                //  We did not find the symbol - is it either
                //      a DEFAULT or an OPTIONAL?
                //
                
                if (!f) {
                    if (sym.flags.Optional) {
                        //
                        // THis is fine - no complaints
                        //

                        ;
                    }
                    else if (sym.flags.Default) {
                        //
                        //  Need to set the default value in the list
                        //

                        lstFinal.Add(sym.flags.valDefault);
                        sym.flags.valDefault.name = sym.name;
                    }
                    else {
                        //
                        //  An error -create the error and tag it onto the warning list

                        errlst.Add(new Error(ErrorNumber.SeqSetValueMissingField, tkn, sym.name));
                        Debug.Assert(false, "Missing field in sequence");
                    }
                }
            }

            ret = new RuleValue(new Value(lstFinal));
            ret.value.type = m_context.GetTopType();
            if (errlst.Count > 0) ret.warnList = errlst;
            return ret;
        }

        void CheckTagging(SymbolList lst, bool fSequence, out ErrorList errors)
        {
            bool fApplyAutoTagging = false;

            errors = null;

            //
            //  We need to do some additional processing
            //  1.  Should we be doing automatic tagging?
            //  2.  Any complayts about sequential tags for DEFAULT and OPTIONAL
            //

            if (m_fUseAutomaticTagging) {
                fApplyAutoTagging = true;
                foreach (Symbol sym in lst) {
                    if (sym.IsTagged() && !sym.FromComponentOf) {
                        fApplyAutoTagging = false;
                        break;
                    }
                }
            }

            //
            //  Apply automatic tagging
            //

            if (fApplyAutoTagging) {
                int iTag = 0;
                foreach (Symbol sym in lst) {
                    if (sym.symType != SymbolType.Extension) {
                        Type typ = new Type(BaseType.Tagged);
                        typ.tagged.value = iTag++;
                        typ.tagged.tagClass = TagClass.Context;
                        typ.tagged.tagType = TagType.Automatic;
                        typ.subType = sym.type;
                        sym.type = typ;
                    }
                }
            }
            else {
                //  M00TODO - check DEFAULT/OPTIONAL sequencing
            }
        }


        SymbolListList BuildPotentialParameterLists(SymbolList symlstParams)
        {
            int c = 0;
            SymbolListList lstParams = new SymbolListList();

            foreach (Symbol sym in symlstParams) {
                if (sym.symType == SymbolType.TypeOrClassParameter) c += 1;
            }

            //
            //  Simple parameter list - only one set of parameters to consider.
            //

            if (c == 0) {
                lstParams.Add(symlstParams);
            }
            else {
                //
                // We have a complex set of parameters to deal with.
                // We have to build a separate set of parameters for each of the possible
                //  responses that we can get and parse the type for each of those possiblities
                //

                int i2 = 0;
                Symbol sym3;
                int[] rgi = new int[c + 1];

                for (int j = 0; j < 1 << c; j++) {
                    int i1 = 0;
                    SymbolList lst = new SymbolList();
                    Symbol[] rgsym = new Symbol[c];

                    //
                    //  THis needs to be done in two passes since there is the possiblibty
                    //      that some idiot put the value first and the type second.
                    //
                    //  Create a new symbol for each ambigious type/class parameter

                    foreach (Symbol sym in symlstParams) {
                        switch (sym.symType) {
                        case SymbolType.TypeOrClassParameter:
                            if (rgi[i1] == 0) {
                                rgsym[i1] = new Symbol(sym, SymbolType.TypeParameter);
                            }
                            else {
                                Debug.Assert(rgi[i1] == 1);
                                rgsym[i1] = new Symbol(sym, SymbolType.ObjectClassParameter);
                            }
                            i1 += 1;
                            break;

                        default:
                            break;
                        }
                    }

                    //
                    //  Now build the new list of parameters for this combination
                    //

                    i1 = 0;

                    foreach (Symbol sym in symlstParams) {
                        switch (sym.symType) {
                        case SymbolType.TypeOrClassParameter:
                            lst.Add(rgsym[i1]);
                            i1 += 1;
                            break;

                        case SymbolType.ValueOrObjectParameter:
                            sym3 = null;
                            foreach (Symbol sym2 in rgsym) {
                                if (sym.type.sym.name == sym2.name) {
                                    if (sym2.symType == SymbolType.TypeParameter) {
                                        sym3 = new Symbol(sym, SymbolType.ValueParameter);
                                    }
                                    else {
                                        sym3 = new Symbol(sym, SymbolType.ObjectParameter);
                                    }
                                    sym3.type = new Type(sym2);
                                    break;
                                }
                            }
                            Debug.Assert(sym3 != null);
                            lst.Add(sym3);
                            break;

                        case SymbolType.ValueSetOrObjectSetParameter:
                            sym3 = null;
                            foreach (Symbol sym2 in rgsym) {
                                if (sym.type.sym.name == sym2.name) {
                                    if (sym2.symType == SymbolType.TypeParameter) {
                                        sym3 = new Symbol(sym, SymbolType.ValueSetParameter);
                                    }
                                    else {
                                        sym3 = new Symbol(sym, SymbolType.ObjectSetParameter);
                                    }
                                    sym3.type = new Type(sym2);
                                    break;
                                }
                            }
                            Debug.Assert(sym3 != null);
                            lst.Add(sym3);
                            break;

                        default:
                            lst.Add(sym);
                            break;
                        }


                    }
                    lstParams.Add(lst);


                    rgi[0] += 1;
                    for (int i9 = 0; i9 < c; i9++) {
                        if (rgi[i9] == 2) {
                            rgi[i9] = 0;
                            rgi[i9 + 1] += 1;
                        }
                    }
                }

            }
            return lstParams;
        }

        /// <summary>
        /// Monkey though the list of tokens removing extranious EOL markers.
        /// We keep those that immediately preceed a '::=' token and then we
        /// can skip directly to them when we are processing command lines.
        /// </summary>
        /// 
        public void UseEOLHints()
        {
            int  iToken;

            for (iToken = 0; iToken < m_rgTokens.Count; iToken++) {
                if (m_rgTokens[iToken].tknType == TknType.NewLine) {
                    bool fAssignment = false;
                    for (int iToken2 = iToken + 1; iToken2 < m_rgTokens.Count; iToken2++) {
                        if (m_rgTokens[iToken2].tknType == TknType.NewLine) break;
                        if ((m_rgTokens[iToken2].tknType == TknType.Definition) ||
                            (m_rgTokens[iToken2].tknType == TknType.CMDSTART)) {
                            fAssignment = true;
                            break;
                        }
                    }

                    if (!fAssignment) {
                        m_rgTokens.RemoveAt(iToken);
                        iToken -= 1;
                    }
                }
            }
        }
    }
}
