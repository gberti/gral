#! /opt/local/bin/perl -w

use Getopt::Long;


#$STL = "http://www.sgi.com/Technology/STL";
#$GRALCONCEPTS = "file:/home/berti/CVS-work/gral/base/doc/hyperlatex/html";
#$ROOT = "file:/home/berti/CVS-work/";
#$DOXYPATH = "doxygen/html";

#GetOptions("root=s" => \$ROOT,
#           "path=s" => \$DOXYPATH,
#           "stl=s"  => \$STL.
#           "gral=s" => \$GRALCONCEPTS);


while(<>) {
  s|\$gral-([^ ]+)[ ]*([a-z\-_A-Z0-9]+)|<A HREF="%ROOT/$1/%DOXYPATH/index.html"> $2 </A>|g;
  s|\$GrAL[ ]+([a-z\-_A-Z0-9]+)|<A HREF="%GRALCONCEPTS/$1.html">$1</A>|g;
  s|Adaptable Unary Function|<A HREF="%STL/AdaptableUnaryFunction.html"> Adaptable Unary Function</A>|g;
  s|Forward Iterator|<A HREF="%STL/ForwardIterator.html">Forward Iterator</A>|g;
  s|Equality Comparable|<A HREF="%STL/EqualityComparable.html">Equality Comparable</A>|g;
  s|Assignable|<A HREF="%STL/Assignable.html">Assignable</A>|g;
  s|Default Constructible|<A HREF="%STL/DefaultConstructible.html">Default Constructible</A>|g;
  s|Random Access Container|<A HREF="%STL/RandomAccessContainer.html">Random Access Container</A>|g;
  s|SGI iota|<A HREF="%STL/iota.html">SGI iota</A>|g;
  print;
}
