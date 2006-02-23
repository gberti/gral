
/*! \file
 */

#include "Gral/IO/stream-grid-adapter.h"
#include "Gral/IO/complex2d-format-output.h"

#include "Container/tuple-point-traits.h"

namespace GrAL {

  template class stream_grid_mask<int *>;
  template class stream_grid_mask_vertex<int *>;
  template class stream_grid_mask_cell<int *>;
  template class stream_grid_mask_vertex_on_cell<int *>;

  template class grid_types<stream_grid_mask<int *> >;
}

int main() {

  int connectivity[] = {
    3, 0, 1, 2,
    3, 0, 3, 1,
    3, 0, 2, 3,
    3, 1, 3, 2
  };

  typedef GrAL::tuple<double,3> ct; 
  ct coords[] = { 
    ct(0.0, 0.0, 0.0), 
    ct(1.0, 0.0, 0.0), 
    ct(0.0, 1.0, 0.0),
    ct(0.0, 0.0, 1.0)
  };
  
  int nv = 4;
  int nc = 4;
  GrAL::stream_grid_mask<int *> tetrahedron(nv,nc,connectivity);
  GrAL::stream_geom_mask<ct  *> tet_geom(coords);

  GrAL::OstreamComplex2DFmt Out("tet.complex2d.out");
  GrAL::ConstructGrid(Out, tetrahedron, tet_geom);
}
