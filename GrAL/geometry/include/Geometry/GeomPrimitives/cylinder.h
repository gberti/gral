#ifndef GRAL_GB_GEOMETRY_GEOM_PRIMITIVES_CYLINDER_H
#define GRAL_GB_GEOMETRY_GEOM_PRIMITIVES_CYLINDER_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file 
  
  \author Guntram Berti  <berti@ccrl-nece.de>
  
  <tt> $Id: </tt>
 */

#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/geometric-primitives.h"


namespace GrAL { 

  template<class P>
  class cylinder 
  {
    typedef P                           coord_type;
    typedef point_traits<P>             pt;
    typedef algebraic_primitives<P>     ap;
    typedef typename pt::component_type real;
  private:
    line<P> axis;
    real    radius;
  public:
    cylinder() : radius(0) {}
    cylinder(segment<P> const& ax, real r)
      : axis(ax), radius(r) 
    {}
    cylinder(P p0, P p1, real r)
      : axis(p0,p1-p0), radius(r) 
    {}


    bool inside(coord_type const& X) const
    {
      real t = ap::dot(X-axis.p0(),axis.dir()) / ap::dot(axis.dir(), axis.dir());
      return ( 0 <=t && t <= 1 && ap::squared_norm_2(X - axis(t)) <= radius * radius);
    }
    bool operator()(coord_type const& X) const { return inside(X);}
  };

} // namespace GrAL

#endif
