

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

/*! \file Test harness for linear_simplex_interpolator<>

*/

#include "Gral/Geometry/linear-simplex.h"
#include "Gral/Grids/Triang2D/all.h"
#include "Container/functions.h"

#include <iostream>

using namespace GrAL;

template<class RES, class ARG>
class constant {
public:
  typedef RES result_type;
  typedef ARG argument_type;
private:
  result_type r;
public:
  constant() {}
  constant(result_type rr) : r(rr) {}
  
  result_type const& operator()(argument_type const&) const { return r;}
};

typedef Triang2D                 grid2d_type;
typedef stored_geometry_triang2d geom2d_type;
typedef geom2d_type::coord_type coord_type;
typedef grid_types<grid2d_type>   gt2d;
typedef grid_function<gt2d::Vertex,coord_type> gf2d_type;
typedef linear_simplex_interpolator<geom2d_type, gf2d_type> interpolator2d_type;

template class linear_simplex_interpolator<geom2d_type, gf2d_type>;

int main() {
  using namespace GrAL;
  using namespace std;

  typedef gt2d::size_type triangle[3];
  triangle t[] = { {0,1,2}, {1,3,2}, {1,4,3}};
  typedef double ct[2];
  typedef coord_type co;
  ct       coords[] = {   {0.0, 0.0},   {1.0, 0.0},   {0.0, 1.0},   {1.0, 1.0},   {1.5, 0.0}};
  co       ff[]     = { co(0.0, 0.0), co(1.0, 0.0), co(0.0, 1.0), co(1.0, 1.0), co(1.5, 0.0)};
  
  gt2d::size_type nc = sizeof(t)/sizeof(triangle);
  gt2d::size_type nv = sizeof(coords)/sizeof(ct);

  grid2d_type T    ((gt2d::size_type*)t, nc, nv);
  geom2d_type GeomT(T, (double *)coords);

  grid_function<gt2d::Vertex, coord_type> f(T);
  f.set_values(ff, ff + nv);

  constant<double, coord_type> C1(1.0);
  stdext::identity<coord_type>         Id;
  for(gt2d::CellIterator c(T); !c.IsDone(); ++c) {
    cout << "Cell " << c.handle() << endl;
    interpolator2d_type I(GeomT, f, *c);
    for(int i = 0; i < (int)I.TheCubature()->size(); ++i) {
      cout << "  local cubature point: " << I.TheCubature()->point(i)
	   << "  weight= " << I.TheCubature()->weight(i) 
	   << "  global cubature point: " << I.cubature_global_coord(i) << endl;
    }
    for(gt2d::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
      cout << "  global: "   << GeomT.coord(*vc) << "  local: " <<  I.local(GeomT.coord(*vc))  
	   << "  local->global: " << I.global(I.local(GeomT.coord(*vc)))
	   << "  jacobian: " << I.jacobian(I.local(GeomT.coord(*vc))) << endl;
    }
    cout << "Volume=integrate_global(1): "
	 << I.integrate_global(C1) << endl;
    cout << "integrate_global(coord): "
	 << I.integrate_global(Id) << endl;
    cout << "Center: " << I.integrate_global(Id) / I.integrate_global(C1)  << endl;
  }
}
