#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Test/all.h"

#include <iostream>

int main() {
  using std::cout;  

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

  
}
