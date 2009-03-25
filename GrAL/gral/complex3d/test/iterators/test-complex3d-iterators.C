

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



/*! \file
 */


#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/cartesian3d.h"

#include "Gral/Test/all.h"

#include "Utility/pre-post-conditions.h"
#include "IO/control-device.h"

#include <iostream>


int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;
  using GrAL::cartesian3d::CartesianGrid3D;

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

  test_partial_grid_functions<gt::Vertex>(G,cout);
  test_partial_grid_functions<gt::Edge  >(G,cout);
  test_partial_grid_functions<gt::Facet >(G,cout);
  test_partial_grid_functions<gt::Cell  >(G,cout);


  test_vertex_iterator(G, cout);
  test_edge_iterator  (G, cout);
  test_facet_iterator (G, cout);
  test_cell_iterator  (G, cout);
  
  test_vertex_on_cell_iterator(G,cout);
  test_edge_on_cell_iterator  (G,cout);
  test_facet_on_cell_iterator (G,cout);

  test_sequence_iterator<gt::Vertex>(G, cout);
  test_sequence_iterator<gt::Edge>  (G, cout);
  test_sequence_iterator<gt::Facet> (G, cout);
  test_sequence_iterator<gt::Cell>  (G, cout);
  

  test_incidence_iterator<gt::Vertex, gt::Edge> (G, cout); 

  test_incidence_iterator<gt::Edge,   gt::Facet>(G, cout);
  test_incidence_iterator<gt::Vertex, gt::Facet>(G, cout);

  test_incidence_iterator<gt::Vertex, gt::Cell>(G, cout);
  test_incidence_iterator<gt::Edge,   gt::Cell>(G, cout);
  test_incidence_iterator<gt::Facet,  gt::Cell>(G, cout);


  test_switch_vertex(G);
  test_switch_edge  (G);
  test_switch_facet (G);

}



