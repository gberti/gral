
// $LICENSE_NEC

#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/cartesian3d.h"

#include "Gral/Test/test-iterators.h"
#include "Gral/Test/test-grid-functions.h"

#include "Utility/pre-post-conditions.h"
#include "IO/control-device.h"



int main(int argc, char* argv[]) {
  
  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"map.in","main");

  int nx = 3, ny = 3, nz = 3; 
  RegisterAt(Ctrl, "-nx", nx);
  RegisterAt(Ctrl, "-ny", ny);
  RegisterAt(Ctrl, "-nz", nz);

  Ctrl.update();

  typedef grid_types<Complex3D> gt;

  CartesianGrid3D R(nx,ny,nz);
  Complex3D       G;
  ConstructGrid0(G,R);


  test_grid_functions<gt::Vertex>(G,cout);
  test_grid_functions<gt::Edge  >(G,cout);
  test_grid_functions<gt::Facet >(G,cout);
  test_grid_functions<gt::Cell  >(G,cout);


  test_vertex_iterator(G, cout);
  test_edge_iterator  (G, cout);
  test_facet_iterator (G, cout);
  test_cell_iterator  (G, cout);
  
  test_vertex_on_cell_iterator(G,cout);
  test_edge_on_cell_iterator  (G,cout);
  test_facet_on_cell_iterator (G,cout);


}


