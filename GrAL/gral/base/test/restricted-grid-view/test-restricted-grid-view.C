
/*! \file
    
   Test harness for \c restricted_grid_view
*/


#include "Gral/Views/restricted-grid-view.h"
#include "Gral/Base/restricted-grid-function-adapter.h"

#include "Gral/Base/partial-grid-function-hash.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Test/all.h"

#include "Gral/Base/element-numbering.h"

#include "Utility/pre-post-conditions.h"

#include <boost/static_assert.hpp>

#include <algorithm>



typedef GrAL::cartesian2d::CartesianGrid2D cart_grid_type;

struct true_pred  { bool operator()(cart_grid_type::Cell)  const { return true; } };
struct false_pred { bool operator()(cart_grid_type::Cell)   const { return false;} };
struct even_pred  { bool operator()(cart_grid_type::Cell c) const { return ( (c.handle() % 2) == 0 ? true : false); } };

// using namespace GrAL;
namespace GrAL {
namespace restricted_grid_view {
  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;
  typedef cfg<gt, true_pred>           cfg_t;
  
  template class grid_view<cfg_t>; // grid_type, true_pred>;
}}


int main() {
  using namespace std;
  using namespace GrAL;

  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;
  grid_type R(3,3);
  
  namespace rgv = restricted_grid_view;
  typedef rgv::grid_view<rgv::cfg<gt, true_pred > > all_view_type;
  typedef rgv::grid_view<rgv::cfg<gt, false_pred> > nil_view_type;
  typedef rgv::grid_view<rgv::cfg<gt, even_pred > > even_view_type;

  typedef grid_types<all_view_type>  truegt;
  typedef grid_types<nil_view_type>  falsegt;
  typedef grid_types<even_view_type> evengt;

  all_view_type  all_view (R, true_pred());
  nil_view_type  nil_view (R, false_pred());
  even_view_type even_view(R, even_pred());
  
  typedef rgv::grid_view<rgv::cfg<truegt, true_pred> > all2_view_type;
  all2_view_type all2_view(all_view, true_pred());

  REQUIRE_ALWAYS( all_view.NumOfCells() == R.NumOfCells(), "", 1);
  REQUIRE_ALWAYS( nil_view.NumOfCells() == 0,              "", 1);
  REQUIRE_ALWAYS( all_view.NumOfVertices() == R.NumOfVertices(), "", 1);
  REQUIRE_ALWAYS( nil_view.NumOfVertices() == 0,              "", 1);


  cout << "checkgt<evengt>\n";
  checkgt<evengt>(cout);

  gt::FacetOnCellIterator fc(all_view.FirstCell());
  gt::Facet f = * fc;

  test_cell_iterator  (all_view, cout);
  test_vertex_iterator(all_view, cout);
  test_cell_iterator  (nil_view, cout);
  test_vertex_iterator(nil_view, cout);

  gt::CellIterator c(R);
  truegt::CellIterator tc(all_view);
  for( ; !c.IsDone(); ++c, ++tc)
    REQUIRE_ALWAYS( *c == *tc, "", 1);

  grid_function<gt::Cell, int> gf(R,0);
  grid_function_view<evengt::Cell, int> even_gf(even_view, gf);

  grid_function_view<evengt::Cell, int, const grid_function<gt::Cell, int> > even_const_gf(even_view, gf);
  grid_function_view<evengt::Cell, int> even_gf2 = make_grid_function_view(even_view,gf);


  REQUIRE_ALWAYS(even_gf.size() == even_view.NumOfCells(), "", 1);

  for(evengt::CellIterator ec(even_view); !ec.IsDone(); ++ec) {
    even_gf[*ec] = 1;
    int i = even_const_gf(*ec);
  }

  bool eq = std::equal(even_gf.begin(), even_gf.end(), even_const_gf.begin());

  grid_function        <truegt::Edge, int> tge(all_view,1);
  partial_grid_function<truegt::Edge, int> pge(all_view,1);

  BOOST_STATIC_ASSERT((1== is_base_or_same<grid_incidence_iterator_category, 
		                           truegt::categories<truegt::FacetOnCellIterator>::type>::value));
  BOOST_STATIC_ASSERT((1== is_base_or_same<grid_incidence_iterator_category, 
		                           truegt::categories<truegt::VertexOnFaceIterator>::type>::value));
  BOOST_STATIC_ASSERT((1== is_base_or_same<grid_sequence_iterator_category, 
		                           truegt::categories<truegt::CellIterator>::type>::value));
  BOOST_STATIC_ASSERT((1== boost::is_same<truegt::VertexOnCellIterator, truegt::VertexOnFaceIterator>::value));


  test_sequence_iterator<truegt::Vertex>(all_view, cout);
  test_sequence_iterator<truegt::Edge  >(all_view, cout);
  test_sequence_iterator<truegt::Cell  >(all_view, cout);

  test_incidence_iterator<truegt::Vertex,truegt::Edge>(all_view, cout);

  test_incidence_iterator<truegt::Vertex,truegt::Cell>(all_view, cout);
  test_incidence_iterator<truegt::Edge,  truegt::Cell>(all_view, cout);
  test_incidence_iterator<truegt::Cell,  truegt::Cell>(all_view, cout);


  element_numbering<gt::Vertex>  VNum(R,1);

  for(truegt::CellIterator tc(all_view); !tc.IsDone(); ++tc) {
    REQUIRE_ALWAYS((*tc).NumOfCells() == (*tc).Base().NumOfCells(), "tc.NumOfCells() = " << (*tc).NumOfCells(), 1);
    truegt::CellOnCellIterator tcc1 = (*tc).FirstCell();
    for(truegt::CellOnCellIterator tcc(*tc); !tcc.IsDone(); ++tcc) {
      REQUIRE_ALWAYS(all_view.inside(*tcc), "", 1);
    }
  }

  for(truegt::VertexIterator tv(all_view); !tv.IsDone(); ++tv) {
    REQUIRE_ALWAYS((*tv).NumOfCells() == (*tv).Base().NumOfCells(), "tv.NumOfCells() = " << (*tv).NumOfCells(), 1);
    truegt::CellOnVertexIterator tcv1 = (*tv).FirstCell();
    for(truegt::CellOnVertexIterator tcv(*tv); !tcv.IsDone(); ++tcv) {
      REQUIRE_ALWAYS(all_view.inside(*tcv), "", 1);
    }
  }



}
