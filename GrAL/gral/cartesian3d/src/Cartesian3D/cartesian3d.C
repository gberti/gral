
// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/IO/stream-grid-adapter.h"

#include "Gral/Grids/Complex2D/construct.h"

namespace cartesian3d {
  CartesianGrid3D::CartesianGrid3D()
    {
      init(0,0,0);
      /*
      int I[3] = {0,0,0};
      vertex_map = index_map_type(I); 
      cell_map   = index_map_type(I); 
      */
    }
  CartesianGrid3D::CartesianGrid3D(int vx, int vy, int vz)
  { 
    if(vx <=0 || vy <= 0 || vz <= 0)
      // empty grid
      init(0,0,0);
    else  
      init(vx,vy,vz);
  }
  
  void CartesianGrid3D::init(int vx, int vy, int vz)
  {
    int vx_1 = (vx > 0 ? vx-1 : 0);
    int vy_1 = (vy > 0 ? vy-1 : 0);
    int vz_1 = (vz > 0 ? vz-1 : 0);
    int v[3] = {vx,vy,vz};
    vertex_map = index_map_type(v);
    int c[3] = {vx_1,vy_1,vz_1};
    cell_map   = index_map_type(c); 
    
    int ex[3] = {vx_1,vy  ,vz  };
    edge_maps[edge_direction::x] = index_map_type(ex);
    int ey[3] = {vx  ,vy_1,vz  };
    edge_maps[edge_direction::y] = index_map_type(ey);
    int ez[3] = {vx  ,vy  ,vz_1};
    edge_maps[edge_direction::z] = index_map_type(ez);
    
    int fxy[3] = {vx_1,vy_1,vz  };
    facet_maps[facet_direction::xy] = index_map_type(fxy);
    int fxz[3] = {vx_1,vy  ,vz_1};
    facet_maps[facet_direction::xz] = index_map_type(fxz);
    int fyz[3] = {vx  ,vy_1,vz_1};
    facet_maps[facet_direction::yz] = index_map_type(fyz);

    edge_handle_offset[edge_direction::x] = 0;
    edge_handle_offset[edge_direction::y] = NumOfXDirEdges();
    edge_handle_offset[edge_direction::z] = NumOfXDirEdges()+NumOfYDirEdges();

    facet_handle_offset[facet_direction::xy] = 0;
    facet_handle_offset[facet_direction::xz] = NumOfXYDirFacets();
    facet_handle_offset[facet_direction::yz] = NumOfXYDirFacets()+NumOfXZDirFacets();
  }


// static data
CartesianGrid3D::SD CartesianGrid3D::sd;

// constructur for static data
CartesianGrid3D::SD::SD()
{
  typedef CartesianGrid3D  C3D;
  typedef CartesianGrid3D::index_type it;

  for(int i = 0; i < 8; ++i)
    corner_offset[i] = index_type((i/4)%2,(i/2)%2 ,i%2);
  /* that is,
  corner_offset = 
  { it(0,0,0), it(0,0,1), it(0,1,0), it(0,1,1),
    it(1,0,0), it(1,0,1), it(1,1,0), it(1,1,1) 
  };
  */
  edge_index_offset[C3D::edge_direction::x] = it(1,0,0);
  edge_index_offset[C3D::edge_direction::y] = it(0,1,0);
  edge_index_offset[C3D::edge_direction::z] = it(0,0,1);
  
  facet_index_offset[C3D::facet_direction::xy][0] = it(1,0,0);
  facet_index_offset[C3D::facet_direction::xy][1] = it(0,1,0);
  facet_index_offset[C3D::facet_direction::xz][0] = it(1,0,0);
  facet_index_offset[C3D::facet_direction::xz][1] = it(0,0,1);
  facet_index_offset[C3D::facet_direction::yz][0] = it(0,1,0);
  facet_index_offset[C3D::facet_direction::yz][1] = it(0,0,1);


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



};
