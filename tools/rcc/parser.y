%parser lex.l.cs
%namespace Recurser
%symbol GoopOpt:TOKEN;
%symbol GoopList:TOKEN;
%symbol Code:TOKEN;
%symbol CodeItem:TOKEN;
%symbol RuleToken {
    public RuleElement re;
    public RuleToken(TokenType typ) { re = new RuleElement(typ); }
    public RuleToken(TokenType typ, String val) { re = new RuleElement(typ, val); }
    public RuleToken(TokenType typ, String val, String id) { re = new RuleElement(typ, val, id); }
}
%symbol RuleID {
    public String name;
    public RuleID(String val) { 
        name = val; 
        if (!Program.FQuiet) Console.WriteLine("Rule: " + val); }
}
%symbol Special:RuleToken;
%symbol RuleBody {
    public ElementList lst;
    public RuleBody(RuleElement re) { 
        lst = new ElementList();
        lst.Add(re);
    }
    public RuleBody(ElementList list, RuleElement re) {
        lst = list;
        lst.Add(re);
    }
}
%symbol Rule {
    public SingleRule r;
    public Rule(String name, ElementList lst) { r = new SingleRule(Globals.yyLine, name, lst); }
}
%symbol Rules {
    public RuleList list;
    public Rules(SingleRule r) { 
        list = new RuleList();
        list.Add(r);
    }
    public Rules(RuleList lst, SingleRule r) {
        list = lst;
        lst.Add(r);
    }
}
%symbol IdList {
    public System.Collections.Generic.List<String> list;
    public IdList(String str) {
        list = new System.Collections.Generic.List<String>();
        list.Add(str);
    }
    public IdList(System.Collections.Generic.List<String> lst, String str) {
        list = lst;
        lst.Add(str);
    }
    public void AddToMap(String strType, String strField)
        {
            TypeDesc td = new TypeDesc(strType, strField);
            foreach (String str in list) {
                try {
                    Program.TypeMap.Add(str, td);
                }
                catch {
                    Console.WriteLine("Error: {0}({1}): Identifier {2} already assigned type.", Program.FileName, Globals.yyLine, str);
                }
            }
        }
}
        

Root:   GoopOpt:a BREAK InfoListOpt BREAK Rules:b BREAK GoopOpt:c
        {
            Program.ProcessAll(a.yytext, b.list, c.yytext);
        }
        ;

GoopOpt : GoopList:a { yytext = a.yytext; } 
        | ;

GoopList : ANY:a { yytext = a.yytext; }
         | GoopList:a ANY:b { yytext = a.yytext + b.yytext; } ;

InfoListOpt : InfoList | ;

InfoList :   InfoList InfoItem | InfoItem ;

InfoItem :  NameSpace Identifier
        | TypeItem
        ;

TypeItem : TypeDef Identifier:a LParen Identifier:b RParen IdList:c 
                { c.AddToMap(a.yytext, b.yytext); }
        ;

IdList : Identifier:a                   %IdList(a.yytext)
        | IdList:b COMMA Identifier:a         %IdList(b.list, a.yytext)
        ;

Rules:  Rule:a                          %Rules(a.r)
        | Rules:b Rule:a                %Rules(b.list, a.r)
        ;

Rule:   RuleID:a DEFINE RuleBody:b SEMICOLON %Rule(a.name, b.lst)
        ;

RuleID: Identifier:a    %RuleID(a.yytext)
        ;

RuleBody:  RuleToken:a    %RuleBody(a.re)
        | RuleBody:a RuleToken:b  %RuleBody(a.lst, b.re)
        ;

RuleToken: Literal:a                            %RuleToken(TokenType.Literal, a.yytext)
        | Identifier:a COLON Identifier:b       %RuleToken(TokenType.Identifier, a.yytext, b.yytext)
        | Identifier:a                          %RuleToken(TokenType.Identifier, a.yytext)
        | Special
        | LBRACE Code:a RBRACE                  %RuleToken(TokenType.Code, "{" + a.yytext + "}")
        ;

Special: PLUS:a         %RuleToken(TokenType.Plus)
        | STAR:a        %RuleToken(TokenType.Star)
        | OPTIONAL:a    %RuleToken(TokenType.Optional)
        | VBAR:a        %RuleToken(TokenType.Choice)
        | EMPTY:a       %RuleToken(TokenType.Empty)
        | BANG:a        %RuleToken(TokenType.Bang)
        | BSLASH:a      %RuleToken(TokenType.BSlash)
        | UNDERSCORE:a  %RuleToken(TokenType.UScore)
        | TOKENLIST:a   %RuleToken(TokenType.TokenList)
        | OUT_TOKEN:a   %RuleToken(TokenType.Token, a.yytext)
        | OUT_TOKEN:a COLON Identifier:b        %RuleToken(TokenType.Token, a.yytext, b.yytext)
        ;

Code    : CodeItem:a                    { yytext = a.yytext; }
        | Code:b CodeItem:a             { yytext = b.yytext + a.yytext;  }
        ;

CodeItem : LBRACE Code:b RBRACE     { yytext = "{" + b.yytext + "}"; }
        | ANY:a                         { yytext = a.yytext; }
        ;
