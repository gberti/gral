#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Hierarchical/hierarchical-grid-function-base.h"
#include "Gral/Hierarchical/hierarchical-grid-function.h"
#include "Gral/Hierarchical/hierarchical-partial-grid-function.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"


#include <iostream>

template<class GRID>
void test_hier_gf(GRID const& root, GRID const& pattern, std::ostream& out) 
{
  typedef GRID flat_grid_type;
  typedef grid_types<flat_grid_type> cgt;
  typedef hierarchical::hgrid_cartesian<flat_grid_type> hier_grid_type;
  typedef hier_grid_type                                hgt;
  typedef typename hgt::level_handle                    level_handle;
  typedef typename hgt::hier_cell_type                        hier_cell_type; 

  hier_grid_type H(root,pattern);
  hierarchical::hier_grid_function        <hier_cell_type, int> Hgf (H, 77);
  hierarchical::hier_partial_grid_function<hier_cell_type, int> Hpgf(H, 77);

  
  H   .add_finer_level();
  //  Hgf .add_finer_level(1);
  //  Hpgf.add_finer_level(1);

  H   .add_coarser_level();
  // Hgf .add_coarser_level(-1);
  // Hpgf.add_coarser_level(-1);

  hierarchical::hier_grid_function        <hier_cell_type, int> Hgf2 (Hgf);
  hierarchical::hier_partial_grid_function<hier_cell_type, int> Hpgf2(Hpgf);

  hierarchical::hier_grid_function        <hier_cell_type, int> Hgf3;  Hgf3  = Hgf;
  hierarchical::hier_partial_grid_function<hier_cell_type, int> Hpgf3; Hpgf3 = Hpgf;


  for(level_handle lev = H.coarsest_level(); lev <= H.finest_level(); ++lev) {
    out << "Level " << lev << ":\n";
    for(typename cgt::CellIterator c(* H.FlatGrid(lev)); !c.IsDone(); ++c) {
      out << (*c).index() << ": " << Hgf(lev)(*c) << " = ";
      hier_cell_type hc(H,*c,lev);
      out << Hgf  (hc) << " = "
	  << Hpgf (hc) << " = "
	  << Hgf2 (hc) << " = "
	  << Hpgf2(hc) << " = "
	  << Hgf3 (hc) << " = "
	  << Hpgf3(hc) 
	  << "\n";
      Hpgf[hc] = Hgf[hc];
      REQUIRE_ALWAYS( Hpgf.defined(hc), "",1);
      Hpgf.undefine(hc);
      REQUIRE_ALWAYS(!Hpgf.defined(hc), "",1);
    }
  }

  
}


// explicit instantiation to make sure all members are compilable
namespace hierarchical { 
  template class hgrid_cartesian<cartesian3d::CartesianGrid3D>; 
  typedef hgrid_cartesian<cartesian3d::CartesianGrid3D>::hier_cell_type hier_cell_type;
  template class hier_grid_function<hier_cell_type, int>;

  template class hier_grid_function_base<hier_cell_type, int, grid_function>;
  template class hier_grid_function_base<hier_cell_type, int, partial_grid_function>;
}


int main() {
  using namespace std;
  cout << "\n------------- 2D --------------\n";
  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D               cart_grid_type;
    cart_grid_type root(3,3);
    cart_grid_type ref_pattern(3,2); // 2x1 cells!
    test_hier_gf(root,ref_pattern, cout);
  }
  cout << "\n------------- 3D --------------\n";
  {
    namespace cart = cartesian3d;
    typedef cart::CartesianGrid3D               cart_grid_type;
    cart_grid_type root(3,3,3);
    cart_grid_type ref_pattern(3,2,2); // 2x1x1 cells!
    test_hier_gf(root,ref_pattern, cout);
  }
}
