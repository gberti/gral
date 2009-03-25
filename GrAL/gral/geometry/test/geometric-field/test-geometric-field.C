/*! \file Test harness for geometric_field<>

 */

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometry/geometric-field.h"
#include "Gral/Geometry/linear-simplex.h"
#include "Gral/Grids/Triang2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format.h"

#include <iostream>


using namespace GrAL;

//typedef Complex2D                 grid2d_type;
//typedef stored_geometry_complex2D geom2d_type;
typedef Triang2D                 grid2d_type;
typedef stored_geometry_triang2d geom2d_type;
typedef geom2d_type::coord_type coord_type;
typedef grid_types<grid2d_type>   gt2d;
typedef grid_function<gt2d::Vertex,coord_type> gf2d_type;
typedef linear_simplex_interpolator<geom2d_type, gf2d_type> interpolator2d_type;

template class linear_simplex_interpolator<geom2d_type, gf2d_type>;
template class geometric_field<geom2d_type, gf2d_type, interpolator2d_type>;

int main () {
  using namespace GrAL;
  using namespace std;
  typedef gt2d gt;

  IstreamComplex2DFmt IG("grid.complex2d");
  grid2d_type G;
  geom2d_type GeomG(G);
  ConstructGrid(G, GeomG, IG, IG);

  {
    grid_function<gt::Vertex, coord_type> f(G);
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      f[*v] = GeomG.coord(*v);
    
    geometric_field<geom2d_type, gf2d_type, interpolator2d_type> field(GeomG, f);
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      cout <<  "f(" << GeomG.coord(*v) << ") = " << field(GeomG.coord(*v)) << endl;
    
    for(gt::CellIterator c(G); !c.IsDone(); ++c)
      cout <<  "f(" << GeomG.center(*c) << ") = " << field(GeomG.center(*c)) << endl;
  }

  {
    grid_function<gt::Vertex, double> f(G, 0.0);
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      f[*v] = (double)v.handle();
    
    typedef grid_function<gt::Vertex, double> gf_type;
    typedef linear_simplex_interpolator<geom2d_type, gf_type> interpolator_type;
    geometric_field<geom2d_type,
                    gf_type,
                    interpolator_type> 
             field(GeomG, f);
    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      cout <<  "f(" << GeomG.coord(*v) << ") = " << field(GeomG.coord(*v)) << endl;
    for(gt::CellIterator c(G); !c.IsDone(); ++c)
      cout <<  "f(" << GeomG.center(*c) << ") = " << field(GeomG.center(*c)) << endl;

    typedef coord_type ct;
    ct c[] = { ct(0.5,0.0), ct(0.5,0.5), ct(1.0,0.5), ct(0.5,1.0), ct(0.0,0.5) };

    for(unsigned i = 0; i < sizeof(c)/sizeof(ct); ++i)
      cout <<  "f(" << c[i] << ") = " << field(c[i]) << endl;
  }
}
