
// $LICENSE_NEC

/*! \file
 
*/

#include "Gral/Algorithms/calculate-vertex-stars.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Base/flag.h"

#include <iostream>

int main() {
  using namespace GrAL;
  using std::cout;
  using namespace cartesian2d;

  typedef CartesianGrid2D       grid_type;
  typedef grid_types<grid_type> gt;

  grid_type   R(3,3);
  typedef flag<grid_type> flag_type;
  grid_function<gt::Vertex, flag_type> stars(R);

  calculate_vertex_stars(R,stars);

  for(gt::VertexIterator v(R); ! v.IsDone(); ++v) {
    cout << "Vertex " << v.handle() 
	 << " star germ: " 
	 << " edge: " << stars(*v).edge().handle()
	 << " cell: " << stars(*v).cell().handle()
	 << "\n";
  }
}

  
