#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_SIMPLEX_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_SIMPLEX_H

#include "Utility/pre-post-conditions.h"

#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"


/*! A geometric 3D simplex (tetrahedron)
    \ingroup geometricshape
    \see RSimplex
 */
class geom_simplex : public geom {
private:
  typedef geom_simplex self;
  coord_type c[4];
protected:
  void init(double aa);
  void init(coord_type const& cc1,coord_type const& cc2,
	    coord_type const& cc3,coord_type const& cc4)
  { c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;}
public:
  geom_simplex(double aa) {init(aa);}
  geom_simplex(point cc1,point cc2,point cc3,point cc4) {init(cc1,cc2,cc3,cc4);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_simplex(*this);}

  coord_type const& corner(int i) const 
  {
    REQUIRE( (i<=4 && i>=1), 
      "geom_simplex::corner called with i= "<<i,1);    
    return c[i-1];
  }
};


/*! \brief Creator function for geometric simplex
    \ingroup geometricshape
 */
extern RenderableGeom RSimplex(RenderableGeom::coord_type const& c1,
                               RenderableGeom::coord_type const& c2, 
                               RenderableGeom::coord_type const& c3, 
                               RenderableGeom::coord_type const& c4);

/*! \brief Creator function for geometric simplex, with edge length a
    \ingroup geometricshape
 */
extern RenderableGeom RSimplex(double a);

#endif
