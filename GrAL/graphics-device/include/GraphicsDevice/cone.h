#ifndef NMWR_GB_GEOM_CONE_H
#define NMWR_GB_GEOM_CONE_H

/*----------------------------------------------------------------------------
    cone.h		class to create a cone

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"


class geom_cone : public geom {
private:
  double r1; // lower radius
  double r2; // upper radius
  double h; // height
protected:
  void init(double rr1, double rr2, double hh) {r1=rr1;r2=rr2;h=hh;}
public:
  geom_cone(double rr1, double rr2, double hh) {init(rr1,rr2,hh);}
  double lower_radius() const {return r1;}
  double upper_radius() const {return r2;}
  double height() const {return h;}
  void   write_geom_to(rendering_language& L) const 
  { L.write_cone(*this);}
};


extern RenderableGeom RCone(double r1, double r2, double h);


#endif
