
// $LICENSE

/*----------------------------------------------------------------------------
    torus.C		init a new torus

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/torus.h"

RenderableGeom RTorus(double r1, double r2, char style)
{ return RenderableGeom(new geom_torus(r1,r2,style));}
