
// $LICENSE_NEC_2003

/*! \file 
   Tests for Triang3D
 */

#include "Gral/Grids/Triang3D/all.h"
#include "Gral/IO/gmv-format-output3d.h"
#include "Gral/Test/all.h"

#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;
  
  typedef Triang3D              grid_type;
  typedef grid_types<grid_type> gt;

  const int nc = 2;
  const int nv = 5;
  int    cells [4*nc] = { 0, 1, 2, 3,   1, 3, 2, 4};
  double coords[3*nv] = { 0.0, 0.0, 0.0,
			  1.0, 0.0, 0.0, 
			  0.0, 1.0, 0.0, 
			  0.0, 0.0, 1.0, 
			  1.0, 1.0, 1.0}; 
  Triang3D                     T(cells, nc, nv);
  stored_geometry_triang3d GeomT(T, coords);

  test_vertex_iterator        (T, cout);
  test_edge_iterator          (T, cout);
  test_facet_iterator         (T, cout);
  test_cell_iterator          (T, cout);
  test_vertex_on_cell_iterator(T, cout);
  test_edge_on_cell_iterator  (T, cout);
  test_facet_on_cell_iterator (T, cout);

  test_vertex_on_facet_iterator(T, cout);

  test_grid_functions<gt::Vertex>(T, cout);
  test_grid_functions<gt::Cell>  (T, cout);
  test_grid_functions<gt::Edge>  (T, cout);
  test_grid_functions<gt::Facet> (T, cout);
  test_partial_grid_functions<gt::Vertex>(T, cout);
  test_partial_grid_functions<gt::Edge>(T, cout);
  test_partial_grid_functions<gt::Facet>(T, cout);
  test_partial_grid_functions<gt::Cell>  (T, cout);

  test_archetypes(T, cout);

  vector<grid_type> TT(2);
  TT[0] = TT[1] = T;
  test_vertex_iterator        (TT[0], cout);
  test_cell_iterator          (TT[0], cout);
  test_vertex_on_cell_iterator(TT[0], cout);

  test_grid_functions<gt::Vertex>(TT[0], cout);
  test_grid_functions<gt::Cell>  (TT[0], cout);
  test_partial_grid_functions<gt::Vertex>(TT[0], cout);
  test_partial_grid_functions<gt::Cell>  (TT[0], cout);

  test_archetypes(TT[0], cout);


  OstreamGMV3DFmt Out("2cells.gmv");
  ConstructGrid(Out,T,GeomT);


}
