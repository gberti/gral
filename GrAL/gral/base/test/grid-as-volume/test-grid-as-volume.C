

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
*/

#include  "Gral/Geometries/grid-as-volume.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"

#include "Geometry/coords.h"
#include "Geometry/geometric-primitives.h"
#include "Container/functions.h" // std extensions: identity

#include "Gral/IO/complex2d-format-input.h"

#include <iostream>

using namespace GrAL;

typedef coordN<2> coord_type;
typedef stdext::identity<coord_type>              mapping_type;
typedef cartesian2d::CartesianGrid2D               grid_type;
typedef cartesian2d::mapped_geometry<mapping_type> cart_geom_type;
typedef grid_types<grid_type>        gt;

//template class grid_as_volume<cart_geom_type>;


int main() {
  using namespace std;

  {
    grid_type R(2,2);
    cart_geom_type GeomR(R);
    grid_as_volume<cart_geom_type> VolR(GeomR);
    
    typedef coord_type ct;
    coord_type p[5] = { ct(0,0), ct(0.5, 0.5), ct(1.0,1.5), ct(1.0,1.0), ct(1.0, 0.333)};
    
    for(int i = 0; i < sizeof(p)/sizeof(coord_type); ++i)
      cout << "point = " << p[i] << " is " << (VolR.is_inside(p[i]) ? "    " : " not ") << "inside grid" << endl;
  }

  {
    grid_type R(4,4);
    cart_geom_type GeomR(R);
    grid_as_volume<cart_geom_type> VolR(GeomR);
    
    typedef coord_type ct;
    coord_type p[5] = { ct(0,0), ct(0.5, 0.5), ct(1.0,1.5), ct(1.0,1.0), ct(1.0/3.0, 1.0/3.0)};
    
    for(int i = 0; i < sizeof(p)/sizeof(coord_type); ++i)
      cout << "point = " << p[i] << " is " << (VolR.is_inside(p[i]) ? "    " : " not ") << "inside grid" << endl;
    
    gt::Cell c[3] = { gt::Cell(R, gt::index_type(0,0)), 
		      gt::Cell(R, gt::index_type(1,1)),
		      gt::Cell(R, gt::index_type(1,0))};
    for(int i = 0; i < sizeof(c)/sizeof(gt::Cell); ++i) {
      cout << "Cell " << c[i].index() << " intersection with grid: " << flush;
      cout << VolR.name(VolR.intersection_check(c[i],GeomR)) << endl;
    }
    /*
    IstreamComplex2DFmt In("triangle.complex2d");
    
    Complex2D Triangle;
    stored_geometry_complex2D GeomTriangle(Triangle);
    ConstructGrid(Triangle, GeomTriangle, In, In);

    cout << "Triangle "  << " intersection with grid: " 
	 << VolR.name(VolR.intersection_check(* Triangle.FirstCell(),GeomTriangle)) << endl;
    */

    triangle<coord_type> t[5] = { 
      triangle<ct>(ct(0.0,0.0), ct(1.0,0.0), ct(0.0,1.0)), // intersection
      triangle<ct>(ct(0.1,0.1), ct(0.9,0.1), ct(0.1,0.9)), // inside
      triangle<ct>(ct(0.5,0.5), ct(1.5,0.5), ct(0.5,1.5)), // intersection
      triangle<ct>(ct(2.0,2.0), ct(3.0,2.0), ct(2.0,3.0)), // outside
      triangle<ct>(ct(-1 ,-1 ), ct(-1 ,5.0), ct(5.0,-1 )),  // contains -> intersects
    };
  
    for(int i = 0; i < sizeof(t)/sizeof(triangle<ct>); ++i) 
      cout << "Triangle "  << " intersection with grid: " 
	   << VolR.name(VolR.intersection_check(t[i])) << endl;
    
  }

  {
    Complex2D Triangle;
    stored_geometry_complex2D GeomTriangle(Triangle);
    
    IstreamComplex2DFmt In("triangle.complex2d");
    ConstructGrid(Triangle, GeomTriangle, In, In);
    grid_as_volume<stored_geometry_complex2D> VolT(GeomTriangle);

    cout << "Triangle "  << " intersection with grid: " 
	 << VolT.name(VolT.intersection_check(* Triangle.FirstCell(),GeomTriangle)) << endl;

  }


}
