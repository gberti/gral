#! /opt/local/bin/perl -w

$STL = "http://www.sgi.com/Technology/STL";
$GRAL = "file:/home/berti/CVS-work/gral/base/doc/hyperlatex/html";
$ROOT = "file:/home/berti/CVS-work/";

while(<>) {
  s|\$gral-([^ ]+)[ ]*([a-z\-_A-Z0-9]+)|<A HREF="$ROOT/$1/doxygen/html/index.html"> $2 </A>|g;
  s|\$GrAL[ ]+([a-z\-_A-Z0-9]+)|<A HREF="$GRAL/$1.html">$1</A>|g;
  s|Adaptable Unary Function|<A HREF="$STL/AdaptableUnaryFunction.html"> Adaptable Unary Function</A>|g;
  s|Forward Iterator|<A HREF="$STL/ForwardIterator.html">Forward Iterator</A>|g;
  s|Equality Comparable|<A HREF="$STL/EqualityComparable.html">Equality Comparable</A>|g;
  s|Assignable|<A HREF="$STL/Assignable.html">Assignable</A>|g;
  s|Default Constructible|<A HREF="$STL/DefaultConstructible.html">Default Constructible</A>|g;
  s|Random Access Container|<A HREF="$STL/RandomAccessContainer.html">Random Access Container</A>|g;
  s|SGI iota|<A HREF="$STL/iota.html">SGI iota</A>|g;
  print;
}
