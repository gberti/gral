
#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

template<class GRID>
void test_hier_grid(GRID const& root,
		    GRID const& pattern,
		    ostream & out)
{
  typedef GRID cart_grid_type;
  typedef grid_types<cart_grid_type> cgt;
  typedef hierarchical::hgrid_cartesian<cart_grid_type> hier_grid_type;
  typedef hier_grid_type                                hgt;

  hier_grid_type H(root,pattern);
  H.add_finer_level();
  H.add_coarser_level();
    
  for(typename hgt::level_handle lev = H.coarsest_level(); lev <= H.finest_level(); lev = H.next_finer_level(lev))
    out << "Level " << lev << ": " << H.FlatGrid(lev).cell_size() << " cells\n";
  
  for(typename cgt::CellIterator c = H.FlatGrid(H.finest_level()).FirstCell(); !c.IsDone(); ++c) {
    hgt::HierCell h(H,*c,H.finest_level());
    hgt::HierCell p = H.Parent(h);
    hgt::HierCell pp = H.Parent(p);
    
    out << "child: " << h.Flat().index()  << "; "
	<< "parent: " << p.Flat().index() << "; (children: ";
    for(typename hgt::CellChildIterator ch(p); ! ch.IsDone(); ++ch) {
      REQUIRE_ALWAYS(p == ch.Parent(), "", 1);
      out << ch.Flat().index() << ", ";
    }
    out << "\b\b) "
	<< "grandp: " << pp.Flat().index() << "\n";
  }
}

int main() {
  using namespace std;
  namespace hier = hierarchical;

  {
    namespace cart = cartesian3d;
    typedef cart::CartesianGrid3D cart_grid_type;

    cart_grid_type root(3,3,3);
    cart_grid_type ref_pattern(3,3,3); // 2x2x2 cells!
    test_hier_grid(root, ref_pattern, cout);     
  }
  
  cout << "----------------------------\n";
  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D cart_grid_type;

    // CartesianGrid2D can be have lower index != (0,0)!
    // Then hierarchical_grid does not work any more!
    cart_grid_type root(4,4);
    cart_grid_type ref_pattern(4,4); // 3x3 cells!
    test_hier_grid(root, ref_pattern, cout);   
  }
  
}
