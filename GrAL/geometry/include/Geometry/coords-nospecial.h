#ifndef GENERAL_COORDS_TEMPLATES_NOSPECIAL_H
#define GENERAL_COORDS_TEMPLATES_NOSPECIAL_H

#include "is-specialized.h"
#include "geometric-point-traits.h"

typedef double component;

template<unsigned N>
class coordN_ns {
public:
  typedef unsigned index;
  typedef coordN_ns self;
  //typedef component component;

  coordN_ns() {}
  coordN_ns(const component& x)  { for(index i=0; i<N; i++) X[i] = x;}
  coordN_ns(const component Y[]) { for(index i=0; i<N; i++) X[i] = Y[i];}
  coordN_ns(const self& rhs)     { for(index i=0; i<N; i++) X[i] = rhs.X[i];}
  self& operator=(const self& rhs)
  { 
    //    if(this != & rhs)
    for(index i=0; i<N; i++)
      X[i] = rhs.X[i];
   return *this; 
  }

  ~coordN_ns() {}
  const component& operator[](index i) const { return X[i];}
        component& operator[](index i)       { return X[i];}
  self& operator+=(const self& rhs) { for(index i=0; i<N; i++) X[i] += rhs.X[i]; return *this; }
  self& operator-=(const self& rhs) { for(index i=0; i<N; i++) X[i] -= rhs.X[i]; return *this; }
  self& operator*=(const component& rhs) { for(index i=0; i<N; i++) X[i] *= rhs; return *this; }
  self& operator/=(const component& rhs) { for(index i=0; i<N; i++) X[i] /= rhs; return *this; }

  static self origin() { return self(component(0));}
  static index dim()   { return N;}
private:
  component X[N];
};


template<unsigned K, unsigned L>
inline coordN_ns<K+L> operator,(const coordN_ns<K>& PK, const coordN_ns<L>& PL)
{
 coordN_ns<K+L> result;
 for(unsigned k = 1; k <= K; k++)
   result[k] = PK[k];
 for(unsigned l = 1; l <= L; l++)
   result[l+K] = PL[l];
 return result;
} 


template<unsigned N>
inline coordN_ns<N> operator+(const coordN_ns<N>& lhs, const coordN_ns<N>& rhs)
{ coordN_ns<N> tmp(lhs); return (tmp+= rhs);}

template<unsigned N>
inline coordN_ns<N> operator-(const coordN_ns<N>& lhs, const coordN_ns<N>& rhs)
{ coordN_ns<N> tmp(lhs); tmp-= rhs; return tmp;} 

template<unsigned N>
inline coordN_ns<N> operator*(const coordN_ns<N>& lhs, const component& rhs)
{ coordN_ns<N> tmp(lhs); tmp *= rhs; return tmp; }

template<unsigned N>
inline coordN_ns<N> operator*( const component& lhs, const coordN_ns<N>& rhs)
{ return rhs*lhs;}

template<unsigned N>
inline coordN_ns<N> operator/(const coordN_ns<N>& lhs, const component& rhs)
{ coordN_ns<N> tmp(lhs); tmp /= rhs; return tmp; }



template<unsigned N>
ostream& operator<<(ostream& out, const coordN_ns<N>& P)
{
  for(unsigned i = 0; i<= N-1; i++)
    out <<  P[i] << ' ';
  return out;
}

template<unsigned N>
istream& operator>>(istream& in, coordN_ns<N>& P)
{
  for(unsigned i = 0; i<= N-1; i++)
    in >> P[i];
  return in;
}


template<unsigned N>
struct point_traits_for_coordN_ns : public point_traits_default {
  typedef coordN_ns<N> point_t;
  static int LowerIndex(const point_t&) { return 0;}
  static int UpperIndex(const point_t&) { return N-1;}
  static int Dimension(const point_t&)  { return N;}

  static int Dimension()                 { return N;}
  static fixed_size_tag point_size_tag() { return fixed_size_tag();}

  static point_t origin() { return point_t(0.0);}

  typedef double component_type;

  //static string name() { return "coordN_ns<N=" + make_string(N) +">";} 
};

// partielle Spezialisierung von point_traits<T> :
// (geht noch nicht)
/*
template<unsigned N>
struct point_traits<coordN_ns<N> > :  public point_traits_for_coordN_ns<N> {};
*/
// daher:
// totale Spezialisierung f"ur alle ben"otigten N.
struct point_traits<coordN_ns<2> > : public point_traits_for_coordN_ns<2> {};
struct point_traits<coordN_ns<3> > : public point_traits_for_coordN_ns<3> {};

#endif
