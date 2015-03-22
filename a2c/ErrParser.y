D_Assignment ::=		D_TypeAssignment | D_ValueAssignment | D_ValueSetTypeAssignment | D_ObjectClassAssignment | D_ObjectAssignment |  D_ObjectSetAssignment

D_TypeAssignment ::=		U_Identifier MatchedBraces?             "::=" D_Type
D_ObjectClassAssignment ::=	U_Identifier MatchedBraces?             "::=" D_ObjectClass
D_ValueSetTypeAssignment ::=	U_Identifier MatchedBraces? D_Type      "::=" MatchedBraces
D_ObjectSetAssignment ::=	U_Identifier MatchedBraces? D_DefinedObjectClass "::=" MatchedBraces

D_ValueAssignment ::=		L_Identifier MatchedBraces? D_Type      "::=" D_Value
D_ObjectAssignment ::=		L_Identifier MatchedBraces? D_DefinedObjectClass "::=" D_Object

D_Type ::=			D_PrimativeType D_ConstraintList | D_TypeWithConstraint
D_PrimativeType ::=		D_BuiltinType  | D_ReferencedType
D_ConstraintList ::=		MatchParens D_ConstraintList | MatchParens
D_BuiltinType ::=		D_BitStringType | BOOLEAN | D_CharacterStringType | EMBEDDED PDV | EXTERNAL | D_InstanceOfType | D_IntegerType | NULL | D_ObjectClassFieldType | OBJECT IDENTIFIER | OCTET STRING | REAL | RELATIVE-OID | D_SequenceType | D_SequenceOfType | D_TaggedType
D_ReferencedType ::=		D_DefinedType | D_UsefulType | D_SelectionType | D_TypeFromObject | D_ValueSetFromObjects
D_BitStringType ::=		BIT STRING MatchedBraces | BIT STRING
D_InstanceOfType ::=		INSTANCE OF D_DefinedObjectClass
D_IntegerType ::=		INTEGER MatchedBraces | INTEGER
D_ObjectClassFieldType ::=	D_DefinedObjectClass "." D_FieldName
D_SetOrSequence ::=             SET | SEQUENCE | CHOICE | ENUMERATED
D_SequenceType ::=		D_SetOrSequence MatchedBraces
D_SequenceOfType ::=		D_SetOrSequence OF D_TypeOrNamedType
D_TaggedType ::=		D_Tag IMPLICIT D_Type | D_Tag EXPLICIT D_Type | D_Tag D_Type
D_TypeWithConstraint ::=	D_SetOrSequence SIZE ? MatchParens OF D_TypeOrNamedType
D_DefinedType ::=		D_UIdDot ? U_Identifier  | D_ParameterizedType
D_UsefulType ::=		GeneralizedTime | UTCTime | ObjectDescriptor
D_SelectionType ::=		L_Identifier "<" D_Type
D_TypeFromObject ::=		D_ReferencedObjects "." D_FieldName
D_ValueSetFromObjects ::=	D_ReferencedObjects "." D_FieldName
D_CharacterStringType ::=	BMPString | GeneralString | GraphicString | IA5String | ISO646String | NumericString | PrintableString | TeletexString | T61String | UniversalString | UTF8String | VideotexString | VisibleString | CHARACTER STRING
D_DefinedObjectClass ::=	D_UIdDot ? U_Identifier | D_UsefulObjectClassReference
D_FieldName ::=			D_PrimitiveFieldName "." +
D_PrimitiveFieldName ::=	typefieldreference | valuefieldreference | valuesetfieldreference | objectfieldreference | objectsetfieldreference
D_Tag ::=			"[" D_Tag1 "]"
D_Tag1 ::=                      ! "]" ANY D_Tag1 | empty
D_ParameterizedType ::=		D_SimpleDefinedType MatchedBraces
D_ReferencedObjects ::=		D_DefinedObject | D_ParameterizedObject | D_DefinedObjectSet | D_ParameterizedObjectSet
D_UsefulObjectClassReference ::=	TYPE-IDENTIFIER | ABSTRACT-SYNTAX
D_SimpleDefinedType ::=		D_UIdDot? U_Identifier
D_DefinedObject ::=		D_UIdDot? L_Identifier
D_ParameterizedObjectSet ::=	D_DefinedObjectSet MatchedBraces
D_DefinedObjectSet ::=		U_Identifer "." U_Identifier | U_Identifier
D_Value ::=			D_BuiltinValue  | D_ReferencedValue  | D_ObjectClassFieldValue
D_BuiltinValue ::=		D_BitStringValue | D_BooleanValue | D_CharacterStringValue | D_ChoiceValue | MatchedBraces | L_Identifier | D_IntegerValue | NULL | D_OctetStringValue | D_RealValue
D_ReferencedValue ::=		D_DefinedValue | D_ValueFromObject
D_ObjectClassFieldValue ::=	D_OpenTypeFieldVal | D_FixedTypeFieldVal
D_OpenTypeFieldVal ::=		D_Type ":" D_Value
D_FixedTypeFieldVal ::=		D_BuiltinValue | D_ReferencedValue
D_BitStringValue ::=		bstring  | hstring  | MatchedBraces | CONTAINING D_Value
D_BooleanValue::=		TRUE  | FALSE
D_CharacterStringValue ::=	cstring  | MatchedBraces
D_ChoiceValue ::=		L_Identifier ":" D_Value
D_IntegerValue ::=		D_SignedNumber  | L_Identifier
D_SignedNumber ::=		number  | "-" number
D_OctetStringValue ::=		bstring  | hstring  | CONTAINING D_Value
D_RealValue ::=			D_NumericRealValue  | D_SpecialRealValue
D_NumericRealValue ::=		realnumber | "-" realnumber | MatchedBraces
D_SpecialRealValue ::=		PLUS-INFINITY  | MINUS-INFINITY
D_DefinedValue ::=		U_Identifer "." L_Identifier | L_Identifier | D_ParameterizedValue
D_ValueFromObject ::=		D_ReferencedObjects "." D_FieldName
D_ParameterizedValue ::=	D_SimpleDefinedValue MatchedBraces
D_SimpleDefinedValue ::=	U_Identifer "." L_Identifier  | L_Identifier
D_Object ::=			D_DefinedObject | MatchedBraces | D_ObjectFromObject | D_ParameterizedObject
D_ObjectFromObject ::=		D_ReferencedObjects "." D_FieldName
D_ParameterizedObject ::=	D_DefinedObject MatchedBraces
D_ObjectClass ::=		D_DefinedObjectClass | D_ObjectClassDefn | D_ParameterizedObjectClass
D_ObjectClassDefn ::=		CLASS MatchedBraces D_WithSyntaxSpec?
D_ParameterizedObjectClass ::=	D_DefinedObjectClass MatchedBraces
D_WithSyntaxSpec ::=		WITH SYNTAX MatchedBraces
D_TypeOrNamedType ::=           L_Identifier? D_Type
D_UIdDot ::=                    U_Identifier "."

MatchedBraces ::=               "{" MB1 "}"
MB1 ::=                         "{" MB1 "}" | ! "{" ! "}" ANY MB1

MatchParens ::=                 "(" MP1 ")"
MP1 ::=                         "(" MP1 ")" | ! ")" ! ")" ANY MP1
