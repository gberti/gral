#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Hierarchical/hierarchical-grid-function-base.h"
#include "Gral/Hierarchical/hierarchical-grid-function.h"
#include "Gral/Hierarchical/hierarchical-partial-grid-function.h"
#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Base/multi-grid-functions.h"
#include "Gral/Base/partial-multi-grid-functions.h"

#include <iostream>

template<class GRID>
void test_hier_gf(GRID const& root, GRID const& pattern, std::ostream& out) 
{
  using std::flush;

  typedef GRID flat_grid_type;
  typedef grid_types<flat_grid_type> cgt;
  typedef hierarchical::hgrid_cartesian<flat_grid_type> hier_grid_type;
  typedef hier_grid_type                                hgt;
  typedef typename hgt::level_handle                    level_handle;
  typedef typename hgt::hier_cell_type                        hier_cell_type; 

  hier_grid_type H(root,pattern);
  hierarchical::hier_grid_function        <hier_cell_type, int> Hgf (H, 77);
  hierarchical::hier_partial_grid_function<hier_cell_type, int> Hpgf(H, 77);

  REQUIRE_ALWAYS(Hgf .valid(), "", 1);
  REQUIRE_ALWAYS(Hpgf.valid(), "", 1);
  
  H   .add_finer_level();
  REQUIRE_ALWAYS(Hgf .valid(), "", 1);
  REQUIRE_ALWAYS(Hpgf.valid(), "", 1);

  H   .add_coarser_level();
  REQUIRE_ALWAYS(Hgf .valid(), "", 1);
  REQUIRE_ALWAYS(Hpgf.valid(), "", 1);


  hierarchical::hier_grid_function        <hier_cell_type, int> Hgf2 (Hgf);
  hierarchical::hier_partial_grid_function<hier_cell_type, int> Hpgf2(Hpgf);

  hierarchical::hier_grid_function        <hier_cell_type, int> Hgf3;  Hgf3  = Hgf;
  hierarchical::hier_partial_grid_function<hier_cell_type, int> Hpgf3; Hpgf3 = Hpgf;


  for(level_handle lev = H.coarsest_level(); lev <= H.finest_level(); ++lev) {
    out << "Level " << lev << ":\n" << flush;
    for(typename cgt::CellIterator c(* H.FlatGrid(lev)); !c.IsDone(); ++c) {
      out << (*c).index() << ": " << Hgf(lev)(*c) << " = " << flush;
      hier_cell_type hc(H,*c,lev);
      out << Hgf  (hc) << " = " << flush;
      out << Hpgf (hc) << " = " << flush;
      out << Hgf2 (hc) << " = " << flush;
      out << Hpgf2(hc) << " = " << flush;
      out << Hgf3 (hc) << " = " << flush;
      out << Hpgf3(hc) << flush;
      out << "\n"  << flush;
      Hpgf[hc] = Hgf[hc];
      REQUIRE_ALWAYS( Hpgf.defined(hc), "",1);
      Hpgf.undefine(hc);
      REQUIRE_ALWAYS(!Hpgf.defined(hc), "",1);
    }
  }

  while(! H.empty()) {
    H.remove_finest_level();
    REQUIRE_ALWAYS(Hgf  .valid(), "", 1);
    REQUIRE_ALWAYS(Hpgf .valid(), "", 1);
    REQUIRE_ALWAYS(Hgf2 .valid(), "", 1);
    REQUIRE_ALWAYS(Hpgf2.valid(), "", 1);
    REQUIRE_ALWAYS(Hgf3 .valid(), "", 1);
    REQUIRE_ALWAYS(Hpgf3.valid(), "", 1);

  }
  
}


// explicit instantiation to make sure all members are compilable
namespace hierarchical { 
  typedef cartesiannd::grid<3>       cart_grid_type;
  typedef grid_types<cart_grid_type> cgt;

  template class hgrid_cartesian<cart_grid_type>;

  typedef hgrid_cartesian<cart_grid_type> hier_grid_type;
  typedef hier_grid_type::hier_cell_type hier_cell_type;
  template class hier_grid_function<hier_cell_type, int>;

  template class hier_grid_function_base<hier_grid_type,         grid_function<cgt::Cell, int> >;
  template class hier_grid_function_base<hier_grid_type, partial_grid_function<cgt::Cell, int> >;

  template class hier_grid_function_base<hier_grid_type,         multi_grid_function<cart_grid_type, int> >;
  template class hier_grid_function_base<hier_grid_type, partial_multi_grid_function<cart_grid_type, int> >;
}


namespace hierarchical { 
  typedef cartesiannd::grid<2>       cart_grid_type_2d;
  typedef grid_types<cart_grid_type_2d> cgt2d;
  template class hgrid_cartesian<cart_grid_type_2d>;

  typedef hgrid_cartesian<cart_grid_type_2d> hier_grid_type_2d;
  typedef hier_grid_type_2d::hier_cell_type hier_cell_type_2d;
  template class hier_grid_function<hier_cell_type_2d, int>;

  template class hier_grid_function_base<hier_grid_type_2d, grid_function<cgt2d::Cell, int> >;
  template class hier_grid_function_base<hier_grid_type_2d, partial_grid_function<cgt2d::Cell, int> >;

  template class hier_grid_function_base<hier_grid_type_2d,         multi_grid_function<cart_grid_type_2d, int> >;
  template class hier_grid_function_base<hier_grid_type_2d, partial_multi_grid_function<cart_grid_type_2d, int> >;
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
    typedef cartesiannd::grid<3>  cart_grid_type;
    typedef grid_types<cart_grid_type> gt;
    typedef gt::index_type             it;
    cart_grid_type root(it(3,3,3));
    cart_grid_type ref_pattern(it(3,2,2)); // 2x1x1 cells!
    test_hier_gf(root,ref_pattern, cout);
  }
}
