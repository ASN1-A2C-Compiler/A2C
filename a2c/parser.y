using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;


%%
%namespace asn_compile_cs

%type Constraint(constraint)    AdditionalElementSetSpec, ComponentRelationConstraint, Constraint, ConstraintSpec, ContainedSubtype, ContentsContraint,
                                Elements, ElementSetSpec, ElementSetSpecs, Exclusions, GeneralConstraint,
                                InnerTypeConstraints, IntersectionElements, Intersections, 
                                LowerEndpoint, LowerEndValue, Marker, ObjectSetElements, PatternConstraint, 
                                PermittedAlphabet, RootElementSetSpec, 
                                SimpleTableConstraint, SingleValue, SizeConstraint, SubtypeConstraint, SubtypeElements, TableConstraint, TypeConstraint, 
                                Unions, UpperEndpoint, UpperEndValue, UserDefinedConstriant, ValueRange, ValueSet

%type ConstraintList(constraintList) ConstraintList, ConstraintListOpt, JointUnions

%type AsnFile(af)               ModuleName

%type Symbol(sym)               DummyGovernor, DummyReference, EnumerationItem, ExceptionSpec, 
                                ExtensionAdditionAlternative, ExtensionAdditionAlternativesGroup, ExtensionEndMarker,
                                ExternalObjectClassReference, ExternalObjectReference, ExternalObjectSetReference, ExternalTypeReference, ExternalValueReference, 
                                FieldSpec, FixedTypeValueFieldSpec, FixedTypeValueSetFieldSpec,
                                GlobalModuleReference, Governor, ItemSpec, 
                                ModuleIdentifier, 
                                NamedBit, NamedNumber, NamedType, 
                                objectclassreference, objectfieldreference, objectreference, objectsetfieldreference, objectsetreference,
                                OptionalExtensionMarker, ObjectFieldSpec, ObjectSetFieldSpec,
                                ParameterizedReference, ParamGovernor, PrimitiveFieldName, Reference, SimpleDefinedType, Symbol, SymbolsFromModule, 
                                typereference, typefieldreference, TypeFieldSpec, 
                                valuefieldreference, valueReference, valuesetfieldreference, VariableTypeValueFieldSpec, VariableTypeValueSetFieldSpec

%type SymbolList(symList)       ActualParameterList, ActualParameterList1, AdditionalEnumeration, 
                                AlternativeTypeList, AlternativeTypeLists, AlternativeTypeLists1, AtNotation, AtNotationList,
                                ComponentIdList, ComponentType, ComponentTypeList, ComponentTypeLists,
                                Enumeration, Enumerations, EnumExtension, 
                                ExtensionAddition,  ExtensionAdditionGroup, ExtensionAdditionList, ExtensionAdditions, 
                                ExtensionAlternativeList, ExtensionAlternatives, ExtensionAndException,
                                FieldSpecList, IdentifierList, Level,
                                NamedBitList, NamedNumberList, 
                                ParameterList,
                                R0, R1, R2, R3, RootAlternativeTypeList, RootComponentTypeList, RootEnumeration, 
                                SymbolList, SymbolsExported, SymbolsFromModuleList, SymbolsImported

%type Token(token)              A_L_Identifier, A_U_Identifier, BinaryString, cstring, HexString, L_Identifier, U_Identifier, mstring, Number, 
                                objectclassreferenceLHS, objectreferenceLHS, objectsetreferenceLHS, 
                                Real, ReferenceLHS, typefieldreferenceLHS, typereferenceLHS, UNIQUE, valuefieldreferenceLHS, valueReferenceLHS,
                                valuesetfieldreferenceLHS, objectfieldreferenceLHS, objectsetfieldreferenceLHS,
                                Group, Plane, Row, Cell, TableColumn, TableRow, moduleReferenceLHS, moduleReference, PrimitiveFieldNameAnon,
                                DefinedValueLHS, ExternalValueReferenceLHS

%type TokenList(tokenList)      ItemList1, FieldName

%type Type(type)                BuiltinType, 
                                DefinedObjectClass, DefinedType,
                                ObjectClass, ObjectClassDefn,
                                ParameterizedObjectClass, ParameterizedType, ParameterizedValueSetType, PrimitiveType,
                                ReferencedType, RestrictedCharacterStringType, 
                                SelectionType, Tag, Type, TypeFromObject,
                                TypeWithConstraint, UsefulObjectClassReference, UsefulType, ValueSetFromObjects, 
                                UnrestrictedCharacterStringType,

                                BitStringType, BooleanType, CharacterStringType, ChoiceType, EmbeddedPDVType, EnumeratedType, ExternalType,
                                InstanceOfType, IntegerType, NullType, ObjectClassFieldType, ObjectIdentifierType, OctetStringType, 
                                RealType,
                                RelativeOIDType, SequenceType, SequenceOfType, SetType , SetOfType, TaggedType


%type Value(value)              AS1, AS1Value, AssignedIdentifier, BuiltinValue, Class, ClassNumber,
                                DefinedObject, DefinedObjectSet, DefinedValue, DefinitiveNumberForm, DefinitiveNameAndNumberForm, DefinitiveObjIdComponent, DefinitiveOID,
                                DefinitiveIdentifier,
                                ExceptionIdentification,
                                NameAndNumberForm, NameForm, NamedValue, NumberForm, NumericRealValue,
                                Object, ObjectSetFromObjects, ObjectClassFieldValue, ObjectDefn, ObjectSet, ObjectSetSpec, ObjIdComponents, OIDValue, OpenTypeFieldVal,
                                ParameterizedObjectSet, ParameterizedValue, ReferencedObjects, ReferencedValue, SignedNumber, SpecialRealValue, TagType, TypeIdentifier, 
                                Value, ValueFromObject, VersionNumber,

                                BitStringValue, BooleanValue, CharacterStringList, CharacterStringValue, ChoiceValue, 
                                EmbeddedPDVValue, EnumeratedValue, ExternalValue,
                                InstanceOfValue, IntegerValue, NullValue, 
                                ObjectIdentifierValue, OctetStringValue, 
                                RealValue, RelativeOIDComponents, RelativeOIDValue, RestrictedCharacterStringValue,
                                SequenceValue, SequenceOfValue, SetValue, SetOfValue, TaggedValue, UnrestrictedCharacterStringValue


%type ValueList(valueList)      ComponentValueList,
                                DefinitiveObjIdComponentList,
                                NamedValueList, 
                                ObjectSetSpec1, ObjectSetSpec2, ObjIdComponentsList,
                                RelativeOIDComponentsList, ValueList

%type RuleStep(rs)              Literal, OptionalGroup, RequiredToken, SyntaxList, word, WithSyntaxSpec

%type RuleStepList(rsList)      TokenList

%type FieldFlags(fieldFlags)    ComponentTypeOption, ObjectOptionalitySpec, ObjectSetOptionalitySpec, 
                                TypeOptionalitySpec, ValueOptionalitySpec, ValueSetOptionalitySpec

%%

//
//  SECTION 12.1
//
ModuleDefinition ::= Commands? ModuleHeader Commands? BEGIN ModuleBody ? EncodingControlSections ? END 
        	{
                    //
                    //  Make sure that all items in the export list are actually defined
                    //

                    if (m_symlstExports != null) {
                        foreach (Symbol sym in m_symlstExports) {
                            Symbol sym2 = m_symbols.Find(sym.name);

                            if (sym2 == null) {
                                // M00BUG - Produce an error  - Symbol not defined
                            }
                            else if ((sym.symType & SymbolType.NameConflict) != 0) {
                                // M00BUG - Produce an error - Ambigious symbol export
                            }
                            else {
                                if ((sym.symType & SymbolType.Parameterized) != 0) {
                                    if ((sym2.symType & SymbolType.Parameterized) == 0) {
                                        // M00BUG - Produce an error - Exported symbol is not actually parameterized
                                    }
                                }

                                if ((sym.symType & SymbolType.NotYetDefined) != 0) {
                                    // M00BUG - Produce an error - Exported symbol definition is not completed
                                    // Should never get here as that would mean that a rule did not get reduced,
                                    // but it may come about later when we fix the way we keep and emit errors.
                                }

                                //
                                //  Not everthing should be exportable
                                //
                                //  M00TODO - Complete list
                                //
                                
                                switch (sym2.symType & SymbolType.BaseMask) {
                                default:
                                    break;
                                }
                            }
                        }
                    }
                    
                    //
                    //  We have finished parsing and are successful. 
                    //
                    //  Clean out all of the tokens and just leave the Module defintion token so that
                    //  later passes can do no work
                    //
            
                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ModuleDefinition, 0, 1));
                    cTokensUsed = 1;

                    //
                    //  Rule has no resulting value
                    //

                    $$ = null;
                } ;

ModuleHeader ::= ModuleIdentifier:a DEFINITIONS EncodingReferenceDefault ? TagDefault ? ExtensionDefault ? "::="
                {
                    //
                    //  Add the symbol to (1) the symbol table for this module,
                    //  (2) the list of all modules being compiled
                    //  (3) the list of all object identifiers compiled
                    //
                    //  We should always be able to add this symbol since this is the first 
                    //  operation done on the symbol table
                    //

                    m_symbols.Add($a);

                    //
                    //  Dump out the list of warnings for all to see
                    //  We lose the list of errors and warnings when we reduce the token --
                    //  however we should never have any errors to lose since to get here we
                    //  have declared success.
                    //

                    for (int i=0; i<history.Count; i++) {
                        if ((history[i].rv != null) && (history[i].rv.warnList != null)) {
                            history[i].rv.warnList.Print();
                        }
                    }

                    //
                    //  Only do the reduction on this rule once - 
                    //
                    //  Replace the tokens making up this rule with a single token for the rule
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ModuleHeader, 0, 0));
                    cTokensUsed = 1;

                    //
                    //  Rule has no resulting value
                    //

                    $$ = null;
                } ;

ModuleIdentifier ::= moduleReferenceLHS:a DefinitiveIdentifier:b ? 
                {
                    //
                    //  Put our name at the top so it can easily be found
                    //

                    m_strModuleName = $a.name;

                    //  
                    //  Create a symbol to be returned
                    //
                    
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Module));
                    
                    //
                    //  Set the value of the symbol to the OID passed in
                    //  It may not have a value in the event that no OID is specified
                    //

                    if ($b.result != MatchResult.Fail) {
                        $$.value = $b;
                        $$.type = Type.ObjectIdentifier;
                        
                        m_valModuleOID = $$.value;
                    }
                } ;

moduleReferenceLHS ::= U_Identifier;

moduleReference ::= U_Identifier:a ;

DefinitiveIdentifier ::= DefinitiveOID | DefinitiveOIDandIRI;

DefinitiveOID ::=  "{" DefinitiveObjIdComponentList:a "}"                 
                {
                    ErrorList   errs;
                    Value val = new Value($a);
                    MatchResult mr = val.PerformOIDChecks(m_rgTokens[$a.iTokenLast], out errs);

                    if (mr == MatchResult.Success) {
                        $$ = new RuleValue(val);
                        $a.rv.AddWarning(errs);
                    }
                    else {
                        $$ = new RuleValue(errs);
                        return mr;
                    }
                } ;

DefinitiveOIDandIRI ::= DefinitiveOID IRIValue;

DefinitiveObjIdComponentList ::= DefinitiveObjIdComponent:a Empty +             { $$ = new RuleValue(new ValueList($a)); } ;

DefinitiveObjIdComponent ::= DefinitiveNameAndNumberForm
                | NameForm 
                | DefinitiveNumberForm 
                ;

DefinitiveNumberForm ::= number:a
                {
                    //
                    //  Return the value
                    //
                    
                    $$ = new RuleValue(new Value(new OIDNode($a.number)));
                };

DefinitiveNameAndNumberForm ::= L_Identifier:a "(" DefinitiveNumberForm:b ")"
                {
                    //
                    //  Return as a named value
                    //
            
                    $$ = new RuleValue(new Value(new OIDNode($b.oidNode.iValue, $a.name)));
                    $$.name = $a.name;
                } ;

EncodingReferenceDefault ::= encodingreference INSTRUCTIONS;

encodingreference ::= L_Identifier;

TagDefault ::=  EXPLICIT TAGS
                {
                    m_tagTypeDefault = TagType.Explicit;
                    $$ = null;
                }
                | IMPLICIT TAGS 
                {
                    m_tagTypeDefault = TagType.Implicit;
                    $$ = null;
                }
                | AUTOMATIC TAGS
                {
                    m_tagTypeDefault = TagType.Implicit;
                    m_fUseAutomaticTagging = true;
                    $$ = null;
                } ;

ExtensionDefault ::= EXTENSIBILITY IMPLIED 
                {
                    m_fExtensibilityImplied = true;
                    $$ = null;
                } ;

ModuleBody ::= Exports Imports AssignmentList 
                {
                    //
                    //  Grab a corner case - you have a bad import but never use it.
                    //
                      
                    if (m_symlstImports != null) {
                        ProcessImportList();
                    }
                    
                    rvOut = null;
                };

Exports ::=     EXPORTS SymbolsExported:a ? ";" 
                {
                    //
                    //  Provide a list of symbols that are to be exported.
                    //

                    if ($a.result != MatchResult.Fail) {
                        m_symlstExports = $a;
                    }
                    else {
                        //
                        // Explicitly - no symbols are exported
                        //
                        
                        m_symlstExports = new SymbolList();
                    }
                    
                    ProcessExportList();
                    
                    //
                    //  nothing to return
                    //
                    
                    $$ = null;

                    //
                    //  Remove the tokens to make second passes faster
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.Exports, 0, 1));
                    cTokensUsed = 1;
                }
                / EXPORTS ALL ";"
                {
                    ProcessExportList();
                    
                    //
                    //  nothing to return
                    //
                    
                    $$ = null;

                    //
                    //  Remove the tokens to make second passes faster
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.Exports, 0, 1));
                    cTokensUsed = 1;
                }
                / ! EXPORTS
                    // M00BUG - Suggest that "EXPORTS ALL" be used instead
                 ;

SymbolsExported ::= SymbolList ;

Imports ::=     IMPORTS SymbolsImported:a
                {
                    MatchResult mr = MatchResult.Success;

                    //
                    //  nothing to return
                    //
                    
                    $$ = null;

                    //
                    //  Save the list of imports
                    //

                    mr = ProcessImportListPassOne($a, m_rgTokens[history[0].iToken], out rvOut);
                    m_fHasImports = true;

                    //
                    //  Remove the tokens to make second passes faster
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.Imports, 0, 1));
                    cTokensUsed = 1;

                    return mr;
                }
                / ! IMPORTS 
                    // M00TODO - Recommend use of "IMPORTS;"
                ;

SymbolsImported ::= SymbolsFromModuleList
                    | ";" { $$ = new RuleValue(new SymbolList()); }
                 ;

SymbolsFromModuleList ::= SymbolsFromModule:a SymbolsFromModuleList:b
                {
                    $$ = $b.rv;
                    $$.Insert(0, $a);           // Keep the order correct
                }
                / SymbolsFromModule:a  ";"
                {
                    $$ = new RuleValue(new SymbolList());
                    $$.Add($a);
                };

SymbolsFromModule ::= SymbolList:a FROM GlobalModuleReference:b
                {
                    //
                    //  Attach the symbol list to the module reference
                    //

                    $$ = $b.rv;
                    $b.symList = $a;
                } ;

GlobalModuleReference ::= moduleReferenceLHS:a AssignedIdentifier:b ?
                {
                    //
                    //  Create a module symbol for this name
                    //

                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Module | SymbolType.Imported));

                    //
                    //  Set the value of the symbol to the OID passed in
                    //  It may not have a value in the event that no OID is specified
                    //

                    if ($b.result != MatchResult.Fail) {
                        $$.value = $b;
                        $$.type = Type.ObjectIdentifier;
                    }
                };

AssignedIdentifier ::= ObjectIdentifierValue / DefinedValueLHS:a
                {
                    $$ = new RuleValue(new Value(new Symbol($a.name, SymbolType.Value)));
                };

//  Should have a ParameterizedValue, but I don't know under what circumstances this would acutally work
//  If there is a LHS value at this point, it cannot be followed by a comma as that would mean it is the first
//      symbol in a list on the next line.  It cannot be folled by a FROM as that would mean it was the sole
//      symbol on the next line
DefinedValueLHS ::= ExternalValueReferenceLHS:a ! "{" ! "," ! FROM
                {
                    $$ = $a.rv;
                }
                / valueReferenceLHS:a ! "{" ! "," ! FROM
                {
                    $$ = $a.rv;
                };

ExternalValueReferenceLHS ::= moduleReferenceLHS "." valueReferenceLHS 
                {
                    // M00BUG - no action created at the moment
                    $$ = new RuleValue(new Error(ErrorNumber.ICE));
                    return MatchResult.Fail;
                };


SymbolList ::= Symbol:a "," SymbolList:b
                {
                    $$ = $b.rv;
                    $$.Insert(0, $a);
                }
                / Symbol:a ! ","
                {
                    $$ = new RuleValue(new SymbolList($a));
                };

Symbol ::=      ReferenceLHS:a ! "{"
                {
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Unknown | SymbolType.Imported | SymbolType.NotYetDefined));
                }
                / ParameterizedReference ;

Reference ::= typereference | valueReference | objectclassreference | objectreference | objectsetreference ;

ReferenceLHS ::= U_Identifier | L_Identifier ;

AssignmentList ::= Assignment Empty + ;

Assignment ::=  Command
                | TypeAssignment
                | ValueAssignment
//              | XMLValueAssignment
                | ValueSetTypeAssignment
                | ObjectClassAssignment
                | ObjectAssignment
                | ObjectSetAssignment 
                | ParameterizedAssignment 
                ;

//
// SECTION 11.2,
//
//  Type references must start with an upper case character.

typereference ::= U_Identifier:a
                {
                    //
                    //  Must resolve the symbol in the symbol table
                    //  Must be a type symbol
                    //

                    Symbol sym = FindSymbol($a.name);
                    if (sym == null) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, $a));
                        return MatchResult.Fail;
                    }
                    else if (!sym.IsBaseType(SymbolType.Type)) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotType, $a));
                        return MatchResult.Fail;
                    }

                    $$ = new RuleValue(sym);
                } ;

typereferenceLHS ::= U_Identifier:a
                {
                    //
                    //  just pass up the token
                    //

                    $$ = $a.rv;
                } ;

//
// SECTION 11.4
//
valueReference ::= L_Identifier:a
                {
                    //
                    //  MUST be already a defined symbol
                    //  MUST be a value symbol

                    Symbol sym = FindSymbol($a.name);

                    if ((sym == null) || !sym.IsBaseType(SymbolType.Value)) FAIL;

                    //  pass up the symbol

                    $$ = new RuleValue(sym);
                } ;

valueReferenceLHS ::= L_Identifier:a
                {
                    //
                    //  Pass the token up.
                    //

                    $$ = $a.rv;
                } ;

//
// SECTION 13.1
//
// Parser for parameterized items first since they consume more tokens and thus lead to fewer retries
//

DefinedType ::= ParameterizedType
                / ParameterizedValueSetType
                / ExternalTypeReference:a ! "{"
                {
                    //  Convert the symbol into a type.
                    //

                    if ($a.type == null) {
                        $$ = new RuleValue(new Type($a));
                    }
                    else {
                        $$ = new RuleValue($a.type);
                    }
                }                    
                / typereference:a ! "{"
                {
                    //
                    //  This could be either a type or a parametrized type (w/o parameters)
                    //
                    //  Return the type of the symbol
                    //

                    if ($a.type == null) {
                        $$ = new RuleValue(new Type($a));
                    }
                    else {
                        $$ = new RuleValue($a.type);
                    }
                }
                ;

DefinedValue ::= ParameterizedValue
                / ExternalValueReference:a ! "{"
                {
                    //
                    //  return the symbol name wrapped as a value
                    //

                    $$ = new RuleValue(new Value($a));
                }
                / valueReference:a ! "{"
                {
                    //
                    //  return the symbol name wrapped as a value
                    //

                    $$ = new RuleValue(new Value($a));
                }
                ;
            
//   SECTION 13.2
//
//  NonParameterizedTypeName ::= ExternalTypeReference | typereference | xmlasn1typename ;
//                          for XML

//
// SECTION 13.6
//

ExternalTypeReference ::= moduleReference:a "." typereferenceLHS:b
                {
                    Symbol sym;

                    sym = GetExternalReference($a.name, $b.name);

                    if (sym == null) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, $b));
                        return MatchResult.Fail;
                    }

                    if ((sym.symType & SymbolType.BaseMask) != SymbolType.Type) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotType, $b));
                        return MatchResult.Fail;
                    }                        
                    
                    $$ = new RuleValue(sym);
                };

ExternalValueReference ::= moduleReference:a "." valueReferenceLHS:b
                {
                    Symbol sym;
                    
                    sym = GetExternalReference($a.name, $b.name);

                    if (sym == null) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, $b));
                        return MatchResult.Fail;
                    }
                    
                    if ((sym.symType & SymbolType.BaseMask) != SymbolType.Value) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotValue, $b));
                        return MatchResult.Fail;
                    }                        

                    $$ = new RuleValue(sym);
                };


//
//  SECTION 14.3
//
// AbsoluteReference ::= "@" ModuleIdentifier "." ItemSpec ;
// ItemSpec ::= typereference | ItemId "." ComponentId ;
// ItemId ::= ItemSpec ;
// ComponentId ::= identifier | number | "*" ;


//
// SECTION 15.1
//
TypeAssignment ::= typereferenceLHS:a "::=" ! CLASS
                {
                    //
                    //  Create the symbol and place it in the context stack so that
                    //  it can be found by later recursive references
                    //

                    if ($a.sym == null) {
                        history.sym = new Symbol($a.name, SymbolType.Type);
                    }
                    else {
                        history.sym = $a.sym;
                    }

                    //
                    //  We can take a short cut in some cases and know that this is
                    //  a type sooner than we might otherwise
                    //
                    //  If we are using end-of-line hinting - then if the name has
                    //  any lower case letters in it we can know that this is a type
                    //  reference.  If it is all upper-case letters then we cannot know
                    //  if it is a type or an class tag.  If we are not using end-of-line
                    //  hinting then we cannot know for sure that we are not looking
                    //  at the wrong location for a value definition
                    //

                    if (Program.args.fEOLHint && ($a.sym == null)) {
                        bool fLower = false;
                        foreach (char ch in $a.name) {
                            if (Char.IsLower(ch)) fLower = true;
                        }
                        if (fLower) {
                            history.sym.type = new Type(BaseType.Assignment);
                            history.sym.type.owner = history.sym;

                            if (m_symbols.Find(history.sym) != null) {
                                //  Error message?
                            }
                            else {
                                m_symbols.Add(history.sym);
                            }
                            
                            $a.sym = history.sym;
                        }
                    }
                        

                    $$ = null;
                }
                Type:b
                {
                    //
                    //  We have already done some setup work
                    //
                    
                    if ($a.sym != null) {
                        $a.sym.type.subType = $b;
                    }
                    else {
                        //
                        //  Set the type on the symbol ...
                        //

                        history.sym.type = new Type(BaseType.Assignment);
                        history.sym.type.subType = $b;

                        //
                        //  ... and the symbol on the type
                        //

                        history.sym.type.owner = history.sym;

                        //
                        //  Place the symbol in the symbol table
                        //

                        if (m_symbols.Find(history.sym) != null) {
                            LogError.Error("Duplicate symbol defined ", history.sym.name);
                        }
                        else {
                            m_symbols.Add(history.sym);
                        }
                    }

                    //
                    //  This is a textual root
                    //

                    $b.IsRootType = true;

                    //
                    //  No return value
                    //

                    $$ = null;

                    //
                    //  Collapse down to a single token since finished processing
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.TypeAssignment, 0, 0));
                    cTokensUsed = 1;
                }
                | typereferenceLHS:a "::=" ComplexType
                {
                    if ($a.sym == null) {
                        //  Put it into the symbol table as an undefined type.

                        Symbol sym = new Symbol($a.name, SymbolType.Type);

                        //
                        //  This is what everybody will point to - so set it up.
                        //

                        sym.type =  new Type(BaseType.Assignment);
                        sym.type.owner = sym;

                        //
                        //  Try and put it into the symbol table
                        //

                        if (m_symbols.Find(sym) != null) {
                            //  Create the error message
                        }
                        else {
                            m_symbols.Add(sym);
                        }

                        //
                        //  Put the symbol onto the token so we can find it later
                        //

                        $a.sym = sym;
                    }
                    FAIL;
                }
                ;

ComplexType ::= SEQUENCE | SET | CHOICE | Tag ;


//
//  SECTION 15.2
//
ValueAssignment ::= valueReferenceLHS:a Type:b "::="
                {
                    //
                    //  Place the type on the context stack so we know what we are looking for
                    //  when doing evaluations.
                    //  We don't care about the name of the value reference - since we can't circular reference
                    //

                    if ($b.SkipFluff() == null) {
                        //
                        //  We have not yet completed this type definition - we can't do this as well.
                        //
                        
                        $$ = null;
                        return MatchResult.Fail;
                    }
                    
                    history.type = $b;
                    $$ = null;
                }
                Value:c
                {
                    //
                    //  Create the symbol and set the type and value on it
                    //

                    Symbol sym = new Symbol($a.name, SymbolType.Value);
                    sym.type = $b;
                    sym.value = $c;

                    //
                    //  Insert the symbol in the symbol table
                    //
                    //
                    //  Place the symbol in the symbol table
                    //

                    if (m_symbols.Find(sym) != null) {
                        LogError.Error("Duplicate symbol defined ", history.sym.name);
                    }
                    else {
                        m_symbols.Add(sym);
                    }

                    //
                    //  If this is an Object Identifier - put the symbol at the appropriate point in the OID tree
                    //
                    
                    if (($c.type == Type.ObjectIdentifier) && ($c.valueList != null)) {
                        OIDNode node = $c.valueList[$c.valueList.Count-1].oidNode;
                        if (node.symbol == null) node.symbol = sym;
                    }

                    //
                    //  No return value
                    //

                    $$ = null;

                    //
                    //  Collapse down to a single token since finished processing
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ValueAssignment, 0, 0));
                    cTokensUsed = 1;
                }
                ;

// XMLValueAssignment ::= valueReference "::=" XMLTypedValue ;
// XMLTypedValue ::= "<" & NonParameterizedTypeName ">" XMLValue "</" & NonParameterizedTypeName ">" | "<" & NonParameterizedTypeName "/>"

//  SECTION 15.6
//

ValueSetTypeAssignment ::= typereferenceLHS:a Type:b "::=" 
                {
                    //
                    //  Put the type on the stack so that we can use it in choosing values
                    //

                    history.type = $b;
                    $$ = null;
                }
                ValueSet:c
                {
                    //
                    //  Create a new Type symbol and attach the type
                    //

                    Symbol sym = new Symbol($a.name, SymbolType.Type);
                    Type   type = $b;
                    sym.type = new Type(BaseType.Assignment);

                    //
                    //  Now attach the value set as a constraint to the type
                    //

                    if (type.readOnly) {
                        Type typ2 = new Type(BaseType.Constraint);
                        typ2.subType = type;
                        type = typ2;
                    }
                    
                    type.AddConstraint($c);

                    sym.type.subType = type;

                    //
                    //  now lets put the symbol into the symbol table
                    //

                    if (m_symbols.Find(sym) != null) {
                        // Error;
                    }
                    else {
                        m_symbols.Add(sym);
                    }

                    //
                    //  No return value
                    //

                    $$ = null;

                    //
                    //  Collapse down to a single token
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ValueSetTypeAssignment, 0, 0));
                    cTokensUsed = 1;
                };

//  SECTION 15.7
//
ValueSet ::= "{" ElementSetSpecs:a "}" { $$ = $a.rv; };

//  SECTION 16.1
//
//  Type ::= BuiltinType | ReferencedType | ConstrainedType ;
//  ConstrainedType ::= Type Constraint | TypeWithConstraint ;
//
//  We have combined this rule with that of ConstrainedType for a new pair of rules
//
//  If we don't then we have a left recursion problem 
//

Type ::=        PrimitiveType:a
                {
                    //
                    //  Push the type onto the context stack so that we can use it
                    //  in evaluating constraints
                    //

                    history.type = $a;
                    $$ = null;
                }
                ConstraintListOpt:b
                {
                    if ($b.rv != null) {
                        $$ = new RuleValue(new Type(BaseType.Constraint));
                        $$.subType = $a;
                        $$.constraintList = $b;
                    }
                    else {
                        $$ = $a.rv;
                    }
                }
                | TypeWithConstraint
                ;

ConstraintListOpt ::= ConstraintList
                / ! "("
                {
                    $$ = null;
                    return MatchResult.Success;
                }
                ;

ConstraintList ::=  Constraint:a ConstraintList:b
                {
                    $$ = $b.rv;
                    $b.Insert(0, $a);
                }
                / Constraint:a ! "("
                {
                    $$ = new RuleValue(new ConstraintList($a));
                }
                ;

PrimitiveType ::= BuiltinType / ReferencedType ;


//
//  SECTION 16.2
//
BuiltinType ::= BitStringType / BooleanType / CharacterStringType / ChoiceType / DateType
                / DateTimeType / DurationType / EmbeddedPDVType
                / EnumeratedType / ExternalType
                / InstanceOfType
                / IntegerType / IRIType
                / NullType
                / ObjectClassFieldType
                / ObjectIdentifierType
                / OctetStringType / RealType / RelativeIRIType / RelativeOIDType / SequenceType
                / SequenceOfType
                / SetType  / SetOfType / PrefixedType / TimeType / TimeOfDayType ;

//
//  SECTION 16.3
//
ReferencedType ::= DefinedType / UsefulType / SelectionType
                / TypeFromObject
                / ValueSetFromObjects
                ;

//
// SECTION 16.5
//

NamedType ::=   L_Identifier:a Type:b
                {
                    //
                    //  Return the name and type as a field symbol
                    //

                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Component));
                    $$.type = $b;
                }
                ;

//
// SECTION 16.7
//

Value ::= BuiltinValue | ReferencedValue | ObjectClassFieldValue
                ;

// XMLValue ::= XMLBuiltinValue | XMLObjectClassFieldValue ;

//
// SECTION 16.9
//

BuiltinValue ::= BitStringValue | BooleanValue | CharacterStringValue | ChoiceValue 
                | EmbeddedPDVValue
                | EnumeratedValue | ExternalValue
                | InstanceOfValue
                | IntegerValue | IRIValue | NullValue | ObjectIdentifierValue
                | OctetStringValue | RealValue | RelativeIRIValue | RelativeOIDValue | SequenceValue | SequenceOfValue | SetValue
                | SetOfValue | PrefixedValue | TimeValue ;

// XMLBuiltinValue ::= 

//
// SECTION 16.11
//

ReferencedValue ::= DefinedValue 
                | ValueFromObject
                ;

// 
// SECTION 16.13
//
NamedValue ::=  L_Identifier:a
                {
                    //
                    //  Locate the symbol name and determine its type.
                    //  We then put the type onto the context in order to 
                    //  allow the value evaluation to know what it is looking for
                    //

                    Type type = m_context.GetTopType();
                    bool f = true;

                    while (f) {
                        switch (type.baseType) {
                        case BaseType.Assignment:
                        case BaseType.Param_Assignment:
                        case BaseType.Tagged:
                            type = type.subType;
                            break;

                        case BaseType.Sequence:
                        case BaseType.Set:
                        case BaseType.SequenceOf:
                        case BaseType.SetOf:
                            f = false;
                            break;

                        default:
                            Debug.Assert(false);
                            FAIL;
                        }
                    }

                    foreach (Symbol fld1 in type.fieldList.list) {
                        if (fld1.name == $a.name) {
                            history.type = fld1.type;
                            break;
                        }
                    }

                    if (history.type == null) {
                        LogError.Error("No field found here", history[0].rv.token.name);
                        FAIL;
                    }

                    $$ = null;
                }
                Value:c
                {
                    //
                    //  Return the actual value
                    //

                    $$ = $c.rv;
                    $$.name = $a.name;
                }
                ;

// XMLNamedValue ::= "<" & identifier ">" XMLValue "</" & identifier ">" ;

//
// SECTION 17.1
//
BooleanType ::=     BOOLEAN                     { $$ = new RuleValue(Type.BooleanType); };

//
// SECTION 17.3
//
BooleanValue ::= TRUE                           { $$ = new RuleValue(new Value(true)); }
                | FALSE                         { $$ = new RuleValue(new Value(false)); }
                ;

// XMLBooleanValue ::= "<" & "true" "/>" | "<" & "false" "/>" ;

//
//  SECTION 18.1
//
//  IntegerType ::= INTEGER | INTEGER "{" NamedNumberList "}"
//  NamedNumberList ::= NamedNumber "," +

IntegerType ::= INTEGER NamedNumberList:a
                {
                    //
                    //  If there was a named number list, then attach it
                    //

                    $$ = new RuleValue(new Type(BaseType.NamedList));
                    $$.subType = m_DefaultIntegerType;
                    $$.fieldList = $a;
                }
                / INTEGER ! "{"                                 // "}"
                {
                    $$ = new RuleValue(m_DefaultIntegerType);
                }
                ;

NamedNumberList ::= "{" 
                {
                    //
                    //  All values are of type integer
                    //
                    
                    history.type = m_DefaultIntegerType;

                    $$ = null;
                }
                NamedNumber "," + "}"
                {
                    //
                    //  Build the list of the symbols
                    //

                    $$ = new RuleValue(new SymbolList());
                    for (int i=3; i<history.Count-1; i+=2) {
                        $$.Add(history[i].rv.sym);
                    }

                    //
                    //  No two symbols are allowed to have the same name
                    //  No two symbols are allowed to have the same value
                    //

                    for (int i=0; i<$$.Count; i++) {
                        for (int j=i+1; j<$$.Count; j++) {
                            if ($$[i].name == $$[j].name) {
                                LogError.Error("Can't have two names in number list with same name", $$[i].name);
                            }

                            if ($$[i].value == $$[j].value) {
                                LogError.Error("Can't have two names in the number list with the same value", $$[i].name);
                            }
                        }
                    }
                };

NamedNumber ::= L_Identifier:a "(" SignedNumber:b ")"
                {
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Value | SymbolType.InList));
                    $$.type = m_DefaultIntegerType;
                    $$.value = $b;
                }
                | L_Identifier:a "(" DefinedValue:b ")"
                {
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Value | SymbolType.InList));
                    $$.type = m_DefaultIntegerType;
                    $$.value = $b;
                }
                ;

SignedNumber ::= number:a
                {
                    $$ = new RuleValue(new Value($a.number, m_DefaultIntegerType));
                }
                | "-" number:a
                {
                    $$ = new RuleValue(new Value(-$a.number, m_DefaultIntegerType));
                }
                ;

//
// SECTION 18.9
//

IntegerValue ::= SignedNumber
                | L_Identifier:a
                {
                    //
                    // Symbol needs to be an integer value
                    //

                    Symbol sym = FindSymbol($a.name);
                    if (sym == null) FAIL;

                    // M00BUG - does not do a type check

                    if (sym.IsType(SymbolType.Value)) {
                        Debug.Assert(sym.value != null);
                        $$ = new RuleValue(new Value(sym));
                    }
                    else if (sym.IsType(SymbolType.ValueParameter)) {
                        $$ = new RuleValue(new Value(sym));
                    }
                    else FAIL;

                };

// XMLIntegerValue ::= SignedNumber | "<" & identifier "/>" ;

//
// SECTION 19.1
//
EnumeratedType ::= ENUMERATED
                {
                    //   All values are integer values - so let the system know

                    history.type = Type.IntegerType;
                    $$ = null;
                }
                "{" Enumerations:a "}"
                {
                    Type type = null;
                    int i;
                    
                    //
                    //  Create and return the type with the enumeration values attached
                    //
                            
                    $$ = new RuleValue(new Type(BaseType.NamedList));
                    $$.subType = m_DefaultEnumeratedType;
                    $$.fieldList = $a;
                }
                ;

// Enumerations ::= RootEnumeration
//                | RootEnumeration "," "..." ExceptionSpec?
//                | RootEnumeration "," "..." ExceptionSpec? "," AdditionalEnumeration 

Enumerations ::= RootEnumeration:a  EnumExtension:b ?
                {
                    long iValue = 0;
                    Dictionary<Int64, Symbol> map = new Dictionary<Int64,Symbol>();
                    Dictionary<String, Int32> map2 = new Dictionary<String, Int32>();
                    SymbolList symList = new SymbolList();
                    bool f;

                    //
                    //  Walk the root list - restrictions are:
                    //    1. no two items can have the same name
                    //    2. no two items can have the same value
                    //
                    //  Items without values are are to be assigned starting w/ 0
                    //    and incrementing - skiping anything already assigned.
                    //
                    //  We walk the list twice
                    //     First, we look for all symbols which have values.
                    //

                    foreach (Symbol sym in $a) {
                        //
                        // Skip symbols w/o values
                        if (sym.value == null) {
                            continue;
                        }

                        //
                        //  Insert into the name map
                        //

                        try {
                            map2.Add(sym.name, 0);
                        }
                        catch (ArgumentException) {
                            $a.rv.AddWarning(new Error(ErrorNumber.EnumNameTwice, m_rgTokens[$a.iTokenLast], sym.name));
                        }

                        //
                        // Insert into the value map
                        //

                        try {
                            Value val = sym.value;
                            bool f1 = true;
                            Int64 i = 0;

                            while (f1) {
                                switch (val.valueType) {
                                case ValueType.Integer:
                                    f1 = false;
                                    i = val.iVal;
                                    break;

                                case ValueType.Symbol:
                                    Debug.Assert(val.sym.GetSymbolType() == SymbolType.Value);
                                    val = val.sym.value;
                                    break;

                                default:
                                    Debug.Assert(false);
                                    f1 = false;
                                    break;
                                }
                            }

                            map.Add(i, sym);
                        }
                        catch (ArgumentException) {
                            $a.rv.AddWarning(new Error(ErrorNumber.EnumValueTwice, m_rgTokens[$a.iTokenLast], sym.name));
                        }

                        //
                        //  Place on the output list
                        //
                        
                        symList.Add(sym);
                        sym.m_symTable = m_symbols;        //  M00QUEST - Do we 
                    }

                    //
                    //  Second pass, process those items w/o values
                    //

                    foreach (Symbol sym in $a) {
                        //
                        //  Skip symbols w/ values
                        //

                        if (sym.value != null) {
                            continue;
                        }

                        //
                        //  Insert into the name map
                        //

                        try {
                            map2.Add(sym.name, 0);
                        }
                        catch (ArgumentException) {
                            $a.rv.AddWarning(new Error(ErrorNumber.EnumNameTwice, m_rgTokens[$a.iTokenLast], sym.name));
                        }

                        //
                        //  Find the next unused value
                        //

                        for (f = true; f; iValue += 1) {
                            try {
                                Symbol symT = map[iValue];
                            }
                            catch (KeyNotFoundException) {
                                sym.value = new Value(iValue, Type.IntegerType);
                                f = false;
                            }
                        }

                        //
                        // Place on the output list
                        //

                        symList.Add(sym);
                        sym.m_symTable = m_symbols;        //  M00QUEST - Do we 
                    }

                    //
                    //  Now process the extension list if present
                    //
                    //  The restrictions are:
                    //     1. no two items of same name in entire list
                    //     2. no two items of same value in entire list
                    //     3. values are strictly ascending inside of the extension,
                    //          but can interweave with values in the root
                    //
                            

                    if ($b.result != MatchResult.Fail) {
                        Debug.Assert($b.rv != null);

                        foreach (Symbol sym in $b) {
                            //
                            //  We don't do any processing for Exception and Extension markers
                            //

                            if (sym.IsType(SymbolType.Extension) || sym.IsType(SymbolType.Exception)) {
                                symList.Add(sym);
                                continue;
                            }
                            
                            //
                            //  Insert symbol into the name map
                            //

                            try {
                                map2.Add(sym.name, 0);
                            }
                            catch (ArgumentException) {
                                $b.rv.AddWarning(new Error(ErrorNumber.EnumNameTwice, m_rgTokens[$b.iTokenLast], sym.name));
                            }

                            //
                            // Process value based code
                            //

                            if (sym.value != null) {
                                //
                                // If the symbol has a value it must be
                                //      1. greater than anyy default value assigned and
                                //      2. shal not be in use.
                                //

                                try {
                                    map.Add(sym.value.iVal, sym);
                                    
                                    if (sym.value.iVal < iValue) {
                                        $b.rv.AddWarning(new Error(ErrorNumber.EnumExtValue, m_rgTokens[$b.iTokenLast], sym.name));
                                    }
                                    else {
                                        iValue = sym.value.iVal;
                                    }
                                }
                                catch (ArgumentException) {
                                    $b.rv.AddWarning(new Error(ErrorNumber.EnumValueTwice, m_rgTokens[$b.iTokenLast], sym.name));
                                }
                            }
                            //
                            //  Using a default value
                            //
                            
                            else {
                                //
                                //  Find the next unused value
                                //

                                for (f = true; f; iValue += 1) {
                                    try {
                                        Symbol symT = map[iValue];
                                    }
                                    catch (KeyNotFoundException) {
                                        sym.value = new Value(iValue, Type.IntegerType);
                                        f = false;
                                    }
                                }
                            }

                            //
                            //  Plance the symbol on the output list
                            //

                            symList.Add(sym);
                        }
                    }

                    $$ = new RuleValue(symList);
                }
                ;

EnumExtension ::= "," "." _ "." _ "." ExceptionSpec:a ? AdditionalEnumeration:b ?
                {
                    $$ = new RuleValue(new SymbolList());
                    $$.Add(new Symbol(SymbolType.Extension));
                    
                    //
                    //  If there is an ExceptionSpec it is returned as a symbol
                    //

                    if ($a.result != MatchResult.Fail) {
                        $$.Add($a);
                    }
                    
                    //
                    //  If there is an Additional Enumeration return it as the value.
                    //

                    if ($b.result != MatchResult.Fail) {
                        $$.AddRange($b);
                    }
                };

RootEnumeration ::= Enumeration ;

AdditionalEnumeration ::= "," Enumeration:a { $$ = $a.rv; } ;

Enumeration ::= EnumerationItem "," + 
                {
                    //
                    //  Build and return the list of symbols
                    //

                    $$ = new RuleValue(new SymbolList());
                    for (int i=1; i<history.Count; i+= 2) {
                        $$.Add(history[i].rv.sym);
                    }
                };

EnumerationItem ::= NamedNumber 
                | L_Identifier:a
                {
                    //
                    //  Create a new value symbol, but we don't know what the value will be until 
                    //  we move do the final evaluation of the type.
                    //

                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Value | SymbolType.InList));
                    $$.type = Type.IntegerType;
                };


//
//  SECTION 19.8
//
EnumeratedValue ::= L_Identifier:a
                {
                    //  We only allow for symbols to be from the enumeration itself.

                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || (sym.type != Type.EnumeratedType)) FAIL;   // M00BUG - find a better way to restrict the symbol search!!!
                    
                    $$ = new RuleValue(sym);
                };

// XMLEnumeratedValue ::= "<" & identifier "/>" ;

//  SECTION 20.1
//
RealType ::=    REAL   { $$ = new RuleValue(new Type(BaseType.Real)); };

//  SECTION 20.5
//
//  SEQUENCE {  mantissa INTEGER, 
//              base INTEGER (2|10),
//              exponent INTEGER
//              -- The associated mathematical real number is "mantissa"
//              -- multiplied by "base" raised to the power "exponent"
//          }

//  SECTION 20.6
//
RealValue ::= NumericRealValue | SpecialRealValue ;
NumericRealValue ::= realnumber:a
                {
                    $$ = new RuleValue(new Value($a.real, new Type(BaseType.Real)));
                }
                | "-" realnumber:a
                {
                    $$ = new RuleValue(new Value(-$a.real, new Type(BaseType.Real)));
                }
                |
                { // need to push the sequence on the stack here
                    $$ = null;
                }
                SequenceValue 
                {
                    Debug.Assert(false, "NYI");
                    //  Convert from sequence to real number via m * b ^ e
                    $$ = new RuleValue(new Value(0, new Type(BaseType.Real)));
                }
                ;

SpecialRealValue ::= PLUS-INFINITY                              { $$ = new RuleValue(new Value(ValueType.PlusInfinity)); }
                | MINUS-INFINITY                                { $$ = new RuleValue(new Value(ValueType.MinusInfinity)); Debug.Assert(false); }
                | NOT-A-NUMBER                                  { $$ = new RuleValue(new Value(ValueType.NotANumber)); Debug.Assert(false); }
                ;

// XMLRealValue ::= XMLNumericRealValue | XMLSpecialRealValue ;
// XMLNumericRealValue ::= realnumber | "-" realnumber ;
// XMLSpecialRealValue ::= "<" & PLUS-INFINITY "/>" | "<" & MINUS-INFINITY "/>" ;

//  SECTION 21.1
//
//  BitStringType ::= BIT STRING | BIT STRING "{" NamedBitList "}
//  NamedBitList ::= NamedBit "," +

BitStringType ::= BIT STRING NamedBitList:a
                {
                    //
                    //  Base type is a bit string
                    //

                    $$ = new RuleValue(new Type(BaseType.NamedList));
                    $$.subType = Type.BitStringNamed;
                    $$.fieldList = $a;
                }
                / BIT STRING ! "{"              // }
                {
                    $$ = new RuleValue(Type.BitStringType);
                }
                ;

NamedBitList ::=  "{" 
                {
                    //
                    //  All values are of type integer
                    //

                    history.type = Type.IntegerNative;

                    $$ = null;
                }
                NamedBit "," + "}"
                {
                    //
                    //  Build the list of the symbols
                    //

                    $$ = new RuleValue(new SymbolList());
                    for (int i=3; i<history.Count-1; i+=2) {
                        $$.Add(history[i].rv.sym);
                    }

                    //
                    //  No two symbols are allowed to have the same name
                    //  No two symbols are allowed to have the same value
                    //

                    for (int i=0; i<$$.Count-2; i++) {
                        for (int j=i+1; j<$$.Count; j++) {
                            if ($$[i].name == $$[j].name) {
                                LogError.Error("Can't have two names in number list with same name", $$[i].name);
                            }

                            if ($$[i].value == $$[j].value) {
                                LogError.Error("Can't have two names in the number list with the same value", $$[i].name);
                            }

                            if ($$[i].value.iVal < 0) {
                                // ICE
                            }
                        }
                    }
                };

NamedBit ::=    L_Identifier:a "(" number:b ")"
                {
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Value));
                    $$.type = Type.IntegerNative;
                    $$.value = new Value($b.number, Type.IntegerNative);
                }
                | L_Identifier:a "(" DefinedValue:b ")"
                {
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Value));
                    $$.type = Type.IntegerNative;
                    if ($b.valueType != ValueType.Symbol) {
                        // ICE
                        $$.value = new Value(0);
                    }
                    else {
                        Symbol sym = $b.sym;
#if false
                        while (sym.GetSymbolType() == SymbolType.Symbol) {
                            sym = sym.m_sym;
                        }
#endif

                        if (sym.GetSymbolType() != SymbolType.Value) {
                            // ICE
                            $$.value = new Value(0);
                        }

                        if (sym.value.valueType != ValueType.Integer) {
                            // ICE
                            $$.value = new Value(0);
                        }
                        
                        $$.value = sym.value;
                    }
                };

//  SECTION 21.9
//
BitStringValue ::= bstring:a
                {
                    $$ = new RuleValue(new Value(Binary.Convert($a.name)));
                    $$.type = Type.BitStringType;
                }
                | hstring:a
                {
                    $$ = new RuleValue(new Value(Binary.Convert($a.name)));
                    $$.type = Type.BitStringType;
                }
                | "{" IdentifierList:a ? "}"
                {
                    //
                    //  Items refer to bit numbers, convert this from bit numbers to a bit string
                    //

                    $$ = new RuleValue(new Value(new Binary()));
                    $$.type = Type.BitStringType;

                    if ($a.result != MatchResult.Fail) {
                        foreach (Symbol sym in $a) {
                            $$.bin.SetBit((int) sym.value.iVal);
                        }
                    }
                }
                | CONTAINING Value ;

IdentifierList ::= L_Identifier:a "," + 
                {
                    //
                    //  Turn this into a symbol list 
                    //

                    $$ = new RuleValue(new SymbolList());
                    
                    foreach (Token tkn in $a) {
                        //
                        //  Need to restrict to the symbols in the bit list
                        //
                        
                        Symbol sym = FindSymbol(tkn.name);
                        if (sym == null) {
                            ; //  Error;
                        }
                        else {
                            sym.Collapse();
                            $$.Add(sym);
                        }
                    }
                };

// XMLBitStringValue ::= XMLTypedValue | Xmlbstring | XMLIdentifierList | Empty ;
// XMLIdentifierList ::= "<" & identifier "/>" | XMLIdentifierList "<" & identifier "/>" ;

//
//  SECTION 22.1
//

OctetStringType ::= OCTET STRING { $$ = new RuleValue(Type.OctetString); };

//  SECTION 22.3
//

OctetStringValue ::= bstring:a
                {
                    $$ = new RuleValue(new Value(Binary.Convert($a.name)));
                    $$.type = Type.OctetString;
                }
                | hstring:a
                {
                    $$ = new RuleValue(new Value(Binary.Convert($a.name)));
                    $$.type = Type.OctetString;
                }
                | CONTAINING Value
                ;


// XMLOctetStringValue ::= XMLTypedValue | xmlhstring ;
//  SECTION 23.1
//
NullType ::= NULL           { $$ = new RuleValue(Type.Null); } ;

//  SECTION 23.2
//
NullValue ::= NULL              { $$ = new RuleValue(Value.ValueNull); };

// XMLNullValue ::= Empty ;

//
// SECTION 24.1
//
// SequenceType ::= SEQUENCE "{" "}" | SEQUENCE "{" ExtensionAndException OptionalExtensionMarker "}" | SEQUENCE "{" ComponentTypeLists "}"
// ExtensionAndException ::= "..." | "..." ExceptionSpec
// OptionalExtensionMarker ::= "," "..." | empty
// ComponentTypeLists ::= RootComponentTypeList 
//              | RootComponentTypeList "," ExtensionAndException ExtensionAdditions OptionalExtensionMarker 
//              | RootComponentTypeList "," ExtensionAndException ExtensionAdditions ExtensionEndMarker "," RootComponentTypeList
//              | ExtensionAndException ExtensionAdditions ExtensionEndMarker "," RootComponentTypeList
//              | ExtensionAndException ExtensionAdditions OptionalExtensionMarker
// RootComponentTypeList ::= ComponentTypeList
// ExtensionEndMarker ::= "," "..."
// ExtensionAdditions ::= "," ExtensionAdditionList | empty
// ExtensionAdditionList ::= ExtensionAddition "," +
// ExtensionAddition ::= ComponentType | ExtensionAdditionGroup
// ExtensionAdditionGroup ::= "[[" VersionNumber ComponentTypeList "]]"
// VersionNumber ::= empty | number ":"
// ComponentTypeList ::= ComponentType "," +
// ComponentType ::= NamedType | NamedType OPTIONAL | NamedType DEFAULT Value | COMPONENTS OF Type
//
// SequenceType ::= SEQUENCE "{" ComponentTypeLists ? "}"
// ComponentTypeLists ::= RootComponentTypeList ("," "..." ExceptionSpec? ("," ExtensionAdditionList)? ("," "..." ( "," RootComponentTypeList)?)?)?
//                      |                            "..." ExceptionSpec? ("," ExtensionAdditionList)? ("," "..." ( "," RootComponentTypeList)?)?
//

// ComponentTypeLists ::= 
//      RootComponentTypeList 
//    | RootComponentTypeList "," ExtensionAndException ExtensionAdditions OptionalExtensionMarker 
//    | RootComponentTypeList "," ExtensionAndException ExtensionAdditions ExtensionEndMarker "," RootComponentTypeList
//    |                           ExtensionAndException ExtensionAdditions ExtensionEndMarker "," RootComponentTypeList
//    |                           ExtensionAndException ExtensionAdditions OptionalExtensionMarker

SequenceType ::= SEQUENCE "{" ComponentTypeLists:a ? "}"
                {
                    bool fApplyAutoTagging = false;
                    ErrorList errs;
                    
                    //
                    //  Create a new type and, if necessary, add the field list
                    //
                    
                    $$ = new RuleValue(new Type(BaseType.Sequence));
                    if ($a.result != MatchResult.Fail) {
                        $$.fieldList = $a;
                    }
                    else {
                        $$.fieldList = new SymbolList();
                    }

                    CheckTagging($a, true, out errs);
                    
                }
                ;

ComponentTypeLists ::= RootComponentTypeList:a R3:b ?
                {
                    $$ = $a.rv;

                    if ($b.result != MatchResult.Fail) {
                        $$.AddRange($b);
                    }
                }
                | ExtensionAndException:a ExtensionAdditions:b ? R0:c
                {
                    $$ = $a.rv;

                    if ($b.result != MatchResult.Fail) {
                        $$.AddRange($b);
                    }

                    $$.AddRange($c);
                }
                ;

R0 ::=          R2 | R1 ;

R1 ::=          OptionalExtensionMarker:a ? 
                {
                    $$ = new RuleValue(new SymbolList());

                    if ($a.result != MatchResult.Fail) {
                        $$.Add($a);
                    }
                    else {
                        $$.Add(new Symbol(SymbolType.ExtensionEnd));
                    }
                };

R2 ::=          ExtensionEndMarker:a "," RootComponentTypeList:b
                {
                    $$ = new RuleValue(new SymbolList());

                    $$.Add($a);
                    $$.AddRange($b);
                };

R3 ::=          "," ExtensionAndException:a ExtensionAdditions:b ? R0:c 
                {
                    $$ = new RuleValue(new SymbolList());

                    $$.AddRange($a);
                    
                    if ($b.result != MatchResult.Fail) {
                        $$.AddRange($b);
                    }

                    $$.AddRange($c);
                };

ExtensionAndException ::= "." _ "." _ "." ExceptionSpec:a ?
                {
                    $$ = new RuleValue(new SymbolList());

                    $$.Add(new Symbol(SymbolType.Extension));

                    if ($a.result != MatchResult.Fail) {
                        $$.Add($a);
                    }
                };

OptionalExtensionMarker ::= "," "." _ "." _ "."
                {
                    $$ = new RuleValue(new Symbol(SymbolType.ExtensionEnd));
                };

RootComponentTypeList ::= ComponentTypeList ;

ExtensionEndMarker ::= "," "." _ "." _ "." 
                {
                    $$ = new RuleValue(new Symbol(SymbolType.ExtensionEnd));
                };

ExtensionAdditions ::= "," ExtensionAdditionList:a                              { $$ = $a.rv; };

ExtensionAdditionList ::= ExtensionAddition:a "," ExtensionAdditionList:b
                {
                    $$ = $b.rv;

                    $$.Insert(0, $a);
                }
                / ExtensionAddition:a
                {
                    $$ = new RuleValue(new SymbolList($a));
                };

ExtensionAddition ::= ExtensionAdditionGroup / ComponentType ;

ExtensionAdditionGroup ::= "[" _ "[" VersionNumber:a ? ComponentTypeList:b "]" _ "]"
                {
                    $$ = new RuleValue(new SymbolList());
                    Symbol sym = new Symbol(SymbolType.ExtensionGroup);
                    $$.Add(sym);

                    if ($a.result != MatchResult.Fail) {
                        sym.groupVersion = (int) $a.iVal;
                    }
                    else {
                        sym.groupVersion = -1;
                    }
                    sym.symList = $b;
                };


VersionNumber ::= number:a ":"
                {
                    $$ = new RuleValue(new Value($a.number, Type.IntegerType));
                };

ComponentTypeList ::= ComponentType:a "," 
                {
                    //
                    //  We know that we have successfully reduced the component type $a, we want to 
                    //  do something to make this more effecent at this point.
                    //
                    //  We are going to do two things
                    //  1.  We are going to change it from a ChoiceLeft result to a Success result - there is 
                    //          no reason to go back into it and keep looking
                    //  2.  We are going to force a cache of the result so that when we come back and parse
                    //          it again, it will parse really fast.
                    //

                    if ($a.result == MatchResult.ChoiceLeft) {
                        $a.result = MatchResult.Success;
                    }
                    m_rgTokens[$a.iToken].SetResult(RuleNo.ComponentType, -1, new Results(MatchResult.Success, $a.rv, $a.cTokens));

                    //
                    //  This rule has no result and always succeeds
                    //
                    
                    $$ = null;
                }
                ComponentTypeList:b
                {
                    $$ = new RuleValue(new SymbolList($b));
                    $$.Insert(0, $a);
                }
                / ComponentType:a
                {
                    $$ = new RuleValue($a);
                }
                ;


ComponentType ::= NamedType:a
                {
                    //
                    //  Push the type of the field onto the stack for processing of DEFAULTs
                    //
                    
                    history.type = $a.type;
                    $$ = null;
                } 
                ComponentTypeOption:b
                {
                    if ($b.result != MatchResult.Fail) {
                        $a.flags = $b;
                    }
                    $$ = new RuleValue(new SymbolList($a));
                }
                / COMPONENTS OF Type:a
                {
                    //
                    //  1.  The type must be a sequence
                    //  2.  Create a new symbol list 
                    //  3.  Add each of the fields in the old item until you
                    //      get to the first extension marker.
                    //

                    $$ = new RuleValue(new SymbolList());
                    
                    if ($a.IsSequence()) {
                        Boolean f = true;
                        SymbolList lst = $a.fieldList;

                        foreach (Symbol sym in lst) {
                            if (sym.IsType(SymbolType.Extension)) f = false;
                            if (f) {
                                Symbol symT = new Symbol(sym);
                                symT.FromComponentOf = true;
                                if ((symT.type.baseType == BaseType.Tagged) &&
                                    (symT.type.tagged.tagType == TagType.Automatic)) {
                                    symT.type = symT.type.subType;
                                }
                                $$.Add(symT);
                            }
                        }
                    }
                    else {
                        Debug.Assert(false);
                        ; // Error
                    }
                };

ComponentTypeOption ::= OPTIONAL
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.Optional = true;
                }
                / DEFAULT Value:a
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.SetDefault($a);
                }
                / ! OPTIONAL ! DEFAULT 
                {
                    $$ = new RuleValue(new FieldFlags());
                };

//
//  SECTION 24.17
//
SequenceValue ::= "{" "}":c
                {
                    Type type = m_context.GetTopType();
                    
                    //  Requires that all fields are either defaulted or optional or
                    //  that there are no fields

                    if (type.fieldList.Count == 0) {
                        //
                        //  This is an ok initializer
                        //
                        //  Create an empty value list and set the value to that
                        //

                        $$ = new RuleValue(new Value(new ValueList()));
                        $$.type = type;
                    }
                    else {
                        //
                        //  Check to see if all of the required fields are present
                        //
                        
                        $$ = CheckSequenceFields(new ValueList(), $c.token);
                    }
                    

                }
                / "{" ComponentValueList:a "}":c
                {
                    //
                    //  Sort the fields into order and check that all are present
                    //

                    $$ = CheckSequenceFields($a, $c.token);
                }
                ;

ComponentValueList ::= NamedValue:a "," +
                {
                    //
                    //  Create a list of the values and return it
                    //

                    $$ = new RuleValue(new ValueList($a));
                }
                ;

// XMLSequenceValue ::= XMLComponentValueList | Empty ;
// XMLComponentValueList ::= XMLNamedValue | XMLComponentValueList XMLNamedValue ;

//  SECTION 25.1
//

SequenceOfType ::= SEQUENCE OF Type:a
                {
                    $$ = new RuleValue(new Type(BaseType.SequenceOf));
                    Symbol sym = new Symbol("array", SymbolType.Component);
                    sym.type = $a;
                    $$.fieldList = new SymbolList(sym);
                }
                / SEQUENCE OF NamedType:a
                {
                    $$ = new RuleValue(new Type(BaseType.SequenceOf));
                    $$.fieldList = new SymbolList($a);
                };

//  SECTION 25.3

SequenceOfValue ::= "{" "}"                                        
                {
                    $$ = new RuleValue(new Value(new ValueList()));
                    $$.type = m_context.GetTopType();
                }
                | "{" 
                {
                    $$ = null;                                  // No return value for intermediates
                    
                    Type type = m_context.GetTopType().SkipFluff();
                    if (type.baseType != BaseType.SequenceOf) {
                        return MatchResult.Fail;
                    }

                    if (type.fieldList.list.Count != 1) {
                        return MatchResult.Fail;
                    }

                    if (type.fieldList.list[0].name != "array") {
                        return MatchResult.Fail;
                    }

                    history.type = type.fieldList.list[0].type;
                }
                ValueList:a "}"
                {
                    $$ = new RuleValue(new Value($a));
                    history.type = null;
                    $$.type = m_context.GetTopType();
                }
                | "{" NamedValueList:a "}"
                {
                    $$ = new RuleValue(new Value($a)); 
                    $$.type = m_context.GetTopType();
                }
                ;

ValueList ::= Value:a "," +                                                             { $$ = new RuleValue(new ValueList($a)); };

NamedValueList ::= NamedValue:a "," +                                                   { $$ = new RuleValue(new ValueList($a)); };

// XMLSequenceOfValue ::= XMLValueList | XMLDelimitedItemList | XMLSpaceSeparatedList | Empty ;
// XMLValueList ::= XMLValueOrEmpty | XMLValueOrEmpty XMLValueList ;
// XMLValueOrEmpty ::= XMLValue | "<" & NonParameterizedTypeName "/>" ;
// XMLSpaceSeparatedList ::= XMLValueOrEmpty | XMLValueOrEmpty " " XMLSpaceSeparatedList ;
// XMLDelimitedItemList ::= XMLDelimitedItem | XMLDelimitedItem XMLDelimitedItemList ;
// XMLDelimitedItem ::= "<" & NonParameterizedTypeName ">" XMLValue "</" & NonParameterizedTypeName ">" | "<" & identifier ">" XMLValue "</" & identifier ">" ;

//  SECTION 26.1
//
SetType ::=     SET "{" "}"
                {
                    $$ = new RuleValue(new Type(BaseType.Set));
                    $$.fieldList = new SymbolList();
                }
                / SET "{" ExtensionAndException:a OptionalExtensionMarker:b? "}" 
                {
                    $$ = new RuleValue(new Type(BaseType.Set));
                    $$.fieldList = $a;

                    if ($b.result != MatchResult.Fail) {
                        $$.fieldList.Add($b);
                    }
                }
                / SET "{" ComponentTypeLists:a "}" 
                {
                    ErrorList errs;
                    
                    $$ = new RuleValue(new Type(BaseType.Set));
                    $$.fieldList = $a;

                    CheckTagging($a, true, out errs);

                };

// SECTION 26.7
//
SetValue ::=    "{" "}":c
                {
                    Type type = m_context.GetTopType();
                    
                    //  Requires that all fields are either defaulted or optional or
                    //  that there are no fields

                    if (type.fieldList.Count == 0) {
                        //
                        //  This is an ok initializer
                        //
                        //  Create an empty value list and set the value to that
                        //

                        $$ = new RuleValue(new Value(new ValueList()));
                        $$.type = type;
                    }
                    else {
                        //
                        //  Check to see if all of the required fields are present
                        //
                        
                        $$ = CheckSequenceFields(new ValueList(), $c.token);
                    }
                }                    
                / "{" ComponentValueList:a "}":c
                {
                    //
                    //  Sort the fields into order and check that all are present
                    //

                    $$ = CheckSequenceFields($a, $c.token);
                }
                ;

// XMLSetValue ::= XMLComponentValueList ? ;

//  SECTION 27.1
//
SetOfType ::=   SET OF Type:a 
                {
                    $$ = new RuleValue(new Type(BaseType.SetOf));
                    Symbol sym = new Symbol("array", SymbolType.Component);
                    sym.type = $a;
                    $$.fieldList = new SymbolList(sym);
                }
                / SET OF NamedType:a
                {
                    $$ = new RuleValue(new Type(BaseType.SetOf));
                    $$.fieldList = new SymbolList($a);
                };

//  SECTION 27.3
//
SetOfValue ::=  "{" "}"
                {
                    $$ = new RuleValue(new Value(new ValueList()));
                    $$.type = m_context.GetTopType();
                }
                | "{" 
                {
                    $$ = null;                                  // No return value for intermediates
                    
                    Type type = m_context.GetTopType().SkipFluff();
                    if (type.baseType != BaseType.SetOf) {
                        return MatchResult.Fail;
                    }

                    if (type.fieldList.list.Count != 1) {
                        return MatchResult.Fail;
                    }

                    if (type.fieldList.list[0].name != "array") {
                        return MatchResult.Fail;
                    }

                    history.type = type.fieldList.list[0].type;
                                            
                }
                ValueList:a "}"   
                {
                    $$ = new RuleValue(new Value($a));
                    history.type = null;
                    $$.type = m_context.GetTopType();
                }
                | "{" NamedValueList:a "}"              
                {
                    $$ = new RuleValue(new Value($a)); 
                    $$.type = m_context.GetTopType();
                }
                ;

// XMLSetOfValue ::= XMLValueList | XMLDelimitedItemList | XMLSpaceSeparatedList | Empty ;

//  SECTION 28.1
//
ChoiceType ::= CHOICE "{" AlternativeTypeLists:a "}" 
                {
                    ErrorList errs;

                    //
                    //  Create a new type and add the choice field list
                    //

                    $$ = new RuleValue(new Type(BaseType.Choice));
                    $$.fieldList = $a;

                    CheckTagging($a, true, out errs);

                } ;

//AlternativeTypeLists ::= 
//                RootAlternativeTypeList "," ExtensionAndException ExtensionAdditionAlternatives? OptionalExtensionMarker? 
//                | RootAlternativeTypeList ;

AlternativeTypeLists ::= RootAlternativeTypeList:a AlternativeTypeLists1:b ?
                {
                    $$ = $a.rv;

                    if ($b.result != MatchResult.Fail) {
                        $$.AddRange($b);
                    }
                };

AlternativeTypeLists1 ::= "," ExtensionAndException ExtensionAdditionAlternatives? OptionalExtensionMarker? 
                {
                    $$ = history[1].rv;
                    if (history[3].result != MatchResult.Fail) {
                        $$.AddRange(history[3].rv.symList);
                    }
                    if (history[5].result != MatchResult.Fail) {
                        $$.AddRange(history[5].rv.symList);
                    }
                };

RootAlternativeTypeList ::= AlternativeTypeList ;

ExtensionAdditionAlternatives ::= "," ExtensionAdditionAlternativesList:a                       { $$ = $a.rv; };

ExtensionAdditionAlternativesList ::= ExtensionAdditionAlternative:a "," + 
                {
                    $$ = new RuleValue(new SymbolList($a));
                };

ExtensionAdditionAlternative ::= ExtensionAdditionAlternativesGroup | NamedType ;

ExtensionAdditionAlternativesGroup ::= "[" _ "[" VersionNumber:a ? AlternativeTypeList:b  "]" _ "]"
                {
                    $$ = new RuleValue(new Symbol(SymbolType.ExtensionGroup));

                    if ($a.result != MatchResult.Fail) {
                        $$.groupVersion = (int) $a.iVal;
                    }
                    else {
                        $$.groupVersion = -1;
                    }

                    $$.symList = $b;
                };

AlternativeTypeList ::= NamedType:a "," +
                {
                    $$ = new RuleValue(new SymbolList($a));
                };

//  SECTION 28.10
//
ChoiceValue ::= L_Identifier ":" Value ;
// XMLChoiceValue ::= "<" & identifier ">" XMLValue "</" & identifier ">" ;

//  SECTION 29.1
//
SelectionType ::= L_Identifier "<" Type ;

//  SECTION 30.1
//

PrefixedType ::= TaggedType | EncodingPrefixedType ;

PrefixedValue ::= Value;

TaggedType ::=  Tag:a TagType:b Type:c
                {
                    $$ = $a.rv;
                    $$.subType = $c;
                    $$.tagged.tagType = (TagType) $b.iVal;

                    // M00TODO - If explictily IMPLICIT then Type cannot be a CHOICE, Open Type or Dummy Ref
                }
                ;

TagType ::=     IMPLICIT
                {
                    $$ = new RuleValue(new Value((int) TagType.Implicit, Type.IntegerType));
                }
                / EXPLICIT
                {
                    $$ = new RuleValue(new Value((int) TagType.Explicit, Type.IntegerType));
                }
                / Empty
                {
                    $$ = new RuleValue(new Value((int) m_tagTypeDefault, Type.IntegerType));
                };
                    

Tag ::= "[" EncodingReference ? Class:a ClassNumber:b "]"
                {
                    $$ = new RuleValue(new Type(BaseType.Tagged));

                    $$.tagged.value = (int) $b.iVal;

                    $$.tagged.tagClass = (TagClass) $a.iVal;
                };

EncodingReference ::= encodingreference ":";

ClassNumber ::= number:a                                        { $$ = new RuleValue(new Value($a.number, Type.IntegerType)); }
                / DefinedValue:a
                {
                    $$ = $a.rv;
                    $$.Collapse();
                };

Class ::=       UNIVERSAL
                {
                    $$ = new RuleValue(new Value((int) TagClass.Universal, Type.IntegerType));
                }
                / APPLICATION
                {
                    $$ = new RuleValue(new Value((int) TagClass.Application, Type.IntegerType));
                }
                / PRIVATE
                {
                    $$ = new RuleValue(new Value((int) TagClass.Private, Type.IntegerType));
                }
                / Empty
                {
                    $$ = new RuleValue(new Value((int) TagClass.Context, Type.IntegerType));
                };

//  SECTION 30.9
//
TaggedValue ::= Value ;
// XMLTaggedValue ::= XMLValue ;


EncodingPrefixedType ::= EncodingPrefix Type;

EncodingPrefix ::= "[" EncodingReference EncodingInstruction "]";

EncodingInstruction ::= OBJECT IDENTIFIER; // M00BUG

//
// SECTION 31.1
//
ObjectIdentifierType ::= OBJECT IDENTIFIER 
                {
                    $$ = new RuleValue(Type.ObjectIdentifier);
                }
                ;

//
// SECTION 31.3
//
ObjectIdentifierValue ::= "{" DefinedValue:a ? ObjIdComponentsList:b
                {
                    ValueList lst;
                    bool fCheckOID = true;
                    //
                    //  If we have a defined value, then put it in first.
                    //
                    //
                    
                    if ($a.result != MatchResult.Fail) {
                        //
                        //  This must be an integer or an OID anything else is an error
                        //
                        //  Must be a non-negative integer from the DefinedValue in ObjIdComponent or
                        //  Must be a object identifier for the ObjectIdentifierValue rule
                        //
                        //  However we are pushing all of this onto the validation function
                        //
                        
                        lst = new ValueList();

                        lst.Add($a);
                        lst.Add($b);
                    }
                    else {
                        //
                        //  Returns the list of values
                        //
                        
                        lst = $b;
                    }

                    $$ = new RuleValue(new Value(lst));
                    $$.type = Type.ObjectIdentifier;

                    //
                    // M00TODO - OID validation to be performed
                    // 1.  Must be of length 2 or greater
                    // 2.  All values must be non-negative value
                    // 3.  NameForm must be used in correct order and correct location only
                    // 4.  When a Relative OID is used, it must be at least the "third" element in the OID list
                    // 5.  identifiers SHOULD be unique and not re-assigned
                    // 6.  The first two values have effective size limitations
                    //

                    if (fCheckOID) {
                        ErrorList errs;
                        MatchResult     result;
                        
                        result = $$.PerformOIDChecks(m_rgTokens[$b.iTokenLast], out errs);

                        //
                        //  If the validation code returns a list of errors - propigate them forward
                        //
                        
                        if ((errs != null) && (errs.Count > 0)) {
                            $b.rv.AddWarning(errs);
                        }

                        //
                        //  If we failed - then return the failure code
                        //

                        if (result != MatchResult.Success) {
                            $$ = new RuleValue(errs);
                            return result;
                        }
                    }
                };

ObjIdComponentsList ::= ObjIdComponents:a ObjIdComponentsList:b
                {
                    $$ = $b.rv;
                    $$.Insert(0, $a);
                }
                /
                ObjIdComponents:a "}"
                {
                    $$ = new RuleValue(new ValueList($a));
                    $$.type = Type.ObjectIdentifier;
                }
                ;

//
//  THis is an ordered list of locations to be looked at.
//
//  NameAndNumber must be before both NameFormand DefinedValue so that
//      it will be preferencientally used
//  DefinedValue preceeds NameForm so that a real variable would be 
//      preferencially used
//
//  Checking that DefinedValue is a Relative OID is defered to the
//      final check code
//

ObjIdComponents ::= NumberForm / NameAndNumberForm
                / DefinedValue
                / NameForm;

NameForm ::=    L_Identifier:a
                {
                    //
                    //  Always reduce - the correctness of this node will be
                    //  checked on the final reduction of the OID value
                    //

                    $$ = new RuleValue(new Value(new OIDNode($a.name)));
                };

//
//  Checking that DefinedValue is a positive integer value is defered until
//      the final check code
//
NumberForm ::=  number:a
                {
                    $$ = new RuleValue(new Value(new OIDNode($a.number)));
                }
                / DefinedValue
                ;

NameAndNumberForm ::= L_Identifier:a "(" NumberForm:b ")"
                {
                    //  Return the number as a named value

                    $$ = new RuleValue(new Value(new OIDNode($b.oidNode.iValue, $a.name)));
                    $$.name = $a.name;
                }
                ;

//XMLObjectIdentifierValue ::= XMLObjIdComponentList ;
//XMLObjIdComponentList ::= XMLObjIdComponent | XMLObjIdComponent & "." & XMLObjIdComponentList ;
//XMLObjIdComponent ::= NameForm | XMLNumberForm | XMLNameAndNumberForm ;
//XMLNumberForm ::= number ;
//XMLNameAndNumberForm ::= identifier & "(" & XMLNumberForm & ")" ;

//  SECTION 32.1
//
RelativeOIDType ::= RELATIVE-OID                        { $$ = new RuleValue(Type.RelativeOID); };

//  SECTION 32.3
//
RelativeOIDValue ::= "{" RelativeOIDComponentsList:a "}" 
                {
                    $$ = new RuleValue(new Value($a));
                }
                ;

RelativeOIDComponentsList ::= RelativeOIDComponents:a Empty +           { $$ = null; /*new RuleValue(new Value($a));*/ };
RelativeOIDComponents ::= NumberForm 
                | NameAndNumberForm 
                | DefinedValue ;

// XMLRelativeOIDValue ::= XMLRelativeOIDComponentList  ;
// XMLRelativeOIDComponentList ::= XMLRelativeOIDComponent | XMLRelativeOIDComponent & "." & XMLRelativeOIDComponentList ;
// XMLRelativeOIDComponent ::= XMLNumberForm | XMLNameAndNumberForm ;

//
//

IRIType ::= OID-IRI                                     { $$ = new RuleValue(Type.OID_IRI); };

//

IRIValue ::= cstring; // '"' FirstArcIdentifier SubsequenceArcIdentifier '"' ;

// FirstArcIdentifier ::= "/" ArcIdentifier ;
// SubsequentArcIdentifier "/" ArcIdentifier SubsequenceArcIdentifier 
//                / empty ;
// ArcIdentifier = integerUnicodeLabel | non-integerUnicodeLabel ;
        
//

RelativeIRIType ::= RELATIVE-OID-IRI;

//

RelativeIRIValue ::= cstring;  // '"' FirstRelativeArcIdentifier SubsequenceArcIdentifier '"';

//  SECTION 33.1
//
EmbeddedPDVType ::= EMBEDDED PDV                        { $$ = new RuleValue(Type.EmbeddedPDV); } ;

//  SECTION 33.8
//
EmbeddedPDVValue ::= SequenceValue ;
// XMLEmbeddedPDVValue ::= XMLSequenceValue ;

//  SECTION 34.1
//
ExternalType ::= EXTERNAL                               { $$ = new RuleValue(Type.External); };

//  SECTION 34.6
//
ExternalValue ::= SequenceValue ;
// XMLExternalValue ::= XMLSequenceValue ;


//

TimeType ::= TIME       { $$ = new RuleValue(Type.Time); };

//

TimeValue ::= tstring ;

tstring ::= cstring;

//

DateType ::= DATE;

TimeOfDayType ::= TIME-OF-DAY ;

DateTimeType ::= DATE-TIME;

DurationType ::= DURATION ;


//  SECTION 36.1
//
CharacterStringType ::= RestrictedCharacterStringType / UnrestrictedCharacterStringType ;

//  SECTION 36.3
//
CharacterStringValue ::= RestrictedCharacterStringValue | UnrestrictedCharacterStringValue ;

// XMLCharacterStringValue ::= XMLRestrictedCharacterStringValue | XMLUnrestrictedCharacterStringValue ;

//  SECTION 37.0
//
RestrictedCharacterStringType ::= BMPString 
                {
                    $$ = new RuleValue(Type.BmpString);
                }
                / GeneralString 
                {
                    $$ = new RuleValue(Type.GeneralString);
                }
                / GraphicString 
                {
                    $$ = new RuleValue(Type.GraphicString);
                }
                / IA5String 
                {
                    $$ = new RuleValue(Type.IA5String);
                }
                / ISO646String 
                {
                    $$ = new RuleValue(Type.VisibleString);
                }
                / NumericString 
                {
                    $$ = new RuleValue(Type.NumericString);
                }
                / PrintableString
                {
                    $$ = new RuleValue(Type.PrintableString);
                }
                / TeletexString
                {
                    $$ = new RuleValue(Type.TeletexString);
                }
                / T61String 
                {
                    $$ = new RuleValue(Type.TeletexString);
                }
                / UniversalString
                {
                    $$ = new RuleValue(Type.UniversalString);
                }
                / UTF8String
                {
                    $$ = new RuleValue(Type.Utf8String);
                }
                / VideotexString
                {
                    $$ = new RuleValue(Type.VideotexString);
                }
                / VisibleString
                {
                    $$ = new RuleValue(Type.VisibleString);
                }
                ;

//  SECTION 37.8
//
RestrictedCharacterStringValue ::= cstring:a
                {
                    $$ = new RuleValue(new Value($a.name));
                }
                | CharacterStringList 
                | Quadruple
                {
                    $$ = new RuleValue(new Value(ValueType.String));
                    $$.name = "";
                }
                | Tuple 
                {
                    $$ = new RuleValue(new Value(ValueType.String));
                    $$.name = "";
                }
                ;

CharacterStringList ::= "{" CharSyms "}"
                {
                    $$ = new RuleValue(new Value(ValueType.String));
                    $$.name = "";
                }
                ;

CharSyms ::= CharsDefn | CharSyms "," CharsDefn ;
CharsDefn ::= cstring | Quadruple | Tuple | DefinedValue ;
Quadruple ::= "{" Group "," Plane "," Row "," Cell "}" ;
Group ::= number ;
Plane ::= number ;
Row ::= number ;
Cell ::= number ;
Tuple ::= "{" TableColumn "," TableRow "}" ;
TableColumn ::= number ;
TableRow ::= number ;

//  SECTION 37.9
//
// XMLRestrictedCharacterStringValue ::= xmlcstring ;

//  SECTION 40.1
//
UnrestrictedCharacterStringType ::= CHARACTER STRING                            { $$ = new RuleValue(Type.CharacterString); } ;

//  SECTION 40.7
//
UnrestrictedCharacterStringValue ::= SequenceValue ;

// XMLUnrestrictedCharacterStringValue ::= XMLSequenceValue ;

//  SECTION 41.1
//
UsefulType ::= GeneralizedTime                          { $$ = new RuleValue(Type.GeneralizedTime); }
                / UTCTime                               { $$ = new RuleValue(Type.UTCTime); }
                / ObjectDescriptor                      { $$ = new RuleValue(Type.ObjectDescriptor); }
                ;

//  SECTION 45.1
//
//  This is now built into the definition of type
//
//  ConstrainedType ::= Type Constraint | TypeWithConstraint ;


//  SECTION 45.5
//
//TypeWithConstraint ::= SET Constraint OF Type
//                        | SET SizeConstraint OF Type
//                        | SEQUENCE Constraint OF Type
//                        | SEQUENCE SizeConstraint OF Type
//                        | SET Constraint OF NamedType
//                        | SET SizeConstraint OF NamedType
//                        | SEQUENCE Constraint OF NamedType
//                        | SEQUENCE SizeConstraint OF NamedType
//                        ;
//
//  M00BUG: Constraint implies that Type has been parsed beforehand?
//              Contraint on first rule applies to 'SET OF Type' - question is what this really means for things like SingleValue.
//

TypeWithConstraint ::= SET SizeConstraint:a ? OF Type:b
                {
                    $$ = new RuleValue(new Type(BaseType.SetOf));
                    Symbol sym = new Symbol("array", SymbolType.Component);
                    sym.type = $b;
                    $$.fieldList = new SymbolList(sym);
                    if ($a.result != MatchResult.Fail) {
                        $$.AddConstraint($a);
                    }
                }
                | SEQUENCE SizeConstraint:a ? OF Type:b
                {
                    $$ = new RuleValue(new Type(BaseType.SequenceOf));
                    Symbol sym = new Symbol("array", SymbolType.Component);
                    sym.type = $b;
                    $$.fieldList = new SymbolList(sym);
                    if ($a.result != MatchResult.Fail) {
                        $$.AddConstraint($a);
                    }
                }
                | SET SizeConstraint:a OF NamedType:b
                {
                    $$ = new RuleValue(new Type(BaseType.SetOf));
                    $$.fieldList = new SymbolList($b);
                    $$.AddConstraint($a);
                }
                | SEQUENCE SizeConstraint:a OF NamedType:b
                {
                    $$ = new RuleValue(new Type(BaseType.SequenceOf));
                    $$.fieldList = new SymbolList($b);
                    $$.AddConstraint($a);
                }
                ;

//  SECTION 45.6
//

Constraint ::= "(" ConstraintSpec:a ExceptionSpec? ")"
                {
                    $$ = $a.rv;

                    //  Currently ignoring exception information since we don't pass to the implementation code
                }
                ;

ConstraintSpec ::= SubtypeConstraint
                | GeneralConstraint
                ;

//  SECTION 45.7
//

SubtypeConstraint ::= ElementSetSpecs ;

//  SECTION 46.1
//

ElementSetSpecs ::= RootElementSetSpec "," "." _ "." _ "." "," AdditionalElementSetSpec 
                / RootElementSetSpec "," "." _ "." _ "."
                / RootElementSetSpec
                ;

RootElementSetSpec ::= ElementSetSpec ;

AdditionalElementSetSpec ::= ElementSetSpec ;

//
//  We are re-writing this to make it more efficent
//
//  ElementSetSpec ::= Unions | ALL Exclusions ;
//  Unions ::=      Intersections UnionMark Unions | Intersections ;
//  Intersections ::= Elements IntersectionMark Intersections | Elements ;
//  IntersectionElements ::=  Elements Exclusions?  ;
//  Exclusions ::=  EXCEPT Elements ;
//  UnionMark ::= "|" | UNION ;
//  IntersectionMark ::= "^" | INTERSECTION ;
//

ElementSetSpec ::= ALL EXCEPT Elements
                / JointUnions:a  { $$ = new RuleValue($a.BuildConstraintFromList()); }
                ;

JointUnions ::= Elements:a Marker:b JointUnions:c
                {
                    $$ = new RuleValue(new ConstraintList($c));
                    $$.Insert(0, $b);
                    $$.Insert(0, $a);
                }
                / Elements:a ! Marker
                {
                    $$ = new RuleValue(new ConstraintList($a));
                }
                ;

Marker ::=      "|"                     { $$ = new RuleValue(new Constraint(ConstraintType.Union)); }
                / UNION                 { $$ = new RuleValue(new Constraint(ConstraintType.Union)); }
                / "^"                   { $$ = new RuleValue(new Constraint(ConstraintType.Intersection)); }
                / INTERSECTION          { $$ = new RuleValue(new Constraint(ConstraintType.Intersection)); }
                / EXCEPT                { $$ = new RuleValue(new Constraint(ConstraintType.Except)); }
                ;

//  SECTION 46.5
//
Elements ::=    SubtypeElements
                | ObjectSetElements
                | "(" ElementSetSpec:a ")" { $$ = $a.rv; }
                ;

//  SECTION 47.1
//
//  If you change this item, you MUST change the case statement in AsnFile.cs to match any changes made.
//
SubtypeElements ::= ValueRange | SingleValue | ContainedSubtype | PermittedAlphabet | SizeConstraint | TypeConstraint
                | InnerTypeConstraints | PatternConstraint | PropertySettings | DurationRange | TimePointRange |
                RecurrenceRange ;

//  SECTION 47.2
//
SingleValue ::= Value:a ! DotDot 
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.Value, $a));
                };

DotDot ::=      "." _ ".";

//  SECTION 47.3
//
ContainedSubtype ::= Includes? Type 
                {
                    // M00BUG
                    $$ = new RuleValue(new Constraint(ConstraintType.Contained));
                };

Includes ::= INCLUDES ;

//  SECTION 47.4
//
ValueRange ::=  LowerEndpoint:a "." _ "." UpperEndpoint:b
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.Range, $a, $b));
                }
                ;

LowerEndpoint ::= LowerEndValue:a "<" ?
                {
                    $$ = $a.rv;
                    if (history[2].result != MatchResult.Fail) {
                        $$.ChangeType(ConstraintType.LB_Value);
                    }
                }
                ;

UpperEndpoint ::= "<" ? UpperEndValue:a
                {
                    $$ = $a.rv;
                    if (history[1].result != MatchResult.Fail) {
                        $$.ChangeType(ConstraintType.UB_Value);
                    }
                }
                ;

LowerEndValue ::= MIN 
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.Value_Min));
                }
                / Value:a
                {
                    $$ = new RuleValue(new Constraint( ConstraintType.Value, $a));
                }
                ;

UpperEndValue ::= MAX 
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.Value_Max));
                }
                / Value:a
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.Value, $a));
                }
                ;

//
//  SECTION 47.5
//
SizeConstraint ::= SIZE 
                {
                    //
                    //  Values at this point should be integers - so push that info into the context.
                    //
                    //  values must meet INTEGER (0..MAX) constraint
 
                    history.type = Type.PositiveInteger;
                    $$ = null;
                }
                Constraint:a
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.Size, $a));
                } ;

//  SECTION 47.6
//
TypeConstraint ::= Type ;

//  SECTION 47.7
//
PermittedAlphabet ::= FROM Constraint:a
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.From, $a));
                } ;

//  SECTION 47.8
//  

InnerTypeConstraints ::= WITH COMPONENT SingleTypeConstraint 
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.With));
                }
                |
                WITH COMPONENTS MultipleTypeConstraints 
                { 
                    $$ = new RuleValue(new Constraint(ConstraintType.With));
                }
                ;
SingleTypeConstraint ::= Constraint ;
MultipleTypeConstraints ::= FullSpecification | PartialSpecification ;
FullSpecification ::= "{" TypeConstraints "}" ;
PartialSpecification ::= "{" "." _ "." _ "." "," TypeConstraints "}" ;
TypeConstraints ::= NamedConstraint "," + ;
NamedConstraint ::= L_Identifier ComponentConstraint ;
ComponentConstraint ::= ValueConstraint? PresenceConstraint? ;
ValueConstraint ::= Constraint ;
PresenceConstraint ::= PRESENT | ABSENT | OPTIONAL ;

//  SECTION 47.9
//
PatternConstraint ::= PATTERN Value ;

//

PropertySettings ::= SETTINGS cstring; // simplestring ;

//

PropertySettingsList ::= PropertyAndSettingPair | PropertySettingsList PropertyAndSettingPair ;

PropertyAndSettingPair ::= PropertyName "=" SettingName ;

PropertyName ::= psname;

SettingName ::= psname;

psname ::= cstring;

//

DurationRange ::= ValueRange;

TimePointRange ::= ValueRange;

RecurrenceRange ::= ValueRange;

//
//  SECTION 49.4
//
ExceptionSpec ::= "!" 
                {
                    //
                    //  By default we are looking for an integer number as the identifier
                    //

                    history.type = Type.IntegerType;
                    $$ = null;
                }
                ExceptionIdentification :a
                {
                    $$ = new RuleValue(new Symbol(SymbolType.Exception));
                    $$.value = $a;
                };

ExceptionIdentification ::=
                SignedNumber
                | DefinedValue
                | Type:a { history.type = $a; $$=null; } ":" Value:b { $$ = $b.rv; } ;


EncodingControlSections ::= EncodingControlSection EncodingControlSections;

EncodingControlSection ::= ENCODING-CONTROL encodingreference ; // EncodingInstructionAssignmentList;


// **********************************************************************************
//
//                                  X.681
//
// **********************************************************************************


//  SECTION 7.1
//

objectclassreference ::= U_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);

                    if ((sym == null) || !sym.IsBaseType(SymbolType.ObjectClass)) FAIL;

                    $$ = new RuleValue(sym);
                };

objectclassreferenceLHS ::= U_Identifier:a
                {
                    //
                    //  All uppercase letters, hyphen allowed
                    //

                    for (int ii = 0; ii < $a.name.Length; ii++) {
                        if (Char.IsLower($a.name[ii])) FAIL;
                    }

                    $$ = $a.rv;
                };

//  SECTION 7.2
//
objectreference ::= L_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || !sym.IsBaseType(SymbolType.Object)) FAIL;

                    $$ = new RuleValue(sym);
                };

objectreferenceLHS ::= L_Identifier:a
                {
                    $$ = $a.rv;
                };


//  SECTION 7.3
//
objectsetreference ::= U_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || !sym.IsBaseType(SymbolType.ObjectSet)) FAIL;

                    $$ = new RuleValue(sym);
                };

objectsetreferenceLHS ::= U_Identifier:a
                {
                    $$ = $a.rv;
                };

//  SECTION 7.4
//
typefieldreference ::= A_U_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || !sym.IsType(SymbolType.TypeField)) FAIL;

                    //
                    //  Return the token
                    //

                    $$ = new RuleValue(sym);
                } ;

typefieldreferenceLHS ::= A_U_Identifier:a
                {
                    //  return the token

                    $$ = $a.rv;
                };


//  SECTION 7.5
//
valuefieldreference ::= A_L_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || !sym.IsType(SymbolType.ValueField)) FAIL;

                    $$ = new RuleValue(sym);
                } ;

valuefieldreferenceLHS ::= A_L_Identifier:a
                {
                    //  Return the token

                    $$ = $a.rv;
                };

            
//  SECTION 7.6
//
valuesetfieldreference ::= A_U_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || !sym.IsType(SymbolType.ValueSetField)) FAIL;                // M00BUG - wrong SymbolType

                    $$ = new RuleValue(sym);
                };

valuesetfieldreferenceLHS ::= A_U_Identifier;
            
//  SECTION 7.7
//
objectfieldreference ::= A_L_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);

                    if ((sym == null) || !sym.IsType(SymbolType.ObjectField)) FAIL;                // M00BUG - wrong type

                    $$ = new RuleValue(sym);
                };

objectfieldreferenceLHS ::= A_L_Identifier;
            
//  SECTION 7.8
//
objectsetfieldreference ::= A_U_Identifier:a
                {
                    Symbol sym = FindSymbol($a.name);
                    if ((sym == null) || !sym.IsType(SymbolType.ObjectSetField)) FAIL;                // M00BUG - wrong type

                    $$ = new RuleValue(sym);
                }
                ;

objectsetfieldreferenceLHS ::= A_U_Identifier;

//  SECTION 7.9
//
word ::= U_Identifier ;

//          All sorts of fun rules here - all upper case & hyphen, but can include some tokens as well.

//  SECTION 8.1
//

DefinedObjectClass ::= ExternalObjectClassReference:a           
                {
                    //  Convert the symbol into a type

                    if ($a.type == null) {
                        $$ = new RuleValue(new Type($a));
                    }
                    else {
                        $$ = new RuleValue($a.type);
                    }
                }
                / objectclassreference:a
                {
                    //  Convert the symbol into a type

                    if ($a.type == null) {
                        $$ = new RuleValue(new Type($a));
                    }
                    else {
                        $$ = new RuleValue($a.type);
                    }
                }
                / UsefulObjectClassReference
                ;

DefinedObject ::= ExternalObjectReference:a                     { $$ = new RuleValue(new Value($a)); }
                | objectreference:a                             { $$ = new RuleValue(new Value($a)); }
                ;

DefinedObjectSet ::= ExternalObjectSetReference:a               { $$ = new RuleValue(new Value($a)); }
                | objectsetreference:a                          { $$ = new RuleValue(new Value($a)); }
                ;

//  SECTION 8.3
//
ExternalObjectClassReference ::= moduleReference:a "." objectclassreferenceLHS:b
                {
                    Symbol sym;
                    
                    Debug.Assert(false,"NYI");

                    sym = GetExternalReference($a.name, $b.name);

                    if (sym == null) {
                        $$ = null; // $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, $b));
                        return MatchResult.Fail;
                    }
                    
                    if ((sym.symType & SymbolType.BaseMask) != SymbolType.Value) {
                        $$ = null; // $$ = new RuleValue(new Error(ErrorNumber.SymbolNotValue, $b));
                        return MatchResult.Fail;
                    }                        

                    $$ = new RuleValue(sym);
                } ;

ExternalObjectReference ::= moduleReference:a "." objectreference:b
                {
                    Symbol sym;
                    
                    Debug.Assert(false,"NYI");

                    sym = GetExternalReference($a.name, $b.name);

                    if (sym == null) {
                        $$ = null; // $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, $b));
                        return MatchResult.Fail;
                    }
                    
                    if ((sym.symType & SymbolType.BaseMask) != SymbolType.Object) {
                        $$ = null; // $$ = new RuleValue(new Error(ErrorNumber.SymbolNotValue, $b));
                        return MatchResult.Fail;
                    }                        

                    $$ = new RuleValue(sym);
                } ;

ExternalObjectSetReference ::= moduleReference:a "." objectsetreferenceLHS:b
                {
                    Symbol sym;
                    
                    // Debug.Assert(false,"NYI");

                    sym = GetExternalReference($a.name, $b.name);

                    if (sym == null) {
$$ = null; //                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, $b));
                        return MatchResult.Fail;
                    }
                    
                    if ((sym.symType & SymbolType.BaseMask) != SymbolType.ObjectSet) {
$$ = null; //                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotValue, $b));
                        return MatchResult.Fail;
                    }                        

                    $$ = new RuleValue(sym);
                } ;


//  SECTION 8.4
//
UsefulObjectClassReference ::= 
                TYPE-IDENTIFIER 
                {
                    $$ = new RuleValue(new Type(BaseType.Class));
                    $$.fieldList = new SymbolList();
                    
                    Symbol sym = new Symbol("&id", SymbolType.ValueField);
                    sym.type = Type.ObjectIdentifier;
                    sym.flags.Unique = true;
                    $$.fieldList.Add(sym);

                    $$.fieldList.Add(new Symbol("&Type", SymbolType.TypeField));

                    $$.syntax = new RuleStep(RuleNo.TypeIdentifier);
                }                  
                | ABSTRACT-SYNTAX ;

//
//  SECITON 9.1
//
ObjectClassAssignment ::= objectclassreferenceLHS:a "::=" 
                {
                    //
                    //  Create the symbol and place it in the context stack so that
                    //  it can be used by recursive references
                    //

                    if ($a.sym == null) {
                        history.sym = new Symbol($a.name, SymbolType.ObjectClass);
                    }
                    else {
                        history.sym = $a.sym;
                    }
                    $$ = null;
                }
                ObjectClass:b
                {
                    //
                    //  We have already done some setup work
                    //

                    if ($a.sym != null) {
                        $a.sym.type.subType = $b;
                    }
                    else {
                        //
                        //  Set the type on the symbol ...
                        //

                        history.sym.type = new Type(BaseType.Assignment);
                        history.sym.type.subType = $b;

                        //
                        // .. and the symbol on the type ...
                        //

                        history.sym.type.owner = history.sym;

                        //
                        //  ... and put into the symbol table
                        //

                        if (m_symbols.Find(history.sym) != null) {
                            LogError.Error("Duplicate symbol defined ", history.sym.name);
                        }
                        else {
                            m_symbols.Add(history.sym);
                        }
                    }

                    //
                    //  No return value
                    //
                    
                    $$ = null;

                    //
                    //  Collapse down the tokens on success     
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ObjectClassAssignment, 0, 1));
                    cTokensUsed = 1;
                }
                | objectclassreferenceLHS:a "::=" CLASS
                {
                    if ($a.sym == null) {
                        //  Create a symbol in the table as an unset class.

                        Symbol sym = new Symbol($a.name, SymbolType.ObjectClass);

                        //
                        //  This is what everybody will point to - so set it up.
                        //

                        sym.type = new Type(BaseType.Assignment);
                        sym.type.owner = sym;

                        //
                        //  Define the class as having now fields.
                        //

                        sym.type.subType = new Type(BaseType.Class);
                        sym.type.subType.fieldList = new SymbolList();

                        //
                        //  Try and put it into the symbol table
                        //

                        if (m_symbols.Find(sym) != null) {
                            LogError.Error("Duplicate symbol defined ", history.sym.name);
                        }
                        else {
                            m_symbols.Add(sym);
                        }

                        //
                        // Put the symbol on the token so it can be found later
                        //

                        $a.sym = sym;
                    }
                    FAIL;
                }
                ;

//
//  SECTION 9.2
//
ObjectClass ::= DefinedObjectClass:a ! "{"
                / ObjectClassDefn
                / ParameterizedObjectClass ;

//  SECTION 9.3
//
ObjectClassDefn ::= CLASS "{" FieldSpecList:a "}" 
                {
                    //
                    //  Put the field list on the context so we can match it up
                    //

                    history.fieldList = $a;
                    $$  = null;
                }
                WithSyntaxSpec:c ?
                {
                    Type type = new Type(BaseType.Class);
                    type.fieldList = history.fieldList;

                    if ($c.result != MatchResult.Fail) {
                        type.syntax = $c;
                    }

                    $$ = new RuleValue(type);
                } ;

WithSyntaxSpec ::= WITH SYNTAX SyntaxList:a                                                     { $$ = $a.rv; };

FieldSpecList ::= FieldSpec:a "," FieldSpecList:b
                {
                    $a.flags.Pointer = true;

                    $$ = new RuleValue($b);
                    $$.Insert(0, $a);
                }
                / FieldSpec:a ! ","
                {
                    $a.flags.Pointer = true;

                    $$ = new RuleValue(new SymbolList($a));
                }
                ;


//  SECTION 9.4
//
FieldSpec ::=   FixedTypeValueFieldSpec
                / VariableTypeValueFieldSpec 
                / FixedTypeValueSetFieldSpec
                / VariableTypeValueSetFieldSpec 
                / ObjectFieldSpec 
                / ObjectSetFieldSpec
                / TypeFieldSpec 
                ;

//  SECTION 9.5
//
TypeFieldSpec ::= typefieldreferenceLHS:a TypeOptionalitySpec:b ? 
                {
                    Symbol fld = new Symbol($a.name, SymbolType.TypeField);

                    //  Check the optional

                    if ($b.result != MatchResult.Fail) {
                        fld.flags = $b;
                    }

                    //  return the field

                    $$ = new RuleValue(fld);
                } ;

TypeOptionalitySpec ::= OPTIONAL
                {
                    FieldFlags flags = new FieldFlags();
                    flags.Optional = true;
                    $$ = new RuleValue(flags);
                }
                | DEFAULT Type:a
                {
                    FieldFlags flags = new FieldFlags();
                    flags.SetDefault($a);
                    $$ = new RuleValue(flags);
                } ;

//  SECTION 9.6
//
FixedTypeValueFieldSpec ::= valuefieldreferenceLHS:a Type:b
                {
                    //
                    //  If Type is not either an assigment or a leaf, then create a new assigment type
                    //  and make that the type instead.
                    //

                    if (($b.baseType != BaseType.Assignment) && ($b.baseType != BaseType.Leaf)) {
                        //
                        //  We can't add to the symbol table because we don't know the parse is correct yet.
                        //  Defer adding tothe symbol table until the re-write pass
                        //

                        Type typ = new Type(BaseType.Assignment);
                        typ.subType = $b;
                        typ.owner = new Symbol($a.name + "__type", SymbolType.Type);
                        typ.owner.type = typ;
                        $b.rv = new RuleValue(typ);
                    }
                    
                    //  Push the type of the field on the stack
                    history.type = $b;
                    $$ = null;
                }
                UNIQUE:c ? ValueOptionalitySpec:d ? 
                {
                    Symbol fld = new Symbol($a.name, SymbolType.ValueField);
                    fld.type = $b;

                    //  Check the first optional

                    if ($d.result != MatchResult.Fail) {
                        fld.flags = $d;
                    }

                    if ($c.result == MatchResult.Success) {
                        if (fld.flags.Default) {
                            //  M00BUG - Generate an error, but don't fail the parse
                        }
                        fld.flags.Unique = true;
                    }


                    $$ = new RuleValue(fld);
                };

ValueOptionalitySpec ::= OPTIONAL 
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.Optional = true;
                }
                | DEFAULT Value:a
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.SetDefault($a);
                };


//  SECTION 9.8

VariableTypeValueFieldSpec ::= valuefieldreferenceLHS FieldName ValueOptionalitySpec? ;

//  SECTION 9.9

FixedTypeValueSetFieldSpec ::= valuesetfieldreferenceLHS:a Type:b
                {
                    //
                    //  If Type is not either an assigment or a leaf, then create a new assigment type
                    //  and make that the type instead.
                    //

                    if (($b.baseType != BaseType.Assignment) && ($b.baseType != BaseType.Leaf)) {
                        //
                        //  We can't add to the symbol table because we don't know the parse is correct yet.
                        //  Defer adding tothe symbol table until the re-write pass
                        //

                        Type typ = new Type(BaseType.Assignment);
                        typ.subType = $b;
                        typ.owner = new Symbol($a.name + "__type", SymbolType.Type);
                        typ.owner.type = typ;
                        $b.rv = new RuleValue(typ);
                    }
                    
                    //  Push the type of the field on the stack
                    //

                    history.type = $b;
                    $$ = null;
                }
                ValueSetOptionalitySpec:c ?
                {
                    Symbol fld = new Symbol($a.name, SymbolType.ValueSetField);
                    fld.type = $b;

                    if ($c.result != MatchResult.Fail) {
                        fld.flags = $c;
                    }

                    $$ = new RuleValue(fld);
                };

ValueSetOptionalitySpec ::= OPTIONAL
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.Optional = true;
                    $$.Pointer = true;

                }
                | DEFAULT ValueSet:a
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.SetDefault($a.ToValue());
                    Debug.Assert($$.valDefault.valueType == ValueType.SetOf);
                };

//  SECTION 9.10

VariableTypeValueSetFieldSpec ::= valuesetfieldreferenceLHS FieldName ValueSetOptionalitySpec? ;

//  SECTION 9.11

ObjectFieldSpec ::= objectfieldreferenceLHS:a DefinedObjectClass:b
                {
                    //  Push the type of the field on the stack
                    history.type = $b;
                    $$ = null;
                }
                ObjectOptionalitySpec:c ? 
                {
                    Symbol fld = new Symbol($a.name, SymbolType.ObjectField);
                    fld.type = $b;

                    //  Check the optional item

                    if ($c.result != MatchResult.Fail) {
                        fld.flags = $c;
                    }

                    $$ = new RuleValue(fld);
                };

ObjectOptionalitySpec ::= OPTIONAL
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.Optional = true;
                }
                | DEFAULT Object:a
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.SetDefault($a);
                };

//  SECTION 9.12

ObjectSetFieldSpec ::= objectsetfieldreferenceLHS:a DefinedObjectClass:b 
                {
                    //  Push the type of the field on the stack
                    history.type = $b;
                    $$ = null;
                }
                ObjectSetOptionalitySpec:c ? 
                {
                    Symbol fld = new Symbol($a.name, SymbolType.ObjectSetField);
                    fld.type = $b;

                    //  Check the optional item

                    if ($c.result != MatchResult.Fail) {
                        fld.flags = $c;
                    }

                    $$ = new RuleValue(fld);
                };

ObjectSetOptionalitySpec ::= OPTIONAL
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.Optional = true;
                }
                | DEFAULT ObjectSet:a
                {
                    $$ = new RuleValue(new FieldFlags());
                    $$.SetDefault($a);
                };

//  SECTION 9.13
//
PrimitiveFieldName ::= typefieldreference | valuefieldreference | valuesetfieldreference | objectfieldreference | objectsetfieldreference ;

PrimitiveFieldNameAnon ::= A_U_Identifier | A_L_Identifier ;

//  SECTION 9.14

FieldName ::= PrimitiveFieldNameAnon:a "." + 
                {
                    $$ = new RuleValue(new TokenList($a));
                };


//  SECTION 10.5
//
SyntaxList ::= "{" TokenList:a "}" 
                {
                    int c = history.Count - 3;
                    int c2;
                    int i;

                    //
                    //  Rule starts with a "{"
                    //  Rules ends with a "}"
                    //  Add those tokens in
                    //

                    $a.InsertAt(0, new RuleStep(TknType.LBrace));
                    $a.Add(new RuleStep(TknType.RBrace));
                    $a.Add(new RuleStep(RuleType.ActionFinal));

                    //
                    //  Create and add the rule to the list
                    //

                    RuleNo rn = Rules.NextRuleNo();
                    Rule ruleNew = new Rule(rn, $a);
                    Rules.m_rgRules2.Add(ruleNew);

                    //
                    //  Pass the rule number up
                    //

                    $$ = new RuleValue(new RuleStep(rn));
                } ;

TokenList       ::= RequiredToken:a TokenList:b
                {
                    $$ = $b.rv;
                    $$.InsertAt(0, $a);
                }
                / OptionalGroup:a TokenList:b
                {
                    $$ = $b.rv;
                    $$.InsertAt(0, $a);
                    $$.InsertAt(0, new RuleStep(RuleType.Optional1));
                }
                / RequiredToken:a
                {
                    $$ = new RuleValue(new RuleStepList($a));
                }
                / OptionalGroup:a
                {
                    $$ = new RuleValue(new RuleStepList(new RuleStep(RuleType.Optional1), $a));
                }
                ;

OptionalGroup ::= "[" TokenList:a "]" 
                {
                    //
                    //  Build a rule for this optional group.
                    //
                    //  Now create the rule and add it to the end of the system
                    //

                    $a.Add(new RuleStep(RuleType.ActionFinal));

                    RuleNo rn = Rules.NextRuleNo();
                    Rule ruleNew = new Rule(rn, $a);
                    Rules.m_rgRules2.Add(ruleNew);

                    //
                    //  Now build the return value - a pointer a the rule we just built
                    //

                    $$ = new RuleValue(new RuleStep(rn));
                };

RequiredToken ::= Literal
                / PrimitiveFieldName:a
                {
                    $$ = new RuleValue(new RuleStep($a));
                }
                ;

//  SECTION 10.7
//
Literal ::= word / ","
                {
                    $$ = new RuleValue(new RuleStep(TknType.Comma));
                } ;

//  SECTION 11.1
//
ObjectAssignment ::= objectreferenceLHS:a DefinedObjectClass:b "::=" 
                {
                    //
                    // M00BUG - define the symbol even though this step fails
                    //
                    
                    //
                    //  If the type has not been filled out, then we cannot do anything useful with it yet.
                    //

                    if ($b.subType == null) FAIL;
                    
                    //
                    //  Save the name of the object and the type to be defined
                    //  in the current context
                    //
                    
                    history.sym = new Symbol($a.name, SymbolType.Object);
                    history.type = $b;
                    $$ = null;
                }
                Object:c
                {
                    //
                    //  Attach the object and object type to the symbol
                    //
                    
                    history.sym.type = history.type;
                    history.sym.value = $c;
                    $c.type = history.type;

                    //
                    //  Inset the symbol in the symbol table.
                    //

                    if (m_symbols.Find(history.sym) != null) {
                        LogError.Error("Duplicate symbol defined ", history.sym.name);
                    }
                    else {
                        m_symbols.Add(history.sym);
                    }

                    //
                    //  No return value
                    //

                    $$ = null;

                    //
                    //  Collapse down to a single token since finished processing
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ObjectAssignment, 0, 0));
                    cTokensUsed = 1;
                };

//  SECTION 11.3
//
Object ::= ObjectDefn | ObjectFromObject | ParameterizedObject | DefinedObject
                ;

//   SECTION 11.4
//
ObjectDefn ::= DefaultSyntax | DefinedSyntax ;

//  SECTION 11.5
//
DefaultSyntax ::= "{" FieldSettingList ? "}" ;

FieldSettingList ::=   FieldSetting "," FieldSettingList
                / FieldSetting ! ","
                ;

FieldSetting ::= PrimitiveFieldName 
                {
                    $$ = null;
                } 
                Setting 
                {
                    $$ = null;
                };

//  SECTION 11.6
//
DefinedSyntax ::= "{" DefinedSyntaxToken Empty * "}" ;
DefinedSyntaxToken ::= Literal | Setting ;
        // Override with the WITH SYNTAX rules

//  SECTION 11.7
//
Setting ::= Type | Value | ValueSet | Object 
                | ObjectSet
                ;
        // Override to match the field type

//  SECTION 12.1

ObjectSetAssignment ::= objectsetreferenceLHS:a DefinedObjectClass:b "::=" 
                {
                    //
                    // M00BUG - define the symbol even though this step fails
                    //
                    
                    //
                    //  If the type has not been filled out, then we cannot do anything useful with it yet.
                    //

                    if ($b.subType == null) FAIL;
                    
                    //
                    //  Make a symbol for what we are defining just in case it is referenced
                    //
                    
                    history.sym = new Symbol($a.name, SymbolType.ObjectSet);

                    //
                    //  Put the type of this object set on the stack so we know what type of things
                    //  to be looking for.  This may be either a symbol or a type (/M00QUEST?) so handle each case
                    //
                    
                    history.type = $b;

                    $$ = null;
                }
                ObjectSet:c
                {
                    //
                    //  Attach the object set to the value
                    //

                    Debug.Assert($c.valueType == ValueType.SetOf);
                    
                    history.sym.type = history.type;
                    history.sym.value = $c;
                    $c.type = history.type;

                    //
                    //  Inset the symbol in the symbol table.
                    //

                    if (m_symbols.Find(history.sym) != null) {
                        LogError.Error("Duplicate symbol defined ", history.sym.name);
                    }
                    else {
                        m_symbols.Add(history.sym);
                    }

                    //
                    //  No return value
                    //

                    $$ = null;

                    //
                    //  Collapse down to a single token since finished processing
                    //

                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.ObjectSetAssignment, 0, 0));
                    cTokensUsed = 1;
                }                    
                ;

//  SECTION 12.3

ObjectSet ::=   "{" 
                {
                    Type typeTop = m_context.GetTopType();
                    if (typeTop.baseType == BaseType.ObjectClassField) {
                        history.type = typeTop.classType;
                    }
                    $$ = null;
                }
                ObjectSetSpec:a "}"
                {
                    $$ = $a.rv;
                };


//ObjectSetSpec ::= RootElementSetSpec
//                | RootElementSetSpec "," "..."
//                | "..."
//                | "..." "," AdditionalElementSetSpec
//                | RootElementSetSpec "," "..." "," AdditionalElementSetSpec ;
ObjectSetSpec ::= "." _ "." _ "." ObjectSetSpec2:a ?
                {
                    if ($a.result != MatchResult.Fail) {
                        Value val = new Value(ValueType.Extension);
                        val.type = m_context.GetTopType();
                        
                        $a.Insert(0, val);
                        $$ = new RuleValue(new Value($a));
                    }
                    else {
                        Value val = new Value(ValueType.Extension);
                        val.type = m_context.GetTopType();
                        
                        Constraint ct = new Constraint(ConstraintType.Value, val);
                        
                        $$ = new RuleValue(ct.ToValue());
                        val.type = m_context.GetTopType();
                    }
                }
                / RootElementSetSpec:a ObjectSetSpec1:b ?
                {
                    Value val = $a.ToValue();
                    if ($b.result != MatchResult.Fail) {
                        val.valueList.Add($b);
                    }
                    $$ = new RuleValue(val);
                }
                ;

ObjectSetSpec1 ::= "," "." _ "." _ "." ObjectSetSpec2:a ?
                {
                    //
                    //  Start by creating the extension marker
                    //
                    
                    ValueList lst = new ValueList();
                    lst.Add(new Value(ValueType.Extension));

                    //
                    //  If exists, then append the seconary list
                    //

                    if ($a.result != MatchResult.Fail) {
                        lst.Add($a);
                    }

                    $$ = new RuleValue(lst);
                };

ObjectSetSpec2 ::= "," AdditionalElementSetSpec:a                               { $$ = new RuleValue($a.ToValueList()); } ;

//  SECTION 12.10

ObjectSetElements ::= ParameterizedObjectSet:a                  { $$  = new RuleValue(new Constraint(ConstraintType.Value, $a)); }
                | ObjectSetFromObjects:a                        { $$  = new RuleValue(new Constraint(ConstraintType.Value, $a)); }
                | Object:a                                      { $$ = new RuleValue(new Constraint(ConstraintType.Value, $a)); }
                | DefinedObjectSet:a                            { $$ = new RuleValue(new Constraint(ConstraintType.Value, $a)); }
                ;

//  SECTION 14.1

ObjectClassFieldType ::= DefinedObjectClass:a
                {
                    //
                    //  If not fully defined - then error
                    //

                    if (($a.baseType == BaseType.Symbol) && ($a.sym.symType != SymbolType.ObjectClassParameter)) {
                        $$ = null;;
                        return MatchResult.Fail;;
                    }

                    //
                    //  Push the type of the class on to the context stack so we can find it's elements
                    //
                    
                    history.type = $a;
                    $$ = null;
                }
                "." FieldName:b
                {
                    $$ = new RuleValue(new Type(BaseType.ObjectClassField));
                    $$.classType = $a;
                    $$.fieldTokens = $b;
                }
                ;

//  SECTION 14.6

ObjectClassFieldValue ::= OpenTypeFieldVal | FixedTypeFieldVal ;

OpenTypeFieldVal ::= Type:a ":"
                {
                    history.type = $a;
                    $$ = null;
                } 
                Value:b 
                {
                    $$ = new RuleValue($b);
                };

FixedTypeFieldVal ::= BuiltinValue | ReferencedValue ;

//  SECTION 15.1

// This line is for use in the text of the recommendation only.
// InformationFromObjects ::= ValueFromObject | ValueSetFromObjects | TypeFromObject | ObjectFromObject | ObjectSetFromObjects ;

ValueFromObject ::= ReferencedObjects:a "." FieldName:b
                {
                    //  Can be a fixed or a variable type field
                    //
                    //  Execute the following steps.
                    //  1. Walk the list of tokens - makes sure that they all exist and
                    //     that the path is a valid path to walk.
                    //  2. The thing at the end of the path needs to be a value
                    //  3. The thing at the end of the path needs to have the correct type
                    //  4. Return the value reference.
                    //

                    $$ = new RuleValue(new Value(ValueType.ExtractFromObject, $a, m_context.GetTopType(), $b));
                }
                ;

ValueSetFromObjects ::= ReferencedObjects:a "." FieldName:b 
                {
                    //  Can be a fixed type value
                    //  Can be a fixed type value set

                    $$ = new RuleValue(new Value(ValueType.ExtractFromObjectSet, $a, m_context.GetTopType(), $b));
                };

TypeFromObject ::= ReferencedObjects "." FieldName 
                {
                    //  Can be a type field
                    
                    Debug.Assert(false, "NYI");
                    rvOut = null;
                    return MatchResult.Fail;
                };

ObjectFromObject ::= ReferencedObjects:a "." FieldName:b
                {
                    //  Can be object

                    $$ = new RuleValue(new Value(ValueType.ExtractFromObject, $a, m_context.GetTopType(), $b));
                }
                ;       

ObjectSetFromObjects ::= ReferencedObjects:a "." FieldName:b
                {
                    //  Can be object field
                    //  Can be object set field

                    $$ = new RuleValue(new Value(ValueType.ExtractFromObjectSet, $a, m_context.GetTopType(), $b));
                }                    
                ;

ReferencedObjects ::= DefinedObject | ParameterizedObject | DefinedObjectSet | ParameterizedObjectSet ;

//  Annex C

InstanceOfType ::= INSTANCE OF DefinedObjectClass:a
                {
                    //  1. Check that DefinedObjectClass is derived from TYPE-IDENTIFIER

                    //  2. Sub in the following
                    //     SEQUENCE {
                    //          type-id PARAM.&id,
                    //          value PARAM.&Type
                    //    }
                    //
                    //    If you had a table constraint applied to the class, then
                    //    apply a fun constraint to the system
                    //       SEQUENCE {
                    //          type-id PARAM.&id({Table}),
                    //          value   PARAM.&Type({Table}{@type-id})
                    //       }
                    
                    $$ = new RuleValue(Type.InstanceOf);
                };

InstanceOfValue ::= Value ;

// ********************************************************************************
//
//                                      X.682
//
// ********************************************************************************

// SECTION 8.1

GeneralConstraint ::= UserDefinedConstraint | TableConstraint | ContentsConstraint ;

//  SECTION 9.1

UserDefinedConstraint ::= CONSTRAINED BY "{" UserDefinedConstraintParameter "," * "}" 
                {
                    $$ = new RuleValue(new Constraint(ConstraintType.UserDefined));
                };

//  SECTION 9.3

UserDefinedConstraintParameter ::= Governor ":" Value
                | Governor ":" ValueSet
                | Governor ":" Object
                | Governor ":" ObjectSet
                | Type
                | DefinedObjectClass ;

//  SECTION 10.3

TableConstraint ::= SimpleTableConstraint / ComponentRelationConstraint ;

SimpleTableConstraint ::= ObjectSet:a ! "{"                        { $$ = new RuleValue(new Constraint(ConstraintType.Table, $a)); };

//  SEcTION 10.7

ComponentRelationConstraint ::= "{" DefinedObjectSet:a "}" "{" AtNotationList:b "}" 
                        {
                            Constraint c = new Constraint(ConstraintType.Relation, $a);
                            Type        typX = m_context.GetTopType();

                            while (typX.baseType == BaseType.Assignment) {
                                typX = typX.subType;
                            }
                            
                            c.text = typX.fieldTokens.ToDotString();
                            c.symlst = $b;
                            $$ = new RuleValue(c);

                        }
                        ;

AtNotationList ::=      AtNotation:a "," AtNotationList:b
                        {
                            //
                            //  Order is not important for these - it is a parallel requirement
                            //
                            
                            Symbol sym = new Symbol(" component", SymbolType.Relationship);
                            sym.symList = $a;

                            $$ = $b.rv;
                            $$.Add(sym);
                        }
                        / AtNotation:a ! ","                            
                        {
                            //
                            //  Grab the current type for giggles
                            //

                            Type type = m_context.GetTopType();
                            
                            Symbol sym = new Symbol(" component", SymbolType.Relationship);
                            sym.symList = $a;
                            $$ = new RuleValue(new SymbolList(sym));
                        }
                        ;

AtNotation ::=          "@" ComponentIdList:a                           { $$ = $a.rv; }
                        / "@" _ "." Level:a ComponentIdList:b           { $$ = $a.rv; $$.AddRange($b); $$.Insert(0, new Symbol("@.", SymbolType.Dummy)); }
                        ;

Level ::=               "." Level:a                                     { $$ = $a.rv; $$.Add(new Symbol(".", SymbolType.Dummy)); } 
                        / Empty                                         { $$ = new RuleValue(new SymbolList()); }
                        ;

ComponentIdList         ::= L_Identifier:a "." ComponentIdList:b        { $$ = $b.rv; $b.Insert(0, new Symbol($a.name, SymbolType.Dummy)); }
                        / L_Identifier:a ! "."                          { $$ = new RuleValue(new SymbolList(new Symbol($a.name, SymbolType.Dummy))); }
                        ;

//  SEcTION 11.1

ContentsConstraint ::= CONTAINING Type:a ENCODED BY Value:b 
                { 
                    if ($b.type != Type.ObjectIdentifier) {
                        // M00BUG - produce an error message - wrong type.
                    }
                    $$ = new RuleValue(new Constraint(ConstraintType.Contents, $a, $b));
                }
                /
                CONTAINING Type:a ! ENCODED
                { 
                    $$ = new RuleValue(new Constraint(ConstraintType.Contents, $a, null));
                }
                / ENCODED BY Value:a
                { 
                    if ($a.type != Type.ObjectIdentifier) {
                        // M00BUG - produce an error message - wrong type.
                    }
                    $$ = new RuleValue(new Constraint(ConstraintType.Contents, null, $a));
                }
                ;

// ********************************************************************************
//
//                                      X.683
//
// ********************************************************************************


// SECTION 8.1
//
ParameterizedAssignment ::= ParameterizedTypeAssignment | ParameterizedValueAssignment | ParameterizedValueSetTypeAssignment 
| ParameterizedObjectClassAssignment | ParameterizedObjectAssignment | ParameterizedObjectSetAssignment ;
           
//  SECTION 8.2
//
ParameterizedTypeAssignment ::= typereferenceLHS:a ParameterList:b "::=":d
                {
                    MatchResult mr;

                    //
                    //  If we have not put the symbol in our history, then we
                    //    are doing the setup pass for this type.
                    //

                    if (history.sym == null) {
                        //
                        //  Put the two fields into the context stack
                        //

                        history.sym = new Symbol($a.name, SymbolType.ParameterizedType);

                        //
                        //  Determine if we are looking at a situation where different parameter lists are possible
                        //

                        $d.rv.paramlsts = BuildPotentialParameterLists($b);
                    }
                    
                    //
                    //  If there is something on the list to process, then we need to continue and process the 
                    //  next one.
                    //
                    
                    if ($d.rv.paramlsts.Count > 0) {

                        history.paramList = $d.rv.paramlsts[0];
                        $d.rv.paramlsts.Remove(history.paramList);
                        
                        $$ = null;
                        mr = MatchResult.ChoiceLeft;
                    }

                    //
                    //  There are no items left to process.
                    else {
                        //
                        //  If we have successfully reduced any time, then collapse down the tokens
                        //      and return the fact that we successfully reduced something.
                        //

                        if (history.sym.parameterization != null) {
                            Debug.Assert(history.sym.parameterization.Count != 0);

                            m_symbols.Add(history.sym);

                            //
                            //  Collapse down the nodes to a single token
                            //

                            ReplaceTokens(history[0].iToken, $d.tokenCount, new Token(RuleNo.ParameterizedTypeAssignment, 0, 0));
                            cTokensUsed = 1;

                            $$ = null;
                            mr = MatchResult.Reduced;
                        }
                        else {

                            //
                            //  Never successfully reduced anything - fail out.
                            //

                            $$ = null;
                            mr = MatchResult.Fail;
                        }
                    }
                    return mr;
                }
                Type:c
                {
                    Type type;

                    //
                    //  Put together the pieces and place into the symbol table.
                    //


                    type = new Type(BaseType.Param_Assignment);
                    type.subType = $c;
                    if (history.sym.parameterization == null) history.sym.parameterization = new List<ParamItems>();
                    history.sym.parameterization.Add(new ParamItems(history.paramList, type));
                    $d.tokenCount = cTokensUsed;

                    //
                    //  This is a textual root
                    //

                    $c.IsRootType = true;

                    //
                    //  Always fail out here, the previous code will take care of doing a real success
                    //
                    //  Don't let Type potentially make another match.  One is sufficient.
                    //

                    $c.result = MatchResult.Success;
                    $$ = null;
                    return MatchResult.Fail;
                };

ParameterizedValueAssignment ::= valueReferenceLHS:a ParameterList:b Type:c "::=":d 
                {
                    MatchResult mr;
                    
                    //
                    //  Parameter list is not allowed to affect the type.
                    //

                    if (history.sym == null) {
                        //
                        //  First time executing this action, do the full setup.
                        //

                        history.sym = new Symbol($a.name, SymbolType.ParameterizedValue);
                        history.sym.type = $c;
                        history.type = $c;

                        //
                        //  Figure out the number of passes we need to make
                        //  This is based on the potential paraemters in the list.
                        //      i.e (FOO, FOO:bar) - FOO could be either a type or a class
                        //

                        $d.rv.paramlsts = BuildPotentialParameterLists($b);
                        
                    }

                    //
                    //  Do we have any more lists of parameters to parse? if so then
                    //  continue with the next item, otherwise execute the finish code
                    //

                    if ($d.rv.paramlsts.Count > 0) {
                        history.paramList = $d.rv.paramlsts[0];
                        $d.rv.paramlsts.Remove(history.paramList);

                        $$ = null;
                        return MatchResult.ChoiceLeft;
                    }

                    //
                    //  If we have sucecessfully reduced any time, then collapse down the tokens
                    //   and return the fact that we were successful.
                    //  Otherwise fail
                    //

                    if (history.sym.parameterization != null) {
                        Debug.Assert(history.sym.parameterization.Count != 0);

                        //
                        // Insert into the symbol table
                        //
                        
                        m_symbols.Add(history.sym);

                        //
                        //  Collapse the nodes
                        //

                        ReplaceTokens(history[0].iToken, $d.tokenCount, new Token(RuleNo.ParameterizedValueAssignment, 0, 0));
                        cTokensUsed = 1;

                        mr = MatchResult.Reduced;
                    }
                    else {
                        // Never made a match

                        mr = MatchResult.Fail;
                    }
                        
                    $$ = null;
                    return mr;
                }
                Value:e
                {
                    //
                    //  Put together the pieces and place them into the symbol table.
                    //
                    
                    if (history.sym.parameterization == null) history.sym.parameterization = new List<ParamItems>();
                    history.sym.parameterization.Add(new ParamItems(history.paramList, $e));
                    $d.tokenCount = cTokensUsed;

                    //
                    //  We only need one successful match per parameter list.
                    //
                    
                    $c.result = MatchResult.Success;

                    //
                    //  Retry the next parameter list
                    //
                    
                    $$ = null;
                    return MatchResult.Fail;
                }
                ;

ParameterizedValueSetTypeAssignment ::= typereferenceLHS ParameterList Type "::=" ValueSet ;
ParameterizedObjectClassAssignment ::= objectclassreferenceLHS ParameterList "::=" ObjectClass ;

ParameterizedObjectAssignment ::= objectreferenceLHS:a ParameterList:b DefinedObjectClass:c "::=":d
                {
                    MatchResult mr;

                    //
                    //  Parameter list is not allowed to affect the class
                    //

                    if (history.sym == null) {
                        //
                        //  First time in - do all of the setup
                        //

                        history.sym = new Symbol($a.name, SymbolType.ParameterizedObject);
                        history.sym.type = $c;
                        history.type = $c;

                        //
                        //  Don't go back to the parameter list
                        //

                        $b.result = MatchResult.Success;

                        //
                        //  Figure out the number of passes we need to make
                        //  This is based on the potential paraemters in the list.
                        //      i.e (FOO, FOO:bar) - FOO could be either a type or a class
                        //

                        $d.rv.paramlsts = BuildPotentialParameterLists($b);
                    }

                    //
                    //  Are there any parameter lists left to check?
                    //  If so then setup to check that item
                    //

                    if ($d.rv.paramlsts.Count > 0) {
                        history.paramList = $d.rv.paramlsts[0];
                        $d.rv.paramlsts.Remove(history.paramList);

                        $$ = null;
                        return MatchResult.ChoiceLeft;
                    }

                    //
                    //  If we have sucecessfully reduced any time, then collapse down the tokens
                    //   and return the fact that we were successful.
                    //  Otherwise fail
                    //

                    if (history.sym.parameterization != null) {
                        Debug.Assert(history.sym.parameterization.Count != 0);

                        //
                        // Insert into the symbol table
                        //
                        
                        m_symbols.Add(history.sym);

                        //
                        //  Collapse the nodes
                        //

                        ReplaceTokens(history[0].iToken, $d.tokenCount, new Token(RuleNo.ParameterizedObjectAssignment, 0, 0));
                        cTokensUsed = 1;

                        mr = MatchResult.Reduced;
                    }
                    else {
                        // Never made a match

                        mr = MatchResult.Fail;
                    }
                        
                    $$ = null;
                    return mr;
                    
                }
                Object:e
                {
                    //
                    //  Put together the pieces and place them into the symbol table.
                    //
                    
                    if (history.sym.parameterization == null) history.sym.parameterization = new List<ParamItems>();
                    history.sym.parameterization.Add(new ParamItems(history.paramList, $e));
                    $d.tokenCount = cTokensUsed;

                    //
                    //  We only need one successful match per parameter list.
                    //
                    
                    $c.result = MatchResult.Success;

                    //
                    //  Retry the next parameter list
                    //
                    
                    $$ = null;
                    return MatchResult.Fail;
                }
                ;

ParameterizedObjectSetAssignment ::= objectsetreferenceLHS ParameterList DefinedObjectClass "::=" ObjectSet ; 
            
//  SECTION 8.3
//
ParameterList ::= "{" 
                {
                    $$ = null;
                    return PreParseParameterList(history);
                    
                }
                Parameter "," + "}"
                {
                    if (ParameterList_Action(ref history, out rvOut) == MatchResult.Fail) {
                        return MatchResult.Fail;
                    }
                    
                }
                ;

Parameter ::=   ParamGovernor:a ":" DummyReference:b
                {
                    //
                    //  Return the parameter after assembling the pieces
                    //

                    $$ = new RuleValue(new Parameter($a, $b));
                }
                / DummyReference:b
                {
                    //
                    //  Build the item into a parameter and return
                    //

                    $$ = new RuleValue(new Parameter(null, $b));
                }
                ;

ParamGovernor ::= Governor / DummyGovernor ;

Governor ::=    Type:a
                {
                    $$ = new RuleValue(new Symbol(" Governor Type", SymbolType.Governor));
                    $$.type = $a;
                }
                / DefinedObjectClass:a
                {
                    $$ = new RuleValue(new Symbol(" Governor Class", SymbolType.Governor));
                    $$.type = $a;
                };

DummyGovernor ::= DummyReference:a
                {
                    $a.symType = SymbolType.Governor;
                    $$ = $a.rv;
                };

DummyReference ::= ReferenceLHS:a
                {
                    Symbol sym;

                    if (char.IsUpper($a.name[0])) {
                        sym = new Symbol($a.name, SymbolType.UnknownDummy);
                    }
                    else {
                        sym = new Symbol($a.name, SymbolType.UnknownDummy);
                    }
                    $$ = new RuleValue(sym);
                };

            // This is always a new identifier, unless it comes from
            // DummyGovernor in which case it could be an existing identifier
            // Symbol type is going to have to be determined at a later date.

//  SECTION 9.1
//
// ParameterizedReference ::= Reference | Reference "{" "}" ;

ParameterizedReference ::= ReferenceLHS:a "{" "}"
                {
                    $$ = new RuleValue(new Symbol($a.name, SymbolType.Parameterized | SymbolType.Unknown | SymbolType.Imported));
                } ;


//  SECTION 9.2
//
ParameterizedType ::= SimpleDefinedType:a
                {
                    if (!$a.IsType(SymbolType.ParameterizedType)) FAIL;

                    //
                    //  Place the parameter list in the current context so we have
                    //  the name and type of each parameter for evaluation
                    //

                    Debug.Assert($a.parameterization != null);
                    Debug.Assert($a.parameterization.Count == 1);

                    history.paramList = $a.parameterization[0].paramList;
                    Debug.Assert(history.paramList != null);

                    $$ = null;
                }
                ActualParameterList:b
                {
                    $$ = new RuleValue($a.InstanceOfType($b));
                }
                ;

SimpleDefinedType ::= ExternalTypeReference:a
                {
                    if (($a.GetSymbolType() != SymbolType.ParameterizedType) && 
                        ($a.GetSymbolType() != SymbolType.ParameterizedValue)) FAIL;

                    $$ = $a.rv;
                }
                | typereference:a
                {
                    if (($a.GetSymbolType() != SymbolType.ParameterizedType) && 
                        ($a.GetSymbolType() != SymbolType.ParameterizedValue)) FAIL;

                    $$ = $a.rv;
                }
                ;

ParameterizedValue ::= SimpleDefinedValue:a 
                {
                    if ($a.sym.symType != SymbolType.ParameterizedValue) FAIL;

                    //
                    //  Place the parameter list in the current context so we have
                    //  the name and type of each parameter for evaluation
                    //

//                    history.paramList = $a.paramList;
                    Debug.Assert(history.paramList != null);

                    $$ = null;
                }
                ActualParameterList:b
                {
                    Debug.Assert(false, "Need to debug this");
                    $$ = null;
                }
                ;

SimpleDefinedValue ::= ExternalValueReference | valueReference ;

ParameterizedValueSetType ::= SimpleDefinedType:a
                {
                    if ($a.GetSymbolType() != SymbolType.ParameterizedValueSet) FAIL;
                    Debug.Assert(false, "Need to debug this code");

                    //
                    //  Place the parameter list in the current context so we have
                    //  the name and type of each parameter for evaluation
                    //

                    history.paramList = $a.paramList;
                    Debug.Assert(history.paramList != null);
                    
                    $$ = null;
                } ActualParameterList:b
                {
                    $$ = new RuleValue($a.InstanceOfType($b));
                };

ParameterizedObjectClass ::= DefinedObjectClass { $$=null; FAIL; } ActualParameterList ;
ParameterizedObjectSet ::= DefinedObjectSet { $$=null; FAIL; } ActualParameterList  ;
ParameterizedObject ::= DefinedObject { $$=null; FAIL; }ActualParameterList ;

//  SECTION 9.5
//
ActualParameterList ::= "{" ActualParameterList1:a  "}" { $$ = $a.rv; };

ActualParameterList1 ::= ActualParameter "," + ;

ActualParameter ::= Type | Value | ValueSet | DefinedObjectClass | Object | ObjectSet ;


// ****************************************************************************
//
//                                      X.681 Annex A
//
// ***************************************************************************

TypeIdentifier ::= "{" Type:a "IDENTIFIED" BY 
                {
                    //
                    //  The value we are going to locate must be an OID value
                    //
                    
                    history.type = Type.ObjectIdentifier;
                    $$ = null;
                }
                OIDValue:b "}" 
                {
                    //
                    //  Grab the two values and assoicate them with the correct field
                    //  names in the system.
                    //
                    
                    SymbolList list = new SymbolList();
                    Symbol sym;

                    sym = new Symbol("&id", SymbolType.Value);
                    sym.value = $b;
                    sym.type = Type.ObjectIdentifier;
                    list.Add(sym);
                    
                    sym = new Symbol("&Type", SymbolType.Type);
                    sym.type = $a;
                    list.Add(sym);

                    $$ = new RuleValue(new Value(list));
                };

OIDValue ::= ObjectIdentifierValue | DefinedValue ;

AbstractSyntax ::= "{" Type:a "IDENTIFIED" BY OIDValue AS1:b ?
                {
                    Debug.Assert(false, "NYI");

                    $$ = null;
                };

AS1 ::=         "HAS" "PROPERTY" AS1Value:a                             { $$ = $a.rv; } ;

AS1Value ::= BitStringValue | DefinedValue ;

//  Stuff we have added

Commands ::=    Command Commands 
                / Command ! CMDSTART
                ;

Command ::=     CMDSTART CommandBody
                {
                    //
                    //  Collapse out the tokens we just consumed
                    //
                    
                    ReplaceTokens(history[0].iToken, cTokensUsed, new Token(RuleNo.Command, 0, 1));
                    cTokensUsed = 1;

                    //  No return value

                    $$ = null;
    
                };

CommandBody ::= NameSpaceCmd
                / BinaryCmd
                / HugeIntegerCmd
                / NativeIntegerCmd
                / HugeEnumeratedCmd
                / NativeEnumeratedCmd
                / PointerCmd
                / UseFunctionCmd
                / UseTypeCmd
                / AddToObjectSetCmd
                / EmitStringCmd
                / ViewCmd
                ;

NameSpaceCmd ::=   NAMESPACE ":" mstring:a  CMDEND 
                {
                    m_strNamespace = $a.name;
                    
                    $$ = null;
                };

BinaryCmd ::=   BINARY ItemSpec:a CMDEND 
                {
                    if ($a.type.baseType == BaseType.Assignment) {
                        $a.type.IsAtomic = true;
                    }
                    else {
                        Debug.Assert(false, "NYI: BinaryCmd support");
                    }
                    $$ = null;
                };

HugeIntegerCmd ::= HUGEINTEGER:b ItemSpec:a ? CMDEND 
                {
                    //
                    //  No return value from here
                    //
                    
                    $$ = null;

                    //
                    //  Change the type on the destination if you can.
                    //
                    
                    if ($a.result == MatchResult.Fail) {
                        //
                        //  No type was specified - warn if this is not the first time
                        //
                        
                        if (m_DefaultIntegerType.emitName == null) {
                            m_DefaultIntegerType.emitName = "A2C_INTEGER_HUGE";
                        }
                        else {
                            $b.rv.AddWarning(new Error(ErrorNumber.HugeIntegerTwice, $b.token));
                        }
                    }
                    else {
                        RuleValue rv = null;
                        MatchResult mr;
                        
                        mr = StdTypeReplacement($a, $a.iTokenLast, m_DefaultIntegerType, Type.IntegerType, Type.IntegerNative, "HugeInteger", ref rv);
                        $$ = rv;
                        return mr;
                    }
                };

NativeIntegerCmd ::= NATIVEINTEGER:b ItemSpec:a ? CMDEND 
                {
                    //
                    //  No return value from here
                    //
                    
                    $$ = null;

                    //
                    //  If no tag - then we modify the default int
                    //
                    
                    if ($a.result == MatchResult.Fail) {
                        //
                        //  No type was specified - warn if this is not the first time
                        //
                        
                        if (m_DefaultIntegerType.emitName == null) {
                            m_DefaultIntegerType.emitName = "A2C_ENUMERATED_HUGE";
                        }
                        else {
                            $b.rv.AddWarning(new Error(ErrorNumber.HugeIntegerTwice, $b.token));
                        }
                    }
                    else {
                        RuleValue rv = null;
                        MatchResult mr;
                        
                        mr = StdTypeReplacement($a, $a.iTokenLast, m_DefaultIntegerType, Type.IntegerNative, Type.IntegerType, "NativeInteger", ref rv);
                        $$ = rv;
                        return mr;
                    }
                }
                ;

HugeEnumeratedCmd ::= HUGEENUMERATED:b ItemSpec:a ? CMDEND 
                {
                    //
                    //  No return value from here
                    //
                    
                    $$ = null;

                    //
                    //  If no tag - then we modify the default int
                    //
                    
                    if ($a.result == MatchResult.Fail) {
                        //
                        //  No type was specified - warn if this is not the first time
                        //
                        
                        if (m_DefaultEnumeratedType.emitName == null) {
                            m_DefaultEnumeratedType.emitName = "A2C_ENUMERATED_HUGE";
                        }
                        else {
                            $b.rv.AddWarning(new Error(ErrorNumber.HugeIntegerTwice, $b.token));
                        }
                    }
                    else {
                        RuleValue rv = null;
                        MatchResult mr;
                        
                        mr = StdTypeReplacement($a, $a.iTokenLast, m_DefaultEnumeratedType, Type.EnumeratedType, Type.EnumeratedNative, "HugeEnumerated", ref rv);
                        $$ = rv;
                        return mr;
                    }
                };

NativeEnumeratedCmd ::= NATIVEENUMERATED:b ItemSpec:a ? CMDEND 
                {
                    //
                    //  No return value from here
                    //
                    
                    $$ = null;

                    //
                    //  If no tag - then we modify the default int
                    //
                    
                    if ($a.result == MatchResult.Fail) {
                        //
                        //  No type was specified - warn if this is not the first time
                        //
                        
                        if (m_DefaultEnumeratedType.emitName == null) {
                            m_DefaultEnumeratedType.emitName = "A2C_ENUMERATED_NATIVE";
                        }
                        else {
                            $b.rv.AddWarning(new Error(ErrorNumber.HugeIntegerTwice, $b.token));
                        }
                    }
                    else {
                        RuleValue rv = null;
                        MatchResult mr;
                        
                        mr = StdTypeReplacement($a, $a.iTokenLast, m_DefaultEnumeratedType, Type.EnumeratedNative, Type.EnumeratedType, "NativeEnumerated", ref rv);
                        $$ = rv;
                        return mr;
                    }
                }
                ;

PointerCmd ::=  POINTER ItemSpec:a CMDEND
                {
                    $$ = null;
                    
                    /*
                     *  Must point to a field
                     */

                    if ($a.GetSymbolType() != SymbolType.Component) {
                        $a.rv.AddWarning(new Error(ErrorNumber.MustBeField, m_rgTokens[$a.iTokenLast]));
                    }
                    else {
                        $a.flags.Pointer = true;
                    }
                };

UseFunctionCmd ::= USEFUNCTION ItemSpec:a mstring:b mstring:c CMDEND
                {
                    /*
                     *  No return
                     */

                    $$ = null;
                    
                    /*
                     *  Must point to a type
                     */

                    if ($a.GetSymbolType() != SymbolType.Type) {
                        $a.rv.AddWarning(new Error(ErrorNumber.MustBeType, m_rgTokens[$a.iTokenLast]));
                    }

                    /*
                     *  We are going to check that it is a legal value for the tag
                     */

                    switch ($b.name) {
                    case "Validate":
                    case "EncodeBER":
                    case "DecodeBER":
                    case "EncodeDER":
                    case "DecodeDER":
                    case "Copy":
                    case "Compare":
                    case "Init":
                    case "Release":
                    case "Print":
                        $a.codenames.SetUserFunctionName($b.name, $c.name);
                        break;
                    
                    default:
                        $$ = new RuleValue();
                        $$.AddWarning(new Error(ErrorNumber.UseFunctionTag, m_rgTokens[$b.iTokenLast]));
                        break;
                    }
                };

UseTypeCmd      ::= USETYPE ItemSpec:a mstring:b CMDEND
                {
                    /*
                     *  No return
                     */

                    Debug.Assert(false, "UseTypeCmd");
                    $$ = null;

                    /*
                     *  Must point to a type
                     */

                    if ($a.GetSymbolType() == SymbolType.Type) {
                        $a.codenames.SetUserType($b.name);
                    }
                    else if ($a.GetSymbolType() == SymbolType.Component) {
                        //
                        //  We need to find out what the tagging is on the type so that we can transfer that in to the type
                        //      we are creating just now
                        //

                        Type  typ = $a.type;
                        bool fDone = false;

                        while (!fDone) {
                            switch (typ.baseType) {
                            case BaseType.Leaf:
                            case BaseType.OpenType:
                                fDone = true;
                                break;
                                
                            case BaseType.Tagged:
                            case BaseType.Constraint:
                                typ = typ.subType;
                                break;

                            case BaseType.ObjectClassField:
                                Symbol sym = typ.GetObjectClassField();
                                switch (sym.GetSymbolType()) {
                                case SymbolType.ValueField:
                                    typ = sym.type;
                                    break;

                                case SymbolType.TypeField:
                                    typ = Type.Open;
                                    break;

                                default:
                                    Debug.Assert(false, "Missing case statement - UseType command");
                                    fDone = true;
                                    break;
                                }
                                break;
                                
                            default:
                                Debug.Assert(false, "Missing case statement - UseType command");
                                fDone = true;
                                break;
                            }
                        }

                        Type  typNew = new Type(BaseType.Leaf, typ.tagged /*new Tagging(TagClass.Universal, TagType.Specfied, 0)*/, DataType.Null, $b.name, $b.name); 

                        //
                        //  We want to walk down through some items, but not others to find where we are actually going to make
                        //      the substition of the user's type for our type.
                        //  We currently only walk through tagging.
                        //

                        typ = $a.type;
                        Type  typLast = null;

                        while (typ != null) {
                            switch (typ.baseType) {
                            case BaseType.Tagged:
                                typLast = typ;
                                typ = typ.subType;
                                break;

                            default:
                                typ = null;
                                break;
                            }
                        }

                        if (typLast == null) {
                            $a.type = typNew;
                        }
                        else {
                            typLast = typNew;
                        }
                    }
                    else {
                        Debug.Assert(false, "Where is the warning?");
                        $a.rv.AddWarning(new Error(ErrorNumber.MustBeType, m_rgTokens[$a.iTokenLast]));
                    }

                    /*
                     *
                     */

                };

AddToObjectSetCmd  ::= ADDTOOBJECTSET ":" ModuleName:a ? objectsetreferenceLHS:b DefinedObject:c CMDEND
                {
                    Symbol      sym;
                    
                    if ($a.result != MatchResult.Fail) {
                        //  Find the object set in the module
                        sym = $a.FindSymbol($b.name);
                    }
                    else {
                        //  Find object set in this module
                        sym = FindSymbol($b.name);
                    }

                    if (sym == null) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, m_rgTokens[$a.iTokenLast]));
                        return MatchResult.Fail;
                    }
                    else if (sym.GetSymbolType() != SymbolType.ObjectSet) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotObjectSet, m_rgTokens[$a.iTokenLast]));
                        return MatchResult.Fail;
                    }

                    //
                    //  List must be of the ame type.
                    //

                    //
                    //  List must be extensible
                    //

                    //
                    //  Add the item to the end of the list
                    //

                    sym.value.valueList.Add($c);
                    $$ = null;
                }
                / ADDTOOBJECTSET ":" ModuleName:a ? objectsetreferenceLHS:b DefinedObjectSet:c CMDEND
                {
                    Symbol      sym;
                    
                    if ($a.result != MatchResult.Fail) {
                        //  Find the object set in the module
                        sym = $a.FindSymbol($b.name);
                    }
                    else {
                        //  Find object set in this module
                        sym = FindSymbol($b.name);
                    }

                    if (sym == null) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, m_rgTokens[$a.iTokenLast]));
                        return MatchResult.Fail;
                    }
                    else if (sym.GetSymbolType() != SymbolType.ObjectSet) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotObjectSet, m_rgTokens[$a.iTokenLast]));
                        return MatchResult.Fail;
                    }

                    //
                    //  List must be of the ame type.
                    //

                    //
                    //  List must be extensible
                    //

                    //
                    //  Add the item to the end of the list
                    //

                    sym.value.valueList.Add($c);
                    $$ = null;

                }
                ;

EmitStringCmd ::= EMITSTRING ":" ModuleName:c ? mstring:a mstring:b CMDEND
                {
                    AsnFile  af = this;

                    if ($c.result != MatchResult.Fail) {
                        af = $c;
                    }
                    
                    //
                    //  We must be inside of a module in order to have this command
                    //
                    
                    if (($a.name == "h") || ($a.name == "H")) {
                        if (af.m_mpszszEmitData == null) {
                            af.m_mpszszEmitData = new Dictionary<string, string>();
                        }
                        if (af.m_mpszszEmitData.ContainsKey("h")) {
                            af.m_mpszszEmitData["h"] = af.m_mpszszEmitData["h"] + "\n" + $b.name;
                        }
                        else {
                            af.m_mpszszEmitData.Add("h", $b.name);
                        }
                    }
                    else if (($a.name == "c") || ($a.name == "C")) {
                        if (af.m_mpszszEmitData == null) {
                            af.m_mpszszEmitData = new Dictionary<string, string>();
                        }
                        if (af.m_mpszszEmitData.ContainsKey("c")) {
                            af.m_mpszszEmitData["c"] = m_mpszszEmitData["c"] + "\n" + $b.name;
                        }
                        else {
                            af.m_mpszszEmitData.Add("c", $b.name);
                        }
                    }
                    else {
                        $$ = new RuleValue(new Error(ErrorNumber.InvalidEmitString, m_rgTokens[$b.iTokenLast]));
                        return MatchResult.Fail;
                    }

                    //
                    //  No return value
                    //

                    $$ = null;
                };

ViewCmd ::= VIEW ItemSpec:a CMDEND
                {
                    //  No return value from here

                    $$ = null;

                    //
                    
                    $a.flags.View = true;
                };

ItemSpec ::=    ":" ModuleName:a ? typereferenceLHS:b  ItemList1:c ?
                {
                    bool        f;
                    Symbol      sym;
                    Type        type;
                    
                    if ($a.result != MatchResult.Fail) {
                        //  Find type in module
                        sym = $a.FindSymbol($b.name);
                    }
                    else {
                        //  Find type in this module
                        sym = FindSymbol($b.name);
                    }
                    
                    if (sym == null) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotFound, m_rgTokens[$b.iTokenLast]));
                        return MatchResult.Fail;
                    }
                    else if (sym.GetSymbolType() != SymbolType.Type) {
                        $$ = new RuleValue(new Error(ErrorNumber.SymbolNotType, m_rgTokens[$b.iTokenLast]));
                        return MatchResult.Fail;
                    }


                    //
                    //  If there is a list of tokens, trace down the list of tokens
                    //

                    if ($c.result != MatchResult.Fail) {
                        foreach (Token tkn in $c) {
                            //
                            //  Skip fluff
                            //

                            type = sym.type;
                            type = type.SkipFluff();
                            if (type == null) {
                                $$ = new RuleValue(new Error(ErrorNumber.ICE, tkn));
                                return MatchResult.Fail;
                            }

                            //
                            //  Symbol must be a choice, sequence or set
                            //

                            if ((type.baseType != BaseType.Choice) && (type.baseType != BaseType.Sequence) &&
                                (type.baseType != BaseType.Set)) {
                                    $$ = new RuleValue(new Error(ErrorNumber.NotChoiceSetSeq, m_rgTokens[$c.iTokenLast]));
                                    return MatchResult.Fail;
                                }

                            //

                            f = false;
                            foreach (Symbol symField in type.fieldList) {
                                if (symField.name == tkn.name) {
                                    sym = symField;
                                    f = true;
                                }
                            }

                            if (!f) {
                                $$ = new RuleValue(new Error(ErrorNumber.FieldNotFound, tkn));
                                return MatchResult.Fail;
                            }
                        }
                    }

                    $$ = new RuleValue(sym);
                };

ItemList1 ::=   "." L_Identifier:a ItemList1:b
                {
                    $$ = new RuleValue($b);
                    $$.Insert(0, $a);
                }
                / "." L_Identifier:a ! "."
                {
                    $$ = new RuleValue(new TokenList($a));
                };

ModuleName ::=  moduleReference:a "." 
                {
                    $$ = null;
                    foreach (AsnFile f in m_lstModules) {
                        if ($a.name == f.moduleName) {
                            $$ = new RuleValue(f);
                            break;
                        }
                    }
                    if ($$ == null) return MatchResult.Fail;
                };


DnnAssignment ::=		DnnTypeAssignment | DnnValueAssignment | DnnValueSetTypeAssignment | DnnObjectClassAssignment | DnnObjectAssignment |  DnnObjectSetAssignment ;

DnnTypeAssignment ::=		U_Identifier MatchedBraces?             "::=" DnnType ;
DnnObjectClassAssignment ::=	U_Identifier MatchedBraces?             "::=" DnnObjectClass ;
DnnValueSetTypeAssignment ::=	U_Identifier MatchedBraces? DnnType      "::=" MatchedBraces ;
DnnObjectSetAssignment ::=	U_Identifier MatchedBraces? DnnDefinedObjectClass "::=" MatchedBraces ;

DnnValueAssignment ::=		L_Identifier MatchedBraces? DnnType      "::=" DnnValue ;
DnnObjectAssignment ::=		L_Identifier MatchedBraces? DnnDefinedObjectClass "::=" DnnObject ;

DnnType ::=			DnnPrimativeType DnnConstraintList? / DnnTypeWithConstraint ;
DnnPrimativeType ::=		DnnBuiltinType  / DnnReferencedType ;
DnnConstraintList ::=		MatchParens Empty *;
DnnBuiltinType ::=		DnnBitStringType / BOOLEAN / DnnCharacterStringType / EMBEDDED PDV / EXTERNAL / DnnInstanceOfType / DnnIntegerType / NULL / DnnObjectClassFieldType / OBJECT IDENTIFIER / OCTET STRING / REAL / RELATIVE-OID / DnnSequenceType / DnnSequenceOfType / DnnTaggedType ;
DnnReferencedType ::=		DnnDefinedType | DnnUsefulType | DnnSelectionType | DnnTypeFromObject | DnnValueSetFromObjects ;
DnnBitStringType ::=		BIT STRING MatchedBraces / BIT STRING ;
DnnInstanceOfType ::=		INSTANCE OF DnnDefinedObjectClass ;
DnnIntegerType ::=		INTEGER MatchedBraces / INTEGER ;
DnnObjectClassFieldType ::=	DnnDefinedObjectClass "." DnnFieldName ;
DnnSetOrSequence ::=            SET / SEQUENCE / CHOICE / ENUMERATED ;
DnnSequenceType ::=		DnnSetOrSequence MatchedBraces ;
DnnSequenceOfType ::=		DnnSetOrSequence OF DnnTypeOrNamedType ;
DnnTaggedType ::=		DnnTag IMPLICIT DnnType / DnnTag EXPLICIT DnnType / DnnTag DnnType ;
DnnTypeWithConstraint ::=	DnnSetOrSequence SIZE ? MatchParens OF DnnTypeOrNamedType ;
DnnDefinedType ::=		DnnParameterizedType / DnnUIdDot ? U_Identifier ;
DnnUsefulType ::=		GeneralizedTime / UTCTime / ObjectDescriptor ;
DnnSelectionType ::=		L_Identifier "<" DnnType ;
DnnTypeFromObject ::=		DnnReferencedObjects "." DnnFieldName ;
DnnValueSetFromObjects ::=	DnnReferencedObjects "." DnnFieldName ;
DnnCharacterStringType ::=	BMPString / GeneralString / GraphicString / IA5String / ISO646String / NumericString / PrintableString / TeletexString / T61String / UniversalString / UTF8String / VideotexString / VisibleString / CHARACTER STRING ;
DnnDefinedObjectClass ::=	DnnUIdDot ? U_Identifier / DnnUsefulObjectClassReference ;
DnnFieldName ::=		DnnPrimitiveFieldName "." + ;
DnnPrimitiveFieldName ::=	typefieldreferenceLHS | valuefieldreferenceLHS | valuesetfieldreferenceLHS | objectfieldreferenceLHS | objectsetfieldreferenceLHS ;
DnnTag ::=			"[" DnnTag1 Empty * "]" ;
DnnTag1 ::=                      ! "]" ANY ;
DnnParameterizedType ::=	DnnSimpleDefinedType MatchedBraces ;
DnnReferencedObjects ::=	DnnParameterizedObject | DnnParameterizedObjectSet | DnnDefinedObject | DnnDefinedObjectSet;
DnnUsefulObjectClassReference ::=	TYPE-IDENTIFIER / ABSTRACT-SYNTAX ;
DnnSimpleDefinedType ::=	DnnUIdDot? U_Identifier ;
DnnDefinedObject ::=		DnnUIdDot? L_Identifier ;
DnnParameterizedObjectSet ::=	DnnDefinedObjectSet MatchedBraces ;
DnnDefinedObjectSet ::=		U_Identifier "." U_Identifier | U_Identifier ;
DnnValue ::=			DnnObjectClassFieldValue | DnnBuiltinValue   | DnnReferencedValue ;
DnnBuiltinValue ::=		DnnBitStringValue | DnnBooleanValue | DnnCharacterStringValue | DnnChoiceValue | MatchedBraces | L_Identifier | DnnIntegerValue | NULL | DnnOctetStringValue | DnnRealValue ;
DnnReferencedValue ::=		DnnDefinedValue | DnnValueFromObject ;
DnnObjectClassFieldValue ::=	DnnOpenTypeFieldVal | DnnFixedTypeFieldVal ;
DnnOpenTypeFieldVal ::=		DnnType ":" DnnValue ;
DnnFixedTypeFieldVal ::=	DnnBuiltinValue | DnnReferencedValue ;
DnnBitStringValue ::=		bstring / hstring / MatchedBraces / CONTAINING DnnValue ;
DnnBooleanValue::=		TRUE / FALSE ;
DnnCharacterStringValue ::=	cstring / MatchedBraces ;
DnnChoiceValue ::=		L_Identifier ":" DnnValue ;
DnnIntegerValue ::=		DnnSignedNumber  / L_Identifier ;
DnnSignedNumber ::=		number  / "-" number ;
DnnOctetStringValue ::=		bstring  / hstring  / CONTAINING DnnValue ;
DnnRealValue ::=		DnnNumericRealValue  / DnnSpecialRealValue ;
DnnNumericRealValue ::=		realnumber / "-" realnumber / MatchedBraces ;
DnnSpecialRealValue ::=		PLUS-INFINITY  / MINUS-INFINITY ;
DnnDefinedValue ::=		DnnParameterizedValue | U_Identifier "." L_Identifier | L_Identifier ;
DnnValueFromObject ::=		DnnReferencedObjects "." DnnFieldName ;
DnnParameterizedValue ::=	DnnSimpleDefinedValue MatchedBraces ;
DnnSimpleDefinedValue ::=	U_Identifier "." L_Identifier  | L_Identifier ;
DnnObject ::=			DnnParameterizedObject | DnnDefinedObject | MatchedBraces | DnnObjectFromObject;
DnnObjectFromObject ::=		DnnReferencedObjects "." DnnFieldName ;
DnnParameterizedObject ::=	DnnDefinedObject MatchedBraces ;
DnnObjectClass ::=		DnnParameterizedObjectClass | DnnDefinedObjectClass | DnnObjectClassDefn  ;
DnnObjectClassDefn ::=		CLASS MatchedBraces DnnWithSyntaxSpec? ;
DnnParameterizedObjectClass ::=	DnnDefinedObjectClass MatchedBraces ;
DnnWithSyntaxSpec ::=		WITH SYNTAX MatchedBraces ;
DnnTypeOrNamedType ::=          L_Identifier? DnnType ;
DnnUIdDot ::=                   U_Identifier "." ;


MatchedBraces ::=               "{" MB1 Empty * "}" ;
MB1 ::=                         "{" MB1 Empty * "}" / ! "{" ! "}" ANY;

MatchParens ::=                 "(" MP1 Empty * ")" ;
MP1 ::=                         "(" MP1 Empty * ")" / ! ")" ! ")" ANY ;


%%

namespace asn_compile_cs 
{
    partial class AsnFile
    {
        MatchResult ParameterList_Action(ref HistoryStack history, out RuleValue rvOut)
        {
            Boolean f;
            MatchResult err = MatchResult.Success;
            ErrorList errlst = new ErrorList();
            int i;
            SymbolList lst;
            

//  SECTION 8.3
            //
            // "{" Parameter "," + "}"
            //
            //

            Debug.Assert(history[0].rv.IsToken());
            Debug.Assert(history[2].choice == RuleType.Plus2);
            Debug.Assert(history[history.Count - 2].rv.IsToken());
            
            Token tknLast = history[history.Count-2].rv.token;

            //
            //  Validate the different rules that occur at this point on the list of parameters.
            //
            //  A "DummyGovernor" shall be a "DummyReference" that has no "Governor"
            //
            //  A "DummyReference" 
            //      No ParamGovernor if Type or DefinedObjectClass
            //      Value/ValueSet --> ParamGovernor is a Type
            //      Object/ObjectSet --> ParamGovernor is a DefinedObjectClass
            //
            //  If you find a DummyGovernor which is not also a reference then 
            //      return an error as it needs to become a defined type a some point.
            //

            lst = new SymbolList();

            for (i = 3; i < history.Count - 2; i += 2) {
                Debug.Assert(history[i].rv.IsParameter());

                Parameter param = history[i].rv.param;

                //
                //  Cases are:
                //  1.  DummyReference
                //              - No governor exits - must be a type parameter
                //  2.  DummyGovernor : DummyReference
                //              - Both type and item are dummys.
                //              - identified by name=null
                //  3.  Type : DummyReference
                //              - known fixed type
                //              - identified by name=" Governor Type"
                //  4.  ObjectClass : DummyReference
                //              - known fixed object type
                //              - identified by name=" Governor Class"
                //

                //
                //  There is no governor --> This must be a TYPE!
                //
                
                if (param.m_governor == null) {
                    //
                    //  If all characters are upper case, then it could be
                    //  a Class or a type
                    //

                    param.m_reference.symType = SymbolType.TypeOrClassParameter;
                    for (int i1=0; i1<param.m_reference.name.Length; i1++) {
                        if (Char.IsLower(param.m_reference.name[i1])) {
                            param.m_reference.symType = SymbolType.TypeParameter;
                            break;
                        }
                    }
                }

                //
                // Should never get to here
                //
                
                else if (param.m_governor.name == null) {
                    Debug.Assert(false);
                }

                //
                //  Check for DefinedObjectClass case
                //      Case #4
                //

                else if (param.m_governor.name == " Governor Class") {
                    param.m_reference.type = param.m_governor.type;
                    if (Char.IsUpper(param.m_reference.name[0])) {
                        param.m_reference.symType = SymbolType.ObjectSetParameter;
                    }
                    else {
                        param.m_reference.symType = SymbolType.ObjectParameter;
                    }
                }

                //
                //  It's a simple type.
                //      Case #3
                //
                
                else if (param.m_governor.name == " Governor Type") {
                    param.m_reference.type = param.m_governor.type;
                    if (Char.IsUpper(param.m_reference.name[0])) {
                        param.m_reference.symType = SymbolType.ValueSetParameter;
                    }
                    else {
                        param.m_reference.symType = SymbolType.ValueParameter;
                    }
                }

                //
                //  case #2
                //
                
                else {
                    param.m_reference.type = new Type(param.m_governor);
                    if (Char.IsUpper(param.m_reference.name[0])) {
                        param.m_reference.symType = SymbolType.ValueSetOrObjectSetParameter;
                    }
                    else {
                        param.m_reference.symType = SymbolType.ValueOrObjectParameter;
                    }
                }

                //
                //  Make sure that no duplicate parameters
                //

                if (lst.Count > 0) {
                    f = false;
                    foreach (Symbol sym in lst) {
                        if (sym.name == param.m_reference.name) {
                            f = true;
                            break;
                        }
                    }
                    if (f) {
                        errlst.Add(new Error(ErrorNumber.DuplicateParameter, tknLast, param.m_reference.name));
                        err = MatchResult.Fail;
                    }
                }


                lst.Add(param.m_reference);
                
            }

            //
            //  Check that all dummy governors are defined
            //

            foreach (Symbol sym in lst) {
                if (((sym.GetSymbolType() == SymbolType.ValueParameter) ||
                     (sym.GetSymbolType() == SymbolType.ObjectSetParameter)) &&
                    (sym.type.baseType != BaseType.Assignment) &&
                    (sym.type.baseType != BaseType.Leaf)) {
                    
                    f = false;
                    foreach (Symbol sym1 in lst) {
                        if ((sym1.GetSymbolType() == SymbolType.TypeParameter) &&
                            (sym1.name == sym.type.sym.name)) {
                            f = true;
                            break;
                        }
                    }
                    if (!f) {
                        err = MatchResult.Fail;
                        errlst.Add(new Error(ErrorNumber.UndefinedDummyGovernor, tknLast, sym.type.sym.name));
                    }
                }
            }
                
            if (err == MatchResult.Success) {
                rvOut = new RuleValue(lst);
            }
            else {
                rvOut = new RuleValue(errlst);
            }

            return err;
        }

    }
}


namespace asn_compile_cs {
    partial class AsnFile
    {
        MatchResult ObjectDefn(int iTokenIn, int iLevel, ref HistoryStack history, out int cTokensConsumed, out RuleValue rvOut)
        {
            MatchResult mr;
            RuleNo rn = RuleNo.DefaultSyntax;
            // Symbol sym;
            Type type;
            Type typeRoot;

            //
            //  There is a choice here - either a default syntax or a defined syntax.  You only get to use one of
            //      these based on the definition of the object
            //

            typeRoot = m_context.GetTopType();
            type = typeRoot.SkipFluff();

            if (type.syntax != null) {
                rn = type.syntax.ruleNo;
            }

            mr = MatchRules(rn, iLevel, iTokenIn, ref history, out cTokensConsumed, out rvOut);
            Debug.Assert((mr == MatchResult.Fail) || rvOut.IsValue());
            if (mr != MatchResult.Fail) {
                mr = MatchResult.Success;
                rvOut.value.type = typeRoot;
            }
            return mr;
        }
    }
}
