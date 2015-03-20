%lexer   Parsing the grammer and output the parser data file

%namespace Recurser

%{
class Globals {
  static public int braceCount;
  static public int yyLine;
}
%}

%define WhiteSpace [ \t\r]+

<YYINITIAL> "%%"    %BREAK { yym.yy_begin("SPACE2"); }
<YYINITIAL> .      %ANY
<YYINITIAL> "\n"    %ANY { Globals.yyLine += 1; }

<SPACE2> "%%"           %BREAK { yym.yy_begin("RULES"); }
<SPACE2> "%namespace"   %NameSpace
<SPACE2> "%type"        %TypeDef
<SPACE2> [A-Za-z][_A-Za-z0-9\-]* %Identifier
<SPACE2> {WhiteSpace}    ;
<SPACE2> "\n"           {Globals.yyLine += 1 ; }
<SPACE2> "("            %LParen
<SPACE2> ")"            %RParen
<SPACE2> ","            %COMMA


<RULES> "%%"    %BREAK { yym.yy_begin("YYINITIAL");  }
<RULES> "+"     %PLUS
<RULES> "*"     %STAR
<RULES> "?"     %OPTIONAL
<RULES> "|"     %VBAR
<RULES> "/"     %BSLASH
<RULES> "::="   %DEFINE
<RULES> ";"     %SEMICOLON
<RULES> ":"     %COLON
<RULES> "!"     %BANG
<RULES> "_"     %UNDERSCORE
<RULES> {WhiteSpace}     ;
<RULES> "\n"            {Globals.yyLine += 1; }
<RULES> "Empty"   %EMPTY
<RULES> "tokenList" %TOKENLIST
<RULES> "{"     %LBRACE { yym.yy_begin("CODEFRAG"); Globals.braceCount += 1; }

<RULES> "L_Identifier" %OUT_TOKEN { yytext = "L_Identifier"; }
<RULES> "U_Identifier" %OUT_TOKEN { yytext = "U_Identifier"; }
<RULES> "A_L_Identifier" %OUT_TOKEN { yytext = "A_L_Identifier"; }
<RULES> "A_U_Identifier" %OUT_TOKEN { yytext = "A_U_Identifier"; }
<RULES> "number" %OUT_TOKEN { yytext = "Number"; }
<RULES> "realnumber"    %OUT_TOKEN { yytext = "Real"; }
<RULES> "PLUS-INFINITY" %OUT_TOKEN { yytext = "PLUS_INFINITY"; }
<RULES> "MINUS-INFINITY" %OUT_TOKEN { yytext = "MINUS_INFINITY"; }
<RULES> "TYPE-IDENTIFIER" %OUT_TOKEN { yytext = "TYPE_IDENTIFIER"; }
<RULES> "ABSTRACT-SYNTAX" %OUT_TOKEN { yytext = "ABSTRACT_SYNTAX"; }
<RULES> "NOT-A-NUMBER"  %OUT_TOKEN { yytext = "NOT_A_NUMBER"; }
<RULES> "OID-IRI" %OUT_TOKEN { yytext = "OID_IRI"; }
<RULES> "RELATIVE-OID-IRI" %OUT_TOKEN { yytext = "RELATIVE_OID_IRI"; }
<RULES> "TIME-OF-DAY" %OUT_TOKEN { yytext = "TIME_OF_DAY"; }
<RULES> "DATE-TIME" %OUT_TOKEN { yytext = "DATE_TIME"; }
<RULES> "ENCODING-CONTROL" %OUT_TOKEN { yytext = "ENCODING_CONTROL"; }
<RULES> "bstring"       %OUT_TOKEN { yytext = "BinaryString"; }
<RULES> "cstring"       %OUT_TOKEN
<RULES> "mstring"       %OUT_TOKEN
<RULES> "hstring"       %OUT_TOKEN { yytext = "HexString"; }
<RULES> "RELATIVE-OID"  %OUT_TOKEN { yytext = "RELATIVE_OID"; }
<RULES> "BMPString"     %OUT_TOKEN
<RULES> "GeneralString" %OUT_TOKEN
<RULES> "GraphicString" %OUT_TOKEN
<RULES> "IA5String"     %OUT_TOKEN
<RULES> "ISO646String"  %OUT_TOKEN
<RULES> "NumericString" %OUT_TOKEN
<RULES> "PrintableString" %OUT_TOKEN
<RULES> "TeletexString" %OUT_TOKEN
<RULES> "T61String"     %OUT_TOKEN
<RULES> "UniversalString" %OUT_TOKEN
<RULES> "UTF8String"    %OUT_TOKEN
<RULES> "VideotexString" %OUT_TOKEN
<RULES> "VisibleString" %OUT_TOKEN
<RULES> "GeneralizedTime" %OUT_TOKEN
<RULES> "UTCTime"       %OUT_TOKEN
<RULES> "ObjectDescriptor" %OUT_TOKEN

<RULES> '"::="' %OUT_TOKEN { yytext = "Definition"; }
<RULES> '"{"'   %OUT_TOKEN { yytext = "LBrace"; }
<RULES> '"}"'   %OUT_TOKEN { yytext = "RBrace"; }
<RULES> '"("'   %OUT_TOKEN { yytext = "LParen"; }
<RULES> '")"'   %OUT_TOKEN { yytext = "RParen"; }
<RULES> '";"'   %OUT_TOKEN { yytext = "SemiColon"; }
<RULES> '","'   %OUT_TOKEN { yytext = "Comma"; }
<RULES> '"."'   %OUT_TOKEN { yytext = "Dot"; }
<RULES> '".."'  %OUT_TOKEN { yytext = "DotDot"; }
<RULES> '"..."' %OUT_TOKEN { yytext = "DotDotDot"; }
<RULES> '"-"'   %OUT_TOKEN { yytext = "Dash"; }
<RULES> '"["'   %OUT_TOKEN { yytext = "LSqr"; }
<RULES> '"[["'  %OUT_TOKEN { yytext = "LSqrLSqr"; }
<RULES> '"]"'   %OUT_TOKEN { yytext = "RSqr"; }
<RULES> '"]]"'  %OUT_TOKEN { yytext = "RSqrRSqr"; }
<RULES> '":"'   %OUT_TOKEN { yytext = "Colon"; }
<RULES> '"<"'   %OUT_TOKEN { yytext = "LAngle"; }
<RULES> '">"'   %OUT_TOKEN { yytext = "RAngle"; }
<RULES> '"|"'   %OUT_TOKEN { yytext = "VLine"; }
<RULES> '"^"'   %OUT_TOKEN { yytext = "Carat"; }
<RULES> '"!"'   %OUT_TOKEN { yytext = "Bang"; }
<RULES> '"@"'   %OUT_TOKEN { yytext = "AtSign"; }
<RULES> '"@."'  %OUT_TOKEN { yytext = "AtDot"; }
<RULES> '"--("' %OUT_TOKEN { yytext = "CmdBegin"; }
<RULES> '")--"' %OUT_TOKEN { yytext = "CmdEnd"; }

<RULES> '"'[^\"]*'"'     %Literal
<RULES> [A-Za-z][A-Za-z0-9\-]* %Identifier
<RULES> "/""/".*          ;

<CODEFRAG> "\n" %ANY { Globals.yyLine += 1; }
<CODEFRAG> "{"  %LBRACE { Globals.braceCount += 1; }
<CODEFRAG> "}"  %RBRACE { Globals.braceCount -= 1; if (Globals.braceCount == 0) yym.yy_begin("RULES"); }
<CODEFRAG> [^{}\n]+ %ANY


