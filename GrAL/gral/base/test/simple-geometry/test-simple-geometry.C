
/*! \file 

     Test harness for \c simple_geometry.
*/

#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format.h"

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

#include <iostream>

using namespace GrAL;

typedef tuple<double,2> coord_type_2d;
typedef tuple<double,3> coord_type_3d;
template class simple_geometry<Complex2D, coord_type_2d>;
template class simple_geometry<Complex2D, coord_type_3d>;


int main() {
  using namespace std;

  {
    typedef Complex2D grid_type;
    typedef grid_types<grid_type> gt;
    typedef tuple<double,2> coord_type;
    typedef simple_geometry<grid_type, coord_type> geom_type;
    IstreamComplex2DFmt In("grid2d.complex2d");
    grid_type G;
    geom_type GeomG(G);
    ConstructGrid(G, GeomG, In, In);

    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      cout << "v" << v.handle() << " @ " << GeomG.coord(*v) << "\n";
    for(gt::EdgeIterator   e(G); !e.IsDone(); ++e)
      cout << "e [" << e.v1() << "," << e.v2() << "]  length:  " << GeomG.length(*e) << "\n";
    for(gt::FaceIterator f(G); !f.IsDone(); ++f)
      cout << "f" << f.handle() << " area: " << GeomG.area(*f) << "\n";
  }

  {
    typedef Complex2D grid_type;
    typedef grid_types<grid_type> gt;
    typedef tuple<double,3> coord_type;
    typedef simple_geometry<grid_type, coord_type> geom_type;
    IstreamComplex2DFmt In("grid3d.complex2d");
    In.set_spacedim(3);
    grid_type G;
    geom_type GeomG(G);
    ConstructGrid(G, GeomG, In, In);

    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      cout << "v" << v.handle() << " @ " << GeomG.coord(*v) << "\n";
    for(gt::EdgeIterator   e(G); !e.IsDone(); ++e)
      cout << "e [" << e.v1() << "," << e.v2() << "]  length:  " << GeomG.length(*e) << "\n";
    for(gt::FaceIterator f(G); !f.IsDone(); ++f)
      cout << "f" << f.handle() << " area: " << GeomG.area(*f) << "\n";
  }
}
