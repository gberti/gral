
// $LICENSE

#include "GraphicsDevice/simplex.h"

namespace GrAL {

void geom_simplex::init(double aa) 
{
  c[0]=point(0    ,  aa*sqrt(3)/3, 0           );
  c[1]=point( aa/2, -aa*sqrt(3)/6, 0           );
  c[2]=point(-aa/2, -aa*sqrt(3)/6, 0           );
  c[3]=point(0    ,  0           , aa/2*sqrt(3));
}


RenderableGeom RSimplex(RenderableGeom::coord_type const& c1,
                        RenderableGeom::coord_type const& c2, 
                        RenderableGeom::coord_type const& c3, 
                        RenderableGeom::coord_type const& c4)
{ return RenderableGeom(new geom_simplex(c1,c2,c3,c4));}

RenderableGeom RSimplex(double a)
{ return RenderableGeom(new geom_simplex(a));}

} // namespace GrAL 
