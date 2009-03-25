#ifndef NMWR_GB_GRAPHICSDEVICE_CALC_TRANSFORMATIONS_H
#define NMWR_GB_GRAPHICSDEVICE_CALC_TRANSFORMATIONS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "GraphicsDevice/transformation.h"

namespace GrAL {


/*! \brief Calculate affine mapping which maps a rectangle in x,y-plane to plane 
    \f$ \{ b_l +\lambda (b_r-b_l) + \mu (t_l - b_l) \} \f$.

    The returned mapping T is such that
     -  \f$ 0 \mapsto b_l \f$,
     -   x-Axis maps to the ray trough t_l, starting at b_l, and
     -   y-Axis maps to the ray throug b_r, starting at b_l.

 */
extern
Transformation TransformRectangle(Transformation::coord_type const& b_l, // lower left corner
                                  Transformation::coord_type const& b_r, 
                                  Transformation::coord_type const& t_l);

} // namespace GrAL 

#endif
