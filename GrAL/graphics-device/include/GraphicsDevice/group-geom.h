#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_GROUP_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_GROUP_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



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
