#! /opt/local/bin/perl

use Getopt::Long;

$allroot = "/home/berti/CVS-work";

&GetOptions("root=s" => \$allroot);

while (<>) {
 # $_ =~ 
   s|\$ALLROOT|$allroot|g;
  print;
}
