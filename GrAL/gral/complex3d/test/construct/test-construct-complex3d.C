#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Test/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include <iostream>

int main() {
  using std::cout;  
  using cartesian3d::CartesianGrid3D;

  typedef grid_types<Complex3D> gt; 
  CartesianGrid3D R(3,3,3);
  Complex3D       G;

  ConstructGrid0(G,R);
 
  test_vertex_iterator(G,cout);
  test_edge_iterator(G,cout);
  test_facet_iterator(G,cout);
  test_cell_iterator(G,cout);

  test_vertex_on_cell_iterator(G,cout);
  test_edge_on_cell_iterator(G,cout);
  test_facet_on_cell_iterator(G,cout);

  Complex3D G2;
  ConstructGrid0(G2,G);

  test_vertex_iterator(G,cout);
  test_edge_iterator(G,cout);
  test_facet_iterator(G,cout);
  test_cell_iterator(G,cout);

  test_vertex_on_cell_iterator(G,cout);
  test_edge_on_cell_iterator(G,cout);
  test_facet_on_cell_iterator(G,cout);


  Complex3D G3;
  cartesian3d::lattice_geometry GeomR(R);
  typedef cartesian3d::lattice_geometry::coord_type coord_type;
  simple_geometry<Complex3D, coord_type> GeomG3(G3);
  std::vector<gt::vertex_handle> v_corr;
  std::vector<gt::cell_handle>   c_corr;
  ConstructGridVC(G3, GeomG3,
		  R,  GeomR,
		  v_corr, c_corr);

  
}
