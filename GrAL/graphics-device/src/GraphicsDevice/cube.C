
// $LICENSE

/*----------------------------------------------------------------------------
    cube.C		init a new cube

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/cube.h"

void geom_cube::init(double aa) {
  c[5]=point(aa/2,aa/2,aa/2);c[4]=point(aa/2,aa/2,-aa/2);
  c[7]=point(aa/2,-aa/2,aa/2);c[6]=point(aa/2,-aa/2,-aa/2);
  c[3]=point(-aa/2,aa/2,aa/2);c[2]=point(-aa/2,aa/2,-aa/2);
  c[1]=point(-aa/2,-aa/2,aa/2);c[0]=point(-aa/2,-aa/2,-aa/2);}

void geom_cube::init(const point& cc1,const point& cc2,
		     const point& cc3,const point& cc4,
		     const point& cc5,const point& cc6,
		     const point& cc7,const point& cc8) {
  c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;c[4]=cc5;c[5]=cc6;c[6]=cc7;c[7]=cc8;}

RenderableGeom RCube(const point& c1, const point& c2, const point& c3, const point& c4, const point& c5, const point& c6, const point& c7, const point& c8)
{ return RenderableGeom(new geom_cube(c1,c2,c3,c4,c5,c6,c7,c8));}
RenderableGeom RCube(double a)
{ return RenderableGeom(new geom_cube(a));}
