#include "Gral/Subranges/enumerated-subrange.h"


#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

int main() {
  using namespace std;
  using namespace cartesian2d;

  typedef grid_types<CartesianGrid2D> rgt;
  CartesianGrid2D R(3,3);
  
  typedef enumerated_subrange<CartesianGrid2D> range_type;
  typedef grid_types<range_type >              rgegt;

  cout << "Constructing subrange from 3x3 grid ... " << flush;
  range_type S(R, R);
  cout << " done." << endl;

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
    for(rgt::CellIterator c(R); !c.IsDone(); ++c)
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

}
