
/*! \file
    
   Test harness for \c restricted_grid_view
*/


#include "Gral/Views/restricted-grid-view.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Test/all.h"


#include "Utility/pre-post-conditions.h"


typedef GrAL::cartesian2d::CartesianGrid2D cart_grid_type;

struct true_pred  { bool operator()(cart_grid_type::Cell) const { return true; } };
struct false_pred { bool operator()(cart_grid_type::Cell) const { return false;} };

// using namespace GrAL;
namespace GrAL {
namespace restricted_grid_view {
  typedef cartesian2d::CartesianGrid2D grid_type;
  
  
  template class grid_view<grid_type, true_pred>;
}}


int main() {
  using namespace std;
  using namespace GrAL;

  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;
  grid_type R(2,2);
  
  namespace rgv = restricted_grid_view;
  rgv::grid_view<grid_type, true_pred >  all_view(R, true_pred());
  rgv::grid_view<grid_type, false_pred>  nil_view(R, false_pred());
  

  REQUIRE_ALWAYS( all_view.NumOfCells() == R.NumOfCells(), "", 1);
  REQUIRE_ALWAYS( nil_view.NumOfCells() == 0,              "", 1);
  REQUIRE_ALWAYS( all_view.NumOfVertices() == R.NumOfVertices(), "", 1);
  REQUIRE_ALWAYS( nil_view.NumOfVertices() == 0,              "", 1);

  typedef grid_types<rgv::grid_view<grid_type, true_pred >  > truegt;
  typedef grid_types<rgv::grid_view<grid_type, false_pred > > falsegt;


  /* cannot construct cells/vertices from handles
  test_cell_iterator  (all_view, cout);
  test_vertex_iterator(all_view, cout);
  test_cell_iterator  (nil_view, cout);
  test_vertex_iterator(nil_view, cout);
  */

  gt::CellIterator c(R);
  truegt::CellIterator tc(all_view);
  for( ; !c.IsDone(); ++c, ++tc)
    REQUIRE_ALWAYS( *c == *tc, "", 1);

}
