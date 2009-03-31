#ifndef GRAL_GB_GEOMETRIC_PRIMITIVES_H
#define GRAL_GB_GEOMETRIC_PRIMITIVES_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
 

#include "Geometry/algebraic-primitives.h"
#include "Utility/pre-post-conditions.h"
#include "Container/integer-iterator.h"

// FIXME: workaround for missing <limits>
#include <boost/limits.hpp>

namespace GrAL {

template<class Geom>
struct geom_traits {};


  /*! \defgroup geometric_primitives Geometric primitives

  */



  /*! \brief Point in space

      \ingroup geometric_primitives
  */
  template<class P>
  class space_point {
    typedef space_point<P> self;
  public:
    typedef P coord_type;
    typedef point_traits<P> pt;
    typedef typename pt::component_type scalar_type;
    
  private:
    coord_type  p[1];
  public:
    space_point() {}
    space_point(coord_type const& P0)
    { p[0]=P0;}
    coord_type const& p0() const { return p[0];}
    coord_type const& operator[](int i) const { return p[i];}
    coord_type const& operator()(int i) const { return p[i];}
    
    int space_dimension() const { return pt::Dim(p[0]);}
    
    typedef integer_iterator<int, self>     VertexIterator;
    VertexIterator FirstVertex() const { return VertexIterator(0);}
    VertexIterator EndVertex()   const { return VertexIterator(1);}
    
    typedef coord_type const*  geom_vertex_iterator;
    geom_vertex_iterator begin_vertex() const { return p;}
    geom_vertex_iterator end_vertex()   const { return p+1;}
  };



  /*! \brief Segment in space

      \ingroup geometric_primitives
  */
template<class P>
class segment {
  typedef segment<P> self;
public:
  typedef P                           coord_type;
  typedef point_traits<P>             pt;
  typedef algebraic_primitives<P>     ap;
  typedef typename pt::component_type scalar_type;
private:
  coord_type  p[2];
public:
  segment() {}
  segment(coord_type const& P0, coord_type const& P1)
  { p[0]=P0; p[1]=P1;}
  coord_type const& p0() const { return p[0];}
  coord_type const& p1() const { return p[1];}

  coord_type const& operator[](int i) const { return p[i];}
  coord_type const& operator()(int i) const { return p[i];}
  coord_type operator()(scalar_type t) const { return (1-t)*p[0] + t*p[1];}

  int space_dimension() const { return pt::Dim(p[0]);}

  typedef integer_iterator<int, self>     VertexIterator;
  VertexIterator FirstVertex() const { return VertexIterator(0);}
  VertexIterator EndVertex()   const { return VertexIterator(2);}

  typedef coord_type const*  geom_vertex_iterator;
  geom_vertex_iterator begin_vertex() const { return p;}
  geom_vertex_iterator end_vertex()   const { return p+2;}

  scalar_type project(coord_type const& q) const { return ap::dot(q-p0(), p1()-p0())/squared_length(); }
  scalar_type squared_length()   const { return ap::squared_norm_2(p1()-p0()); }
  scalar_type length()           const { return sqrt(squared_length());}
  coord_type  nearest(coord_type const& q) const { 
    scalar_type t = project(q);
    return (t <= 0 ? p0() : (t >=1 ? p1() : this->operator()(t)));
  }

};



  /*! \brief Triangle in space

      \ingroup geometric_primitives
  */

template<class P>
class triangle {
  typedef triangle<P> self;
public:
  typedef P coord_type;
  typedef point_traits<P> pt;
  typedef typename pt::component_type scalar_type;

private:
  coord_type  p[3];
public:
  triangle() {}
  triangle(coord_type const& P0, 
	   coord_type const& P1,
	   coord_type const& P2)
    { p[0]=P0; p[1]=P1;p[2]=P2;}

  coord_type const& operator[](int i) const { return p[i];}
  coord_type const& operator()(int i) const { return p[i];}
  coord_type const& p0() const { return p[0];}
  coord_type const& p1() const { return p[1];}
  coord_type const& p2() const { return p[2];}

  int space_dimension() const { return pt::Dim(p[0]);}

  typedef integer_iterator<int, self>     VertexIterator;
  VertexIterator FirstVertex() const { return VertexIterator(0);}
  VertexIterator EndVertex()   const { return VertexIterator(3);}

  typedef coord_type const*  geom_vertex_iterator;
  geom_vertex_iterator begin_vertex() const { return p;}
  geom_vertex_iterator end_vertex()   const { return p+3;}
};


  /*! \brief Tetrahedron in space

      \ingroup geometric_primitives
  */

template<class P>
class tetrahedron {
  typedef tetrahedron<P> self;
public:
  typedef P coord_type;
  typedef point_traits<P> pt;
  typedef typename pt::component_type scalar_type;

private:
  coord_type  p[4];
public:
  tetrahedron() {}
  tetrahedron(coord_type const& P0, 
	      coord_type const& P1,
	      coord_type const& P2,
	      coord_type const& P3)
    { p[0]=P0; p[1]=P1;p[2]=P2;p[3]=P3;}

  coord_type const& operator[](int i) const { return p[i];}
  coord_type const& operator()(int i) const { return p[i];}

  int space_dimension() const { return pt::Dim(p[0]);}

  typedef integer_iterator<int, self>  VertexIterator;
  VertexIterator FirstVertex() const { return VertexIterator(0);}
  VertexIterator EndVertex()   const { return VertexIterator(4);}

  typedef coord_type const*  geom_vertex_iterator;
  geom_vertex_iterator begin_vertex() const { return p;}
  geom_vertex_iterator end_vertex()   const { return p+4;}
};



  /*! \brief Ray in space

      \ingroup geometric_primitives
  */

template<class P> class ray {
public:
  typedef P coord_type;
  P p_0, dir_;
  typedef point_traits<P> pt;
  typedef typename pt::component_type scalar_type;
  typedef scalar_type                 real;
public:
  ray() {}
  ray(P const& pp0, P const& ddir) : p_0(pp0), dir_(ddir) {}
  coord_type const& p0() const { return p_0;}  
  coord_type        p1() const { return p_0 + dir_;}
  coord_type const& dir() const { return dir_;}
  coord_type operator()(real t) const { return p_0 + t*dir_;}
};


  /*! \brief Line in space

      \ingroup geometric_primitives
  */

template<class P> class line
{ 
public:
  typedef P coord_type;
  P p_0, dir_;

  typedef algebraic_primitives<coord_type> ap;
public:
  line() {}
  line(P const& pp0, P const& ddir) : p_0(pp0), dir_(ddir) {}
  line(ray<P> const& r)     : p_0(r.p0()), dir_(r.dir()) {}
  line(segment<P> const& s) : p_0(s.p0()), dir_(s.p1() -s.p0()) {}
  coord_type const& p0() const { return p_0;}  
  coord_type const& dir() const { return dir_;}
  coord_type operator()(double t) const { return p_0 + t*dir_;}
  
  void normalize() { ap::normalize(dir_);}
};


  /*! \brief Hyperplane   

      \ingroup geometric_primitives
   */
  template<class P>  class hyper_plane {
    typedef P coord_type;
    P p0, n;
    typedef algebraic_primitives<coord_type> ap;
    typedef point_traits<coord_type>         pt;
    typedef typename pt::scalar_type              scalar_type;
    typedef typename real_type<scalar_type>::type real_type;
  public:
    hyper_plane() {}
    hyper_plane(P const& pp0, P const& nn) : p0(pp0), n(nn) { ap::normalize(n);}
    //! \brief Constructor for 3D case only
    hyper_plane(P const& pp0, P const& a1, P const& a2) 
      : p0(pp0), n(ap::vectorproduct(a1,a2)) 
    { ap::normalize(n);}

    real_type oriented_distance(P const& x) const { return ap::dot(n,x-p0);}
  };

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
  typedef ray<P>    const& const_ref;
  static coord_type const& p0 (const_ref T) { return T.p0();}
  static coord_type const& dir(const_ref T) { return T.dir();}

};

template<class P>
struct geom_traits<line<P> > {
  typedef typename line<P>::coord_type coord_type;
  typedef line<P>   const& const_ref;
  static coord_type const& p0 (const_ref T) { return T.p0();}
  static coord_type const& dir(const_ref T) { return T.dir();}

};
  

/*! Calculate normal from 3-point plane representation 

      \ingroup geometric_primitives
   */
template<class Triangle>
inline
typename geom_traits<Triangle>::coord_type
plane_normal(Triangle const& T)
{
  typedef geom_traits<Triangle> tt;
  typedef algebraic_primitives<typename tt::coord_type> ap;
  return  ap::vectorproduct(tt::p1(T) - tt::p0(T), 
			    tt::p2(T) - tt::p0(T));
}

/*! Calculate normal from 3-point plane representation 

      \ingroup geometric_primitives

   */
template<class Triangle>
inline
typename geom_traits<Triangle>::coord_type
plane_normed_normal(Triangle const& T)
{ 
  typedef geom_traits<Triangle> tt;
  typedef algebraic_primitives<typename tt::coord_type> ap;
  return (ap::normalization(plane_normal(T)));
}



/*! \brief Class for intersecting a segment and a triangle in 3D
      
      \ingroup geometric_primitives

     \todo: optimize by introducing caches for calculated quantities.
   */

template<class Segment, class Triangle>
class intersection_segment_triangle {
  typedef geom_traits<Segment>  st;
  typedef geom_traits<Triangle> tt;
  typedef typename st::coord_type coord_type; 
  typedef algebraic_primitives<coord_type> ap;
  // assume tt::coord_type is the same
  typedef point_traits<coord_type>         pt;
  typedef typename pt::component_type      scalar;
  typedef typename real_type<scalar>::type real;

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
      ap::solve3(tt::p1(T) - tt::p0(T),
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
      
      \ingroup geometric_primitives

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
  typedef typename pt::component_type       scalar;
  typedef typename real_type<scalar>::type  real;

  typename rt::const_ref R;
  typename tt::const_ref T;
  real t;
  bool t_defined;

public: 
  intersection_ray_triangle(Ray const& R_, Triangle const& T_)
    : R(R_), T(T_), t_defined(false) 
    {}

  real eps() const { 
    return std::numeric_limits<real>::epsilon();
  }

  bool ray_intersects_plane() {
    coord_type n = plane_normal(T);

    // if direction of ray points away from plane, no intersection
    if(ap::dot(n, rt::dir(R)) * ap::dot(n, rt::p0(R) -tt::p0(T)) >= 0)
      return false;
    else
      return true;
    /*
    real       d = ap::dot(ap::normalization(n),
			   ap::normalization(rt::dir(R)));
    // is eps the correct quantity?
    // case fabs(d) < eps is considered parallel to plane
    real abs_d = ( d >= real(0) ? d : -d);
    return ( abs_d >= eps());
    */
  }

  bool ray_intersects_triangle() {
    // check whether ray is (to) parallel to triangle plane

    // solve p0(T) + r*(p1(T)-p0(T)) + s*(p2(T)-p0(T)) 
    //    =  p0(R) + t* dir(R)   for (r,s,t)
    if(! ray_intersects_plane())
      return false;
    coord_type rst; pt::ConstructWithDim(3,rst);
    ap::solve3(tt::p1(T) - tt::p0(T),
	       tt::p2(T) - tt::p0(T), 
	       - rt::dir(R),
	       rst,
	       rt::p0(R) - tt::p0(T));
      t = pt::z(rst); t_defined = true;
      real r = pt::x(rst), s = pt::y(rst);
      return (t>= 0 && r >= 0 && s >= 0 && s+r <= 1);
  }
  //  bool lines_does_intersect_plane();
  real ray_intersection() {
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


/*! Absolute distance between a point and a segment
      
   \ingroup geometric_primitives
*/
template<class P> 
inline typename segment<P>::scalar_type
distance(space_point<P> const& p, segment<P> const& s) 
{ 
  typedef algebraic_primitives<P> ap;
  return ap::distance(p.p0(), s.nearest(p.p0())); 
}

/*! \brief Absolute distance between a point and a segment
      
   \ingroup geometric_primitives
*/
template<class P> 
inline typename segment<P>::scalar_type
distance(segment<P> const& s, space_point<P> const& p)
{ return distance(s,p);}

/*! \brief Signed distance between a point and a hyperplane
      
   \ingroup geometric_primitives
*/
template<class P>
inline typename space_point<P>::scalar_type
oriented_distance(hyper_plane<P> const& pl, space_point<P> const& p)
{ return pl.oriented_distance(p);}

  /*! \brief Absolute distance between a point and a hyperplane
      
  \ingroup geometric_primitives
  */
  template<class P>
  inline typename space_point<P>::scalar_type
  distance(hyper_plane<P> const& pl, space_point<P> const& p)
  { return fabs(distance(pl,p)); }




} // namespace GrAL 

#endif




