#include "Gral/Base/element-grid.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Test/test-iterators.h"

#include <iostream>


  typedef cartesian2d::CartesianGrid2D  grid_type_2d;
  typedef grid_types<grid_type_2d>         gt2;
  template class element_grid<gt2::Cell>; 
  template class element_grid<gt2::Edge>; 
  
  template class grid_types< element_grid<gt2::Cell> >;
  template class grid_types< element_grid<gt2::Edge> >;
  template class grid_types< element_grid<gt2::Vertex> >;



  typedef cartesian3d::CartesianGrid3D  grid_type_3d;
  typedef grid_types<grid_type_3d>         gt3;
  template class element_grid<gt3::Cell>; 
  template class element_grid<gt3::Face>; 
  template class element_grid<gt3::Edge>; 
  template class element_grid<gt3::Vertex>; 
  
  template class grid_types< element_grid<gt3::Cell> >;
  template class grid_types< element_grid<gt3::Face> >;
  template class grid_types< element_grid<gt3::Edge> >;
  template class grid_types< element_grid<gt3::Vertex> >;



int main () 
{
  using namespace std;

  // we cannot use test_vertex_iterator() etc
  // because VertexIterator(grid, handle) is not supported
  // -> must define handles as archgt::xxx_handle

  { 
    typedef grid_type_2d grid_type;
    typedef gt2          gt;
    grid_type R(3,3);
    
    element_grid<gt::Cell> c1(* R.FirstCell());
    cout  << "Dimension: " << c1.dimension()  << "  "
	  << "c1.NumOfCells()="  << c1.NumOfCells() << "  "
	  << "c1.NumOfFaces()=" << c1.NumOfFaces() << " "
	  << "c1.NumOfEdges()=" << c1.NumOfEdges() << " "
	  << "c1.NumOfVertices()=" << c1.NumOfVertices() << endl;
    
    test_vertex_iterator(c1,cout);
    test_edge_iterator(c1,cout);
    test_face_iterator(c1,cout);
    test_cell_iterator(c1,cout);
    
    //  test_vertex_on_cell_iterator(c1,cout);
    test_vertex_on_edge_iterator(c1,cout);
    
    element_grid<gt::Edge> e1(* R.FirstEdge());
    cout  << "Dimension: " << e1.dimension()  << "  "
	  << "e1.NumOfCells()="  << e1.NumOfCells() << "  "
	 << "e1.NumOfEdges()=" << e1.NumOfEdges() << " "
	 << "e1.NumOfVertices()=" << e1.NumOfVertices() << endl;
    
    test_vertex_iterator(e1,cout);
    test_edge_iterator(e1,cout);
    test_cell_iterator(e1,cout);


    element_grid<gt::Vertex> v1(* R.FirstVertex());
    cout  << "Dimension: " << v1.dimension()  << "  "
	  << "v1.NumOfCells()="  << v1.NumOfCells() << "  "
	  << "v1.NumOfVertices()=" << v1.NumOfVertices() << endl;

    test_vertex_iterator(v1,cout);

  }


 { 
    typedef grid_type_3d grid_type;
    typedef gt3          gt;
    grid_type R(3,3,3);
    
    // Cell
    element_grid<gt::Cell> c1(* R.FirstCell());
    cout  << "Dimension: " << c1.dimension()  << "  "
	  << "c1.NumOfCells()="  << c1.NumOfCells() << "  "
	  << "c1.NumOfFaces()=" << c1.NumOfFaces() << " "
	  << "c1.NumOfEdges()=" << c1.NumOfEdges() << " "
	  << "c1.NumOfVertices()=" << c1.NumOfVertices() << endl;
    
    test_vertex_iterator(c1,cout);
    test_edge_iterator(c1,cout);
    test_face_iterator(c1,cout);
    test_cell_iterator(c1,cout);
    
    //  test_vertex_on_cell_iterator(c1,cout);
    test_vertex_on_edge_iterator(c1,cout);

    // Face
    element_grid<gt::Face> f1(* R.FirstFace());
    cout  << "Dimension: " << f1.dimension()  << "  "
	  << "f1.NumOfCells()="    << f1.NumOfCells() << "  "
	  << "f1.NumOfFaces()="    << f1.NumOfFaces() << " "
	  << "f1.NumOfEdges()="    << f1.NumOfEdges() << " "
	  << "f1.NumOfVertices()=" << f1.NumOfVertices() << endl;
    
    test_vertex_iterator(f1,cout);
    test_edge_iterator(f1,cout);
    test_face_iterator(f1,cout);
    test_cell_iterator(f1,cout);
    
    // Edge
    element_grid<gt::Edge> e1(* R.FirstEdge());
    cout  << "Dimension: " << e1.dimension()  << "  "
	  << "e1.NumOfCells()="    << e1.NumOfCells() << "  "
	 << "e1.NumOfEdges()="    << e1.NumOfEdges() << " "
	 << "e1.NumOfVertices()=" << e1.NumOfVertices() << endl;
    
    test_vertex_iterator(e1,cout);
    test_edge_iterator(e1,cout);
    test_cell_iterator(e1,cout);

    // Vertex
    element_grid<gt::Vertex> v1(* R.FirstVertex());
    cout  << "Dimension: " << v1.dimension()  << "  "
	  << "v1.NumOfCells()="  << v1.NumOfCells() << "  "
	  << "v1.NumOfVertices()=" << v1.NumOfVertices() << endl;
    
    test_vertex_iterator(v1,cout);

  }
}
