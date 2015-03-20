using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace asn_compile_cs
{

    enum ValueType { None, Integer, Boolean, List, Symbol, Double, Binary, SymbolList, 
        Null, Extension, PlusInfinity, MinusInfinity, NotANumber, OIDNode, String, SetOf,

        ValueFromObject, ValueSetFromObject, ObjectFromObject, ObjectSetFromObject,
        ValueSetFromObjects, ObjectSetFromObjects, ExtractFromObject, ExtractFromObjectSet
    };
 
    class Value
    {
        public static Value ValueNull = new Value(ValueType.Null);


        ValueType m_valType;

        long m_iValue;
        bool m_b;
        Binary m_bin;
        double m_dValue;
        Type m_type;
        String m_name;
        OIDNode m_oidNode;
        Symbol m_sym;
        SymbolList m_symList;
        TokenList m_tknlst;
        ValueList m_valList;

        Value()
        {
            m_valType = ValueType.None;
        }

        public Value(ValueType vt)
        {
            m_valType = vt;
            if (vt == ValueType.Null) m_type = Type.Null;
            else {
                Debug.Assert (vt == ValueType.Extension);
            }
        }


        public Value(long iVal, Type typ)
        {
            m_valType = ValueType.Integer;
            m_iValue = iVal;
            m_type = typ;
        }

        public Value(double val, Type typ)
        {
            m_valType = ValueType.Double;
            m_dValue = val;
            m_type = typ;
        }

        public Value(Binary bin)
        {
            m_valType = ValueType.Binary;
            m_bin = bin;
        }

        public Value(bool f)
        {
            m_valType = ValueType.Boolean;
            m_b = f;
            m_type = Type.BooleanType;
        }

        public Value(ValueList lst)
        {
            m_valType = ValueType.List;

            m_valList = lst;
            if (lst.type != null) {
                m_type = lst.type;
            }
        }

        public Value(String str)
        {
            m_valType = ValueType.String;
            m_name = str;
        }

        public Value(Symbol sym)
        {
            m_valType = ValueType.Symbol;

            Debug.Assert(sym.IsType(SymbolType.Object) || sym.IsType(SymbolType.Value) || sym.IsType(SymbolType.ObjectSet) ||
                         sym.IsType(SymbolType.ObjectParameter) || sym.IsType(SymbolType.ValueParameter) || sym.IsType(SymbolType.ObjectSetParameter) ||
                         sym.IsType(SymbolType.Value | SymbolType.InList));
            m_sym = sym;
            m_type = sym.type;
        }

        public Value(SymbolList symList)
        {
            m_valType = ValueType.SymbolList;
            m_symList = symList;
        }

        public Value(ValueList valList, ValueType vt)
        {
            m_valType = vt;
            m_valList = valList;
        }

        public Value(ValueType vt, Value val, Type type, TokenList tknlst)
        {
            Debug.Assert(vt == ValueType.ExtractFromObject || vt == ValueType.ExtractFromObjectSet);

            m_valType = vt;
            m_valList = new ValueList(val);
            m_type = type;
            m_tknlst = tknlst;
        }

        public Value(OIDNode oidNode)
        {
            m_oidNode = oidNode;
            m_valType = ValueType.OIDNode;
            m_type = Type.IntegerType;
        }

        public Binary bin { get { Debug.Assert(m_valType == ValueType.Binary); return m_bin; } }

        public Boolean bVal
        {
            get { /*Debug.Assert(m_valType == ValueType.Boolean); M00BUG - this is a real assert*/return m_b; }
        }

        public long iVal
        {
            get { Debug.Assert(m_valType == ValueType.Integer);  return m_iValue; }
        }

        public OIDNode oidNode
        {
            get { Debug.Assert(m_valType == ValueType.OIDNode); return m_oidNode; }
        }   

        public String name
        {
            get { return m_name; }
            set { m_name = value; }
        }

        public Symbol sym
        {
            get { Debug.Assert(m_valType == ValueType.Symbol);  return m_sym; }
            set { m_sym = value; m_valType = ValueType.Symbol; }
        }

        public SymbolList symList
        {
            get { return m_symList; }
        }

        public TokenList tknlst { get { return m_tknlst; } }

        public Type type
        {
            get { return m_type; }
            set { m_type = value; }
        }

        public ValueList valueList
        {
            get { return m_valList; }
            set { m_valList = value; }
        }

        public ValueType valueType
        {
            get { return m_valType; }
        }

        public void ChangeTo(Value val)
        {
            m_valType = val.m_valType;
            m_iValue = val.m_iValue;
            m_b = val.m_b;
            m_bin = val.m_bin;
            m_dValue = val.m_dValue;
            m_type = val.m_type;
            m_name = val.m_name;
            m_sym = val.m_sym;
            m_symList = val.m_symList;
            m_tknlst = val.m_tknlst;
            m_valList = val.m_valList;
        }

        /// <summary>
        /// Collapse removes indirections from the system.
        /// </summary>
        public void Collapse()
        {
            switch (m_valType) {
            case ValueType.Symbol:
                m_sym.Collapse();
                if (m_sym.GetSymbolType() == SymbolType.ObjectSetParameter) {
                    break;
                }

                m_valType = m_sym.value.m_valType;
                switch (m_valType) {
                case ValueType.Boolean:
                    m_b = m_sym.value.bVal;
                    break;

                case ValueType.Integer:
                    m_iValue = m_sym.value.iVal;
                    break;

                case ValueType.List:
                    m_valList = m_sym.value.m_valList;
                    foreach (Value val in m_valList) {
                        val.Collapse();
                    }
                    break;

                }
                m_sym = null;
                break;

            case ValueType.List:
                foreach (Value val in m_valList) {
                    val.Collapse();
                }
                break;

            case ValueType.SymbolList:
                foreach (Symbol sym in m_symList) sym.Collapse();
                break;
            }
        }

        /// <summary>
        /// Dump displays the structure on the Console
        /// </summary>
        /// <param name="tab">Indentation level for the dump</param>
        public void Dump(String tab)
        {
            switch (m_valType) {
            case ValueType.Binary:
                Console.Error.WriteLine(tab + m_bin.ToString());
                break;

            case ValueType.Boolean:
                Console.Error.WriteLine(tab + m_b.ToString());
                break;

            case ValueType.Integer:
                Console.Error.WriteLine(tab + m_iValue.ToString());
                break;

            case ValueType.List:
                Console.Error.WriteLine(tab + "<");
                foreach (Value val in m_valList) {
                    val.Dump(tab + "    ");
                }
                Console.Error.WriteLine(tab + ">");
                break;

            case ValueType.SymbolList:
                Console.Error.WriteLine(tab + "<");
                m_symList.Dump(tab + "    ");
                Console.Error.WriteLine(tab + ">");
                break;

            case ValueType.Symbol:
                Console.Error.WriteLine(tab + m_sym.name);
                break;

            default:
                Console.Error.WriteLine(tab + m_valType.ToString());
                break;
            }
        }

        public byte[] Encode()
        {
            Type type = m_type.SkipFluff();

            if (m_valType == ValueType.Symbol) {
                return m_sym.value.Encode();
            }
            else if (type.baseType == BaseType.ObjectClassField) {
                Type classType = type.classType;
                if (classType.owner != null) {
                    classType = classType.owner.type.subType;
                }
                else {
                    Debug.Assert(false);
                    //  Need to figure out this sooner or later to set an object class on the node.
                }


                foreach (Symbol field in classType.fieldList) {
                    if (field.name == type.fieldTokens[0].name) {
                        type = field.type;
                        break;
                    }
                }
            }

            switch (type.baseType) {
            case BaseType.Leaf:
                switch (type.dataType) {

                case DataType.BitString:
                    return new byte[1];

                case DataType.ObjectIdentifier:
                    return EncodeObjectIdentifier();

                case DataType.Integer:
                    return EncodeInteger();

                case DataType.OctetString:
                    break;

                case DataType.Boolean:
                    return new byte[3] { 1, 1, 0 };

                case DataType.Null:
                    return new byte[2] { 5, 0 };

                default:
                    Debug.Assert(false);
                    break;
                }
                break;

            case BaseType.ObjectClassField: {
                Debug.Assert(false);
                    Type classType = type.classType;
                    if (classType.owner != null) {
                        classType = classType.owner.type.subType;
                    }
                    else {
                        Debug.Assert(false);
                        //  Need to figure out this sooner or later to set an object class on the node.
                    }


                    foreach (Symbol field in classType.fieldList) {
                        if (field.name == type.fieldTokens[0].name) {
                            type = field.type;
                            break;
                        }
                    }
                }
                break;

            case BaseType.Sequence:
                break;

            default:
                Debug.Assert(false);
                break;
            }
            return new byte[0];
        }

        private byte[] EncodeInteger()
        {
            byte[] rgb = null;

            switch (valueType) {
            case ValueType.Integer:
                rgb = BitConverter.GetBytes((Int64) m_iValue);        // Will throw an exception of m_i64Value is not set.
                if (BitConverter.IsLittleEndian) Array.Reverse(rgb);
                break;

            default:
                Debug.Assert(false);
                return new byte[0];
            }

            int cbData = rgb.Length;
            int ibStart = 0;
            if (rgb[0] == 0) {
                for (ibStart = 0; ibStart < rgb.Length - 1; ibStart++) if (rgb[ibStart] != 0 || rgb[ibStart+1] >= 0x80) break;
                }
            else if (rgb[0] == 0xff) {
                for (ibStart = 0; ibStart < rgb.Length - 1; ibStart++) if (rgb[ibStart] != 0xff || rgb[ibStart+1] < 0x80) break;
            }

            byte[] rgb2 = new byte[1 + 1 + (cbData - ibStart)];
            rgb2[0] = 4;
            Debug.Assert(rgb.Length <= 0x7f);
            rgb2[1] = (byte) (cbData - ibStart);
            Array.Copy(rgb, ibStart, rgb2, 1 + 1, (cbData - ibStart));
            return rgb2;
        }

        private byte[] EncodeObjectIdentifier()
        {
            List<byte> rgbOut = new List<byte>();

            rgbOut.Add(6);

            int i;
            int j;

            long[] rgInts = new long[valueList.Count];
            for (i = 0; i < valueList.Count; i++) {
                rgInts[i] = valueList[i].oidNode.iValue;
            }

            if (rgInts.Length == 1) throw new Exception("Invalid Object Identifier value");

            rgInts[1] = rgInts[0] * 40 + rgInts[1];

            byte[] rgb = new byte[6];
            List<byte> rgb2 = new List<byte>();

            for (i = 1; i < rgInts.Length; i++) {
                for (j = 5; (j >= 0) && (rgInts[i] != 0); j--) {
                    rgb[j] = (byte) (rgInts[i] & 0x7f);
                    rgInts[i] >>= 7;
                    if (j != 5) rgb[j] |= 0x80;
                }

                if (rgInts[i] != 0) throw new Exception("Invalid OID value");

                for (j = j + 1; j < 6; j++) rgb2.Add(rgb[j]);
            }

            if (rgb2.Count > 0x7f) throw new Exception("Too Long");
            rgbOut.Add((byte) rgb2.Count);
            rgbOut.AddRange(rgb2);

            return rgbOut.ToArray();
        }

        public override bool Equals(object obj)
        {
            // If parameter is null return false.
            if (obj == null) {
                return false;
            }

            // If parameter cannot be cast to Point return false.
            Value val = obj as Value;
            if ((System.Object)val == null) {
                return false;
            }

            //
            //  If the addresses match - good
            //

            if (this == val) return true;

            // Return true if the fields match:
            if (m_valType != val.m_valType) return false;

            switch (m_valType) {
            case ValueType.Integer:
                return m_iValue == val.m_iValue;

            case ValueType.SymbolList:
                if (m_symList.Count != val.m_symList.Count) return false;
                for (int i = 0; i < m_symList.Count; i++) {
                    if (!m_symList[i].Equals(val.m_symList[i])) return false;
                }
                return true;

            case ValueType.List:
                if (m_valList.Count != val.m_valList.Count) return false;
                for (int i = 0; i < m_valList.Count; i++) {
                    if (!m_valList[i].Equals(val.m_valList[i])) return false;
                }
                return true;

            case ValueType.SetOf:
                Debug.Assert(false, "NYI: Value.Equals.SetOf");
                return false;

            case ValueType.Symbol:
                return m_sym == val.m_sym;

            case ValueType.OIDNode:
                return m_oidNode.iValue == val.m_oidNode.iValue;
            }

            Debug.Assert(false, "NYI: Value.Equals");
            return false;
        }

        /// <summary>
        /// Creates a new value object after replacing items from the parameter list
        /// </summary>
        /// <param name="paramList">list of parameters to be replaced</param>
        /// <returns>new value </returns>
        public Value Instance(SymbolList paramList)
        {
            //
            //  This is pointing to a symbol object.  Depending on what that symbol
            //  is we either need to clone the value on it or to replace it with a 
            //  parameter value.
            //

            if (m_sym != null) {
                switch (m_sym.GetSymbolType()) {
                case SymbolType.ValueParameter:
                    foreach (Symbol sym in paramList) {
                        if (sym.name == m_sym.name) {
                            return sym.value;
                        }
                    }
                    break;

                case SymbolType.ObjectSetParameter:
                    foreach (Symbol sym in paramList) {
                        if (sym.name == m_sym.name) {
                            return sym.value;
                        }
                    }
                    break;

                case SymbolType.ObjectParameter:
                    foreach (Symbol sym in paramList) {
                        if (sym.name == m_sym.name) {
                            return sym.value;
                        }
                    }
                    break;

                default:
                    // Collapse();
                    Debug.Assert(m_sym != null);
                    break;
                }
            }

            Value newVal = new Value();

            newVal.m_valType = m_valType;
            newVal.m_bin = m_bin;
            newVal.m_b = m_b;
            newVal.m_dValue = m_dValue;
            newVal.m_iValue = m_iValue;
            newVal.m_sym = m_sym;
            // M00QUEST - What about m_symList?
            
            //
            //  Deal with complex items
            //

            if (m_type != null) {
                newVal.m_type = m_type.Instance(paramList);
            }
            if (m_valList != null) {
                newVal.m_valList = m_valList.Instance(paramList);
            }
            return newVal;
        }

        /// <summary>
        /// ToString textizes the object
        /// </summary>
        /// <returns>text version of the string.</returns>
        public override string ToString()
        {
            return m_iValue.ToString();
        }

        public MatchResult PerformOIDChecks(Token tkn, out ErrorList errs)
        {
            ErrorList errLst = new ErrorList();
            OIDNode oidTree = OIDNode.m_treeRoot;
            MatchResult mr = MatchResult.Success;

            //
            //  Perform the list of changes necessary for an OID verification
            //

            //
            //  We are going to index walk the OID from the root down to the leaf
            //  performing checks on each node as we go down the list.  This might also
            //  change the items in the list -- this means that doing it by enumerating 
            //  the list won't work because changing the list in the middle of an enumeration
            //  is a no-no.
            //

            for (int i = 0; i < m_valList.Count; i++) {
                Value val = m_valList[i];
                long iVal;
                string szName = null;
                OIDNode oidItem;

                if (val.valueType == ValueType.Symbol) {
                    //
                    //  Pointers to symbols need to be transversed to get a real value.
                    //

                    while (val.sym.value.valueType == ValueType.Symbol) {
                        val = val.sym.value;
                    }

                    //
                    //  The legal symbols that can show here are:
                    //  Position #1 - Either an OID or an integer
                    //  Position #2 - an integer
                    //  Position #3+ - Either a relative OID or an integer

                    if (val.type.IsInteger()) {
                        //
                        //  An integer has a value, but no name
                        //

                        oidItem = new OIDNode(val.sym.value.iVal);
                        m_valList[i] = new Value(oidItem);
                    }
                    else if (val.type.IsOID()) {
                        if (i != 0) {
                            //
                            //  Log the error and create a fake value for processing
                            //
                            errLst.Add(new Error(ErrorNumber.OIDFirstOnlyOID, tkn, val.name));
                            oidItem = new OIDNode(0);
                        }
                        else {
                            //
                            //  Substitute in the OID
                            //

                            m_valList.Remove(m_valList[0]);
                            for (int i2 = 0; i2 < val.sym.value.valueList.Count; i2++) {
                                m_valList.Insert(i2, val.sym.value.valueList[i2]);
                            }
                            i -= 1;
                            continue;
                        }
                    }
                    else {
                        //
                        //  Not a legal type of symbol
                        //  Log an error and create a fake value for processing
                        //

                        if (i == 0) {
                            errLst.Add(new Error(ErrorNumber.OIDStartDefinedValue, tkn, val.sym.name));
                        }
                        else if (i == 1) {
                            errLst.Add(new Error(ErrorNumber.OIDSecondDefinedValue, tkn, val.sym.name));
                        }
                        else {
                            errLst.Add(new Error(ErrorNumber.OIDThirdDefinedValue, tkn, val.sym.name));
                        }
                        oidItem = new OIDNode(0);
                    }
                }
                else {
                    Debug.Assert(val.valueType == ValueType.OIDNode);
                    oidItem = val.oidNode;
                }

                //
                //  We need to get an integer value and possibly a name for this
                //  point in the OID arc.  If we are given a value - that is the
                //  easy case.
                //

                if (oidItem.HasValue) {
                    iVal = oidItem.iValue;
                    if (oidItem.HasName) szName = oidItem.name;
                }
                else {
                    Debug.Assert(oidItem.HasName);
                    iVal = -1;
                    foreach (OIDNode oid2 in oidTree.children) {
                        if ((oid2 != null) && (oid2.HasName && oid2.name == oidItem.name)) {
                            iVal = oid2.iValue;
                            oidItem.iValue = iVal;
                            break;
                        }
                    }
                    if (iVal == -1) {
                        errLst.Add(new Error(ErrorNumber.OIDNameFormMissing, tkn, oidItem.name));
                        errs = errLst;
                        return MatchResult.Fail;
                    }
                }

                if (i == 0) {
                    //
                    //  First number must be no greater than 5
                    //      (X*40)+Y
                    //

                    if (iVal > 5) {
                        //  M00BUG - X.690-0207 Seciont 8.19.4 implies that ths is wrong and the value should be 3.
                        //  it also implies that the encoding functions in the runtime are wrong.
                        errLst.Add(new Error(ErrorNumber.OIDVal1TooLarge, tkn));
                    }
                }
                else if (i == 1) {
                    //
                    //  Second number must not exceed 39
                    //

                    if (iVal > 39) {
                        errLst.Add(new Error(ErrorNumber.OIDVal2TooLarge, tkn));
                    }
                }

                if (iVal < 0) {
                    errLst.Add(new Error(ErrorNumber.OIDNumberNonNegative, tkn));
                }

                //
                //  Check to see if this node is already in the tree
                //

                OIDNode oidLeaf = oidTree[(int) iVal];

                if (oidLeaf == null) {
                    if (szName != null) {
                        OIDNode oidByName = oidTree.FindByName(szName);
                        if (oidByName != null) {
                            Error err = new Error(ErrorNumber.OIDDuplicateName, tkn);
                            err.AddObject(szName);
                            err.AddObject(oidByName.iValue);
                            err.AddObject(iVal);
                            errLst.Add(err);
                        }
                    }
                    oidTree.Add(new OIDNode((int) iVal, szName));
                    oidTree = oidTree[(int) iVal];
                }
                else if (szName != null) {
                    if (oidLeaf.name == null) {
                        oidLeaf.name = szName;
                    }
                    else {
                        //
                        //  Check that the name matches
                        //

                        if ((szName != oidLeaf.name) && (szName != oidLeaf.AltName)) {
                            Error err = new Error(ErrorNumber.OIDWrongName, tkn);
                            err.AddObject(szName);
                            err.AddObject(oidLeaf.name);
                            errLst.Add(err);
                        }
                    }
                    oidTree = oidLeaf;
                }
                else {
                    oidTree = oidLeaf;
                }

                m_valList[i].m_oidNode = oidTree;
            }

            if (m_valList.Count < 2) {
                errLst.Add(new Error(ErrorNumber.OIDLength, tkn));
            }

            errs = errLst;
            return mr;
        }
    }

    class ValueList : IEnumerable<Value>
    {
        List<Value> m_lst;
        Type m_type;

        public ValueList()
        {
            m_lst = new List<Value>();
        }

        public ValueList(Value val)
        {
            m_lst = new List<Value>();
            m_lst.Add(val);
        }

        public ValueList(List<Value> lst)
        {
            m_lst = lst;
        }

        public int Count { get { return m_lst.Count; } }

        public Type type
        {
            get { return m_type; }
            set { m_type = value; }
        }

        public Value this[int i]
        {
            get { return m_lst[i]; }
            set { m_lst[i] = value; }
        }


        public void Add(Value val)
        {
            m_lst.Add(val);
        }

        public void Add(ValueList lst)
        {
            m_lst.AddRange(lst.m_lst);
        }

        public void Insert(int i, Value val)
        {
            m_lst.Insert(i, val);
        }

        /// <summary>
        /// Creates a new value object after replacing items from the parameter list
        /// </summary>
        /// <param name="paramList">list of parameters to be replaced</param>
        /// <returns>new value </returns>
        public ValueList Instance(SymbolList paramList)
        {
            ValueList lst = new ValueList();

            foreach (Value val in m_lst) {
                lst.Add(val.Instance(paramList));
            }

            return lst;
        }

        public void Remove(Value val)
        {
            m_lst.Remove(val);
        }

        public IEnumerator<Value> GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return m_lst.GetEnumerator();
        }

    }

    class Binary
    {
        int m_cbits;            // # of bits in the object - may not be a multiple of 8!!!!
        Byte[] m_rgb;           // Array of bytes - most significant bit is to the far left

        public Byte[] rgb { get { return m_rgb; } }
        public int cbits { get { return m_cbits; } }

        static public Binary Convert(String str)
        {
            Binary bin = new Binary();
            int cb;
            int cbit;
            int i;

            //
            //  Two different input types - either hex or binary based on the last character
            //

            if (str[str.Length - 1] == 'H') {
                //
                //  Start by over allocating the array
                //

                bin.m_rgb = new Byte[str.Length];
                cb = 0;
                cbit = 4;
                for (i = 1; i < str.Length - 2; i++) {
                    if (('0' <= str[i]) && (str[i] <= '9')) {
                        bin.m_rgb[cb] |= (byte) ((str[i] - '0') << cbit);
                    }
                    else if (('A' <= str[i]) && (str[i] <= 'F')) {
                        bin.m_rgb[cb] |= (byte)((str[i] - 'A'+10) << cbit);
                    }
                    else if (('a' <= str[i]) && (str[i] <= 'f')) {
                        bin.m_rgb[cb] |= (byte)((str[i] - 'a'+10) << cbit);
                    }
                    else {
                        continue;
                    }
                    if (cbit == 0) { cbit = 4; cb += 1; }
                    else cbit = 0;
                }

                if (cbit == 0) { cb += 1; }
                Array.Resize<Byte>(ref bin.m_rgb, cb);
                if (cbit == 0) bin.m_cbits = cb * 8 - 4;
                else bin.m_cbits = cb * 8;
            }
            else {
                //
                //  Start by over allocating the array
                //

                bin.m_rgb = new Byte[str.Length / 8 + 1];
                cb = 0;
                cbit = 7;
                for (i = 1; i < str.Length-2; i++) {
                    if (str[i] == '0') {
                        cbit -= 1;
                    }
                    else if (str[i] == '1') {
                        bin.m_rgb[cb] |= (byte) (1 << cbit);
                        cbit -= 1;
                    }
                    if (cbit == -1) {
                        cbit = 7;
                        cb += 1;
                    }
                }

                bin.m_cbits = cb * 8 + (7 - cbit);
                if (cbit != 7) cb += 1;

                Array.Resize<Byte>(ref bin.m_rgb, cb);
            }
            return bin;
        }

        public void SetBit(int iBit)
        {
            int cb = iBit / 8;
            int cbit = 7 - (iBit % 8);

            if ((m_rgb == null) || (cb + 1 > m_rgb.Length)) {
                Array.Resize<Byte>(ref m_rgb, cb + 1);
            }

            m_rgb[cb] |= (byte) (1 << cbit);
        }

        public override string ToString()
        {
            String str = "'";

            if (m_rgb != null) foreach (Byte b in m_rgb) str += b.ToString("X2");
            str += "'H";
            return str;
        }

        public String ToByteArray ()
        {
            String str = "";
            foreach (Byte b in m_rgb) str += ", " + b.ToString ();
            if (str.Length > 0) str = str.Substring (2);

            return "new Byte[]{" + str + "}";
        }

    }

    class OIDNode
    {
        String m_strName;
        String m_strAltName;
        long? m_value;
        OIDNode[] m_rgoiNodes;
        Symbol m_sym = null;

        static public OIDNode m_treeRoot = new OIDNode(-1, "ROOT");

        //
        //  I would love it if there was a low level sparse array, since I don't know
        //  one we will have to basically implement one here.
        //

        public OIDNode(long iVal)
        {
            m_value = iVal;
            m_rgoiNodes = new OIDNode[10];
        }

        public OIDNode(long iVal, String strName)
        {
            m_strName = strName;
            m_value = iVal;
            m_rgoiNodes = new OIDNode[10];
        }

        public OIDNode(String strName)
        {
            m_strName = strName;
        }

        static public void Init()
        {
            Int32 i1;
            m_treeRoot.Add(new OIDNode(0, "itu-t"));
            m_treeRoot[0].AltName = "ccitt";
            m_treeRoot[0].Add(new OIDNode(0, "recommendation"));
            m_treeRoot[0].Add(new OIDNode(1, "question"));
            m_treeRoot[0].Add(new OIDNode(2, "administration"));
            m_treeRoot[0].Add(new OIDNode(3, "network-operator"));
            m_treeRoot[0].Add(new OIDNode(4, "identified-organization"));
            for (Int32 i = 1; i < 27; i++) {
                i1 = i + 95;
                m_treeRoot[0][0].Add(new OIDNode(i, new string(new char[]{Convert.ToChar(i1)})));
            }

            m_treeRoot.Add(new OIDNode(1, "iso"));
            m_treeRoot[1].Add(new OIDNode(0, "standard"));
            m_treeRoot[1].Add(new OIDNode(2, "member-body"));
            m_treeRoot[1].Add(new OIDNode(3, "identified-organization"));

            m_treeRoot.Add(new OIDNode(2, "joint-iso-itu-t"));
            m_treeRoot[2].AltName = "joint-iso-ccitt";
        }

        public bool Add(OIDNode oidChild)
        {
            int i;

            for (i = 0; i < m_rgoiNodes.GetUpperBound(0); i++) {
                if (m_rgoiNodes[i] == null) {
                    m_rgoiNodes[i] = oidChild;
                    return true;
                }
                if (m_rgoiNodes[i].m_value == oidChild.m_value) {
                    //
                    // Duplicate node found
                    //
                    return false;
                }
            }
            //
            //  We need to extend the array and then add it in
            //

            Array.Resize(ref m_rgoiNodes, i+10);
            m_rgoiNodes[i] = oidChild;

            return true;
        }

        public OIDNode FindByName(string str)
        {
            int i;

            for (i = 0; i < m_rgoiNodes.GetUpperBound(0); i++) {
                if ((m_rgoiNodes[i] != null) &&
                    ((m_rgoiNodes[i].m_strName == str) ||
                     (m_rgoiNodes[i].m_strAltName == str))) {
                    return m_rgoiNodes[i];
                }
            }

            return null;
        }

        public OIDNode this[int i]
        {
            get
            {
                foreach (OIDNode oid in m_rgoiNodes) {
                    if ((oid != null) && (oid.m_value == i)) return oid;
                }
                return null;
            }
        }

        public String AltName
        {
            get { return m_strAltName; }
            set { m_strAltName = value; }
        }

        public OIDNode[] children
        {
            get { return m_rgoiNodes; }
        }

        public bool HasName
        {
            get { return m_strName != null; }
        }

        public bool HasValue
        {
            get { return m_value.HasValue; }
        }

        public long iValue
        {
            get { return (long) m_value; }
            set { m_value = value; }
        }

        public string name
        {
            get { return m_strName; }
            set { Debug.Assert(m_strName == null); m_strName = value; }
        }

        public Symbol symbol
        {
            get { return m_sym; }
            set { Debug.Assert(m_sym == null); m_sym = value; }
        }

        public override string ToString()
        {
            String str = "";

            if (m_strName != null) str += m_strName;
            if (m_value.HasValue) str += "(" + m_value + ")";
            return str;
        }
    }
}
