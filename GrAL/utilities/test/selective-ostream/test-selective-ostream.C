

/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

     Test harness for \c selective_ostream
*/

#include "Utility/selective-ostream.h"
#include "Utility/pre-post-conditions.h"

int main() {
  
  GrAL::selective_ostream out(std::cout);
  out << "passed" << std::endl;
  out.off(); REQUIRE_ALWAYS(out.is_off(), "",1);
  out << "ignored" << std::endl;
  out.on();  REQUIRE_ALWAYS(out.is_on(), "",1);
  out << "passed" << std::endl;

  GrAL::selective_ostream out2;
  REQUIRE_ALWAYS(out2.is_off(), "",1);
  out2 << "ignored";
  try {
    out2.on();
  }
  catch(...) {
    std::cout << "Continuing." << std::endl;
  }
  out2.redirect(std::cout);
  out2.on();
  out2 << "passed" << std::endl;
}
