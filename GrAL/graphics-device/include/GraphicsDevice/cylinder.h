#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_CYLINDER_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_CYLINDER_H

// $LICENSE

#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

/*!  \brief Geometric cylinder
     \ingroup geometricshape
 
   Axis is in z direction.
   \see RCylinder();

 */

class geom_cylinder : public geom {
private:
  typedef geom_cylinder self;

  double r; //< radius
  double h; //< height
protected:
  void init(double rr, double hh) {r=rr;h=hh;}
public:
  geom_cylinder(double rr, double hh) {init(rr,hh);}
  virtual self * clone() const { return new self(*this); }
  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_cylinder(*this);}

  double radius() const {return r;}
  double height() const {return h;}
};

/*!  \brief Creator function for geometric cylinder 
    \ingroup geometricshape

    Axis is in z-direction.
 */
extern RenderableGeom RCylinder(double r, double h);

} // namespace GrAL 

#endif
