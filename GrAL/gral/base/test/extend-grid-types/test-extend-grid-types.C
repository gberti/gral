#include "Gral/Base/extend-grid-types.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/Grids/Complex3D/complex3d.h"

#include <iostream>


template<class GT>
void checkgt(std::ostream& out)
{
  typedef GT gt;

#define PRINT_HAS_MEMBER(T)  out << "has" << ( has_##T<gt>::result ? "   " : " no") << " type " << #T << "\n";

  PRINT_HAS_MEMBER(vertex_handle);
  PRINT_HAS_MEMBER(edge_handle);
  PRINT_HAS_MEMBER(face_handle);
  PRINT_HAS_MEMBER(facet_handle);
  PRINT_HAS_MEMBER(cell_handle);

  PRINT_HAS_MEMBER(Vertex);
  PRINT_HAS_MEMBER(Edge);
  PRINT_HAS_MEMBER(Face);
  PRINT_HAS_MEMBER(Facet);
  PRINT_HAS_MEMBER(Cell);

  PRINT_HAS_MEMBER(VertexIterator);
  PRINT_HAS_MEMBER(EdgeIterator);
  PRINT_HAS_MEMBER(FaceIterator);
  PRINT_HAS_MEMBER(FacetIterator);
  PRINT_HAS_MEMBER(CellIterator);

  PRINT_HAS_MEMBER(VertexOnVertexIterator);
  PRINT_HAS_MEMBER(EdgeOnVertexIterator);
  PRINT_HAS_MEMBER(FaceOnVertexIterator);
  PRINT_HAS_MEMBER(FacetOnVertexIterator);
  PRINT_HAS_MEMBER(CellOnVertexIterator);

  PRINT_HAS_MEMBER(VertexOnEdgeIterator);
  PRINT_HAS_MEMBER(FaceOnEdgeIterator);
  PRINT_HAS_MEMBER(FacetOnEdgeIterator);
  PRINT_HAS_MEMBER(CellOnEdgeIterator);

  PRINT_HAS_MEMBER(VertexOnFaceIterator);
  PRINT_HAS_MEMBER(EdgeOnFaceIterator);
  PRINT_HAS_MEMBER(CellOnFaceIterator);

  PRINT_HAS_MEMBER(VertexOnFacetIterator);
  PRINT_HAS_MEMBER(EdgeOnFacetIterator);
  PRINT_HAS_MEMBER(CellOnFacetIterator);

  PRINT_HAS_MEMBER(VertexOnCellIterator);
  PRINT_HAS_MEMBER(EdgeOnCellIterator);
  PRINT_HAS_MEMBER(FaceOnCellIterator);
  PRINT_HAS_MEMBER(FacetOnCellIterator);
  PRINT_HAS_MEMBER(CellOnCellIterator);

#undef PRINT_HAS_MEMBER
}

int main() {
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

}
