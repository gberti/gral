#ifndef NMWR_GB_GEOM_SIMPLEX_H
#define NMWR_GB_GEOM_SIMPLEX_H

/*----------------------------------------------------------------------------
    simplex.h		class to create a simplex

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Utility/pre-post-conditions.h"
#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"


class geom_simplex : public geom {
private:
  point c[4];
protected:
  void init(double aa);
  void init(const point& cc1,const point& cc2,
	    const point& cc3,const point& cc4);
public:
  geom_simplex(double aa) {init(aa);}
  geom_simplex(point cc1,point cc2,point cc3,point cc4) {init(cc1,cc2,cc3,cc4);}
  point  corner(int i) const 
  {
    REQUIRE( (i<=4 && i>=1), 
      "geom_simplex::corner called with i= "<<i,1);    
    return c[i-1];
  }
  void   write_geom_to(rendering_language& L) const 
  { L.write_simplex(*this);}
};

extern RenderableGeom RSimplex(const point& c1, const point& c2, const point& c3, const point& c4);
extern RenderableGeom RSimplex(double a);

#endif
