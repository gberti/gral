/*----------------------------------------------------------------------------
    triangle.C		init new triangle

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/triangle.h"

RenderableGeom RTriangle(const point& c1, const point& c2, const point& c3)
{ return RenderableGeom(new geom_triangle(c1,c2,c3));}

RenderableGeom RTriangle_1(const point& c1, const point& c2, const point& c3)
{ return RenderableGeom(new geom_triangle(c1,c2,c3));}
