
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
  
  
  template class grid_view<grid_type, true_pred>;
}}


int main() {
  using namespace std;
  using namespace GrAL;

  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;
  grid_type R(3,3);
  
  namespace rgv = restricted_grid_view;
  rgv::grid_view<grid_type, true_pred >  all_view (R, true_pred());
  rgv::grid_view<grid_type, false_pred>  nil_view (R, false_pred());
  rgv::grid_view<grid_type, even_pred >  even_view(R, even_pred());
  

  REQUIRE_ALWAYS( all_view.NumOfCells() == R.NumOfCells(), "", 1);
  REQUIRE_ALWAYS( nil_view.NumOfCells() == 0,              "", 1);
  REQUIRE_ALWAYS( all_view.NumOfVertices() == R.NumOfVertices(), "", 1);
  REQUIRE_ALWAYS( nil_view.NumOfVertices() == 0,              "", 1);

  typedef grid_types<rgv::grid_view<grid_type, true_pred  > > truegt;
  typedef grid_types<rgv::grid_view<grid_type, false_pred > > falsegt;
  typedef grid_types<rgv::grid_view<grid_type, even_pred  > > evengt;

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
  BOOST_STATIC_ASSERT((1== is_base_or_same<grid_types_detail::grid_test_category,
		                           truegt::categories<truegt::test_type>::type>::value));
  
  BOOST_STATIC_ASSERT((1== boost::is_same<truegt::VertexOnCellIterator, truegt::VertexOnFaceIterator>::value));


  test_sequence_iterator<truegt::Vertex>(all_view, cout);
  test_sequence_iterator<truegt::Edge  >(all_view, cout);
  test_sequence_iterator<truegt::Cell  >(all_view, cout);

  test_incidence_iterator<truegt::Vertex,truegt::Edge>(all_view, cout);

  test_incidence_iterator<truegt::Vertex,truegt::Cell>(all_view, cout);
  test_incidence_iterator<truegt::Edge,  truegt::Cell>(all_view, cout);
  test_incidence_iterator<truegt::Cell,  truegt::Cell>(all_view, cout);

  //  truegt::CellIterator c1 =  begin<truegt::Cell>(all_view);
  //truegt::CellOnCellIterator cc1 = begin<truegt::Cell>(*c1);

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
