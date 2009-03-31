#ifndef NMWR_GB_IDENTITY_2D_H
#define NMWR_GB_IDENTITY_2D_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/point-traits.h"


template<class P2D>
struct identity2d {
public:
  typedef P2D        coord_type;
  typedef coord_type result_type;
  typedef coord_type argument_type;
  typedef typename GrAL::point_traits<P2D>::component_type real;
  
  static unsigned dim_of_image()  { return 2;}
  static unsigned dim_of_domain() { return 2;}

  coord_type operator()(real x, real y) const {
    return coord_type(x,y);
  }

  const coord_type& operator()(const coord_type& x) const { return x;}
};




#endif
