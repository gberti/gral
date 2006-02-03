
// $LICENSE_NEC

/*! \file
 
*/

#include "Gral/Base/extend-grid-types.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/Grids/Complex3D/complex3d.h"

#include <iostream>

struct test_gt {
  typedef int grid_type;
  typedef int Vertex;

  typedef int Cell;
};

int main() {
  using namespace GrAL;
  using namespace std; 
  typedef grid_types<cartesian2d::CartesianGrid2D> origgt;
  cout << "grid_types<CartesianGrid2D>:\n";
  checkgt<grid_types<cartesian2d::CartesianGrid2D> >(cout);

  cout << "\n";

  cout << "grid_types<CartesianGrid3D>:\n";
  checkgt<grid_types<cartesian3d::CartesianGrid3D> >(cout);

  cout << "\n";

  cout << "grid_types<Complex2D>:\n";
  checkgt<grid_types<Complex2D> >(cout);

  cout << "\n";

  cout << "grid_types<Complex3D>:\n";
  checkgt<grid_types<Complex3D> >(cout);


  //  cout << "has " << ( has_Vertex<origgt>::result ? "" : " no ") << " type Vertex\n";
 
  typedef int my_vertex_handle; 
  typedef int my_facet_handle;
  typedef int my_cell_handle;
  
  typedef xgt<origgt, override_facet_handle<my_facet_handle> > newgt;

  // nesting xgt
  typedef xgt<xgt<
              origgt,
              override_facet_handle<my_facet_handle> >,
              override_cell_handle <my_cell_handle>  >   newgt2;

  typedef xgt<xgt<xgt<
              origgt,
              override_facet_handle <my_facet_handle > >,
              override_cell_handle  <my_cell_handle  > >, 
              override_vertex_handle<my_vertex_handle> >  newgt3;
                  
  

  cout << "\n";

  cout << "xgt<origgt, override_facet_handle<my_facet_handle> >:\n";
  checkgt<newgt>(cout);

  cout << "\n";

  cout << "override_facet_handle<my_facet_handle>:\n";
  checkgt<override_facet_handle<my_facet_handle> >(cout);

  cout << "checkgt<test_gt>\n";
  checkgt<test_gt>(cout);
  cout << "\n";


}
