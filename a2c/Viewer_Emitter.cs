using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace asn_compile_cs
{
    enum asnTypes { None=0, Sequence=16, Set=17, SymRef=128, Leaf, Any, SeqOf, SetOf, Extension, ExtensionEnd, Choice, List, Tag, ITag };

    class MyFileStream
    {
        FileStream m_fs;

        public MyFileStream(FileStream fs) { m_fs = fs; }

        public void Close() { m_fs.Close(); }

        public void WriteBytes(byte[] rgb)
        {
            WriteI1((byte) rgb.Length);
            m_fs.Write(rgb, 0, rgb.Length);
        }

        public void WriteI1(byte i)
        {
            byte[] rgb = new byte[1] { i };
            m_fs.Write(rgb, 0, 1);
        }

        public void WriteI2(Int32 i)
        {
            byte[] rgb = BitConverter.GetBytes(i);
            m_fs.Write(rgb, 0, 2);
        }

        public void WriteI4(int i)
        {
            byte[] rgb = BitConverter.GetBytes(i);
            m_fs.Write(rgb, 0, 4);
        }

        public void WriteString(string str)
        {
            if (str == null) {
                m_fs.Write(new byte[1], 0, 1);
            }
            else {
                byte[] rgbName = UTF8Encoding.UTF8.GetBytes(str);

                m_fs.Write(BitConverter.GetBytes((byte) rgbName.Length), 0, 1);
                m_fs.Write(rgbName, 0, rgbName.Length);
            }

        }
    }

    abstract class ViewerNode
    {
        protected int m_indexEmit = -1;

        public int index { get { return m_indexEmit; } set { m_indexEmit = value; } }

        abstract public void Emit(MyFileStream fs);
    }

    abstract class ViewerConstraintNode : ViewerNode
    {
        protected Constraint m_ct;
    }

    class ViewerDummyNode:ViewerNode
    {
        public ViewerDummyNode()
        {
            m_indexEmit = -2;
        }

        public override void Emit(MyFileStream fs)
        {
            throw new NotImplementedException();
        }
    }

    class ViewerRelationConstraintNode : ViewerConstraintNode
    {
        Symbol m_class;
        List<int> m_fieldList = new List<int>();
        bool m_Closed;
        ViewerValueSetNode m_valList;
        ViewerObjectSetNode m_objList;
        int m_iTarget;
        ViewerClassNode m_classNode;

        public ViewerRelationConstraintNode(Constraint ct)
        {
            m_Closed = true;
            m_ct = ct;
        }
        public bool Closed { get { return m_Closed; } set { m_Closed = value; } }
        public Symbol objectClass
        {
            get { return m_class; }
            set { m_class = value; }
        }
        public ViewerClassNode objectNode { set { m_classNode = value; } }
        public int target { set { m_iTarget = value; } }
        public ViewerObjectSetNode ObjectList { set { m_objList = value; } }
        public ViewerValueSetNode ValueList { set { m_valList = value; } }

        public void AddField(int iField)
        {
            m_fieldList.Add(iField);
        }

        override public void Emit(MyFileStream fs)
        {
            if (m_objList != null) {
                fs.WriteString("CONSTRAINT");
                fs.WriteI1(1);
                if (m_class != null) fs.WriteI2(m_class.ViewerNode.index);
                else fs.WriteI2(m_classNode.index);
                fs.WriteI1((byte) m_fieldList.Count);
                foreach (int i in m_fieldList) fs.WriteI1((byte) i);
                fs.WriteI2(m_objList.index);
            }
        }
    }

    class ViewerTableConstraintNode : ViewerConstraintNode
    {
        Symbol m_class;
        int m_iField;
        bool m_Closed;
        ViewerValueSetNode m_valList;
        ViewerObjectSetNode m_objList;
        ViewerClassNode m_classNode;

        public ViewerTableConstraintNode(Constraint ct)
        {
            m_Closed = true;
            m_ct = ct;
        }

        public bool Closed { get { return m_Closed; }  set { m_Closed = value; } }
        public int iField { set { m_iField = value; } }
        public Symbol objectClass
        {
            get { return m_class; }
            set { m_class = value; }
        }
        public ViewerClassNode objectNode { set { m_classNode = value; } }
        public ViewerObjectSetNode ObjectList { set { m_objList = value; } }
        public ViewerValueSetNode ValueList { set { m_valList = value; } }

        override public void Emit(MyFileStream fs)
        {
            fs.WriteString("CONSTRAINT");
            if (m_objList != null) {
                fs.WriteI1(0);
                fs.WriteI2(m_objList.index);
                fs.WriteI1((byte) (m_Closed ? 1 : 0));
                fs.WriteI1((byte) m_iField);
            }
            else {
                fs.WriteI1(0x80);
                fs.WriteI2(m_valList.index);
            }
        }
    }

    class ViewerTypeNode : ViewerNode
    {
        Tagging m_tag;
        asnTypes m_asnType;
        string m_fieldName;
        List<ViewerTypeNode> m_children;
        Symbol m_symRef = null;
        byte m_flags = 0;
        static int s_indexEmit = 0;
        static int s_emitCount = 0;
        ViewerConstraintNode m_constraint;

        static List<ViewerTypeNode> s_EmissionList = new List<ViewerTypeNode>();

        public ViewerConstraintNode constraint { set { Debug.Assert(m_constraint == null); m_constraint = value; m_flags |= 8; } }
        public string fieldName { set { m_fieldName = value; } }
        public Tagging tag { set { m_tag = value; } }
        public asnTypes asnType { set { m_asnType = value; } }
        public List<ViewerTypeNode> children { get { if (m_children == null) m_children = new List<ViewerTypeNode>(); return m_children; } }
        public Symbol symbol { get { return m_symRef; } set { m_symRef = value; } }
        public bool Optional { get { return (m_flags & 1) == 1; } set { if (value)  m_flags |= 1; else m_flags &= 0xe; } }

        public void AddForEmit(ViewerTypeNode node)
        {
            Debug.Assert(node.m_indexEmit == -1);

            node.m_indexEmit = s_emitCount;

        }

        public int ChildCount()
        {
            int i = 0;
            if (m_asnType == asnTypes.List) return m_children.Count - 1;
            return i;
        }

        override public void Emit(MyFileStream fs)
        {
            Debug.Assert(m_indexEmit == -1 || m_indexEmit == s_indexEmit);

            if (m_asnType == asnTypes.List) {
                foreach (ViewerTypeNode node2 in m_children) node2.Emit(fs);
                return;
            }

            //  1x type
            //  1x tag
            //  2x start
            //  1x count
            //  1x flags
            //  nx name

            fs.WriteI1((byte) m_asnType);
            if (m_tag != null) fs.WriteI1((byte) m_tag.Encoded);
            else fs.WriteI1(0);

            switch (m_asnType) {
            case asnTypes.Choice:
            case asnTypes.Sequence:
            case asnTypes.Set:
            case asnTypes.Tag:
            case asnTypes.ITag:
            case asnTypes.SeqOf:
            case asnTypes.SetOf:
                fs.WriteI2(m_children[0].m_indexEmit);
                fs.WriteI1((byte) m_children[0].children.Count);
                break;

            case asnTypes.SymRef:
                fs.WriteI2(m_symRef.ViewerNode.index);
                fs.WriteI1(0);
                break;

            default:
                fs.WriteI2(0);
                fs.WriteI1(0);
                break;
            }

            fs.WriteI1(m_flags);
            if (m_constraint != null) fs.WriteI2(m_constraint.index);
            else fs.WriteI2(0);
            fs.WriteString(m_fieldName);

            s_indexEmit += 1;

            if (m_children != null) foreach (ViewerTypeNode node in m_children) Debug.Assert(node.m_indexEmit != -1);
        }
    }

    class ViewerClassNode : ViewerNode
    {
        public enum Types { Type=1, Value=2, ValueSet = 3, Object = 4, ObjectSet = 5};
        public class Line
        {
            string m_strName;
            Types m_fieldType;
            int m_flags = 0;

            public Line(string fieldName)
            {
                m_strName = fieldName;
            }

            public Types fieldType { set { m_fieldType = value;  } }
            public bool Optional
            {
                set { m_flags |= (value ? 1 : 0); }
            }

            public void Emit(MyFileStream fs)
            {
                fs.WriteI1((byte) m_fieldType);
                fs.WriteI1((byte) m_flags);
                fs.WriteString(m_strName);
            }
        }

        private string m_strName;
        private List<Line> m_Lines = new List<Line>();

        public ViewerClassNode(String strClassName)
        {
            m_strName = strClassName;
        }

        public void Add(Line newLine) { m_Lines.Add(newLine); }

        override public void Emit(MyFileStream fs)
        {
            fs.WriteString("CLASS");
            fs.WriteString(m_strName);
            fs.WriteI2(m_Lines.Count);
            foreach (Line line in m_Lines) {
                line.Emit(fs);
            }
        }
    }

    class ViewerObjectNode : ViewerNode
    {
        Symbol m_class;
        List<ViewerNode> m_lstFields = new List<ViewerNode>();

        public ViewerObjectNode(Symbol classNode)
        {
            m_class = classNode;
        }

        public void Add(ViewerNode node)
        {
            Debug.Assert(node != null);
            Debug.Assert((node.index != -1));
            m_lstFields.Add(node);
        }

        override public void Emit(MyFileStream fs)
        {
            fs.WriteString("OBJECT");
            Debug.Assert(m_class.ViewerNode.index >= 0);
            fs.WriteI4(m_class.ViewerNode.index);
            foreach (ViewerNode node in m_lstFields) {
                Debug.Assert(node.index != -1);
                fs.WriteI4(node.index);
            }
        }
    }

    class ViewerObjectSetNode : ViewerNode
    {
        Symbol m_class;
        List<ViewerObjectNode> m_lstObjects = new List<ViewerObjectNode>();

        public ViewerObjectSetNode(Symbol classNode)
        {
            m_class = classNode;
            Debug.Assert(classNode != null);
        }

        public void AddObject(ViewerObjectNode obj)
        {
            Debug.Assert(obj.index != -1);
            m_lstObjects.Add(obj);
        }

        override public void Emit(MyFileStream fs)
        {
            fs.WriteString("OBJECTSET");
            fs.WriteI4(m_class.ViewerNode.index);
            fs.WriteI4(m_lstObjects.Count);
            foreach (ViewerObjectNode obj in m_lstObjects) fs.WriteI4(obj.index);
        }
    }

    class ViewerValueTree
    {
        ViewerValueTree[] m_children = null;
        ViewerValueNode m_node;

        static public ViewerValueTree s_root = new ViewerValueTree();

        public ViewerValueNode AddToTree(ViewerValueNode nodeToAdd, int iLevel = 1)
        {
            if (iLevel == nodeToAdd.value.Length) {
                if (m_node == null) m_node = nodeToAdd;
                return m_node;
            }

            if (m_children == null) m_children = new ViewerValueTree[256];
            if (m_children[nodeToAdd.value[iLevel]] == null) m_children[nodeToAdd.value[iLevel]] = new ViewerValueTree();
            return m_children[nodeToAdd.value[iLevel]].AddToTree(nodeToAdd, iLevel + 1);
        }
    }

    class ViewerValueNode : ViewerNode
    {
        string m_strName;
        byte[] m_rgbValue;

        public ViewerValueNode(string strName, byte[] rgbValue)
        {
            m_strName = strName;
            m_rgbValue = rgbValue;
        }

        public byte[] value { get { return m_rgbValue; } }

        override public void Emit(MyFileStream fs)
        {
            fs.WriteString("VALUE");
//            fs.WriteString(m_strName);
            byte[] rgbValue = new byte[m_rgbValue.Length - 1];
            Array.Copy(m_rgbValue, 1, rgbValue, 0, m_rgbValue.Length - 1);
            fs.WriteBytes(m_rgbValue);
        }
    }

    class ViewerValueSetNode : ViewerNode
    {
        SortedList<int, ViewerValueNode> m_lstValues = new SortedList<int, ViewerValueNode>();

        public void AddValue(ViewerValueNode val)
        {
            m_lstValues.Add(val.index, val);
        }

        override public void Emit(MyFileStream fs)
        {
            fs.WriteString("VALUESET");
            fs.WriteI4(m_lstValues.Count);
            foreach (ViewerValueNode val in m_lstValues.Values) fs.WriteI4(val.index);
        }

        public bool IsSameAs(ViewerValueSetNode node2)
        {
            if (this.m_lstValues.Count != node2.m_lstValues.Count) return false;

            foreach (int i in m_lstValues.Keys) {
                if (node2.m_lstValues[i] == null) return false;
            }
            return true;
        }
    }

    class Viewer_Emitter : CodeEmitter
    {
        readonly MyFileStream m_fileOut;
        private string m_strModule;
        private List<ViewerTypeNode> m_emitList = new List<ViewerTypeNode>();
        private List<ViewerClassNode> m_classList = new List<ViewerClassNode>();
        private List<ViewerValueNode> m_valueList = new List<ViewerValueNode>();
        private List<ViewerValueSetNode> m_valueSetList = new List<ViewerValueSetNode>();
        private List<ViewerObjectNode> m_objectList = new List<ViewerObjectNode>();
        private List<ViewerObjectSetNode> m_objectSetList = new List<ViewerObjectSetNode>();
        private List<ViewerConstraintNode> m_constraintList = new List<ViewerConstraintNode>();
        int m_emitCount = 0;
        private List<Symbol> m_symbolsToView = new List<Symbol>();

        public Viewer_Emitter(string fileName, CmdLineArgs args)
        {
            if (fileName.Substring(fileName.Length - 4) != ".a2c") fileName += ".a2c";
            m_fileOut = new MyFileStream(File.Create(fileName));
            m_fileOut.WriteI2( 0); // Version #0
            EmitOIDTable();
        }

        private void AddToList( ViewerTypeNode node)
        {
            if (m_emitList.Contains(node)) Debug.Assert(false);
            node.index = m_emitCount;
            m_emitCount += 1 + node.ChildCount();
            m_emitList.Add(node);
        }

        private void AddToList(ViewerClassNode node)
        {
            if (m_classList.Contains(node)) Debug.Assert(false);
            node.index = m_classList.Count;
            m_classList.Add(node);
        }

        private ViewerValueNode AddToList(ViewerValueNode node)
        {
            node = ViewerValueTree.s_root.AddToTree(node);
            if (!m_valueList.Contains(node)) {
                node.index = m_valueList.Count;
                m_valueList.Add(node);
            }
            return node;
        }

        private ViewerValueSetNode AddToList(ViewerValueSetNode node)
        {
            foreach (ViewerValueSetNode node2 in m_valueSetList) {
                if (node2.IsSameAs(node)) return node2;
            }
            node.index = m_valueSetList.Count;
            m_valueSetList.Add(node);
            return node;
        }

        public ViewerObjectNode AddToList(ViewerObjectNode node)
        {
            if (m_objectList.Contains(node)) Debug.Assert(false);
            node.index = m_objectList.Count;
            m_objectList.Add(node);
            return node;
        }

        public void AddToList(ViewerObjectSetNode node)
        {
            if (m_objectSetList.Contains(node)) Debug.Assert(false);
            node.index = m_objectSetList.Count;
            m_objectSetList.Add(node);
        }

        private void AddToList(ViewerConstraintNode node)
        {
            if (m_constraintList.Contains(node)) Debug.Assert(false);
            node.index = m_constraintList.Count;
            m_constraintList.Add(node);
        }

        public override void Close()
        {
            //  Emit the Types Table

            m_fileOut.WriteString("TYPE-TABLE");
            m_fileOut.WriteI4(m_emitCount);
            foreach (ViewerTypeNode node in m_emitList) {
                node.Emit(m_fileOut);
            }

            //  Emit the View Symbols table

            m_fileOut.WriteString("SYMBOL-TABLE");
            m_fileOut.WriteI2(m_symbolsToView.Count);
            foreach (Symbol sym in m_symbolsToView) {
                m_fileOut.WriteI2(sym.ViewerNode.index);
                m_fileOut.WriteString(sym.name);
            }

            //  Emit the class table
            m_fileOut.WriteString("CLASS-TABLE");
            m_fileOut.WriteI2(m_classList.Count);
            foreach (ViewerClassNode node in m_classList) {
                node.Emit(m_fileOut);
            }

            //  Emit the value table
            m_fileOut.WriteString("VALUE-TABLE");
            m_fileOut.WriteI2(m_valueList.Count);
            foreach (ViewerValueNode node in m_valueList) {
                node.Emit(m_fileOut);
            }

            //  Emit the value list table
            m_fileOut.WriteString("VALUESET-TABLE");
            m_fileOut.WriteI2(m_valueSetList.Count);
            foreach (ViewerValueSetNode node in m_valueSetList) {
                node.Emit(m_fileOut);
            }

            //  Emit the object table
            m_fileOut.WriteString("OBJECT-TABLE");
            m_fileOut.WriteI2(m_objectList.Count);
            foreach (ViewerObjectNode node in m_objectList) {
                node.Emit(m_fileOut);
            }

            m_fileOut.WriteString("OBJECTSET-TABLE");
            m_fileOut.WriteI2(m_objectSetList.Count);
            foreach (ViewerObjectSetNode node in m_objectSetList) {
                node.Emit(m_fileOut);
            }

            m_fileOut.WriteString("CONSTRAINT-TABLE");
            m_fileOut.WriteI2(m_constraintList.Count);
            foreach (ViewerConstraintNode node in m_constraintList) {
                node.Emit(m_fileOut);
            }

            m_fileOut.Close();
        }

        private void EmitOIDTable()
        {
            OIDNode node = OIDNode.m_treeRoot;

            EmitOIDNode(node);
        }

        private void EmitOIDNode(OIDNode node)
        {
            byte[] rgb = BitConverter.GetBytes((Int32) node.iValue);
            Int16 i = 0;

            m_fileOut.WriteI4((Int32) node.iValue);
            foreach (OIDNode child in node.children) { if (child != null) i += 1; }
            m_fileOut.WriteI2(i);

            m_fileOut.WriteString(node.name);
            if (node.symbol != null) m_fileOut.WriteString(node.symbol.name);
            else m_fileOut.WriteString(null);

            foreach (OIDNode child in node.children) {
                if (child != null) EmitOIDNode(child);
            }
        }



        ViewerClassNode.Line EmitObjectClassLine(Symbol sym)
        {
            ViewerClassNode.Line node = new ViewerClassNode.Line(sym.name);

            switch (sym.symType) {
            case SymbolType.TypeField:
                node.fieldType = ViewerClassNode.Types.Type;
                break;

            case SymbolType.ValueField:
                node.fieldType = ViewerClassNode.Types.Value;
                break;

            case SymbolType.ValueSetField:
                node.fieldType = ViewerClassNode.Types.ValueSet;
                break;

            case SymbolType.ObjectField:
                node.fieldType = ViewerClassNode.Types.Object;
                break;

            case SymbolType.ObjectSetField:
                node.fieldType = ViewerClassNode.Types.ObjectSet;
                break;

            default:
                break;
            }
            if (sym.flags.Optional) node.Optional = true;
            return node;
        }

        void EmitObjectClass(Symbol sym)
        {
            ViewerClassNode node = new ViewerClassNode(sym.name);
            Type classType = sym.type.SkipFluff();
            Debug.Assert(classType.baseType == BaseType.Class);

            foreach (Symbol field in classType.fieldList) {
                node.Add(EmitObjectClassLine(field));
            }

            AddToList(node);
            sym.ViewerNode = node;
        }

        ViewerClassNode EmitObjectClass(Type classType)
        {
            ViewerClassNode node = new ViewerClassNode("");
            classType = classType.SkipFluff();
            Debug.Assert(classType.baseType == BaseType.Class);

            foreach (Symbol field in classType.fieldList) {
                node.Add(EmitObjectClassLine(field));
            }

            AddToList(node);
            return node;
        }

        private ViewerObjectNode EmitObject(Value val)
        {
            ViewerObjectNode node;

            switch (val.valueType) {
            case ValueType.Symbol:
                return EmitObject(val.sym.value);

            case ValueType.SymbolList:
                node = new ViewerObjectNode(val.type.owner);
                foreach (Symbol sym in val.symList) {
                    node.Add(ViewerNodeFromSymbol(sym));
                }
                return node;

            default:
                Debug.Assert(false);
                break;
            }
            return null;
        }

        void EmitObject(Symbol sym)
        {

        }

        private ViewerNode ViewerNodeFromSymbol(Symbol sym)
        {
            switch (sym.symType) {
            case SymbolType.Object:
                ViewerObjectNode objNode =  EmitObject(sym.value);
                objNode = AddToList(objNode);
                return objNode;

            case SymbolType.Type:
                if (sym.ViewerNode == null) {
                    ViewerTypeNode typNode = EmitType(sym.name, sym.type, null);
                    sym.ViewerNode = typNode;
                    AddToList(typNode);
                    return typNode;
                }
                return sym.ViewerNode;

            case SymbolType.Value:
                ViewerValueNode valNode = EmitValue(sym.value);
                valNode = AddToList(valNode);
                return valNode;

            case SymbolType.Dummy:
                return new ViewerDummyNode();

            case SymbolType.ObjectSet:
                ViewerObjectSetNode objsetNode = EmitObjectSet(sym.value);
                AddToList(objsetNode);
                return objsetNode;

            default:
                Debug.Assert(false);
                break;
            }

            return null;
        }
 
        public override void EmitSymbol(Symbol sym)
        {
            //  We ignore the -emit flag - so ignore the flag in the module that says we are not emitting

            switch (sym.GetSymbolType()) {
            case SymbolType.Module:
                //  Save the string for later use
                m_strModule = sym.name;
                break;

            case SymbolType.ObjectClass:
                EmitObjectClass(sym);
                break;

            case SymbolType.Object:
                // EmitObject(sym);
                break;

            case SymbolType.Type:
                EmitType(sym, sym.type);

                if (sym.flags.View) m_symbolsToView.Add(sym);
                break;

            case SymbolType.Value:
                // EmitValue(sym);
                break;
            }
        }

        public void EmitType(Symbol symRoot, Type typ)
        {
            Debug.Assert(typ.baseType == BaseType.Assignment);
            if (symRoot.ViewerNode != null) return;

            ViewerTypeNode node = EmitType(symRoot.name, typ.subType, null);
            AddToList(node);

            symRoot.ViewerNode = node;
        }

        public ViewerTypeNode EmitType(string symName, Type typ, List<Tagging> tagsIn)
        {
            ViewerTypeNode node = new ViewerTypeNode();
            ViewerTypeNode node2;
            ViewerTypeNode node3;

            switch (typ.baseType) {
            case BaseType.Assignment:
                if (typ.owner.m_symTable != null) {
                    node.fieldName = symName;
                    node.asnType = asnTypes.SymRef;
                    node.symbol = typ.owner;
                }
                else {
                    node = EmitType(symName, typ.subType, tagsIn);
                }
                break;

            case BaseType.Choice:
                node3 = new ViewerTypeNode();
                node3.asnType = asnTypes.List;

                foreach (Symbol sym1 in typ.fieldList) {
                    if (sym1.symType == SymbolType.Extension) {
                        node2 = new ViewerTypeNode();
                        node2.asnType = asnTypes.Extension;
                        node3.children.Add(node2);
                    }
                    else if (sym1.symType == SymbolType.ExtensionEnd) {
                        node2 = new ViewerTypeNode();
                        node2.asnType = asnTypes.ExtensionEnd;
                        node3.children.Add(node2);
                    }
                    else if (sym1.symType == SymbolType.ExtensionGroup) {
                        foreach (Symbol sym2 in sym1.symList) {
                            node2 = EmitType(sym2.name, sym2.type, null);
                            node3.children.Add(node2);
                        }
                    }
                    else {
                        node2 = EmitType(sym1.name, sym1.type, null);
                        node3.children.Add(node2);
                        if (sym1.flags.Optional) node2.Optional = true;
                    }
                }

                AddToList(node3);
                node.children.Add(node3);

                node.fieldName = symName;
                node.asnType = asnTypes.Choice;
                break;

            case BaseType.Constraint:
                node = EmitType(symName, typ.subType, tagsIn);
                foreach (Constraint ct in typ.constraintList) {
                    ViewerValueSetNode valList = null;
                    ViewerObjectSetNode objList = null;

                    switch (ct.ct) {
                    case ConstraintType.Table: {
                            ViewerTableConstraintNode cNode;
                            cNode = new ViewerTableConstraintNode(ct);

                            AddToList(cNode);
                            node.constraint = cNode;

                            if (typ.subType.baseType == BaseType.ObjectClassField) {
                                //  Figure out the base class

                                Type classType = typ.subType.classType;
                                if (classType.owner != null) {
                                    cNode.objectClass = classType.owner;
                                    classType = classType.owner.type.subType;
                                }
                                else {
                                    //  Need to figure out this sooner or later to set an object class on the node.

                        // M00BUG            cNode.objectClass =  EmitObjectClass(typ.subType);
                                }

                                Debug.Assert(typ.subType.fieldTokens.Count == 1);
                                for (int iField = 0; iField < classType.fieldList.Count; iField++) {
                                    if (typ.subType.fieldTokens[0].name == classType.fieldList[iField].name) {
                                        cNode.iField = iField;
                                        break;
                                    }
                                }

                                objList = new ViewerObjectSetNode(cNode.objectClass);
                            }
                            else {
                                valList = new ViewerValueSetNode();
                            }

                            //  Jump through hoops first

                            bool closed = false;
                            BuildObjectList(ct.value, ref closed, ref objList, ref valList);
                            cNode.Closed = closed;


                            if (objList != null) {
                                AddToList(objList);
                                cNode.ObjectList = objList;
                            }
                            else {
                                valList = AddToList(valList);
                                cNode.ValueList = valList;
                            }
                        }
                        break;

                    case ConstraintType.Relation: {
                            ViewerRelationConstraintNode cNode = new ViewerRelationConstraintNode(ct);

                            AddToList(cNode);
                            node.constraint = cNode;

                            if (typ.subType.baseType == BaseType.ObjectClassField) {
                                Type classType = typ.subType.classType;
                                if (classType.owner != null) {
                                    cNode.objectClass = classType.owner;
                                    classType = classType.owner.type.subType;
                                }
                                else {
                                    cNode.objectNode = EmitObjectClass(classType);
                                }

                                cNode.objectClass = typ.subType.classType.owner;

                                for (int iField = 0; iField < classType.fieldList.Count; iField++) {
                                    if (ct.text == classType.fieldList[iField].name) {
                                        cNode.target = iField;
                                        break;
                                    }
                                }

                                foreach (Symbol symField in ct.symlst) {
                                    for (int iField = 0; iField < classType.fieldList.Count; iField++) {
                                        cNode.AddField(iField);
                                        break;
                                    }
                                }

                                objList = new ViewerObjectSetNode(cNode.objectClass);
                            }
                            else {
                                Debug.Assert(false);
                            }

                            bool closed = false;
                            BuildObjectList(ct.value, ref closed, ref objList, ref valList);
                            cNode.Closed = closed;

                            if (objList != null) {
                                AddToList(objList);
                                cNode.ObjectList = objList;
                            }
                            else {
                                valList = AddToList(valList);
                                cNode.ValueList = valList;
                            }
                        }
                        break;

                    case ConstraintType.Contents:
                    case ConstraintType.Range:
                    case ConstraintType.Size:
                    case ConstraintType.Union:
                    case ConstraintType.Value:
                        break;

                    default:
                        Debug.Assert(false);
                        break;
                    }
                }
                break;

            case BaseType.Leaf:
                node.fieldName = symName;
                node.tag = typ.tagged;
                if (typ.tagged.tagClass == TagClass.Universal) {
                    node.asnType = (asnTypes) typ.tagged.value;
                }
                else {
                    node.asnType = (asnTypes) typ.dataType;
                }
                break;

            case BaseType.NamedList:
                node = EmitType(symName, typ.subType, tagsIn);
                break;

            case BaseType.ObjectClassField:
                Symbol symT = typ.GetObjectClassField();
                switch (symT.symType) {
                case SymbolType.ValueField:
                    node = EmitType(symName, symT.type, tagsIn);
                    break;

                case SymbolType.TypeField:
                    node.fieldName = symName;
                    node.asnType = asnTypes.Any;
                    break;

                default:
                    Debug.Assert(false);
                    break;
                }
                break;

            case BaseType.OpenType:
                node.fieldName = symName;
                node.asnType = asnTypes.Any;
                break;

            case BaseType.Param_Assignment:
                node = EmitType(symName, typ.subType, tagsIn);
                break;

            case BaseType.Sequence:
                node3 = new ViewerTypeNode();
                node3.asnType = asnTypes.List;

                foreach (Symbol sym1 in typ.fieldList) {
                    if (sym1.symType == SymbolType.Extension) {
                        node2 = new ViewerTypeNode();
                        node2.asnType = asnTypes.Extension;
                        node3.children.Add(node2);
                    }
                    else if (sym1.symType == SymbolType.ExtensionEnd) {
                        node2 = new ViewerTypeNode();
                        node2.asnType = asnTypes.ExtensionEnd;
                        node3.children.Add(node2);
                    }
                    else if (sym1.symType == SymbolType.ExtensionGroup) {
                        foreach (Symbol sym2 in sym1.symList) {
                            node2 = EmitType(sym2.name, sym2.type, null);
                            node3.children.Add(node2);
                            if (sym2.flags.Optional) node2.Optional = true;
                        }
                    }
                    else {

                        node2 = EmitType(sym1.name, sym1.type, null);
                        node3.children.Add(node2);
                        if (sym1.flags.Optional) node2.Optional = true;
                    }
                }

                AddToList(node3);
                node.children.Add(node3);

                node.fieldName = symName;
                node.tag = new Tagging(TagClass.Universal, TagType.Specified, 16);
                node.asnType = asnTypes.Sequence;
                break;

            case BaseType.SequenceOf:
                node2 = EmitType(null, typ.fieldList[0].type, null);
                AddToList(node2);
                node.children.Add(node2);
                node.fieldName = symName;
                node.asnType = asnTypes.SeqOf;
                node.tag = new Tagging(TagClass.Universal, TagType.Specified, 16);
                break;

            case BaseType.Set:
                node3 = new ViewerTypeNode();
                node3.asnType = asnTypes.List;

                foreach (Symbol sym1 in typ.fieldList) {
                    if (sym1.symType == SymbolType.Extension) {
                        node2 = new ViewerTypeNode();
                        node2.asnType = asnTypes.Extension;
                        node3.children.Add(node2);
                    }
                    else if (sym1.symType == SymbolType.ExtensionEnd) {
                        node2 = new ViewerTypeNode();
                        node2.asnType = asnTypes.ExtensionEnd;
                        node3.children.Add(node2);
                    }
                    else if (sym1.symType == SymbolType.ExtensionGroup) {
                        foreach (Symbol sym2 in sym1.symList) {

                            node2 = EmitType(sym2.name, sym2.type, null);
                            node3.children.Add(node2);
                        }
                    }
                    else {

                        node2 = EmitType(sym1.name, sym1.type, null);
                        node3.children.Add(node2);
                        if (sym1.flags.Optional) node2.Optional = true;
                    }
                }
                AddToList(node3);
                node.children.Add(node3);

                node.fieldName = symName;
                node.tag = new Tagging(TagClass.Universal, TagType.Specified, 17);
                node.asnType = asnTypes.Set;
                break;

            case BaseType.SetOf:
                node2 = EmitType(null, typ.fieldList[0].type, null);
                AddToList(node2);

                node.children.Add(node2);
                node.fieldName = symName;
                node.tag = new Tagging(TagClass.Universal, TagType.Specified, 17);
                node.asnType = asnTypes.SetOf;
                break;

            case BaseType.Symbol:
                if (typ.sym.m_symTable != null) {
                    node.fieldName = symName;
                    node.asnType = asnTypes.SymRef;
                    node.symbol = typ.sym;
                }
                else {
                    node = EmitType(symName, typ.sym.type, null);
                }
                break;

            case BaseType.Tagged:
                if (typ.tagged.tagType == TagType.Explicit) {
                    node2 = EmitType(null, typ.subType, tagsIn);
                    AddToList(node2);
                    node.children.Add(node2);
                    node.asnType = asnTypes.Tag;
                    node.tag = typ.tagged;
                    node.fieldName = symName;
                }
                else {
                    node2 = EmitType(symName, typ.subType, tagsIn);
                    AddToList(node2);
                    node.tag = typ.tagged;
                    node.asnType = asnTypes.ITag;
                    node.children.Add(node2);
                    node.fieldName = symName;
                }
                break;

            default:
                Debug.Assert(false, "Unknown basetype " + typ.baseType);
                break;
            }

            return node;
        }


        void BuildObjectList(Value valueX, ref bool closed, ref ViewerObjectSetNode objList, ref ViewerValueSetNode valList)
        {
            bool fDone = false;
            while (!fDone) {
                switch (valueX.valueType) {
                case ValueType.Symbol:
                    valueX = valueX.sym.value;
                    break;

                case ValueType.SetOf:
                    fDone = true;
                    break;

                default:
                    Debug.Assert(false);
                    fDone = true;
                    break;
                }
            }

            switch (valueX.valueType) {
            case ValueType.SetOf:
                foreach (Value valItem in valueX.valueList) {
                    switch (valItem.valueType) {
                    case ValueType.Extension:
                        closed = false;
                        break;

                    case ValueType.Symbol:
                        if (objList != null) {
                            ViewerObjectNode objNode = EmitObject(valItem);
                            if (objNode != null) {
                                objNode = AddToList(objNode);
                                objList.AddObject(objNode);
                            }
                        }
                        else {
                            ViewerValueNode valNode = EmitValue(valItem);
                            if (valNode != null) {
                                valNode = AddToList(valNode);
                                valList.AddValue(valNode);
                            }
                        }
                        break;

                    default:
                        Debug.Assert(false);
                        break;
                    }
                }
                break; 

            default:
                Debug.Assert(false);
                break;
            }
        }

        private ViewerValueNode EmitValue(Value val)
        {
            switch (val.valueType) {
            case ValueType.Symbol:
                return EmitValue(val.sym.value);

            default:
                Debug.Assert(false);
                break;

            case ValueType.Boolean:
            case ValueType.Binary:
            case ValueType.Integer:
            case ValueType.List:
                break;

            case ValueType.SetOf:
                break;
            }

            byte[] rgbValue = val.Encode();
            if (rgbValue.Length == 0) return null;

            ViewerValueNode node = new ViewerValueNode(null, rgbValue);
            node = AddToList(node);
            return node;
        }

        public void EmitValue(Symbol sym)
        {
            byte[] rgbValue = sym.value.Encode();

            if (rgbValue.Length == 0) return;

            ViewerValueNode node = new ViewerValueNode(sym.name, rgbValue);

            sym.ViewerNode = node;
        }

        private ViewerObjectSetNode EmitObjectSet(Value val)
        {
            switch (val.valueType) {
            case ValueType.Symbol:
                return EmitObjectSet(val.sym.value);

            case ValueType.SetOf:
                Debug.Assert(val.valueList.Count > 0);
                
                ViewerObjectSetNode node = new ViewerObjectSetNode(val.type.owner);
                
                foreach (Value val2 in val.valueList) {
                    ViewerObjectNode objNode = EmitObject(val2);
                    AddToList(objNode);
                    node.AddObject(objNode);
                }
                return node;

            default:
                Debug.Assert(false);
                break;
            }
            return null;
        }

        public override void PreEmitSymbol(Symbol sym)
        {
        }
    }
}
