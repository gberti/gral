#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_CIRCLE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_CIRCLE_H

// $LICENSE


/*----------------------------------------------------------------------------
    circle.h		class to create a circle

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        August 6, 1997
-----------------------------------------------------------------------------*/
#include "GraphicsDevice/geom.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"


class geom_circle : public geom {
private:
  point m;  // midpoint of the circle
  double r1,r2; // Radien von Kreis/Ellipse
protected:
  void init(const point& cc, double rr) {m=cc;r1=rr;r2=rr;}
  void init(const point& cc, double rr1, double rr2) {m=cc;r1=rr1;r2=rr2;}
public:
  geom_circle(const point& cc, double rr) {init(cc,rr);}
  geom_circle(const point& cc, double rr1, double rr2) {init(cc,rr1,rr2);}

  point midpoint() const {return m;}
  double radius1() const {return r1;}
  double radius2() const {return r2;}

  void   write_geom_to(rendering_language& L) const 
  { L.write_circle(*this);}
};


extern RenderableGeom RCircle(const point& c1, double r);
extern RenderableGeom REllipse(const point& c1, double r1, double r2);


#endif
