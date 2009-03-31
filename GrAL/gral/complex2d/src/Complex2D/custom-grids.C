
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex2D/custom-grids.h"
#include "Gral/IO/stream-grid-adapter.h"

namespace GrAL {


  custom_grids::grid_type custom_grids::simplex3d;
  custom_grids::grid_type custom_grids::pyramid;
  custom_grids::grid_type custom_grids::prism;
  custom_grids::grid_type custom_grids::cube;

  custom_grids::geom_type custom_grids::simplex3d_geom;
  custom_grids::geom_type custom_grids::pyramid_geom;
  custom_grids::geom_type custom_grids::prism_geom;
  custom_grids::geom_type custom_grids::cube_geom;

  void  custom_grids::init()
  {
    static bool initialized = false;
    if(!initialized) {
      initialized = true;
      init_simplex3d();
      init_pyramid();
      init_prism();
      init_cube();
    }
  }


  void custom_grids::init_simplex3d() 
  {
    int conn[4*4] = {
      3,  0, 1, 2,
      3,  0, 3, 1,
      3,  0, 2, 3,
      3,  1, 3, 2
    };
    typedef coord_type ct;
    ct coords[4] = {
      ct(0,0,0),
      ct(1,0,0),
      ct(0,1,0),
      ct(0,0,1)
    };
    stream_grid_mask<int *>   g(4,4,conn);
    stream_geom_mask<ct  *>   geom(coords);
    ConstructGrid(simplex3d, simplex3d_geom, g, geom);
  }

  void custom_grids::init_pyramid()
  {
    int conn[5*1 + 4*4] = {
      4, 1, 4, 3, 2,
      3, 0, 1, 2,
      3, 0, 2, 3,
      3, 0, 3, 4,
      3, 0, 4, 1
    };
    typedef coord_type ct;
    ct coords[5] = {
      ct( 0, 0,1),
      ct(-1,-1,0),
      ct( 1,-1,0),
      ct( 1, 1,0),
      ct(-1, 1,0)
    };
    stream_grid_mask<int *> g(5,5,conn);
    stream_geom_mask<ct  *> geom(coords);
    ConstructGrid(pyramid, pyramid_geom, g, geom);
  }

  void custom_grids::init_prism()
  {
    int conn[4*2 + 5*3] = {
      3, 0, 1, 2,
      3, 3, 4, 5,
      4, 0, 1, 4, 3,
      4, 1, 2, 5, 4,
      4, 2, 0, 3, 5
    };
    typedef coord_type ct;
    ct coords[6] = {
      ct(0,0,0),
      ct(1,0,0),
      ct(0,1,0),
      ct(0,0,1),
      ct(1,0,1),
      ct(0,1,1),
    };
    stream_grid_mask<int *> g(6,5,conn);
    stream_geom_mask<ct  *> geom(coords);
    ConstructGrid(prism, prism_geom, g, geom);
  }


  void custom_grids::init_cube() 
  {
    int conn[5*6] = {
      4,  0, 3, 2, 1,
      4,  0, 1, 5, 4,
      4,  0, 4, 7, 3,
      4,  1, 2, 6, 5,
      4,  2, 3, 7, 6,
      4,  4, 5, 6, 7
    };
    typedef coord_type ct;
    ct coords[8] = {
      ct(0,0,0),
      ct(1,0,0),
      ct(1,1,0),
      ct(0,1,0),
      ct(0,0,1),
      ct(1,0,1),
      ct(1,1,1),
      ct(0,1,1),
    };
    stream_grid_mask<int *> g(8,6,conn);
    stream_geom_mask<ct  *> geom(coords);
    ConstructGrid(cube, cube_geom, g, geom);
  }

};
