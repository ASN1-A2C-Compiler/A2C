using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace asn_compile_cs
{
    public enum SymbolType {
        //
        //  This is made up from an enumerated section and a bit map section.
        //  The enumerated section covers those things which are required to be
        //  unique in some fashing.
        //
        //  This is the set of primitive types that can be defined by an ASN.1 module
        //

        Unknown =                       0x00000000,     // We do not know the type of this symbol
        Module =                        0x00000001,     // Symbol is a module
        Type =                          0x00000002,     // Symbol is a Type
        Value =                         0x00000003,     // Symbol is a value
        ValueSet =                      0x00000004,  // Pure version is really a type
        ObjectClass =                   0x00000005,     // Symbol is an Class Defintion
        Object =                        0x00000006,     // Symbol is an Object
        ObjectSet =                     0x00000007,     // Symbol is an Object Set
        Extension =                     0x00000008,     // Start an extension
        Exception =                     0x00000009,     // Note an exception
        ExtensionGroup =                0x0000000a,     // Grouped Extension element
        ExtensionEnd =                  0x0000000b,     // End of an extension

                                                        // The following are used ONLY as parameters 
                                                        //   for parameterized types
        TypeOrClass =                   0x0000000c,     // Symbol is a Type OR a Class Definition
        ValueOrObject =                 0x0000000d,     // Symbol is a Value OR an Object
        ValueSetOrObjectSet =           0x0000000e,     // Symbol is a ValueSet OR an Object Set

        BaseMask =                      0x000000ff, // Set of bits related to the base

        //
        //  This is a set of bits to denote other data
        //

        Parameterized =                 0x00001000,     // Symbol is Parameterized.
        Parameter =                     0x00002000,     // Symbol is a parameter of a parameterized type
        Governor =                      0x00004000,     // Used in parameter lists
        Dummy =                         0x00008000,     // Used in Parameter Lists
        FieldX =                        0x00010000,     // Element in a class definition
        Component =                     0x00020000,     // Element in SET, SEQUENCE, CHOICE
        Relationship =                  0x00040000,
        InList =                        0x00080000,     // Part of a list (i.e. integer, bit string or enum)


        Imported =                      0x01000000,     // Symbol was imported
        NameConflict =                  0x02000000,     // Symbol has a name conflict
        NotYetDefined =                 0x04000000,     // Create symbol to be defined later

        //
        //  Pre-constructed types we know about
        //

        ParameterizedType =             Parameterized | Type,
        ParameterizedValue =            Parameterized | Value,
        ParameterizedValueSet =         Parameterized | ValueSet, 
        ParameterizedObjectClass =      Parameterized | ObjectClass, 
        ParameterizedObject =           Parameterized | Object,
        ParameterizedObjectSet =        Parameterized | ObjectSet,

        TypeParameter =                 Parameter | Type,
        ValueParameter =                Parameter | Value,
        ValueSetParameter =             Parameter | ValueSet,
        ObjectClassParameter =          Parameter | ObjectClass,
        ObjectParameter =               Parameter | Object,
        ObjectSetParameter =            Parameter | ObjectSet,
        TypeOrClassParameter =          Parameter | TypeOrClass,
        ValueOrObjectParameter =        Parameter | ValueOrObject,
        ValueSetOrObjectSetParameter =  Parameter | ValueSetOrObjectSet,
        UnknownParameter =              Parameter | Unknown,

        TypeGovernor =                  Governor | Unknown,
        ClassGovernor =                 Governor | ObjectClass,
        TypeOrClassGovernor =           Governor | TypeOrClass,
        UnknownGovernor =               Governor | Unknown,

        UnknownDummy =                  Dummy | Unknown,

        TypeField =                     FieldX | Type,
        ValueField =                    FieldX | Value,
        ValueSetField =                 FieldX | ValueSet,
        ObjectField =                   FieldX | Object,
        ObjectSetField =                FieldX | ObjectSet,
    };

    class Symbol
    {
        String m_strName;                       // Name of the symbol
                                                //   Some internal names exist which have spaces in them
        SymbolType m_symType;                   // What kind of symbol is this
        public SymbolTable m_symTable;          // Pointer to containing symbol table if one exists.
        Type m_type;                            // What is the type of the symbol (if it has one and it is known)
        Symbol m_symParent;                     // For contained symbols - what is the parent of this symbol

        //  ADDITIONAL INFORMATION
        //
        //  For Symbols which are imported:
        //      The SymbolType.Imported is set in m_symType
        //      For conflicting names on imported symbols,
        //              The first symbol is inserted into the symbol table and SymbolType.NameConflict is set
        //              Subsequenct symbols are not in the symbol table
        //      

        public Symbol m_symModule;     // Pointer symbol in this symbol table for the module we imported from
        public AsnFile m_af;           // Pointer to the AsnFile object which defines the symbol
                                       // Note: definition could be an imported symbol in that module!

        //  For symbol type
        //      Module  - contains the list of symbols (if any) imported from that module
        //      
        //

        SymbolList m_symbolList;

        //  For symbol type
        //      * | Imported - Points to the symbol we imported this symbol from
        //

        public Symbol m_sym;

        //  For the symbol type
        //      * | Parameterized - parameterized something

        List<ParamItems> m_paramOptions;

        //  This is a list of symbols that make up subsections of this symbol.
        //

        SymbolList m_symlstParts;

        //
        //
        //

        FieldFlags m_flags = new FieldFlags();
        int m_iValue;
        Value m_value;
        Boolean m_fEmitted;
        string m_strFullFieldName;
        string m_strStructureName;
        int m_iDepth = 0;
        CodePointers m_codenames;
        string m_strFullSymbolName;         // The fully decorated symbol name
        bool m_fFromComponentOf = false;
        ViewerNode m_viewernode = null;

        public Symbol(SymbolType symType)
        {
            m_symType = symType;
            m_fEmitted = false;
        }

        public Symbol(String str, SymbolType symType)
        {
            m_strName = str;
            m_symType = symType;
            m_fEmitted = false;
        }

        public Symbol(Symbol sym, SymbolType symType) :this(sym)
    {
        Debug.Assert((sym.symType & SymbolType.Parameter) != 0);
        m_symType = symType;

        }

        public Symbol(Symbol sym)
        {

            m_strName = sym.m_strName;
            m_symType = sym.m_symType;
            m_symTable = sym.m_symTable;
            m_type = sym.m_type;
            m_symModule = sym.m_symModule;
            m_af = sym.m_af;
            m_symbolList = sym.m_symbolList;
            m_sym = sym.m_sym;
            m_flags = sym.m_flags;
            m_iValue = sym.m_iValue;
            m_value = sym.m_value;
            m_strFullFieldName = sym.m_strFullFieldName;
            m_strStructureName = sym.m_strStructureName;
            m_iDepth = sym.m_iDepth;
            m_codenames = sym.m_codenames;
            m_strFullSymbolName = sym.m_strFullSymbolName;
            m_fFromComponentOf = false;
            m_symlstParts = new SymbolList();
        }

        public CodePointers codenames
        {
            get { 
                if (m_codenames == null) m_codenames = new CodePointers((Program.UseConst != 0) ? "const " : "");
                return m_codenames;
            }
        }

        public Boolean emitted
        {
            get { return m_fEmitted; }
            set { m_fEmitted = value; }
        }

        public String CSharpName
        {
            get {
                String str = "";
                if (m_symTable != null) str = m_symTable.m_module.nameSpace;
                if (m_symParent != null) str = m_symParent.CSharpName;
                if (str != "") str += ".";

                switch (m_symType) {
                case SymbolType.Type: return str + Clean (m_strName);
                case SymbolType.Value: return str + "values." + Clean (m_strName);
                case SymbolType.Object: return str + "objects." + Clean (m_strName);
                case SymbolType.ObjectClass: return str + Clean (m_strName);
                case SymbolType.ObjectSet: return str + "objects." + Clean (m_strName);
                case SymbolType.InList | SymbolType.Value: return m_type.CSharpName + ".values." + Clean (m_strName);

                default:
                    return Clean (m_strName) + "*Missing case*";
                }
            }
        }

        private String Clean (String str)
        {
            return str.Replace ('-', '_').Replace("&", null);
        }

        public FieldFlags flags
        {
            get { return m_flags; }
            set { m_flags = value; }
        }

        public int groupVersion
        {
            get { 
                Debug.Assert((m_symType == SymbolType.ExtensionGroup)); 
                return m_iValue; 
            }
            set { 
                Debug.Assert((m_symType == SymbolType.ExtensionGroup) || (m_symType == SymbolType.Object)); 
                m_iValue = value; 
            }
        }

        public bool FromComponentOf
        {
            get { return m_fFromComponentOf; }
            set { m_fFromComponentOf = value; }
        }

        public AsnFile module
        {
            get { return m_symTable.module; }
        }

        public String name
        {
            get { return m_strName; }
            set { m_strName = value; }
        }

        public String fullName
        {
            get { 
                //  If we have been explicit about the name - return it.
                if (m_strFullSymbolName != null) return m_strFullSymbolName;
                if ((m_symTable != null) && (module != null)) return module.nameSpace + m_strName;
                return m_strName;
            }
            set { m_strFullSymbolName = value; }
        }

        public SymbolList symlstParts
        {
            get { if (m_symlstParts == null) { m_symlstParts = new SymbolList(); } return m_symlstParts; }
        }

        public SymbolList symList
        {
            get {
                Debug.Assert((m_symType == SymbolType.ExtensionGroup) || (m_symType == (SymbolType.Module|SymbolType.Imported)) ||
                    (m_symType == SymbolType.Object) || (m_symType == SymbolType.Relationship));
                return m_symbolList;
            }
            set {
                Debug.Assert((m_symType == SymbolType.ExtensionGroup) || (m_symType == (SymbolType.Module|SymbolType.Imported)) || 
                    (m_symType == SymbolType.Object) || (m_symType == SymbolType.Relationship)); 
                m_symbolList = value;
            }
        }

        public SymbolList paramList
        {
            get { return m_symbolList; }
            set { m_symbolList = value; }
        }

        public int Value
        {
            get { return m_iValue; }
            set { m_iValue = value; }
        }

        public Value value
        {
            get { return m_value; }
            set { m_value = value; }
        }


        public SymbolType symType
        {
            get { return m_symType; }
            set { m_symType = value; }
        }

        public Type type
        {
            get { return m_type; }
            set { m_type = value; }
        }

        public int depth
        {
            get { return m_iDepth; }
            set { m_iDepth = value; }
        }


        public String fieldName
        {
            get { return m_strFullFieldName; }
            set { m_strFullFieldName = value; }
        }

        public string structureName
        {
            get { return m_strStructureName; }
            set { m_strStructureName = value; }
        }

        public List<ParamItems> parameterization
        {
            get { return m_paramOptions; }
            set { m_paramOptions = value; }
        }

        public Symbol parent
        {
            get { return m_symParent; }
            set { m_symParent = value; }
        }

        public ViewerNode ViewerNode
        {
            get { return m_viewernode; }
            set { m_viewernode = value; }
        }

        public void Defined()
        {
            Debug.Assert((m_symType & SymbolType.NotYetDefined) != 0);
            m_symType &= ~SymbolType.NotYetDefined;
        }

        public SymbolType GetSymbolType() { return m_symType; }

        public Symbol Instance(SymbolList paramList)
        {
            Symbol newSym = new Symbol(m_symType);

            //
            //  Copy over elements of the symbol
            //

            newSym.m_strName = m_strName;

            newSym.m_symTable = m_symTable;

            if (m_flags != null) newSym.m_flags = m_flags.Instance(paramList);

            newSym.m_iValue = m_iValue;

            if (m_type != null) {
                newSym.m_type = m_type.Instance(paramList);
            }

            if (m_value != null) {
                newSym.m_value = m_value.Instance(paramList);
            }

            if (m_symbolList != null) {
                newSym.m_symbolList = m_symbolList.Instance(paramList);
            }

            //  Ignore import items?

            if (m_sym != null) {
                newSym.m_sym = m_sym;
            }

            newSym.m_flags = m_flags;

            return newSym;
        }

        /// <summary>
        /// Convert a template into an instance a type structure
        /// </summary>
        /// <param name="paramList">List of parameters and data to replace in template</param>
        /// <returns>new Type instance</returns>
        public Type InstanceOfType(SymbolList paramList)
        {
            //
            //  We only deal with parameterized types here
            //

            if (m_symType != SymbolType.ParameterizedType) {
                Debug.Assert(false);
                return null;
            }

            Debug.Assert(parameterization != null);
            Debug.Assert(parameterization.Count == 1);
            Debug.Assert(parameterization[0].type.baseType == BaseType.Param_Assignment);


            return parameterization[0].type.Instance(paramList);
        }

        public Boolean IsBaseType(BaseType baseType)
        {
            if (m_type == null) {
                Debug.Assert(false);
                return false;
            }

            return (m_type.baseType == baseType);
        }

        public Boolean IsTagged()
        {
            Type type = m_type;

            while (type != null) {
                switch (type.baseType) {
                case BaseType.Tagged:
                    return true;

                case BaseType.Constraint:
                case BaseType.NamedList:
                    type = type.subType;
                    break;

                case BaseType.ObjectClassField:
                case BaseType.Symbol:   //  If we hit a symbol, then it was not tagged.
                    return false;

                default:
                    Debug.Assert(false);
                    return false;
                }
            }

            Debug.Assert(false);
            return false;
        }

        public Boolean IsType(SymbolType symType)
        {
            return m_symType == symType;
        }

        public Boolean IsBaseType(SymbolType symType)
        {
            return (m_symType & SymbolType.BaseMask) == symType;
        }

        public void Print()
        {
            Debug.Print(m_strName + "\t" + m_symType.ToString());
        }

        public override string ToString()
        {
            String str;

            str = m_strName + " Type: " + m_symType;

            return str;
        }


        public void Collapse()
        {
            switch (m_symType) {
            case SymbolType.Value:
                m_value.Collapse();
                break;

            case SymbolType.ObjectClass:
            case SymbolType.Type:
                m_type.Collapse();
                break;

            case SymbolType.Object:
                m_value.Collapse();
                break;

            case SymbolType.ObjectSet:
                m_value.Collapse();
                break;

            case SymbolType.ObjectSetParameter:
                break;

            case SymbolType.NotYetDefined | SymbolType.Value:
                if (m_value != null) {
                    m_value.Collapse();
                }
                break;

            default:
                Debug.Assert(false);
                break;
            }
        }

        public void Dump(String tab)
        {
            Console.Error.WriteLine(tab+m_strName);
            switch (m_symType) {
            case SymbolType.Module:
                Console.Error.WriteLine(tab+"    Module");
                break;

            case SymbolType.Type:
                Console.Error.WriteLine(tab+"    Type");
                m_type.Dump(tab+"    ");
                break;

            case SymbolType.ParameterizedType:
                Console.Error.WriteLine(tab + "    Parameterized Type");
                for (int i = 0; i < parameterization.Count; i++) {
                    Console.Error.WriteLine(tab + "        Parameters " + i.ToString());
                    parameterization[i].paramList.Dump(tab + "            ");
                    Console.Error.WriteLine(tab + "        Type");
                    parameterization[i].type.Dump(tab + "            ");
                }
                break;


            case SymbolType.Value:
                Console.Error.WriteLine(tab + "    Value");
                if (m_type != null) m_type.Dump(tab + "    ");
                if (m_value != null) m_value.Dump(tab + "    ");
                break;

            case SymbolType.Component:
                Console.Error.WriteLine(tab + "    Field");
                m_type.Dump(tab + "    ");
                break;

            case SymbolType.TypeField:
                Console.Error.WriteLine(tab + "    Type Field");
                break;

            case SymbolType.ValueField:
                Console.Error.WriteLine(tab + "    Value Field");
                m_type.Dump(tab + "    ");
                break;

            case SymbolType.TypeParameter:
                Console.Error.WriteLine(tab + "    Type Parameter");
                break;

            case SymbolType.ValueParameter:
                Console.Error.WriteLine(tab + "    Value Parameter");
                if (m_type != null) m_type.Dump(tab + "    ");
                break;

            case SymbolType.Extension:
                Console.Error.WriteLine(tab + "...");
                break;

            case SymbolType.ExtensionGroup:
                Console.Error.WriteLine(tab + "[[   {0}", m_iValue);
                foreach (Symbol sym in m_symbolList) sym.Dump(tab + "    ");
                Console.Error.WriteLine(tab + "]]");
                break;

            case SymbolType.Exception:
                Console.Error.WriteLine(tab + "!");
                m_value.Dump(tab + "    ");
                break;

            case SymbolType.Object:
                Console.Error.WriteLine(tab + "    Object");
                m_value.Dump(tab + "    ");
                break;

            case SymbolType.ObjectClass:
                Console.Error.WriteLine(tab + "    Object Class");
                m_type.Dump(tab + "    ");
                break;

            case SymbolType.ObjectSet:
                Console.Error.WriteLine(tab + "    Object Set");
                m_value.Dump(tab + "    ");
                break;

            case SymbolType.Relationship:
                Console.Error.WriteLine(tab + "    " + m_symType.ToString());
                if (m_sym != null) m_sym.Dump(tab + "    ");
                break;

            default:
                Console.Error.WriteLine(tab + "    *" + m_symType.ToString());
                break;
            }
        }

        public void PreEmit(CodeEmitter emitter)
        {
            emitter.PreEmitSymbol(this);
        }

        public void Emit(CodeEmitter emitter)
        {
            //  We are going to emit the symbol now - assume that all fixed fluff has already
            //      been dumped out the door.

            //  If this is a complex type, emit everything that it depends on first

            //  Now ask the Emitter we are using to emit this symbol

            emitter.EmitSymbol(this);
        }

        public void SetConflict()
        {
            m_symType |= SymbolType.NameConflict;
        }

        public void SetImport(Symbol symImport)
        {
            Debug.Assert((symImport.m_symType & SymbolType.BaseMask) != SymbolType.Unknown);
            Debug.Assert((m_symType & SymbolType.Imported) != 0);
            
            m_sym = symImport;
        }

        public void SetImportSource(Symbol symModule, AsnFile af)
        {
            m_symModule = symModule;
            m_af = af;
        }
    }

    class SymbolList : IEnumerable<Symbol>
    {
        List<Symbol> m_lst;


        public SymbolList()
        {
            m_lst = new List<Symbol>();
        }

        public SymbolList(Symbol sym)
        {
            m_lst = new List<Symbol>();
            m_lst.Add(sym);
        }

        public SymbolList(List<Symbol> lst)
        {
            m_lst = lst;
        }

        public SymbolList(SymbolList lst)
        {
            m_lst = new List<Symbol>();
            foreach (Symbol sym in lst.m_lst) {
                m_lst.Add(sym);
            }
        }

        public Symbol this [int i] 
        {
            get { return m_lst[i]; }
        }

        public int Count { get { return m_lst.Count; } }

        public List<Symbol> list
        {
            get { return m_lst; }
        }

        public void Add(Symbol sym)
        {
            m_lst.Add(sym);
        }

        public void AddRange(SymbolList lst)
        {
            m_lst.AddRange(lst.m_lst);
        }

        public void Dump(String tab)
        {
            foreach (Symbol sym in m_lst) sym.Dump(tab);
        }

        /// <summary>
        /// Return a new symbol list with the parameters replaced
        /// </summary>
        /// <param name="paramList">List of parameters to replace</param>
        /// <returns>New symbol list</returns>
        public SymbolList Instance(SymbolList paramList)
        {
            SymbolList lst = new SymbolList();
            foreach(Symbol sym in m_lst) {
                lst.Add(sym.Instance(paramList));
            }
            return lst;
        }

        public void Insert(int i, Symbol sym)
        {
            m_lst.Insert(i, sym);
        }

        public void Insert(int i, SymbolList symlst)
        {
            if (symlst.Count == 1) {
                m_lst.Insert(i, symlst[0]);
            }
            else {
                for (int i1 = symlst.Count - 1; i1 >= 0; i1--) {
                    m_lst.Insert(i, symlst[i1]);
                }
            }
        }

        public void Remove(Symbol sym)
        {
            m_lst.Remove(sym);
        }

        public IEnumerator<Symbol> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

    }

    class SymbolListList : IEnumerable<SymbolList>
    {
        List<SymbolList> m_lst;


        public SymbolListList()
        {
            m_lst = new List<SymbolList>();
        }

        public SymbolListList(SymbolList symlst)
        {
            m_lst = new List<SymbolList>();
            m_lst.Add(symlst);
        }

        public SymbolListList(List<SymbolList> lst)
        {
            m_lst = lst;
        }

        public SymbolListList(SymbolListList lst)
        {
            m_lst = lst.m_lst;
        }

        public SymbolList this[int i]
        {
            get { return m_lst[i]; }
        }

        public int Count { get { return m_lst.Count; } }

        public List<SymbolList> list
        {
            get { return m_lst; }
        }

        public void Add(SymbolList symlst)
        {
            m_lst.Add(symlst);
        }

        public void Dump(String tab)
        {
            foreach (SymbolList sym in m_lst) sym.Dump(tab);
        }

#if false
        /// <summary>
        /// Return a new symbol list with the parameters replaced
        /// </summary>
        /// <param name="paramList">List of parameters to replace</param>
        /// <returns>New symbol list</returns>
        public SymbolListList Instance(SymbolListList paramList)
        {
            SymbolListList lst = new SymbolListList();
            foreach (SymbolList sym in m_lst) {
                lst.Add(sym.Instance(paramList));
            }
            return lst;
        }
#endif

        public void Insert(int i, SymbolList sym)
        {
            m_lst.Insert(i, sym);
        }

        public void Insert(int i, SymbolListList symlst)
        {
            if (symlst.Count == 1) {
                m_lst.Insert(i, symlst[0]);
            }
            else {
                for (int i1 = symlst.Count - 1; i1 >= 0; i1--) {
                    m_lst.Insert(i, symlst[i1]);
                }
            }
        }

        public void Remove(SymbolList sym)
        {
            m_lst.Remove(sym);
        }

        public IEnumerator<SymbolList> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }
    }

    class SymbolTable : IEnumerable<Symbol>
    {
        //
        //  Variables
        //
        
        public AsnFile m_module;
        Dictionary<string, Symbol> m_table = new Dictionary<string, Symbol>();

        //
        //  accessors
        //

        public AsnFile module { get { return m_module; } }

        //
        //  Functions
        //

        public Symbol Add(String strName, SymbolType symType)
        {
            Symbol sym = new Symbol(strName, symType);
            m_table.Add(strName, sym);
            sym.m_symTable = this;
            return sym;
        }

        public void Add(Symbol sym)
        {
            m_table.Add(sym.name, sym);
            sym.m_symTable = this;
        }

        public void Collapse()
        {
            foreach (KeyValuePair<string, Symbol> kvp in m_table) {
                kvp.Value.Collapse();
            }
        }

        public void Dump()
        {
            foreach (KeyValuePair<string, Symbol> kvp in m_table) {
                kvp.Value.Dump("");
            }
        }

        public MatchResult PreEmit(CodeEmitter emitter)
        {
            foreach (Symbol sym in m_table.Values) {
                sym.PreEmit(emitter);
            }
            return MatchResult.Success;
        }

#if EMIT_RANDOM_ORDER
        public static Random rnd = new Random(888);
#endif

        public MatchResult Emit(CodeEmitter emitter)
        {
#if EMIT_RANDOM_ORDER
            Symbol[] rgsym = new Symbol[m_table.Values.Count];
            int i = 0;

            foreach (Symbol sym in m_table.Values) {
                rgsym[i++] = sym;
            }

            for (i=0; i<m_table.Values.Count; i++) {
                rgsym[rnd.Next(m_table.Values.Count-1)].Emit(emitter);
            }
#endif
            foreach (Symbol sym in m_table.Values) {
                sym.Emit(emitter);
            }
            return MatchResult.Success;
        }

        public Symbol Find(String strName)
        {
            Symbol sym;
            
#if true
            if (m_table.TryGetValue(strName, out sym)) {
                while (sym.GetSymbolType() == SymbolType.Imported) {
                    sym = sym.m_sym;
                }
            }

            return sym;
#else
            try {
                
                sym = m_table[strName];
                while (sym.GetSymbolType() == SymbolType.Imported) {
                    sym = sym.m_sym;
                }
                return sym;
            }
            catch (KeyNotFoundException) {
                return null;
            }
#endif
        }

        public Symbol Find(Symbol sym)
        {
            return Find(sym.name);
        }

        public void Print()
        {
            foreach (KeyValuePair<string, Symbol> kvp in m_table) {
                kvp.Value.Print();
            }
        }

        public IEnumerator<Symbol> GetEnumerator()
        {
            return m_table.Values.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_table.Values.GetEnumerator();
        }

    }

    class ParamItems {

        SymbolList m_symlst;    //  Parameter list for this item

        //  Only one of these two items should ever be set.

        Type m_type;            //  If not null - then the type structure assoicated with the parameters
        Value m_value;          //  If not null - then the value stucture associated with the parameters

        public ParamItems(SymbolList symlstParameters, Type type)
        {
            m_symlst = symlstParameters;
            m_type = type;
        }

        public ParamItems(SymbolList symlstParameters, Value val)
        {
            m_symlst = symlstParameters;
            m_value = val;
        }

        public SymbolList paramList { get { return m_symlst; } }
        public Type type { get { return m_type; } }
    }
}
