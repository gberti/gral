#ifndef NMWR_GB_GRAPHICSDEVICE_EMPTY_GEOM_H
#define NMWR_GB_GRAPHICSDEVICE_EMPTY_GEOM_H

// $LICENSE


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable_object.h"


class empty_geom : public geom {
public:
  void write_geom_to(rendering_language&) {}
  // > p forall p \in R^3
  //  virtual point box_min() const { return point(DBL_MAX,DBL_MAX,DBL_MAX);}
 // < p forall p \in R^3 
  //  virtual point box_max() const { return point(DBL_MIN,DBL_MIN,DBL_MIN);}
  //  virtual bbox() const { return bbox(box_min(),box_max());}
  virtual ~empty_geom() {}
};


RenderableGeom REmptyGeom() { return RenderableGeom(new empty_geom());}


#endif
