#ifndef NMWR_GB_GENERAL_COORDS_TEMPLATES_H
#define NMWR_GB_GENERAL_COORDS_TEMPLATES_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include <iostream.h>
#include "Geometry/point-traits.h"

#ifndef NO_COORD_SPECIAL

#include "Geometry/coords_2_special.h"
#include "Geometry/coords_3_special.h"

#endif

#include "Geometry/algebraic-primitives.h"

//----------------------------------------------------------------
//
//  class coord<N>;
//
//  A general class template for small geometric points.
//  The only template parameter is currently the dimension N.
//  This class could as well be parameterized over the type
//  of the components, now fixed to double. Default template 
//  parameters offer a clean way to add this generality later
//  without having to change code elsewhere.
//
//  Features:
//   * 1-based indexing with [] and ()
//   * all algebraic operators: 
//      R^N x R^N -> R^N :+=, -=, +, - 
//      R^N       -> R^N : unary -
//      R^N x R   -> R^N
//      R   x R^N -> R^N : *=, /=, *, /
//  There is NO operator* for the dot-product due to non-associativity!!
//   
//
//  There exist specializations for N=2 and N=3.
//  They can be disabled by
//  #define NO_COORD_SPECIAL 1
//
//  This class template can work together with the matrix<M.N> template.
//
//----------------------------------------------------------------

typedef double coord_N_component;

template<unsigned N>
class coordN {
public:
  typedef double component;
  typedef unsigned index;
  typedef coordN<N> self;

  coordN() {}
  coordN(const component& x)  { for(index i=0; i<N; i++) X[i] = x;}
  coordN(const component Y[]) { for(index i=0; i<N; i++) X[i] = Y[i];}
  coordN(const self& rhs)     { for(index i=0; i<N; i++) X[i] = rhs.X[i];}
  ~coordN() {}

  const component& operator[](index i) const { return X[--i];}
        component& operator[](index i)       { return X[--i];}
  const component& operator()(index i) const { return X[--i];}
        component& operator()(index i)       { return X[--i];}

  self& operator+=(const self& rhs) { for(index i=0; i<N; i++) X[i] += rhs.X[i]; return *this; }
  self& operator-=(const self& rhs) { for(index i=0; i<N; i++) X[i] -= rhs.X[i]; return *this; }
  self& operator*=(const component& rhs) { for(index i=0; i<N; i++) X[i] *= rhs; return *this; }
  self& operator/=(const component& rhs) { for(index i=0; i<N; i++) X[i] /= rhs; return *this; }

  static self origin() { return self(component(0));}
  static self Origin() { return self(component(0));}
  static index dim()   { return N;}
private:
  component X[N];
};



template<unsigned N>
bool operator==(const coordN<N>& lhs, const coordN<N> rhs)
{ for(unsigned int i=1; i<=N; i++) if (lhs[i]!=rhs[i]) return false;
 return true; }

template<unsigned N>
inline coordN<N> operator+(const coordN<N>& lhs, const coordN<N>& rhs)
{ coordN<N> tmp(lhs); return (tmp+= rhs);}

template<unsigned N>
inline coordN<N> operator-(const coordN<N>& lhs, const coordN<N>& rhs)
{ coordN<N> tmp(lhs); return(tmp-= rhs);}

template<unsigned N>
inline coordN<N> operator*(const coordN<N>& lhs, const coord_N_component& rhs)
{ coordN<N> tmp(lhs); return (tmp *= rhs);}

template<unsigned N>
inline coordN<N> operator*( const coord_N_component& lhs, const coordN<N>& rhs)
{ return rhs*lhs;}

template<unsigned N>
inline coordN<N> operator/(const coordN<N>& lhs, const coord_N_component& rhs)
{ coordN<N> tmp(lhs); return (tmp /= rhs); }

// unary -
template<unsigned N>
inline coordN<N> operator-(const coordN<N>& rhs)
{ coordN<N> tmp(rhs); return(tmp *= -1);}


template<unsigned N>
inline ostream& operator<<(ostream& out, const coordN<N>& P)
{
  for(unsigned i = 1; i<= N-1; i++)
    out <<  P[i] << ' ';
  out << P[N]; // no blank after last number
  return out;
}

template<unsigned N>
inline istream& operator>>(istream& in, coordN<N>& P)
{
  for(unsigned i = 1; i<= N; i++)
    in >> P[i];
  return in;
}


template<unsigned N>
struct point_traits_for_coordN {
  typedef coordN<N> Ptype;
  typedef typename Ptype::component      component_type;
  typedef component_type                 value_type;

  static int LowerIndex(const Ptype&) { return 1;}
  static int UpperIndex(const Ptype&) { return N;}
  static int Dimension(const Ptype&)  { return N;}
  static int Dim      (const Ptype&)  { return N;}

  static int Dimension()                 { return N;}
  static int Dim      ()                 { return N;}

  static void ConstructWithDim(unsigned, Ptype&) {}
  //static fixed_size_tag point_size_tag() { return fixed_size_tag();}

  static Ptype Origin()         { return Ptype(0.0);}
  static Ptype Origin(unsigned) { return Ptype(0.0);}

  static component_type  x(const Ptype& p) {return p[1];}
  static component_type& x(      Ptype& p) {return p[1];}
  static component_type  y(const Ptype& p) {return p[2];}
  static component_type& y(      Ptype& p) {return p[2];}
  static component_type  z(const Ptype& p) {return p[3];}
  static component_type& z(      Ptype& p) {return p[3];}

  //static string name() { return "coordN<N=" + make_string(N) +">";} 
};

// specialization for N=2 ( no reference for z() - functional)
struct point_traits_for_coordN_2 {
  typedef coordN<2> Ptype;
  typedef Ptype::component       component_type;
  typedef component_type                  value_type;
  typedef tag2D                           dimension_tag;

  static int LowerIndex(const Ptype&) { return 1;}
  static int UpperIndex(const Ptype&) { return 2;}
  static int Dimension(const Ptype&)  { return 2;}
  static int Dim      (const Ptype&)  { return 2;}

  static int Dimension()                 { return 2;}
  static int Dim()                       { return 2;}
  static void ConstructWithDim(unsigned, Ptype&) {}
  //static fixed_size_tag point_size_tag() { return fixed_size_tag();}

  static Ptype Origin()         { return Ptype(0.0);}
  static Ptype Origin(unsigned) { return Ptype(0.0);}

  static component_type  x(const Ptype& p) {return p[1];}
  static component_type& x(      Ptype& p) {return p[1];}
  static component_type  y(const Ptype& p) {return p[2];}
  static component_type& y(      Ptype& p) {return p[2];}
  static component_type  z(const Ptype&  ) {return 0;}

  static const char* name() { return "coord<2>"; }
};

template<unsigned N>
struct point_traits<coordN<N> > :  public point_traits_for_coordN<N> {};

/*
struct point_traits<coordN<2> > : public point_traits_for_coordN_2  {};
struct point_traits<coordN<3> > : public point_traits_for_coordN<3> {};
struct point_traits<coordN<4> > : public point_traits_for_coordN<4> {};
struct point_traits<coordN<5> > : public point_traits_for_coordN<5> {};
*/


// total specializations, could be made partial in
// algebraic-primitives.
// these should really be elsewhere, because not everyone
// wants to use them.
struct dimension_dependent_primitives<coordN<3> >
  : public dimension_dependent_primitives_3d<coordN<3> > {};

template<>
struct dimension_dependent_primitives<coordN<2> >
  : public dimension_dependent_primitives_2d<coordN<2> > {};


#endif
