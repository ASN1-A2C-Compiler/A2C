# Makefile for rcc
#
# Be sure that the .NET development tools are installed

LEXER = ..\lg.exe
YACC = ..\pg.exe
TOOLS = ..\Tools.dll

rcc.exe: lex.cs parser.cs program.cs 
	csc /out:rcc.exe /r:$(TOOLS) /debug+ /warn:0 lex.cs parser.cs program.cs
	del /F ..\rcc.exe
	copy rcc.exe ..\rcc.exe 

lex.cs: lex.l
	$(LEXER) lex.l lex

parser.cs: parser.y
	$(YACC) -Ilex parser.y parser

clean:
	del /F rcc.exe lex.cs parser.cs
