#! /opt/local/bin/perl

#package Shell;
#use  Shell cp; # cw(sdiff);

$ncases = 8;
for( $i = 1; $i < $ncases; $i++) {
    print "

sdiff case$i.grid case$i.grid.out
--------------------------------------
";
    system("sdiff","case$i.grid","case$i.grid.out");
}
