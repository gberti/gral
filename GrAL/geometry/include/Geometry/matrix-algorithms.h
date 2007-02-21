#ifndef GRAL_GB_GEOMETRY_MATRIX_ALGORITHMS_H
#define GRAL_GB_GEOMETRY_MATRIX_ALGORITHMS_H

// $LICENSE_NEC_2005

#include "Geometry/point-traits.h"
#include "Geometry/matrix-traits.h"
#include "Geometry/algebraic-primitives.h"

#include <cmath>
#include <limits>

namespace GrAL {

  /*! \brief Determine rotation parameters from matrix
      This works only if M represents a rotation in 3D.

      Tested in \ref test-affine-mapping.C
   */
  template<class MATRIX, class COORD, class REAL>
  bool get_rotation_axis_angle(MATRIX const& M, COORD & axis, REAL & angle)
  {
    typedef COORD                            coord_type;
    typedef REAL                             real;
    typedef algebraic_primitives<coord_type> ap;
    typedef matrix_traits<MATRIX>            mt;

    // TODO: check if M is indeed rotation
    REQUIRE_ALWAYS(mt::NumOfRows(M) == 3 && mt::NumOfCols(M) == 3, 
		   "Cannot determine rotation axis of non-3x3 matrix!", 1);

    coord_type e1(1,0,0);
    coord_type e2(0,1,0);
    coord_type e3(0,0,1);

    // 3 vectors in rotation plane, must span plane
    coord_type r[] = { e1 - M*e1,e2 - M*e2,e3 - M*e3};

    // find 2 linear independent among r1,r2,r3
    // we set s[i] = r[(i+1)%3] * r[(i+2)%3]
    real s[] = {fabs(ap::norm_2(ap::vectorproduct(r[1],r[2]))),
		fabs(ap::norm_2(ap::vectorproduct(r[0],r[2]))),
		fabs(ap::norm_2(ap::vectorproduct(r[0],r[1]))) };
    int min_s = std::max_element(s, s+3) - s;

    // choose r[i],r[j] with minimal scalar product
    coord_type ra = r[(min_s+1)%3];
    coord_type rb = r[(min_s+2)%3];
    
    axis = ap::vectorproduct(ra,rb);
    // ra,rb,axis is a right-hand system

    if(ap::norm_2(axis) < 3*std::numeric_limits<real>::epsilon()) {
      angle = 0.0;
      axis  = e1; 
      return true;
    }

    angle = ap::angle(ra, M*ra);
    if(ap::dot(axis, ap::vectorproduct(ra, M*ra)) < 0 )
      angle = -angle;
    return true;

  }

} // namespace GrAL


#endif
