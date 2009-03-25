
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
