#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_SEGMENT_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_SEGMENT_H

// $LICENSE

#include <list>

#include "Utility/pre-post-conditions.h"
#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

class geom_segment : public geom {
private:
  typedef geom_segment self;
  coord_type c[2];
protected:
  void init(coord_type const& cc1,coord_type const& cc2) {c[0]=cc1;c[1]=cc2;}
public:
  geom_segment(coord_type const& cc1 = point(0,0,0),
	       coord_type const& cc2 = point(0,0,0)) {init(cc1,cc2);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_segment(*this);}

  coord_type const&  corner(int i) const 
  {
    REQUIRE((i == 1 || i == 2), 
	    "geom_segment::corner called with i= "<< i << " !",1);
    return c[i-1];
  }

};


/*! \brief Creator function for geometric segment
    \ingroup geometricshape
 */
extern RenderableGeom RSegment(RenderableGeom::coord_type const& c1, 
                               RenderableGeom::coord_type const& c2);

/*! \brief List of segments
   \ingroup geometricshape
  \todo Is this useful or offers any advantage over geom_group?
 */
class geom_segment_list : public geom {
private:
  typedef geom_segment_list self;
  ::std::list<geom_segment> LI;
protected:
  
public:
  geom_segment_list(::std::list<geom_segment> const& LL) : LI(LL){}
  virtual self * clone() const { return new self(*this); }

  virtual void  write_geom_to(rendering_language& L) const;
};

} // namespace GrAL 

#endif
