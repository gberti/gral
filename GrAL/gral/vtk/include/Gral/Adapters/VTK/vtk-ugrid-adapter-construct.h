#ifndef GRAL_VTK_UGRID_ADAPTER_CONSTRUCT_GRID_H
#define GRAL_VTK_UGRID_ADAPTER_CONSTRUCT_GRID_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti,  Roman Putanowicz, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/construct-grid.h"
#include "Gral/Adapters/VTK/vtk-ugrid-adapter.h"

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Adapters/VTK/vtk-ugrid-adapter-construct.C"
#else

namespace GrAL {

/*! \defgroup vtkugridcopy Generic copy operations for UGridVTKAdapter<D>
    \ingroup vtkugridadaptermodule

    \see $GrALConstructGrid
    \see Gral base module \ref copyoperations
 */

/*! \brief Copy any grid to UGridVTKAdapter<D>
    \ingroup vtkugridcopy
    \see Test in \ref test-construct-vtkugrid.C
 */
template<int D, class G_SRC, class VCORR, class CCORR>
void ConstructGrid0(vtk_ugrid::UGridVTKAdapter<D> & G_dest,
		    G_SRC    const& G_src,
		    VCORR         & v_corr,
		    CCORR         & c_corr);

} // namespace GrAL

#endif

namespace GrAL {

/*! \brief Copy any grid to UGridVTKAdapter<D>

    This version copies also the geometry.
    \ingroup vtkugridcopy
    \see Test in \ref test-construct-vtkugrid.C
*/
template<int D, class G_SRC, class GEOM_SRC,
         class GEOM_DEST,
         class VCORR, class CCORR>
inline
void ConstructGridVC (vtk_ugrid::UGridVTKAdapter<D>  & G_dest,
		      GEOM_DEST     & Geom_dest,
		      G_SRC    const& G_src,
		      GEOM_SRC const& Geom_src,
		      VCORR         & v_corr,
		      CCORR         & c_corr)
{ ConstructGridVC_generic(G_dest,Geom_dest, G_src, Geom_src, v_corr, c_corr); }

} // namespace GrAL 

#endif
