#! /opt/local/bin/perl -i


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
  # doxygen transforms xxx.h -> xxx_h.html
  s|\.h\.html|_h\.html|g;
  s|\.C\.html|_C\.html|g;

  # replace dummies set by gen-tex-paths.pl
  s|STLURL|$STL|g;
  s|GRALROOT|$GRALPATH|g;

  print;
}
