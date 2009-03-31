/*! \file
    Test harness for cartesiannd::grid<DIM> grid functions
*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/CartesianND/grid-functions.h"
#include "Gral/Grids/CartesianND/partial-grid-functions.h"

#include "Gral/Test/test-grid-functions.h"
#include "Gral/Test/test-partial-grid-functions.h"
#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;

  typedef grid_types<cartesiannd::grid<2> > gt2;
  typedef gt2::index_type it2;  
  cartesiannd::grid<2> R(it2(2,2));

  grid_function<gt2::Vertex, int> gf1(R,0);

  test_grid_functions<gt2::Vertex>(R, cout);
  test_grid_functions<gt2::Edge>  (R, cout);
  test_grid_functions<gt2::Cell>  (R, cout);
  test_partial_grid_functions<gt2::Vertex>(R, cout);
  test_partial_grid_functions<gt2::Edge>  (R, cout);
  test_partial_grid_functions<gt2::Cell>  (R, cout);
}
