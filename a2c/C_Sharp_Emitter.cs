using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.IO;

namespace asn_compile_cs
{
    class CSharp_CodeEmitter : CodeEmitter
    {
        readonly StreamWriter m_fileOut;
        int m_indent = 0;
        int m_fNoWrite = 0;

        public CSharp_CodeEmitter (string fileName, CmdLineArgs args)
        {
            if (fileName.Substring (fileName.Length - 3) != ".cs") fileName += ".cs";
            m_fileOut = File.CreateText (fileName);
            EmitHeader ();
        }

        override public void Close ()
        {
            EmitFooter ();
            m_fileOut.Close ();
        }

        public void EmitHeader ()
        {
            m_fileOut.WriteLine ("using System;");
            m_fileOut.WriteLine ("using System.Collections.Generic;");
            m_fileOut.WriteLine ("using A2C;");
            m_fileOut.WriteLine ("");
        }

        public void EmitFooter ()
        {
            if (m_szLastNamespace != null) {
                Outdent ();
                WriteLine ("}");
            }
            WriteLine ("");
        }

        void Indent () { m_indent += 1; }
        void Outdent () { m_indent -= 1; }

        void WriteTab ()
        {
            int i;

            for (i = 0; i < m_indent; i++) m_fileOut.Write ("   ");
        }

        void Write (String str) { if (m_fNoWrite == 0) m_fileOut.Write (str); }
        void Write(String str, Object obj) { if (m_fNoWrite == 0) m_fileOut.Write(str, obj); }
        void Write(String str, Object obj1, Object obj2) { if (m_fNoWrite == 0) m_fileOut.Write(str, obj1, obj2); }
        void WriteLine() { if (m_fNoWrite == 0) m_fileOut.WriteLine(); }
        void WriteLine (String str)
        {
            if (m_fNoWrite == 0) {
                WriteTab();
                m_fileOut.WriteLine(str);
            }
        }

        void WriteLine(String str, Object obj) { if (m_fNoWrite == 0) { WriteTab(); m_fileOut.WriteLine(str, obj); } }
        void WriteLine(String str, Object obj1, Object obj2) { if (m_fNoWrite == 0) { WriteTab(); m_fileOut.WriteLine(str, obj1, obj2); } }
        void WriteLine(String str, Object obj1, Object obj2, Object obj3) { if (m_fNoWrite == 0) { WriteTab(); m_fileOut.WriteLine(str, obj1, obj2, obj3); } }


        private String Clean (String str) 
        { 
            str = str.Replace ('-', '_').Replace ("&", null); 
            //  Replace key words
            switch (str) {
            case "base":
            case "string":
                str = "_" + str;
                break;

            default:
                break;
            }

            return str;
        }

        static string m_szLastNamespace;

        private void EmitNamespace (string szNameSpace)
        {
            //
            //  If we are in the same name space, then nothing to emit
            //

            if (szNameSpace == m_szLastNamespace) return;

            //
            //  If there was a previous name space, then close it
            //

            if (m_szLastNamespace != null) {
                Outdent ();
                WriteLine ("}");
                WriteLine ("");
            }

            //
            //  Emit the new namespace
            //

            if (szNameSpace == "") {
                WriteLine ("namespace A2C {");
                Indent ();
            }
            else {
                WriteLine ("namespace {0} {{", szNameSpace);
                Indent ();
            }
            m_szLastNamespace = szNameSpace;
        }

        public override void PreEmitSymbol(Symbol sym)
        {
            //  Don't write out anything

            m_fNoWrite += 1;

            switch (sym.GetSymbolType()) {
            case SymbolType.Module:
                //  If we don't have a namespace on the module, make it "A2C"
                //

                // EmitNamespace (sym.module.nameSpace);
                //  We don't do anything to emit modules.
                break;

            //
            //  Parameterized items are not directly emitted.
            //
            case SymbolType.ParameterizedType:
                break;

            case SymbolType.Type:
                WriteLine ();
                EmitType (sym, sym.type);
                break;

            case SymbolType.Value:
                EmitValue (sym);
                break;

            case SymbolType.Object:
                EmitObject (sym);
                break;

            case SymbolType.ObjectClass:
                EmitObjectClass (sym);
                break;

            case SymbolType.ObjectSet:
                EmitObjectSet (sym);
                break;

            case SymbolType.Imported | SymbolType.Module:           // Name of an imported module
            case SymbolType.Imported | SymbolType.NotYetDefined:    // Indirect referene symbol
            case SymbolType.Imported | SymbolType.NotYetDefined | SymbolType.NameConflict:
                break;

            case SymbolType.Imported | SymbolType.Parameterized:
                WriteLine ("// Imported Parameterized {0}", sym.name);
                break;

            default:
                Debug.Assert (false);
                break;
            }

            m_fNoWrite -= 1;
        }

        override public void EmitSymbol (Symbol sym)
        {
            //  Is this symbol in the emit list?

            if (!sym.module.EmitSymbols) return;
            //
            //  Thanks to the C# compiler, we never have to recurse back into here
            //  so all of the code deailing with recusion in the C emitter can be omitted here.
            //

            switch (sym.GetSymbolType ()) {
            case SymbolType.Module:
                //  If we don't have a namespace on the module, make it "A2C"
                //

                EmitNamespace (sym.module.nameSpace);
                //  We don't do anything to emit modules.
                break;

            //
            //  Parameterized items are not directly emitted.
            //
            case SymbolType.ParameterizedType:
                break;

            case SymbolType.Type:
                WriteLine ();
                EmitType (sym, sym.type);
                break;

            case SymbolType.Value:
                EmitValue (sym);
                break;

            case SymbolType.Object:
                EmitObject (sym);
                break;

            case SymbolType.ObjectClass:
                EmitObjectClass (sym);
                break;

            case SymbolType.ObjectSet:
                EmitObjectSet (sym);
                break;

            case SymbolType.Imported | SymbolType.Module:           // Name of an imported module
            case SymbolType.Imported | SymbolType.NotYetDefined:    // Indirect referene symbol
            case SymbolType.Imported | SymbolType.NotYetDefined | SymbolType.NameConflict:
                break;

            case SymbolType.Imported | SymbolType.Parameterized:
                WriteLine ("// Imported Parameterized {0}", sym.name);
                break;

            default:
                Debug.Assert (false);
                break;
            }
        }

        void EmitType (Symbol symRoot, Type typ)
        {
            Debug.Assert (typ.baseType == BaseType.Assignment);
            EmitType (symRoot.name, typ.subType, null);
        }

        void EmitType (String symName, Type typ, List<Tagging> tagsIn)
        {
            String strConstructorArgs;
            String strConstructorBody;

            symName = Clean (symName);

            switch (typ.baseType) {
            case BaseType.Assignment:
                WriteLine ("public partial class {0} : {1} {{", symName, typ.owner.CSharpName);
                Indent ();
                EmitConstructors (symName, typ);
                EmitTagOverrides (tagsIn);
                Outdent ();
                WriteLine ("}");
                WriteLine ();
                break;

            case BaseType.Leaf:
                WriteLine ("public partial class {0} : {1} {{", symName, typ.CSharpName);
                Indent ();
                EmitConstructors (symName, typ);
                EmitTagOverrides (tagsIn);
                Outdent ();
                WriteLine ("}");
                WriteLine ();
                break;

            case BaseType.Set:
            case BaseType.Sequence:
                WriteLine ("public partial class {0} : {1} {{", symName, typ.baseType == BaseType.Sequence ? "A2C.Sequence" : "A2C.Set");
                Indent ();

                List<String> lstSzTable = new List<string> ();
                List<String> lstSzAccess = new List<string> ();
                String funcParams = "";
                String funcBody = "";

                int i = 0;
                foreach (Symbol sym1 in typ.fieldList) {
                    String sz;

                    switch (sym1.symType) {
                    case SymbolType.ExtensionGroup:
                        foreach (Symbol sym2 in sym1.symList) {
                            sz = EmitTableLine(sym2, 0);
                            if (sz != null) {
                                lstSzTable.Add(sz);

                                lstSzAccess.Add(String.Format("public {0} {1} {{ get {{ return ({0}) m_data[{2}];}} set {{ m_data[{2}] = value; }} }}", sym2.type.CSharpName, Clean(sym2.name), i));

                                funcParams += String.Format("{0} {1}, ", sym2.type.CSharpName, Clean(sym2.name));
                                funcBody += String.Format("m_data[{0}] = {1}; ", i, Clean(sym2.name));

                                i = i + 1;
                            }
                        }
                        break;

                    default:
                        sz = EmitTableLine(sym1, 0);
                        if (sz != null) {
                            lstSzTable.Add(sz);

                            lstSzAccess.Add(String.Format("public {0} {1} {{ get {{ return ({0}) m_data[{2}];}} set {{ m_data[{2}] = value; }} }}", sym1.type.CSharpName, Clean(sym1.name), i));

                            funcParams += String.Format("{0} {1}, ", sym1.type.CSharpName, Clean(sym1.name));
                            funcBody += String.Format("m_data[{0}] = {1}; ", i, Clean(sym1.name));

                            i = i + 1;
                        }
                        break;
                    }
                }

                if (funcParams != "") funcParams = funcParams.Substring (0, funcParams.Length - 2);

                WriteLine ("static readonly ASNTable[] m_table = new ASNTable[] {");
                Indent ();
                foreach (String sz in lstSzTable) {
                    WriteLine (sz + ",");
                }
                Outdent ();
                WriteLine ("};");
                WriteLine ("");
                WriteLine ("public {0} () : base (m_table) {{ }}", symName);
                if (funcParams != "") {
                    WriteLine ("public {0}({1}) : base (m_table) {{ {2} }}", symName, funcParams, funcBody);
                }
                WriteLine ("public {0}({0} var) : base(var) {{ }}", symName);
                WriteLine ("");

                foreach (String sz in lstSzAccess) {
                    WriteLine (sz);
                }
                Outdent ();
                WriteLine ("}");
                break;

            case BaseType.Class:
                WriteLine ("public partial class {0} : A2C.ClassObject {{", symName);
                Indent ();

                lstSzTable = new List<string> ();
                i = 0;
                strConstructorArgs = "";
                strConstructorBody = "";
                foreach (Symbol sym1 in typ.fieldList) {
                    String sz = EmitTableLine (sym1, 0);
                    if (sz != null) {
                        lstSzTable.Add (sz);

                        sz = BuildTypeName (sym1.type);
                        if (sym1.symType == SymbolType.ObjectSetField || sym1.symType == SymbolType.ValueSetField) sz += "__Set";

                        strConstructorArgs += String.Format (", {0} p{1}", sz, i);
                        strConstructorBody += String.Format ("m_data[{1}] = p{1}; ", sz, i);
                        i += 1;
                    }
                }

                WriteLine ("static readonly ASNTable[] m_table = new ASNTable[] {");
                Indent ();
                foreach (String sz in lstSzTable) {
                    WriteLine (sz + ",");
                }
                Outdent ();
                WriteLine ("};");
                WriteLine ();
                WriteLine ("public {0} ({1}) : base (m_table) {{ {2} }}", symName, strConstructorArgs.Substring(2), strConstructorBody);
                WriteLine ();

                Outdent ();
                WriteLine ("}");
                WriteLine ();

                WriteLine ("public partial class {0}__Set : A2C.ObjectSet {{", symName);
                Indent ();
                WriteLine ("public {0}__Set() {{ }}", symName);
                WriteLine ("public {0}__Set({0}[] rgobjs) : base(rgobjs) {{ }}", symName);
                Outdent ();
                WriteLine ("}");
                WriteLine ();
                break;

            case BaseType.SetOf:
            case BaseType.SequenceOf:
                WriteLine ("public partial class {0} : {1} {{", symName, typ.baseType == BaseType.SequenceOf ? "A2C.SequenceOf" : "A2C.SetOf");
                Indent ();

                string szTable = EmitTableLine (typ.fieldList[0], 0);

                WriteLine ("static readonly ASNTable m_table = {0};", szTable);
                WriteLine ();
                WriteLine ("public {0}() : base (m_table) {{ }}", symName);
                WriteLine ("public {0}({1}[] var) : base(m_table) {{ foreach (A2C.ASN obj in var) {{ m_lst.Add(obj);}} }}", symName, "A2C.ASN");
                WriteLine ("public {0}({0} var) : base(var) {{ }}", symName);
                WriteLine ();
                WriteLine ("public new {0} this[int i] {{ get {{ return ({0}) base.GetIndex(i); }} set {{ base.SetIndex(i, value); }} }}", typ.fieldList[0].type.CSharpName);
                WriteLine ("public void Add({0} value) {{ base.Add(value); }}", typ.fieldList[0].type.CSharpName);
                Outdent ();

                WriteLine ("}");
                break;

            case BaseType.Choice:
                WriteLine ("public partial class {0} : A2C.Choice {{", symName);
                Indent ();

                lstSzTable = new List<string> ();
                lstSzAccess = new List<string> ();
                String strChoices = "";
                String szT;

                i = 0;
                foreach (Symbol sym1 in typ.fieldList) {
                    if (sym1.symType == SymbolType.ExtensionGroup) {
                        foreach (Symbol sym2 in sym1.symList) {
                            szT = EmitTableLine (sym2, i);
                            if (szT != null) {
                                lstSzTable.Add (szT);
                                strChoices += String.Format ("{2} {0}={1}", Clean(sym2.name), i, i > 0 ? ", " : "");
                                lstSzAccess.Add (String.Format ("public {0} {1} {{ get {{ if (m_choice == (int) choice.{1}) return ({0}) m_data; return null; }} set {{ m_data = value; m_choice = (int) choice.{1}; }} }}", sym2.type.CSharpName, Clean(sym2.name)));

                                i = i + 1;
                            }
                        }
                    }
                    else {
                        szT = EmitTableLine (sym1, i);
                        if (szT != null) {
                            lstSzTable.Add (szT);

                            strChoices += String.Format ("{2} {0}={1}", Clean(sym1.name), i, i > 0 ? ", " : "");

                            lstSzAccess.Add (String.Format ("public {0} {1} {{ get {{ if (m_choice == (int) choice.{1}) return ({0}) m_data; return null; }} set {{ m_data = value; m_choice = (int) choice.{1}; }} }}", sym1.type.CSharpName, Clean(sym1.name)));

                            i = i + 1;
                        }
                    }
                }

                WriteLine ("static readonly ASNTable[] m_table = new ASNTable[] {");
                Indent ();
                foreach (String sz in lstSzTable) {
                    WriteLine (sz + ",");
                }
                Outdent ();
                WriteLine ("};");
                WriteLine ("");
                WriteLine ("public {0} () : base (m_table) {{}}", symName);
                WriteLine ("public {0}(choice which, A2C.ASN obj) : base(m_table, (int) which, obj) {{ }}", symName);
                WriteLine ("public {0}({0} obj) : base(obj) {{ }}", symName);
                WriteLine ("");
                WriteLine ("public enum choice {{{0}}};", strChoices);
                WriteLine ("");
                WriteLine ("public choice which { get { return (choice) m_choice; } }");
                foreach (String sz in lstSzAccess) {
                    WriteLine (sz);
                }
                Outdent ();
                WriteLine ("};");
                break;

            case BaseType.Constraint:
                EmitType (symName, typ.subType, tagsIn);
                break;

            case BaseType.Tagged:
                if (tagsIn == null) tagsIn = new List<Tagging> ();
                tagsIn.Add (typ.tagged);
                EmitType (symName, typ.subType, tagsIn);
                break;

            case BaseType.NamedList:
                WriteLine ("public partial class {0} : {1} {{", symName, typ.subType.CSharpName);
                Indent ();

                lstSzTable = new List<string> ();
                lstSzAccess = new List<string> ();
                strChoices = "";

                i = 0;
                foreach (Symbol sym1 in typ.fieldList) {

                    switch (sym1.symType) {
                    case SymbolType.Exception:
                    case SymbolType.Extension:
                    case SymbolType.ExtensionEnd:
                    case SymbolType.ExtensionGroup:
                        break;

                    default:
                        strChoices += String.Format ("{2} {0}={1}", Clean(sym1.name), sym1.value.iVal, i > 0 ? ", " : "");
                        break;
                    }

                    i = i + 1;
                }

                WriteLine ();
                WriteLine ("public enum values {{{0}}};", strChoices);
                WriteLine ();
                WriteLine ("public {1} () {{ }}", typ.subType.CSharpName, symName);
                if (typ.subType.dataType == DataType.BitString) {
                    WriteLine ("public {1} (values val) {{ SetBit((int) val); }}", typ.subType.CSharpName, symName);
                    WriteLine ("public {0} (byte[] rgb) : base(rgb) {{ }}", symName);
                    WriteLine ("public {0} (byte[] rgb, int cbitsUsed) : base(rgb, cbitsUsed) {{ }}", symName);
                }
                else {
                    WriteLine ("public {1} (values val) : base((int) val) {{ }}", typ.subType.CSharpName, symName);
                }

                WriteLine ("public {1} ({1} val) : base(val) {{ }}", typ.subType.CSharpName, symName);
                if (typ.subType.dataType != DataType.BitString) {
                    WriteLine ();
                    WriteLine ("public new values Value { get { return (values) base.Value; } set { base.Value = (int) value; }}");
                }
                if (typ.subType.dataType == DataType.BitString) {
                    WriteLine ();
                    WriteLine ("public void ClearBit(values val) {{ base.ClearBit((int) val); }}");
                    WriteLine ("public void SetBit(values val) {{ base.SetBit((int) val); }}");
                    WriteLine ("public bool TestBit(values val) {{ return base.TestBit((int) val); }}");
                }
                Outdent ();
                WriteLine ("};");
                break;

            case BaseType.Param_Assignment:
                EmitType (symName, typ.subType, tagsIn);
                break;

            case BaseType.ObjectClassField:
                Symbol symT = typ.GetObjectClassField ();
                switch (symT.symType) {
                case SymbolType.ValueField:
                    EmitType (symName, symT.type, tagsIn);
                    break;

                default:
                    WriteLine ("// Object Class Field {0} {1}", symName, symT.symType);
                    Debug.Assert (false);
                    break;
                }
                break;

            default:
                Debug.Assert (false, "Unknown base type");
                break;
            }
        }

        void EmitTagOverrides (List<Tagging> tags)
        {
            if (tags != null) {
                String s1 = "";
                foreach (Tagging oneTag in tags) {
                    if (s1 != "") s1 += ", ";
                    s1 += oneTag.Format();
                }
                if (s1 != "") {
                    WriteLine ();
                    WriteLine ("private Tag[] m_tags = new Tag[]{{{0}}};", s1);
                    WriteLine ();
                    WriteLine ("protected override void _Decode (A2C_FLAGS flags, bool fDecodeAsDer, Context ctxt, Tag[] tagChild, ParserStream stm)");
                    WriteLine ("{");
                    Indent ();
                    WriteLine ("Tag[] tag2 = Tag.Append(tagChild, m_tags);");
                    WriteLine ("base._Decode (flags, fDecodeAsDer, ctxt, tag2, stm);");
                    Outdent ();
                    WriteLine ("}");
                    WriteLine ();
                    WriteLine ("protected override void _Encode (A2C_FLAGS flags, bool fEncodeAsDer, Context cctxt, Tag[] tag, Stream stm)");
                    WriteLine ("{");
                    Indent ();
                    WriteLine ("Tag[] tag2 = Tag.Append (tag, m_tags);");
                    WriteLine ("base._Encode (flags, fEncodeAsDer, cctxt, tag2, stm);");
                    Outdent ();
                    WriteLine ("}");
                }
            }
        }

        void EmitConstructors (String strName, Type symType)
        {
            switch (symType.baseType) {
            case BaseType.Assignment:
            case BaseType.Param_Assignment:
                EmitConstructors (strName, symType.subType);
                break;

            case BaseType.Constraint:
                EmitConstructors (strName, symType.subType);
                break;

            case BaseType.Choice:
                WriteLine ("public {0}() {{ }}", strName);
                WriteLine ("public {0}(choice which, A2C.ASN obj) : base(which, obj) {{ }}", strName);
                WriteLine ("public {0}({0} obj) : base(obj) {{ }}", strName);
                break;

            case BaseType.Leaf:
                switch (symType.dataType) {
                case DataType.BitString:
                    WriteLine("public {0}() {{ }}", strName);
                    WriteLine ("public {0}(byte[] bytes, int cbitsUsed) : base (bytes, cbitsUsed) {{ }}", strName);
                    WriteLine ("public {0}(byte[] bytes) : base(bytes) {{ }}", strName);
                    WriteLine ("public {0}({0} val) : base(val) {{ }}", strName);
                    break;

                case DataType.Boolean:
                    WriteLine ("public {0}() {{ }}", strName);
                    WriteLine ("public {0}(bool val) : base(val) {{ }}", strName);
                    WriteLine ("public {0}({0} val) : base(val) {{ }}", strName);
                    break;

                case DataType.Integer:
                    WriteLine ("public {0}(){{ }}", strName);
                    WriteLine ("public {0}(Int64 val) : base(val) {{ }}", strName);
                    WriteLine("public {0}(byte[] val) : base(val) {{ }}", strName);
                    WriteLine("public {0}({0} item) : base (item) {{ }}", strName);
                    break;

                case DataType.Null:
                    WriteLine ("public {0}() {{ }}", strName);
                    WriteLine ("public {0}({0} val) : base (val) {{ }}", strName);
                    break;

                case DataType.ObjectIdentifier:
                    WriteLine ("public {0}() {{ }}", strName);
                    WriteLine ("public {0}(string oidValue) : base(oidValue) {{ }}", strName);
                    WriteLine ("public {0}({0} oidValue) : base(oidValue) {{ }}", strName);
                    break;

                case DataType.OctetString:
                    WriteLine ("public {0}() {{ }}", strName);
                    WriteLine ("public {0}(byte[] rgb) : base(rgb) {{ }}", strName);
                    WriteLine ("public {0}({0} val) : base(val) {{ }}", strName);
                    break;

                case DataType.String:
                    WriteLine ("public {0}(){{ }}", strName);
                    WriteLine ("public {0}(String str) : base(str) {{ }}", strName);
                    WriteLine ("public {0}({0} str) : base(str) {{ }}", strName);
                    break;

                default:
                    WriteLine ("Emit Constructor {0} -- B", symType.dataType);
                    break;
                }
                break;

            case BaseType.Sequence:
                String strArgs = "";
                String strParams = "";

                foreach (Symbol sym in symType.fieldList) {
                    switch (sym.symType) {
                    case SymbolType.Exception:
                    case SymbolType.Extension:
                    case SymbolType.ExtensionEnd:
                        break;

                    case SymbolType.ExtensionGroup:
                        foreach (Symbol sym2 in sym.symList) {
                            strArgs += String.Format("{0} {1}, ", BuildTypeName(sym2.type), Clean(sym2.name));
                            strParams += String.Format("{0}, ", Clean(sym2.name));
                        }
                        break;

                    default:
                        strArgs += String.Format ("{0} {1}, ", BuildTypeName (sym.type), Clean(sym.name));
                        strParams += String.Format ("{0}, ", Clean(sym.name));
                        break;
                    }
                }

                WriteLine ("public {0}() {{ }}", strName);
                if (strParams != "") {
                    WriteLine ("public {0}({1}) : base({2}) {{ }}", strName, strArgs.Substring (0, strArgs.Length - 2), strParams.Substring (0, strParams.Length - 2));
                }
                WriteLine ("public {0}({0} var) : base(var) {{ }}", strName);
                break;
                    
            case BaseType.SequenceOf:
            case BaseType.SetOf:
                WriteLine ("public {0}() {{ }}", strName);
                WriteLine ("public {0}({1}[] val) : base(val) {{ }}", strName, symType.fieldList[0].type.owner.CSharpName);
                WriteLine ("public {0}({0} val) : base(val) {{ }}", strName);
                break;

            case BaseType.Tagged:
                EmitConstructors (strName, symType.subType);
                break;

            default:
                WriteLine ("Emit Constructor {0} -- A", symType.baseType);
                break;
            }
        }

        public void EmitValue (Symbol sym)
        {
            WriteLine ("public static partial class values {");
            Indent ();

            WriteLine ("public static {0} {1} = {2};", BuildTypeName(sym.type), Clean(sym.name), BuildValue (sym.value));

            Outdent ();
            WriteLine ("}");
            WriteLine ();

            if (BuildTypeName(sym.type) == "A2C.ObjectIdentifier") {
                WriteLine("public partial class OIDS {");
                Indent();
                WriteLine("public const string {0} = {1};", Clean(sym.name), OidToString(sym.value));
                Outdent();
                WriteLine("}");
                WriteLine();
            }
        }

        private string BuildTypeName (Type symType)
        {
            if (symType == null) return "A2C.ASN";

            switch (symType.baseType) {
            case BaseType.Leaf:
                return symType.CSharpName;

            case BaseType.Assignment:
                return symType.owner.CSharpName;

            case BaseType.Constraint:
            case BaseType.Param_Assignment:
            case BaseType.Tagged:
                return BuildTypeName (symType.subType);

            case BaseType.Symbol:
                return symType.CSharpName;

            case BaseType.Sequence:
            case BaseType.SequenceOf:
            case BaseType.Set:
            case BaseType.SetOf:
                if (symType.CSharpName != null) return symType.CSharpName;
                return symType.baseType.ToString();

            default:
                return symType.baseType.ToString ();
            }
        }

        String BuildValue (Value val)
        {
            if (val == null) return "null";

            String str = "";
            String strSep;
            bool fFirst;
            String strT;

            switch (val.valueType) {
            case ValueType.Binary:
                Type typeT = val.type.SkipFluff();
                switch (typeT.dataType) {
                case DataType.BitString:
                    str = string.Format ("new {0}({1}, {2})", val.type.CSharpName, val.bin.ToByteArray(), val.bin.cbits);
                    break;

                case DataType.OctetString:
                    str = String.Format ("new {0}({1})", val.type.CSharpName, val.bin.ToByteArray());
                    break;

                default:
                    Debug.Assert(false);
                    str += "Binary " + val.type.dataType;
                    break;
                }
                break;

            case ValueType.Boolean:
                return String.Format("new {0}({1})", val.type.CSharpName,  val.bVal.ToString ().ToLower());

            case ValueType.Integer:
                return String.Format("new {0}({1})", val.type.CSharpName,  val.iVal.ToString ());

            case ValueType.List:
                String strEnd = "";

                typeT = null;
                if (val.type != null) {
                    typeT = val.type.SkipFluff ();
                    while (typeT.baseType == BaseType.ObjectClassField) {
                        Symbol symT = typeT.GetObjectClassField ();
                        switch (symT.symType) {
                        case SymbolType.ValueField:
                            typeT = symT.type.SkipFluff ();
                            break;

                        default:
                            typeT = Type.Open;
                            break;
                        }
                    }
                }

                if ((typeT != null) && (typeT.dataType == DataType.ObjectIdentifier)) {
                    strSep = ".";
                    str = "\"";
                    strEnd = "\"";
                }
                else if (typeT != null) {
                    str = "";
                    strSep = ", ";
                    strEnd = "";
                }
                else {
                    str = "{";
                    strSep = ", ";
                }
                fFirst = true;

                foreach (Value val2 in val.valueList) {
                    if (fFirst) fFirst = false;
                    else str += strSep;

                    str += BuildValue (val2);
                }

                if (typeT != null) str = String.Format ("new {0}({1}{2})", val.type.CSharpName, str, strEnd);
                else str += "}";
                break;

            case ValueType.Null:
                str = String.Format ("new {0}()", val.type.CSharpName);
                break;

            case ValueType.OIDNode:
                return val.oidNode.iValue.ToString ();

            case ValueType.Symbol:
                if ((val.sym.symType & SymbolType.InList) != 0) {
                    str = String.Format ("new {0}({1})", val.sym.type.CSharpName, val.sym.CSharpName);
                }
                else {
                    return val.sym.CSharpName;
                }
                break;

            case ValueType.SymbolList:
                fFirst = true;
                if (val.type == null) str = "new MissingObjectType(";
                else str = String.Format ("new {0}(", val.type.CSharpName);
                foreach (Symbol sym2 in val.symList) {
                    if (fFirst) fFirst = false;
                    else str += ", ";

                    str += BuildValue(sym2.value);
                }
                str += ")";
                break;

            case ValueType.SetOf:
                if (val.type == null) str = "new MISSTING_TYPE[]{";
                else str = String.Format("new {0}__Set(new {0}[]{{", val.type.CSharpName);
                fFirst = true;
                foreach (Value val2 in val.valueList) {
                    strT = BuildValue (val2);
                    if (strT != null) {
                        if (fFirst) fFirst = false;
                        else str += ", ";

                        str += strT;
                    }
                }
                str += "})";
                break;

            case ValueType.Extension:
                return null;

            case ValueType.ExtractFromObject:
                str = "*ExtractFromObject*";
                break;

            case ValueType.ExtractFromObjectSet:
                str = "/*ExtractFromObjectSet*/null";
                break;

            default:
                Debug.Assert (false, "Unkonwn value type");
                return val.valueType.ToString ();
            }

            return str;
        }

        String OidToString(Value val)
        {
            bool fFirst;
            Type typeT;
            String str;

            if (val == null) return "null";

            switch (val.valueType) {
            case ValueType.OIDNode:
                return val.oidNode.iValue.ToString();

            case ValueType.Symbol:
                return OidToString(val.sym.value);

            case ValueType.List:
                typeT = null;
                if (val.type != null) {
                    typeT = val.type.SkipFluff();
                    while (typeT.baseType == BaseType.ObjectClassField) {
                        Symbol symT = typeT.GetObjectClassField();
                        switch (symT.symType) {
                        case SymbolType.ValueField:
                            typeT = symT.type.SkipFluff();
                            break;

                        default:
                            typeT = Type.Open;
                            break;
                        }
                    }
                }

                str = "\"";
                fFirst = true;

                foreach (Value val2 in val.valueList) {
                    if (fFirst) fFirst = false;
                    else str += ".";

                    str += BuildValue(val2);
                }

                str += "\"";
                return str;

            default:
                Debug.Assert(false, "Unknown Value Type in OidToString = " + val.valueType.ToString());
                return "null";
            }
        }

        String EmitTableLine (Symbol symField, int iExtensionNo)
        {
            bool fNoCreator = false;

            switch (symField.GetSymbolType ()) {
            //
            //  Begin by dealing with any potential extension markers
            //

            case SymbolType.Extension:
            case SymbolType.ExtensionGroup:
            case SymbolType.ExtensionEnd:
                return null;

            //
            //  Now deal with exception markers
            //

            case SymbolType.Exception:
                return null;

            //
            //  Deal with Class emissions
            //

            case SymbolType.TypeField:
                return String.Format ("new ASNTable(\"{0}\", null, null, null, 0, 0)", symField.name);

            case SymbolType.Component:
            case SymbolType.ValueField:
            case SymbolType.ValueSetField:
            case SymbolType.ObjectField:
            case SymbolType.ObjectSetField:
                //
                //  Based on the structure inside of here - we may need to emit that structure
                //

                Type typ2 = symField.type;
                String szTypeName = null;
                bool fTagged = false;

                bool fDone = false;
                while (!fDone) {
                    switch (typ2.baseType) {
                    case BaseType.Assignment:
                        if (typ2.owner.parent != null) {
                            EmitType (typ2.owner.name, typ2.owner.type.subType, null);
                            szTypeName = typ2.owner.CSharpName;
                            Debug.Assert (typ2.owner.m_symModule == null);
                        }
                        else {
                            szTypeName = typ2.CSharpName;
                        }
                        fDone = true;
                        break;

                    case BaseType.Leaf:
                        fDone = true;
                        szTypeName = typ2.CSharpName;
                        break;

                    case BaseType.Choice:
                    case BaseType.Set:
                    case BaseType.SetOf:
                    case BaseType.Sequence:
                    case BaseType.SequenceOf:
                        szTypeName = "T_" + symField.name;
                        EmitType (szTypeName, typ2, null);
                        typ2.CSharpName = szTypeName;
                        WriteLine ();
                        fDone = true;
                        break;

                    case BaseType.Symbol:
                        szTypeName = typ2.sym.CSharpName;
                        fDone = true;
                        break;

                    case BaseType.Tagged:
                        if (fTagged) {
                            szTypeName = "T_" + symField.name;
                            EmitType (szTypeName, typ2.subType, null);
                            WriteLine ();
                            fDone = true;
                        }
                        else {
                            typ2 = typ2.subType;
                            fTagged = true;
                        }
                        break;

                    case BaseType.Constraint:       // M00BUG - Should emit code to enforce the constraint somewhere
                        typ2 = typ2.subType;
                        break;

                    case BaseType.Param_Assignment:
                        typ2 = typ2.subType;
                        break;

                    case BaseType.OpenType:
                        fNoCreator = true;
                        fDone = true;
                        szTypeName = "A2C.ASN";
                        break;

                    case BaseType.ObjectClassField:
                        Symbol symT = typ2.GetObjectClassField ();
                        switch (symT.symType) {
                        case SymbolType.ValueField:
                            typ2 = symT.type;
                            break;

                        case SymbolType.TypeField:
                            fDone = true;
                            fNoCreator = true;
                            szTypeName = "A2C.ASN";
                            break;

                        default:
                            WriteLine ("// Object Class Field {0}", symT.name);
                            fDone = true;
                            fNoCreator = true;
                            break;
                        }
                        break;

                    default:
                        Debug.Assert (false, "What do we do with " + typ2.baseType);
                        szTypeName = "T_" + symField.name;
                        fDone = true;
                        break;
                    }
                }

                if (szTypeName != null) szTypeName = Clean (szTypeName);
                if (symField.type.CSharpName == null) symField.type.CSharpName = szTypeName;

                String str = "new ASNTable(";
                String szFlags = "";
                Tagging[] tags = null;

                // Add name of field
                str += "\"" + symField.name + "\", ";

                //  Build the tagging 
                if (symField.type != null) {
                    tags = FindTagging (symField.type);
                }
                if (tags == null) {
                    str += "null, ";
                }
                else {
                    string strTagType;

                    str += "new Tag[]{";
                    for (int iTag = 0; iTag < tags.Length; iTag++) {
                        if (iTag != 0) str += ", ";

                        if ((tags[iTag].tagType == TagType.Implicit) || (tags[iTag].tagType == TagType.Specified)) strTagType = "TagType.Implicit";
                        else strTagType = "TagType.Explicit";

                        switch (tags[iTag].tagClass) {
                        case TagClass.Context:
                            str += "new Tag(TagClass.Context, " + tags[iTag].value + "," + strTagType + ")";
                            break;

                        case TagClass.Application:
                            str += "new Tag(TagClass.Application, " + tags[iTag].value + "," + strTagType + ")";
                            break;

                        case TagClass.Private:
                            str += "new Tag(TagClass.Private, " + tags[iTag].value + "," + strTagType + ")";
                            break;

                        case TagClass.Universal:
                            str += "new Tag(TagClass.Universal, " + tags[iTag].value + "," + strTagType + ")";
                            break;
                        }
                    }
                    str += "}, ";
                }

                //  Emit creater function
                if (fNoCreator) {
                    str += "null, ";
                }
                else {
                    if ((tags != null) && (tags[0].tagType == TagType.Implicit)) {
                        str += "new A2C.ASNType(typeof(" + szTypeName + ")), ";
                    }
                    else {
                        str += "new A2C.ASNType(typeof(" + szTypeName + ")), ";
                    }
                }

                //  Emit default value if one exists
                if ((symField.flags != null) && (symField.flags.valDefault != null)) {
                    str += BuildValue(symField.flags.valDefault) + ", ";
                }
                else {
                str += "null, ";
                }

                //  Emit flags
                szFlags = "";

                if (symField.flags != null) {
                    if (symField.flags.Optional) {
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C.ASNTableFlags.Optional";
                    }
                    if (symField.flags.Unique) {
                        if (szFlags.Length > 0) szFlags += " | ";
                        szFlags += "A2C.ASNTableFlags.Unique";
                    }
                }

                if (szFlags.Length > 0) str += szFlags + ", ";
                else str += "0, ";

                //  Emit trailer and then write it out
                str += "0";
                str += ")";

                return str;

            default:
                Debug.Assert (false, "What do we do with " + symField.symType);
                break;
            }
            return null;
        }

        Tagging[] FindTagging (Type typ)
        {
            switch (typ.baseType) {
            case BaseType.Assignment:
            case BaseType.Param_Assignment:
                return FindTagging(typ.subType);    //  Needed for choice

            case BaseType.Constraint:
            case BaseType.NamedList:
                return FindTagging (typ.subType);    // M00QUEST - may not be the correct thing to do.

            case BaseType.Leaf:
                return new Tagging[]{typ.tagged};

            case BaseType.Tagged:
                Tagging[] tagsT = FindTagging (typ.subType);
                if (tagsT == null) return new Tagging[] { typ.tagged };

                Tagging[] tagsT2 = new Tagging[tagsT.Length + 1];
                tagsT2[0] = typ.tagged;
                Array.Copy (tagsT, 0, tagsT2, 1, tagsT.Length);
                return tagsT2;

            case BaseType.Sequence:
            case BaseType.SequenceOf:
                return new Tagging[]{new Tagging (TagClass.Universal, TagType.Specified, 16)};

            case BaseType.Set:
            case BaseType.SetOf:
                return new Tagging[]{new Tagging (TagClass.Universal, TagType.Specified, 17)};

            case BaseType.Choice:
            case BaseType.Class:
            case BaseType.ObjectClassField:
            case BaseType.OpenType:
                return null;

            case BaseType.Symbol:
                return FindTagging (typ.sym.type);
            }

            Debug.Assert (false, "Missing tag value");
            return null;
        }

        void EmitObjectClass (Symbol sym)
        {
            EmitType (Clean (sym.name), sym.type.subType, null);
        }

        public void EmitObject (Symbol sym)
        {
            WriteLine ("public static partial class objects {");
            Indent ();

            String strVal = BuildValue(sym.value);
            WriteLine ("public static {0} {1} = {2};", BuildTypeName (sym.type), Clean (sym.name), strVal);

            Outdent ();
            WriteLine ("}");
            WriteLine ();
        }

        void EmitObjectSet(Symbol sym)
        {
            WriteLine ("public static partial class objects {");
            Indent ();

            WriteLine ("public static {0}__Set {1} = {2};", BuildTypeName (sym.type), Clean (sym.name), BuildValue (sym.value));

            Outdent ();
            WriteLine ("}");
            WriteLine ();
        }


    }
}
