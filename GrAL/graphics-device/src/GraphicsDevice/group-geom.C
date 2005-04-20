#include "GraphicsDevice/group-geom.h"  

namespace GrAL {

geom_group::geom_group() {}

void geom_group::write_geom_to(rendering_language& L) const 
{
  L.begin_block();
  for(std::list<RenderableGeom>::const_iterator g = geoms.begin(); 
      g != geoms.end(); ++g)
    L.filter(*g);
  L.end_block();
}

} // namespace GrAL 
