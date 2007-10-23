#ifndef NMWR_GB_GENERAL_COORDS_TEMPLATES_H
#define NMWR_GB_GENERAL_COORDS_TEMPLATES_H


// $LICENSE


#include <iostream>
#include "Geometry/point-traits.h"

#ifndef NO_COORD_SPECIAL

#include "Geometry/coords_2_special.h"
#include "Geometry/coords_3_special.h"

#endif

#include "Geometry/algebraic-primitives.h"


namespace GrAL {

typedef double coord_N_component;

/*! \defgroup coordNalg Simple vector type and related operations

*/

/*! \brief  A general class template for small geometric points.

   \ingroup coordNalg

 
  The only template parameter is currently the dimension N.
  This class could as well be parameterized over the type
  of the components, now fixed to double. Default template 
  parameters offer a clean way to add this generality later
  without having to change code elsewhere.

  Features:
   * 1-based indexing with [] and ()
   * all algebraic operators: 
      - \f$ \R^N \times \R^N \mapsto \R^N \f$ :+=, -=, +, - 
      - \f$ \R^N   \mapsto \R^N \f$ : unary -
      - \f$ \R^N \times \R   \mapsto \R^N,
            \R \times \R^N   \mapsto \R^N \f$ : *=, /=, *, /
   * There is \e NO operator* for the dot-product 
     due to non-associativity!! 
     (\f$ (x*y)*z is different from x*(y*z)\f$ for \f$ x,y,z \in \R^N \f$)
   

  Specializations exist for N=2 and N=3.
  They can be disabled by
  #define NO_COORD_SPECIAL 1

  This class template can work together with the matrix<M,N> template.

  Test in  test-coords.C
*/


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

  const component& operator[](index i) const { _c(i); return X[--i];}
        component& operator[](index i)       { _c(i); return X[--i];}
  const component& operator()(index i) const { _c(i); return X[--i];}
        component& operator()(index i)       { _c(i); return X[--i];}

  void _c(index i) const { REQUIRE ( (1 <= i && i <= N), "i = " << i << '\n',1);}

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


/*! \brief equality
   \ingroup coordNalg
 */
template<unsigned N>
inline 
bool operator==(const coordN<N>& lhs, const coordN<N> rhs)
{ for(unsigned int i=1; i<=N; i++) if (lhs[i]!=rhs[i]) return false;
 return true; }

/*! \brief non-equality
   \ingroup coordNalg
 */
template<unsigned N>
inline bool operator!=(const coordN<N>& lhs, const coordN<N> rhs)
{ return !(lhs == rhs);}

/*! \brief vector addition
   \ingroup coordNalg
 */
template<unsigned N>
inline coordN<N> operator+(const coordN<N>& lhs, const coordN<N>& rhs)
{ coordN<N> tmp(lhs); return (tmp+= rhs);}

/*! \brief vectro subtraction
   \ingroup coordNalg
 */
template<unsigned N>
inline coordN<N> operator-(const coordN<N>& lhs, const coordN<N>& rhs)
{ coordN<N> tmp(lhs); return(tmp-= rhs);}

/*! \brief multiplication with scalar
   \ingroup coordNalg
 */
template<unsigned N>
inline coordN<N> operator*(const coordN<N>& lhs, coord_N_component rhs)
{ coordN<N> tmp(lhs); return (tmp *= rhs);}

/*! \brief multiplication with scalar
   \ingroup coordNalg
 */
template<unsigned N>
inline coordN<N> operator*( const coord_N_component& lhs, const coordN<N>& rhs)
{ return rhs*lhs;}

/*! \brief division by scalar
   \ingroup coordNalg
 */
template<unsigned N>
inline coordN<N> operator/(const coordN<N>& lhs, coord_N_component rhs)
{ coordN<N> tmp(lhs); return (tmp /= rhs); }


/*! \brief unary -
   \ingroup coordNalg
 */
template<unsigned N>
inline coordN<N> operator-(const coordN<N>& rhs)
{ coordN<N> tmp(rhs); return(tmp *= -1);}


/*! \brief output
   \ingroup coordNalg
 */
template<unsigned N>
inline ::std::ostream& operator<<(::std::ostream& out, const coordN<N>& P)
{
  for(unsigned i = 1; i<= N-1; i++)
    out <<  P[i] << ' ';
  out << P[N]; // no blank after last number
  return out;
}


/*! \brief input
   \ingroup coordNalg
 */
template<unsigned N>
inline ::std::istream& operator>>(::std::istream& in, coordN<N>& P)
{
  for(unsigned i = 1; i<= N; i++)
    in >> P[i];
  return in;
}


template<unsigned N> struct array_offset<coordN<N> > { enum { value = 1}; };

template<unsigned N>
struct point_traits<coordN<N> >
  : public point_traits_fixed_size_array<coordN<N>, typename coordN<N>::component, N> {};


} // namespace GrAL 

#endif
