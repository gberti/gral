#ifndef GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_H
#define GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_H

// $LICENSE_NEC

#include "Gral/Base/construct-grid.h"
#include "Gral/Grids/Complex3D/complex3d.h"

template<class G_SRC, class VCORR, class CCORR>
void ConstructGrid0(Complex3D     & G_dest,
		    G_SRC    const& G_src,
		    VCORR         & v_corr,
		    CCORR    const& c_corr);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex3D/construct-grid.C"
#endif

#endif
