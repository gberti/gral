
// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/IO/stream-grid-adapter.h"

#include "Gral/Grids/Complex2D/construct.h"

// static data
CartesianGrid3D::SD CartesianGrid3D::sd;

// constructur for static data
CartesianGrid3D::SD::SD()
{
  typedef CartesianGrid3D::index_type it;

  for(int i = 0; i < 8; ++i)
    corner_offset[i] = index_type((i/4)%2,(i/2)%2 ,i%2);
  /* that is,
  corner_offset = 
  { it(0,0,0), it(0,0,1), it(0,1,0), it(0,1,1),
    it(1,0,0), it(1,0,1), it(1,1,0), it(1,1,1) 
  };
  */
  int conn[5*6] = { 
    4,  0, 1, 3, 2,
    4,  0, 2, 6, 4,
    4,  0, 4, 5, 1,
    4,  1, 5, 7, 3,
    4,  2, 3, 7, 6,
    4,  4, 6, 7, 5
  };
  stream_grid_mask<int *> arch(8,6,conn); 
  ConstructGrid0(the_archetype[0], arch); 
}



