#include "Gral/Algorithms/construct-isomorphism.h"
#include "Gral/Base/partial-grid-morphism.h"

#include "Gral/Grids/Cartesian2D/all.h"


int main() {
  typedef RegGrid2D CartesianGrid2D;

  typedef grid_types<CartesianGrid2D> gt;

  CartesianGrid2D R1(3,3);
  // CartesianGrid2D R2(3,2);

  partial_grid_morphism<CartesianGrid2D,CartesianGrid2D> phi(R1,R1);
  bool success = construct_isomorphism(R1,R1,phi);
  std::cout << "Isomorphism does " << (success ? "" : "not ") << "exist.\n";
  std::cout << "Vertices:\n";
  for(gt::VertexIterator v(R1); ! v.IsDone(); ++v) {
    std::cout << v.handle() <<  " -> " << phi(*v).handle() << '\n';
    REQUIRE_ALWAYS( phi(*v) == *v, "wrong isomorphism: Is not identity!\n",1);
  }
  std::cout << "Edges:\n";
  for(gt::EdgeIterator e(R1); ! e.IsDone(); ++e) {
    std::cout << e.handle() <<  " -> " << phi(*e).handle() 
	      << " ( [" << (*e).v1() << "," << (*e).v2() 
	      << "] -> [" << phi(*e).v1() << "," << phi(*e).v2() << "])" << '\n';
    REQUIRE_ALWAYS( phi(*e) == *e, "wrong isomorphism: Is not identity!\n",1);
  }
  std::cout << "Cells:\n";
  for(gt::CellIterator c(R1); ! c.IsDone(); ++c) {
    std::cout << c.handle() <<  " -> " << phi(*c).handle() << '\n';
    REQUIRE_ALWAYS( phi(*c) == *c, "wrong isomorphism: Is not identity!\n",1);
  }


}
