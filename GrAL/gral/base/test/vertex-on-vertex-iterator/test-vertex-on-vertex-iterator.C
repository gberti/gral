
// $LICENSE_NEC

/*! \file 

   Test cases for vertex_on_vertex_iterator<>

*/

#include "Gral/Iterators/vertex-on-vertex-iterator.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;

  {
    using namespace cartesian2d;
    
    CartesianGrid2D R(3,3);
    typedef grid_types<CartesianGrid2D> gt;
    
    typedef vertex_on_vertex_iterator<CartesianGrid2D> v_o_v_iterator;
    
    v_o_v_iterator::init(R);
    
    for(gt::VertexIterator v(R); ! v.IsDone(); ++v) {
      cout << "v=" << v.handle() << "  ";
      cout << "[";
      for(gt::VertexOnVertexIterator vv(*v); ! vv.IsDone(); ++vv)
	cout << (*vv).handle() << " ";
      cout << "]"
	   << "  "
	   << "[";
      for(v_o_v_iterator vv(*v); ! vv.IsDone(); ++vv)
	cout << (*vv).handle() << " ";
      cout << "]";
      cout << endl;
    }
  }

  {
    using namespace cartesian3d;
    
    CartesianGrid3D R(3,3,3);
    typedef grid_types<CartesianGrid3D> gt;
    
    typedef vertex_on_vertex_iterator<CartesianGrid3D> v_o_v_iterator;
    for(gt::VertexIterator v(R); ! v.IsDone(); ++v) {
      cout << "v=" << v.handle() << "  ";
      cout << "[";
      for(v_o_v_iterator vv(*v); ! vv.IsDone(); ++vv)
	cout << (*vv).handle() << " ";
      cout << "]";
      cout << endl;
    }
   }
}
