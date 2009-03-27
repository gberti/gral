

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file 

    Test harness for \c cell_as_volume<>
*/

#include "Gral/Geometries/cell-as-volume.h"

#include "Gral/Grids/CartesianND/all.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include <iostream>

using namespace GrAL;

namespace cnd = cartesiannd;
typedef cnd::grid<2>                       grid_type;
typedef cnd::default_coord<cnd::grid<2> >::type coord2d;
typedef matrix<2,2,0>                              matrix2d;
typedef affine_mapping<matrix2d, coord2d>          mapping2d;
typedef cnd::mapped_geometry<grid_type, mapping2d>   geom_type;
typedef grid_types<grid_type> gt;

template class cell_as_volume<geom_type, gt::Cell, gt>;

int main() 
{
  using namespace std;
  grid_type R(gt::index_type(5,5));
  matrix2d A(0.0); A(0,0) = A(1,1) = 1.0;
  mapping2d M(A);
  geom_type GeomR(R, gt::index_type(0,0), gt::index_type(1,1), M);
  typedef cell_as_volume<geom_type, gt::Cell, gt> volume_type;
  volume_type Vol(GeomR, * R.FirstCell());
  
  cout << "Grid 1:\n";
  for(gt::VertexIterator v(R); ! v.IsDone(); ++v)
    cout << "V @ " << GeomR.coord(*v) << " is " 
	 << (Vol.is_inside(GeomR.coord(*v)) ? "    " : " not ") << "inside" << endl;
 
  for(gt::CellIterator c(R); ! c.IsDone(); ++c)
    cout << "C " << (*c).index() << ": " << Vol.name(Vol.intersection_check(*c, GeomR)) << endl;


  grid_type R2(gt::index_type(0,0), gt::index_type(3,3));
  matrix2d A2(0.0); A2(0,0) = A2(1,1) = 0.25;
  mapping2d M2(A2);
  geom_type GeomR2(R2,M2);

  cout << "Grid 2:\n";
  for(gt::VertexIterator v(R2); ! v.IsDone(); ++v)
    cout << "V @ " << GeomR2.coord(*v) << " is " 
	 << (Vol.is_inside(GeomR2.coord(*v)) ? "    " : " not ") << "inside" << endl;
  for(gt::CellIterator c(R2); ! c.IsDone(); ++c)
    cout << "C2 " << (*c).index() << ": " << Vol.name(Vol.intersection_check(*c, GeomR2)) << endl;
}
