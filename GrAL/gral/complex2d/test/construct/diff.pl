#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w

$ncases = 8;
for( $i = 1; $i < $ncases; $i++) {
    print "

sdiff case$i.grid case$i.grid.out
--------------------------------------
";
    system("sdiff","case$i.grid","case$i.grid.out");
}
