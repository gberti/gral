#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -w -i


use Getopt::Long;

$STL = "http://www.sgi.com/Technology/STL";
$GRALROOT = "..";       # rel. path from gral/base to gral/
$PREFIX   = "../../.."; # rel. path from base/doc/hyperlatex/html to base/


&GetOptions("stl=s"      => \$STL,
            "gralroot=s" => \$GRALROOT,
            "prefix=s"   => \$PREFIX);

# set path to GRAL source-code documentation
# use absolute path or relative path, depending on $GRALROOT
$GRALPATH= "";
if( $GRALROOT =~ m|^/| or $GRALROOT =~ m|file:/| ) { # an absolute path
  $GRALPATH = $GRALROOT;
}
else { # relative path: prepend
  $GRALPATH = "$PREFIX/$GRALROOT";
}

# filter html files
while(<>) {
    # doxygen transforms:
    # xxx.h -> xxx_h.html
    # class A_B -> classA__B.html
    $n = 0;
    $first = 0;   
    while(substr($_,$n) =~ m|[^a-zA-Z_0-9]struct[^.]*\.html|) {
	$n =  $n + length($`);
        $n1 = $n + length($&);
	#$ss = substr($_,$n, length($&));
	#print "struct: $n1 $n Prematch: $` Match: $& Substr: $ss\n ";
	substr($_,$n, length($&)) =~ s/_/__/g;
        $n = $n1;
    }
    $n = 0; 
    $first = 0;  
    while(substr($_,$n) =~ m|[^a-zA-Z_0-9]class[^.]*\.html|) {
	$n = $n + length($`);
        $n1 = $n + length($&);
	#$ss = substr($_,$n, length($&));
	#print "class: $n1 $n Prematch: $` Match: $& Substr: $ss\n";
	substr($_,$n, length($&)) =~ s/_/__/g;
        $n = $n1;
    }


   #s|class[^.]*\.html
   s|\.h\.html|_h\.html|g;
   s|\.C\.html|_C\.html|g;

  # replace dummies set by gen-tex-paths.pl
  s|STLURL|$STL|g;
  s|GRALROOT|$GRALPATH|g;
  # insert style sheet
  s|<HEAD>|<HEAD> <link  type="text/css" rel="stylesheet" href="gralconcepts.css">|;
  s|<head>|<head> <link  type="text/css" rel="stylesheet" href="gralconcepts.css">|;
  print;
}
