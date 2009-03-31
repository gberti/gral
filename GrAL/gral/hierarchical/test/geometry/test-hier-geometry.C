/*! \file
    Tests for \c hierarchical::hier_geometry<>

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Hierarchical/hierarchical-geometry.h"

#include "Gral/Hierarchical/hierarchical-grid.h"

#include "Geometry/point-traits.h"

#include "Gral/Grids/CartesianND/mapped-geometry.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include <iostream>

using namespace GrAL;

template<class GRID, class GEOM>
void test_hier_geom(GRID const& root,
		    GRID const& pattern,
		    GEOM const& root_geom,
		    std::ostream & out)
{
  typedef GRID cart_grid_type;
  typedef GEOM cart_geom_type;
  typedef grid_types<cart_grid_type> cgt;
  typedef hierarchical::hgrid_cartesian<cart_grid_type> hier_grid_type;
  typedef hier_grid_type                                hgt;
  typedef typename hgt::hier_cell_type                  hier_cell_type;
  typedef typename hgt::hier_vertex_type                hier_vertex_type;
  typedef typename hgt::level_handle                    level_handle;
  typedef typename hier_grid_type::element_base_type    element_base_type;
  typedef hierarchical::hier_geometry<hier_grid_type,
                                      cart_geom_type>   hier_geom_type;

  hier_grid_type H(root,pattern);

  H.add_finer_level();
  hier_geom_type HGeom(H, root_geom);
  H.add_finer_level();


  //  H    .add_coarser_level();
  // HGeom.add_coarser_level();

  for(level_handle lev = H.coarsest_level(); lev <= H.finest_level(); ++lev) {
    out << "Level " << lev << ":\n";
    for(typename cgt::VertexIterator v = H.FlatGrid(lev)->FirstVertex(); !v.IsDone(); ++v) {
      hier_vertex_type hv(H,*v,lev);
      out << hv.Flat().index() << " @ " << HGeom.coord(hv) << "\n";
    }
    out << "\n";
  }
}

// explicit instantiation to make sure all members are compilable
/*
namespace hierarchical { 
  //  template class hgrid_cartesian<cartesian3d::CartesianGrid3D>; 
  template class hgrid_cartesian<cartesian2d::CartesianGrid2D>; 

  typedef hgrid_cartesian<cartesian2d::CartesianGrid2D> hier_grid2d_type;

  typedef tuple<double,2>                            coord_type;
  typedef stdext::identity<coord_type>               mapping_type;
  typedef cartesian2d::mapped_geometry<mapping_type> cart_geom2d_type;
  template class hier_geometry<hier_grid2d_type, cart_geom2d_type>;
}
*/

namespace GrAL {
namespace hierarchical { 
  typedef cartesiannd::grid<2>                                           cart_grid2d_type;
  typedef hgrid_cartesian<cartesiannd::grid<2> >                         hier_grid2d_type;

  typedef tuple<double,2>                                                coord2d_type;
  typedef matrix<2,2,0>                                                  matrix2d_type;
  typedef affine_mapping<matrix2d_type, coord2d_type>                    mapping2d_type;
  typedef cartesiannd::mapped_geometry<cart_grid2d_type, mapping2d_type> cart_geom2d_type;

  template class hgrid_cartesian<cart_grid2d_type>;
  template class hier_geometry<hier_grid2d_type, cart_geom2d_type>;
}}

int main() {
  using namespace GrAL;
  using namespace std;
  namespace hier = hierarchical;

  {
    typedef hier::cart_grid2d_type  cart_grid_type;
    typedef hier::cart_geom2d_type  cart_geom_type;
    typedef hier::coord2d_type      coord_type;
    typedef hier::mapping2d_type    mapping_type;
  
    {
      cout << "Grid1: 1x1 cells" << endl;
      cart_grid_type root(2,2);
      cart_grid_type ref_pattern(3,3); // 2x2 cells!
      cart_geom_type root_geom(root,mapping_type::scaling(coord_type(4,8)));
      test_hier_geom(root, ref_pattern, root_geom, cout);     
    }
    {
      cout << "Grid2: 2x1 cells" << endl;
      cart_grid_type root(3,2); // 2x1 cells
      cart_grid_type ref_pattern(3,3); // 2x2 cells!
      cart_geom_type root_geom(root, mapping_type::scaling(coord_type(4,4)));
      test_hier_geom(root, ref_pattern, root_geom, cout);     
    }
  }
  

  
}
