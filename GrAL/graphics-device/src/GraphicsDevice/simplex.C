
// $LICENSE

/*----------------------------------------------------------------------------
    simplex.C		init a new simplex

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/simplex.h"

void geom_simplex::init(double aa) {
  c[0]=point(0,aa/3*sqrt(3),0);c[1]=point(aa/2,-aa/2*sqrt(3)/3,0);
  c[2]=point(-aa/2,-aa/2*sqrt(3)/3,0);c[3]=point(0,0,aa/2*sqrt(3));}

void geom_simplex::init(const point& cc1,const point& cc2,
			const point& cc3,const point& cc4) {
  c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;}

RenderableGeom RSimplex(const point& c1, const point& c2, const point& c3, const point& c4)
{ return RenderableGeom(new geom_simplex(c1,c2,c3,c4));}
RenderableGeom RSimplex(double a)
{ return RenderableGeom(new geom_simplex(a));}
