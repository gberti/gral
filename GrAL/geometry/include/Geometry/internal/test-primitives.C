#ifndef GRAL_GB_GEOMETRY_TEST_PRIMITIVES_C
#define GRAL_GB_GEOMETRY_TEST_PRIMITIVES_C

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/test-primitives.h"
#include "Geometry/algebraic-primitives.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class POINT>
void test_algebraic_primitives<POINT>::do_tests(std::ostream& out)
{
  typedef point_traits<POINT>         pt;
  typedef algebraic_primitives<POINT> ap;

  typename pt::Ptype p1 = pt::Origin(3);
  int li = pt::LowerIndex(p1); // should be the same for all points

  POINT e[3];
  for(int i = 0; i < 3; ++i) {
    e[i] = pt::Origin(3);
    e[i][li + i] = 1;
  }

  for(int i = 0; i < 3; ++i) {
    int j = (i < 2 ? i+1 : 0);
    int k = (i > 0 ? i-1 : 2);
    out << "ap::dot(p1,  e[" << i << "]) = " << ap::dot(p1,  e[i]) << '\n'
	<< "ap::dot(e["<<i<<"],e["<<i<<"]) = " << ap::dot(e[i],e[i]) << '\n'
	<< "ap::dot(e["<<k<<"],e["<<i<<"]) = " << ap::dot(e[k],e[i]) << '\n'
	<< "ap::dot(e["<<j<<"],e["<<i<<"]) = " << ap::dot(e[j],e[i]) << '\n'
	<< "ap::cos_of_angle(e["<<i<<"],e["<<i<<"]) = " 
	<< ap::cos_of_angle(e[i],e[i]) << '\n'
	<< "ap::cos_of_angle(e["<<k<<"],e["<<i<<"]) = " 
	<< ap::cos_of_angle(e[k],e[i]) << '\n'
	<< "ap::cos_of_angle(e["<<j<<"],e["<<i<<"]) = " 
	<< ap::cos_of_angle(e[j],e[i]) << '\n';

        
  }
}

} // namespace GrAL 

#endif
