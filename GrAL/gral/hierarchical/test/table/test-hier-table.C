
#include "Gral/Hierarchical/hierarchical-grid-table.h"
#include "Gral/Hierarchical/hierarchical-grid.h"

#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include "Geometry/point-traits.h"

#include "Container/functions.h"
#include <iostream>


template<class GEOM, class GRID>
void test_level(GRID const& G, GEOM const& Geom, std::ostream& out)
{
  typedef grid_types<GRID> gt;
  for (typename gt::CellIterator c = G.FirstCell(); ! c.IsDone(); ++c)
    out << "Cell " << (*c).index()  << " center @ " 
	<< Geom.center(*c) << "\n";
}

template<class GEOM, class GRID>
void test_hier_grid_table(GRID const& root,
			  GRID const& pattern,
			  std::ostream & out)
{
  typedef GRID flat_grid_type;
  typedef GEOM flat_geom_type;
  typedef grid_types<flat_grid_type> cgt;
  typedef hierarchical::hgrid_cartesian<flat_grid_type> hier_grid_type;
  typedef hier_grid_type                                hgt;
  typedef typename hgt::level_handle                    level_handle;
  typedef hierarchical::hier_grid_table<hier_grid_type, flat_geom_type> hier_geom_type;
  typedef typename flat_geom_type::mapping_type         mapping_type;
  
  mapping_type M(mapping_type::identity());

  hier_grid_type H(root,pattern);
  H.add_finer_level();
  H.add_coarser_level();
  hier_geom_type Hgeom(H, M);    

  hier_grid_type H2(H);
  H = H2;
  hier_geom_type Hgeom2(H);

  for(typename hgt::level_handle lev = H.coarsest_level(); lev <= H.finest_level(); lev = H.next_finer_level(lev))
    out << "Level " << lev << ": " << H.FlatGrid(lev)->cell_size() << " cells\n";
  REQUIRE_ALWAYS( (Hgeom.coarsest_level() == Hgeom2.coarsest_level() && Hgeom.finest_level() == Hgeom2.finest_level()), "",1);

  {
    level_handle lev = H.finest_level();
    out << "Level " << lev << ":\n";
    test_level(* H.FlatGrid(lev), Hgeom(lev), out);
  }
  {
    H.add_finer_level();
    REQUIRE_ALWAYS( (Hgeom.coarsest_level() == Hgeom2.coarsest_level() && Hgeom.finest_level() == Hgeom2.finest_level()), "",1);

    level_handle lev = H.finest_level();
    out << "Level " << lev << ":\n";
    test_level(* H.FlatGrid(lev), Hgeom(lev), out);
  }

  {
    H.remove_finest_level();
    REQUIRE_ALWAYS( (Hgeom.coarsest_level() == Hgeom2.coarsest_level() && Hgeom.finest_level() == Hgeom2.finest_level()), "",1);
    level_handle lev = H.finest_level();
    out << "Level " << lev << ":\n";
    test_level(* H.FlatGrid(lev), Hgeom(lev), out);
  }

  {
    H    .remove_coarsest_level();
    REQUIRE_ALWAYS( (Hgeom.coarsest_level() == Hgeom2.coarsest_level() && Hgeom.finest_level() == Hgeom2.finest_level()), "",1);
    level_handle lev = H.coarsest_level();
    out << "Level " << lev << ":\n";
    test_level(* H.FlatGrid(lev), Hgeom(lev), out);
  }
  {
    H    .add_coarser_level();
    REQUIRE_ALWAYS( (Hgeom.coarsest_level() == Hgeom2.coarsest_level() && Hgeom.finest_level() == Hgeom2.finest_level()), "",1);
    level_handle lev = H.coarsest_level();
    out << "Level " << lev << ":\n";
    test_level(* H.FlatGrid(lev), Hgeom(lev), out);
  }

  // test copying
  {
    hier_geom_type Hgeom2 = Hgeom;
    level_handle lev = H.coarsest_level();
    out << "Copy: Level " << lev << ":\n";
    test_level(* H.FlatGrid(lev), Hgeom2(lev), out);
  }

  while(! H.empty()) {
    H    .remove_coarsest_level();
  }


}

// explicit instantiation to make sure all members are compilable
namespace hierarchical { 
  //  template class hgrid_cartesian<cartesian3d::CartesianGrid3D>; 
  template class hgrid_cartesian<cartesiannd::grid<3> >;
  template class hgrid_cartesian<cartesian2d::CartesianGrid2D>; 
}


int main() {
  using namespace std;
  namespace hier = hierarchical;
 
  {
    namespace cart = cartesiannd;
    typedef cart::grid<3>                       cart_grid_type;
    typedef tuple<double,3>                     coord_type;
    typedef matrix<3,3,0>                                       matrix_type;
    typedef affine_mapping<matrix_type, coord_type>             mapping_type;
    typedef cart::mapped_geometry<cart_grid_type, mapping_type> cart_geom_type;
    typedef grid_types<cart_grid_type> gt;
    typedef gt::index_type it;
    cart_grid_type root(it(3,3,3));
    cart_grid_type ref_pattern(it(3,2,2)); // 2x1x1 cells!
    test_hier_grid_table<cart_geom_type>(root, ref_pattern, cout);     


  }
  

  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D               cart_grid_type;
    typedef tuple<double,2>                     coord_type;
    typedef matrix<2,2,0>                                       matrix_type;
    typedef affine_mapping<matrix_type, coord_type>             mapping_type;
    typedef cart::mapped_geometry<mapping_type> cart_geom_type;

    cart_grid_type root(3,3);
    cart_grid_type ref_pattern(3,2); // 2x1 cells!
    test_hier_grid_table<cart_geom_type>(root, ref_pattern, cout);     
  }

  
}
