#ifndef NMWR_GB_COORDS_3_SPECIALIZATION_H
#define NMWR_GB_COORDS_3_SPECIALIZATION_H


// $LICENSE

namespace GrAL {

//----------------------------------------------------------------
// 
// to be included by coord.h
// specialization of coordN<N> for N = 3
//
//----------------------------------------------------------------

template<unsigned N> class coordN;

template<>
class coordN<3> {
public:
  typedef double   component;
  typedef unsigned index;
  typedef coordN<3> self;

  coordN() {}
  coordN(const component& x) { X[0] = X[1] = X[2] = x;}
  coordN(const component x1, const component x2, const component x3) 
  { X[0] = x1; X[1] = x2; X[2] = x3;}
  coordN(const self& rhs)    { X[0] = rhs.X[0]; X[1] = rhs.X[1];X[2] = rhs.X[2];}
  self& operator=(const self& rhs)
  { 
    //if(this != & rhs){
    X[0] = rhs.X[0]; X[1] = rhs.X[1];X[2] = rhs.X[2];
    //}   
    return *this;
  }

  ~coordN() {}
  
  component  operator[](index i) const {  _c(i); return X[--i];}
  component& operator[](index i)       {  _c(i); return X[--i];}
  component  operator()(index i) const {  _c(i); return X[--i];}
  component& operator()(index i)       {  _c(i); return X[--i];}

  void _c(index i) const { REQUIRE ( (1 <= i && i <= 3), "i = " << i << '\n',1);}


  self& operator+=(const self& rhs) { X[0] += rhs.X[0];X[1] += rhs.X[1];X[2] += rhs.X[2]; return *this; }
  self& operator-=(const self& rhs) { X[0] -= rhs.X[0];X[1] -= rhs.X[1];X[2] -= rhs.X[2];return *this; }
  self& operator*=(const component& rhs) { X[0] *= rhs;X[1] *= rhs;X[2] *= rhs; return *this; }
  self& operator/=(const component& rhs) { X[0] /= rhs;X[1] /= rhs;X[2] /= rhs; return *this; }
  
  friend inline self operator+(const self& ls, const self& rs)
  {  return self(ls.X[0]+rs.X[0], ls.X[1]+rs.X[1],ls.X[2]+rs.X[2]);}
  friend inline self operator-(const self& ls, const self& rs)
  {  return self(ls.X[0]-rs.X[0], ls.X[1]-rs.X[1],ls.X[2]-rs.X[2]);}

  static self origin() { return self(component(0));}
  static self Origin() { return self(component(0));}
  static index dim()   { return 3;}
private:
  component X[3];
};

template<class T> struct is_specialized;

template<>
struct is_specialized<coordN<3> > {
  static bool specialized() { return true;}
  static const char* name() { return "coordN<3>";}
};

} // namespace GrAL 

#endif
