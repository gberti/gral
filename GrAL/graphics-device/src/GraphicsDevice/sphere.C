/*----------------------------------------------------------------------------
    sphere.C		init a new sphere

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/sphere.h"

RenderableGeom RSphere(double r, const point& m)
{ return RenderableGeom(new geom_sphere(r,m));}
