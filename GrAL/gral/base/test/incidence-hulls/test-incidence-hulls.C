
// $LICENSE_NEC_2003

/*! \file 

    Test cases for the algorithms in the incidence_hull module.

 */


#include "Gral/Algorithms/incidence-hulls.h"
#include "Gral/Algorithms/incidence-stencil.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Base/partial-multi-grid-functions.h"
#include "Gral/Subranges/layered-subrange.h"
#include "Gral/Subranges/enumerated-subrange.h"


#include "Container/function-adapter.h"


#include <iostream>

int main() {
  using namespace std;
  
  incidence_stencil s("VC");

  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;

  grid_type R(4,4);
  typedef  layered_vertex_range<grid_type>  vertex_range;
  typedef  layered_cell_range<grid_type>    cell_range;
  typedef  vertex_range::range_type_ref     vrt;
  typedef  cell_range::range_type_ref       crt;
  typedef  vrt::VertexIterator              rgeVertexIterator;
  typedef  crt::CellIterator                rgeCellIterator;
  

  vertex_range v_layers(R);
  cell_range   c_layers(R);

  partial_multi_grid_function<grid_type, int> visited(R,0);

  gt::Cell seed_c = * R.FirstCell();
  visited[seed_c] = 1;
  c_layers.append_layer();
  c_layers.append(seed_c);

  int level = 1;
  mark_on_cells<gt>(c_layers.LastLayer().FirstCell(),
		    v_layers, c_layers,
		    visited,
		    s,
		    level,
		    constant<gt::Cell, bool>(true));

  cout << "Seed cell: " << seed_c.index() << endl;
  for(unsigned i = 1; i <= c_layers.NumOfLayers(); ++i) {
    cout << "Cell layer " << i  << ": ";
    for(rgeCellIterator c(c_layers.Layer(i).FirstCell()); ! c.IsDone(); ++c)
      cout << "[" << (*c).index() << "]" << " ";
    cout << endl;
  }
  for(unsigned i = 1; i <= v_layers.NumOfLayers(); ++i) {
    cout << "Vertex layer " << i  << ": ";
    for(rgeVertexIterator v(v_layers.Layer(i).FirstVertex()); ! v.IsDone(); ++v)
      cout << "[" << (*v).index() << "]" << " ";
    cout << endl;
  }
	

	
  enumerated_subrange<grid_type> seed_range(R);
  seed_range.append_cell(seed_c);
  incidence_stencil s2("CVCVC");
  typedef incidence_hull<enumerated_subrange<grid_type>, incidence_stencil, gt> hull_type;
  hull_type Hull(seed_range, s2);
  hull_type Hull2(seed_range, s2, hull_type::periodic);

  for(unsigned i = 1; i <= Hull.cells().NumOfLayers(); ++i) {
    cout << "Cell layer " << i  << ": ";
    for(rgeCellIterator c(Hull.cells(i)->FirstCell()); ! c.IsDone(); ++c) {
      cout << "[" << (*c).index() << "]" << " ";
      // REQUIRE_ALWAYS( Hull.level(*c) == i, "Hull.level(*c)=" << Hull.level(*c) << " i=" << i, 1);
    }
    cout << endl;
  }
  for(unsigned i = 1; i <= Hull.vertices().NumOfLayers(); ++i) {
    cout << "Vertex layer " << i  << ": ";
    for(rgeVertexIterator v(Hull.vertices(i)->FirstVertex()); ! v.IsDone(); ++v) {
      cout << "[" << (*v).index() << "]" << " ";
      //REQUIRE_ALWAYS( Hull.level(*v) == i, "Hull.level(*v)=" << Hull.level(*v) << " i=" << i, 1);
    }
    cout << endl;
  }
 
 for(unsigned i = 1; i <= Hull2.cells().NumOfLayers(); ++i) {
    cout << "Cell layer " << i  << ": ";
    for(rgeCellIterator c(Hull2.cells(i)->FirstCell()); ! c.IsDone(); ++c)
      cout << "[" << (*c).index() << "]" << " ";
    cout << endl;
  }
  for(unsigned i = 1; i <= Hull2.vertices().NumOfLayers(); ++i) {
    cout << "Vertex layer " << i  << ": ";
    for(rgeVertexIterator v(Hull2.vertices(i)->FirstVertex()); ! v.IsDone(); ++v)
      cout << "[" << (*v).index() << "]" << " ";
    cout << endl;
  }


}
