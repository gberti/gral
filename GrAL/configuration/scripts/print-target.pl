#! /opt/local/bin/perl

# list commands associated to a make target, e.g. clean:
use Getopt::Long;

$target = clean;

&GetOptions("t=s" => \$target);

$found=0;
while(<>) {
  if(m/^$target:/) {
    $found = 1;
    print;
    next;
  }
  if($found) {
    if(! m/(^[ \t]*$|:)/) {
      print;
    }
    else {
      $found = 0;
    }
  }
}
