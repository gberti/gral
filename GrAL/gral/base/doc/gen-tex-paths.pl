#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w


$DOXYPATH="doxygen/html";

print "
\\newcommand{\\GRALROOT}{GRALROOT}
\\newcommand{\\GRALINC}[2]{\\GRALROOT/#1/$DOXYPATH/#2.html}
\\newcommand{\\STLURL}{STLURL}
\\newcommand{\\STLPATH}[1]{\\STLURL/#1}
\\newcommand{\\traitsURL}{}

";
