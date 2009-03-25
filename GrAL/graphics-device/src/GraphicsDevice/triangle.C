

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/triangle.h"

namespace GrAL {

RenderableGeom RTriangle(RenderableGeom::coord_type const& c1,
                         RenderableGeom::coord_type const& c2, 
                         RenderableGeom::coord_type const& c3)
{ return RenderableGeom(new geom_triangle(c1,c2,c3));}

RenderableGeom RTriangle_1(RenderableGeom::coord_type const& c1,
                           RenderableGeom::coord_type const& c2, 
                           RenderableGeom::coord_type const& c3)
 { return RenderableGeom(new geom_triangle(c1,c2,c3));}

} // namespace GrAL 
