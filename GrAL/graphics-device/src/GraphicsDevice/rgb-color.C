

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
