#ifndef NMWR_GB_POINT_TRAITS_H
#define NMWR_GB_POINT_TRAITS_H



// $LICENSE

#include "Config/compiler-config.h"
#include "Utility/pre-post-conditions.h"

// must be specialized
template<class P>
struct point_traits {};

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

};

// TODO: let all point_traits for fixed dimension types inherit this.
template<class POINT, unsigned DIM>
struct point_traits_fixed_dim 
  : public point_traits_base<POINT>
{
  typedef dim_tag<N>                dimension_tag; 
  static unsigned Dim()             { return DIM;}
  static unsigned Dim(Ptype const&) { return DIM;}
  static void     ConstructWithDim(unsigned d, Ptype&)
    { REQUIRE(d == DIM, "Cannot construct: d = " << d << "; DIM = " << DIM,1);}
};

template<class REAL>
struct real_point_traits : public point_traits_base<REAL> { 
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




template<class ARRAY, class COMPONENT, unsigned DIM>
struct point_traits_fixed_size_array :
  public point_traits_base<ARRAY>
{
  typedef ARRAY          Ptype;
  typedef COMPONENT      component_type;
  typedef component_type value_type;
  typedef dim_tag<DIM>::dimension_tag dimension_tag;

  static void ConstructWithDim(unsigned d, Ptype&)
    { REQUIRE(d == DIM, "Cannot construct! d = " << d 
	      << ", DIM = " << DIM <<'\n',1);
    }
  static unsigned Dim(const Ptype&) { return DIM;}
  static unsigned Dim() { return DIM;}
  
  static int LowerIndex()             { return 0;}
  static int LowerIndex(Ptype const&) { return 0;}
  static int UpperIndex()             { return DIM-1;}
  static int UpperIndex(Ptype const&) { return DIM-1;}


  static Ptype Origin() {
    Ptype p; 
    for(unsigned i = 0; i < DIM; ++i) p[i] = component_type(0.0);
    return p;
  }
  static Ptype Origin(unsigned) { return Origin();}


  // should branch on DIM here.
  static component_type  x(const Ptype& p) {return p[0];}
  static component_type& x(      Ptype& p) {return p[0];}
  static component_type  y(const Ptype& p) {return p[1];}
  static component_type& y(      Ptype& p) {return p[1];}
  static component_type  z(const Ptype& p) {return p[2];}
  static component_type& z(      Ptype& p) {return p[2];}

};


template<class ARRAY, unsigned N>
struct array_operators 
{
  ARRAY      & to_derived()       { return static_cast<ARRAY&>(*this);}
  ARRAY const& to_derived() const { return static_cast<ARRAY const&>(*this);}


  ARRAY& operator-=(ARRAY const& rhs) {
    for(unsigned i  = 0; i < N; ++i) 
      to_derived()[i] -= rhs[i];
    return to_derived(); 
  }
  ARRAY operator-(ARRAY const& rhs) const {
    ARRAY lhs(to_derived());
    return lhs -= rhs;
  }
  ARRAY& operator+=(ARRAY const& rhs) {
    for(unsigned i  = 0; i < N; ++i) 
      to_derived()[i] += rhs[i];
    return to_derived(); 
  }
  ARRAY operator+(ARRAY const& rhs) const {
    ARRAY lhs(to_derived());
    return lhs += rhs;
  }

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

template<class P, class Q>
inline void assign_point(P      & p,
			 Q const& q) 
{
 typedef point_traits<P> ptP;
 typedef point_traits<Q> ptQ;

 // if variable dimension: adapt dim of p, else this is a no-op.
 ptP::ConstructWithDim(ptQ::Dim(q), p); 

 
 REQUIRE( (ptP::Dim(p) == ptQ::Dim(q)), 
	  "Cannot assign points of different dimension!" 
	  << "(dim(p) = " << ptP::Dim(p)  << ", "
	  << "(dim(q) = " << ptQ::Dim(q), 1);
 
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

#endif
