/*----------------------------------------------------------------------------
    segment.C		init a new segment

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/segment.h"

RenderableGeom RSegment(const point& c1, const point& c2)
{ return RenderableGeom(new geom_segment(c1,c2));}

RenderableGeom RSegment_1(const point& c1, const point& c2)
{ return RenderableGeom(new geom_segment(c1,c2));}

