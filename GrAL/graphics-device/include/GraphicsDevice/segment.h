#ifndef NMWR_GB_GEOM_SEGMENT_H
#define NMWR_GB_GEOM_SEGMENT_H

/*----------------------------------------------------------------------------
    segment.h		class to create a segment

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Utility/pre-post-conditions.h"
#include "Graphics/geom.h"
#include "Graphics/translators.h"
#include "Graphics/renderable-object.h"

#include <list.h>

class geom_segment : public geom {
private:
  point c[2];
protected:
  void init(const point& cc1,const point& cc2) {c[0]=cc1;c[1]=cc2;}
public:
  geom_segment(const point& cc1 = point(0,0,0),
	       const point& cc2 = point(0,0,0)) {init(cc1,cc2);}
  point  corner(int i) const 
  {
    REQUIRE((i == 1 || i == 2), 
	    "geom_segment::corner called with i= "<< i << " !",1);
    return c[i-1];
  }

  void   write_geom_to(rendering_language& L) const 
  { L.write_segment(*this);}
};

extern RenderableGeom RSegment(const point& c1, const point& c2);

class geom_segment_list : public geom {
private:
  list<geom_segment> LI;
protected:
  
public:
  geom_segment_list(const list<geom_segment>& LL) : LI(LL){}
  void   write_geom_to(rendering_language& L) const 
  { 
    L.begin_block();
    //geom_segment S; 
    typedef list<geom_segment>::const_iterator cit;
    for (cit i=LI.begin();i!=LI.end();++i)
      {L.write_segment((*i));//cerr << *i << "\n";
      }
    L.end_block();
  }
};

#include "Work-around/default-operators.h"
MAKE_DEFAULT_OPS(geom_segment,"geom_segment");
#undef MAKE_DEFAULT_OPS

#endif
