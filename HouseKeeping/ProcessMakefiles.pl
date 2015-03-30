#!/usr/bin/perl

# Program to create the .NET and Mono versions of the 
#   makefiles. This is only used by a2c's authors.

use strict;
use File::Copy;
use File::Spec;

my @FilesToProcess = qw (
a2c-makefile
rcc-makefile
cstools-3.5-makefile
cslex-makefile
runtime-c-makefile
test-makefile
tools-makefile
main-makefile
unittest-compile-makefile
);

foreach my $ThisName (@FilesToProcess) {
	my $InName = 'base-' . $ThisName;
	open(IN, $InName) or die "Could not open $InName for reading\n";
	my @All=(); while(<IN>) { push (@All, $_) };  # Pull the contents in
	open(NET, ">$ThisName-NET") or die "Could not write to $ThisName-NET\n";
	open(MONO, ">$ThisName-Mono") or die "Could not write to $ThisName-Mono\n";
	my($NETstring, $MONOstring);
	foreach my $Line (@All) {
		chomp($Line); my($First, $Rest) = split(/:/, $Line, 2);
		if(uc($First) eq "NET") { $NETstring .= "$Rest\n"; }
		elsif(uc($First) eq "MONO") { $MONOstring .= "$Rest\n"; }
		else { $NETstring .= "$Line\n"; $MONOstring .= "$Line\n"; }
	}
	print NET $NETstring; print MONO $MONOstring;
	close(NET); close(MONO);
	my $NetName = File::Spec->catfile('..', 'NET-files', "$ThisName-NET");
	my $MonoName = File::Spec->catfile('..', 'Mono-files', "$ThisName-Mono");
}

