
// $LICENSE

#include "GraphicsDevice/rgb-color.h"

namespace GrAL {

void rgb_color::print(std::ostream& out) const {
  out << _r << ' ' << _g << ' ' << _b;
}

void rgb_color::read(std::istream& in) {
  in >> _r >> _g >> _b;
}

std::string wrap_rgb_color::key() const { return "color";}

} // namespace GrAL 
