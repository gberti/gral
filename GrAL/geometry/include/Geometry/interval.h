#ifndef GRAL_GB_GEOMETRY_INTERVAL_H
#define GRAL_GB_GEOMETRY_INTERVAL_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


// FIXME: replace with <limits> when gcc 2.9x does not need to be supported any more
#include <boost/limits.hpp>
#include <algorithm> // swap

namespace GrAL {

/*! Representation of real interval

 */
template<class REAL>
class interval {
private:
  typedef REAL scalar_type;
  scalar_type t0, t1;
public:

  //! create empty interval
  interval() : t0( ::std::numeric_limits<scalar_type>::max()),
	       t1(-::std::numeric_limits<scalar_type>::max())
  {}
  //! 
  interval(scalar_type left, scalar_type right) 
    : t0(left),
      t1(right)
  {}
  bool empty() const { return t0 > t1;}
  bool contains(scalar_type x) const { return t0 <= x && x <= t1;}

  void sort() { if (t0 > t1) ::std::swap(t0,t1);}
  scalar_type l() const { return t0;}
  scalar_type r() const { return t1;}
  scalar_type low () const { return t0;}
  scalar_type high() const { return t1;}


  //! return \f$\conv(\mbox{\texttt{*this}} \cup \{p\}) \f$
  interval& operator|=(scalar_type p)
  {
    t0 = ( p < t0 ? p : t0);
    t1 = ( p > t1 ? p : t1);
    return *this;
  }
  //! return \f$\conv(\mbox{\texttt{*this}} \cup I) \f$
  interval& operator|=(interval<scalar_type> I)
  {
    t0 = (I.l() < t0 ? I.l() : t0);
    t1 = (I.r() > t1 ? I.r() : t1);
    return *this;
  }
};

template<class REAL> 
inline bool left_of(interval<REAL> I1, interval<REAL> I2)
{ return I1.r() < I2.l();}

template<class REAL> 
inline bool right_of(interval<REAL> I1, interval<REAL> I2)
{ return I1.l() > I2.r();}

template<class REAL> 
inline bool left_touch(interval<REAL> I1, interval<REAL> I2)
{ return I1.r() == I2.l();}

template<class REAL> 
inline bool right_touch(interval<REAL> I1, interval<REAL> I2)
{ return I1.l() == I2.r();}

template<class REAL> 
inline bool disjoint(interval<REAL> I1, interval<REAL> I2)
{ return I1.r() < I2.l() || I1.l() > I2.r(); }


} // namespace GrAL 

#endif
