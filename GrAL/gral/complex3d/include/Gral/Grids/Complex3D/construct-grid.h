#ifndef GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_H
#define GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/construct-grid.h"
#include "Gral/Grids/Complex3D/complex3d.h"

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex3D/construct-grid.C"
#else

namespace GrAL {

/*! \defgroup complex3dcopy Generic copy operations for Complex3D
    \ingroup complex3dmodule

    \see $GrALConstructGrid
    \see Gral base module \ref copyoperations
 */

/*! \brief Copy any grid to Complex3D
    \ingroup complex3dcopy
    \see Test in \ref test-construct-complex3d.C
 */
template<class G_SRC, class VCORR, class CCORR>
void ConstructGrid0(Complex3D     & G_dest,
		    G_SRC    const& G_src,
		    VCORR         & v_corr,
		    CCORR         & c_corr);

} // namespace GrAL

#endif

namespace GrAL {

/*! \brief Copy any grid to Complex3D

    This version copies also the geometry.
    \ingroup complex3dcopy
    \see Test in \ref test-construct-complex3d.C
*/
template<class G_SRC, class GEOM_SRC,
         class GEOM_DEST,
         class VCORR, class CCORR>
inline
void ConstructGridVC (Complex3D     & G_dest,
		      GEOM_DEST     & Geom_dest,
		      G_SRC    const& G_src,
		      GEOM_SRC const& Geom_src,
		      VCORR         & v_corr,
		      CCORR         & c_corr)
{ ConstructGridVC_generic(G_dest,Geom_dest, G_src, Geom_src, v_corr, c_corr); }

} // namespace GrAL 

#endif
