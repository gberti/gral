#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-

#-i
#!perl
  use Getopt::Long;

  $eps = 1.0e-15;
  &GetOptions("eps=f" => \$eps);

  while(<>) {
    ## replace -0 by 0
    s/-0(\s)/ 0$1/g;
    ## replace "almost 0" by 0
    while(m/([+|-]?[0-9]+[.]?[0-9]*[e|E][-]?[0-9]+)/g) {
      $wert = abs $1;
      if($wert < $eps) {
	s/$1/0/g;
      }
    }
    print;
    
  }

