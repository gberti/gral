
#include "Gral/Grids/ComplexND/complexnd.h"
#include "Gral/Grids/ComplexND/construct.C"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Complex2D/all.h"


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
  typedef ComplexND<3>              Complex3D;

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
  typedef element_t<Complex1D,   1,  0> Edge1D;
  typedef element_t<Complex1D,   1,  0> Cell1D;

  // Fixed dim: Elements for 2D grid
  template class element_t<Complex2D,   0,  2>; // Vertex
  template class element_t<Complex2D,   1,  1>; // Edge = Facet
  template class element_t<Complex2D,   2,  0>; // Face = Cell
  template class element_iterator_t<Complex2D,   0,  2>; // Vertex
  template class element_iterator_t<Complex2D,   1,  1>; // Edge = Facet
  template class element_iterator_t<Complex2D,   2,  0>; // Face = Cell
  typedef element_t<Complex2D,   0,  2>  Vertex2D;
  typedef element_t<Complex2D,   1,  1>  Edge2D;
  typedef element_t<Complex2D,   2,  0>  Cell2D;

  // Fixed dim: Elements for 3D grid
  template class element_t<Complex3D,   0,  3>; // Vertex
  template class element_t<Complex3D,   1,  2>; // Edge 
  template class element_t<Complex3D,   2,  1>; // Face = Facet
  template class element_t<Complex3D,   3,  0>; // Cell
  template class element_iterator_t<Complex3D,   0,  3>; // Vertex
  template class element_iterator_t<Complex3D,   1,  2>; // Edge
  template class element_iterator_t<Complex3D,   2,  1>; // Face = Facet
  template class element_iterator_t<Complex3D,   3,  0>; // Cell
  typedef element_t<Complex2D,   0,  3>  Vertex3D;
  typedef element_t<Complex2D,   1,  2>  Edge3D;
  typedef element_t<Complex2D,   2,  1>  Face3D;
  typedef element_t<Complex2D,   3,  0>  Cell3D;



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
template class grid_function<complexnd::Edge1D, int>;

template class grid_function<complexnd::Vertex2D, int>;
template class grid_function<complexnd::Edge2D, int>;
template class grid_function<complexnd::Cell2D, int>;

template class grid_function<complexnd::Vertex3D, int>;
template class grid_function<complexnd::Edge3D, int>;
template class grid_function<complexnd::Face3D, int>;
template class grid_function<complexnd::Cell3D, int>;




int main() {
  using namespace complexnd;
  using namespace std;


   
  ComplexND<3> C3;
  cartesian3d::CartesianGrid3D  R3(2,2,2);
  vertex_morphism<cartesian3d::CartesianGrid3D, ComplexND<3> >  Phi3D(R3, C3);
   ConstructGrid0(C3, R3, Phi3D);
  cout << "ComplexND<3> incidences:" << endl;
  print_incidences(C3, cout);
 


  ComplexND<2> C;
  cartesian2d::CartesianGrid2D  R(3,3);


  vertex_morphism<cartesian2d::CartesianGrid2D, ComplexND<2> >  Phi2D(R, C);
  ConstructGrid0(C, R, Phi2D);

  cout << "ComplexND<2> incidences:" << endl;
  print_incidences(C, cout);

  test_vertex_iterator        (C, std::cout);
  test_cell_iterator          (C, std::cout);
  test_vertex_on_cell_iterator(C, std::cout);

  unsigned tri[3][2] = { {0, 1}, {1,2}, {2,0}};
  ComplexND<1> triangle(tri, 3);
  cout << "ComplexND<1> triangle incidences:" << endl;
  print_incidences(triangle, cout);

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
  
  ComplexND<1> polygon1(complexnd::polygon(5));
  ComplexND<1> polygon2;

  vertex_morphism<ComplexND<1>, ComplexND<1> > Phi1(polygon1, polygon2);
  ConstructGrid0(polygon2, polygon1, Phi1);  

  cout << "Incidences of polygon1:" << endl;
  print_incidences(polygon1, cout);

  cout << "Incidences of polygon2:" << endl;
  print_incidences(polygon2, cout);


  test_vertex_iterator        (polygon1, std::cout);
  test_cell_iterator          (polygon1, std::cout);
  test_vertex_on_cell_iterator(polygon1, std::cout);
  test_edge_on_cell_iterator  (polygon1, std::cout);

  // test function not implemented
  // test_cell_on_vertex_iterator(polygon1, std::cout);
  test_vertex_iterator        (polygon2, std::cout);
  test_cell_iterator          (polygon2, std::cout);
  test_vertex_on_cell_iterator(polygon2, std::cout);
  //  This is not yet defined!
  //  test_edge_on_cell_iterator  (polygon2, std::cout);



}
