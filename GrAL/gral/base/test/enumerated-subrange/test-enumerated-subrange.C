#include "Gral/Subranges/enumerated-subrange.h"


#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Test/test-archetypes.h"

#include <iostream>

int main() {
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

  test_archetypes(S, cout);
  {
    // test construction of ranges
    cout << "Subrange S: " 
	 << S.NumOfCells()    << " cells, "  << flush
	 << S.NumOfVertices() << " vertices" << endl;
    
    cout << "Cells: " << flush;
    for(rgegt::CellIterator c(S.FirstCell()); ! c.IsDone(); ++c)
      cout << "C " << c.handle() << " = [" << (*c).ll() << "]  " << flush;
    cout << endl;
    
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
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
    for(rgegt::CellIterator c(S2.FirstCell()); ! c.IsDone(); ++c)
      cout << "C" << c.handle() << " = [" << (*c).ll() << "]  " << flush;
    cout << endl;
    
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S2.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
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
    for(rgegt::CellIterator c(S2.FirstCell()); ! c.IsDone(); ++c)
      cout << "C" << c.handle() << " = [" << (*c).ll() << "]  " << flush;
    cout << endl;
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S2.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
    cout << endl;
  }
  cout << endl;
  {
    // test empty ranges
    range_type S2;
    cout << "Empty subrange: " 
	 << S2.NumOfCells()    << " cells, "  << flush
	 << S2.NumOfVertices() << " vertices" << endl;
    
    cout << "Cells: " << flush;
    for(rgegt::CellIterator c(S2.FirstCell()); ! c.IsDone(); ++c)
      cout << "C" << c.handle() << " = [" << (*c).ll() << "]  " << flush;
    cout << endl;
    cout << "Vertices: " << flush;
    for(rgegt::VertexIterator v(S2.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
    cout << endl;
  }

  {
    enumerated_subrange_ref<grid_type> Sref(S);
    typedef grid_types<enumerated_subrange_ref<grid_type> > rfgt;
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
