#! /opt/local/bin/perl

##############################################################
#
# Generate installdox call commandline
# from list of dependencies 
# (modules on which the current module depends)
# 
##############################################################


use Getopt::Long;

$allroot  = "/home/berti/CVS-work";
$doxypath = "doxygen/html";

&GetOptions("root=s" => \$allroot,
            "path=s" => \$doxypath);

@modules = ();

while(<>) {
  $_ =~ s|/|_|g; 
  push(@modules, split(' ', $_));
}

if(@modules) {
  print "(cd doxygen/html; ./installdox ";
  foreach $mod(@modules) {
    $modpath = $mod;
    $modpath =~ s|_|/|; 
    print " \\\n";
    print "-l $mod.tags\@$allroot/$modpath/$doxypath";
  }
  print ")\n";
}
else {
  print "echo Nothing to do ...";
}
