
// $LICENSE

#include "GraphicsDevice/cone.h"

RenderableGeom RCone(double r1, double r2, double h)
{ return RenderableGeom(new geom_cone(r1,r2,h));}
