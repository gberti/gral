#ifndef NMWR_GB_GEOM_TRIANGLE_H
#define NMWR_GB_GEOM_TRIANGLE_H

/*----------------------------------------------------------------------------
    triangle.h		class to create a triangle

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/

#include "Utility/pre-post-conditions.h"
#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"


class geom_triangle : public geom {
private:
  point c[3];  // 3 corners of the triangle
protected:
  void init(const point& cc1,const point& cc2,const point& cc3) {c[0]=cc1;c[1]=cc2;c[2]=cc3;}
public:
  geom_triangle(const point& cc1,const point& cc2,const point& cc3) {init(cc1,cc2,cc3);}
  point  corner(int i) const 
  {
    REQUIRE( (i<=3 || i>=1),
	     "geom_triangle::corner called with i= "<<i,1);
    return c[i-1];
  }
  void   write_geom_to(rendering_language& L) const 
  { L.write_triangle(*this);}
};


extern RenderableGeom RTriangle(const point& c1, const point& c2, const point& c3);


#endif
