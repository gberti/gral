#ifndef NMWR_GB_GRAPHICSDEVICE_EMPTY_GEOM_H
#define NMWR_GB_GRAPHICSDEVICE_EMPTY_GEOM_H

// $LICENSE


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

/*! \brief Empty geometry
     
    \ingroup geometricshape 
    \see REmptyGeom()
 */
class empty_geom : public geom {
private:
  typedef empty_geom self;
public:
  virtual self * clone() const { return new self(*this); }

  virtual void write_geom_to(rendering_language&) {}

  // > p forall p \in R^3
  //  virtual point box_min() const { return point(DBL_MAX,DBL_MAX,DBL_MAX);}
  // < p forall p \in R^3 
  //  virtual point box_max() const { return point(DBL_MIN,DBL_MIN,DBL_MIN);}
  //  virtual bbox() const { return bbox(box_min(),box_max());}
};


/*! \brief Creator function for empty geometry
    \ingroup geometricshape 
 */
inline RenderableGeom REmptyGeom() { return RenderableGeom(new empty_geom());}

} // namespace GrAL 

#endif
