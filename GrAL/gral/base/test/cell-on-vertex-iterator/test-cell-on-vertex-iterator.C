

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file 

   Test cases for cell_on_vertex_iterator<>

*/

#include "Gral/Iterators/cell-on-vertex-iterator.h"

#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;
  using namespace cartesian2d;

  CartesianGrid2D R(3,3);
  typedef grid_types<CartesianGrid2D> gt;

  typedef cell_on_vertex_iterator<CartesianGrid2D> c_o_v_iterator;

  c_o_v_iterator::init(R);
  
  for(gt::VertexIterator v(R); ! v.IsDone(); ++v) {
    cout << "v=" << v.handle() << "  ";
    cout << "[";
    for(gt::CellOnVertexIterator cv(*v); ! cv.IsDone(); ++cv)
      cout << (*cv).handle() << " ";
    cout << "]"
	 << "  "
	 << "[";
    for(c_o_v_iterator cv(*v); ! cv.IsDone(); ++cv)
      cout << (*cv).handle() << " ";
    cout << "]";
    cout << endl;
  }
}
