#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_QUADRILATERAL_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_QUADRILATERAL_H

// $LICENSE


/*----------------------------------------------------------------------------
    quadrilateral.h		class to create a quadrilateral

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/

#include "Utility/pre-post-conditions.h"
#include "Visualization/colormap.h"
#include "GraphicsDevice/geom.h"

#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"


class geom_quadrilateral : public geom {
private:
  point c[4];  // 4 corners of the quadrilateral
protected:
  void init(const point& cc1,const point& cc2,const point& cc3,const point& cc4) {c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;}
public:
  geom_quadrilateral(const point& cc1,const point& cc2,const point& cc3,const point& cc4) {init(cc1,cc2,cc3,cc4);}
  point  corner(int i) const 
  {
    REQUIRE ( (i<=4 && i>=1), 
      "geom_quadrilateral::corner called with i= "<<i,1);
     return c[i-1];
  }
  void   write_geom_to(rendering_language& L) const 
  { L.write_quadrilateral(*this);}
};

class geom_color_quad : public geom {
private:
  point c[4];  // 4 corners of the quadrilateral
  rgb_color col[4];// 1 color for each corner 	    
protected:
  void init(const point& cc1,const point& cc2,
	    const point& cc3,const point& cc4,const rgb_color& ccol) 
    {c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4; 
     for (int i=0;i<4;i++) col[i]=ccol;
    }
  void init(const point& cc1,const rgb_color ccol1,
	    const point& cc2,const rgb_color ccol2,
	    const point& cc3,const rgb_color ccol3,
	    const point& cc4,const rgb_color ccol4) 
    {c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;
     col[0]=ccol1;col[1]=ccol2;col[2]=ccol3;col[3]=ccol4;
    }
public:
  geom_color_quad(const point& cc1,const point& cc2,const point& cc3,const point& cc4,const rgb_color& ccol) {init (cc1,cc2,cc3,cc4,ccol);}
  geom_color_quad(const point& cc1,const rgb_color& ccol1,
		  const point& cc2,const rgb_color& ccol2,
		  const point& cc3,const rgb_color& ccol3,
		  const point& cc4,const rgb_color& ccol4) 
    {init(cc1,ccol1,cc2,ccol2,cc3,ccol3,cc4,ccol4);}
  point  corner(int i) const 
  {
    REQUIRE( (i>= 1 && i<=4),
	     "geom_quadrilateral::corner called with i= "<<i, 1);
    return c[i-1];
  }

  rgb_color  color_corner(int i) const
  {
    return col[i-1];
  }
  void   write_geom_to(rendering_language& L) const 
  { L.write_color_quad(*this);}
};


extern RenderableGeom RQuadrilateral(const point& c1, const point& c2, const point& c3, const point& c4);

extern RenderableGeom RQuadrilateral(const point& c1, const point& c2, const point& c3, const point& c4, const rgb_color& rgb_colorr);

extern RenderableGeom RQuadrilateral(const point& c1, const rgb_color& color1,
				     const point& c2, const rgb_color& color2,
				     const point& c3, const rgb_color& color3,
				     const point& c4, const rgb_color& color4);


#endif
