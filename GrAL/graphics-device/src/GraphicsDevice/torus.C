
// $LICENSE

#include "GraphicsDevice/torus.h"

namespace GrAL {

void  geom_torus::write_geom_to(rendering_language& L) const 
{ 
  if (style=='n') 
    L.write_torus(*this);
  else 
    L.write_torus_bezier(*this);
}

RenderableGeom RTorus(double r1, double r2, char style)
{ return RenderableGeom(new geom_torus(r1,r2,style));}

} // namespace GrAL 
