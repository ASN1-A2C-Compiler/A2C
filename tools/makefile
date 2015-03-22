all:
	@echo Making cstools-3.5
	cd cstools-3.5
	nmake
	@echo Making CS_Lex
	cd ..\CS_Lex
	nmake
	@echo Making rcc
	cd ..\rcc
	nmake

clean:
	del /F Tools.dll cs_lex.exe lg.exe pg.exe rcc.exe
	cd cstools-3.5
	nmake clean
	cd ..\CS_Lex
	nmake clean
	cd ..\rcc
	nmake clean
