
// $LICENSE

#include "GraphicsDevice/segment.h"

void geom_segment_list::write_geom_to(rendering_language& L) const 
{ 
  L.begin_block();
  typedef std::list<geom_segment>::const_iterator cit;
  for (cit i=LI.begin();i!=LI.end();++i) {
    L.write_segment((*i));
  }
  L.end_block();
}

RenderableGeom RSegment(RenderableGeom::coord_type const& c1, 
                        RenderableGeom::coord_type const& c2)
{ return RenderableGeom(new geom_segment(c1,c2));}

//RenderableGeom RSegment_1(RenderableGeom::coord_type const& c1, RenderableGeom::coord_type const& c2)
//{ return RenderableGeom(new geom_segment(c1,c2));}

