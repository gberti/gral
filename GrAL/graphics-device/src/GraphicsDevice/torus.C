

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/torus.h"

namespace GrAL {

void  geom_torus::write_geom_to(rendering_language& L) const 
{ 
  if (style=='n') 
    L.write_torus(*this);
  else 
    L.write_torus_bezier(*this);
}

RenderableGeom RTorus(double r1, double r2, char style)
{ return RenderableGeom(new geom_torus(r1,r2,style));}

} // namespace GrAL 
