
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <iostream>
#include <fstream>

#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Test/test-grid-functions.h"
#include "Gral/Test/test-partial-grid-functions.h"

#include "Utility/pre-post-conditions.h"

int main() {
  using namespace GrAL;
  using namespace std;
  using namespace cartesian2d;

  ostream& out = cout; // new ofstream("gridfunctions.out");

  typedef grid_types<CartesianGrid2D> gt;

  int NX = 3, NY = 4;
  CartesianGrid2D R(NX,NY);


  out << "CartesianGrid2D (" << NX << ',' << NY << ")\n"
      << R.NumOfCells()    << " cells\n"
      << R.NumOfEdges()    << " edges\n"
      << R.NumOfVertices() << " vertices\n";

  test_grid_functions<gt::Vertex>        (R,out);
  test_partial_grid_functions<gt::Vertex>(R,out);
  test_grid_functions<gt::Edge>        (R,out);
  test_partial_grid_functions<gt::Edge>(R,out);
  test_grid_functions<gt::Cell>        (R,out);
  test_partial_grid_functions<gt::Cell>(R,out);

  out << endl;

  return 0;
}
