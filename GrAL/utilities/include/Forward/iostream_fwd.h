#ifndef NMWR_GB_IOSTREAM_FORWARDDECL_H
#define NMWR_GB_IOSTREAM_FORWARDDECL_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



//  forward declarations, these might change
//  if iostreams are implemented as templates!
//  this is also compiler-dependend

#include "compiler-config.h"

#ifndef __KCC
class istream;
class ostream;
#else
#include <iostream>
USING_STD(ostream);
USING_STD(istream);
#endif

#endif
