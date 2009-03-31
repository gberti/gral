/*! \file
 
*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Subranges/boundary-grid.h"

#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

int main() {
  using namespace std;
  using namespace GrAL;

  using namespace cartesian2d;

  typedef grid_types<CartesianGrid2D> gt;

  CartesianGrid2D R(2,2);

  typedef boundary_grid<gt::Cell> cell_bd_t;
  typedef grid_types<cell_bd_t>   bdcgt;
  boundary_grid<gt::Cell> bd(*(R.FirstCell()));
  

  for(bdcgt::CellIterator c(bd); ! c.IsDone(); ++c) {
    cout << c.handle() << " [facets: ";
    for(bdcgt::FacetOnCellIterator fc(*c); ! fc.IsDone(); ++fc) {
      cout << fc.handle() << " ";
    }
    cout << "]\n"; 
  }
}
