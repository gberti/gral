/*! \file

*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/quantization.h"

#include "Utility/pre-post-conditions.h"

#include <string>
#include <sstream>

namespace GrAL {
  template class quantization<double,int>;
}


int main() {
  
  using namespace GrAL;
  using namespace std;

  quantization<double,int> q(-1.0, 0.0, 1.0);

  REQUIRE_ALWAYS(q(-2.0) == 0, "q(-2.0)=" << q(-2.0), 1);
  REQUIRE_ALWAYS(q(-1.0) == 1, "q(-1.0)=" << q(-1.0), 1);
  REQUIRE_ALWAYS(q(-0.5) == 1, "q(-0.5)=" << q(-0.5), 1);
  REQUIRE_ALWAYS(q( 1.0) == 3, "q( 1.0)=" << q( 1.0), 1);
  REQUIRE_ALWAYS(q( 2.0) == 3, "q( 2.0)=" << q( 2.0), 1);

  string s = "3 -1.0  0.0  1.0  0 1 2 3 ";
  istringstream in(s);

  quantization<double,int> q2;
  in >> q2;
  REQUIRE_ALWAYS(q(-2.0) == 0, "q(-2.0)=" << q(-2.0), 1);
  REQUIRE_ALWAYS(q(-1.0) == 1, "q(-1.0)=" << q(-1.0), 1);
  REQUIRE_ALWAYS(q(-0.5) == 1, "q(-0.5)=" << q(-0.5), 1);
  REQUIRE_ALWAYS(q( 1.0) == 3, "q( 1.0)=" << q( 1.0), 1);
  REQUIRE_ALWAYS(q( 2.0) == 3, "q( 2.0)=" << q( 2.0), 1);



}
