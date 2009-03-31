/*! \file
 */

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/predicates.h"
#include "Gral/Grids/CartesianND/all.h"

int main() {
  using namespace GrAL;
  cartesiannd::grid<3> R(3,3,3);
  typedef grid_types<cartesiannd::grid<3> > gt;

  for(gt::CellIterator c(R); !c.IsDone(); ++c)
    for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
      REQUIRE_ALWAYS(incident    (*vc,*c), "", 1);
      REQUIRE_ALWAYS(incident<gt>(*vc,*c), "", 1);
      REQUIRE_ALWAYS(incident    (*c,*vc), "", 1);
      REQUIRE_ALWAYS(incident<gt>(*c,*vc), "", 1);
    }
}
