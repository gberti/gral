
// $LICENSE

#include "GraphicsDevice/sphere.h"

namespace GrAL {

RenderableGeom RSphere(double r, const point& m)
{ return RenderableGeom(new geom_sphere(r,m));}

} // namespace GrAL 
