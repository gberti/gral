#include "Gral/Algorithms/construct-isomorphism.h"
#include "Gral/Base/partial-grid-morphism.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Complex2D/all.h"

#include "Gral/IO/stream-grid-adapter.h"

template<class PHI>
void print_morphism(PHI const& phi, std::ostream& out)
{
  typedef grid_types<typename PHI::grid_type_def> gt;
  typename PHI::grid_type_def const& G(phi.DefGrid());

  out << "Vertices:\n";
  for(typename gt::VertexIterator v(G); ! v.IsDone(); ++v) {
    out << v.handle() <<  " -> " << phi(*v).handle() << '\n';
  }
  out << "Edges:\n";
  for(typename gt::EdgeIterator e(G); ! e.IsDone(); ++e) {
    out << e.handle() <<  " -> " << phi(*e).handle() 
	      << " ( [" << (*e).v1() << "," << (*e).v2() 
	      << "] -> [" << phi(*e).v1() << "," << phi(*e).v2() << "])" << '\n';
  }
  out << "Cells:\n";
  for(typename gt::CellIterator c(G); ! c.IsDone(); ++c) {
    out << c.handle() <<  " -> " << phi(*c).handle() << '\n';
  }
}

template<class GRID, class PHI>
void check_identity(GRID const& G, PHI const& phi)
{
  typedef grid_types<GRID> gt;
  std::cout << "Vertices:\n";
  for(typename gt::VertexIterator v(G); ! v.IsDone(); ++v) {
    std::cout << v.handle() <<  " -> " << phi(*v).handle() << '\n';
    REQUIRE_ALWAYS( phi(*v) == *v, "wrong isomorphism: Is not identity!\n",1);
  }
  std::cout << "Edges:\n";
  for(typename gt::EdgeIterator e(G); ! e.IsDone(); ++e) {
    std::cout << e.handle() <<  " -> " << phi(*e).handle() 
	      << " ( [" << (*e).v1() << "," << (*e).v2() 
	      << "] -> [" << phi(*e).v1() << "," << phi(*e).v2() << "])" << '\n';
    REQUIRE_ALWAYS( phi(*e) == *e, "wrong isomorphism: Is not identity!\n",1);
  }
  std::cout << "Cells:\n";
  for(typename gt::CellIterator c(G); ! c.IsDone(); ++c) {
    std::cout << c.handle() <<  " -> " << phi(*c).handle() << '\n';
    REQUIRE_ALWAYS( phi(*c) == *c, "wrong isomorphism: Is not identity!\n",1);
  }
}

int main() {
  using namespace cartesian2d;
  using cartesian3d::CartesianGrid3D;

  typedef grid_types<CartesianGrid2D> rgt2d;

  CartesianGrid2D R1(3,3);
  // CartesianGrid2D R2(3,2);

  partial_grid_morphism<CartesianGrid2D,CartesianGrid2D> phi(R1,R1);
  bool success = construct_isomorphism(R1,R1,phi);
  std::cout << "Isomorphism does " << (success ? "" : "not ") << "exist.\n";
  std::cout << "Vertices:\n";
  for(rgt2d::VertexIterator v(R1); ! v.IsDone(); ++v) {
    std::cout << v.handle() <<  " -> " << phi(*v).handle() << '\n';
    REQUIRE_ALWAYS( phi(*v) == *v, "wrong isomorphism: Is not identity!\n",1);
  }
  std::cout << "Edges:\n";
  for(rgt2d::EdgeIterator e(R1); ! e.IsDone(); ++e) {
    std::cout << e.handle() <<  " -> " << phi(*e).handle() 
	      << " ( [" << (*e).v1() << "," << (*e).v2() 
	      << "] -> [" << phi(*e).v1() << "," << phi(*e).v2() << "])" << '\n';
    REQUIRE_ALWAYS( phi(*e) == *e, "wrong isomorphism: Is not identity!\n",1);
  }
  std::cout << "Cells:\n";
  for(rgt2d::CellIterator c(R1); ! c.IsDone(); ++c) {
    std::cout << c.handle() <<  " -> " << phi(*c).handle() << '\n';
    REQUIRE_ALWAYS( phi(*c) == *c, "wrong isomorphism: Is not identity!\n",1);
  }


  CartesianGrid3D R3(2,2,2);
  typedef grid_types<CartesianGrid3D> rgt3d;
  rgt3d::archetype_type A(*(R3.BeginArchetype()));
  partial_grid_morphism<rgt3d::archetype_type,rgt3d::archetype_type>
    phi_a(A,A);
  success = construct_isomorphism(A,A,phi_a);

  std::cout << "\n"
	    << "Isomorphism A -> A does " << (success ? "" : "not ") << "exist.\n";
  if(success) 
    check_identity(A,phi_a);


  Complex2D A2;
     int conn_hex[5*6] = {
   4,  0, 3, 2, 1,
   4,  0, 1, 5, 4,
   4,  0, 4, 7, 3,
   4,  1, 2, 6, 5,
   4,  2, 3, 7, 6,
   4,  4, 5, 6, 7
 };
 stream_grid_mask<int *> hex(8,6,conn_hex); 
 ConstructGrid0(A2, hex);
 partial_grid_morphism<Complex2D, rgt3d::archetype_type>
   phi_a2(A2,A);
 success = construct_isomorphism(A2,A,phi_a2);

  std::cout << "\n"
	    << "Isomorphism A2 -> A does " << (success ? "" : "not ") << "exist.\n";
  if(success) 
    print_morphism(phi_a2, std::cout);

  /* archetype of Cartesian3D Cell:
   int conn[5*6] = { 
    4,  0, 1, 3, 2,
    4,  0, 2, 6, 4,
    4,  0, 4, 5, 1,
    4,  1, 5, 7, 3,
    4,  2, 3, 7, 6,
    4,  4, 6, 7, 5
   };
  */
}




