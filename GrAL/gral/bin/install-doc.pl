#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w -i

use Getopt::Long;

$STL = "http://www.sgi.com/tech/stl";
$METIS ="http://www-users.cs.umn.edu/~karypis/metis/";
$GRALCONCEPTS = "file:/home/berti/CVS-work/gral/base/doc/hyperlatex/html";
$ROOT = "file:/home/berti/CVS-work/";
$DOXYPATH = "doxygen/html";

&GetOptions("root=s" => \$ROOT,
            "path=s" => \$DOXYPATH,
            "stl=s"  => \$STL,
            "gral=s" => \$GRALCONCEPTS);

while(<>) {
  s|%STL|$STL|g;
  s|%Metis|$METIS|g;
  s|%METIS|$METIS|g;
  s|%GRALCONCEPTS|$GRALCONCEPTS|g;
  s|%ROOT|$ROOT|g;
  s|%DOXYPATH|$DOXYPATH|g;
  print;
}
