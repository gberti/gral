#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_CONE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_CONE_H

// $LICENSE


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

/*! A geometric (truncated) cone
   \ingroup geometricshape
   \see RCone
 */
class geom_cone : public geom {
private:
  typedef geom_cone self;
  double r1; //< lower radius
  double r2; //< upper radius
  double h;  //< height
protected:
  void init(double rr1, double rr2, double hh) {r1=rr1;r2=rr2;h=hh;}
public:
  geom_cone(double rr1, double rr2, double hh) {init(rr1,rr2,hh);}
  virtual self * clone() const { return new self(*this); }

  virtual void  write_geom_to(rendering_language& L) const 
  { L.write_cone(*this);}

  double lower_radius() const {return r1;}
  double upper_radius() const {return r2;}
  double height() const {return h;}
};

/*! Creator function for geometric (truncated) cone
   
    \ingroup geometricshape
 */
extern RenderableGeom RCone(double r1, double r2, double h);


#endif
