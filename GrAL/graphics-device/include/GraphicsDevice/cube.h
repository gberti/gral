#ifndef NMWR_GB_GEOM_CUBE_H
#define NMWR_GB_GEOM_CUBE_H

/*----------------------------------------------------------------------------
    cube.h		class to create a cube

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/

#include "Utility/pre-post-conditions.h"
#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"


class geom_cube : public geom {
private:
  point c[8];
protected:
  void init(double aa);
  void init(const point& cc1,const point& cc2,
	    const point& cc3,const point& cc4,
	    const point& cc5,const point& cc6,
	    const point& cc7,const point& cc8);
public:
  geom_cube(double aa) {init(aa);}
  geom_cube(point cc1,point cc2,point cc3,point cc4,
	    point cc5,point cc6,point cc7,point cc8) 
    {init(cc1,cc2,cc3,cc4,cc5,cc6,cc7,cc8);}

  point  corner(int i) const 
  {
    REQUIRE( ( i >= 1 && i <= 8),
	     "geom_cube::corner called with i= "<< i << " ! ",1);
    return c[i-1];
  }

  void   write_geom_to(rendering_language& L) const 
  { L.write_cube(*this);}
};

extern RenderableGeom RCube(const point& c1, const point& c2, const point& c3, const point& c4, const point& c5, const point& c6, const point& c7, const point& c8);
extern RenderableGeom RCube(double a);

#endif
