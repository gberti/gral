
// $LICENSE

#include "GraphicsDevice/quadrilateral.h"

namespace GrAL {

RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, 
                              RenderableGeom::coord_type const& c2, 
                              RenderableGeom::coord_type const& c3, 
                              RenderableGeom::coord_type const& c4)
{ return RenderableGeom(new geom_quadrilateral(c1,c2,c3,c4));}


RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, 
                              RenderableGeom::coord_type const& c2, 
                              RenderableGeom::coord_type const& c3,  
                              RenderableGeom::coord_type const& c4, 
                              const rgb_color& colorr)
{ return RenderableGeom(new geom_color_quad(c1,c2,c3,c4,colorr));}


RenderableGeom RQuadrilateral(RenderableGeom::coord_type const& c1, const rgb_color& color1,
                              RenderableGeom::coord_type const& c2, const rgb_color& color2,
                              RenderableGeom::coord_type const& c3, const rgb_color& color3,
                              RenderableGeom::coord_type const& c4, const rgb_color& color4)
{ return RenderableGeom(new geom_color_quad(c1,color1,c2,color2,c3,color3,c4,color4));}



RenderableGeom RQuadrilateral_1(RenderableGeom::coord_type const& c1, 
                                RenderableGeom::coord_type const& c2, 
                                RenderableGeom::coord_type const& c3, 
                                RenderableGeom::coord_type const& c4)
{ return RenderableGeom(new geom_quadrilateral(c1,c2,c3,c4));}


RenderableGeom RQuadrilateral_1(RenderableGeom::coord_type const& c1, 
                                RenderableGeom::coord_type const& c2, 
                                RenderableGeom::coord_type const& c3,  
                                RenderableGeom::coord_type const& c4, 
                                const rgb_color& colorr)
{ return RenderableGeom(new geom_color_quad(c1,c2,c3,c4,colorr));}


RenderableGeom RQuadrilateral_1(RenderableGeom::coord_type const& c1, const rgb_color& color1,
                              RenderableGeom::coord_type const& c2, const rgb_color& color2,
                              RenderableGeom::coord_type const& c3, const rgb_color& color3,
                              RenderableGeom::coord_type const& c4, const rgb_color& color4)
{ return RenderableGeom(new geom_color_quad(c1,color1,c2,color2,c3,color3,c4,color4));}


} // namespace GrAL 
