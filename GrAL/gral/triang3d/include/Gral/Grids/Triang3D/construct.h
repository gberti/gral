#ifndef GRAL_GB_GRIDS_TRIANG3D_CONSTRUCT_H
#define GRAL_GB_GRIDS_TRIANG3D_CONSTRUCT_H

// $LICENSE 


#include "Gral/Base/construct-grid.h"
#include "Gral/Grids/Triang3D/triang3d.h"



/*! \file 
 */

namespace GrAL {

/*! \defgroup triang3dcopy Generic copy operations for Triang3D
   
    \ingroup triang3dmodule

    Overloads of the templates in \ref Gral/Base/construct-grid.h
    \see $GrALConstructGrid
    \see GrAL base module \ref copyoperations
*/


/*! \brief Copy any grid \c srcG to \c  Triang3D, including geometry

    \ingroup triang3dcopy
 */
template<class Geom, class G2, class Geom2>
extern  void 
ConstructGrid(Triang3D   & destG,
	      Geom       & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);


/*! \brief Copy any grid \c srcG to \c  Triang3D, including geometry and vertex mapping

    \ingroup triang3dcopy
 */
template<class Geom, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(Triang3D      & destG,
	       Geom          & destGeom, 
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 
// VertexCorr  maps G2::vertex_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

/*! \brief Copy any grid \c srcG to \c  Triang3D, including geometry and vertex/cell mapping

    \ingroup triang3dcopy
 */
template<class Geom, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(Triang3D      & destG, 
		Geom          & destGeom, 
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 
// CellCorr  maps G2::cell_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

/*! \brief Copy any grid \c srcG to \c  Triang3D (without geometry, with vertex and cell mapping)

    \ingroup triang3dcopy
 */
template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang3D      & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 

/*! \brief Copy any grid \c srcG to \c  Triang3D (without geometry and mapping)

    \ingroup triang3dcopy
 */
template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang3D      & destG, 
	       G2       const& srcG);


} // namespace GrAL 

// for inclusion model

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Triang3D/construct.C"
#endif

#endif
