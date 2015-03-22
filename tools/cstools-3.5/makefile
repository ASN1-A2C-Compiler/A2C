# Creates lg.exe, pg.exe, and Tools.dll
# Be sure that the .NET development tools are installed

all:
	csc /debug+ /warn:0 /out:Tools.dll /target:library dfa.cs lexer.cs parser.cs olist.cs genbase0.cs
	csc /debug+ /warn:0 /r:Tools.dll lg.cs
	lg pg.lexer ptokens
	csc /debug+ /warn:0 /r:Tools.dll pg.cs ptokens.cs
	lg cs0.lexer cs0tokens
	pg -Ics0tokens cs0.parser cs0syntax
	csc /debug+ /warn:0 /out:Tools.dll /target:library dfa.cs lexer.cs parser.cs olist.cs genbase.cs cs0tokens.cs cs0syntax.cs
	csc /debug+ /warn:0 /r:Tools.dll lg.cs
	csc /debug+ /warn:0 /r:Tools.dll pg.cs ptokens.cs
	del /F ..\pg.exe ..\lg.exe ..\Tools.dll
	copy pg.exe ..\pg.exe
	copy lg.exe ..\lg.exe
	copy Tools.dll ..\Tools.dll

clean:
	del /F lg.exe pg.exe Tools.dll
