#ifndef NMWR_GB_GEOM_BALL_H
#define NMWR_GB_GEOM_BALL_H

/*----------------------------------------------------------------------------
    ball.h		class to create a ball (sphere with triangles)

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/geom.h"

#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"


class geom_ball : public geom {
private:
  double r; // radius
  point m;  // midpoint
protected:
  void init(double rr, const point& mm) {r=rr;m=mm;}
public:
  geom_ball(double rr, const point& mm) {init(rr,mm);}
  double radius()   const { return r;}
  point  midpoint() const { return m;}
  void   write_geom_to(rendering_language& L) const 
  { L.write_ball(*this);}
};


extern RenderableGeom RBall(double r, const point& m);


#endif
