all:
	@echo Must specify an option

mono:
	@echo "This batch file deletes the current makefiles and copies"
	@echo "  new copies from this directory into the proper places"

	perl ProcessMakefiles.pl

	@echo "Deleting current makefile for a2c"
	rm -f ../a2c/makefile$(EXT)
	@echo "Copying makefile for a2c"
	cp a2c-makefile-Mono ../a2c/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for rcc"
	rm -f ../tools/rcc/makefile$(EXT)
	@echo "Copying makefile for rcc"
	cp rcc-makefile-Mono ../tools/rcc/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for tools/cstools-3.5"
	rm -f ../tools/cstools-3.5/makefile$(EXT)
	@echo "Copying makefile for tools/cstools-3.5"
	cp cstools-3.5-makefile-Mono ../tools/cstools-3.5/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for tools/CS_Lex"
	rm -f ../tools/CS_Lex/makefile$(EXT)
	@echo "Copying makefile for tools/CS_Lex"
	cp cslex-makefile-Mono ../tools/CS_Lex/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for runtime/C"
	rm -f ../runtime/C/makefile$(EXT)
	@echo "Copying makefile for runtime/C"
	cp runtime-c-makefile-Mono ../runtime/C/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for test"
	rm -f ../test/makefile$(EXT)
	@echo "Copying makefile for test"
	cp test-makefile-Mono ../test/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for tools"
	rm -f ../tools/makefile$(EXT)
	@echo "Copying makefile for tools"
	cp tools-makefile-Mono ../tools/makefile$(EXT)

	@echo ""
	@echo "Deleting current makefile for the main directory"
	rm -f ../makefile$(EXT)
	@echo "Copying makefile for the main directory"
	cp main-makefile-Mono ../makefile$(EXT)

net:
	@echo off
	@echo This batch file deletes the current makefiles and copies
	@echo   new copies from this directory into the proper places

	perl ProcessMakeFiles.pl

	@echo Deleting current makefile for a2c
	del /F ..\a2c\makefile
	@echo Copying makefile for a2c
	copy a2c-makefile-NET ..\a2c\makefile

	@echo Deleting current makefile for rcc
	del /F ..\tools\rcc\makefile
	@echo Copying makefile for rcc
	copy rcc-makefile-NET ..\tools\rcc\makefile

	@echo Deleting current makefile for tools/cstools-3.5
	del /F ..\tools\cstools-3.5\makefile
	@echo Copying makefile for tools/cstools-3.5
	copy cstools-3.5-makefile-NET ..\tools\cstools-3.5\makefile

	@echo Deleting current makefile for tools/CS_Lex
	del /F ..\tools\CS_Lex\makefile
	@echo Copying makefile for tools/CS_Lex
	copy cslex-makefile-NET ..\tools\CS_Lex\makefile

	@echo Deleting current makefile for runtime/C
	del /F ..\runtime\C\makefile
	@echo Copying makefile for runtime/C
	copy runtime-c-makefile-NET ..\runtime\C\makefile

	@echo Deleting current makefile for test
	del /F ..\test\makefile
	@echo Copying makefile for test
	copy test-makefile-NET ..\test\makefile

	@echo Deleting current makefile for tools
	del /F ..\tools\makefile
	@echo Copying makefile for tools
	copy tools-makefile-NET ..\tools\makefile

	@echo Deleting current makefile for the main directory
	del /F ..\makefile
	@echo Copying makefile for the main directory
	copy main-makefile-NET ..\makefile
