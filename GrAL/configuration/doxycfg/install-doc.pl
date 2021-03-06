#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w -i

use Getopt::Long;

$STL = "http://www.sgi.com/tech/stl";
$METIS ="http://www-users.cs.umn.edu/~karypis/metis/";
$BOOST="http://www.boost.org";
$ROOT = "../../../..";
$DOXYPATH = "doxygen/html";


&GetOptions("root=s" => \$ROOT,
            "path=s" => \$DOXYPATH,
            "stl=s"  => \$STL,
            "gralconcepts=s" => \$GRALCONCEPTS);

$GRAL= "$ROOT/gral";
$GRALCONCEPTS   ="$GRAL/base/doc/hyperlatex/html";
$GRALCONCEPTSTOP="$GRALCONCEPTS/top.html";
$GRALINDEX      ="$GRAL/$DOXYPATH/index.html";

while(<>) {
  s|%STL|$STL|g;
  s|%Metis|$METIS|g;
  s|%METIS|$METIS|g;
  s|<a href=\"%GRALCONCEPTSTOP|<a TARGET=_parent  HREF=\"$GRALCONCEPTSTOP|g;
  s|<a href=\"%GRALCONCEPTS|<a TARGET=_parent  HREF=\"$GRALCONCEPTS|g;
  s|%GRALCONCEPTSTOP\"|$GRALCONCEPTSTOP\"|g;
  s|%GRALCONCEPTS\"|$GRALCONCEPTS\"|g;
  s|%GRALCONCEPTS/|$GRALCONCEPTS/|g;
  s|%GRALINDEX\"|$GRALINDEX\"|g;
  s|%ROOT|$ROOT|g;
  s|%DOXYPATH|$DOXYPATH|g;
  s|GRALPROTECT||g;
  s|\$boost|<a target=_parent href=\"$BOOST\">boost</a>|g;
  s|\"memItemLeft\"[ ]+nowrap|\"memItemLeft\" |g;
  s|class[ ]+([a-z_A-Z][a-z_A-Z0-9]*),|class&nbsp;$1,|g;
  print;
}
