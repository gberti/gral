/*----------------------------------------------------------------------------
    quadrilateral.C     init new quadrilateral

    by Heiko Schwierz, BTU-Cottbus, torus@math.tu-cottbus.de
    at Lehrstuhl Numerische Mathematik und Wissenschaftliches Rechnen (NMWR)

    last change:        July 4, 1997
-----------------------------------------------------------------------------*/
#include "Graphics/quadrilateral.h"

RenderableGeom RQuadrilateral(const point& c1, const point& c2,
			      const point& c3, const point& c4)
{ return RenderableGeom(new geom_quadrilateral(c1,c2,c3,c4));}


RenderableGeom RQuadrilateral(const point& c1, const point& c2,
			      const point& c3, const point& c4, 
			      const rgb_color& colorr)
{ return RenderableGeom(new geom_color_quad(c1,c2,c3,c4,colorr));}

RenderableGeom RQuadrilateral(const point& c1, const rgb_color& color1,
			      const point& c2, const rgb_color& color2, 
			      const point& c3, const rgb_color& color3,
			      const point& c4, const rgb_color& color4)
{ return RenderableGeom(new geom_color_quad(c1,color1,c2,color2,c3,color3,c4,color4));}


RenderableGeom RQuadrilateral_1(const point& c1, const point& c2, 
			      const point& c3, const point& c4)
{ return RenderableGeom(new geom_quadrilateral(c1,c2,c3,c4));}


RenderableGeom RQuadrilateral_1(const point& c1, const point& c2, 
				const point& c3, const point& c4, const rgb_color& colorr)
{ return RenderableGeom(new geom_color_quad(c1,c2,c3,c4,colorr));}

RenderableGeom RQuadrilateral_1(const point& c1, const rgb_color& color1, 
				const point& c2, const rgb_color& color2, 
				const point& c3, const rgb_color& color3, 
				const point& c4,const rgb_color& color4)
{ return RenderableGeom(new geom_color_quad(c1,color1,c2,color2,c3,color3,c4,color4));}
