# Makefile for tools/cs_lex.exe
#
SOURCES = CS_Lex.cs CLexGen.cs CInput.cs CSpec.cs CMakeNfa.cs \
	CNfa2Dfa.cs CMinimize.cs CSimplifyNfa.cs CEmit.cs CSet.cs CNfa.cs \
	CAccept.cs Vector.cs SparseBitSet.cs CNfaPair.cs CBunch.cs CDfa.cs \
	CError.cs CUtility.cs CDTrans.cs CAlloc.cs mEnum.cs

cs_lex.exe: $(SOURCES)
	csc /out:cs_lex.exe /debug+ /warn:0 $(SOURCES)
	del /F ..\cs_lex.exe
	copy cs_lex.exe ..\cs_lex.exe

clean:
	del /F cs_lex.exe
