all:
	cd tools
	nmake clean
	nmake
	cd ..\a2c
	nmake clean
	nmake
	cd ..\runtime\C
	nmake clean
	nmake
