/*! \file

*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Views/collapsed-grid-view.h"
#include "Gral/Grids/Triang2D/all.h"

#include "Gral/IO/complex2d-format.h"
#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Test/all.h"

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

  {
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
    typedef grid_types<Triang2D>                  gt;
    typedef grid_types<cgv::grid_view<Triang2D> > cgt;
    
    int cnt = 0;
    for(gt::EdgeIterator e(T); !e.IsDone(); ++e) {
      
      test_sequence_iterator <cgt::Vertex>          (C, cout);
      test_sequence_iterator <cgt::Cell  >          (C, cout);
      test_incidence_iterator<cgt::Vertex,cgt::Cell>(C, cout);
      
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

  {
    string gridfile = "2x2.complex2d";
    IstreamComplex2DFmt In(gridfile);
    In.set_spacedim(2);
    typedef tuple<double,2> coord_type;
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
    typedef grid_types<Triang2D>                  gt;
    typedef grid_types<cgv::grid_view<Triang2D> > cgt;

    vector<enumerated_vertex_range<Triang2D> > vs(3, enumerated_vertex_range<Triang2D>(T));
    typedef point_traits<coord_type>           pt;
    for(gt::VertexIterator v(T); !v.IsDone(); ++v)
      if(pt::y(GeomT.coord(*v)) < 1)
	vs[(int) (2 * pt::x(GeomT.coord(*v)))].push_back(v);
    C.collapse_vertex_sets(vs.begin(), vs.end());
    for(int i = 0; i < 3; ++i)
      GeomC.coord(* vs[i].FirstVertex()) = coord_type(pt::x(GeomT.coord(* vs[i].FirstVertex())), 0);
    OstreamGMV2DFmt Out("2x2-collapsed.gmv");
    ConstructGrid(Out, C, GeomC);
  }

}

