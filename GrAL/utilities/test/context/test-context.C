/*! \file

*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/context.h"


int main() {

  typedef GrAL::io_context ctxt;

  ctxt::info()    << "Info."    << std::endl;
  ctxt::warning() << "Warning." << std::endl;
  ctxt::skip()    << "Null (ignored)."  << std::endl;
  ctxt::error()   << "Error."   << std::endl;
}
