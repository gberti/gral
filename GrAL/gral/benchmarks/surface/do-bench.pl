#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w


use Getopt::Long;

$compiler = "gcc3"; #"KCC"; # gcc

&GetOptions("c=s" => \$compiler);

@optflags = ();
if($compiler eq "g++" || $compiler eq "gcc3") {
#  @optflags = ("", "-O", "'-O -finline-functions'", "-O2", '"-O2 -finline-functions"', 
#	       '"-O3 -fstrict-aliasing -finline-functions -funroll-loops"');
  @optflags = ('"-O3 -fstrict-aliasing -finline-functions -funroll-loops -finline-limit=100000"',
	       '"-O3 -fstrict-aliasing -finline-functions -funroll-loops -finline-limit=1000000"');
#("'-O -finline-functions'",
}
else {
 @optflags = ('"+K3 -O --abstract_float --abstract_pointer --restrict"',
	      '"+K3 -O --abstract_float --abstract_pointer --restrict --inline_implicit_space_time=100"');
}

@tableflags  = ( "-DDO_MAP", "-DDO_HASH");
@gridx = (10,  20,  500);
@gridy = (10,  20,  500);

$work = 10000000; 

#$resultfile = "bench0.out";
#$tmpfile    = "bench.tmp";
$i = 0;
#open RES, ">$resultfile" or die "cannot open $resultfile";

foreach $opt (@optflags) {
  $arg = "gmake  -s new CCC=$compiler CCNAME=$compiler DEBUG=opt CCOPTFLAGS=$opt  2>&1 > make.out.$i";
  $i = $i + 1;
  system $arg;
  foreach $nx (@gridx) {
    $ny = $nx;
    $nloop = $work / ($nx * $ny); # keep total work fixed
    print "--------------------------------------------\n";
    print "Compiler: $compiler\n"; 
    print "Flags: $opt\n"; 
    print "Loop: $nx x $ny grid, $nloop loops\n";
    $arg1 = "./bench-surface  -nx $nx -ny $ny -nloop $nloop";
    #   print "$arg1\n";
    system $arg1;
  }
}

