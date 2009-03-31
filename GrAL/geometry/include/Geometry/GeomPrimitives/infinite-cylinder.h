#ifndef GRAL_GB_GEOMETRY_GEOM_PRIMITIVES_INFINITE_CYLINDER_H
#define GRAL_GB_GEOMETRY_GEOM_PRIMITIVES_INFINITE_CYLINDER_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

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
  class infinite_cylinder 
  {
    typedef P                       coord_type;
    typedef point_traits<P>         pt;
    typedef algebraic_primitives<P> ap;
    typedef typename pt::component_type real;
  private:
    GrAL::line<P> axis;
    double  radius;
  public:
    infinite_cylinder(GrAL::line<P> const& ax, real r)
      : axis(ax), radius(r) 
    {
      axis.normalize();
    }

    bool inside(coord_type const& X) const
    {
      coord_type Xp = X - axis.p0();
      return ap::squared_norm_2(Xp - ap::dot(axis.dir(), Xp)*axis.dir()) 
	<= radius*radius; 
    }
    bool operator()(coord_type const& X) const { return inside(X);}
  };

} // namespace GrAL

#endif
