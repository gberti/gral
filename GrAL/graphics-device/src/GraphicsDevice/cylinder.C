
// $LICENSE

#include "GraphicsDevice/cylinder.h"

namespace GrAL {

RenderableGeom RCylinder(double r, double h)
{ return RenderableGeom(new geom_cylinder(r,h));}

} // namespace GrAL 
