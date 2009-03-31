

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
