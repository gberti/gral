#ifndef SIMBIO_GRIDQUALITY_MAPPINGS_H
#define SIMBIO_GRIDQUALITY_MAPPINGS_H

// some mappings for producing distorted elements.

#include "Geometry/point-traits.h"
#include <cmath>

template<class P>
class identity3d {
public:
  typedef P result_type;  
  typedef point_traits<P> pt;

 
  result_type operator()(double x, double y, double z) const 
    { 
      P p; pt::ConstructWithDim(3,p);
      pt::x(p) = x;
      pt::y(p) = y;
      pt::z(p) = z;
      return p;
    }

};


template<class P>
class warping3d {
  double f;
public:
  typedef P result_type;  
  typedef point_traits<P> pt;

  warping3d(double fac = 0.0) : f(2*M_PI*fac) {} 

  result_type operator()(P const& p) const 
    { return (*this)(pt::x(p), pt::y(p), pt::z(p));}

  result_type operator()(double x, double y, double z) const 
    { 
      P p; pt::ConstructWithDim(3,p);
      double xx = x -0.5;
      double yy = y -0.5;
      pt::x(p) = cos(f*z)*xx - sin(f*z)*yy;
      pt::y(p) = sin(f*z)*xx + cos(f*z)*yy;
      pt::z(p) = z;
      return p;
    }

};



#endif
