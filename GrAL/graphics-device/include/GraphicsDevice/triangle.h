#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_TRIANGLE_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_TRIANGLE_H

// $LICENSE


#include "Utility/pre-post-conditions.h"
#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

/*! \brief A geometric triangle in 3D
   \ingroup geometricshape
   \todo could be complemented by geom_color_triangle,
   like geom_quadrilateral is complemented by geom_color_quad.
 */
class geom_triangle : public geom {
private:
  typedef geom_triangle self;
  coord_type c[3];  //<  3 corners of the triangle
protected:
  void init(coord_type const& cc1,
            coord_type const& cc2,
            coord_type const& cc3) 
  {c[0]=cc1;c[1]=cc2;c[2]=cc3;}
public:
  geom_triangle(coord_type const& cc1,
                coord_type const& cc2,
                coord_type const& cc3) 
  {init(cc1,cc2,cc3);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_triangle(*this);}

  coord_type const& corner(int i) const 
  {
    REQUIRE( (i<=3 || i>=1),
	     "geom_triangle::corner called with i= "<<i,1);
    return c[i-1];
  }
};


extern RenderableGeom RTriangle(RenderableGeom::coord_type const& c1,
                                RenderableGeom::coord_type const& c2, 
                                RenderableGeom::coord_type const& c3);


#endif
