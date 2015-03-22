# Makefile for a2c
#
# Be sure that the .NET development tools are installed

LEXER = ..\tools\cs_lex.exe
RCC = ..\tools\rcc.exe

SOURCES = AsnFile.cs C_Emitter.cs C_Sharp_Emitter.cs CodeEmitter.cs Constraints.cs \
	Error.cs lex.l.cs LogError.cs parser.y.cs Program.cs Rule.cs SymbolTable.cs \
	Token.cs Type.cs Value.cs Viewer_Emitter.cs

a2c: $(SOURCES)
	csc /out:a2c.exe /debug+ /warn:0 $(SOURCES)
	@echo "Running a sanity test to see if the new a2c.exe works"
	a2c.exe -out=sanitytest ..\test\rfc5280.asn ..\test\pkix-common.asn ..\test\algorithm-class.asn
	del /F sanitytest.*
	@echo "Copying a2c to the distribution base"
	copy a2c.exe ..

lex.l.cs: lex.l
	$(LEXER) lex.l

parser.y.cs: parser.y
	$(RCC) -quiet parser.y

clean:
	del /F lex.l.cs parser.y.cs a2c.exe
