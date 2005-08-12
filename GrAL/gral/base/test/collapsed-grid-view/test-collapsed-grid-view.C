/*! \file

*/

#include "Gral/Views/collapsed-grid-view.h"
#include "Gral/Grids/Triang2D/all.h"

#include "Gral/IO/complex2d-format.h"
#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Gral/Subranges/enumerated-subrange.h"

#include "Container/tuple-point-traits.h"
#include "Utility/as-string.h"

#include <iostream>
#include <string>
#include <vector>

namespace GrAL { namespace collapsed_grid_view {

  template class grid_view<Triang2D>;

} }


int main()
{
  using namespace std;
  using namespace GrAL;

  string gridfile = "triang.complex2d";
  IstreamComplex2DFmt In(gridfile);
  In.set_spacedim(3);
  typedef tuple<double,3> coord_type;
  typedef Triang2D        grid_type;
  typedef simple_geometry<grid_type, coord_type> geom_type;
  grid_type T;
  geom_type GeomT;
  ConstructGrid(T, GeomT, In, In);

  namespace cgv = collapsed_grid_view;
  cgv::grid_view<Triang2D> C(T);
  cgv::geom_view<Triang2D, geom_type> GeomC(C, GeomT);
  
  REQUIRE_ALWAYS(C.NumOfVertices() == T.NumOfVertices(), "",1);
  REQUIRE_ALWAYS(C.NumOfCells()    == T.NumOfCells(),    "",1);

  cout.precision(16);
  typedef grid_types<Triang2D> gt;
  int cnt = 0;
  for(gt::EdgeIterator e(T); !e.IsDone(); ++e) {
    if(C.CollapsedVertex((*e).V1()) != C.CollapsedVertex((*e).V2())) {
      cout << "Collapsing edge " << (*e).v1() << "->" << (*e).v2() << "\n";
      ++cnt;
      vector<enumerated_vertex_range<Triang2D> > vs(1);
      vs[0].set_grid(T);
      vs[0].push_back((*e).V1());
      vs[0].push_back((*e).V2());
      coord_type v1 = convert_point<coord_type>(GeomC.coord((*e).V1()));
      coord_type v2 = convert_point<coord_type>(GeomC.coord((*e).V2()));
      C.collapse_vertex_sets(vs.begin(), vs.end());
      GeomC.coord((*e).V1()) = 0.5*(v1+v2);
      cout << "New coord: " <<  0.5*(v1+v2)
	   << " = " << GeomC.coord((*e).V1()) << " = " << GeomC.coord((*e).V2()) << "\n";
      cout << C.NumOfVertices() << " vertices, " << C.NumOfCells() << " cells.\n";
      OstreamGMV2DFmt Out("collapsed" + as_string(cnt) + ".gmv");
      ConstructGrid(Out, C, GeomC);
    }
    else
      cout << "Skipping edge " << (*e).v1() << "->" << (*e).v2() << "\n";
  }

}

