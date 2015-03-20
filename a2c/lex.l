using System;
using System.Collections.Generic;
using System.Diagnostics;

%%

%namespace asn_compile_cs
%class Lexer
%unicode
%line
%char
%state cpp_comment
%state dash_comment
%state ModuleCommands
%state CString
%state MString
%type Token

%{
    private Token symbol(TknType type) {
        Token tkn = new Token(type, !fWhiteLast, yyline, yychar);
        fWhiteLast = false;
        return tkn;
    }

    private Stack<int> rgState = new Stack<int>();

    private void yy_push_state(int newState)
    {
        rgState.Push(yy_lexical_state);
        yy_lexical_state = newState;
    }

    private void yy_pop_state()
    {
        Debug.Assert(rgState.Count > 0);
        yy_lexical_state = rgState.Pop();
    }

    private String _CString;
    private bool fWhiteLast = true;

   private Token token_version(TknType type, CompileVersion iVersion) {
      if (Program.args.iVersion < iVersion) {
          // M00BUG - I want to warn about bad key words
          return new Token(TknType.U_Identifier, yytext(), yyline, yychar);
      }
      return symbol(type);
   }
%}

LineTerminator = \n
WhiteSpace = {LineTerminator} | [ \t\f\v\r]
UIdentifier = [A-Z][a-zA-Z0-9]*([-][a-zA-Z0-9]+)*
LIdentifier = [a-z][a-zA-Z0-9]*([-][a-zA-Z0-9]+)*

%%

<YYINITIAL> "ABSENT"            { return symbol(TknType.ABSENT); }
<YYINITIAL> "ABSTRACT-SYNTAX"   { return symbol(TknType.ABSTRACT_SYNTAX); }
<YYINITIAL> "ALL"               { return symbol(TknType.ALL); }
<YYINITIAL> "APPLICATION"       { return symbol(TknType.APPLICATION); }
<YYINITIAL> "AUTOMATIC"         { return symbol(TknType.AUTOMATIC); }
<YYINITIAL> "BEGIN"             { return symbol(TknType.BEGIN); }
<YYINITIAL> "BIT"               { return symbol(TknType.BIT); }
<YYINITIAL> "BMPString"         { return symbol(TknType.BMPString); }
<YYINITIAL> "BOOLEAN"           { return symbol(TknType.BOOLEAN); }
<YYINITIAL> "BY"                { return symbol(TknType.BY); }
<YYINITIAL> "CHARACTER"         { return symbol(TknType.CHARACTER); }
<YYINITIAL> "CHOICE"            { return symbol(TknType.CHOICE); }
<YYINITIAL> "CLASS"             { return symbol(TknType.CLASS); }
<YYINITIAL> "COMPONENT"         { return symbol(TknType.COMPONENT); }
<YYINITIAL> "COMPONENTS"        { return symbol(TknType.COMPONENTS); }
<YYINITIAL> "CONSTRAINED"       { return symbol(TknType.CONSTRAINED); }
<YYINITIAL> "CONTAINING"        { return symbol(TknType.CONTAINING); }
<YYINITIAL> "DATE"              { return token_version(TknType.DATE, CompileVersion.V2008); }
<YYINITIAL> "DATE-TIME"         { return token_version(TknType.DATE_TIME, CompileVersion.V2008); }
<YYINITIAL> "DEFAULT"           { return symbol(TknType.DEFAULT); }
<YYINITIAL> "DEFINITIONS"       { return symbol(TknType.DEFINITIONS); }
<YYINITIAL> "DURATION"          { return token_version(TknType.DURATION, CompileVersion.V2008); }
<YYINITIAL> "EMBEDDED"          { return symbol(TknType.EMBEDDED); }
<YYINITIAL> "ENCODED"           { return symbol(TknType.ENCODED); }
<YYINITIAL> "ENCODING-CONTROL"  { return token_version(TknType.ENCODING_CONTROL, CompileVersion.V2008); }
<YYINITIAL> "END"               { return symbol(TknType.END); }
<YYINITIAL> "ENUMERATED"        { return symbol(TknType.ENUMERATED); }
<YYINITIAL> "EXCEPT"            { return symbol(TknType.EXCEPT); }
<YYINITIAL> "EXPLICIT"          { return symbol(TknType.EXPLICIT); }
<YYINITIAL> "EXPORTS"           { return symbol(TknType.EXPORTS); }
<YYINITIAL> "EXTENSIBILITY"     { return symbol(TknType.EXTENSIBILITY); }
<YYINITIAL> "EXTERNAL"          { return symbol(TknType.EXTERNAL); }
<YYINITIAL> "FALSE"             { return symbol(TknType.FALSE); }
<YYINITIAL> "FROM"              { return symbol(TknType.FROM); }
<YYINITIAL> "GeneralizedTime"   { return symbol(TknType.GeneralizedTime); }
<YYINITIAL> "GeneralString"     { return symbol(TknType.GeneralString); }
<YYINITIAL> "GraphicString"     { return symbol(TknType.GraphicString); }
<YYINITIAL> "IA5String"         { return symbol(TknType.IA5String); }
<YYINITIAL> "IDENTIFIER"        { return symbol(TknType.IDENTIFIER); }
<YYINITIAL> "IMPLICIT"          { return symbol(TknType.IMPLICIT); }
<YYINITIAL> "IMPLIED"           { return symbol(TknType.IMPLIED); }
<YYINITIAL> "IMPORTS"           { return symbol(TknType.IMPORTS); }
<YYINITIAL> "INCLUDES"          { return symbol(TknType.INCLUDES); }
<YYINITIAL> "INSTANCE"          { return symbol(TknType.INSTANCE); }
<YYINITIAL> "INSTRUCTIONS"      { return token_version(TknType.INSTRUCTIONS, CompileVersion.V2008); }
<YYINITIAL> "INTEGER"           { return symbol(TknType.INTEGER); }
<YYINITIAL> "INTERSECTION"      { return symbol(TknType.INTERSECTION); }
<YYINITIAL> "ISO646String"      { return symbol(TknType.ISO646String); }
<YYINITIAL> "MAX"               { return symbol(TknType.MAX); }
<YYINITIAL> "MIN"               { return symbol(TknType.MIN); }
<YYINITIAL> "MINUS-INFINITY"    { return symbol(TknType.MINUS_INFINITY); }
<YYINITIAL> "NOT-A-NUMBER"      { return token_version(TknType.NOT_A_NUMBER, CompileVersion.V2008); }
<YYINITIAL> "NULL"              { return symbol(TknType.NULL); }
<YYINITIAL> "NumericString"     { return symbol(TknType.NumericString); }
<YYINITIAL> "OBJECT"            { return symbol(TknType.OBJECT); }
<YYINITIAL> "ObjectDescriptor"  { return symbol(TknType.ObjectDescriptor); }
<YYINITIAL> "OCTET"             { return symbol(TknType.OCTET); }
<YYINITIAL> "OID-IRI"           { return token_version(TknType.OID_IRI, CompileVersion.V2008); }
<YYINITIAL> "OF"                { return symbol(TknType.OF); }
<YYINITIAL> "OPTIONAL"          { return symbol(TknType.OPTIONAL); }
<YYINITIAL> "PATTERN"           { return symbol(TknType.PATTERN); }
<YYINITIAL> "PDV"               { return symbol(TknType.PDV); }
<YYINITIAL> "PLUS-INFINITY"     { return symbol(TknType.PLUS_INFINITY); }
<YYINITIAL> "PRESENT"           { return symbol(TknType.PRESENT); }
<YYINITIAL> "PrintableString"   { return symbol(TknType.PrintableString); }
<YYINITIAL> "PRIVATE"           { return symbol(TknType.PRIVATE); }
<YYINITIAL> "REAL"              { return symbol(TknType.REAL); }
<YYINITIAL> "RELATIVE-OID"      { return symbol(TknType.RELATIVE_OID); }
<YYINITIAL> "RELATIVE-OID-IRI"  { return token_version(TknType.RELATIVE_OID_IRI, CompileVersion.V2008); }
<YYINITIAL> "SEQUENCE"          { return symbol(TknType.SEQUENCE); }
<YYINITIAL> "SET"               { return symbol(TknType.SET); }
<YYINITIAL> "SIZE"              { return symbol(TknType.SIZE); }
<YYINITIAL> "STRING"            { return symbol(TknType.STRING); }
<YYINITIAL> "SYNTAX"            { return symbol(TknType.SYNTAX); }
<YYINITIAL> "T61String"         { return symbol(TknType.T61String); }
<YYINITIAL> "TAGS"              { return symbol(TknType.TAGS); }
<YYINITIAL> "TeletexString"     { return symbol(TknType.TeletexString); }
<YYINITIAL> "TIME"              { return token_version(TknType.TIME, CompileVersion.V2008); }
<YYINITIAL> "TIME-OF-DAY"       { return token_version(TknType.TIME_OF_DAY, CompileVersion.V2008); }
<YYINITIAL> "TRUE"              { return symbol(TknType.TRUE); }
<YYINITIAL> "TYPE-IDENTIFIER"   { return symbol(TknType.TYPE_IDENTIFIER); }
<YYINITIAL> "UNION"             { return symbol(TknType.UNION); }
<YYINITIAL> "UNIQUE"            { return symbol(TknType.UNIQUE); }
<YYINITIAL> "UNIVERSAL"         { return symbol(TknType.UNIVERSAL); }
<YYINITIAL> "UniversalString"   { return symbol(TknType.UniversalString); }
<YYINITIAL> "UTCTime"           { return symbol(TknType.UTCTime); }
<YYINITIAL> "UTF8String"        { return symbol(TknType.UTF8String); }
<YYINITIAL> "VideotexString"    { return symbol(TknType.VideotexString); }
<YYINITIAL> "VisibleString"     { return symbol(TknType.VisibleString); }
<YYINITIAL> "WITH"              { return symbol(TknType.WITH); }

<YYINITIAL> "{"                 { return symbol(TknType.LBrace); }
<YYINITIAL> "}"                 { return symbol(TknType.RBrace); }
<YYINITIAL> "<"                 { return symbol(TknType.LAngle); }
<YYINITIAL> ">"                 { return symbol(TknType.RAngle); }
<YYINITIAL> ","                 { return symbol(TknType.Comma); }
<YYINITIAL> "."                 { return symbol(TknType.Dot); }
<YYINITIAL> "/"                 { return symbol(TknType.Slash); }
<YYINITIAL> "("                 { return symbol(TknType.LParen); }
<YYINITIAL> ")"                 { return symbol(TknType.RParen); }
<YYINITIAL> "["                 { return symbol(TknType.LSqr); }
<YYINITIAL> "]"                 { return symbol(TknType.RSqr); }
<YYINITIAL> "-"                 { return symbol(TknType.Dash); }
<YYINITIAL> ":"                 { return symbol(TknType.Colon); }
<YYINITIAL> "="                 { return symbol(TknType.Equal); }
<YYINITIAL> "\""                { yy_push_state(CString); _CString = ""; break;}
<YYINITIAL> "'"                 { return symbol(TknType.SQuote); }
<YYINITIAL> ";"                 { return symbol(TknType.SemiColon); }
<YYINITIAL> "@"                 { return symbol(TknType.AtSign); }
<YYINITIAL> "|"                 { return symbol(TknType.VLine); }
<YYINITIAL> "!"                 { return symbol(TknType.Bang); }
<YYINITIAL> "^"                 { return symbol(TknType.Carat); }

<YYINITIAL> "&"                 { return symbol(TknType.And); }
<YYINITIAL> "::="               { return symbol(TknType.Definition); }
<YYINITIAL> "*"                 { return symbol(TknType.Star); }
<YYINITIAL> "_"                 { return symbol(TknType.UScore); }
<YYINITIAL> "--#"               { yy_push_state(ModuleCommands); return symbol(TknType.CMDSTART); }
<YYINITIAL> "\""                { return symbol(TknType.DQuote); }

<YYINITIAL> [1-9][0-9]*         { return new Token((long) Convert.ToInt64(yytext()), yyline, yychar); }
<YYINITIAL> "0"                 { return new Token((long) 0, yyline, yychar); }

<YYINITIAL> [0-9]+(\.)[0-9]+    { return new Token(Convert.ToDouble(yytext()), yyline, yychar); }
<YYINITIAL> [0-9]+[eE][-+](([1-9][0-9]*)|0) { return new Token(Convert.ToDouble(yytext()), yyline, yychar); }
<YYINITIAL> [0-9]+(\.)[0-9]*[eE][-+](([1-9][0-9]*)|0) {
                                  return new Token(Convert.ToDouble(yytext()), yyline, yychar); }

<YYINITIAL> {UIdentifier}       { return new Token(TknType.U_Identifier, yytext(), yyline, yychar); }
<YYINITIAL> "&"{UIdentifier}    { return new Token(TknType.A_U_Identifier, yytext(), yyline, yychar); }
<YYINITIAL> {LIdentifier}       { return new Token(TknType.L_Identifier, yytext(), yyline, yychar); }
<YYINITIAL> "&"{LIdentifier}    { return new Token(TknType.A_L_Identifier, yytext(), yyline, yychar); }

<YYINITIAL> '([0-9A-F]|{WhiteSpace})*'H { return new Token(TknType.HexString, yytext(), yyline, yychar); }
<YYINITIAL> '([01]|{WhiteSpace})*'B { return new Token(TknType.BinaryString, yytext(), yyline, yychar); }

<YYINITIAL> "--"                { yy_push_state(dash_comment); fWhiteLast = true; break; }
<dash_comment> \r               { /* -- eat newline */ yy_pop_state(); break; }
<dash_comment> \n               { /* -- eat newline */ yy_pop_state(); return new Token(TknType.NewLine, yytext(), yyline, yychar); }
<dash_comment> "--"             { yy_pop_state(); break; }
<dash_comment> "-"              { /* -- eat one dash*/ break; }
<dash_comment> [^\r\n\-]+       { /* -- eat line */ break; }
<dash_comment> .                { /* eat */ break; }

<YYINITIAL, cpp_comment> "/*"   { yy_push_state(cpp_comment); fWhiteLast = true; break; }
<cpp_comment> [^*/<]            { /* eat */ break; }
<cpp_comment> "*/"              { yy_pop_state(); break; }
<cpp_comment> .                 { /* eat */ break; }

<YYINITIAL> [ \t\f\r]+          { /* eat whitespace */ fWhiteLast = true; break; }
<YYINITIAL> \n                  { /* eat new line */ fWhiteLast = true; return new Token(TknType.NewLine, yytext(), yyline, yychar); }

<YYINITIAL> .                   { Error err = new Error(ErrorNumber.BadCharacter); err.AddObject(yyline); err.AddObject(yytext()); err.Print(); break; }

<ModuleCommands> "#--"          { yy_pop_state(); fWhiteLast = true; return symbol(TknType.CMDEND); }
<ModuleCommands> "Namespace"    { return symbol(TknType.NAMESPACE); }
<ModuleCommands> "Binary"       { return symbol(TknType.BINARY); }
<ModuleCommands> "HugeInteger"  { return symbol(TknType.HUGEINTEGER); }
<ModuleCommands> "NativeInteger" { return symbol(TknType.NATIVEINTEGER); }
<ModuleCommands> "HugeEnumerated"  { return symbol(TknType.HUGEENUMERATED); }
<ModuleCommands> "NativeEnumerated" { return symbol(TknType.NATIVEENUMERATED); }
<ModuleCommands> "Pointer"      { return symbol(TknType.POINTER); }
<ModuleCommands> "UseFunction"  { return symbol(TknType.USEFUNCTION); }
<ModuleCommands> "UseType"      { return symbol(TknType.USETYPE); }
<ModuleCommands> "AddToObjectSet" { return symbol(TknType.ADDTOOBJECTSET); }
<ModuleCommands> "EmitString"   { return symbol(TknType.EMITSTRING); }
<ModuleCommands> "View"         { return symbol(TknType.VIEW); }
<ModuleCommands> ":"            { return symbol(TknType.Colon); }
<ModuleCommands> [ \t\f\r]+          { /* eat whitespace */ break; }
<ModuleCommands> "\""           { yy_push_state(MString); _CString = ""; break; }
<ModuleCommands> \n             { /* eat new line */ return new Token(TknType.NewLine, yytext(), yyline, yychar); }
<ModuleCommands> {UIdentifier}       { return new Token(TknType.U_Identifier, yytext(), yyline, yychar); }
<ModuleCommands> {LIdentifier}       { return new Token(TknType.L_Identifier, yytext(), yyline, yychar); }
<ModuleCommands> "."            { return symbol(TknType.Dot); }
<ModuleCommands> .              { Error err = new Error(ErrorNumber.BadCharacter); err.AddObject(yyline); err.AddObject(yytext()); err.Print(); break; }


<CString> "\"\""                { _CString += '"'; fWhiteLast = true; break; }
<CString> "\""                  { yy_pop_state(); return new Token(TknType.cstring, _CString, yyline, yychar); }
<CString> [ \t\r]*\n[ \t\r]*    { /* eat this */ break; }
<CString> [ \t]*\r[ \t]*        { /* eat this */ break; }
<CString> [ \t]*                { _CString += yytext(); break; }
<CString> .                     { _CString += yytext(); break; }

<MString> "\"\""                { _CString += '"'; fWhiteLast = true; break; }
<MString> "\""                  { yy_pop_state(); return new Token(TknType.mstring, _CString, yyline, yychar); }
<MString> [ \t]*                { _CString += yytext(); break; }
<MString> .                     { _CString += yytext(); break; }

