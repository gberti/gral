#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w

$STL = "http://www.sgi.com/Technology/STL";

while(<>) {
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
