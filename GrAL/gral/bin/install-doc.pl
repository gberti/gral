#! /opt/local/bin/perl  -i 

use Getopt::Long;

$STL = "http://www.sgi.com/tech/stl";
$GRALCONCEPTS = "file:/home/berti/CVS-work/gral/base/doc/hyperlatex/html";
$ROOT = "file:/home/berti/CVS-work/";
$DOXYPATH = "doxygen/html";

&GetOptions("root=s" => \$ROOT,
            "path=s" => \$DOXYPATH,
            "stl=s"  => \$STL,
            "gral=s" => \$GRALCONCEPTS);

while(<>) {
  s|%STL|$STL|g;
  s|%GRALCONCEPTS|$GRALCONCEPTS|g;
  s|%ROOT|$ROOT|g;
  s|%DOXYPATH|$DOXYPATH|g;
  print;
}
