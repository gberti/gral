
// $LICENSE

/*----------------------------------------------------------------------------
    cylinder.C		init a new cylinder

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/cylinder.h"

RenderableGeom RCylinder(double r, double h)
{ return RenderableGeom(new geom_cylinder(r,h));}
