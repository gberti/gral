
// $LICENSE

#include "GraphicsDevice/ball.h"

namespace GrAL {

RenderableGeom RBall(double r, const point& m)
{ return RenderableGeom(new geom_ball(r,m));}

} // namespace GrAL 
