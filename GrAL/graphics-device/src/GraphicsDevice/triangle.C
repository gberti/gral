
// $LICENSE

#include "GraphicsDevice/triangle.h"

RenderableGeom RTriangle(RenderableGeom::coord_type const& c1,
                         RenderableGeom::coord_type const& c2, 
                         RenderableGeom::coord_type const& c3)
{ return RenderableGeom(new geom_triangle(c1,c2,c3));}

RenderableGeom RTriangle_1(RenderableGeom::coord_type const& c1,
                           RenderableGeom::coord_type const& c2, 
                           RenderableGeom::coord_type const& c3)
 { return RenderableGeom(new geom_triangle(c1,c2,c3));}
