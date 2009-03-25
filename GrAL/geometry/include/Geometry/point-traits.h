#ifndef NMWR_GB_POINT_TRAITS_H
#define NMWR_GB_POINT_TRAITS_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"
#include "Utility/pre-post-conditions.h"
#include "Geometry/scalar-traits.h"

#include <iostream>

/*! \file
 */

namespace GrAL {

/*! To be specialized for concrete P

   \see real_point_traits
 */
template<class P>
struct point_traits {
  enum { is_specialised = 0 };
};

// TODO: tags system is a mess!

struct fixed_dimension_tag    {};
struct variable_dimension_tag {};

struct tag_unknown_dim {};
//struct tag_runtime_dim {};
struct tag1D : public fixed_dimension_tag {};
struct tag2D : public fixed_dimension_tag {};
struct tag3D : public fixed_dimension_tag {};
struct tag4D : public fixed_dimension_tag {};

template<unsigned N>
struct dim_tag {
  enum { dim = N};
  typedef fixed_dimension_tag dimension_tag;
};

template<> struct dim_tag<1> { typedef tag1D dimension_tag;};
template<> struct dim_tag<2> { typedef tag2D dimension_tag;};
template<> struct dim_tag<3> { typedef tag3D dimension_tag;};
template<> struct dim_tag<4> { typedef tag4D dimension_tag;};


template<class POINT>
struct point_traits_base 
{
  typedef tag_unknown_dim dimension_tag;
  typedef POINT  Ptype;
  enum { is_specialised = 1 };
};

// TODO: let all point_traits for fixed dimension types inherit this.
template<class POINT, unsigned DIM>
struct point_traits_fixed_dim 
  : public point_traits_base<POINT>
{
  typedef POINT Ptype;
  typedef dim_tag<DIM>                dimension_tag; 
  enum    { dimension = DIM};
  static unsigned Dim()             { return DIM;}
  static unsigned Dim(Ptype const&) { return DIM;}
  static void     ConstructWithDim(unsigned d, Ptype&)
    { REQUIRE(d == DIM, "Cannot construct: d = " << d << "; DIM = " << DIM,1);}
};



/*! \brief Point traits for scalar types
    
 */


template<class SCALAR>
struct scalar_point_traits : public point_traits_base<SCALAR> { 
  typedef SCALAR         Ptype;
  typedef tag1D          dimension_tag;
  typedef SCALAR         component_type;
  typedef component_type value_type;
  typedef SCALAR         scalar_type;
  typedef typename real_type<scalar_type>::type     real_type;

  enum { dimension = 1};
  static unsigned Dim(const Ptype&) { return 1;}
  static unsigned Dim() { return 1;}
  
  static Ptype Origin(unsigned) { return component_type(0);}
  static Ptype Origin()         { return component_type(0);}
  static component_type  x(const Ptype& p) {return p;}
  static component_type& x(      Ptype& p) {return p;}
  static component_type  y(const Ptype&) {return component_type(0);}
  static component_type  z(const Ptype&) {return component_type(0);}
};

template<>
struct point_traits<float> 
  : public scalar_point_traits<float> {};

template<>
struct point_traits<double> 
  : public scalar_point_traits<double> {};


template<>
struct point_traits<signed char> 
  : public scalar_point_traits<signed char> {};

template<>
struct point_traits<unsigned char> 
  : public scalar_point_traits<unsigned char> {};

template<>
struct point_traits<signed short> 
  : public scalar_point_traits<signed short> {};

template<>
struct point_traits<unsigned short> 
  : public scalar_point_traits<unsigned short> {};


template<>
struct point_traits<int> 
  : public scalar_point_traits<int> {};

template<>
struct point_traits<unsigned> 
  : public scalar_point_traits<unsigned> {};


/*! \brief Hook for indicating different array offset.

    \example
    \code
     class MyPoint {
       double X[3];    

       // offset 1, a la Fortran
       double operator[](int i) { return X[i-1];}
     };
     ...
     template<> struct GrAL::array_offset<MyPoint> { enum { value = 1}; };
     
     template<> struct GrAL::point_trait<MyPoint> 
       : public GrAL::point_traits_fixed_size_array<MyPoint, double, N> {}; 
    \endcode
 */
template<class ARRAY> struct array_offset { enum { value = 0 }; };

template<class ARRAY, class COMPONENT, unsigned DIM>
struct point_traits_fixed_size_array_base :
  public point_traits_base<ARRAY>
{
  typedef ARRAY          Ptype;
  typedef COMPONENT      component_type;
  typedef component_type value_type;
  typedef typename dim_tag<DIM>::dimension_tag dimension_tag;
  enum { dimension = DIM };

  static void ConstructWithDim(unsigned d, Ptype&)
    { REQUIRE(d <= DIM, "Cannot construct! d = " << d 
	      << ", DIM = " << DIM <<'\n',1);
    }
  static unsigned Dim(const Ptype&) { return DIM;}
  static unsigned Dim() { return DIM;}
  
  static int Offset() { return array_offset<ARRAY>::value; }
  static int LowerIndex()             { return Offset(); } //0;}
  static int LowerIndex(Ptype const&) { return Offset(); } //0;}
  static int UpperIndex()             { return Offset() + DIM-1;}
  static int UpperIndex(Ptype const&) { return Offset() + DIM-1;}


  static Ptype Origin() {
    Ptype p; 
    for(int i = LowerIndex(); i <= UpperIndex(); ++i) 
      p[i] = component_type(0.0);
    return p;
  }
  static Ptype Origin(unsigned) { return Origin();}

  // should i be in [LowerIndex(), UpperIndex()]  or in [0, DIM-1] ??
  static Ptype unit_vector(int i) {
    REQUIRE( LowerIndex() <= i && i <= UpperIndex(), "i=" << i << " ", 1);
    Ptype res = Origin();
    res[i] = 1;
    return res;
  }
  static Ptype unit_vector(int i, unsigned) { return unit_vector(i);}

  // should branch on DIM here.
  static component_type  x(const Ptype& p) {return p[Offset()+0];}
  static component_type& x(      Ptype& p) {return p[Offset()+0];}
  static component_type  y(const Ptype& p) {return p[Offset()+1];}
  static component_type& y(      Ptype& p) {return p[Offset()+1];}
  static component_type  z(const Ptype& p) {return p[Offset()+2];}
  static component_type& z(      Ptype& p) {return p[Offset()+2];}

};

template<class ARRAY, class COMPONENT, unsigned DIM>
struct point_traits_fixed_size_array :
  public point_traits_fixed_size_array_base<ARRAY,COMPONENT,DIM> {};


template<class ARRAY, class COMPONENT>
struct point_traits_fixed_size_array<ARRAY, COMPONENT, 2>
  : public point_traits_fixed_size_array_base<ARRAY, COMPONENT, 2>
{
  typedef point_traits_fixed_size_array_base<ARRAY, COMPONENT, 2> base;
  typedef typename base::component_type component_type;  

  static component_type  z(typename base::Ptype const& p) {return zero();}
  static component_type& z(typename base::Ptype      & p) {return (dummy_zero=component_type(0));}
  
  static component_type zero() { return component_type(0);}
  static component_type dummy_zero;

  static typename base::Ptype construct(component_type xx, component_type yy)
  { 
    typename base::Ptype res; 
    x(res) = xx;
    y(res) = yy;
    return res;
  }
};

template<class ARRAY, class COMPONENT>
struct point_traits_fixed_size_array<ARRAY, COMPONENT, 3>
  : public point_traits_fixed_size_array_base<ARRAY, COMPONENT, 3>
{
  typedef point_traits_fixed_size_array_base<ARRAY, COMPONENT, 3> base;
  typedef typename base::component_type                           component_type;

  static typename base::Ptype construct(component_type xx, component_type yy, component_type zz)
  { 
    typename base::Ptype res; 
    x(res) = xx;
    y(res) = yy;
    z(res) = zz;
    return res;
  }
};

template<class ARRAY, class COMPONENT>
typename 
point_traits_fixed_size_array<ARRAY, COMPONENT, 2U>::component_type 
point_traits_fixed_size_array<ARRAY, COMPONENT, 2U>::dummy_zero = 0;


template<class ARRAY, class V, unsigned N>
struct array_io_operators 
{
  typedef point_traits<ARRAY> pt;
  friend std::ostream& operator<<(std::ostream& out, ARRAY const& a) { 
    for(int i  = pt::LowerIndex(); i < pt::UpperIndex(); ++i) 
      out << a[i] << ' ';
    out << a[pt::UpperIndex()];
    return out;
  }

  friend std::istream& operator>>(std::istream& in, ARRAY & a) { 
    for(unsigned i  = pt::LowerIndex(); i <= pt::UpperIndex(); ++i) 
       in >> a[i];
    return in;
  }
  
};

  /*! \brief CRTP for injecting algebraic operators into an array class

  */
template<class ARRAY, class V, unsigned N>
struct array_operators : public array_io_operators<ARRAY,V,N>
{
  ARRAY      & to_derived()       { return static_cast<ARRAY&>(*this);}
  ARRAY const& to_derived() const { return static_cast<ARRAY const&>(*this);}
  typedef V value_type;

  ARRAY& operator-=(ARRAY const& rhs) {
    typedef point_traits<ARRAY> pt;
    for(int i  = pt::LowerIndex(); i <= pt::UpperIndex(); ++i) 
      to_derived()[i] -= rhs[i];
    return to_derived(); 
  }
  ARRAY operator-(ARRAY const& rhs) const {
    ARRAY lhs(to_derived());
    return lhs -= rhs;
  }
  ARRAY& operator+=(ARRAY const& rhs) {
    typedef point_traits<ARRAY> pt;
    for(int i  = pt::LowerIndex(); i <= pt::UpperIndex(); ++i) 
      to_derived()[i] += rhs[i];
    return to_derived(); 
  }
  ARRAY operator+(ARRAY const& rhs) const {
    ARRAY lhs(to_derived());
    return lhs += rhs;
  }

  ARRAY& operator*=(value_type v) {
    typedef point_traits<ARRAY> pt;
    for(int i  = pt::LowerIndex(); i <= pt::UpperIndex(); ++i)  
      to_derived()[i] *= v;
    return to_derived(); 
  }  
  
  ARRAY& operator/=(value_type v) {
    typedef point_traits<ARRAY> pt;
    for(int i  = pt::LowerIndex(); i <= pt::UpperIndex(); ++i) 
      to_derived()[i] /= v;
    return to_derived(); 
  }

  ARRAY plus(value_type v) const {
    ARRAY lhs(to_derived());
    return lhs *= v;
  } 

  ARRAY operator/(value_type v) const {
    ARRAY lhs(to_derived());
    return lhs /= v;
  }


  friend ARRAY operator*(ARRAY const& a, value_type v) { return a.plus(v);}
  friend ARRAY operator*(value_type v, ARRAY const& a) { return a.plus(v);}

};

/*! \brief Specialization of point_traits for fixed-size builtin arrays

 */
template<class T, size_t N>
struct point_traits_builtin_base
{
  typedef T              Ptype[N];
  typedef T              component_type;
  typedef component_type value_type;
  typedef typename dim_tag<N>::dimension_tag dimension_tag;
  enum { dimension = N };

  static void ConstructWithDim(unsigned d, Ptype&)
    { REQUIRE(d == N, "Cannot construct! d = " << d 
	      << ", N = " << N <<'\n',1);
    }
  static unsigned Dim(const Ptype&) { return N;}
  static unsigned Dim() { return N;}
  
  static int LowerIndex()             { return 0;}
  static int LowerIndex(Ptype const&) { return 0;}
  static int UpperIndex()             { return N-1;}
  static int UpperIndex(Ptype const&) { return N-1;}

  static Ptype    origin;
  static Ptype    unitv[N];

  static Ptype const& Origin() { 
    for(unsigned i = 0; i < N; ++i) 
      origin[i] = component_type(0.0);
    return origin;
  }
  static Ptype const& Origin(unsigned) { return Origin();}

  static Ptype const& unit_vector(int i) {
    REQUIRE( 0 <= i && i < N, "i=" << i << " ", 1);
    unitv[i]    = origin;
    unitv[i][i] = component_type(1);
    return unitv[i];
  }
  static Ptype const& unit_vector(int i, unsigned) { return unit_vector(i);}

};

template<class T, size_t N>
struct point_traits<T[N]> : public point_traits_builtin_base<T,N> 
{
  typedef typename  point_traits_builtin_base<T,N>::component_type component_type;
  typedef typename  point_traits_builtin_base<T,N>::Ptype          Ptype;

  static component_type  x(const Ptype& p) {return p[0];}
  static component_type& x(      Ptype& p) {return p[0];}
  static component_type  y(const Ptype& p) {return p[1];}
  static component_type& y(      Ptype& p) {return p[1];}
  static component_type  z(const Ptype& p) {return p[2];}
  static component_type& z(      Ptype& p) {return p[2];}
};

template<class T>
struct point_traits<T[1]> : public point_traits_builtin_base<T,1> 
{
  typedef typename  point_traits_builtin_base<T,1>::component_type component_type;
  typedef typename  point_traits_builtin_base<T,1>::Ptype          Ptype;

  static component_type  x(const Ptype& p) {return p[0];}
  static component_type& x(      Ptype& p) {return p[0];}
  static component_type  y(const Ptype& p) {return 0;}
  static component_type  z(const Ptype& p) {return 0;}
};

template<class T>
struct point_traits<T[2]> : public point_traits_builtin_base<T,2> 
{
  typedef typename  point_traits_builtin_base<T,2>::component_type component_type;
  typedef typename  point_traits_builtin_base<T,2>::Ptype          Ptype;

  static component_type  x(const Ptype& p) {return p[0];}
  static component_type& x(      Ptype& p) {return p[0];}
  static component_type  y(const Ptype& p) {return p[1];}
  static component_type& y(      Ptype& p) {return p[1];}
  static component_type  z(const Ptype& p) {return 0;}
};



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


/*! \brief Conversion between scalars

    \ingroup algebraicprimitives 
 */
template<class S>
class convert_scalar {
public:
  template<class T>
  static S from(T t) { return S(t);}

  template<class T>
  S operator()(T t) const { return from(t);}
};


/*! \brief conversion between points of arbitrary type

 \ingroup algebraicprimitives 
*/
template<class P, class Q>
inline void assign_point(P      & p,
			 Q const& q) 
{
 typedef point_traits<P> ptP;
 typedef point_traits<Q> ptQ;

 // if variable dimension: adapt dim of p, else this is a no-op.
 ptP::ConstructWithDim(ptQ::Dim(q), p); 

 
 REQUIRE( (ptP::Dim(p) >= ptQ::Dim(q)), 
	  "Cannot assign points of different dimension!" 
	  << "(dim(p) = " << ptP::Dim(p)  << ", "
	  << "(dim(q) = " << ptQ::Dim(q), 1);
 
 int iq = ptQ::LowerIndex(q); 
 int ip = ptP::LowerIndex(p);  
 for(; iq <= ptQ::UpperIndex(q); ++ip, ++iq)
   p[ip] = convert_scalar<typename ptP::component_type>::from(q[iq]); 
 for(; ip <= ptP::UpperIndex(p); ++ip)
   p[ip] = 0;
}  

// P = [b,e)
template<class P, class T>
inline void assign_point(P & p, T const* b, T const* e) 
{
  REQUIRE( (e-b > 0), "invalid pointer range: b = " << b << "; e = " << e << "\n",1);
  typedef point_traits<P> ptP;

  // if variable dimension: adapt dim of p, else this is a no-op.
  ptP::ConstructWithDim(e-b,p);

  REQUIRE( ptP::Dim(p) == e-b, 
	   "Cannot assign points of different dimension!" 
	  << "(dim(p) = " << ptP::Dim(p)  << ", "
	  << "e-p = " <<  e-p, 1);

  int ip = ptP::LowerIndex(p);  
  for(; b < e; ++ip, ++b)
    p[ip] = *b; 
}  

template<class P>
inline void assign_point(P& p, P const& q) { p = q;}

// these specializations should not be necessary!
template<class P>
inline void assign_point(float& p, P const& q) 

{ p = point_traits<P>::x(q);}

inline void assign_point(float& p, float q) { p = q;}



template<class P, class Q>
struct point_converter {
  typedef P result_type;
  typedef Q argument_type;

  result_type operator()(argument_type const& q) { return act(q);}
  static  result_type act(argument_type const& q) {
    P p;
    assign_point(p,q);
    return p;
  }
};

template<class P>
struct point_converter<P,P> {
  typedef P const& result_type;
  typedef P argument_type;

  static  result_type act(argument_type const& q) { return q; }
};
/*! \brief conversion between points of arbitrary type

   \ingroup algebraicprimitives 
*/
template<class P, class Q>
inline typename point_converter<P,Q>::result_type
convert_point(Q const& q) { return point_converter<P,Q>::act(q);}



template<unsigned N, unsigned M>
struct is_equal {
  enum { value = (N==M) };
};

template<class P, unsigned N>
struct point_has_dimension {
  enum { value = is_equal<N, point_traits<P>::dimension>::value };
};

} // namespace GrAL 

#endif
