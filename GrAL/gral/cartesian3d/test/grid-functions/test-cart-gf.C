
// $LICENSE


/*! \file
 */

#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Test/test-grid-functions.h"
#include "Gral/Test/test-partial-grid-functions.h"
#include "Utility/pre-post-conditions.h"

#include <fstream>

int main() {
  using namespace GrAL;
  using namespace std;
  using namespace GrAL::cartesian3d;

  typedef CartesianGrid3D grid_type;
  typedef grid_types<grid_type> gt;
 
  ostream& out = cout;

  // check empty grid
  grid_type EmptyG; 
  REQUIRE_ALWAYS(EmptyG.NumOfVertices() == 0,
		 " NumOfVertices() == " << EmptyG.NumOfVertices(), 1);
  REQUIRE_ALWAYS(EmptyG.NumOfEdges() == 0,
		 " NumOfEdges() == " << EmptyG.NumOfEdges(), 1);
  REQUIRE_ALWAYS(EmptyG.NumOfFacets() == 0,
		 " NumOfFacets() == " << EmptyG.NumOfFacets(), 1);
  REQUIRE_ALWAYS(EmptyG.NumOfCells() == 0,
		 " NumOfCells() == " << EmptyG.NumOfCells(), 1);

  test_grid_functions<gt::Vertex>(EmptyG,out);
  test_grid_functions<gt::Edge  >(EmptyG,out);
  test_grid_functions<gt::Facet >(EmptyG,out);
  test_grid_functions<gt::Cell  >(EmptyG,out);
  test_partial_grid_functions<gt::Vertex>(EmptyG,out);
  test_partial_grid_functions<gt::Edge  >(EmptyG,out);
  test_partial_grid_functions<gt::Facet >(EmptyG,out);
  test_partial_grid_functions<gt::Cell  >(EmptyG,out);


  grid_type G(3,3,3);

  // check grid functions
  test_grid_functions<gt::Vertex>(G,out);
  test_grid_functions<gt::Edge  >(G,out);
  test_grid_functions<gt::Facet >(G,out);
  test_grid_functions<gt::Cell  >(G,out);
  test_partial_grid_functions<gt::Vertex>(G,out);
  test_partial_grid_functions<gt::Edge  >(G,out);
  test_partial_grid_functions<gt::Facet >(G,out);
  test_partial_grid_functions<gt::Cell  >(G,out);

  return 0;
}
