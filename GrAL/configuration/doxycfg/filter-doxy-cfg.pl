#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w

use Getopt::Long;

$allroot = "/home/berti/CVS-work";
$binpath = "/opt/local/bin/";

&GetOptions("root=s" => \$allroot,
	    "binpath=s" => \$binpath);

#$perlpath="$binpath/perl";

open(WHICH, "which perl|");
chop($perlpath = <WHICH>);

while (<>) {
 # $_ =~ 
   s|\$GRALROOT|$allroot|g;
   s|\$PERL_PATH|$perlpath|g;
   s|\$BIN_ABSPATH|$binpath|g;
  print;
}
