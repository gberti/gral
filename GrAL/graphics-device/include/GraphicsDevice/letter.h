
// $LICENSE

#ifndef NMWR_GB_GRAPHICSDEVICE_GEOM_letter_H
#define NMWR_GB_GRAPHICSDEVICE_GEOM_letter_H


#include "GraphicsDevice/geom.h"
#include "GraphicsDevice/rendering-language.h"
#include "GraphicsDevice/renderable-geom.h"

namespace GrAL {

/*! \brief Creator function for a geometric letter
     \ingroup geometricshape
*/
extern RenderableGeom RLetter(char ch,
                              RenderableGeom::coord_type const& offset);


} // namespace GrAL 

#endif
