#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_BALL_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_BALL_H

// $LICENSE


#include "GraphicsDevice/geom.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {


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

} // namespace GrAL 

#endif
