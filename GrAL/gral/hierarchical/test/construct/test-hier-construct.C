
#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

int main() {
  using namespace std;
  namespace hier = hierarchical;

  {
    namespace cart = cartesian3d;
    typedef cart::CartesianGrid3D cart_grid_type;
    typedef grid_types<cart_grid_type> cgt;
    typedef hier::hgrid_cartesian<cart_grid_type> hier_grid_type;
    typedef hier_grid_type                        hgt;

    cart_grid_type root(3,3,3);
    cart_grid_type ref_pattern(3,3,3); // 2x2x2 cells!
    
    hier_grid_type H(root,ref_pattern);
    H.add_finer_level();
    H.add_coarser_level();
    
    for(hgt::level_handle lev = H.coarsest_level(); lev <= H.finest_level(); lev = H.next_finer_level(lev))
      cout << "Level " << lev << ": " << H.FlatGrid(lev).cell_size() << " cells\n";
    
    for(cgt::CellIterator c = H.FlatGrid(H.finest_level()).FirstCell(); !c.IsDone(); ++c) {
      hgt::HierCell h(H,*c,H.finest_level());
      hgt::HierCell p = H.Parent(h);
      hgt::HierCell pp = H.Parent(p);
      
      cout << "child: " << h.Flat().index()  << "; "
	   << "parent: " << p.Flat().index() << "; "
	   << "grandp: " << pp.Flat().index() << "\n";
    }
  }
  
  cout << "----------------------------\n";
  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D cart_grid_type;
    typedef grid_types<cart_grid_type> cgt;
    typedef hier::hgrid_cartesian<cart_grid_type> hier_grid_type;
    typedef hier_grid_type                        hgt;

    // CartesianGrid2D can be have lower index != (0,0)!
    // Then hierarchical_grid does not work any more!
    cart_grid_type root(4,4);
    cart_grid_type ref_pattern(4,4); // 3x3 cells!
    
    hier_grid_type H(root,ref_pattern);
    H.add_finer_level();
    H.add_coarser_level();
    
    for(hgt::level_handle lev = H.coarsest_level(); lev <= H.finest_level(); lev = H.next_finer_level(lev))
      cout << "Level " << lev << ": " << H.FlatGrid(lev).cell_size() << " cells\n";
    
    for(cgt::CellIterator c = H.FlatGrid(H.finest_level()).FirstCell(); !c.IsDone(); ++c) {
      hgt::HierCell h(H,*c,H.finest_level());
      hgt::HierCell p = H.Parent(h);
      hgt::HierCell pp = H.Parent(p);
      
      cout << "child: " << h.Flat().index()  << "; "
	   << "parent: " << p.Flat().index() << "; "
	   << "grandp: " << pp.Flat().index() << "\n";
      
    }
  }
  
}
