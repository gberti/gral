#ifndef NMWR_GB_GEOM_TORUS_H
#define NMWR_GB_GEOM_TORUS_H

/*----------------------------------------------------------------------------
    torus.h		class to create a torus

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"


class geom_torus : public geom {
private:
  double r1; // major radius
  double r2; // minor radius
  char style;// bezier oder nicht
protected:
  void init(double rr1, double rr2, char ssty) {r1=rr1;r2=rr2;style=ssty;}
public:
  geom_torus(double rr1, double rr2, char ssty) {init(rr1,rr2,ssty);}
  double major_radius() const {return r1;}
  double minor_radius() const {return r2;}
  void   write_geom_to(rendering_language& L) const 
    { if (style=='n') 
         {cerr << "Torus: normal version\n";L.write_torus(*this);}
      else 
	 {cerr << "Torus: bezier version\n";L.write_torus_bezier(*this);}}
};


extern RenderableGeom RTorus(double r1, double r2, char style='n');


#endif
