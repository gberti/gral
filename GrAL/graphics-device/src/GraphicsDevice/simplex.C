

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
