#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w -i

use Getopt::Long;

$STL = "http://www.sgi.com/tech/stl";
$METIS ="http://www-users.cs.umn.edu/~karypis/metis/";
$ROOT = "../../../..";
$DOXYPATH = "doxygen/html";


&GetOptions("root=s" => \$ROOT,
            "path=s" => \$DOXYPATH,
            "stl=s"  => \$STL,
            "gral=s" => \$GRALCONCEPTS);

$GRAL= "$ROOT/gral";
$GRALCONCEPTS   ="$GRAL/base/doc/hyperlatex/html";
$GRALCONCEPTSTOP="$GRALCONCEPTS/top.html";
$GRALINDEX      ="$GRAL/$DOXYPATH/index.html";

while(<>) {
  s|%STL|$STL|g;
  s|%Metis|$METIS|g;
  s|%METIS|$METIS|g;
  s|%GRALCONCEPTS\"|$GRALCONCEPTS\"|g;
  s|%GRALINDEX\"|$GRALINDEX\"|g;
  s|%GRALCONCEPTSTOP\"|$GRALCONCEPTSTOP\"|g;
  s|%ROOT|$ROOT|g;
  s|%DOXYPATH|$DOXYPATH|g;
  print;
}
