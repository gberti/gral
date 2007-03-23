#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-

#-i
#!perl
  use Getopt::Long;


  while(<>) {
    ## replace absolute paths
    s|/[^ ]*/GrAL|GrAL|g;
    print;
    
  }

