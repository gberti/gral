

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/cylinder.h"

namespace GrAL {

RenderableGeom RCylinder(double r, double h)
{ return RenderableGeom(new geom_cylinder(r,h));}

} // namespace GrAL 
