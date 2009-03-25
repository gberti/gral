

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/IO/stream-grid-adapter.h"

#include "Gral/Grids/Complex2D/construct.h"

namespace GrAL {
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

  CartesianGrid3D::CartesianGrid3D(CartesianGrid3D::index_type v)
  { 
    if(v[0] <=0 || v[1] <= 0 || v[2] <= 0)
      // empty grid
      init(0,0,0);
    else  
      init(v[0],v[1],v[2]);
  }

  
  void CartesianGrid3D::init(int vx, int vy, int vz)
  {
    int vx_1 = (vx > 0 ? vx-1 : 0);
    int vy_1 = (vy > 0 ? vy-1 : 0);
    int vz_1 = (vz > 0 ? vz-1 : 0);
    int v[3] = {vx,vy,vz};
    vertex_map = index_map_type(index_type(v));
    int c[3] = {vx_1,vy_1,vz_1};
    cell_map   = index_map_type(index_type(c)); 
    
    int ex[3] = {vx_1,vy  ,vz  };
    edge_maps[edge_direction::x] = index_map_type(index_type(ex));
    int ey[3] = {vx  ,vy_1,vz  };
    edge_maps[edge_direction::y] = index_map_type(index_type(ey));
    int ez[3] = {vx  ,vy  ,vz_1};
    edge_maps[edge_direction::z] = index_map_type(index_type(ez));
    
    int fxy[3] = {vx_1,vy_1,vz  };
    facet_maps[facet_direction::xy] = index_map_type(index_type(fxy));
    int fxz[3] = {vx_1,vy  ,vz_1};
    facet_maps[facet_direction::xz] = index_map_type(index_type(fxz));
    int fyz[3] = {vx  ,vy_1,vz_1};
    facet_maps[facet_direction::yz] = index_map_type(index_type(fyz));

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
    vertex_on_cell_index_offset[i] = index_type((i/4)%2,(i/2)%2 ,i%2);
  /* that is,
  corner_offset = 
  { it(0,0,0), it(0,0,1), it(0,1,0), it(0,1,1),
    it(1,0,0), it(1,0,1), it(1,1,0), it(1,1,1) 
  };
  */
  // connectivity of the cube above
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

  typedef grid_types<archetype_type> agt;

  // set edge on cell information
  // loop over edges of archetype and see how they are aligned
  int le = 0;
  for(agt::EdgeIterator e(the_archetype[0]); ! e.IsDone(); ++e, ++le) {
    it v1 = C3D::ArchetypeGeom().coord((*e).V1());
    it v2 = C3D::ArchetypeGeom().coord((*e).V2());
    it dir = v1 - v2;
    int cnt = 0; // for checking that vertex coords differ in exactly one place
    for(int i = 0; i < 3; ++i) {
      // vertex coords differ in exactly one place
      if(dir[i] != 0) { 
	++cnt;
	edge_on_cell_dir[le] = edge_direction::dir(i); 
	if(dir[i] > 0)
	  edge_on_cell_index_offset[le] = v2;
	else // dir[i] < 0
	  edge_on_cell_index_offset[le] = v1;
	// break;
      }
    }
    REQUIRE_ALWAYS( (cnt == 1), "Invalid edge, le=" << le << " v1=" << v1 << " v2=" << v2 << " dir=" << dir, 1);
  }

  // set facet-on-cell information
  // loop over facets (archetype cells) and look where they are
  int lf = 0;
  for(agt::CellIterator f(the_archetype[0]); ! f.IsDone(); ++f, ++lf) {
    it v[4];
    int lv = 0;
    for(agt::VertexOnCellIterator vf(*f); ! vf.IsDone(); ++vf, ++lv) 
      v[lv] = C3D::ArchetypeGeom().coord(*vf);

    int cnt = 0; // for checking that vertex coords coincide in exactly one place
    for(int i = 0; i < 3; ++i) {
      // vertex coords coincide in exactly one place
      if( (v[0][i] == v[1][i]) && (v[1][i] == v[2][i]) && (v[2][i] == v[3][i])) { 
	++cnt;
	facet_on_cell_dir[lf] = facet_direction::dir(2-i); // x values coincide => dir = yz etc.
	it offset(0); offset[i] = v[0][i];
        facet_on_cell_index_offset[lf] = offset; 
      }
    }
    REQUIRE_ALWAYS( (cnt == 1), "Invalid facet, lf=" << lf,1);
  }



  edge_index_offset[C3D::edge_direction::x] = it(1,0,0);
  edge_index_offset[C3D::edge_direction::y] = it(0,1,0);
  edge_index_offset[C3D::edge_direction::z] = it(0,0,1);
  
  facet_index_offset[C3D::facet_direction::xy][0] = it(1,0,0);
  facet_index_offset[C3D::facet_direction::xy][1] = it(0,1,0);
  facet_index_offset[C3D::facet_direction::xz][0] = it(1,0,0);
  facet_index_offset[C3D::facet_direction::xz][1] = it(0,0,1);
  facet_index_offset[C3D::facet_direction::yz][0] = it(0,1,0);
  facet_index_offset[C3D::facet_direction::yz][1] = it(0,0,1);


  // vertices on facets are traversed cyclically
  vertex_on_facet_index_offset[C3D::facet_direction::xy][0] = it(0,0,0);
  vertex_on_facet_index_offset[C3D::facet_direction::xy][1] = it(1,0,0);
  vertex_on_facet_index_offset[C3D::facet_direction::xy][2] = it(1,1,0);
  vertex_on_facet_index_offset[C3D::facet_direction::xy][3] = it(0,1,0);

  vertex_on_facet_index_offset[C3D::facet_direction::xz][0] = it(0,0,0);
  vertex_on_facet_index_offset[C3D::facet_direction::xz][1] = it(1,0,0);
  vertex_on_facet_index_offset[C3D::facet_direction::xz][2] = it(1,0,1);
  vertex_on_facet_index_offset[C3D::facet_direction::xz][3] = it(0,0,1);

  vertex_on_facet_index_offset[C3D::facet_direction::yz][0] = it(0,0,0);
  vertex_on_facet_index_offset[C3D::facet_direction::yz][1] = it(0,1,0);
  vertex_on_facet_index_offset[C3D::facet_direction::yz][2] = it(0,1,1);
  vertex_on_facet_index_offset[C3D::facet_direction::yz][3] = it(0,0,1);
 
  // edges on facets are traversed cyclically
  edge_on_facet_index_offset[C3D::facet_direction::xy][0] = it(0,0,0);
  edge_on_facet_dir         [C3D::facet_direction::xy][0] = edge_direction::x;
  edge_on_facet_index_offset[C3D::facet_direction::xy][1] = it(1,0,0);
  edge_on_facet_dir         [C3D::facet_direction::xy][1] = edge_direction::y;
  edge_on_facet_index_offset[C3D::facet_direction::xy][2] = it(0,1,0);
  edge_on_facet_dir         [C3D::facet_direction::xy][2] = edge_direction::x;
  edge_on_facet_index_offset[C3D::facet_direction::xy][3] = it(0,0,0);
  edge_on_facet_dir         [C3D::facet_direction::xy][3] = edge_direction::y;

  edge_on_facet_index_offset[C3D::facet_direction::xz][0] = it(0,0,0);
  edge_on_facet_dir         [C3D::facet_direction::xz][0] = edge_direction::x;
  edge_on_facet_index_offset[C3D::facet_direction::xz][1] = it(1,0,0);
  edge_on_facet_dir         [C3D::facet_direction::xz][1] = edge_direction::z;
  edge_on_facet_index_offset[C3D::facet_direction::xz][2] = it(0,0,1);
  edge_on_facet_dir         [C3D::facet_direction::xz][2] = edge_direction::x;
  edge_on_facet_index_offset[C3D::facet_direction::xz][3] = it(0,0,0);
  edge_on_facet_dir         [C3D::facet_direction::xz][3] = edge_direction::z;

  edge_on_facet_index_offset[C3D::facet_direction::yz][0] = it(0,0,0);
  edge_on_facet_dir         [C3D::facet_direction::yz][0] = edge_direction::y;
  edge_on_facet_index_offset[C3D::facet_direction::yz][1] = it(0,1,0);
  edge_on_facet_dir         [C3D::facet_direction::yz][1] = edge_direction::z;
  edge_on_facet_index_offset[C3D::facet_direction::yz][2] = it(0,0,1);
  edge_on_facet_dir         [C3D::facet_direction::yz][2] = edge_direction::y;
  edge_on_facet_index_offset[C3D::facet_direction::yz][3] = it(0,0,0);
  edge_on_facet_dir         [C3D::facet_direction::yz][3] = edge_direction::z;




}


} // namespace cartesian3d
} // namespace GrAL 

