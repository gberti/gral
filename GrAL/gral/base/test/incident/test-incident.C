/*! \file
 */

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
