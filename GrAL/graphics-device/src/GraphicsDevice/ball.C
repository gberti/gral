/*----------------------------------------------------------------------------
    ball.C		init a new ball (sphere with triangles)

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/ball.h"

RenderableGeom RBall(double r, const point& m)
{ return RenderableGeom(new geom_ball(r,m));}
