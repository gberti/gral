
#include "Gral/Grids/ComplexND/complexnd.h"
#include "Gral/Grids/ComplexND/construct.C"

#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Base/grid-morphism.h"

#include "Gral/Test/all.h"

#include <iostream>

// #include "Gral/Grids/Cartesian2D/all.h"

using namespace std;

template<int D>
void print_incidences(complexnd::ComplexND<D> const& g, std::ostream& out)
{
  for(unsigned d1 = 0; d1 <= g.dimension(); ++d1) {
    out << "Anchor dimension: " << d1 << endl;
    for(unsigned d2 = 0; d2 <= g.dimension(); ++d2) {
      out << "  Value dimension: " << d2 << endl;
      for(unsigned e1 = 0; e1 < g.NumOfElements(d1); ++e1) {
	if(g.incidences[d1][e1].size() > 0 && g.incidences[d1][e1][d2].size() > 0 ) {
	  out << "    " << e1 << ": " << g.incidences[d1][e1][d2].size() << " incidences of dim " << d2 << ": ";
	  for(unsigned e2 = 0; e2 < g.incidences[d1][e1][d2].size(); ++e2) {
	    out << g.incidences[d1][e1][d2][e2] << " ";
	  }
	  out << endl;
	}
	// out << endl;
      }
      out << "  --------------------------" << endl;
    }
    out << "=================================" << endl;
  }
}

namespace complexnd {
  template class ComplexND<complexnd::ANY>;
  template class ComplexND<0>;
  template class ComplexND<1>;
  template class ComplexND<2>;

  typedef ComplexND<complexnd::ANY> ComplexAnyD;
  typedef ComplexND<0>              Complex0D;
  typedef ComplexND<1>              Complex1D;
  typedef ComplexND<2>              Complex2D;

  template class element_t<ComplexAnyD, complexnd::ANY,complexnd::ANY>;

  // Any: Fixed dim or codim
  template class element_t<ComplexAnyD, complexnd::ANY, 0>;  // Cell
  template class element_t<ComplexAnyD, complexnd::ANY, 1>;  // Facet
  template class element_t<ComplexAnyD, 0,  complexnd::ANY>; // Vertex
  template class element_t<ComplexAnyD, 1,  complexnd::ANY>; // Edge
  template class element_t<ComplexAnyD, 2,  complexnd::ANY>; // Face
  template class element_iterator_t<ComplexAnyD, complexnd::ANY, 0>;  // Cell
  template class element_iterator_t<ComplexAnyD, complexnd::ANY, 1>;  // Facet
  template class element_iterator_t<ComplexAnyD, 0,  complexnd::ANY>; // Vertex
  template class element_iterator_t<ComplexAnyD, 1,  complexnd::ANY>; // Edge
  template class element_iterator_t<ComplexAnyD, 2,  complexnd::ANY>; // Face


  // Fixed Dim: Elements for 0D grid
  template class element_t<Complex0D,   0,  0>;
  template class element_iterator_t<Complex0D,   0,  0>;
  typedef  element_t<Complex0D,   0,  0> Vertex0D;

  // Fixed Dim: Elements for 1D grid
  template class element_t<Complex1D,   0,  1>;
  template class element_t<Complex1D,   1,  0>;
  template class element_iterator_t<Complex1D,   0,  1>;
  template class element_iterator_t<Complex1D,   1,  0>;
  typedef element_t<Complex1D,   0,  1> Vertex1D;

  // Fixed dim: Elements for 2D grid
  template class element_t<Complex2D,   0,  2>; // Vertex
  template class element_t<Complex2D,   1,  1>; // Edge = Facet
  template class element_t<Complex2D,   2,  0>; // Face = Cell
  template class element_iterator_t<Complex2D,   0,  2>; // Vertex
  template class element_iterator_t<Complex2D,   1,  1>; // Edge = Facet
  template class element_iterator_t<Complex2D,   2,  0>; // Face = Cell
  typedef element_t<Complex2D,   0,  2>  Vertex2D;

  // Fixed dim: incidence iterators for 1D grid
  template class incidence_iterator_t<Complex1D, 0, 0, 1, 1>; // VertexOnVertex
  template class incidence_iterator_t<Complex1D, 0, 1, 0, 1>; // VertexOnCell
  template class incidence_iterator_t<Complex1D, 1, 0, 1, 0>; // CellOnVertex
  template class incidence_iterator_t<Complex1D, 1, 1, 0, 0>; // CellOnCell

  // Fixed dim: incidence iterators for 2D grid
  template class incidence_iterator_t<Complex2D, 0, 0, 2, 2>; // VertexOnVertex
  template class incidence_iterator_t<Complex2D, 0, 1, 2, 1>; // VertexOnEdge
  template class incidence_iterator_t<Complex2D, 0, 2, 2, 0>; // VertexOnCell
  template class incidence_iterator_t<Complex2D, 1, 0, 1, 2>; // EdgeOnVertex
  template class incidence_iterator_t<Complex2D, 1, 1, 1, 1>; // EdgeOnEdge
  template class incidence_iterator_t<Complex2D, 1, 2, 1, 0>; // EdgeOnCell
  template class incidence_iterator_t<Complex2D, 2, 0, 0, 2>; // CellOnVertex
  template class incidence_iterator_t<Complex2D, 2, 1, 0, 1>; // CellOnEdge
  template class incidence_iterator_t<Complex2D, 2, 2, 0, 0>; // CellOnCell


}

template class grid_function<complexnd::Vertex0D, int>;
template class grid_function<complexnd::Vertex1D, int>;
template class grid_function<complexnd::Vertex2D, int>;

int main() {
  using namespace complexnd;
  using namespace std;

  typedef cartesian2d::CartesianGrid2D  src_grid_type;
  typedef ComplexND<2>                  dest_grid_type;
  dest_grid_type C;
  src_grid_type  R(3,3);


  vertex_morphism<src_grid_type, dest_grid_type>  Phi(R, C);
  ConstructGrid0(C, R, Phi);

  cout << "ComplexND<2> incidences:" << endl;
  print_incidences(C, cout);

  test_vertex_iterator        (C, std::cout);
  test_cell_iterator          (C, std::cout);
  test_vertex_on_cell_iterator(C, std::cout);

  ComplexND<ANY> C_any(2);
  vertex_morphism<ComplexND<2>, ComplexND<ANY> > Phi_any(C,C_any);
  ConstructGrid0(C_any, C, Phi_any);
 
   cout << "ComplexND<ANY> incidences:" << endl;
   print_incidences(C_any, cout);

  test_vertex_iterator        (C_any, std::cout);
  test_cell_iterator          (C_any, std::cout);
  test_vertex_on_cell_iterator(C_any, std::cout);



  typedef grid_types<ComplexND<0> > gt0;
  
  ComplexND<0> pts1(pointcloud(2));
  test_vertex_iterator(pts1, std::cout);

  grid_function<gt0::Vertex, int> gf0(pts1);
  for(gt0::VertexIterator v(pts1); !v.IsDone(); ++v)
    gf0[*v] = v.handle();

  ComplexND<0> pts2;
  vertex_morphism<ComplexND<0>, ComplexND<0> > Phi0(pts1,pts2);
  ConstructGrid0(pts2, pts1, Phi0);
  
  cout << "pts1.NumOfVertices(): " << pts1.NumOfVertices() << endl
       << "pts2.NumOfVertices(): " << pts2.NumOfVertices() << endl;

  ComplexND<1> polygon1(complexnd::polygon(5));
  ComplexND<1> polygon2;

  vertex_morphism<ComplexND<1>, ComplexND<1> > Phi1(polygon1, polygon2);
  ConstructGrid0(polygon2, polygon1, Phi1);  
  cout << "polygon1.NumOfVertices(): " << polygon1.NumOfVertices() << endl
       << "polygon2.NumOfVertices(): " << polygon2.NumOfVertices() << endl;
  print_incidences(polygon1, cout);
  print_incidences(polygon2, cout);


  test_vertex_iterator        (polygon1, std::cout);
  test_cell_iterator          (polygon1, std::cout);
  test_vertex_on_cell_iterator(polygon1, std::cout);
  test_edge_on_cell_iterator  (polygon1, std::cout);

  test_vertex_iterator        (polygon2, std::cout);
  test_cell_iterator          (polygon2, std::cout);
  test_vertex_on_cell_iterator(polygon2, std::cout);
  //  test_edge_on_cell_iterator  (polygon2, std::cout);


  // test_cell_on_vertex_iterator(polygon, std::cout);
}
