#ifndef NMWR_GB_GRAPHICSDEVICE_RGB_COLOR_H
#define NMWR_GB_GRAPHICSDEVICE_RGB_COLOR_H

//#include "Geometry/point3d.h"
#include <string>
#include <iostream>

namespace GrAL {

class rgb_color {
private:
  double _r,_g,_b;
public:

  rgb_color() : _r(0.0), _g(0.0), _b(0.0) {}
  rgb_color(double rr, double gg, double bb) : _r(rr), _g(gg), _b(bb) {}
  double r() const {return _r;}
  double g() const {return _g;}
  double b() const {return _b;}

  friend ::std::istream& operator>>(::std::istream& in,  rgb_color& rs) { rs.read(in); return in;}
  friend ::std::ostream& operator<<(::std::ostream& out, const rgb_color& rs) { rs.print(out); return out;}
  void print(::std::ostream& out) const;
  void read(::std::istream& in) ;

  //  operator Point3D() const { return Point3D(_r,_g,_b);}

  static rgb_color white() { return rgb_color(1,1,1);}
  static rgb_color black() { return rgb_color(0,0,0);}
  static rgb_color gray(double lum = 0.5) { return rgb_color(lum,lum,lum);}
  static rgb_color red()   { return rgb_color(1,0,0);}
  static rgb_color green() { return rgb_color(0,1,0);}
  static rgb_color blue()  { return rgb_color(0,0,1);}
  static rgb_color yellow() { return rgb_color(1,1,0);}
  static rgb_color cyan()   { return rgb_color(0,1,1);}
  static rgb_color magenta(){ return rgb_color(1,0,1);}
  //  static rgb_color color(const char* name); // get color from rgb.txt
};


class wrap_rgb_color : public rgb_color {
public:
  wrap_rgb_color() {}
  wrap_rgb_color(const rgb_color& c) : rgb_color(c) {}
  ::std::string key() const;

  void  print(::std::ostream& out) const { rgb_color::print(out);}
  friend ::std::ostream& operator<<(::std::ostream& out ,const wrap_rgb_color& c) 
    { c.print(out); return out;}
};

} // namespace GrAL 

#endif
