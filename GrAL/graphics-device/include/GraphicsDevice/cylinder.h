#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_CYLINDER_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_CYLINDER_H

// $LICENSE


/*----------------------------------------------------------------------------
    cylinder.h		class to create a cylinder

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/geom.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"


class geom_cylinder : public geom {
private:
  double r; // radius
  double h; // height
protected:
  void init(double rr, double hh) {r=rr;h=hh;}
public:
  geom_cylinder(double rr, double hh) {init(rr,hh);}
  double radius() const {return r;}
  double height() const {return h;}
  void   write_geom_to(rendering_language& L) const 
  { L.write_cylinder(*this);}
};


extern RenderableGeom RCylinder(double r, double h);


#endif
