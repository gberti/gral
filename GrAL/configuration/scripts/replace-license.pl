#!/bin/sh
exec perl -w -x $0 ${1+"$@"} # -*- mode: perl; perl-indent-level: 2; -*-
#!perl -i


$license1 = "
/* ------------------------------------------------------------

    Copyright (C) 1997-2004 Guntram Berti,
    NEC C&C Research Laboratories, NEC Europe Ltd.,
    Rathausallee 10, D-53757 St. Augustin, Germany.
    Contact: berti AT ccrl-nece DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    which is available at
    http://www.math.tu-cottbus.de/~berti/gral

    You are free to use, copy and modify the sources under
    certain circumstances, provided this copyright notice
    remains intact. See the file LICENSE for details, which
    is contained in this distribution, and is available online at
    http://www.math.tu-cottbus.de/~berti/gral/license.html .

--------------------------------------------------------------- */
";


#$license2 = "
#// \$LICENSE
#";
#$license_nec ="
#// \$LICENSE_NEC
#";

$license = $license1;

#print "executing replace-license.pl\n";
while(<>) {
    s|//[ ]*\$LICENSE_NEC_2004|$license|;
    s|//[ ]*\$LICENSE_NEC_2003|$license|;
    s|//[ ]*\$LICENSE_NEC_2002|$license|;
    s|//[ ]*\$LICENSE_NEC_2001|$license|;
    s|//[ ]*\$LICENSE_NEC|$license|;
    s|//[ ]*\$LICENSE|$license|;
    print;
}
