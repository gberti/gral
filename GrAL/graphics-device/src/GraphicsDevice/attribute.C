

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/attribute.h"
#include "GraphicsDevice/rgb-color.h"

namespace GrAL {

static Attribute MakeAttr(const rgb_color& c) 
{ 
  Attribute A;// = new Attribute;
  SetValue(A,wrap_rgb_color(c));
  return A;
}

Attribute Attribute::black() {return MakeAttr(rgb_color(0.0,0.0,0.0));}
Attribute Attribute::white() {return MakeAttr(rgb_color(1.0,1.0,1.0));}
Attribute Attribute::red()   {return MakeAttr(rgb_color(1.0,0.0,0.0));}
Attribute Attribute::green() {return MakeAttr(rgb_color(0.0,1.0,0.0));}
Attribute Attribute::blue()  {return MakeAttr(rgb_color(0.0,0.0,1.0));}
Attribute Attribute::yellow(){return MakeAttr(rgb_color(1.0,1.0,0.0));}
Attribute Attribute::pink()  {return MakeAttr(rgb_color(1.0,0.0,1.0));}
Attribute Attribute::magenta(){return MakeAttr(rgb_color(1.0,0.0,1.0));}
Attribute Attribute::cyan() {return MakeAttr(rgb_color(0.0,1.0,1.0));}
Attribute Attribute::orange() {return MakeAttr(rgb_color(1.0,0.3,0.0));}
Attribute Attribute::new_color(double r, double g, double b) 
{return MakeAttr(rgb_color(r,g,b));}
//Attribute Attribute::transparent(double alpha) {init(alpha);}

} // namespace GrAL 
