
// $LICENSE

#include "mystring.h"
#include "GraphicsDevice/rgb-color.h"

void rgb_color::print(ostream& out) const {
  out << _r << ' ' << _g << ' ' << _b;
}

void rgb_color::read(istream& in) {
  in >> _r >> _g >> _b;
}

string wrap_rgb_color::key() const { return "color";}
