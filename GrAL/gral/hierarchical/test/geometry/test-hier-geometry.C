/*! \file
    Tests for \c hierarchical::hier_geometry<>

*/

#include "Gral/Hierarchical/hierarchical-geometry.h"

#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include "Geometry/point-traits.h"

#include "Container/functions.h"

#include <iostream>

template<class T, unsigned N>
struct point_traits<tuple<T,N> > : public point_traits_fixed_size_array<tuple<T,N>, T, N> {};


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
  hier_geom_type HGeom(H /*, root_geom */);

  H    .add_finer_level();
  HGeom.add_finer_level();

  H    .add_finer_level();
  HGeom.add_finer_level();

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
namespace hierarchical { 
  //  template class hgrid_cartesian<cartesian3d::CartesianGrid3D>; 
  template class hgrid_cartesian<cartesian2d::CartesianGrid2D>; 

  typedef hgrid_cartesian<cartesian2d::CartesianGrid2D> hier_grid2d_type;

  typedef tuple<double,2>                            coord_type;
  typedef stdext::identity<coord_type>               mapping_type;
  typedef cartesian2d::mapped_geometry<mapping_type> cart_geom2d_type;
  template class hier_geometry<hier_grid2d_type, cart_geom2d_type>;
}

int main() {
  using namespace std;
  namespace hier = hierarchical;

  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D               cart_grid_type;
    typedef tuple<double,2>                     coord_type;
    typedef stdext::identity<coord_type>        mapping_type;
    typedef cart::mapped_geometry<mapping_type> cart_geom_type;

    cart_grid_type root(2,2);
    cart_grid_type ref_pattern(3,3); // 2x2 cells!
    cart_geom_type root_geom(root);
    test_hier_geom(root, ref_pattern, root_geom, cout);     
  }

  
}
