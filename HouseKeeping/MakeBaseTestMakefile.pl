#!/usr/bin/perl
use strict;

# Makes base-test-makefile from dependencies.txt

unless(-r 'base-test-makefile') { die "You need to run this in a directory that already has a copy of base-test-makefile\n" }
open(DEPS, 'dependencies.txt') or die "Could not open dependencies.txt for reading.\n";
my %Dependencies;
while(<DEPS>) {
    my $ThisLine = $_; chomp $ThisLine;
    (my $Left, my $Right) = split(/:/, $ThisLine);
    $Dependencies{$Left} = $Right;
}

my $TheOut = <<'EOF';
#### Make this file with MakeBaseTestMakefile.pl, *not* by editing the file itself
MONO:IARG = -I$(PATH_TO_A2C)/runtime/C
MONO:A2C_LIB = $(PATH_TO_A2C)/runtime/C/A2C_Runtime.lib
MONO:A2C_EXE = mono $(PATH_TO_A2C)/a2c.exe
NET:CFLAGS=/Zi /W4 /Gy -I"$(PATH_TO_A2C)"\runtime\c
NET:A2C_LIB="$(PATH_TO_A2C)"\runtime\C\A2C_Runtime.lib
NET:A2C_EXE="$(PATH_TO_A2C)"\a2c.exe

EOF

foreach my $i (sort(keys(%Dependencies))) {
    $TheOut .= "$i:\n\t" . '$(A2C_EXE) -out=test-temp ' . "$Dependencies{$i}\n";
}

$TheOut .= "\nrfcs: ";
foreach my $i (sort(keys(%Dependencies))) {
    $TheOut .= "$i ";
}

$TheOut .= <<'EOF';


program-1:
MONO:	@echo "Building and running example-1"
MONO:	$(A2C_EXE) -inline=static example-1.asn
MONO:	gcc -pedantic -o example-1 $(IARG) program-1.c example-1.c $(A2C_LIB)
MONO:	./example-1
MONO:	@echo
NET:	@echo Building and running example-1
NET:	$(A2C_EXE) -inline=yes example-1.asn
NET:	cl $(CFLAGS) $(A2C_LIB) program-1.c example-1.c /Feexample-1.exe
NET:	example-1.exe

program-everything:
MONO:	@echo "Building and running example-everything"
MONO:	$(A2C_EXE) -inline=static example-everything.asn
MONO:	gcc -pedantic -o example-everything $(IARG) program-everything.c example-everything.c $(A2C_LIB)
MONO:	./example-everything
MONO:	@echo
NET:	@echo Building and running example-everything
NET:	$(A2C_EXE) -inline=yes example-everything.asn
NET:	cl $(CFLAGS)  $(A2C_LIB) program-everything.c example-everything.c /Feexample-everything.exe
NET:	example-everything.exe

all:
MONO:	make program-1; make program-everything; make rfcs
NET:	nmake program-1
NET:	nmake program-everything
NET:	nmake rfcs
EOF

open(OUT, '>base-test-makefile') or die "Could not open base-test-makefile for writing.\n";
print OUT $TheOut;
close(OUT);
