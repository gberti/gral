#ifndef NMWR_GB_POINT_H
#define NMWR_GB_POINT_H

#include "Config/compiler-config.h"

#include <iostream>

#include <cstdlib>

// error handling macro
#include "Utility/pre-post-conditions.h"




/****************************************************************
*								*
*		       n-dimensional Vector      		*
*								*
****************************************************************/


class point
{
protected:
 int n;
 double* x;

  // const bool zero_init = true;
  // const bool no_init = false;

public:

  // Constructors
  enum { no_init = 0, zero_init = 1}; 
  point() : n(0) { x = 0;}
  
  point(int n1, int init)  // generate point of dim. n1, if init == zero_init : initialize with zero
    : n(n1) {x= new double[n]; if(init == zero_init) for(int i=0;i<=n-1;i++) x[i]=0.0;}
  explicit point(int n1) : n(n1), x(new double[n]) {}
  explicit point(const double x1);
  point(const double x, const double y);
  point(const double x, const double y, const double z);
  point(const double a[], int nn);
  point(const point& v);			// copy constructor
  ~point() {if (n > 0) delete [] x;}

  int dim() const { return n;}

  // Assignment operators

  point& operator  = ( const point& v );	// assignment of a point
  point& operator += ( const point& v );	// incrementation by a point
  point& operator -= ( const point& v );	// decrementation by a point
  point& operator *= ( const double d );	// multiplication by a constant
  point& operator /= ( const double d );	// division by a constant
  double& operator [] ( int i);		        // indexing, assignment poss.
  const double& operator [] ( int i) const;     // indexing (const)

  // special functions

  double length() const;			// length of a point
  double length2() const;			// squared length of a point
  point normalize() const;			// normalize a point

  friend point combine(const point& a, const point& b); // workaround

  friend point operator - (const point& a);			    // -a
  friend point operator + (const point& a, const point& b);	    // a + b
  friend point operator - (const point& a, const point& b);	    // a - b
  friend point operator * (const point& a, const double d);     // a * d
  friend point operator * (const double d, const point& a);     // d * a
  friend double operator * (const point& a, const point& b);    // dot product
  friend point operator / (const point& a, const double d);	    // a / d
  friend std::ostream& operator << (std::ostream& s, const point& v);    
  friend std::istream& operator >> (std::istream& s, point& v);	    
  friend void swap(point& a, point& b);			    // interchange a and b
};


inline double& point::operator [] ( int i) {
  REQUIRE((i >= 1 && i <= n),
	  "point [] operator: illegal access; index = " << i << '\n',1);
  return x[--i];
}

inline const double& point::operator []   ( int i) const {
  REQUIRE((i >= 1 && i <= n),
	  "point [] operator: illegal access; index = " << i << '\n',1);
  return x[i-1];
}


inline bool IsNullvector(const point& p)
{ 
  for(int i=1;i<= p.dim(); i++)
    if(p[i] != 0.0)
      return false;
  return true;
}

// special vectors

inline point unitvector(int dim, int i)
{ 
  point p(dim,point::zero_init); 
  p[i] = 1.0;
  return p;
}


inline point origin(int dim) { return point(dim,point::zero_init);}

/* or:
class unitvector: public point {
private:
  int unit_comp;
public:
  unitvector(int dim, int i) : point(0) {n=dim;}
  double operator[](int k) { return (k==unit_comp? 1.0 :0.0);}
};
*/

#include "Geometry/point-traits.h"

template<>
struct point_traits<point> 
  : public point_traits_base<point> 
{
  typedef point                  Ptype;
  typedef double                 component_type;
  typedef variable_dimension_tag dimension_tag;


  static unsigned Dim      (const Ptype& p) { return p.dim();}
  static int      LowerIndex(const Ptype&)   {return 1;}
  static int      UpperIndex(const Ptype& p) {return Dim(p);}
  static void     ConstructWithDim(int dim, Ptype& p) 
     { p = Ptype(dim,Ptype::no_init);}
  static Ptype    Origin(unsigned d) { return Ptype((int)d,Ptype::zero_init);}

  static component_type x(const Ptype& p)  { return p[1];}
  static component_type y(const Ptype& p)  { return (Dim(p) > 1 ? p[2] : 0.0);}
  static component_type z(const Ptype& p)  { return (Dim(p) > 2 ? p[3] : 0.0);}

    // ????
  static component_type& x(Ptype& p)  { return p[1];}
  static component_type& y(Ptype& p)  { return p[2];}
  static component_type& z(Ptype& p)  { return p[3];}
    // ????

  // problem with the reference version:
  // we can return a reference only if dim(p) is sufficent.
  // else: 1) run-time error
  //       2) return reference to static dummy 0.0
  //       3) use generally another name for ref-version.
  // 1) : problem that compiler takes non-const version if possible,
  // leading to failure of a correct program if one only wants to *read*
  // 2) have to ensure that the dummy always remains zero: i.e. we must
  // use a proxy here.
};

#include "Geometry/algebraic-primitives.h"

struct dimension_dependent_primitives<point> 
  :  public basic_algebraic_primitives<point> 
{
  typedef dimension_dependent_primitives_2d<point> prim2d;
  typedef dimension_dependent_primitives_3d<point> prim3d;

  static double det2(const point& p1, const point& p2) {
    REQUIRE((Dim(p1) == 2 && Dim(p2) == 2), 
	    "det2 called with dim(p,q) = " << Dim(p1) << "," << Dim(p2) << "!",1);
    return prim2d::det2(p1,p2);
  }
  static double det3(const point& p1, const point& p2, const point& p3) { 
    REQUIRE((Dim(p1) == 3 && Dim(p2) == 3 && Dim(p3) == 3), 
	    "det3 called with dim(p,q,r) = "
	    << Dim(p1) << "," << Dim(p2) << "," << Dim(p3) << "!",1);
    return prim3d::det3(p1,p2,p3);
  }

  static double signed_triangle_area(const point& p1, const point& p2, const point& p3) {
    if(Dim(p1) == 2) 
      return prim2d::signed_triangle_area(p1,p2,p3);
    else {
      ENSURE(false, "signed_triangle_area: not defined for dim > 2!",1);
      return 0.0;
    }
  }
  
  static double triangle_area(const point& p1, const point& p2, const point& p3)  {
    if (Dim(p1) == 2)
      return prim2d::triangle_area(p1,p2,p3);
    else if (Dim(p1) == 3)
      return prim3d::triangle_area(p1,p2,p3);
    else {
      ENSURE(false, "triangle_area: not implemented for dim > 3!",1);
      return 0.0;
    }
  }
  
  static point vectorproduct(const point& p1, const point& p2) {
    REQUIRE((Dim(p1) == 3 && Dim(p2) == 3),
	    "vectorproduct: called with dim(p,q) = "
	    << Dim(p1) << "," << Dim(p2) << "!",1);
    return prim3d::vectorproduct(p1,p2);
  }

  static point  normal_with_same_length(const point& p) {
    REQUIRE((Dim(p) == 2),"normal: called with dim(p) = " << Dim(p) <<"!",1);
    return prim2d::normal_with_same_length(p);
  }

  static point  normed_normal(const point& p) {
    REQUIRE((Dim(p) == 2),"normal: called with dim(p) = " << Dim(p) <<"!",1);
    return prim2d::normed_normal(p);
  }

};


//extern void test_algebraic_primitives(const point&);

#endif
