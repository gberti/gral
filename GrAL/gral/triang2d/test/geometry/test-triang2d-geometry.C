
// LICENSE_NEC_2003

/*! \file 
   
    Test harness for stored_geometry_triang2d

*/

#include "Gral/Grids/Triang2D/stored-geometry.h"
#include "Gral/Grids/Triang2D/grid-functions.h"

#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;
  typedef Triang2D                 grid_type;
  typedef stored_geometry_triang2d geom_type;
  typedef grid_types<grid_type>    gt;

  typedef int triangle[3];
  typedef double ct[2];

  {
    triangle tri[] = { {0, 1, 4}, {1, 2, 4}, {2,3,4}, {3,0,4} };
    
    ct coords[] = { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0,1.0}, {0.5,0.4} };
    
    int nc = sizeof(tri)/sizeof(triangle);
    int nv = sizeof(coords)/sizeof(ct);
    
    grid_type T((int *)tri, nc, nv);
    geom_type GeomT1(T, (double *)coords);
    
    grid_function<gt::Vertex, double> sum_ratios(T,0.0);
    for(gt::VertexIterator v(T); ! v.IsDone(); ++v)
      cout << v.handle() << " @ " << GeomT1.coord(*v) << endl;
    for(gt::CellIterator c(T); ! c.IsDone(); ++c) {
      cout << c.handle() 
	   <<  " center=" << GeomT1.center(*c) << "  ";
      for(gt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc) {
	cout << "; v=" << vc.handle()
	     << ": a=" << GeomT1.solid_angle(vc)
	     << "  r=" << GeomT1.solid_angle_ratio(vc);
	sum_ratios[*vc] += GeomT1.solid_angle_ratio(vc);
      }
      cout << endl;
    }
    for(gt::VertexIterator v(T); ! v.IsDone(); ++v)
      cout << v.handle() << ":  sum of ratios=" << sum_ratios[*v] << endl;
  }

  {
    triangle tri[] = { {0, 1, 2}};
    
    ct coords[] = { {0.0, 0.0}, {1.0, 0.0}, {0.0,1.0}};
    
    int nc = sizeof(tri)/sizeof(triangle);
    int nv = sizeof(coords)/sizeof(ct);
    grid_type T((int *)tri, nc, nv);
    geom_type Geom(T, (double *)coords);
    cout << "Triangulation: #cells=" << nc << "; #vertices=" << nv << endl;
    for(gt::EdgeIterator e(T); !e.IsDone(); ++e)
      cout << "E [" << (*e).V1().handle() << "->" << (*e).V2().handle() 
	   << "] length=" << Geom.length(*e) << endl;
    for(gt::FacetIterator f(T); ! f.IsDone(); ++f) {
      cout << "F [";
      for(gt::VertexOnFacetIterator vf(*f); ! vf.IsDone(); ++vf)
	cout << vf.handle() << " ";
      cout << "] volume=" << Geom.volume(*f) << endl;
    }
  }

}
