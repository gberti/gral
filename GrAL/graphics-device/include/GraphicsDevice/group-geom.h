#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_GROUP_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_GROUP_H

// $LICENSE


#include <list>

#include "GraphicsDevice/geom.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

class geom_group : public geom {
  typedef geom_group self;
public:
  geom_group();
  virtual self * clone() const { return new self(*this); }

  virtual void write_geom_to(rendering_language& L) const;

  self& add_geom(const RenderableGeom& g) { geoms.push_back(g); return *this;}

  //  friend geom_group& operator<<(geom_group& grp, const RenderableGeom& g) 
  //   {  return grp.add_geom(g);}
private:
  std::list<RenderableGeom> geoms;
};

} // namespace GrAL 

#endif
