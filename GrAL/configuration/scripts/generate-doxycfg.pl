#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w


##! /opt/local/bin/perl

##############################################################
#
# Generate TAGFILES tag for doxygen
# from list of dependencies 
# (modules on which the current module depends)
# 
##############################################################


use Getopt::Long;

$allroot = "/home/berti/CVS-work";

&GetOptions("root=s" => \$allroot);

@modules = ();
while(<>) {
  $_ =~ s|/|_|g; 
  push(@modules, split(' ', $_));
}

print "TAGFILES = ";
foreach $mod(@modules) {
  $modpath = $mod;
  $modpath =~ s|_|/|; 
  print " \\\n";
  print "$allroot/$modpath/doxygen/$mod.tags=$allroot/$modpath/doxygen/html";
}
print "\n";
