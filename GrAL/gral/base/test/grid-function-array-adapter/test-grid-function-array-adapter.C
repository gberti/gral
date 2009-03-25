
/*! \file

    test harness for grid_function_array_adapter<>
*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/grid-function-array-adapter.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Geometries/simple-geometry.h"
#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"


#include <iostream>

namespace GrAL { 

  template class grid_function_array_adapter<cartesian2d::CartesianGrid2D::Cell, double, 3>;

}

int main() {
  using namespace GrAL;
  using namespace std;

  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;
  typedef tuple<double,3>              coord_type;

  grid_type R(2,2);

  
  double geo[4*3] = { 0,0,0, 1,0,0, 1,1,0, 0,1,0 };
  grid_function_array_adapter<gt::Vertex, double, 3> geoadapt(R, geo);
  
  simple_geometry<grid_type,coord_type> geom(R, geoadapt.begin(), geoadapt.end());

  for(gt::VertexIterator v(R); !v.IsDone(); ++v)
    cout << geom.coord(*v) << endl; 


}
