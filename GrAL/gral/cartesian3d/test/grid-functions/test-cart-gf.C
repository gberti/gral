#include <fstream>

#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/Grids/Cartesian3D/grid-functions.h"

#include "Utility/pre-post-conditions.h"

int main() {

  typedef CartesianGrid3D grid_type;
  typedef grid_types<grid_type> gt;
 
  std::ofstream* out = new std::ofstream("grid-functions.out");

  grid_type EmptyG; 
  REQUIRE_ALWAYS(EmptyG.NumOfVertices() == 0,
		 " NumOfVertices() == " << EmptyG.NumOfVertices(), 1);
  REQUIRE_ALWAYS(EmptyG.NumOfCells() == 0,
		 " NumOfCells() == " << EmptyG.NumOfCells(), 1);

  grid_type G(3,3,3);

  grid_function<gt::Vertex,gt::vertex_handle> gfv(G,1);
  grid_function<gt::Cell,  gt::cell_handle  > gfc(G,1);

  for(gt::VertexIterator v(G); ! v.IsDone(); ++v)
    gfv[*v] = v.handle();

  typedef grid_function<gt::Vertex,gt::vertex_handle>::const_iterator gfv_iter;
  for(gfv_iter vi = gfv.begin(); vi != gfv.end(); ++vi)
    *out << *vi << '\n';
  *out << endl;

  for(gt::CellIterator   c(G); ! c.IsDone(); ++c)
    gfc[*c] = c.handle();
  typedef grid_function<gt::Cell,gt::cell_handle>::const_iterator gfc_iter;
  for(gfc_iter ci = gfc.begin(); ci != gfc.end(); ++ci)
    *out << *ci << '\n';
  *out << endl;



  grid_function<gt::Vertex,gt::vertex_handle> gfv_copy1(gfv);
  REQUIRE_ALWAYS(gfv_copy1.size() == gfv_copy1.TheGrid().NumOfVertices(), 
		 "  size() = " << gfv_copy1.size() 
		 << " NumOfVertices() =  "  
		 << gfv_copy1.TheGrid().NumOfVertices() , 1);
 
  grid_function<gt::Vertex,gt::vertex_handle> gfv_copy2(G);
  REQUIRE_ALWAYS(gfv_copy2.size() == gfv_copy2.TheGrid().NumOfVertices(), 
		 "  size() = " << gfv_copy2.size() 
		 << " NumOfVertices() =  "  
		 << gfv_copy2.TheGrid().NumOfVertices() , 1);
  gfv_copy2 = gfv;

  grid_function<gt::Vertex,gt::vertex_handle> gfv_copy3;
  gfv_copy3 = gfv;
  REQUIRE_ALWAYS(gfv_copy3.size() == gfv_copy3.TheGrid().NumOfVertices(), 
		 "  size() = " << gfv_copy3.size() 
		 << " NumOfVertices() =  "  
		 << gfv_copy3.TheGrid().NumOfVertices() , 1);


  for(gt::VertexIterator v(G); ! v.IsDone(); ++v) {
    REQUIRE_ALWAYS( (gfv(*v) == gfv_copy1(*v)), "v = " << v.handle(),1);
    REQUIRE_ALWAYS( (gfv(*v) == gfv_copy2(*v)), "v = " << v.handle(),1);
    REQUIRE_ALWAYS( (gfv(*v) == gfv_copy3(*v)), "v = " << v.handle(),1);
  } 


  // test cell grid functions
  grid_function<gt::Cell,gt::cell_handle> gfc_copy1(gfc);
  REQUIRE_ALWAYS(gfc_copy1.size() == gfc_copy1.TheGrid().NumOfCells(), 
		 "  size() = " << gfc_copy1.size() 
		 << " NumOfCells() =  "  
		 << gfc_copy1.TheGrid().NumOfCells() , 1);
 
  grid_function<gt::Cell,gt::cell_handle> gfc_copy2(G);
  REQUIRE_ALWAYS(gfc_copy2.size() == gfc_copy2.TheGrid().NumOfCells(), 
		 "  size() = " << gfc_copy2.size() 
		 << " NumOfCells() =  "  
		 << gfc_copy2.TheGrid().NumOfCells() , 1);
  gfc_copy2 = gfc;

  grid_function<gt::Cell,gt::cell_handle> gfc_copy3;
  gfc_copy3 = gfc;
  REQUIRE_ALWAYS(gfc_copy3.size() == gfc_copy3.TheGrid().NumOfCells(), 
		 "  size() = " << gfc_copy3.size() 
		 << " NumOfCells() =  "  
		 << gfc_copy3.TheGrid().NumOfCells() , 1);


  for(gt::CellIterator c(G); ! c.IsDone(); ++c) {
    REQUIRE_ALWAYS( (gfc(*c) == gfc_copy1(*c)), "c = " << c.handle(),1);
    REQUIRE_ALWAYS( (gfc(*c) == gfc_copy2(*c)), "c = " << c.handle(),1);
    REQUIRE_ALWAYS( (gfc(*c) == gfc_copy3(*c)), "c = " << c.handle(),1);
  } 


  delete out;
}
