#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_QUADRILATERAL_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_QUADRILATERAL_H

// $LICENSE

#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"
//#include "GraphicsDevice/colormap.h"
#include "GraphicsDevice/rgb-color.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

class geom_quadrilateral : public geom {
private:
  typedef geom_quadrilateral self;
  coord_type c[4];  //< 4 corners of the quadrilateral
protected:
  void init(coord_type const& cc1,
            coord_type const& cc2,
            coord_type const& cc3,
            coord_type const& cc4) 
  {c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;}
public:
  geom_quadrilateral(coord_type const& cc1,
                     coord_type const& cc2,
                     coord_type const& cc3,
                     coord_type const& cc4) 
  {init(cc1,cc2,cc3,cc4);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_quadrilateral(*this);}

  coord_type const& corner(int i) const 
  {
    REQUIRE ( (i<=4 && i>=1), 
      "geom_quadrilateral::corner called with i= "<<i,1);
     return c[i-1];
  }
};




class geom_color_quad : public geom {
private:
  typedef geom_color_quad self;

  coord_type  c[4]; //< 4 corners of the quadrilateral
  rgb_color col[4]; //< 1 color for each corner 	    
protected:
  void init(coord_type const& cc1,coord_type const& cc2,
	    coord_type const& cc3,coord_type const& cc4,const rgb_color& ccol) 
    {c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4; 
     for (int i=0;i<4;i++) col[i]=ccol;
    }
  void init(coord_type const& cc1,const rgb_color ccol1,
	    coord_type const& cc2,const rgb_color ccol2,
	    coord_type const& cc3,const rgb_color ccol3,
	    coord_type const& cc4,const rgb_color ccol4) 
    {c[0]=cc1;c[1]=cc2;c[2]=cc3;c[3]=cc4;
     col[0]=ccol1;col[1]=ccol2;col[2]=ccol3;col[3]=ccol4;
    }
public:
  geom_color_quad(coord_type const& cc1,
                  coord_type const& cc2,
                  coord_type const& cc3,
                  coord_type const& cc4,
                  const rgb_color& ccol) 
  {init (cc1,cc2,cc3,cc4,ccol);}

  geom_color_quad(coord_type const& cc1,const rgb_color& ccol1,
		  coord_type const& cc2,const rgb_color& ccol2,
		  coord_type const& cc3,const rgb_color& ccol3,
		  coord_type const& cc4,const rgb_color& ccol4) 
  {init(cc1,ccol1,cc2,ccol2,cc3,ccol3,cc4,ccol4);}
  virtual self * clone() const { return new self(*this); }

  virtual void   write_geom_to(rendering_language& L) const 
  { L.write_color_quad(*this);}

  coord_type const& corner(int i) const 
  {
    REQUIRE( (i>= 1 && i<=4),
	     "geom_quadrilateral::corner called with i= "<<i, 1);
    return c[i-1];
  }

  rgb_color const&  color_corner(int i) const
  {
    return col[i-1];
  }
};



extern RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, 
                                     RenderableGeom::coord_type const& c2, 
                                     RenderableGeom::coord_type const& c3, 
                                     RenderableGeom::coord_type const& c4);

extern RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, 
                                     RenderableGeom::coord_type const& c2, 
                                     RenderableGeom::coord_type const& c3,  
                                     RenderableGeom::coord_type const& c4, 
                                     const rgb_color& rgb_colorr);

extern RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, const rgb_color& color1,
				     RenderableGeom::coord_type const& c2, const rgb_color& color2,
				     RenderableGeom::coord_type const& c3, const rgb_color& color3,
				     RenderableGeom::coord_type const& c4, const rgb_color& color4);


} // namespace GrAL 

#endif
