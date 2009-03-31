

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
*/

#include "Gral/Base/partial-grid-morphism.h"

#include "Gral/Grids/Cartesian2D/all.h"

#include "Utility/pre-post-conditions.h"

int main() {
  using namespace GrAL;
  using namespace cartesian2d;

  typedef RegGrid2D grid_type;
  typedef grid_types<grid_type> gt;

  grid_type G1(3,3), G2(3,3);
  partial_grid_morphism<grid_type, grid_type> phi(G1,G2);

  gt::VertexIterator v1(G1), v2(G2);
  for(; !v1.IsDone(); ++v1, ++v2)
    phi[*v1] = *v2;
  v1= G1.FirstVertex();v2=G2.FirstVertex();
  for(; !v1.IsDone(); ++v1, ++v2) {
    REQUIRE_ALWAYS( (phi(*v1)           == *v2), "phi has wrong value!\n",1);
    REQUIRE_ALWAYS( (phi.inverse()(*v2) == *v1), "phi^-1 has wrong value!\n",1);
  }

  gt::EdgeIterator e1(G1), e2(G2);
  for(; !e1.IsDone(); ++e1, ++e2)
    phi[*e1] = *e2;
  e1 = G1.FirstEdge(); e2 = G2.FirstEdge();
  for(; !e1.IsDone(); ++e1, ++e2) {
    REQUIRE_ALWAYS( (phi(*e1) == *e2), "phi has wrong value!\n",1);
    REQUIRE_ALWAYS( (phi.inverse()(*e2) == *e1), "phi^-1 has wrong value!\n",1);
  }

  gt::CellIterator c1(G1), c2(G2);
  for(; !c1.IsDone(); ++c1, ++c2)
    phi[*c1] = *c2;
  c1= G1.FirstCell();c2=G2.FirstCell();
  for(; !c1.IsDone(); ++c1, ++c2) {
    REQUIRE_ALWAYS( (phi(*c1) == *c2), "phi has wrong value!\n",1);
    REQUIRE_ALWAYS( (phi.inverse()(*c2) == *c1), "phi^-1 has wrong value!\n",1);
  }
}
