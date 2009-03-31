
/*! \file 

     Test harness for \c simple_geometry.
*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Complex3D/all.h"
#include "Gral/IO/complex2d-format.h"

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

#include "Gral/Grids/CartesianND/all.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"


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

    for(gt::CellIterator c(G); ! c.IsDone(); ++c) {
      cout << "c" << c.handle() << "  ";
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	cout << "vc" << vc.handle() << " angle: " << GeomG.solid_angle(vc) << " (" << GeomG.solid_angle_ratio(vc) << ")  ";
      cout << "\n";
    }
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

    for(gt::CellIterator c(G); ! c.IsDone(); ++c) {
      cout << "c" << c.handle() << "  ";
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	cout << "vc" << vc.handle() << " angle: " << GeomG.solid_angle(vc) << " (" << GeomG.solid_angle_ratio(vc) << ")  ";
      cout << "\n";

      for(gt::EdgeOnCellIterator ec(*c); !ec.IsDone(); ++ec) {
	cout << "ec" << ec.handle() << " outer normal: " << GeomG.outer_normal(ec) << "   "; 
      }
      cout << "\n";
    }

  } 


  {
    typedef cartesiannd::grid<3>                                  cart_grid_type;
    typedef cartesiannd::default_coord<cart_grid_type>::type      coord_type;
    typedef matrix<3,3,0>                                         matrix_type;
    typedef affine_mapping<matrix_type, coord_type>               mapping_type;
    typedef cartesiannd::mapped_geometry<cart_grid_type, mapping_type> cart_geom_type;

    typedef Complex3D grid_type;
    typedef simple_geometry<grid_type, coord_type> geom_type;
    typedef grid_types<grid_type> gt;



    cart_grid_type R(2,2,2);
    cart_geom_type GeomR(R, mapping_type::identity());
    grid_type G;
    geom_type GeomG;
    ConstructGrid(G,GeomG, R, GeomR);

    for(gt::VertexIterator v(G); !v.IsDone(); ++v)
      cout << "v" << v.handle() << " @ " << GeomG.coord(*v) << "\n";
    for(gt::EdgeIterator   e(G); !e.IsDone(); ++e) {
      cout << "e [";
      for(gt::VertexOnEdgeIterator ve(*e); !ve.IsDone(); ++ve) 
	cout << ve.handle() << " ";
      cout << "]  length:  " << GeomG.length(*e) << "\n";
    }
    for(gt::FaceIterator f(G); !f.IsDone(); ++f)
      cout << "f" << f.handle() << " area: " << GeomG.area(*f) << "\n";

    for(gt::CellIterator c(G); ! c.IsDone(); ++c) {
      cout << "c" << c.handle() << "  ";
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	cout << "vc" << vc.handle() << " angle: " << GeomG.solid_angle(vc) << " (" << GeomG.solid_angle_ratio(vc) << ")  ";
      cout << "\n";
    }

  }
  
}
