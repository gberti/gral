#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w -i

use Getopt::Long;

$newroot="";

&GetOptions("newroot=s" => \$newroot);


while(<>) {
 # 
  if(m|GRALROOT=([^ \#]*)|)  {
    s|GRALROOT=([^\# ]*)|GRALROOT=$newroot\#|g; 
    #print "Old root: $1\n";
    #print "New root: $newroot\n";
  }
  print;
}
