#! /opt/local/bin/perl -w

$STL = "http://www.sgi.com/Technology/STL";
#$GRAL = "file:/home/berti/CVS-work/nmwrlib/doc/tex/html";
$GRAL = "../../doc/hyperlatex/html";

while(<>) {
  s|\$GrAL ([a-z\-_A-Z0-9]+)|
    <A HREF="$GRAL/$1.html">$1</A>|g;
  s|Adaptable Unary Function|
    <A HREF="$STL/AdaptableUnaryFunction.html"> Adaptable Unary Function</A>|g;
  s|Forward Iterator|
   <A HREF="$STL/ForwardIterator.html">Forward Iterator</A>|g;
  s|Equality Comparable|
   <A HREF="$STL/EqualityComparable.html">Equality Comparable</A>|g;
  s|Assignable|
   <A HREF="$STL/Assignable.html">Assignable</A>|g;
  s|Default Constructible|
   <A HREF="$STL/DefaultConstructible.html">Default Constructible</A>|g;
  s|Random Access Container|
    <A HREF="$STL/RandomAccessContainer.html">Random Access Container</A>|g;
  s|SGI iota|
    <A HREF="$STL/iota.html">SGI iota</A>|g;
  print;
}
