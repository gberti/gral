#ifndef NMWR_GB_COORDS_2_SPECIALIZATION_H
#define NMWR_GB_COORDS_2_SPECIALIZATION_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



namespace GrAL {

//----------------------------------------------------------------
// 
// to be included by coord.h
// specialization of coordN<N> for N = 2
//
//----------------------------------------------------------------

template<unsigned N> class coordN;

template<>
class coordN<2> {
public:
  typedef unsigned index;
  typedef coordN<2> self;
  typedef double   component;

  coordN() { X[0] = X[1] = 0;}
  coordN(const component& x) { X[0] = X[1] = x;}
  coordN(const component x1, const component x2) { X[0] = x1; X[1] = x2;}
  coordN(const self& rhs)    { X[0] = rhs.X[0]; X[1] = rhs.X[1];}
  self& operator=(const self& rhs)
  { 
    //if(this != & rhs){
    X[0] = rhs.X[0]; X[1] = rhs.X[1];
    //}   
    return *this;
  }

  ~coordN() {}
  
  component  operator[](index i) const { _c(i); return X[--i];}
  component& operator[](index i)       { _c(i); return X[--i];}
  component  operator()(index i) const { _c(i); return X[--i];}
  component& operator()(index i)       { _c(i); return X[--i];}

  void _c(index i) const { REQUIRE ( (1 <= i && i <= 2), "i = " << i << '\n',1);}

 
  self& operator+=(const self& rhs) { X[0] += rhs.X[0];X[1] += rhs.X[1]; return *this; }
  self& operator-=(const self& rhs) { X[0] -= rhs.X[0];X[1] -= rhs.X[1]; return *this; }
  self& operator*=(const component& rhs) { X[0] *= rhs;X[1] *= rhs; return *this; }
  self& operator/=(const component& rhs) { X[0] /= rhs;X[1] /= rhs; return *this; }
  
  friend inline self operator+(const self& ls, const self& rs)
  {  return self(ls.X[0]+rs.X[0], ls.X[1]+rs.X[1]);}
  friend inline self operator-(const self& ls, const self& rs)
  {  return self(ls.X[0]-rs.X[0], ls.X[1]-rs.X[1]);}

  static self origin() { return self(component(0));}
  static self Origin() { return self(component(0));}
  static index dim()   { return 2;}
private:
  component X[2];
};


template<class T> struct is_specialized;

template<>
struct is_specialized<coordN<2> > {
  static bool specialized() { return true;}
  static const char* name() { return "coordN<2>";}
};

} // namespace GrAL 

#endif
