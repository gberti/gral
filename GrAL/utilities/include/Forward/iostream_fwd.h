#ifndef NMWR_GB_IOSTREAM_FORWARDDECL_H
#define NMWR_GB_IOSTREAM_FORWARDDECL_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


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
