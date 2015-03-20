    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Diagnostics;
    using System.Text;

    namespace asn_compile_cs
    {
        class CodePointers
        {
            public CodePointers(String sz) { m_szConst = sz; }

            enum EmissionState { Unknown = 0, NotStarted, Started, StructNameSet, StructEmitted, FunctionsEmitted, Done };

            String m_strStructName;                     // Name of the emitted structure for this item
            
            Boolean m_fPtrEmitted = false;              // Have I emitted a pointer for this structure?

            EmissionState m_state = EmissionState.NotStarted; //  Where are we in the state of emitting this item.

            //
            //  Does this type need a sub table in order to fully describe the structure.
            //  For example a simple integer does not it one, but a structure does for the field descriptions
            //

            Boolean m_fSubTable = false;
            
            //
            //  These two variables represent the cases where we have "funny" names for either the 
            //  function table variable (descriptor) or the type description table (description).  If
            //  they are not set, then the name of the structure is appended with the correct tag.
            //
            //  These two items may be different for things like open types where the A2C_OPEN_TYPE description table is use,
            //  but the descriptor table is defined to use the correct constraints set.
            //

            String m_strDescriptorBase;
            String m_strDescriptionBase;
            
            //
            //  M00TODO - distinguish between these two fields
            //

            Boolean m_fEnums = false;                   //  Does this item have named sub-items
            Boolean m_fNamed = false;                   //  Does this item have named sub-items

            //
            //  Override the function names -
            //      Our function names are those that are assigned inside of the program and are not the default names
            //      User function names are those that are assigned by the user to a type.
            //

            Dictionary<String, String> m_mpUserFunctionNames = new Dictionary<string, string>();
            Dictionary<String, String> m_mpOurFunctionNames = new Dictionary<string, string>();

            //
            //  If desired, the use can have const emitted in front of everything, in that case m_szConst == "const"
            //

            String m_szConst;

            //
            //  Users have the option of specifying another type to be used for this structure.
            //

            String m_strUserType;

            public Boolean NotStarted { get { return (m_state == EmissionState.NotStarted); } }
            public Boolean IsEmitting { get { return (m_state != EmissionState.NotStarted) && (m_state != EmissionState.Done); } }
            public Boolean IsEmitted { get { return m_state == EmissionState.Done; } }
            public Boolean IsPtrEmitted { get { return m_fPtrEmitted; } }
            public String PDU_Name { get { return ((m_strUserType == null) ? DescriptorBase : /*m_strUserType*/ DescriptorBase ) + "_descriptor"; } }
            public String StructName { get { return m_strStructName; } }
            public Boolean SubTable { set { m_fSubTable = value; } }
            public String DescriptorBase { get { return (m_strDescriptorBase != null) ? m_strDescriptorBase : m_strStructName; } }
            public Boolean named { set { m_fNamed = value; } }

            public void Done() { m_state = EmissionState.Done; }
            public void Started() { m_state = EmissionState.Started; }

            /// <summary>
            /// Emit functions that can be used to make things simplier for the users of the system.
            /// These functions are used to give a function call that has all of the needed internal items to run A2C.  This means
            /// that one call call A2C_EncodeBer_Foo to deal with the type Foo rather than having to know the name of th encoders.
            /// These functions can be suppressed, emitted as inline functions, or emitted as normal functions depending on the
            /// compiler option used.
            /// </summary>
            /// <param name="stmC">source file output stream</param>
            /// <param name="stmH">header file output stream</param>
            /// <param name="args">Command line argument structure</param>
            public void EmitInlines(EmitterStream stmC, EmitterStream stmH, CmdLineArgs args)
            {
                string strInline = "";

                //
                //  Never emit anything for classes, only emit for types.
                //

                if (m_strDefaultFunction == "A2C_CLASS") return;

                //
                //  If we are to supress emission - then we can just return
                //

                if (args.iInline == -1) return;

                //
                //  If we are emitting for inline, then we want to
                //  1. emit with the inline tag (depends on compiler so use a macro)
                //  2. emit all items into the header file.
                //

                if (args.iInline != 0) {
                    strInline = "A2C_INLINE ";
                    stmC = stmH;
                }

                //
                //  Emit function headers if we are not doing the inline thing
                //

                if (args.iInline == 0) {
                    stmH.WriteLine("A2C_ERROR A2C_Alloc_{0}({0} ** p);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Free_{0}({0} * p);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Init_{0}({0} * p);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Release_{0}({0} * p);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_EncodeBer_{0}({0} const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_DecodeBer_{0}({0} ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_EncodeDer_{0}({0} const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_DecodeDer_{0}({0} ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Copy_{0}({0} * pDst, {0} const * pSrc);", m_strStructName);
                    stmH.WriteLine("int A2C_Compare_{0}({0} const * pLeft, {0} const * pRight);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Print_{0}({0} * p, PA2C_STREAM pstm);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Validate_{0}({0} * p, int * pf);", m_strStructName);
                    stmH.WriteLine("A2C_ERROR A2C_Validate2_{0}({0} * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm);", m_strStructName);
                }

                //
                //  Emit the functions
                //

                stmC.WriteLine("{1}A2C_ERROR A2C_Alloc_{0}({0} ** p) {{ return A2C_Alloc((PVOID *) p, &{0}_descriptor); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Free_{0}({0} * p) {{ return A2C_Free((PVOID) p, &{0}_descriptor); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Init_{0}({0} * p) {{ return A2C_Init((PVOID) p, &{0}_descriptor); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Release_{0}({0} * p) {{ return A2C_Release((PVOID) p, &{0}_descriptor); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_EncodeBer_{0}({0} const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) {{ return A2C_EncodeBer((PCVOID) p, &{0}_descriptor, flags, ppcxt, pstm); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_DecodeBer_{0}({0} ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) {{ return A2C_DecodeBer((PVOID *) p, &{0}_descriptor, flags, ppcxt, pb, cb); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_EncodeDer_{0}({0} const * p, int flags, A2C_CONTEXT ** ppcxt, PA2C_STREAM pstm) {{ return A2C_EncodeDer((PVOID) p, &{0}_descriptor, flags, ppcxt, pstm); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_DecodeDer_{0}({0} ** p, int flags, A2C_CONTEXT ** ppcxt, PCBYTE pb, size_t cb) {{ return A2C_DecodeDer((PVOID *) p, &{0}_descriptor, flags, ppcxt, pb, cb); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Copy_{0}({0} * pDst, {0} const * pSrc) {{ return A2C_Copy((PVOID) pDst, (PCVOID) pSrc, &{0}_descriptor); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}int A2C_Compare_{0}({0} const * pLeft, {0} const * pRight) {{ return A2C_Compare((PCVOID) pLeft, (PCVOID) pRight, &{0}_descriptor); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Print_{0}({0} * p, PA2C_STREAM pstm) {{ return A2C_Print((PVOID) p, &{0}_descriptor, pstm); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Validate_{0}({0} * p, int * pf) {{ return A2C_Validate((PVOID) p, &{0}_descriptor, pf); }}", m_strStructName, strInline);
                stmC.WriteLine("{1}A2C_ERROR A2C_Validate2_{0}({0} * p, A2C_CONTEXT ** ppcxt, int * pf, PA2C_STREAM pstm) {{ return A2C_Validate2((PVOID) p, &{0}_descriptor, ppcxt, pf, pstm); }}", m_strStructName, strInline);

                //
                //  If these need arrays for support - then emit the headers and functions to do the array support
                //

                if ((m_strDefaultFunction == "A2C_SET_OF") || (m_strDefaultFunction == "A2C_SEQUENCE_OF")) {
                    if (args.iInline == 0) {
                        stmH.WriteLine("A2C_ERROR A2C_AllocArray_{0}({0} * p, int c);", m_strStructName);
                        stmH.WriteLine("A2C_ERROR A2C_FreeArray_{0}({0} * p);", m_strStructName);
                        stmH.WriteLine("A2C_ERROR A2C_ReallocArray_{0}({0} * p, int c);", m_strStructName);
                    }

                    stmC.WriteLine("{1}A2C_ERROR A2C_AllocArray_{0}({0} * p, int c) {{ return A2C_AllocArray((A2C_SEQUENCE_OF *) p, &{0}_descriptor, c); }}", m_strStructName, strInline);
                    stmC.WriteLine("{1}A2C_ERROR A2C_FreeArray_{0}({0} * p) {{ return A2C_FreeArray((A2C_SEQUENCE_OF *) p, &{0}_descriptor); }}", m_strStructName, strInline);
                    stmC.WriteLine("{1}A2C_ERROR A2C_ReallocArray_{0}({0} * p, int c) {{ return A2C_ReallocArray((A2C_SEQUENCE_OF *) p, &{0}_descriptor, c); }}", m_strStructName, strInline);
                }

                //
                //  If there are names associated with the data type - emit functions to support named lookup
                //

                if (m_fNamed) {
                    if (args.iInline == 0) {
                        stmH.WriteLine("A2C_ERROR A2C_List_{0}(A2C_NAMES const ** p);", m_strStructName);
                        stmH.WriteLine("A2C_ERROR A2C_NameToInt_{0}(char const * szName, int * pi);", m_strStructName);
                        stmH.WriteLine("A2C_ERROR A2C_IntToName_{0}(int i, const char ** pszName);", m_strStructName);
                    }

                    stmC.WriteLine("{1}A2C_ERROR A2C_List_{0}(A2C_NAMES const ** p) {{ return A2C_List(&{0}_descriptor, p); }}", m_strStructName, strInline);
                    stmC.WriteLine("{1}A2C_ERROR A2C_NameToInt_{0}(char const * szName, int * pi) {{ return A2C_NameToInt(&{0}_descriptor, szName, pi); }}", m_strStructName, strInline);
                    stmC.WriteLine("{1}A2C_ERROR A2C_IntToName_{0}(int i, const char ** pszName) {{ return A2C_IntToName(&{0}_descriptor, i, pszName); }}", m_strStructName, strInline);
                }
            }

            /// <summary>
            /// Emit the Protocol Data Unit for this type.
            /// </summary>
            /// <param name="stm">The file to emit the description into</param>
            /// <param name="fBaseType">Is this an ASN.1 type, or a sub-piece of an ASN.1 type?</param>
            public void EmitPDU(EmitterStream stm, Boolean fBaseType)
            {
                //
                //  If we have aleady emitted the base, or we have inherited a base from a derived type then
                //  we don't need to emit the base at this point
                //
                //  The structure we are filling looks like this
                //
                //  typedef struct {
                //    int             cbStruct;               /* Size of the data structure */
                //                                            /*     (real data structure not me) */
                //    int             flags;
                //    int             cChildren;              /* Count of children */
                //    A2C_ITEM const* rgChildren;             /* Description of each child field */
                //    A2C_NAMES       names;                  /* Names for named items */
                // } A2C_ITEM_HEADER;
                //

                if (m_strDescriptionBase == null) {
                    stm.WriteLine();
                    stm.WriteLine( m_szConst + "A2C_ITEM_HEADER {0}_description = {{", m_strStructName);

                    //
                    //  Don't need the size if this is not the lhs of an ASN.1 production
                    //

                    if (fBaseType) {
                        stm.Write("  sizeof({0}),", m_strStructName);
                    }
                    else {
                        stm.Write("  0,");
                    }

                    stm.Write(" 0,");  // Flags - future extensible flag goes here

                    //
                    //  Do we have a subordinate table we need to reference for this type
                    //

                    if (m_fSubTable) {
                        stm.Write(" countof({0}_description_table),", m_strStructName);
                        stm.Write(" {0}_description_table", m_strStructName);
                    }
                    else {
                        stm.Write(" 0, 0");
                    }

                    //
                    //  If there are enumerations - write out the enum table name and size
                    //

                    if (m_fEnums) {
                        stm.Write(", countof({0}_name_table), {0}_name_table", m_strStructName);
                    }
                    else {
                        stm.Write(", 0, 0");
                    }

                    stm.WriteLine();
                    stm.WriteLine("};");

                    //
                    //  The name of the discriptor table is based on the structure name
                    //

                    m_strDescriptionBase = m_strStructName;
                }

                //
                //  Write out the function dispatch table
                //

                stm.WriteLine();
                stm.WriteLine( m_szConst +"A2C_DESCRIPTOR {0} = {{", PDU_Name);
                stm.WriteLine("    {0}, {1}, ", GetFunctionName("Init"), GetFunctionName("Release"));
                stm.WriteLine("    (A2C_decoder_f) {0}, (A2C_encoder_f) {1},", GetFunctionName("DecodeDER"), GetFunctionName("EncodeDER"));
                stm.WriteLine("    (A2C_decoder_f) {0}, (A2C_encoder_f) {1},", GetFunctionName("DecodeBER"), GetFunctionName("EncodeBER"));
                stm.WriteLine("    (A2C_compare_f) {0},", GetFunctionName("Compare"));
                stm.WriteLine("    (A2C_copy_f) {0},", GetFunctionName("Copy"));
                stm.WriteLine("    (A2C_print_f) {0},", GetFunctionName("Print"));
                stm.WriteLine("    (A2C_validate_f) {0},", GetFunctionName("Validate"));
                stm.WriteLine("    &{0}_description", /*m_strStructName*/ m_strDescriptionBase);
                stm.WriteLine("};");
            }

            public void EmitPointer(EmitterStream stm)
            {
                if (!m_fPtrEmitted) {
                    stm.WriteLine("typedef struct _{0} * P{0};", m_strStructName);
                    m_fPtrEmitted = true;
                }
            }

            public void SetDescriptorName(String str) { m_strDescriptorBase = str; }
            public void SetDescriptionName(String str) { m_strDescriptionBase = str; }

            /// <summary>
            /// Return the name of the function to be used for an operation.  We will use the following tables
            /// to determine the name of the function we are going to be using:
            /// 1.  Any name assigned by the user (these will be inherited for TypeA=TypeB)
            /// 2.  Names that we assign internally for functions generated by the compiler
            /// 3.  A default name consisting of the type name followed by 
            /// </summary>
            /// <param name="strTag">Which function name to return</param>
            /// <returns>Name of an A2C runtime function</returns>
            string GetFunctionName(String strTag)
            {
                //
                //  If the user specified a value use it
                //  If value is not in the table, then catch the exception that is thrown.
                //

                try {
                    return m_mpUserFunctionNames[strTag];
                }
                catch {
                }

                //
                //  If we specified a value -- use it
                //

                try {
                    return m_mpOurFunctionNames[strTag];
                }
                catch {
                }

                //
                //  Use the default name
                //

                switch (strTag) {
                case "DecodeBER": return m_strDefaultFunction + "_decode_ber";
                case "EncodeBER": return m_strDefaultFunction + "_encode_ber";
                case "DecodeDER": return m_strDefaultFunction + "_decode_der";
                case "EncodeDER": return m_strDefaultFunction + "_encode_der";
                case "Compare": return m_strDefaultFunction + "_compare";
                case "Copy": return m_strDefaultFunction + "_copy";
                case "Print": return m_strDefaultFunction + "_print";
                case "Validate": return m_strDefaultFunction + "_validate";
                case "Init": return Init;
                case "Release": return Release;
                }

                //
                //  Sombody asked for a bad function name.
                //

                Debug.Assert(false);
                return "##$(";
            }

            public bool HasUserType
            {
                get { return m_strUserType != null; }
            }
            public string UserType
            {
                get { return m_strUserType; }
            }

            public void SetUserFunctionName(String strTag, String strName)
            {
                m_mpUserFunctionNames[strTag] = strName;
            }

            public void SetOurFunctionName(String strTag, String strName)
            {
                m_mpOurFunctionNames[strTag] = strName;
            }

            public void SetStructureName(String str) 
            {
                Debug.Assert((m_strStructName == null) || (m_strStructName == str));

                m_strStructName = str;
               //  m_state = EmissionState.StructNameSet;
            }

            public void SetUserType(String str)
            {
                Debug.Assert(m_strUserType == null);
                m_strUserType = str;
            }

            String m_strDefaultName;
            String m_strDefaultFunction;

            public String Description { get { return (m_strDefaultName == null) ? null  : m_strDefaultName + "_description"; } }
            public String DefaultName { get { return m_strDefaultName; } }
            public Boolean NamedItems { set { m_fEnums = value; } }

            /// <summary>
            /// Return the internal runtime funtion name used to initialize the type. The name of the function
            /// to be used is dependent on the data type that we are using.
            /// </summary>
            public String Init { 
                get {
                    switch (m_strDefaultFunction) {
                    case "A2C_CHOICE":
                    case "A2C_SEQUENCE":
                    case "A2C_SEQUENCE_OF":
                    case "A2C_SET":
                    case "A2C_SET_OF":
                    case "A2C_TAG":
                        return m_strDefaultFunction + "_init";

                    case "A2C_BMP_STRING":
                    case "A2C_CHARACTOR_STRING":
                    case "A2C_GENERAL_STRING":
                    case "A2C_GRAPHIC_STRING":
                    case "A2C_IA5_STRING":
                    case "A2C_NUMERIC_STRING":
                    case "A2C_PRINTABLE_STRING":
                    case "A2C_OBJECT_DESCRIPTOR":
                    case "A2C_OBJECT_IDENTIFIER":
                    case "A2C_RELATIVE_OBJECT_IDENTIFIER":
                    case "A2C_TELETEX_STRING":
                    case "A2C_UNIVERSAL_STRING":
                    case "A2C_UTF8_STRING":
                    case "A2C_VIDEOTEX_STRING":
                    case "A2C_VISIBLE_STRING":
                    case "A2C_GENERALIZED_TIME":
                    case "A2C_UTC_TIME":
                        return "A2C_string_init";

                    case "A2C_INTEGER_HUGE":
                    case "A2C_ENUMERATED_HUGE":
                        return "A2C_INTEGER_HUGE_init";
                    }
                    return "A2C_zero"; 
                } 
            }

            /// <summary>
            /// Return the internal runtime funtion name used to free/release the type. The name of the function
            /// to be used is dependent on the data type that we are using.
            /// </summary>
            public String Release
            {
                get
                {
                    switch (m_strDefaultFunction)
                    {
                        case "A2C_BIT_STRING":
                        case "A2C_BIT_STRING_NAMED":
                        case "A2C_OPEN_BIT_STRING":
                        case "A2C_CHOICE":
                        case "A2C_OPEN_TYPE":
                        case "A2C_OCTET_STRING":
                        case "A2C_OPEN_OCTET_STRING":
                        case "A2C_SEQUENCE":
                        case "A2C_SEQUENCE_OF":
                        case "A2C_SET":
                        case "A2C_SET_OF":
                        case "A2C_TAG":
                            return m_strDefaultFunction + "_release";

                        case "A2C_BMP_STRING":
                        case "A2C_CHARACTOR_STRING":
                        case "A2C_GENERAL_STRING":
                        case "A2C_GRAPHIC_STRING":
                        case "A2C_IA5_STRING":
                        case "A2C_NUMERIC_STRING":
                        case "A2C_PRINTABLE_STRING":
                        case "A2C_OBJECT_DESCRIPTOR":
                        case "A2C_OBJECT_IDENTIFIER":
                        case "A2C_RELATIVE_OBJECT_IDENTIFIER":
                        case "A2C_TELETEX_STRING":
                        case "A2C_UNIVERSAL_STRING":
                        case "A2C_UTF8_STRING":
                        case "A2C_VIDEOTEX_STRING":
                        case "A2C_VISIBLE_STRING":
                        case "A2C_GENERALIZED_TIME":
                        case "A2C_UTC_TIME":
                            return "A2C_string_release";

                        case "A2C_INTEGER_HUGE":
                        case "A2C_ENUMERATED_HUGE":
                            return "A2C_INTEGER_HUGE_release";
                    }
                    return "A2C_nop";
                }
            }

            public void Clear()
            {
                m_mpOurFunctionNames = new Dictionary<string, string>();
                m_mpUserFunctionNames = new Dictionary<string, string>();
                m_strDefaultFunction = null;
                m_strDefaultName = null;
            }

            public void CopyFrom(CodePointers old)
            {
                if (m_strStructName == null) {
                    m_strStructName = old.m_strStructName;
                }
                m_state = old.m_state;

                m_strDefaultName = old.m_strDefaultName;
                m_strDefaultFunction = old.m_strDefaultFunction;
                m_strDescriptionBase = old.m_strDescriptionBase;

                m_mpOurFunctionNames = old.m_mpOurFunctionNames;
                //  Only override those things which the user has not already overridden.
                if (old.m_mpUserFunctionNames != null) {
                    foreach (KeyValuePair<string, string> kpv in old.m_mpUserFunctionNames) {
                        if (!m_mpUserFunctionNames.ContainsKey(kpv.Key)) {
                            m_mpUserFunctionNames.Add(kpv.Key, kpv.Value);
                        }
                    }
                }
                m_strUserType = old.m_strUserType;
            }


            public void SetDefaultFunction(String str)
            {
                m_strDefaultFunction = str;
            }

           
            public override string ToString()
            {
                return m_strDefaultName + " : " + m_strDefaultFunction;
            }
        }

        class EmitterStream
        {
            MemoryStream m_stmm;        // Where we really save the data
            StreamWriter m_stm;         // Where we write onto the memory stream
            bool m_fWrite;              // Are we currently writing ont the stream?

            public EmitterStream()
            {
                m_stmm = new MemoryStream();
                m_stm = new StreamWriter(m_stmm);
                m_fWrite = true;
            }

            public string NewLine
            {
                set { m_stm.NewLine = value; }
            }

            public bool Emit
            {
                get { return m_fWrite; }
                set { m_fWrite = value; }
            }

            public void Write(string str)
            {
                if (m_fWrite) m_stm.Write(str);
            }

            public void Write(string stm, object ob1)
            {
                if (m_fWrite) m_stm.Write(stm, ob1);
            }

            public void WriteLine()
            {
                if (m_fWrite) m_stm.WriteLine();
            }

            public void WriteLine(string fmt)
            {
                if (m_fWrite) m_stm.WriteLine(fmt);
            }

            public void WriteLine(string fmt, object ob1)
            {
                if (m_fWrite) m_stm.WriteLine(fmt, ob1);
            }

            public void WriteLine(string fmt, object ob1, object ob2)
            {
                if (m_fWrite) m_stm.WriteLine(fmt, ob1, ob2);
            }

            public void WriteLine(string fmt, object ob1, object ob2, object ob3)
            {
                if (m_fWrite) m_stm.WriteLine(fmt, ob1, ob2, ob3);
            }

            public void WriteLine(string fmt, object ob1, object ob2, object ob3, object ob4)
            {
                if (m_fWrite) m_stm.WriteLine(fmt, ob1, ob2, ob3, ob4);
            }

            public void WriteLine(string fmt, object ob1, object ob2, object ob3, object ob4, object ob5)
            {
                if (m_fWrite) m_stm.WriteLine(fmt, ob1, ob2, ob3, ob4, ob5);
            }

            public void WriteTo(FileStream fs)
            {
                m_stm.Flush(); fs.Write(m_stmm.GetBuffer(), 0, (int)m_stmm.Length);
            }
        }

    class C_CodeEmitter : CodeEmitter
        {

            String m_strFileName;
            String m_strBaseName;
            String SzConst;
            CmdLineArgs m_args;
            Stack<bool> m_stackEmit = new Stack<bool>();
            EmitterStream m_stm1;
            EmitterStream m_stm2;
            EmitterStream m_stm3;
            EmitterStream m_stm4;
            EmitterStream m_stm5;
            EmitterStream m_stm6;

            EmitterStream m_stmH;
            EmitterStream m_stmC;

            public C_CodeEmitter(string strFileName, CmdLineArgs args)
            {
                FileInfo fi;

                //
                //  Save the passed in base file name for later
                //

                m_strFileName = strFileName;

                //
                //  Save other parameters
                //

                m_args = args;

                //
                //  We have a file name passed in to us.
                //
                //  Delete the destination files if they exist
                //

                fi = new FileInfo(strFileName + ".h");
                if (fi.Exists) fi.Delete();

                fi = new FileInfo(strFileName + ".c");
                if (fi.Exists) fi.Delete();

                m_strBaseName = fi.Name.Substring(0, fi.Name.Length - 2);

                m_stm1 = new EmitterStream();
                m_stm2 = new EmitterStream();
                m_stm3 = new EmitterStream();
                m_stm4 = new EmitterStream();
                m_stm5 = new EmitterStream();
                m_stm6 = new EmitterStream();

                m_stmH = new EmitterStream();
                m_stmC = new EmitterStream();

    #if TRACE
                m_stm1.NewLine = "\n";
                m_stm2.NewLine = "\n";
                m_stm3.NewLine = "\n";
                m_stm4.NewLine = "\n";
                m_stm5.NewLine = "\n";
                m_stm6.NewLine = "\n";

                m_stmC.NewLine = "\n";
                m_stmH.NewLine = "\n";
    #endif

                if (Program.UseConst == 1) {
                    SzConst = "const ";
                }
            }

            override public void Close()
            {
                FileStream hFile = File.OpenWrite(m_strFileName + ".h");
                FileStream cFile = File.OpenWrite(m_strFileName + ".c");

                Debug.Assert(m_stackEmit.Count == 0);
                EmitHeader(cFile, hFile);

                //m_stmH.WriteTo(hFile);

                m_stm1.WriteTo(hFile);
                m_stm2.WriteTo(hFile);
                m_stm3.WriteTo(hFile);

                m_stmC.WriteTo(cFile);

                m_stm4.WriteTo(cFile);
                m_stm5.WriteTo(cFile);
                m_stm6.WriteTo(cFile);

                EmitFooter(cFile, hFile);

                hFile.Close();
                cFile.Close();
            }

            void PushEmit(Boolean fEmitSymbols)
            {
                m_stackEmit.Push(m_stm1.Emit);

                m_stm1.Emit = fEmitSymbols;
                m_stm2.Emit = fEmitSymbols;
                m_stm3.Emit = fEmitSymbols;
                m_stm4.Emit = fEmitSymbols;
                m_stm5.Emit = fEmitSymbols;
                m_stm6.Emit = fEmitSymbols;
            }

            void PopEmit()
            {
                bool fEmit = m_stackEmit.Pop();

                m_stm1.Emit = fEmit;
                m_stm2.Emit = fEmit;
                m_stm3.Emit = fEmit;
                m_stm4.Emit = fEmit;
                m_stm5.Emit = fEmit;
                m_stm6.Emit = fEmit;
            }

            static void EmitString(FileStream file, String str)
            {
                byte[] rgb = Encoding.UTF8.GetBytes(str+"\n");
                file.Write(rgb, 0, rgb.Length);
            }

            public void EmitHeader(FileStream cFile, FileStream hFile)
            {
                EmitString(hFile, "#include <A2C.h>");
                EmitString(hFile, String.Format("#ifndef __{0}__H__", CleanupName(m_strBaseName)));
                EmitString(hFile, String.Format("#define __{0}__H__", CleanupName(m_strBaseName)));
                EmitString(hFile, "");
                m_stmH.WriteTo(hFile);
                EmitString(hFile, "#ifdef __cplusplus");
                EmitString(hFile, "extern \"C\" {");
                EmitString(hFile, "#endif");
                EmitString(hFile, "");

                EmitString(cFile, String.Format("#include \"{0}.h\"", m_strBaseName));
                EmitString(cFile, "");
            }

            public void EmitFooter(FileStream cFile, FileStream hFile)
            {
                EmitString(hFile, "#ifdef __cplusplus");
                EmitString(hFile, "}");
                EmitString(hFile, "#endif");
                EmitString(hFile, String.Format("#endif /* __{0}__H__ */", CleanupName(m_strBaseName)));
            }

            static String CleanupName(String str)
            {
                if (str == null) return str;
                str = str.Replace("&", "");
                return str.Replace("-", "_");
            }

            public override void PreEmitSymbol(Symbol sym)
            {
                return;
            }

            override public void EmitSymbol(Symbol sym)
            {
                String str;
                List<String> lst;
                Boolean fPtr = false;

                //
                //  Symbol has already been emitted
                //

                if (sym.codenames.IsEmitted) return;

                //
                //  Should never come in here in the emitting state - should have been checked before comming here
                //

                if (sym.codenames.IsEmitting) {
                    // Debug.Assert(false, "Currently trying to emit " + sym.name + " And called recursively");
                    return;
                }

                //
                //  Is this symbol emitted?
                //
                
                PushEmit(sym.module.EmitSymbols);

                if (sym.codenames.NotStarted) {
                    sym.codenames.Started();
                }

                //
                //   Dump debug trace information if necessary.
                //

                if (Program.DebugLevel > 0) Console.Error.WriteLine("Emit Symbol: '{0}'", sym.name);

                //
                //  Based on the type of symbol, call the correct function to do the processing
                //

                switch (sym.GetSymbolType()) {
                case SymbolType.Module:
                    //  Emit an include file if necessary
                    if (sym.module.EmitStrings != null) {
                        if (sym.module.EmitStrings.ContainsKey("h")) {
                            m_stmH.WriteLine(sym.module.EmitStrings["h"]);
                        }
                        if (sym.module.EmitStrings.ContainsKey("c")) {
                            m_stmC.WriteLine(sym.module.EmitStrings["c"]);
                        }
                    }
                    break;

                case SymbolType.Imported:
                case SymbolType.Imported | SymbolType.NotYetDefined:
                    //  Imported symbol - we don't emit for anything here
                    break;

                case SymbolType.Type:
                    //
                    //  We are emitting a Type definition.
                    //
                    //  Type defintions should always start with an Assignment Type.  This will
                    //  point back to this symbol.
                    //

                    Debug.Assert(sym.type.baseType == BaseType.Assignment);

                    //
                    //  We build a list of names so that we can get naming information correct for
                    //  the different fields in the type during emission.
                    //
                    //  Create the list object, then push onto the list the name space for the module
                    //  and the name of the symbol itself.
                    //

                    lst = new List<string>();
                    lst.Add(sym.module.nameSpace);
                    lst.Add(CleanupName(sym.name));

                    //
                    //  We now know the name of the symbol.  Set it into the emission data structure
                    //

                    sym.codenames.SetStructureName(UseUScore(lst));

                    //
                    //  And put it onto the type?    Not sure why this is being done - look later at it again - M00QUEST?
                    //

                    sym.type.emitName = UseUScore(lst);

                    //
                    //  If there are any parts that need to be emitted - do so now.
                    //

                    if (sym.symlstParts.Count > 0) {
                        foreach (Symbol symT in sym.symlstParts) {
                            EmitPartSymbol(symT, lst);
                        }
                    }

                    //
                    //  Now build the emission stream for the type structure.  This comes back in multiple 
                    //  pieces
                    //  -- for the .h file
                    //      1.  Any pointers that are emitted due to circular references of data
                    //      2.  Any defines that are emitted for optional or choice data structures
                    //      3.  The actual data structures that are emitted & PDU descrptor table externs.
                    //  -- for the .c file
                    //      4.  Any forward reference data strcuctures needed in the .c file
                    //      5.  Any override functions that are emitted
                    //      6.  The PDU descriptor tables that are emitted
                    //

                    if (!sym.codenames.HasUserType) {
                        str = Structure_Type(sym, sym.type, "", true, false, lst, ref fPtr);
                        Debug.Assert(fPtr == false);

                        m_stm3.WriteLine();
                        if (sym.codenames.IsPtrEmitted) {
                            if (str.Substring(0, 6) == "struct") {
                                m_stm3.WriteLine("typedef struct _{2} {0} {1};", str.Substring(7), sym.codenames.StructName, sym.codenames.StructName);
                            }
                            else {
                                m_stm3.WriteLine("typedef {0} {1};", str, sym.codenames.StructName);
                            }
                        }
                        else {
                            m_stm3.WriteLine("typedef {0} {1};", str, sym.codenames.StructName);
                        }
                        m_stm3.WriteLine();
                        m_stm3.WriteLine("extern " + SzConst + "A2C_DESCRIPTOR {0};", sym.codenames.PDU_Name);
                        m_stm3.WriteLine("extern " + SzConst + "A2C_ITEM_HEADER {0}_description;", sym.codenames.DescriptorBase);
                        EmitFunctions(sym, lst, false);
                    }
                    else {
                        m_stm3.WriteLine("typedef {0} {1};", sym.codenames.UserType, sym.codenames.StructName);
                        m_stm3.WriteLine();

                        sym.codenames.SetDefaultFunction(sym.codenames.UserType);
                    }


                    break;

                case SymbolType.Value:
                    EmitValue(sym);
                    break;

                case SymbolType.Object:
                    EmitObject(sym);
                    break;

                case SymbolType.ObjectClass:
                    //
                    //  We are emitting an Object Class definition
                    //
                    //  Class definitions should always start with an Assignment Type.  This will
                    //  point back to the symbol.
                    //

                    Debug.Assert(sym.type.baseType == BaseType.Assignment);

                    //
                    //  We build a list of names so that we can get naming informaiton correct for
                    //  the different fields in the type during emission.
                    //
                    //  Create the list object, then push onto the list the name space for the module
                    //  and the name of the symbol itself.
                    //

                    lst = new List<string>();
                    lst.Add(sym.module.nameSpace);
                    lst.Add(CleanupName(sym.name));

                    //
                    //  We now know the name of the symbol.  Set it into the emission data structure
                    //

                    sym.codenames.SetStructureName(UseUScore(lst));

                    //
                    //  And put it into the type?  Need to check for reality check
                    //

                    sym.type.emitName = UseUScore(lst);

                    //
                    //  If we are not emitting this module, then we can say we are done at this point
                    //

                    if (sym.module.NoEmission) break;

                    //
                    //  Now build the emission stream for the type structure.  This comes back in multiple 
                    //  pieces
                    //  -- for the .h file
                    //      1.  Any pointers that are emitted due to circular references of data
                    //      2.  Any defines that are emitted for optional or choice data structures
                    //      3.  The actual data structures that are emitted & PDU descrptor table externs.
                    //  -- for the .c file
                    //      4.  Any forward reference data strcuctures needed in the .c file
                    //      5.  Any override functions that are emitted
                    //      6.  The PDU descriptor tables that are emitted
                    //

                    str = Structure_Type(sym, sym.type, "", true, true, lst, ref fPtr);
                    Debug.Assert(fPtr == false);

                    m_stm3.WriteLine();
                    if (sym.codenames.IsPtrEmitted) {
                        Debug.Assert(str.Substring(0, 6) == "struct");
                        m_stm3.WriteLine("typedef struct _{2} {0} {1};", str.Substring(7), sym.codenames.StructName, sym.codenames.StructName);
                    }
                    else {
                        m_stm3.WriteLine("typedef {0} {1};", str, sym.codenames.StructName);
                    }

                    m_stm3.WriteLine("extern " + SzConst + "A2C_DESCRIPTOR {0};", sym.codenames.PDU_Name);
                    m_stm3.WriteLine("extern " + SzConst + "A2C_ITEM_HEADER {0}_description;", sym.codenames.DescriptorBase);                EmitFunctions(sym, lst, false);
                    break;

                case SymbolType.ObjectSet:
                    int cObjects = 0;
                    String strObjects = "";
                    //
                    //  Make sure the type has been emitted
                    //

                    EmitSymbol(sym.type.owner);

                    //
                    //  Make sure that all of the objects have been emitted.
                    //

                    lst = new List<String>();
                    lst.Add(sym.module.nameSpace);
                    lst.Add(CleanupName(sym.name));

                    foreach (Value val in sym.value.valueList) {
                        switch (val.valueType) {
                        case ValueType.Extension:
                            break;

                        case ValueType.Symbol:
                            EmitSymbol(val.sym);
                            cObjects += 1;
                            strObjects += "&" + val.sym.module.nameSpace + CleanupName(val.sym.name) + ", ";
                            break;

                        default:
                            Debug.Assert(false);
                            break;
                        }
                    }

                    m_stm3.WriteLine();
                    m_stm3.WriteLine("extern " + SzConst + "A2C_OBJECT_SET {0};", UseUScore(lst));

                    m_stm6.WriteLine();
                    if (cObjects > 0) {
                        m_stm6.WriteLine(SzConst +"PCVOID {0}_table[] = {{", UseUScore(lst));
                        m_stm6.WriteLine("    {0}", strObjects);
                        m_stm6.WriteLine("};");
                    }
                    m_stm6.WriteLine(SzConst +"A2C_OBJECT_SET {0} = {{", UseUScore(lst));
                    m_stm6.WriteLine("    {0}, &{1}_description,", cObjects, sym.type.emitName);
                    if (cObjects > 0) m_stm6.WriteLine("    {0}_table", UseUScore(lst));
                    m_stm6.WriteLine("};");

                    break;

                case SymbolType.ParameterizedType:
                    //
                    //  We don't emit anything for a Parameterized type - this
                    //  is strictly an internal item
                    //
                    break;

                case SymbolType.Unknown:
                    //
                    //  We don't do anything - but don't really expect to get here.
                    //

                    Debug.Assert(false);
                    break;

                default:
                    //
                    //  Ignore anything that is either imported or parameterized.
                    //

                    if ((sym.symType & (SymbolType.Imported | SymbolType.Parameterized)) == 0) {
                        //
                        //  Unhandled case
                        //

                        Debug.Assert(false);
                    }
                    break;
                }

                //
                //  Mark symbol as emitted.
                //

                sym.emitted = true;
                sym.codenames.Done();
                //
                //  Restore emission state
                //
                PopEmit();
            }

            void EmitPartSymbol(Symbol sym, List<String> lst)
            {
                String str;
                bool fPtr = false;

                if (Program.DebugLevel > 0) Console.Error.WriteLine("Emit Part Symbol: '{0}'", sym.name);

                Debug.Assert(!sym.codenames.IsEmitted);
                Debug.Assert(!sym.codenames.IsEmitting);

                if (sym.codenames.NotStarted) {
                    sym.codenames.Started();
                }

                switch (sym.GetSymbolType()) {
                case SymbolType.Type:
                    Debug.Assert(sym.type.baseType == BaseType.Assignment);

                    //
                    //  Add the name of the symbol to the stack.
                    //

                    lst.Add(CleanupName(sym.name));

                    //
                    //  We now know the name of the symbol.  Set it into the emission data structure
                    //

                    sym.codenames.SetStructureName(UseUScore(lst));

                    //
                    //  And put it onto the type.
                    //

                    sym.type.emitName = UseUScore(lst);

                    //
                    //
                    //  Now build the emission stream for the type structure.  This comes back in multiple 
                    //  pieces
                    //  -- for the .h file
                    //      1.  Any pointers that are emitted due to circular references of data
                    //      2.  Any defines that are emitted for optional or choice data structures
                    //      3.  The actual data structures that are emitted & PDU descrptor table externs.
                    //  -- for the .c file
                    //      4.  Any forward reference data strcuctures needed in the .c file
                    //      5.  Any override functions that are emitted
                    //      6.  The PDU descriptor tables that are emitted
                    //

                    str = Structure_Type(sym, sym.type, "", true, false, lst, ref fPtr);
                    Debug.Assert(fPtr == false);

                    m_stm3.WriteLine();
                    if (sym.codenames.IsPtrEmitted) {
                        Debug.Assert(str.Substring(0, 6) == "struct");
                        m_stm3.WriteLine("typedef struct _{2} {0} {1};", str.Substring(7), sym.codenames.StructName, sym.codenames.StructName);
                    }
                    else {
                        m_stm3.WriteLine("typedef {0} {1};", str, sym.codenames.StructName);
                    }

                    m_stm3.WriteLine();
                    m_stm3.WriteLine("extern " + SzConst + "A2C_DESCRIPTOR {0};", sym.codenames.PDU_Name);
                    m_stm3.WriteLine("extern " + SzConst + "A2C_ITEM_HEADER {0}_description;", sym.codenames.DescriptorBase);
                    EmitFunctions(sym, lst, false);

                    lst.RemoveAt(lst.Count - 1);

                    break;

                default:
                    Debug.Assert(false);
                    break;
                }

                //
                //  Mark symbol as emitted.
                //

                sym.emitted = true;
                sym.codenames.Done();

            }

            string Structure_Field(Symbol symRoot, Symbol sym, string tab, Boolean fIndirect, Boolean fClass, ref int iOption, List<String> lst)
            {
                String sz;
                Boolean fPtr = false ;
                
                //
                //  Start by dealing with extensionand extension group information.
                //
                //  Nothing really needs to be done for an extension marker,
                //

                if (sym.GetSymbolType() == SymbolType.Extension) return tab + "/* Start Extension */";
                if (sym.GetSymbolType() == SymbolType.ExtensionEnd) return tab + "A2C_EXTENSION                        _extension;";

                //
                //  Nothing to do for an exception
                //

                if (sym.GetSymbolType() == SymbolType.Exception) return tab + "/* Exception */";

                //
                //  For an extension group, we need to emit all of the items in the extension group
                //
                
                if (sym.GetSymbolType() == SymbolType.ExtensionGroup) {
                    sz = tab + "/* Extension Group";
                    if (sym.groupVersion != -1) sz += " version: " + sym.groupVersion.ToString();
                    sz += " */\n";
                    foreach (Symbol sym1 in sym.symList) {
                        sz += Structure_Field(symRoot, sym1, tab, false, fClass, ref iOption, lst) + "\n";
                    }
                    return sz;
                }

                //
                //  Now handle class definitions -
                //
                //  Types are emitted as a constant known type
                //

                if (sym.GetSymbolType() == SymbolType.TypeField) {
                    sz = tab+"A2C_DESCRIPTOR *";

                    lst.Add(CleanupName(sym.name));
                    sym.fieldName = UseDots(lst);
                    sym.structureName = lst[0] + lst[1];
                    lst.RemoveAt(lst.Count-1);
                }
                else if (sym.GetSymbolType() == SymbolType.ObjectSetField) {
                    //
                    //  We have to force the type of this object set to be emitted before
                    //  we can go any farther
                    //

                    if (sym.type.baseType == BaseType.Assignment) {
                        EmitSymbol(sym.type.owner);
                    }
                    else if (sym.type.baseType == BaseType.Symbol) {
                        EmitSymbol(sym.type.sym);
                    }
                    else {
                        Debug.Assert(false, "NYI code - object set field for which the type is not a symbol");
                    }
                    sz = tab + "A2C_OBJECT_SET *";

                    lst.Add(CleanupName(sym.name));
                    sym.fieldName = UseDots(lst);
                    sym.structureName = lst[0] + lst[1];
                    lst.RemoveAt(lst.Count - 1);
                }

                //  
                //  Values are emitted as
                //

                else {
                    Debug.Assert((sym.GetSymbolType() == SymbolType.Component) ||
                        (sym.GetSymbolType() == SymbolType.ValueField) ||
                        (sym.GetSymbolType() == SymbolType.ValueSetField) ||
                        (sym.GetSymbolType() == SymbolType.ObjectField) ||
                        (sym.GetSymbolType() == SymbolType.ObjectSetField));

                    //  Put field name on the list
                    lst.Add(CleanupName(sym.name));

                    fPtr = sym.flags.Pointer;

                    sz = Structure_Type(symRoot, sym.type, tab, false, fClass, lst, ref fPtr);

                    sym.flags.Pointer = fPtr;

                    if ((sym.GetSymbolType() == SymbolType.ValueField) ||
                        (sym.GetSymbolType() == SymbolType.ValueSetField) ||
                        (sym.GetSymbolType() == SymbolType.ObjectField) ||
                        (sym.GetSymbolType() == SymbolType.ObjectSetField)) {
                        if (!fPtr && sym.type.emitName == null) {
                            sym.type.emitName = sz;
                        }

                        //  If this is a native time, then don't emit the pointer character.
                        //

                        if ((sym.type.SkipFluff().emitName == "A2C_ENUMERATED_NATIVE") ||
                            (sym.type.SkipFluff().emitName == "A2C_INTEGER_NATIVE")) {
                            fPtr = true;
                        }
                        //  M00BUG - If we are looking at a NATIVE type, then don't do the pointer?
                        if (!fPtr) sz += " *";
                    }
                    else if (fIndirect && !fPtr) sz += " *";
                    else if (sym.flags.Pointer) sz += " *";

                    sym.fieldName = UseDots(lst);
                    sym.structureName = lst[0] + lst[1];

                    lst.RemoveAt(lst.Count - 1);
                }

                if (fClass) sz = tab + SzConst + sz.TrimStart(' ');

                sz = String.Format("{0,-40} {1};", sz, CleanupName(sym.name), tab);

                if (iOption != -1) { 
                    String sz1 = String.Format("#define {1}_{0}_index {2}\n", CleanupName(sym.name), UseUScore(lst), iOption);
                    iOption += 1;
                    sz = sz1 + sz;
                }

                return sz;
            }

            string Structure_Type(Symbol symRoot, Type typ, string tab, Boolean fRoot, Boolean fClass, List<String> lst, ref Boolean fPointer)
            {
                int iOption;
                String str;
                String tab1;
                String tab2;

                //
                //  If this is NOT the root type, and we know the structure name for the type,
                //  then return that structure name
                //

                if (!fRoot) {
                    if (typ.owner == null) {
                        // Debug.Assert(false, "typ.owner == null");
                        if (typ.emitName != null) {
                            //
                            //  Special case to handle atomic types
                            //

                            if (typ.IsAtomic) str = tab + "A2C_BINARY";
                            else str = tab + typ.emitName;

                            if (fPointer) str = str + " *";
                            return str;
                        }
                    }
                    else {
                        if (!typ.owner.codenames.NotStarted) {
                            if (typ.IsAtomic) {
                                return tab + "A2C_BINARY";
                            }
                            if (typ.owner.codenames.IsEmitting) {
                                typ.owner.codenames.EmitPointer(m_stm1);
                                return tab + "P" + typ.owner.codenames.StructName;
                            }
                            if (fPointer) {
                                return tab + typ.owner.codenames.StructName + " *";
                            }
                            return tab + typ.owner.codenames.StructName;
                        }
                    }
                }

                //
                //  Process depending on the type node we are looking at
                //

                switch (typ.baseType) {
                    //
                    //  We just hit an assignment node - 
                    //  Two cases to deal with:
                    //      1.  We are stil at the root, then we need to use that type
                    //      2.  Emit the nested symbol and return it's answer.
                    //

                case BaseType.Assignment:
                    if (fRoot || (typ.owner == null)) {
                        // Debug.Assert(typ.owner != null);
                        if (!fRoot) {
                            if (typ.owner != null && typ.owner.codenames.IsEmitting) {
                                typ.owner.codenames.EmitPointer(m_stm1);
                                return tab + "P" + typ.owner.codenames.StructName;
                            }
                            else if (typ.owner != null && typ.owner.codenames.IsEmitted) {
                                return tab + typ.owner.codenames.StructName;
                            }
                        }
                        str = Structure_Type(symRoot, typ.subType, tab, false, fClass, lst, ref fPointer);
                        return str;
                    }

                    //
                    //  Is this type marked atomic?
                    //

                    if (typ.IsAtomic) {
                        return tab + "A2C_BINARY";
                    }

    #if UNUSED
                    //
                    // Check for pointer
                    //

                    if (fPointer) {
                        if (typ.owner.codenames.StructName == null) {
                            List<String> lst2;

                            lst2 = new List<string>();
                            lst2.Add(typ.owner.module.nameSpace);
                            lst2.Add(CleanupName(typ.owner.name));

                            //
                            //  We now know the name of the symbol.  Set it into the emission data structure
                            //

                            typ.owner.codenames.SetStructureName(UseUScore(lst2));
                        }
                        typ.owner.codenames.EmitPointer(m_stm1);
                        return tab + "P" + typ.owner.codenames.StructName;
                    }

    #endif
                    //
                    //  Check for nested emission
                    //

                    if (!typ.owner.codenames.NotStarted) {
                        if (typ.owner.codenames.IsEmitted) {
                            if (fPointer) return tab + typ.owner.codenames.StructName + " *";
                            return tab + typ.owner.codenames.StructName;
                        }
                        if (typ.owner.codenames.IsEmitting) {
                            typ.owner.codenames.EmitPointer(m_stm1);
                            return tab + "P" + typ.owner.codenames.StructName;
                        }
                    }

                    //
                    //  OK - emit the symbol
                    //

                    EmitSymbol(typ.owner);

                    //
                    //  Return the emitted structure name
                    //

                    if (fPointer) return tab + typ.owner.codenames.StructName + " *";
                    return tab + typ.owner.codenames.StructName;

                case BaseType.Param_Assignment:
                case BaseType.Constraint:
                    return Structure_Type(symRoot, typ.subType, tab, false, fClass, lst, ref fPointer);

                case BaseType.NamedList:
                    symRoot.codenames.named = true;
                    str = Structure_Type(symRoot, typ.subType, tab, false, fClass, lst, ref fPointer);

                    switch (typ.SkipFluff().emitName) {
                    case "A2C_ENUMERATED_NATIVE":
                    case "A2C_INTEGER_NATIVE":
                    case "A2C_BIT_STRING_NAMED":
                        foreach (Symbol sym1 in typ.fieldList) {
                            if ((sym1.GetSymbolType() != SymbolType.Extension) &&
                                (sym1.GetSymbolType() != SymbolType.Exception)) {
                                m_stm2.WriteLine("#define {2}__{0} {1}", CleanupName(sym1.name), ValueBuilder(sym1.value, sym1.value.type, sym1.type, true, lst), UseUScore(lst));
                            }
                        }
                        break;
                    }

                    return str;

                case BaseType.Symbol:
                    return Structure_Type(symRoot, typ.sym.type, tab, fRoot, fClass, lst, ref fPointer);

                case BaseType.Leaf:
                    return tab + typ.emitName;

                case BaseType.Choice:
                    typ.emitting = true;
                    str = tab + "struct {\n";
                    tab1 = tab + "    ";
                    str += tab1 + "int index;\n";
                    str += tab1 + "union {\n";
                    tab2 = tab1 + "    ";
                    iOption = 0;
                    if (!m_args.fAnonUnion) {
                        lst.Add("*u");
                    }
                    foreach (Symbol sym1 in typ.fieldList) {
                        str += Structure_Field(symRoot, sym1, tab2, false, fClass, ref iOption, lst) + "\n";
                    }
                    if (!m_args.fAnonUnion) {
                        lst.RemoveAt(lst.Count - 1);
                    }

                    if (m_args.fAnonUnion) {
                        str += tab1 + "};\n";
                    }
                    else {
                        str += tab1 + "} u;\n";
                    }
                    str += tab + "}";
                    typ.emitting = false;
                    return str;

                case BaseType.ObjectClassField:
                    Symbol symT = typ.GetObjectClassField();
                    switch (symT.GetSymbolType()) {
                    case SymbolType.ValueField:
                        return Structure_Type(symRoot, symT.type, tab, false, fClass, lst, ref fPointer);

                    case SymbolType.TypeField:
                        return tab + "A2C_OPEN_TYPE";
                    }
                    return tab + "<MISSING ObjectClassField:" + typ.sym.GetSymbolType().ToString() + ">";

                case BaseType.Sequence:
                case BaseType.Set:
                    typ.emitting = true;
                    str = tab + "struct {\n";
                    tab1 = tab + "    ";
                    Boolean f = false;
                    iOption = 0;

                    foreach (Symbol sym1 in typ.fieldList) {
                        if ((sym1.flags != null) && sym1.flags.Optional && !sym1.flags.Pointer) {
                            if (!f) {
                                f = true;
                                str += tab1 + "unsigned int optionalFields;\n";
                            }
                            str += String.Format("#define {2}_{0}_present 0x{1:x}\n", CleanupName(sym1.name), 1<<iOption, UseUScore(lst));
                            iOption += 1;
                            if (iOption == Program.IntSize) {
                                Error err = new Error(ErrorNumber.OverflowNativeInt);
                                err.AddObject(UseUScore(lst) + " " + CleanupName(sym1.name));
                                err.Print();
                            }
                        }
                        else if (sym1.GetSymbolType() == SymbolType.ExtensionGroup) {
                            foreach (Symbol sym2 in sym1.symList) {
                                if ((sym1.flags != null) && sym2.flags.Optional && !sym2.flags.Pointer) {
                                    if (!f) {
                                        f = true;
                                        str += tab1 + "unsigned int optionalFields;\n";
                                    }
                                    str += String.Format("#define {2}_{0}_present {1}\n", CleanupName(sym2.name), 1 << iOption, UseUScore(lst));
                                    iOption += 1;
                                    if (iOption == Program.IntSize) {
                                        Error err = new Error(ErrorNumber.OverflowNativeInt);
                                        err.AddObject(UseUScore(lst) + " " + CleanupName(sym1.name));
                                        err.Print();
                                    }
                                }
                            }
                        }
                    }

                    iOption = -1;
                    foreach (Symbol sym1 in typ.fieldList) {
                        str += Structure_Field(symRoot, sym1, tab1, false, fClass, ref iOption, lst) + "\n";
                    }

                    //
                    //  Need to deal with the case that no fields are defined
                    //

                    if (typ.fieldList.Count == 0) {
                        str += tab1 + "int unused;\n";
                    }

                    str += tab + "}";
                    typ.emitting = false;
                    return str;

                case BaseType.SequenceOf:
                case BaseType.SetOf:
                    //
                    //  We may need to forceout what is below this unless it is either
                    //  an assignment, a leaf.  Skip pass tagged as we don't emit them.
                    //

                    typ.emitting = true;
                    str = tab + "struct {\n";
                    tab1 = tab + "    ";
                    str += tab1 + "int count;\n";
                    str += tab1 + "int countAllocated;\n";
                    iOption = -1;
                    foreach (Symbol sym1 in typ.fieldList) {
                        str += Structure_Field(symRoot, sym1, tab1, true, fClass, ref iOption, lst) + "\n";
                    }
                    str += tab + "}";
                    typ.emitting = false;
                    return str;

                case BaseType.Tagged:
                    return Structure_Type(symRoot, typ.subType, tab, fRoot, fClass, lst, ref fPointer);

                case BaseType.Class:
                    typ.emitting = true;
                    str = tab + "struct {\n";
                    tab1 = tab + "    ";
                    iOption = -1;

                    foreach (Symbol sym1 in typ.fieldList) {
                        sym1.flags.Pointer = true;
                        str += Structure_Field(symRoot, sym1, tab1, false, true, ref iOption, lst) + "\n";
                    }

                    str += tab + "}";

                    typ.emitting = false;
                    return str;
                }
                return tab + "Missing " + typ.baseType.ToString();
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="sym"></param>
            public void EmitValue(Symbol sym)
            {
                List<String> lst = new List<String>();
                lst.Add(sym.module.nameSpace);

                //
                //  Emit internal pieces
                //

                foreach (Symbol symT in sym.symlstParts) {
                    EmitSymbol(symT);
                }

                //
                //  Two steps 
                //
                //  1.  Emit the extern into the .h file
                //

                Type type = sym.type;

                while ((type.baseType == BaseType.Symbol) || (type.baseType == BaseType.Tagged) ||
                        (type.baseType == BaseType.Constraint)) {
                    if (type.baseType == BaseType.Symbol) type = type.sym.type;
                    else if (type.baseType == BaseType.Tagged) type = type.subType;
                    else if (type.baseType == BaseType.Constraint) type = type.subType;
                }

                if (type.emitName == null) {
                    //
                    //  Type not yet emitted? -- then force it out
                    //

                    Debug.Assert(type.owner != null, "EmitValue " + sym.name);
                    EmitSymbol(type.owner);
                    sym.type.emitName = type.emitName;
                }

                lst.Add(CleanupName(sym.name));
                m_stm3.WriteLine("extern " + SzConst + "{0} {1};", type.emitName, UseUScore(lst));

                //
                //  2.  Emit the value into the .c file
                //

                m_stm6.WriteLine(SzConst +"{0} {1}={2};", type.emitName, UseUScore(lst), ValueBuilder(sym.value, sym.value.type, sym.type, false, lst));
                m_stm6.WriteLine();
            }

     
            String ValueBuilder(Value val, Type typeVal, Type typeField, Boolean fDefine, List<String> lst)
            {
                int cb;
                int cbits;
                int i;
                int i2;
                String sz = "Missing";
                String sz1;
                String str1;

                //
                //  There are some nodes that cannot be directly emitted as values since they
                //  don't have a value associated with them.  Walk down through these nodes until we
                //  can find a node that has a value.
                //
                //  Do the walk for the field type
                //

                switch (typeField.baseType) {
                case BaseType.Leaf:
                case BaseType.Sequence:
                case BaseType.Set:
                case BaseType.SequenceOf:
                case BaseType.SetOf:
                    break;

                case BaseType.Assignment:
                case BaseType.Constraint:
                case BaseType.NamedList:
                case BaseType.Param_Assignment:
                case BaseType.Tagged:
                    return ValueBuilder(val, typeVal, typeField.subType, fDefine, lst);


                case BaseType.ObjectClassField:
                    Symbol symT = typeField.GetObjectClassField();

                    switch (symT.symType) {
                    case SymbolType.ValueField:
                        return ValueBuilder(val, typeVal, typeField.GetObjectClassField().type, fDefine, lst);

                    case SymbolType.TypeField:
                        Debug.Assert(val.valueType == ValueType.Symbol);
                        if (val.valueType == ValueType.Symbol) {

                            return String.Format("{{0, 0, &{0}_descriptor, &{1}}}",
                                val.sym.type.emitName, val.sym.fullName);
                        }
                        return "ERROR - BuildValue #22";

                    default:
                        Debug.Assert(false);
                        return "ERROR - BuildValue #21";
                    }
                    break;

                default:
                    Debug.Assert(false);
                    return "ERROR - BuildValue #2";
                }

                //
                //  If this points to another symbol, then recurse down using that symbol's value.
                //

                if (val.valueType == ValueType.Symbol) {
                    return ValueBuilder(val.sym.value, val.sym.value.type, typeField, fDefine, lst);
                }

                //
                //  Do the walk for the value type
                //

                switch (typeVal.baseType) {
                case BaseType.Leaf:
                case BaseType.Sequence:
                case BaseType.Set:
                case BaseType.SequenceOf:
                case BaseType.SetOf:
                    break;

                case BaseType.Assignment:
                case BaseType.Constraint:
                case BaseType.NamedList:
                case BaseType.Param_Assignment:
                case BaseType.Tagged:
                    return ValueBuilder(val, typeVal.subType, typeField, fDefine, lst);

                case BaseType.Symbol:
                    return ValueBuilder(val, typeVal.sym.type, typeField, fDefine, lst);

                default:
                    Debug.Assert(false);
                    return "ERROR - BuildValue #1";
                }

                //
                //  Now lets build the string corresponding to the value we are going emitting
                //

                switch (typeField.baseType) {
                default:
                    Debug.Assert(false);
                    break;

                case BaseType.Leaf:
                    /* M00BUG - real assert - Debug.Assert(typeField.baseType == typeVal.baseType); */

                    switch (typeVal.dataType) {
                    case DataType.Boolean:
                        /* M00BUG - real assert - Debug.Assert(val.valueType == ValueType.Boolean); */

                        if (val.bVal) return "{1}";
                        return "{0}";

                    case DataType.Integer:
                    case DataType.Enumerated:
                        Debug.Assert(val.valueType == ValueType.Integer);
                        if ((typeField.emitName == "A2C_INTEGER_HUGE") || (typeField.emitName == "A2C_ENUMERATED_HUGE")) {
                            Byte[] rgb = BitConverter.GetBytes(val.iVal);
                            if (BitConverter.IsLittleEndian) {
                                rgb = SwapOrder(rgb);
                            }
                            for (i = 0; i < rgb.Length - 1; i++) {
                                if ((rgb[i] == 0) && (rgb[i + 1] < 0x80)) { ;}
                                else if ((rgb[i] == 0xff) && (rgb[i + 1] > 0x7f)) { ;}
                                else break;
                            }
                            lst.Add("_array");
                            sz1 = String.Format("{{ {0}, (BYTE *) {1} }}", (rgb.Length - i).ToString(), UseUScore(lst));

                            i2 = i;
                            sz = "{";
                            for (; i < rgb.Length; i++) sz += rgb[i].ToString() + ",";
                            sz += "}";

                            m_stm6.WriteLine("static " + SzConst + "BYTE {1}[{0}] = {2};", rgb.Length - i2, UseUScore(lst), sz);

                            lst.RemoveAt(lst.Count - 1);
                            return sz1;
                        }
                        if (fDefine) return val.iVal.ToString();
                        return "{" + val.iVal.ToString() + "}";

                    case DataType.BitString:
                        Debug.Assert(val.valueType == ValueType.Binary);

                        if (typeField.emitName == "A2C_BIT_STRING_NAMED") {
                            cbits = val.bin.cbits;
                            if (val.bin.rgb == null) {
                                cb = 0;
                            }
                            else {
                                //  Remove all trailing zeros from the string
                                cb = val.bin.rgb.Length;
                                cbits = cb * 8;
                                for (cbits = cb * 8 - 1; cbits > 0; cbits--) {
                                    if ((val.bin.rgb[cbits / 8] & (0x80 >> (cbits % 8))) != 0) {
                                        cbits += 1;
                                        cb = (cbits + 7) / 8;
                                        break;
                                    }
                                }
                            }
                        }
                        else {
                            cbits = val.bin.cbits;
                            if (val.bin.rgb == null) {
                                cb = 0;
                            }
                            else {
                                cb = val.bin.rgb.Length;
                            }
                        }

                        str1 = "";
                        if (cb > 0) {
                            for (i = 0; i < cb; i++) {
                                str1 += String.Format("0x{0:x}, ", val.bin.rgb[i]);
                            }
                            if (str1.Length == 0) {
                                str1 = "0";
                            }
                            else {
                                str1 = str1.Substring(0, str1.Length - 2);
                            }
                        }
                        else {
                            str1 = "0";
                        }


                        lst.Add("_array");
                        m_stm6.WriteLine("static " + SzConst +  "BYTE {0}[] = {{{1}}};", UseUScore(lst), str1);

                        sz = String.Format("{{ {0}, {1}, (PBYTE) {2}}}", cbits, cb, UseUScore(lst));
                        lst.RemoveAt(lst.Count - 1);

                        return sz;

                    case DataType.Null:
                        return "{0}";

                    case DataType.OctetString:
                        Debug.Assert(val.valueType == ValueType.Binary);

                        if ((val.bin.rgb == null) || (val.bin.rgb.Length == 0)) {
                            cb = 0;
                            str1 = "0";
                        }
                        else {
                            str1 = "";
                            cb = val.bin.rgb.Length;
                            for (i = 0; i < cb; i++) {
                                str1 += String.Format("0x{0:x}, ", val.bin.rgb[i]);
                            }
                            str1 = str1.Substring(0, str1.Length - 2);
                        }


                        lst.Add("_array");

                        m_stm6.WriteLine("static " + SzConst + "BYTE {0}[] = {{{1}}};", UseUScore(lst), str1);

                        sz = String.Format("{{ {0}, (PBYTE) {1}}}", cb, UseUScore(lst));
                        lst.RemoveAt(lst.Count - 1);

                        return sz;

                    case DataType.ObjectIdentifier:
                        Debug.Assert(val.valueType == ValueType.List);
                        sz = "{\"";
                        foreach (Value val1 in val.valueList) {
                            Debug.Assert(val1.valueType == ValueType.OIDNode);
                            sz += val1.oidNode.iValue.ToString() + ".";
                        }
                        sz = sz.Remove(sz.Length - 1);
                        sz += "\"}";
                        return sz;
                    }


                    return "DT: " + typeVal.dataType.ToString();

                case BaseType.Sequence:
                case BaseType.Set:
                    Debug.Assert(typeField.baseType == typeVal.baseType);
                    //  Compute the value of optional

                    int iOptionalVal = 0;
                    int iOptional = 1;
                    int iValue = 0;

                    sz = "";

                    foreach (Symbol sym in typeField.fieldList) {
                        if ((iValue < val.valueList.Count) && (val.valueList[iValue].name == sym.name)) {
                            lst.Add(val.valueList[iValue].name);   // M00QUEST - should this by sym.name?
                            sz = sz + ValueBuilder(val.valueList[iValue], val.valueList[iValue].type, sym.type, false, lst) + ", ";
                            lst.RemoveAt(lst.Count - 1);

                            if (sym.flags.Optional) {
                                iOptionalVal |= iOptional;
                                iOptional <<= 1;
                            }

                            iValue += 1;
                        }
                        else {
                            sz = sz + "{0}, ";  // M00BUG - may not work for all compilers and all items.

                            if (sym.flags.Optional) {
                                iOptional <<= 1;
                            }
                        }
                    }


                    //  Walk the list of all children and get their values

                    if (iOptional != 1) {
                        sz = "{" + iOptionalVal.ToString() + ", " + sz.Substring(0, sz.Length - 2); ;
                    }
                    else {
                        if (typeField.fieldList.Count > 0) {
                            sz = "{" + sz.Substring(0, sz.Length - 2);
                        }
                        else {
                            sz = "{ 0";
                        }
                    }

                    sz += "}";
                    return sz;

                case BaseType.SequenceOf:
                case BaseType.SetOf:
                    int c = 0;

                    str1 = "";
                    Debug.Assert(typeField.fieldList.Count == 1);

                    foreach (Value val2 in val.valueList) {
                        c += 1;
                        str1 += ValueBuilder(val2, val2.type, typeField.fieldList[0].type, fDefine, lst) + ", ";
                    }

                    if (c > 0) {
                        lst.Add("_array");

                        m_stm6.WriteLine("static " + SzConst +"{2} {0}[] = {{{1}}};", UseUScore(lst), str1.Substring(0, str1.Length-2), val.valueList[0].type.emitName);

                        sz = String.Format("{{ {0}, 0, ({2} *) {1}}}", c, UseUScore(lst), val.valueList[0].type.emitName);

                        lst.RemoveAt(lst.Count - 1);
                    }
                    else {
                        sz = "{ 0, 0, 0 }";
                    }

                    return sz;
                }   

                return "BT: " + typeVal.baseType.ToString();

            }

            void EmitFunctions(Symbol sym, List<String> lst, Boolean fSupressTag)
            {
                CodePointers codenames = sym.codenames;
                EmitFunctions(sym.type, lst, fSupressTag, ref codenames);
            }

            string EmitFunctions(Type type, List<String> lst, Boolean fSupressTag, ref CodePointers codenames)
            {
                EmitDescription(type, lst, fSupressTag, true, true, ref codenames);

                codenames.EmitPDU(m_stm6, true);
                codenames.EmitInlines(m_stm6, m_stm3, m_args);

                return codenames.DefaultName ;
            }

            void EmitDescription(Type typ, List<String> lst, Boolean fSupressTag, Boolean fSkipAssign, bool fTop, ref CodePointers codenames)
            {
                Boolean fForce = false;
                String szContentLines;
                string szReturn = "<UNKNOWN " + typ.baseType.ToString() + ">";
                String szType = "<XX>";
                int iExtensionNo;
                String str1;
                CodePointers codenames2 = null;

                /*
                 * If this is not the root element, 
                 *   - End if this is a defined symbol
                 *   -
                 */

                if (!fTop) {
                    if (typ.owner != null) {
                        if (typ.IsAtomic) {
                            if (codenames.StructName == null) {
                                codenames.SetStructureName("A2C_BINARY");
                            }
                            codenames.SetDefaultFunction("A2C_BINARY");
                        }
                        else {
                            if ((typ.owner.codenames.NotStarted) || (typ.owner.codenames.Description == null)) {
                                EmitSymbol(typ.owner);
                            }
                            codenames.CopyFrom(typ.owner.codenames);
                        }
                        return;
                    }
                }

                /*
                 * Processed based on the type of the object
                 */

                switch (typ.baseType) {
                case BaseType.Assignment:
                    if (!fSkipAssign && (typ.owner != null)) {
                        if (typ.IsAtomic) {
                            if (codenames.StructName == null) {
                                codenames.SetStructureName("A2C_BINARY");
                            }
                            codenames.SetDefaultFunction("A2C_BINARY");
                        }
                        else {
                            if ((typ.owner.codenames.NotStarted) || (typ.owner.codenames.Description == null)) {
                                EmitSymbol(typ.owner);
                            }
                            codenames.CopyFrom(typ.owner.codenames);
                        }
                        return;
                    }
                    EmitDescription(typ.subType, lst, fSupressTag, false, fTop, ref codenames);
                    return;

                case BaseType.Param_Assignment:
                    EmitDescription(typ.subType, lst, fSupressTag, false, fTop, ref codenames);
                    return;

                case BaseType.Constraint:
                    fForce = false;

                    EmitDescription(typ.subType, lst, fSupressTag, fSkipAssign, fTop, ref codenames);

                    foreach (Constraint cst in typ.constraintList) {
                        if ((cst.ct == ConstraintType.Relation) && (typ.constraintList.Count == 1)) {
                            fForce = Code_Relation(cst, lst, codenames, "");
                        }
                        else if (cst.ct == ConstraintType.Contents) {
                            fForce = Code_Contents(cst, lst, codenames, typ.subType == Type.OpenOctetString ? "_OPEN_OCTET_STRING" : "_OPEN_BIT_STRING");
                        }
                    }

                    if (fForce) {
                        codenames.SetDescriptorName(UseUScore(lst));
                        codenames.SetDescriptionName(codenames.StructName);
                        codenames.EmitPDU(m_stm6, false);
                    }
                    return;

                case BaseType.NamedList:
                    Type type = typ.SkipFluff();

                    //
                    //  We have emitted the type, so now emit all of the symbols using this type.
                    //  But only if it is a huge
                    //

                    switch (type.emitName) {
                    case "A2C_ENUMERTED_HUGE":
                    case "A2C_INTEGER_HUGE":
                        foreach (Symbol sym1 in typ.fieldList) {
                            EmitSymbol(sym1);
                        }
                        break;
                    }

                    //
                    //  Now emit the table for walking through all of the values.
                    //

                    m_stm6.WriteLine(SzConst +"A2C_NAME_ITEM {0}_name_table[] = {{", UseUScore(lst));
                    foreach (Symbol sym1 in typ.fieldList) {
                        if ((sym1.GetSymbolType() != SymbolType.Exception) &&
                            (sym1.GetSymbolType() != SymbolType.Extension)) {

                            switch (type.emitName) {
                            case "A2C_ENUMERATED_HUGE":
                            case "A2C_INTEGER_HUGE":
                                m_stm6.WriteLine("    {{ 0, (A2C_VALUE " + SzConst + "*) &{0}, \"{1}\"}},", CleanupName(UseUScore(lst) + "__" + sym1.name), sym1.name);
                                break;

                            default:
                                m_stm6.WriteLine("    {{ {0}, 0, \"{1}\"}},", CleanupName(UseUScore(lst) + "__" + sym1.name), sym1.name);
                                break;
                            }
                        }
                    }
                    m_stm6.WriteLine("};");
                    codenames.NamedItems = true;

                    EmitDescription(typ.subType, lst, fSupressTag, fSkipAssign, fTop, ref codenames);
                    return;

                case BaseType.Leaf:
                    if (codenames.StructName == null) {
                        codenames.SetStructureName(typ.emitName);
                    }
                    codenames.SetDefaultFunction(typ.emitName);
                    return;

                case BaseType.OpenType:
                    codenames.SetStructureName(typ.emitName);
                    codenames.SetDefaultFunction(typ.emitName);
                    return;

                case BaseType.ObjectClassField:
                    Symbol symT = typ.GetObjectClassField();
                    switch (symT.GetSymbolType()) {
                    case SymbolType.ValueField:
                        EmitDescription(symT.type, lst, fSupressTag, fSkipAssign, fTop, ref codenames);
                        return;

                    case SymbolType.TypeField:
                        codenames.SetStructureName("A2C_OPEN_TYPE");
                        codenames.SetDefaultFunction("A2C_OPEN_TYPE");
                        return;
                    }
                    codenames.SetStructureName("<MISSING ObjectClassField:" + typ.sym.GetSymbolType().ToString() + ">");
                    return;

                case BaseType.Choice:
                case BaseType.Sequence:
                case BaseType.Set:
                    bool fHasSubtable = false;
                    /*
                     * We know we are going to emit a description table at this point
                     */

                    szContentLines = "";
                    iExtensionNo = 0;
                    if (!m_args.fAnonUnion && (typ.baseType == BaseType.Choice)) {
                        lst.Add("*u");
                    }

                    foreach (Symbol sym in typ.fieldList) {
                        if (sym.GetSymbolType() == SymbolType.Exception) {
                            szContentLines += "    /* Exception */\n";
                            continue;
                        }
                        else if (sym.GetSymbolType() == SymbolType.Extension) {
                            /*
                             * Is this the start or end of extensions marker? know based on value of iExtensionNo
                             */
                            if (iExtensionNo == 0) iExtensionNo = 1;
                            else iExtensionNo = -1;
                            continue;
                        }
                        else if (sym.GetSymbolType() == SymbolType.ExtensionEnd) {
                            //  Put this back when the runtime support exists.
    #if false
                            fHasSubtable = true;
                            lst.Add("_extension");
                            if (UseDotsCount(lst) == 3) {
                                szContentLines += String.Format("    {{\"_extension\", OFFSET_OF({0}, {1}), {{0, 0}}, 0, 0, A2C_FLAGS_EXTENSION_END, 0}},\n", lst[0] + lst[1], UseDots(lst));
                            }
                            else {
                                szContentLines += String.Format("    {{\"_extension\", REL_OFFSET_OF({0}, {1}, {2}), {{0, 0}}, 0, 0, A2C_FLAGS_EXTENSION_END, 0}},\n", lst[0] + lst[1], UseDots(lst), UseDotsM1(lst));
                            }
                            lst.RemoveAt(lst.Count - 1);
    #endif
                            continue;
                        }
                        else if (sym.GetSymbolType() == SymbolType.ExtensionGroup) {
                            fHasSubtable = true;
                            /*
                             * We have an extension group that we need to emit - need to walk each
                             * Child and process then we can jump to the next symbol in the containing list
                             */

                            if (sym.groupVersion != -1) iExtensionNo = sym.groupVersion;
                            foreach (Symbol sym1 in sym.symList) {
                                lst.Add(CleanupName(sym1.name));
                                szContentLines += "    " + DescriptionTable_Field(lst, sym1, iExtensionNo) + ",\n";
                                lst.RemoveAt(lst.Count - 1);
                            }
                            continue;
                        }
                        else {
                            fHasSubtable = true;
                            if (iExtensionNo > 0) iExtensionNo += 1;
                        }

                        lst.Add(CleanupName(sym.name));
                        szContentLines += "    " + DescriptionTable_Field(lst, sym, iExtensionNo) + ",\n";
                        lst.RemoveAt(lst.Count - 1);
                    }

                    if (!m_args.fAnonUnion && (typ.baseType == BaseType.Choice)) {
                        lst.RemoveAt(lst.Count - 1);
                    }

                    szContentLines = szContentLines.TrimEnd(" \r\n".ToCharArray());

                    if (fHasSubtable) {
                        m_stm6.WriteLine();
                        m_stm6.WriteLine(SzConst +"A2C_ITEM {0}_description_table[] = {{", UseUScore(lst));
                        m_stm6.WriteLine(szContentLines);
                        m_stm6.WriteLine("};");
                    }

                    switch (typ.baseType) {
                    case BaseType.Choice:
                        szType = "A2C_CHOICE";
                        break;

                    case BaseType.Sequence:
                        szType = "A2C_SEQUENCE";
                        break;

                    case BaseType.Set:
                        szType = "A2C_SET";
                        break;
                    }

                    if (lst.Count > 2) {
                        codenames2 = new CodePointers(SzConst);
                        codenames2.SetStructureName(UseUScore(lst));
                        codenames2.SetDefaultFunction(szType);
                        codenames2.SubTable = fHasSubtable;
                    }
                    else {
                        codenames.SetDefaultFunction(szType);
                        codenames.SubTable = fHasSubtable;
                    }
                    szReturn = UseUScore(lst);
                    fForce = true;
                    break;

                case BaseType.SequenceOf:
                case BaseType.SetOf:
                    /*
                     * We know we need to emit a description table
                     */

                    szContentLines = "";
                    Debug.Assert(typ.fieldList.Count == 1);

                    foreach (Symbol sym in typ.fieldList) {
                        lst.Add(CleanupName(sym.name));
                        szContentLines += "    " + DescriptionTable_Field(lst, sym, 0) + ",\n";
                        lst.RemoveAt(lst.Count - 1);
                    }

                    szContentLines = szContentLines.TrimEnd(" \r\n".ToCharArray());

                    m_stm6.WriteLine();
                    m_stm6.WriteLine(SzConst +"A2C_ITEM {0}_description_table[] = {{", UseUScore(lst));
                    m_stm6.WriteLine(szContentLines);
                    m_stm6.WriteLine("};");

                    szReturn = UseUScore(lst);
                    if (typ.baseType == BaseType.SequenceOf) {
                        szType = "A2C_SEQUENCE_OF";
                        codenames.SetDefaultFunction(szType);
                    }
                    else {
                        szType = "A2C_SET_OF";
                        codenames.SetDefaultFunction(szType);
                    }

                    if (lst.Count > 2) {
                        codenames2 = new CodePointers(SzConst);
                        codenames2.SetStructureName(UseUScore(lst));
                        codenames2.SetDefaultFunction(szType);
                        codenames2.SubTable = true;
                    }
                    else {
                        codenames.SubTable = true;
                    }

                    fForce = true;
                    break;

                case BaseType.Class:
                    /*
                     * We need to emit the description table for this class
                     */

                    szContentLines = "";
                    foreach (Symbol sym in typ.fieldList) {
                        lst.Add(CleanupName(sym.name));
                        szContentLines += "    " + DescriptionTable_Field(lst, sym, 0) + ",\n";
                        lst.RemoveAt(lst.Count - 1);
                    }

                    //
                    //  This is the name of the table we are creating, and thus the type we are creating
                    //

                    szReturn = UseUScore(lst);

                    m_stm6.WriteLine();
                    m_stm6.WriteLine(SzConst +"A2C_ITEM {0}_description_table[] = {{", szReturn);
                    m_stm6.WriteLine(szContentLines);
                    m_stm6.WriteLine("};");

                    //
                    //  We also need to force emitting other infomration
                    //

                    szType = "A2C_CLASS";

                    if (lst.Count > 2) {
                        codenames2 = new CodePointers(SzConst);
                        codenames2.SetDefaultFunction(szType);
                        codenames.SubTable = true;
                    }
                    else {
                        codenames.SubTable = true;
                    }
                    codenames.SetDefaultFunction(szType);
                    fForce = true;
                    break;

                case BaseType.Tagged:
                    if (fSupressTag) {
                        EmitDescription(typ.subType, lst, false, fSkipAssign, fTop, ref codenames);
                        return;
                    }

                    lst.Add("*tag");
                    if ((typ.subType.baseType == BaseType.Assignment) ||
                        (typ.subType.baseType == BaseType.Leaf)) {
                        //  Nothing to do at this point
                        str1 = typ.subType.emitName;
                        codenames2 = new CodePointers(SzConst);
                        codenames2.SetStructureName(typ.subType.emitName);
                    }
                    else {
                        codenames2 = new CodePointers(SzConst);
                        EmitDescription(typ.subType, lst, false, fSkipAssign, false, ref codenames2);
                        str1 = codenames.DefaultName;
                    }
                    lst.RemoveAt(lst.Count - 1);

                    m_stm6.WriteLine();
                    m_stm6.WriteLine(SzConst +"A2C_ITEM {0}_description_table [] = {{", UseUScore(lst));
                    m_stm6.WriteLine("  {{ \"tag\", 0, {{A2C_TAG_CLASS_{0}, {1}}},  &{2}, 0, A2C_FLAGS_TAG_{3}, 0 }}", 
                                      typ.tagged.tagClass.ToString().ToUpper(), typ.tagged.value, codenames2.PDU_Name, typ.tagged.tagType.ToString().ToUpper());
                    m_stm6.WriteLine("};");
                    fForce = true;

                    codenames2 = null;
                    codenames.SubTable = true;
                    szReturn = UseUScore(lst);
                    szType = "A2C_TAG";
                    codenames.SetDefaultFunction(szType);
                    break;

                case BaseType.Symbol:
                    EmitDescription(typ.sym.type, lst, fSupressTag, fSkipAssign, fTop, ref codenames);
                    return;
                }
                
                /*
                 * Emit this table if we are looking at a rooted itm
                 */

                if (codenames2 != null) {
                    codenames2.EmitPDU(m_stm6, lst.Count == 2);

                    szReturn = UseUScore(lst);
                }

                codenames.SetStructureName(szReturn);
                return;
            }

            /// <summary>
            /// Build a string using "." as a seperator
            /// </summary>
            /// <param name="lst"></param>
            /// <returns></returns>
            String UseDots(List<String> lst)
            {
                int i;
                String szOut = null;

                for (i = 2; i < lst.Count; i++) {
                    if ((lst[i] != null) && (lst[i][0] != '*')) {
                        if (szOut == null) szOut = lst[i];
                        else szOut += "." + lst[i];
                    }
                    else if ((lst[i] != null) && (lst[i] == "*u")) {
                        if (szOut == null) szOut = "u";
                        else szOut += ".u";
                    }
                }

                return szOut;
            }

            String UseDotsM1(List<String> lst)
            {
                int i;
                String szOut = null;

                for (i = 2; i < lst.Count-1; i++) {
                    if (lst[i][0] != '*') {
                        if (szOut == null) szOut = lst[i];
                        else szOut += "." + lst[i];
                    }
                    else if (lst[i] == "*u") {
                        if (szOut == null) szOut = "u";
                        else szOut += ".u";
                    }
                }

                return szOut;
            }

            int UseDotsCount(List<String> lst)
            {
                int i;
                int c;

                for (i = 1, c = 1; i < lst.Count; i++) {
                    if ((lst[i] != null) && (lst[i].Length> 0) && (lst[i][0] != '*')) c += 1;
                }
                return c;
            }

            static public String UseUScore(List<String> lst)
            {
                int i;
                String szOut = lst[0];

                if (lst.Count > 1) szOut += lst[1];
                if (lst.Count > 2) szOut += "_";

                for (i = 2; i < lst.Count; i++) {
                    szOut += "_" + lst[i];
                }

                return CleanupName(szOut.Replace("*", ""));
            }

            static public String Use2UScore(List<String> lst)
            {
                int i;
                String szOut = "";

                for (i = 0; i < lst.Count; i++) {
                    if ((lst[i] != null) && (lst[i] != "")) {
                        if (szOut != "") szOut += "__";
                        szOut += lst[i];
                    }
                }

                return szOut;
            }


            String DescriptionTable_Field(List<String> lst, Symbol symField, int iExtensionNo)
            {
                String sz = "{";
                String szFlags = "";
                Tagging tags = null;

                // Name of field
                sz += "\"" + symField.name + "\", ";
                // offset of field
                if (UseDotsCount(lst) == 3) {
                    sz += String.Format("OFFSET_OF({0}, {1}), ", lst[0]+lst[1], UseDots(lst));
                }
                else {
                    sz += String.Format("REL_OFFSET_OF({0}, {1}, {2}), ", lst[0]+lst[1], UseDots(lst), UseDotsM1(lst));
                }

                if (symField.type != null) {
                    tags = FindTagging(symField.type);
                }
                if (tags == null) {
                    // tag class
                    sz += "{A2C_TAG_CLASS_UNIVERSAL, ";
                    // tag value
                    sz += "0}, ";
                }
                else {
                    switch (tags.tagClass) {
                    case TagClass.Universal:
                        sz += "{A2C_TAG_CLASS_UNIVERSAL, " + tags.value + "}, ";
                        break;

                    case TagClass.Application:
                        sz += "{A2C_TAG_CLASS_APPLICATION, " + tags.value + "}, ";
                        break;

                    case TagClass.Context:
                        sz += "{A2C_TAG_CLASS_CONTEXT, " + tags.value + "}, ";
                        break;

                    case TagClass.Private:
                        sz += "{A2C_TAG_CLASS_PRIVATE, " + tags.value + "}, ";
                        break;
                    }
                }
                // pfns
                if (symField.GetSymbolType() == SymbolType.TypeField) {
                    sz += "&A2C_OPEN_TYPE_descriptor, ";       // M00BUG - need to emit a function for this in order to do recursive lookup
                }
                else if (symField.flags.Pointer && (symField.type.baseType == BaseType.Assignment)) {
                    sz += "&" + symField.type.owner.codenames.PDU_Name + ", ";
                }
                else {
                    CodePointers codenames = new CodePointers(SzConst);
                    EmitDescription(symField.type, lst, true, false, true, ref codenames);
                    sz += "&" + codenames.PDU_Name + ", ";
                }
                // Default
                if ((symField.flags != null) && (symField.flags.valDefault != null)) {
                    Type typeReal = symField.type.SkipFluff();

                    if (szFlags.Length > 0) szFlags += " | ";
                    if ((typeReal.baseType == BaseType.Leaf) && 
                        (typeReal.dataType == DataType.Boolean)) {
                        if (symField.flags.valDefault.valueType == ValueType.SetOf) {
                            sz += "0,";
                        }
                        else {
                            szFlags += "A2C_FLAGS_DEFAULT";
                            sz += symField.flags.valDefault.bVal ? "(PBYTE) 0xff, " : "0, ";
                        }
                    }
                    else if ((typeReal.baseType == BaseType.Leaf) &&
                             ((typeReal.dataType == DataType.Integer) && (typeReal.emitName == "A2C_INTEGER_NATIVE")) ||
                             ((typeReal.dataType == DataType.Enumerated) && (typeReal.emitName == "A2C_ENUMERATED_NATIVE"))) {
                        Value valX = symField.flags.valDefault;
                        while (valX.valueType == ValueType.Symbol) {
                            valX = valX.sym.value;
                        }

                        szFlags += "A2C_FLAGS_DEFAULT";
                        sz += "(PBYTE) " + valX.iVal.ToString() + ", ";
                    }
                    else {
                        szFlags += "A2C_FLAGS_DEFAULT_PTR";
                        Debug.Assert(symField.flags.valDefault.valueType == ValueType.Symbol);
                        if (symField.flags.valDefault.sym.module.nameSpace == null) {
                            sz += "(PBYTE) &" + CleanupName(symField.flags.valDefault.sym.name) + ", ";
                        }
                        else {
                            sz += "(PBYTE) &" + CleanupName(symField.flags.valDefault.sym.module.nameSpace + symField.flags.valDefault.sym.name) + ", ";
                        }
                    }
                }
                else {
                    sz += "0, ";
                }
                // flags
                if (tags != null) {
                    switch (tags.tagType) {
                    case TagType.Implicit:
                    case TagType.Specified:
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C_FLAGS_TAG_IMPLICIT";
                        break;
                    case TagType.Explicit:
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C_FLAGS_TAG_EXPLICIT";
                        break;
                    }
                }
                if (symField.flags != null) {
                    if (symField.flags.Optional) {
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C_FLAGS_OPTIONAL";
                    }
                    if (symField.flags.Unique) {
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C_FLAGS_UNIQUE";
                    }
                    if (symField.flags.Pointer) {
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C_FLAGS_DATA_PTR";
                    }
                }

                if (szFlags.Length > 0) {
                    sz += szFlags + ", ";
                }
                else {
                    sz += "0, ";
                }
                // version
                sz += iExtensionNo.ToString();

                return sz + "}";
            }

            Tagging FindTagging(Type typ)
            {
                switch (typ.baseType) {
                case BaseType.Assignment:
                case BaseType.Constraint:
                case BaseType.NamedList:
                case BaseType.Param_Assignment:
                    return FindTagging(typ.subType);    // M00QUEST - may not be the correct thing to do.

                case BaseType.Leaf:
                    return typ.tagged;

                case BaseType.Tagged:
                    return typ.tagged;

                case BaseType.Sequence:
                case BaseType.SequenceOf:
                    return new Tagging(TagClass.Universal, TagType.Specified, 16);

                case BaseType.Set:
                case BaseType.SetOf:
                    return new Tagging(TagClass.Universal, TagType.Specified, 17);

                case BaseType.Choice:
                case BaseType.Class:
                case BaseType.ObjectClassField:
                case BaseType.OpenType:
                    return null;

                case BaseType.Symbol:
                    return FindTagging(typ.sym.type);
                }

                Debug.Assert(false, "Missing tag value");
                return null;
            }

            Byte[] SwapOrder(Byte[] rgb)
            {
                byte[] rgb2 = new byte[rgb.Length];
                for (int i = 0, j = rgb.Length - 1; i < rgb.Length; i++, j--) {
                    rgb2[j] = rgb[i];
                }
                return rgb2;
            }

            void EmitObject(Symbol sym)
            {
                List<String> lst = new List<string>();
                String sz = "";
                Type type = sym.type;

                if (Program.DebugLevel > 3) Console.Error.WriteLine("   Emit Object: '{0}'", sym.name);

                if (type.emitName == null) {
                    Debug.Assert(type.owner != null, "EmitObject " + sym.name);
                    EmitSymbol(type.owner);
                    sym.type.emitName = type.emitName;
                }
                else if ((type != sym.type) && (sym.type.emitName == null)) {
                    sym.type.emitName = type.emitName;
                }

                lst.Add(sym.module.nameSpace);
                lst.Add(CleanupName(sym.name));

                /*
                 * Start by emitting all of the values and types referenced by this object
                 */

                foreach (Symbol sym1 in sym.value.symList) {
                    if (Program.DebugLevel > 4) Console.Error.WriteLine("    Emit Field: {1} '{0}'", sym1.name, sym1.GetSymbolType().ToString());

                    switch (sym1.GetSymbolType()) {
                    case SymbolType.Dummy:
                        sz += "NULL, ";
                        break;

                    case SymbolType.Type:

                        if ((sym1.type.baseType == BaseType.Assignment) && (sym1.type.owner != null)) {
                            EmitSymbol(sym1.type.owner);
                            sz += "&" + sym1.type.emitName + "_descriptor, ";
                        }
                        else if (sym1.type.baseType == BaseType.Leaf) {
                            sz += "&" + sym1.type.emitName + "_descriptor, ";
                        }
                        else {
                            sz += "&" + sym.type.emitName + "_descriptor,";
                        }
                        break;

                    case SymbolType.Value:
                        lst.Add(CleanupName(sym1.name));
                        if (sym1.value.valueType == ValueType.Symbol) {
                            Type typeL = sym1.value.sym.type.SkipFluff();

                            //  Treat enum/int local symbols differently

                            if (!sym1.flags.Pointer &&
                                ((typeL.emitName == "A2C_INTEGER_NATIVE") ||
                                (typeL.emitName == "A2C_ENUMERATED_NATIVE"))) {
                                sz += CleanupName(sym1.value.sym.fullName) + ", ";
                            }
                            else {
                                sz += "&" + CleanupName(sym1.value.sym.fullName) + ", ";
                            }
                        }
                        else {
                            Symbol sym2 = new Symbol(UseUScore(lst), SymbolType.Value);
                            sym2.type = sym1.type;
                            sym2.value = sym1.value;
                            sym2.m_symTable = sym.m_symTable;
                            EmitValue(sym2);
                            sz += "&" + sym2.name + ", ";

                            sym1.value = new Value(sym2);
                        }
                        lst.RemoveAt(lst.Count - 1);
                        break;

                    case SymbolType.Object:
                    case SymbolType.ObjectSet:
                        lst.Add(CleanupName(sym1.name));
                        if (sym1.value.valueType == ValueType.Symbol) {
                            sz += "&" + CleanupName(sym1.value.sym.fullName) + ", ";
                        }
                        else {
                            Symbol sym2 = new Symbol(UseUScore(lst), SymbolType.Value);
                            sym2.type = sym1.type;
                            sym2.value = sym1.value;
                            sym2.m_symTable = sym.m_symTable;
                            EmitValue(sym2);
                            sz += "&" + sym2.name + ", ";

                            sym1.value = new Value(sym2);
                        }
                        lst.RemoveAt(lst.Count - 1);
                        break;

                    default:
                        Debug.Assert(false, "Missing case statement" + sym1.GetSymbolType());
                        break;
                    }
                }

                sz = sz.Substring(0, sz.Length - 2);

                if (Program.DebugLevel > 4) Console.Error.WriteLine("    Emit Object: final type = '{0}'", sym.type.emitName);

                m_stm3.WriteLine("extern " + SzConst + "{0} {1};", CleanupName(sym.type.emitName), UseUScore(lst));
                m_stm6.WriteLine(SzConst +"{0} {1} = {{ {2} }};", CleanupName(sym.type.emitName), UseUScore(lst), sz);
            }

            Boolean Code_Contents(Constraint cst, List<String> lst, CodePointers codenames, String szBase)
            {
                //
                //  We need to go down one level and get the relationship constraint below this point.
                //

                Constraint cst2 = cst.type.constraintList[0];

                return Code_Relation(cst2, lst, codenames, szBase);
            }

            Boolean Code_Relation(Constraint cst, List<String> lst, CodePointers codenames, String szBase)
            {
                String szBase2 = szBase;
                if ((szBase2 == null) || (szBase2 == "")) szBase2 = "_OPEN_TYPE";

                // A2C_COMPONENT_RELATIONS ItemInfo = {
                //  &ContentTypes, "&Type", 1,
                //  {"&id", REL_OFFSET_OF(CMS_ContentInfo, contentType, content)}
                //};

                m_stm5.WriteLine();
                m_stm5.WriteLine("A2C_COMPONENT_RELATIONS_TABLE {0}_Relationship_table[] = {{", UseUScore(lst));
                foreach (Symbol sym in cst.symlst) {
                    Type type = sym.m_sym.type.SkipFluff();
                    String str1 = sym.m_sym.fieldName;
                    String str2 = UseDots(lst);
                    int ix;

                    string[] rgstr1 = str1.Split('.');
                    string[] rgstr2 = str2.Split('.');

                    for (ix = 0; ix < rgstr1.Length; ix++) {
                        if (rgstr1[ix] != rgstr2[ix]) break;
                    }

                    Array.Resize(ref rgstr2, ix);

                    str2 = String.Join(".", rgstr2);

                    if (ix == 0) {
                        m_stm5.WriteLine("\t{{\"{0}\", {1}, OFFSET_OF({2}, {3})}},", type.fieldTokens.ToDotString(), sym.m_sym.depth, sym.m_sym.structureName, str1);
                    }
                    else {
                        m_stm5.WriteLine("\t{{\"{0}\", {1}, REL_OFFSET_OF({2}, {3}, {4})}},", type.fieldTokens.ToDotString(), sym.m_sym.depth, sym.m_sym.structureName, str1, str2);
                    }
                }
                m_stm5.WriteLine("};");            
                m_stm5.WriteLine("A2C_COMPONENT_RELATIONS {0}_Relationship = {{", UseUScore(lst));
                m_stm5.WriteLine("\t&{0}, \"{1}\", {2}, {3}_Relationship_table", cst.value.sym.module.nameSpace + CleanupName(cst.value.sym.name), cst.text, cst.symlst.Count,  UseUScore(lst));
                m_stm5.WriteLine("};");

                m_stm5.WriteLine();
                m_stm5.WriteLine("A2C_ERROR {0}_decode_der(A2C{1} * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,", UseUScore(lst), szBase2);
                m_stm5.WriteLine("\tA2C_TAG " + SzConst +"* ptag, A2C_STREAM_MEMORY * pstm)");
                m_stm5.WriteLine("{");
                m_stm5.WriteLine("    return A2C{1}_RELATION_decode_der(&{0}_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);", UseUScore(lst), szBase);
                m_stm5.WriteLine("}");

                m_stm5.WriteLine();
                m_stm5.WriteLine("A2C_ERROR {0}_decode_ber(A2C{1} * pos, PC_A2C_DESCRIPTOR pdesc, int flags, A2C_CONTEXT * pcxt,", UseUScore(lst), szBase2);
                m_stm5.WriteLine("\tA2C_TAG " + SzConst + "* ptag, A2C_STREAM_MEMORY * pstm)");
                m_stm5.WriteLine("{");
                m_stm5.WriteLine("    return A2C{1}_RELATION_decode_ber(&{0}_Relationship, pos, pdesc, flags, pcxt, ptag, pstm);", UseUScore(lst), szBase);
                m_stm5.WriteLine("}");

                codenames.SetOurFunctionName("DecodeDER", String.Format("{0}_decode_der", UseUScore(lst)));
                codenames.SetOurFunctionName("DecodeBER", String.Format("{0}_decode_ber", UseUScore(lst)));

                return true;
            }
        }
    }
