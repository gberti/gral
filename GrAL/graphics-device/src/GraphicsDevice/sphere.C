

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/sphere.h"

namespace GrAL {

RenderableGeom RSphere(double r, const point& m)
{ return RenderableGeom(new geom_sphere(r,m));}

} // namespace GrAL 
