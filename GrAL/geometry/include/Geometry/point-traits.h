#ifndef NMWR_GB_POINT_TRAITS_H
#define NMWR_GB_POINT_TRAITS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Config/compiler-config.h"
#include "Utility/pre-post-conditions.h"

// must be specialized
template<class P>
struct point_traits;

struct fixed_dimension_tag    {};
struct variable_dimension_tag {};

struct tag1D : public fixed_dimension_tag {};
struct tag2D : public fixed_dimension_tag {};
struct tag3D : public fixed_dimension_tag {};
struct tag4D : public fixed_dimension_tag {};

/* template<unsigned N>
struct dim_tag {};

typedef dim_tag<1> tag1D;
*/

template<class REAL>
struct real_point_traits { 
  typedef REAL Ptype;
  typedef tag1D dimension_tag;
  typedef REAL component_type;
  typedef component_type value_type;

  static unsigned Dim(const Ptype&) { return 1;}
  static unsigned Dim() { return 1;}
  
  static Ptype Origin(unsigned) { return REAL(0.0);}
  static Ptype Origin()         { return REAL(0.0);}
  static component_type  x(const Ptype& p) {return p;}
  static component_type& x(      Ptype& p) {return p;}
  static component_type  y(const Ptype&) {return component_type(0.0);}
  static component_type  z(const Ptype&) {return component_type(0.0);}
};

template<>
struct point_traits<float> 
  : public real_point_traits<float> {};

template<>
struct point_traits<double> 
  : public real_point_traits<double> {};


//----------------------------------------------------------------
//
// void assign_point(P & p, Q const& q);
//
// generic assignment operation
// semantics: p = q
// This could also be provided by a member template:
//  template<class Q>
//  P::operator=(Q const&);
//
//----------------------------------------------------------------

template<class P, class Q>
inline void assign_point(P & p, Q const& q) 
{
 typedef point_traits<P> ptP;
 typedef point_traits<Q> ptQ;
 // if variable dimension: adapt dim of p, else this is a no-op.
 ptP::ConstructWithDim(ptQ::Dim(q), p); 

 int iq = ptQ::LowerIndex(q); 
 int ip = ptP::LowerIndex(p);  
 for(; iq <= ptQ::UpperIndex(q); ++ip, ++iq)
   p[ip] = q[iq]; 
}  

// P = [b,e)
template<class P, class T>
inline void assign_point(P & p, T const* b, T const* e) 
{
  REQUIRE( (e-b > 0), "invalid pointer range: b = " << b << "; e = " << e << "\n",1);
  typedef point_traits<P> ptP;

  // if variable dimension: adapt dim of p, else this is a no-op.
  ptP::ConstructWithDim(e-b,p);
  
  int ip = ptP::LowerIndex(p);  
  for(; b < e; ++ip, ++b)
    p[ip] = *b; 
}  

template<class P>
void assign_point(P& p, P const& q) { p = q;}

template<class P>
void assign_point(float& p, P const& q) 

{ p = point_traits<P>::x(q);}

void assign_point(float& p, float q) { p = q;}
#endif
