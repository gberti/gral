#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_BALL_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_BALL_H

// $LICENSE

/*----------------------------------------------------------------------------
    ball.h		class to create a ball (sphere with triangles)

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/

#include "GraphicsDevice/geom.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"


/*! \brief A geometric representation of a sphere
    \ingroup geometricshape
    \todo Is this different from geom_sphere?? 
    \see RBall()
    \see geom_sphere
 */
class geom_ball : public geom {
private:
  typedef geom_ball self;
  double     r;  //< radius
  coord_type m;  //< midpoint
protected:
  void init(double rr, const point& mm) {r=rr;m=mm;}
public:
  geom_ball(double rr, const point& mm) {init(rr,mm);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_ball(*this);}

  double             radius()   const { return r;}
  coord_type const&  midpoint() const { return m;}
};


extern RenderableGeom RBall(double r, const point& m);


#endif
