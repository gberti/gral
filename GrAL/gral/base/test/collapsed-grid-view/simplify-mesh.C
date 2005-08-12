/*! \file

*/

#include "Gral/Views/collapsed-grid-view.h"
#include "Gral/Grids/Triang2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Gral/IO/complex2d-format.h"
#include "Gral/IO/gmv-format-output2d.h"

#include "Gral/Subranges/enumerated-subrange.h"

#include "Container/tuple-point-traits.h"
#include "Container/sequence-algorithms.h"
#include "IO/control-device.h"
#include "Utility/as-string.h"

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char * argv[])
{
  using namespace std;
  using namespace GrAL;

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"","main");
  string input;
  RegisterAt(Ctrl, "-in", input);
  RegisterAt(Ctrl, "-g",  input);
  string output;
  RegisterAt(Ctrl, "-out", output);
  RegisterAt(Ctrl, "-o",   output);

  double t = 0.25;
  RegisterAt(Ctrl, "-t", t);

  Ctrl.update();

  typedef grid_types<Triang2D> gt;
  typedef tuple<double,3> coord_type;

  IstreamComplex2DFmt In(input);
  In.set_spacedim(3);
  Triang2D T;
  typedef simple_geometry<Triang2D, coord_type> geom_type;
  geom_type GeomT; // (T);
  ConstructGrid(T, GeomT, In, In);

  namespace cgv = collapsed_grid_view;
  cgv::grid_view<Triang2D> C(T);
  cgv::geom_view<Triang2D, geom_type> GeomC(C, GeomT);
  

  //  grid_function<gt::Edge, double> len(T);
  int ecnt = 0;
  for(gt::EdgeIterator e(T); !e.IsDone(); ++e)
    ++ecnt;
  vector<double> len(ecnt);
  ecnt = 0; 
  for(gt::EdgeIterator e(T); !e.IsDone(); ++e)
    len[ecnt++] = GeomT.length(*e);
  
  double avg_len = sequence::average(len.begin(), len.end());
  cout << "average edge length: " << avg_len << endl;

  grid_function<gt::Cell, double> vol(T);
  for(gt::CellIterator c(T); !c.IsDone(); ++c)
    vol[*c] = GeomT.volume(*c);
  double avg_vol = sequence::average(vol.begin(), vol.end());


  for(gt::CellIterator c(T); !c.IsDone(); ++c) {
    if(vol(*c) < t * avg_vol && ! C.collapsed(*c)) {
      vector<enumerated_vertex_range<Triang2D> > vs(1);
      vs[0].set_grid(T);
      coord_type center(0.0);
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
	vs[0].push_back(*vc);
	center += GeomC.coord(*vc);
      }
      center = center * (1.0/(*c).NumOfVertices());
      C.collapse_vertex_sets(vs.begin(), vs.end());
      GeomC.coord(* vs[0].FirstVertex()) = center;
    }
  }

  /*
  int cnt = 0;
  for(gt::EdgeIterator e(T); !e.IsDone(); ++e, ++cnt) {
    if( (len[cnt] < t * avg_len) && C.CollapsedVertex((*e).V1()) != C.CollapsedVertex((*e).V2())) {
      vector<enumerated_vertex_range<Triang2D> > vs(1);
      vs[0].set_grid(T);
      vs[0].push_back((*e).V1());
      vs[0].push_back((*e).V2());
      coord_type v1 = convert_point<coord_type>(GeomC.coord((*e).V1()));
      coord_type v2 = convert_point<coord_type>(GeomC.coord((*e).V2()));
      C.collapse_vertex_sets(vs.begin(), vs.end());
      GeomC.coord((*e).V1()) = 0.5*(v1+v2);
    }
  }
  */

  OstreamGMV2DFmt Out(output);
  ConstructGrid(Out, C, GeomC);

}

