
// $LICENSE

/*----------------------------------------------------------------------------
    cone.C		init a new cone

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/cone.h"

RenderableGeom RCone(double r1, double r2, double h)
{ return RenderableGeom(new geom_cone(r1,r2,h));}
