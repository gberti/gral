
// $LICENSE

#include "GraphicsDevice/cube.h"

namespace GrAL {

void geom_cube::init(double aa) 
{
  c[5]=point( aa/2, aa/2,aa/2); c[4]=point( aa/2, aa/2,-aa/2);
  c[7]=point( aa/2,-aa/2,aa/2); c[6]=point( aa/2,-aa/2,-aa/2);
  c[3]=point(-aa/2, aa/2,aa/2); c[2]=point(-aa/2, aa/2,-aa/2);
  c[1]=point(-aa/2,-aa/2,aa/2); c[0]=point(-aa/2,-aa/2,-aa/2);
}


RenderableGeom RCube(RenderableGeom::coord_type const& c1, 
                     RenderableGeom::coord_type const& c2, 
                     RenderableGeom::coord_type const& c3, 
                     RenderableGeom::coord_type const& c4, 
                     RenderableGeom::coord_type const& c5, 
                     RenderableGeom::coord_type const& c6, 
                     RenderableGeom::coord_type const& c7, 
                     RenderableGeom::coord_type const& c8)
{ return RenderableGeom(new geom_cube(c1,c2,c3,c4,c5,c6,c7,c8));}

RenderableGeom RCube(double a)
{ return RenderableGeom(new geom_cube(a));}

} // namespace GrAL 
