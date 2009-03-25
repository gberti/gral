

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

/*! \file Test harness for linear_cube_interpolator<>

*/

#include "Gral/Geometry/linear-cube.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/CartesianND/all.h"
#include "Gral/IO/complex2d-format.h"

#include "Gral/Geometries/simple-geometry.h"

#include "Container/functions.h"
#include "Container/function-adapter.h" // constant<>

#include "Geometry/coords.h"

#include <iostream>

using namespace GrAL;


// Explicit instantiation for 2D
typedef Complex2D                                  grid2d_type;
typedef tuple<double,2>                            coord2d_type;
typedef simple_geometry<grid2d_type, coord2d_type> geom2d_type;
typedef grid_types<grid2d_type>                    gt2d;
typedef grid_function<gt2d::Vertex,coord2d_type>     gf2d_type;
typedef linear_cube_interpolator<geom2d_type, gf2d_type> interpolator2d_type;

template class linear_cube_interpolator<geom2d_type, gf2d_type>;


// Explicit instantiation for 3D
typedef cartesiannd::grid<3>                         grid3d_type;
typedef tuple<double,3>                              coord3d_type;
typedef simple_geometry<grid3d_type, coord3d_type>   geom3d_type;
typedef grid_types<grid3d_type>                      gt3d;
typedef grid_function<gt3d::Vertex,coord3d_type>     gf3d_type;
typedef linear_cube_interpolator<geom3d_type, gf3d_type> interpolator3d_type;

template class linear_cube_interpolator<geom3d_type, gf3d_type>;



int main() {
  using namespace GrAL;
  using namespace std;


    
  interpolator2d_type::init();
  typedef interpolator2d_type ip;
  typedef ip::index_type      it;
  ip::cart2mesh[ip::cart_index_map(it(0,0))] = 0;
  ip::mesh2cart[0] = ip::cart_index_map(it(0,0));
  ip::cart2mesh[ip::cart_index_map(it(1,0))] = 1;
  ip::mesh2cart[1] = ip::cart_index_map(it(1,0));
  ip::cart2mesh[ip::cart_index_map(it(1,1))] = 2;
  ip::mesh2cart[2] = ip::cart_index_map(it(1,1));
  ip::cart2mesh[ip::cart_index_map(it(0,1))] = 3;
  ip::mesh2cart[3] = ip::cart_index_map(it(0,1));


  {
    
    grid2d_type G;
    geom2d_type GeomG;
    IstreamComplex2DFmt IG("grid.complex2d");
    ConstructGrid(G, GeomG, IG, IG);

    grid_function<gt2d::Vertex, coord2d_type> f(G);
    for(gt2d::VertexIterator v(G); !v.IsDone(); ++v)
      f[*v] = GeomG.coord(*v);
    
    constant<coord2d_type, double> C1(1.0);
    stdext::identity<coord2d_type>         Id;
    for(gt2d::CellIterator c(G); !c.IsDone(); ++c) {
      cout << "Cell " << c.handle() << endl;
      interpolator2d_type I(GeomG, f, *c);
      I.TheCubature()->init(1);
      I.init_integration();
      for(int i = 0; i < (int)I.TheCubature()->size(); ++i) {
	cout << "  local cubature point: " << I.TheCubature()->point(i)
	     << "  weight= " << I.TheCubature()->weight(i) 
	     << "  global cubature point: " << I.cubature_global_coord(i) << endl;
      }
      for(gt2d::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
	cout << "  global: "   << GeomG.coord(*vc) << "  local: " <<  I.local(GeomG.coord(*vc))  
	     << "  local->global: " << I.global(I.local(GeomG.coord(*vc)))
	     << "  jacobian: " << I.jacdet(I.local(GeomG.coord(*vc))) 
	     << "  f(x)=" << I(GeomG.coord(*vc))  << endl;
      }
      cout << "Volume=integrate_global(1): "
	   << I.integrate_global(C1) << endl;
      cout << "integrate_global(coord): "
	   << I.integrate_global(Id) << endl;
      cout << "Center=    " << I.integrate_global(Id) / I.integrate_global(C1)  << endl;

      coord2d_type center = I.integrate_global(Id) / I.integrate_global(C1);
      cout << "I(center)= " << I(center) << endl;
    }
  }

  {
    cout << "\n\n\n";

    grid2d_type G;
    geom2d_type GeomG;
    IstreamComplex2DFmt IG("grid2.complex2d");
    ConstructGrid(G, GeomG, IG, IG);

    grid_function<gt2d::Vertex, coord2d_type> f(G);
    for(gt2d::VertexIterator v(G); !v.IsDone(); ++v)
      f[*v] = GeomG.coord(*v);
    f[gt2d::Vertex(G, 4)] += coord2d_type(0.2);    

    constant<coord2d_type, double> C1(1.0);
    stdext::identity<coord2d_type>         Id;
    for(gt2d::CellIterator c(G); !c.IsDone(); ++c) {
      cout << "Cell " << c.handle() << endl;
      interpolator2d_type I(GeomG, f, *c);
      I.TheCubature()->init(1);
      I.init_integration();
      for(int i = 0; i < (int)I.TheCubature()->size(); ++i) {
	cout << "  local cubature point: " << I.TheCubature()->point(i)
	     << "  weight= " << I.TheCubature()->weight(i) 
	     << "  global cubature point: " << I.cubature_global_coord(i) << endl;
      }
      for(gt2d::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
	cout << "  global: "   << GeomG.coord(*vc) << "  local: " <<  I.local(GeomG.coord(*vc))  
	     << "  local->global: " << I.global(I.local(GeomG.coord(*vc)))
	     << "  jacobian: " << I.jacdet(I.local(GeomG.coord(*vc))) 
	     << "  f(x)=" << I(GeomG.coord(*vc))  << endl;
      }
      cout << "Volume=integrate_global(1): "
	   << I.integrate_global(C1) << endl;
      cout << "integrate_global(coord): "
	   << I.integrate_global(Id) << endl;
      cout << "Center=    " << I.integrate_global(Id) / I.integrate_global(C1)  << endl;

      coord2d_type center = I.integrate_global(Id) / I.integrate_global(C1);
      cout << "I(center)= " << I(center) << endl;
    }
  }


  {
    cout << "\n\n\n";

    // cartesiannd::delta_map<3> d;
    //cartesiannd::delta_map<3>::force_init();
 
    grid3d_type G(gt3d::index_type(3,3,3));
    geom3d_type GeomG(G);
    for(gt3d::VertexIterator v(G); !v.IsDone(); ++v)
      GeomG.coord(*v) = coord3d_type((*v).index());
    GeomG.coord(gt3d::Vertex(G, gt3d::index_type(1))) += coord3d_type(0.0,0.1,0.2);
    
    typedef interpolator3d_type li;
    li::init();
    int vcnt = 0;
    for(gt3d::VertexOnCellIterator vc(* G.FirstCell()); !vc.IsDone(); ++vc, ++vcnt) {
      li::cart2mesh[li::cart_index_map(vc.index())] = vcnt;
      li::mesh2cart[vcnt] = li::cart_index_map(vc.index());
    }
      


    grid_function<gt3d::Vertex, coord3d_type> f(G);
    for(gt3d::VertexIterator v(G); !v.IsDone(); ++v)
      f[*v] = GeomG.coord(*v);

    constant<coord3d_type, double> C1(1.0);
    stdext::identity<coord3d_type>         Id;
    for(gt3d::CellIterator c(G); !c.IsDone(); ++c) {
      cout << "Cell " << c.handle() << endl;
      interpolator3d_type I(GeomG, f, *c);
      I.TheCubature()->init(1);
      I.init_integration();
      for(int i = 0; i < (int)I.TheCubature()->size(); ++i) {
	cout << "  local cubature point: " << I.TheCubature()->point(i)
	     << "  weight= " << I.TheCubature()->weight(i) 
	     << "  global cubature point: " << I.cubature_global_coord(i) << endl;
      }
      for(gt3d::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
	cout << "  global: "   << GeomG.coord(*vc) << "  local: " <<  I.local(GeomG.coord(*vc))  
	     << "  local->global: " << I.global(I.local(GeomG.coord(*vc)))
	     << "  jacobian: " << I.jacdet(I.local(GeomG.coord(*vc))) 
	     << "  f(x)=" << I(GeomG.coord(*vc))  << endl;
      }
      cout << "Volume=integrate_global(1): "
	   << I.integrate_global(C1) << endl;
      cout << "integrate_global(coord): "
	   << I.integrate_global(Id) << endl;
      cout << "Center=    " << I.integrate_global(Id) / I.integrate_global(C1)  << endl;

      coord3d_type center = I.integrate_global(Id) / I.integrate_global(C1);
      cout << "I(center)= " << I(center) << endl;
    }
  }



  
}
