#! /opt/local/bin/perl -i


use Getopt::Long;

$STL       = "http://www.sgi.com/Technology/STL";
$ALLROOT   = "../../.."; # rel. path to root of packages


&GetOptions("stl=s"      => \$STL,
            "allroot=s" => \$ALLROOT);


# filter html files
while(<>) {
  # doxygen transforms xxx.h -> xxx_h.html
  s|\.h\.html|_h\.html|g;
  s|\.C\.html|_C\.html|g;

  # replace dummies set by gen-tex-paths.pl
  s|STLURL|$STL|g;
  s|ALLROOT|$GRALPATH|g;

  print;
}
