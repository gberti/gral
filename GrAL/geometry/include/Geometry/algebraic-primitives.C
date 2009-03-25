#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES_C
#define NMWR_GB_ALGEBRAIC_PRIMITIVES_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Geometry/algebraic-primitives.h"

namespace GrAL {

//-------------------------------------------------------------------------
//
// calculate centroid and area of a simple polygon, given by the vertices
// [begin, end).
// The centroid (or center of inertia) c of a domain D is defined by
// \int_{D} x_i dx dy / area(D)
//
// Algorithm:
// ----------
// This is adapted from GraphicsGems IV, centroid.c
// It uses the theorem of Stokes to calculate the integral:
// \int_{D} x dx \wedge dy = \int{\partial D} 1/2 x^2 dy
//   ( note: d(1/2 x^2 dy) = x dx \wedge dy )
// if P,Q are two adjacent corners of the polygon, then
// \int{[P,Q]} 1/2 x^2 dy  = (P_1^2 + Q_1^2 +P_1Q_1)(Q_2 - P_2)
//                         = det(P,Q)                  * (P_1 + Q_1) 
//                         = 2 area (\triangle(0,P,Q)) * (P_1 + Q_1)
//
//-------------------------------------------------------------------------

template<class PIt, class Q>
void get_polygon2d_center_and_area(PIt begin, PIt end, // in:  iterator over polygon vertices
				   Q& center,          // out: centroid
				   double& area)       // out: polygon area
{
  typedef point_traits<Q> Qpt;
  // should use iterator_traits here to deduce value_type
  typedef typename PIt::value_type  P;
  typedef point_traits<P> Ppt;

  REQUIRE((begin != end), "empty polygon!\n",1);
  PIt v1 = begin;
  PIt vi = v1;
  ++begin; 
  REQUIRE((begin != end), "polygon with only one corner!\n",1);
  PIt vi1 = begin;
  double area_2 = 0.0, xtmp = 0.0, ytmp = 0.0;
  // center_x = \frac{\int_{P} x dx \wedge dy}{ area(P)}
  //  = \frac{\sum_{i = 1}^{n} \int{\triangle{0,vi,v_{i+1 (mod n)}} x dx \wedge dy}%
  //         {area(P)}
  //\int_\triangle(0,v_i,v_{i+1}) x dx \wedge dy 
  //  = 2 area(\triangle(0,v_i,v_{i+1})) * 1/6 ((v_i)_x + (v_{i+1})_x)
  // NOTE: this uses oriented integrals!
  while( vi1 != end) {
    P pi=*vi,pi1=*vi1;
    double ai = Ppt::x(pi) * Ppt::y(pi1) - Ppt::y(pi) * Ppt::x(pi1);
    area_2 += ai;
    xtmp += ai * (Ppt::x(pi) + Ppt::x(pi1));
    ytmp += ai * (Ppt::y(pi) + Ppt::y(pi1));
    ++vi; ++vi1;
  }
  // wrap around
  vi1 = v1;
  P pi=*vi,pi1=*vi1;
  double ai = Ppt::x(pi) * Ppt::y(pi1) - Ppt::y(pi) * Ppt::x(pi1);
  area_2 += ai;
  xtmp += ai * (Ppt::x(pi) + Ppt::x(pi1));
  ytmp += ai * (Ppt::y(pi) + Ppt::y(pi1));
  //  ++vi; ++vi1;
  
  area = 0.5 * area_2;
  // general case: translate center by v1
  Qpt::x(center) = xtmp / (3.0 * area_2);
  Qpt::y(center) = ytmp / (3.0 * area_2);
}

} // namespace GrAL 

#endif
