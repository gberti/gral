#ifndef GRAL_GB_GEOMETRIC_PRIMITIVES_H
#define GRAL_GB_GEOMETRIC_PRIMITIVES_H

// $LICENSE 

#include "Geometry/algebraic-primitives.h"
#include "Utility/pre-post-conditions.h"

// should use C++ <limits>
#include <limits.h>

template<class Geom>
struct geom_traits {};

   
template<class P>
class segment {
  typedef P coord_type;
  typedef coord_type const& const_ref;
  //   coord_type const& p_0, p_1;
  // coord_type  p_0, p_1;
  const_ref  p_0, p_1;
public:
  segment(coord_type const& P0, coord_type const& P1)
    : p_0(P0), p_1(P1) {}
  coord_type const& p0() const { return p_0;}
  coord_type const& p1() const { return p_1;}
};

template<class P>
class triangle {
  typedef P coord_type;
  typedef coord_type const& const_ref;
  // coord_type const& p_0, p_1, p_2;
  // coord_type  p_0, p_1, p_2;
 const_ref  p_0, p_1, p_2;
public:
  triangle(coord_type const& P0, 
	   coord_type const& P1,
	   coord_type const& P2)
    : p_0(P0), p_1(P1), p_2(P2) {}
  coord_type const& p0() const { return p_0;}
  coord_type const& p1() const { return p_1;}
  coord_type const& p2() const { return p_2;}
};


template<class P> class ray {
public:
  typedef P coord_type;
  P p_0, dir_;
public:
  ray(P const& pp0, P const& ddir) : p_0(pp0), dir_(ddir) {}
  coord_type const& p0() const { return p_0;}  
  coord_type const& dir() const { return dir_;}
  coord_type operator()(double t) const { return p_0 + t*dir_;}
};

/*
template<class P> class line {};
*/

template<class P>
struct geom_traits<segment<P> > {
  typedef segment<P> segment_t;
  typedef typename segment_t::coord_type coord_type;
  // typedef segment_t const_ref;
  typedef segment_t const& const_ref;
  static coord_type const& p0(const_ref S) { return S.p0();}
  static coord_type const& p1(const_ref S) { return S.p1();}
};

template<class P>
struct geom_traits<triangle<P> > {
  typedef typename triangle<P>::coord_type coord_type;
  // typedef triangle<P>    const_ref;
  typedef triangle<P>   const & const_ref;
  static coord_type const& p0(const_ref T) { return T.p0();}
  static coord_type const& p1(const_ref T) { return T.p1();}
  static coord_type const& p2(const_ref T) { return T.p2();}
};

template<class P>
struct geom_traits<ray<P> > {
  typedef typename ray<P>::coord_type coord_type;
  // typedef triangle<P>    const_ref;
  typedef ray<P>    const& const_ref;
  static coord_type const& p0 (const_ref T) { return T.p0();}
  static coord_type const& dir(const_ref T) { return T.dir();}

};
  

/*! Calculate normal from 3-point plane representation 
   */
template<class Triangle>
inline
typename geom_traits<Triangle>::coord_type
plane_normal(Triangle const& T)
{
  typedef geom_traits<Triangle> tt;
  typedef algebraic_primitives<typename tt::coord_type> ap;
  return (ap::normalization
	  (ap::vectorproduct(tt::p1(T) - tt::p0(T), 
			     tt::p2(T) - tt::p0(T))));
}


/*! Class for intersecting a segment and a triangle in 3D
      
     \todo: optimize by introducing caches for calculated quantities.
   */

template<class Segment, class Triangle>
class intersection_segment_triangle {
  typedef geom_traits<Segment>  st;
  typedef geom_traits<Triangle> tt;
  typedef typename st::coord_type coord_type; 
  typedef algebraic_primitives<coord_type> ap;
  // assume tt::coord_type is the same
  typedef point_traits<coord_type> pt;
  typedef typename pt::component_type scalar;
  typedef scalar                      real;// FIXME: should be scalar_traits::real

  typename st::const_ref S;
  typename tt::const_ref T;
  real t;
  bool t_defined;

public: 
  intersection_segment_triangle(Segment const& S_, Triangle const& T_)
    : S(S_), T(T_), t_defined(false) 
    {}

  bool segment_intersects_plane() {
    coord_type n = plane_normal(T);
    real       d = ap::dot(n, tt::p0(T));
    return ( (ap::dot(st::p0(S), n)-d) 
	     *(ap::dot(st::p1(S), n)-d)  < 0);
  }

  bool segment_intersects_triangle() {
    if(segment_intersects_plane()) {
      coord_type rst; pt::ConstructWithDim(3,rst);
      // solve p0(T) + r*(p1(T)-p0(T)) + s*(p2(T)-p0(T)) 
      //    =  p0(S) + t*(p1(S)-p0(S))  for (r,s,t)
      ap::solve(tt::p1(T) - tt::p0(T),
		tt::p2(T) - tt::p0(T), 
		st::p0(S) - st::p1(S),
		rst,
		st::p0(S) - tt::p0(T));
      t = pt::z(rst); t_defined = true;
      real r = pt::x(rst), s = pt::y(rst);
      return (r >= 0 && s >= 0 && s+r <= 1);
    }
    else // no intersection with plane => none with triangle
      return false;
  }
  //  bool lines_does_intersect_plane();

  coord_type intersection() { 
    REQUIRE(segment_intersects_plane(), "no intersection!\n",1);
    if(! t_defined)
      segment_intersects_triangle(); // calculates t      
    return st::p0(S) + t * (st::p1(S) - st::p0(S));
  }
};




/*! Class for intersecting a ray and a triangle in 3D
      
     \todo: optimize by introducing caches for calculated quantities.
   */

template<class Ray, class Triangle>
class intersection_ray_triangle {
  typedef geom_traits<Ray>      rt;
  typedef geom_traits<Triangle> tt;
  typedef typename rt::coord_type coord_type; 
  typedef algebraic_primitives<coord_type> ap;
  // assume tt::coord_type is the same
  typedef point_traits<coord_type> pt;
  typedef typename pt::component_type scalar;
  typedef scalar                      real;// FIXME: should be scalar_traits::real

  typename rt::const_ref R;
  typename tt::const_ref T;
  real t;
  bool t_defined;

public: 
  intersection_ray_triangle(Ray const& R_, Triangle const& T_)
    : R(R_), T(T_), t_defined(false) 
    {}

  real eps() const { /* return numeric_limits<real>::epsilon();*/
    return FLT_EPSILON;}

  bool line_intersects_plane() {
    coord_type n = plane_normal(T);

    // if direction of ray points away from plane, no intersection
    if(ap::dot(n, rt::dir(R)) * ap::dot(n, rt::p0(R) -tt::p0(T)) > 0)
      return false;
    // not correct: ray migth lie in plane.

    real       d = ap::dot(ap::normalization(n), ap::normalization(rt::dir(R)));
    //  is eps the correct quantity?
    return ( fabs(d) >= eps());
  }

  bool ray_intersects_triangle() {
    // check whether ray is (to) parallel to triangle plane

    // solve p0(T) + r*(p1(T)-p0(T)) + s*(p2(T)-p0(T)) 
    //    =  p0(R) + t* dir(R)   for (r,s,t)
    if(! line_intersects_plane())
      return false;
    coord_type rst; pt::ConstructWithDim(3,rst);
    ap::solve(tt::p1(T) - tt::p0(T),
	      tt::p2(T) - tt::p0(T), 
	      - rt::dir(R),
	      rst,
	      rt::p0(R) - tt::p0(T));
      t = pt::z(rst); t_defined = true;
      real r = pt::x(rst), s = pt::y(rst);
      return (t>= 0 && r >= 0 && s >= 0 && s+r <= 1);
  }
  //  bool lines_does_intersect_plane();
  double ray_intersection() {
    //REQUIRE(ray_intersects_plane(), "no intersection!\n",1);
    if(! t_defined)
      ray_intersects_triangle(); // calculates t      
    return t;
  }

  coord_type intersection() { 
    // REQUIRE(ray_intersects_plane(), "no intersection!\n",1);
    if(! t_defined)
      ray_intersects_triangle(); // calculates t      
    return rt::p0(R) + t * rt::dir(R);
  }
};

#endif




