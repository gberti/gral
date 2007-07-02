#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w


use Getopt::Long;


while(<>) {
  s|/_\*|/\*|g;
  s|\*_/|\*/|g;
  s|Glossary[ ]+([a-z\-_A-Z0-9]+)|<A HREF="%GRALCONCEPTS/Glossary.html#glossary-$1">GRALPROTECT$1</A>|g;
  s|Glossary[ ]+\"([a-z\-_A-Z0-9 ]+)\"|<A HREF="%GRALCONCEPTS/Glossary.html#glossary-$1">GRALPROTECT$1</A>|g;
  s|\$Metis|<A  TARGET="_parent" HREF="%Metis">Metis</A>|g;
  s|\$gral-([^ ]+)[ ]*([a-z\-_A-Z0-9]+)|<A TARGET="_parent" HREF="%ROOT/$1/%DOXYPATH/index.html"> $2 </A>|g;
  s|\$GrAL[ \-]*([a-z\-_A-Z0-9]+)|<A TARGET="_parent" HREF="%GRALCONCEPTS/$1.html">GRALPROTECT$1</A>|g;
  s|\$GRALCONCEPTSTOP[ ]+'([^']+)'|<A HREF="%GRALCONCEPTSTOP">$1</A>|g;
  s|\$GRALCONCEPTSTOP[ ]+([^ ]+)|<A HREF="%GRALCONCEPTSTOP">$1</A>|g;
  s|\$DISS|<A HREF="http://www.math.tu-cottbus.de/~berti/diss"> [Berti00] </A>|g;
  s|Adaptable Unary Function|<A  TARGET="_parent" HREF="%STL/AdaptableUnaryFunction.html"> Adaptable Unary Function</A>|g;
  s|Unary Function|<A  TARGET="_parent" HREF="%STL/UnaryFunction.html">Unary Function</A>|g;
  s|Adaptable Binary Function|<A  TARGET="_parent" HREF="%STL/AdaptableBinaryFunction.html"> Adaptable Binary Function</A>|g;
  s|Binary Function|<A  TARGET="_parent" HREF="%STL/BinaryFunction.html">Binary Function</A>|g;
  s|Forward Iterator|<A  TARGET="_parent" HREF="%STL/ForwardIterator.html">Forward Iterator</A>|g;
  s|Equality Comparable|<A  TARGET="_parent" HREF="%STL/EqualityComparable.html">Equality Comparable</A>|g;
  s|Assignable|<A  TARGET="_parent" HREF="%STL/Assignable.html">Assignable</A>|g;
  s|Default Constructible|<A  TARGET="_parent" HREF="%STL/DefaultConstructible.html">Default Constructible</A>|g;
  s|Random Access Container|<A  TARGET="_parent" HREF="%STL/RandomAccessContainer.html">Random Access Container</A>|g;
  s|SGI iota|<A  TARGET="_parent" HREF="%STL/iota.html">SGI iota</A>|g;
  s|STL ([A-Z][a-z]+) ([A-Z][a-z]+) ([A-Z][a-z]+)|STL <A TARGET="_parent" HREF="%STL/$1$2$3.html">$1 $2 $3</A>|g;
  s|STL "([A-Z][a-zA-Z ]+)"|STL <A TARGET="_parent" HREF="%STL/$1.html">$1</A>|g;
  print;
}
