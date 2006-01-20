
// $LICENSE_NEC

/*! \file
 
*/

#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Subranges/enumerated-subrange-ref.h"


#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Test/all.h"

#include <iostream>


namespace GrAL {
  //  template class enumerated_subrange<cartesian2d::CartesianGrid2D>;
}

int main() {
  using namespace GrAL;
  using namespace std;
  using namespace cartesian2d;

  typedef grid_types<CartesianGrid2D> gt;
  CartesianGrid2D R(3,3);

  typedef CartesianGrid2D grid_type;  
  typedef enumerated_subrange<CartesianGrid2D> range_type;
  typedef grid_types<range_type >              rgegt;

  cout << "Constructing subrange from 3x3 grid ... " << flush;
  range_type S(R, R);
  cout << " done." << endl;

  checkgt<rgegt>(cout);
  cout << endl;

  test_sequence_iterator<rgegt::Vertex>(S, cout);
  test_sequence_iterator<rgegt::Edge>  (S, cout);
  test_sequence_iterator<rgegt::Cell>  (S, cout);

  test_incidence_iterator<rgegt::Vertex,rgegt::Edge>(S, cout);
  test_incidence_iterator<rgegt::Vertex,rgegt::Cell>(S, cout);
  test_incidence_iterator<rgegt::Edge,  rgegt::Cell>(S, cout);


  {   
    // deducing the type of iteration from the lhs
    rgegt::VertexIterator vb = GrAL::begin(S);
    rgegt::VertexIterator ve = GrAL::end  (S);
    rgegt::CellIterator   cb = GrAL::begin(S);
    rgegt::CellIterator   ce = GrAL::end  (S);
    rgegt::VertexOnCellIterator vcb = GrAL::begin(*cb);
    rgegt::VertexOnCellIterator vce = GrAL::end  (*cb);
    rgegt::EdgeOnCellIterator   ecb = GrAL::begin(*cb);
    rgegt::EdgeOnCellIterator   ece = GrAL::end  (*cb);
    int nv  = GrAL::size<0>(S);
    // int ne = GrAL::size<1>(S);
    int nc  = GrAL::size<2>(S);
    int nvc = GrAL::size<0>(*cb);
    int nec = GrAL::size<1>(*cb);
  }


  test_archetypes(S, cout);

  {
    // test construction of ranges
    cout << "Subrange S: " 
	 << S.NumOfCells()    << " cells, "  << flush
         << GrAL::size<rgegt::Edge> (S)   << " edges, "  << flush
	 << GrAL::size<rgegt::Facet>(S)   << " facets, " << flush
	 << S.NumOfVertices() << " vertices" << endl;
    
    cout << "Cells: " << flush;
    for(rgegt::CellIterator c(S); ! c.IsDone(); ++c) {
      gt::Cell C(*c);
      cout << "C " << C.handle() << " = [" << C.ll() << "]  " << flush;
    }
    cout << endl;
    
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S); ! v.IsDone(); ++v) {
      gt::Vertex V(*v);
      cout << "V" << V.handle() << " = [" << V.x() << " " << V.y() << "]  " << flush;
    }
    cout << endl;
  }
  cout << endl;
  {
    // test piecemeal construction of ranges
    range_type S2(R);
    for(gt::CellIterator c(R); !c.IsDone(); ++c)
      S2.append_cell(c.handle());

    cout << "Piecemeal constructed subrange: " 
	 << S2.NumOfCells()    << " cells, "  << flush
	 << S2.NumOfVertices() << " vertices" << endl;
    
    cout << "Cells: " << flush;
    for(rgegt::CellIterator c(S2.FirstCell()); ! c.IsDone(); ++c) {
      gt::Cell C(*c);
      cout << "C" << C.handle() << " = [" << C.ll() << "]  " << flush;
    }
    cout << endl;
    
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S2.FirstVertex()); ! v.IsDone(); ++v) {
      gt::Vertex V(*v);
      cout << "V" << V.handle() << " = [" << V.x() << " " << V.y() << "]  " << flush;
    }
    cout << endl;
  }
  cout << endl;
  {
    // test copying of ranges
    range_type S2 = S;
    cout << "Copied subrange: " 
	 << S2.NumOfCells()    << " cells, "  << flush
	 << S2.NumOfVertices() << " vertices" << endl;
    
    cout << "Cells: " << flush;
    for(rgegt::CellIterator c(S2.FirstCell()); ! c.IsDone(); ++c) {
      gt::Cell C(*c);
      cout << "C" << C.handle() << " = [" << C.ll() << "]  " << flush;
    }
    cout << endl;
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S2.FirstVertex()); ! v.IsDone(); ++v) {
      gt::Vertex V(*v);
      cout << "V" << V.handle() << " = [" << V.x() << " " << V.y() << "]  " << flush;
    }
    cout << endl;
  }
  cout << endl;
  {
    // test empty ranges
    range_type S2(R);
    cout << "Empty subrange: " 
	 << S2.NumOfCells()    << " cells, "  << flush
	 << S2.NumOfVertices() << " vertices" << endl;
    
    cout << "Cells: " << flush;
    for(rgegt::CellIterator c(S2.FirstCell()); ! c.IsDone(); ++c) {
      gt::Cell C(*c);
      cout << "C" << C.handle() << " = [" << C.ll() << "]  " << flush;
    }
    cout << endl;
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S2.FirstVertex()); ! v.IsDone(); ++v) {
      gt::Vertex V(*v);
      cout << "V" << V.handle() << " = [" << V.x() << " " << V.y() << "]  " << flush;
    }
    cout << endl;
  }

  {
    enumerated_subrange_ref<grid_type> Sref(S);
    typedef grid_types<enumerated_subrange_ref<grid_type> > rfgt;

    test_sequence_iterator<rfgt::Vertex>(S, cout);
    test_sequence_iterator<rfgt::Edge>  (S, cout);
    test_sequence_iterator<rfgt::Cell>  (S, cout);

    test_incidence_iterator<rfgt::Vertex,rfgt::Edge>(S, cout);
    test_incidence_iterator<rfgt::Vertex,rfgt::Cell>(S, cout);
    test_incidence_iterator<rfgt::Edge,  rfgt::Cell>(S, cout);

    for(rfgt::VertexIterator v=Sref.FirstVertex(); !v.IsDone(); ++v)
      rfgt::Vertex vv = *v;

    grid_function<rfgt::Vertex, int> gf_rge(Sref,1);
    grid_function<gt  ::Vertex, int> gf    (R,   2);    
    for(rfgt::VertexIterator v=Sref.FirstVertex(); !v.IsDone(); ++v)
      gf_rge[*v] = gf(*v);

    grid_function<rfgt::Cell, int> gfc_rge(Sref,1);
    grid_function<gt  ::Cell, int> gfc    (R,   2);    
    for(rfgt::CellIterator c=Sref.FirstCell(); !c.IsDone(); ++c)
      gfc_rge[*c] = gfc(*c);


  }


}
