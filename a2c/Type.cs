using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace asn_compile_cs
{
    enum BaseType { Leaf = 1, Sequence, Symbol, Class, Real, NamedList, Constraint,
        Null, Tagged, ObjectDescriptor, OID_IRI, SequenceOf, SetOf, RestrictedCharacter, UnrestrictedCharacter, Choice,
        InstanceOf, ObjectClassField, Param_Assignment, RelativeObjectIdentifier, Set, Time, OpenType, External, Embedded_PDV, Assignment}

    enum RestrictedCharacter { BMPString=1, GeneralString, GraphicString, IA5String, NumericString,
            PrintableString, TeletexString, T61String, UniversalString, UTF8String, VideotexString, VisibleString}

    public enum TagType { Explicit, Implicit, Automatic, Specified };
    public enum TagClass { Universal=1, Application=2, Private=4, Context=3 };

    enum DataType { Boolean = 1, Integer, BitString, OctetString, Enumerated, ObjectIdentifier, Date, RelativeObjectIdentifier, Real, String, Null }

    class FieldFlags
    {
        Boolean m_fUnique = false;
        Boolean m_fOptional = false;
        Boolean m_fPointer = false;
        Boolean m_fView = false;
        Value m_valDefault;
        Type m_typeDefault;

        public Boolean Default
        {
            get { return (m_valDefault != null) || (m_typeDefault != null); }
        }

        public Boolean Optional
        {
            get { return m_fOptional; }
            set { m_fOptional = value; }
        }

        public Boolean Pointer
        {
            get { return m_fPointer; }
            set { m_fPointer = value; }
        }

        public Value valDefault
        {
            get { return m_valDefault; }
            set { m_valDefault = value; }
        }

        public Boolean View
        {
            get { return m_fView; }
            set { m_fView = value; }
        }

        /// <summary>
        /// Unique Property: Is this field tagged as Unique?
        /// </summary>
        public Boolean Unique
        {
            get { return m_fUnique; }
            set { m_fUnique = value; }
        }

        public FieldFlags Instance(SymbolList paramList)
        {
            return null;
        }

        public void SetDefault(Type type)
        {
            m_typeDefault = type;
        }

        public void SetDefault(Value val)
        {
            m_valDefault = val;
        }
    }

    class Type
    {
        //
        //  External public constants
        //

        /// <summary>
        /// BitStringType is a global type object for a BitString
        /// </summary>
        /// 

        public static Type BitStringType;
        public static Type BitStringNamed;
        public static Type BmpString;
        public static Type BooleanType;
        public static Type CharacterString;
        public static Type EmbeddedPDV;
        public static Type EnumeratedType;
        public static Type EnumeratedNative;
        public static Type External;
        public static Type GeneralString;
        public static Type GraphicString;
        public static Type IA5String;
        public static Type IntegerType;
        public static Type IntegerNative;
        public static Type Null;
        public static Type NumericString;
        public static Type ObjectDescriptor;
        public static Type ObjectIdentifier;
        public static Type OctetString;
        public static Type OID_IRI;
        public static Type OpenOctetString;
        public static Type OpenBitString;
        public static Type Open;
        public static Type PrintableString;
        public static Type RelativeOID;
        public static Type TeletexString;
        public static Type UniversalString;
        public static Type Utf8String;
        public static Type VideotexString;
        public static Type VisibleString;

        public static Type GeneralizedTime;
        public static Type UTCTime;

        public static Type Time;

        public static Type InstanceOf;
        public static Type PositiveInteger;

        //
        //  Interior
        //

        // Control on what elements are going to be set
        BaseType m_type;

        //  Item is now marked as read-only
        Boolean m_fReadOnly;

        //  If set - points to the symbol in the symbol table for this type
        Symbol m_symOwner;

        // If BaseType.RestrictedCharacter - which restricted character
        RestrictedCharacter m_restrictedCharacter;

        // If BaseType.Symbol - which symbol is being refered to
        Symbol m_sym;

        TokenList m_tknList;

        String m_strName;
        SymbolList m_symList;
        RuleStep m_syntax;
        ConstraintList m_constraintList;
        Tagging m_tagging;
        Type m_subType;
        DataType m_dataType;

        Boolean m_fAtomic = false;  //  Treat references as atomic items?
        Boolean m_fTextualRoot = false; // Is this node a textual root type?
        Boolean m_fEmitting;
        String m_strEmittedName;
        String m_strCSharpName;

        // Viewer Emitter
        ViewerTypeNode m_viewerNode = null;

        /// <summary>
        /// Type Constructor - Private
        /// </summary>
        Type()
        {
            m_fReadOnly = false;
        }

        public Type(BaseType typ)
        {
            m_type = typ;
            if (typ == BaseType.Tagged) m_tagging = new Tagging();
            m_fReadOnly = false;
        }

        public Type(Symbol sym)
        {
            m_type = BaseType.Symbol;
            m_sym = sym;
            m_fReadOnly = false;
        }

        public Type(BaseType typ, Tagging tag, DataType dataType, String emitName, String CSharpName)
        {
            m_type = typ;
            m_tagging = tag;
            m_dataType = dataType;
            m_fReadOnly = true;
            m_strEmittedName = emitName;
            m_strCSharpName = CSharpName;
        }

        static public void Init()
        {
            BitStringType = new Type(BaseType.Leaf, new Tagging(TagClass.Universal, TagType.Specified, 3), DataType.BitString, "A2C_BIT_STRING", "A2C.BitString");
            BitStringNamed = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 3), DataType.BitString, "A2C_BIT_STRING_NAMED", "A2C.NamedBitString");
            BmpString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 30), DataType.String, "A2C_BMP_STRING", "A2C.BMPString");
            BooleanType = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 1), DataType.Boolean, "A2C_BOOLEAN", "A2C.Boolean");
            CharacterString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 29), DataType.String, "A2C_CHARACTOR_STRING", "A2C.CharacterString");
            EmbeddedPDV = null;
            EnumeratedType = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 10), DataType.Enumerated, "A2C_ENUMERATED_HUGE", "A2C.Enumerated");
            EnumeratedNative = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 10), DataType.Enumerated, "A2C_ENUMERATED_NATIVE", "A2C.Enumerated");
            External = null;
            GeneralString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 27), DataType.String, "A2C_GENERAL_STRING", "A2C.GeneralString");
            GraphicString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 25), DataType.String, "A2C_GRAPHIC_STRING", "A2C.GraphicString");
            IA5String = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 22), DataType.String, "A2C_IA5_STRING", "A2C.IA5String");
            IntegerType = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 2), DataType.Integer, "A2C_INTEGER_HUGE", "A2C.Integer");
            IntegerNative = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 2), DataType.Integer, "A2C_INTEGER_NATIVE", "A2C.Integer");
            Null = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 5), DataType.Null, "A2C_NULL", "A2C.Null");
            NumericString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 18), DataType.String, "A2C_NUMERIC_STRING", "A2C.NumericString");
            PrintableString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 19), DataType.String, "A2C_PRINTABLE_STRING", "A2C.PrintableString");
            ObjectDescriptor = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 7), DataType.String, "A2C_OBJECT_DESCRIPTOR", "A2C.ObjectDescriptor");
            ObjectIdentifier = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 6), DataType.ObjectIdentifier, "A2C_OBJECT_IDENTIFIER", "A2C.ObjectIdentifier");
            OctetString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 4), DataType.OctetString, "A2C_OCTET_STRING", "A2C.OctetString");
            OpenOctetString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 4), DataType.OctetString, "A2C_OPEN_OCTET_STRING", "A2C.OctetString");
            OpenBitString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 3), DataType.BitString, "A2C_OPEN_BIT_STRING", "A2C.BitString");
            Open = new Type (BaseType.OpenType, new Tagging (TagClass.Universal, TagType.Specified, 0), DataType.Null, "A2C_OPEN_TYPE", "A2C.ASN");
            RelativeOID = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 13), DataType.ObjectIdentifier, "A2C_RELATIVE_OBJECT_IDENTIFIER", "A2C.RelativeObjectIdentifier");
            TeletexString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 20), DataType.String, "A2C_TELETEX_STRING", "A2C.TeletexString");
            UniversalString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 28), DataType.String, "A2C_UNIVERSAL_STRING", "A2C.UniversalString");
            Utf8String = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 12), DataType.String, "A2C_UTF8_STRING", "A2C.UTF8String");
            VideotexString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 21), DataType.String, "A2C_VIDEOTEX_STRING", "A2C.VideoTextString");
            VisibleString = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 26), DataType.String, "A2C_VISIBLE_STRING", "A2C.VisibleString");

            GeneralizedTime = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 24), DataType.Date, "A2C_GENERALIZED_TIME", "A2C.GeneralizedTime");
            UTCTime = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 23), DataType.Date, "A2C_UTC_TIME", "A2C.UtcTime");

            PositiveInteger = new Type(BaseType.Constraint);
            PositiveInteger.subType = new Type (BaseType.Leaf, new Tagging (TagClass.Universal, TagType.Specified, 2), DataType.Integer, "A2C_INTEGER", "A2C.Integer");

            //

            {
                SymbolList lst = new SymbolList();
                
                InstanceOf = new Type(BaseType.Sequence);
                lst.Add(new Symbol("&type-id", SymbolType.Component));
                lst[0].type = ObjectIdentifier;
                lst.Add(new Symbol("&value", SymbolType.Component));
                lst[1].type = new Type(BaseType.Tagged);
                lst[1].type.tagged.tagClass = TagClass.Context;
                lst[1].type.tagged.tagType = TagType.Explicit;
                lst[1].type.tagged.value = 0;
                lst[1].type.subType = new Type(BaseType.OpenType);
                lst[1].type.subType.emitName = "A2C_OPEN_TYPE";
                InstanceOf.fieldList = lst;
            }
            
        }

        public BaseType baseType { get { return m_type; } }

        public Type classType
        {
            get { Debug.Assert(m_type == BaseType.ObjectClassField); return m_subType; }
            set { Debug.Assert(!m_fReadOnly);  Debug.Assert(m_type == BaseType.ObjectClassField); m_subType = value; }
        }

        public ConstraintList constraintList
        {
            get { return m_constraintList; }
            set { m_constraintList = value; }
        }

        public DataType dataType
        {
            get { return m_dataType; }
        }

        public string emitName
        {
            get { return m_strEmittedName; }
            set { Debug.Assert((m_strEmittedName == null) || (m_strEmittedName == value)); m_strEmittedName = value; }
        }

        public string CSharpName
        {
            get {
                if (m_type == BaseType.Assignment) return m_symOwner.CSharpName;
                return m_strCSharpName;
            }
            set { Debug.Assert ((m_strCSharpName == null) || (m_strCSharpName == value)); m_strCSharpName = value; }
        }

        public Boolean emitting
        {
            get { return m_fEmitting; }
            set { m_fEmitting = value; }
        }

        public TokenList fieldTokens
        {
            get { Debug.Assert(m_type == BaseType.ObjectClassField); return m_tknList; }
            set { Debug.Assert(!m_fReadOnly);  Debug.Assert(m_type == BaseType.ObjectClassField); m_tknList = value; }
        }

        public SymbolList fieldList
        {
            get { return m_symList; }
            set { Debug.Assert(!m_fReadOnly);  m_symList = value; }
        }

        public Boolean IsAtomic
        {
            get { return m_fAtomic; }
            set { m_fAtomic = value; }
        }

        public Boolean IsRootType
        {
            get { return m_fTextualRoot; }
            set { m_fTextualRoot = value; }
        }

        public String name
        {
            get { return m_strName; }
            set { Debug.Assert(!m_fReadOnly);  m_strName = value; }
        }

        public Symbol owner
        {
            get { return m_symOwner; }
            set { m_symOwner = value; }
        }

        public Boolean readOnly
        {
            get { return m_fReadOnly; }
            set { m_fReadOnly = true; }
        }

        public RestrictedCharacter restrictedString
        {
            get { return m_restrictedCharacter; }
            set { Debug.Assert(!m_fReadOnly);  m_restrictedCharacter = value; }
        }

        public Type subType
        {
            get { return m_subType; }
            set { Debug.Assert(!m_fReadOnly); m_subType = value; }
        }

        public Symbol sym { get { return m_sym; } }

        public RuleStep syntax
        {
            get { return m_syntax; }
            set { Debug.Assert(!m_fReadOnly); m_syntax = value; }
        }

        public Tagging tagged
        {
            get { return m_tagging; }
            set { Debug.Assert(!m_fReadOnly); m_tagging = value; }
        }

        public void AddConstraint(Constraint cst)
        {
            Debug.Assert(!m_fReadOnly);  
            if (m_constraintList == null) {
                m_constraintList = new ConstraintList();
            }
            m_constraintList.Add(cst);
        }

        public void Collapse()
        {
            Debug.Assert(!m_fReadOnly || (baseType == BaseType.Leaf));  
            if (m_symList != null) {
                foreach (Symbol sym in m_symList) sym.Collapse();
            }

            if (m_constraintList != null) {
                foreach (Constraint cst in m_constraintList) cst.Collapse();
            }
        }

        public void Dump(String tab)
        {
            switch (m_type) {
            case BaseType.Assignment:
                m_subType.Dump(tab);
                break;

            case BaseType.Param_Assignment:
                m_subType.Dump(tab);
                break;

            case BaseType.Leaf:
                switch (m_dataType) {
                case DataType.BitString:
                    Console.Error.WriteLine(tab + "[3] Bit String");
                    break;

                case DataType.Boolean:
                    Console.Error.WriteLine(tab + "[1] Boolean");
                    break;

                case DataType.Enumerated:
                    Console.Error.WriteLine(tab + "[10] Integer");
                    break;

                case DataType.Integer:
                    Console.Error.WriteLine(tab + "[2] Integer");
                    break;

                case DataType.ObjectIdentifier:
                    Console.Error.WriteLine(tab + "[6] Object Identifier");
                    break;

                case DataType.OctetString:
                    Console.Error.WriteLine(tab + "[4] Octet String");
                    break;
                }
                break;

            case BaseType.Choice:
                Console.Error.WriteLine(tab + "Choice");
                break;

            case BaseType.Class:
                Console.Error.WriteLine(tab + "Class");
                break;

            case BaseType.Null:
                Console.Error.WriteLine(tab + "[5] Null");
                break;

            case BaseType.RestrictedCharacter:
                switch (m_restrictedCharacter) {
                case RestrictedCharacter.BMPString:
                    Console.Error.WriteLine(tab + "[30] BMP String");
                    break;

                case RestrictedCharacter.GeneralString:
                    Console.Error.WriteLine(tab + "[27] General String");
                    break;

                case RestrictedCharacter.GraphicString:
                    Console.Error.WriteLine(tab + "[25] Graphic String");
                    break;

                case RestrictedCharacter.IA5String:
                    Console.Error.WriteLine(tab + "[22] IA5 String");
                    break;

                case RestrictedCharacter.NumericString:
                    Console.Error.WriteLine(tab + "[18] Numeric String");
                    break;

                case RestrictedCharacter.PrintableString:
                    Console.Error.WriteLine(tab + "[19] Printable String");
                    break;

                case RestrictedCharacter.T61String:
                case RestrictedCharacter.TeletexString:
                    Console.Error.WriteLine(tab + "[20] TeletexString");
                    break;

                case RestrictedCharacter.UniversalString:
                    Console.Error.WriteLine(tab + "[28] Universal String");
                    break;

                case RestrictedCharacter.UTF8String:
                    Console.Error.WriteLine(tab + "[12] UTF8 String");
                    break;

                case RestrictedCharacter.VideotexString:
                    Console.Error.WriteLine(tab + "[21] Videotex String");
                    break;

                case RestrictedCharacter.VisibleString:
                    Console.Error.WriteLine(tab + "[26] Visible String");
                    break;

                default:
                    Console.Error.WriteLine(tab + "[**] " + m_restrictedCharacter.ToString());
                    break;
                }
                break;

            case BaseType.Sequence:
                Console.Error.WriteLine(tab + "[16] Sequence");
                break;

            case BaseType.SetOf:
                Console.Error.WriteLine(tab + "[17] SET OF");
                break;

            case BaseType.Tagged:
                Console.Error.WriteLine(tab + "[ " + m_tagging.tagType.ToString() + " " + m_tagging.value.ToString() + " " + m_tagging.tagClass.ToString() + " ]");
                m_subType.Dump(tab + "    ");
                break;

            case BaseType.Symbol:
                Console.Error.WriteLine(tab + "symbol: " + m_sym.name);
                break;

            case BaseType.Constraint:
                break;

            default:
                Console.Error.WriteLine(tab + "**"+ m_type.ToString());
                break;
            }
            if (m_symList != null) {
                foreach (Symbol sym in m_symList) sym.Dump(tab+"    ");
            }

            if (m_constraintList != null) {
                Console.Error.WriteLine(tab + "    Constraints:");
                foreach (Constraint cst in m_constraintList) cst.Dump(tab + "        ");
            }
        }

        /// <summary>
        /// Replace parameters from list and return the new type object
        /// </summary>
        /// <param name="paramList">List of parameters to be replaced</param>
        /// <returns>new type object</returns>
        public Type Instance(SymbolList paramList)
        {
            if (m_type == BaseType.Leaf) return this;

            //
            //  Depending on what the assignment is, we are going to either just
            //  stop or continue the substitutions.
            //

            if ((m_type == BaseType.Assignment) && (m_symOwner != null)) {
                switch (m_symOwner.GetSymbolType()) {
                case SymbolType.ParameterizedType:
                    break;

                case SymbolType.Object:
                case SymbolType.ObjectClass:
                case SymbolType.ObjectSet:
                case SymbolType.Type:
                    return this;

                default:
                    Debug.Assert(false);
                    return this;
                }
            }

            Type newType = new Type();

            //
            //  Is this what we are replacing?
            //

            if ((m_sym != null) && 
                ((m_sym.GetSymbolType() == SymbolType.TypeParameter) ||
                (m_sym.GetSymbolType() == SymbolType.ObjectClassParameter))) {
                foreach (Symbol sym in paramList) {
                    if (sym.name == m_sym.name) {
                        return sym.type;
                    }
                }
                Debug.Assert(false, "Missing type parameter");
            }

            //
            //  Copy over the base type
            //

            newType.m_type = m_type;
            newType.m_strName = m_strName;
            newType.m_restrictedCharacter = m_restrictedCharacter;
            newType.m_tknList = m_tknList;
            newType.m_tagging = m_tagging;
            newType.m_fTextualRoot = m_fTextualRoot;

            //
            //  Process all of the complex fields if necessary
            //

            if (m_symList != null) {
                newType.m_symList = m_symList.Instance(paramList);
            }
            if (m_constraintList != null) {
                newType.m_constraintList = new ConstraintList();
                foreach (Constraint cst in m_constraintList) {
                    newType.m_constraintList.Add(cst.Instance(paramList));
                }
            }
            if (m_subType != null) {
                newType.m_subType = m_subType.Instance(paramList);
            }
            if (m_sym != null) {
                newType.m_sym = m_sym.Instance(paramList);
            }

            return newType;
        }

        public void SetEmitString(BaseType baseType, DataType dataType, Type typeNew)
        {
            if ((m_type == baseType) && (m_dataType == dataType)) {
                Debug.Assert(false, "Error in SetEmitString - should not have gotten here.");
                return;
            }

            switch (m_type) {
            case BaseType.Assignment:
            case BaseType.NamedList:
                if ((subType.m_type == baseType) && (subType.m_dataType == dataType)) {
                    m_subType = typeNew;
                }
                else {
                    m_subType.SetEmitString(baseType, dataType, typeNew);
                }
                return;
            }
            Debug.Assert(false, "Error in SetEmitString");
        }

        public Type SkipFluff()
        {
            switch (m_type) {
            case BaseType.Assignment:
            case BaseType.Constraint:
            case BaseType.NamedList:
            case BaseType.Param_Assignment:
            case BaseType.Tagged:
                return m_subType == null ? null : m_subType.SkipFluff();

            case BaseType.Symbol:
                if ((m_sym.symType & SymbolType.Parameter) != 0) {
                    return this;
                }

                return m_sym.type.SkipFluff();
            }

            return this;
        }

        /// <summary>
        /// IsInteger checks to see if this is an integer type.
        /// </summary>
        /// <returns>Boolean</returns>
        public Boolean IsInteger()
        {
            return m_dataType == DataType.Integer;
        }

        public Boolean IsOID()
        {
            return m_dataType == DataType.ObjectIdentifier;
        }

        public Boolean IsSequence()
        {
            return m_type == BaseType.Sequence;
        }

        public Boolean IsRelativeOID()
        {
            return m_dataType == DataType.ObjectIdentifier;
        }

        /// <summary>
        /// Follow the list of tokens looking for the actual symbol
        /// </summary>
        /// <returns></returns>
        internal Symbol GetObjectClassField()
        {
            Type type = m_subType.SkipFluff();

            Debug.Assert(baseType == BaseType.ObjectClassField);

            switch (type.baseType) {
            case BaseType.Symbol:
                if ((m_subType.sym.symType & SymbolType.Parameter) != 0) {
                    //
                    //  THis type is parameterized - we will never be able to 
                    //  resolve down to a symbol - so return an error.
                    //

                    return null;
                }
                throw new Exception("The method or operation is not implemented.");
                break;

            case BaseType.Class:
                Symbol symResult = null;
                Debug.Assert(fieldTokens.Count == 1);

                foreach (Token tkn in fieldTokens) {
                    Debug.Assert(type.baseType == BaseType.Class);
                    symResult = null;

                    foreach (Symbol symT in type.fieldList) {
                        if (tkn.name == symT.name) {
                            symResult = symT;
                            break;
                        }
                    }


                }
                return symResult;

            default:

                throw new Exception("The method or operation is not implemented.");
            }

            return null;
        }
    }

    public class Tagging
    {
        TagClass m_tagClass;
        TagType m_tagType;
        int m_value;

        public Tagging()
        {
        }

        public Tagging(TagClass tagClass, TagType tagType, int tagValue)
        {
            m_tagClass = tagClass;
            m_tagType = tagType;
            m_value = tagValue;
        }

        public TagClass tagClass
        {
            get { return m_tagClass; }
            set { m_tagClass = value; }
        }

        public TagType tagType
        {
            get { return m_tagType; }
            set { m_tagType = value; }
        }

        public int value
        {
            get { return m_value; }
            set { m_value = value; }
        }

        public string Format ()
        {
            string strClass;
            switch (m_tagClass) {
            case TagClass.Application: strClass = "TagClass.Application"; break;
            case TagClass.Context: strClass = "TagClass.Context"; break;
            case TagClass.Private: strClass = "TagClass.Private"; break;
            case TagClass.Universal: strClass = "TagClass.Universal"; break;
            default: throw new Exception("ICE");
            }

            string strType;
            switch (m_tagType) {
            case TagType.Automatic: strType = "TagType.Automatic"; break;
            case TagType.Explicit: strType = "TagType.Explicit"; break;
            case TagType.Implicit: strType = "TagType.Implicit"; break;
            case TagType.Specified: strType = "TaqType.Specific"; break;
            default: throw new Exception("ICE");
            }

            string s = "new Tag(" + strClass + ", " + m_value.ToString() + ", " + strType + ")";
            return s;
        }

        public int Encoded
        {
            get { return ((int) m_tagClass-1) << 6 | m_value; }
        }
    }
}
