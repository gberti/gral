
// $LICENSE_NEC

/*! \file
 
*/

#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

int main() {
  using namespace std;
  using namespace cartesian2d;

  typedef grid_types<CartesianGrid2D> rgt;
  CartesianGrid2D R(3,3);
  
  // -----------------------------------------------------------------------
  // test vertex ranges 
  // -----------------------------------------------------------------------

  typedef std::vector<rgt::vertex_handle> vertex_seq_type;
  typedef element_range_ref<rgt::Vertex,     vertex_seq_type> ev_range_type_ref;
  typedef vertex_range_ref <CartesianGrid2D, vertex_seq_type> v_range_type_ref;
  typedef enumerated_element_range<rgt::Vertex>               ev_range_type;
  typedef enumerated_vertex_range<CartesianGrid2D>            v_range_type;

  typedef ev_range_type_ref  evr_gt;
  typedef v_range_type_ref    vr_gt;
  typedef ev_range_type       ev_gt;
  typedef v_range_type         v_gt;


  ev_range_type Sev(R);
  v_range_type  Sv(R);
  vertex_seq_type verts;
  for(rgt::VertexIterator v(R); !v.IsDone(); ++v) {
    verts.push_back(v.handle());
    Sev  .push_back(v.handle());
    Sv   .push_back(v.handle());
  }
  ev_range_type_ref Sevr(0, verts.end() - verts.begin(), verts, R); 
  v_range_type_ref  Svr (0, verts.end() - verts.begin(), verts, R); 
  
  {
    cout << "Vertex range Sv: " 
	 << Sv.NumOfVertices() << " vertices" << endl;
    cout << "Vertices: " << flush;
    for(v_gt::VertexIterator v(Sv.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
    cout << endl;
    cout << "Element range Sev: " 
	 << Sev.NumOfVertices() << " vertices" << endl;
    cout << "Vertices: " << flush;
    for(ev_gt::VertexIterator v(Sev.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
    cout << endl;
  }

  {
    cout << "Vertex range ref Svr: " 
	 << Sv.NumOfVertices() << " vertices" << endl;
    cout << "Vertices: " << flush;
    for(vr_gt::VertexIterator v(Svr.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
    cout << endl;
    cout << "Element range Sevr: " 
	 << Sevr.NumOfVertices() << " vertices" << endl;
    cout << "Vertices: " << flush;
    for(evr_gt::VertexIterator v(Sevr.FirstVertex()); ! v.IsDone(); ++v)
      cout << "V" << v.handle() << " = [" << (*v).x() << " " << (*v).y() << "]  " << flush;
    cout << endl;
  }
  cout << endl;


  // -----------------------------------------------------------------------
  // test cell ranges
  // -----------------------------------------------------------------------
  {
    typedef std::vector<rgt::cell_handle>                   cell_seq_type;
    typedef element_range_ref<rgt::Cell,     cell_seq_type> ec_range_type_ref;
    typedef cell_range_ref <CartesianGrid2D, cell_seq_type> c_range_type_ref;
    typedef enumerated_element_range<rgt::Cell>               ec_range_type;
    typedef enumerated_cell_range<CartesianGrid2D>            c_range_type;

    typedef ec_range_type_ref  ecr_gt;
    typedef c_range_type_ref    cr_gt;
    typedef ec_range_type       ec_gt;
    typedef c_range_type         c_gt;


    ec_range_type Sec(R);
    c_range_type  Sc(R);
    cell_seq_type cells;
    for(rgt::CellIterator c(R); !c.IsDone(); ++c) {
      cells.push_back(c.handle());
      Sec  .push_back(c.handle());
      Sc   .push_back(c.handle());
    }
    ec_range_type_ref Secr(0, cells.end() - cells.begin(), cells, R); 
    c_range_type_ref  Scr (0, cells.end() - cells.begin(), cells, R); 
  
    {
      cout << "Cell range Sc: " 
	   << Sc.NumOfCells() << " cells" << endl;
      cout << "Cells: " << flush;
      for(c_gt::CellIterator c(Sc.FirstCell()); ! c.IsDone(); ++c)
	cout << "C" << c.handle() << " = [" << (*c).ll()  << "]  " << flush;
      cout << endl;
      cout << "Element range Sec: " 
	   << Sec.NumOfCells() << " cells" << endl;
      cout << "Cells: " << flush;
      for(ec_gt::CellIterator c(Sec.FirstCell()); ! c.IsDone(); ++c)
	cout << "C" << c.handle() << " = [" << (*c).ll() << "]  " << flush;
      cout << endl;
    }

    {
      cout << "Cell range ref Scr: " 
	   << Sc.NumOfCells() << " cells" << endl;
      cout << "Cells: " << flush;
      for(cr_gt::CellIterator c(Scr.FirstCell()); ! c.IsDone(); ++c)
	cout << "C" << c.handle() << " = [" << (*c).ll() << "]  " << flush;
      cout << endl;
      cout << "Element range Secr: " 
	   << Secr.NumOfCells() << " cells" << endl;
      cout << "Cells: " << flush;
      for(ecr_gt::CellIterator c(Secr.FirstCell()); ! c.IsDone(); ++c)
	cout << "C" << c.handle() << " = [" << (*c).ll() << "]  " << flush;
      cout << endl;
    }
    cout << endl;
  }

}
