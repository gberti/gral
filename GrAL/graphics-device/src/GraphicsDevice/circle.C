/*----------------------------------------------------------------------------
    circle.C		init new circle

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        August 6, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/circle.h"

RenderableGeom RCircle(const point& c1, double r)
{ return RenderableGeom(new geom_circle(c1,r));}
RenderableGeom RCircle_1(const point& c1, double r)
{ return RenderableGeom(new geom_circle(c1,r));}

RenderableGeom REllipse(const point& c1, double r1, double r2)
{ return RenderableGeom(new geom_circle(c1,r1,r2));}
