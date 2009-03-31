

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/circle.h"

namespace GrAL {

RenderableGeom RCircle(RenderableGeom::coord_type const& c1, double r)
{ return RenderableGeom(new geom_circle(c1,r));}

RenderableGeom RCircle_1(RenderableGeom::coord_type const& c1, double r)
{ return RenderableGeom(new geom_circle(c1,r));}

RenderableGeom REllipse(RenderableGeom::coord_type const& c1, double r1, double r2)
{ return RenderableGeom(new geom_circle(c1,r1,r2));}

} // namespace GrAL 
