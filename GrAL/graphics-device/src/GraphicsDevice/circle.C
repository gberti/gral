
// $LICENSE

#include "GraphicsDevice/circle.h"

namespace GrAL {

RenderableGeom RCircle(RenderableGeom::coord_type const& c1, double r)
{ return RenderableGeom(new geom_circle(c1,r));}

RenderableGeom RCircle_1(RenderableGeom::coord_type const& c1, double r)
{ return RenderableGeom(new geom_circle(c1,r));}

RenderableGeom REllipse(RenderableGeom::coord_type const& c1, double r1, double r2)
{ return RenderableGeom(new geom_circle(c1,r1,r2));}

} // namespace GrAL 
