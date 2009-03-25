#ifndef GRAL_GB_GRIDS_TRIANG2D_CONSTRUCT_H
#define GRAL_GB_GRIDS_TRIANG2D_CONSTRUCT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
 


#include "Gral/Base/construct-grid.h"
#include "Gral/Grids/Triang2D/triang2d.h"

/*! \file 
 */

/*! \defgroup triang2dcopy Generic copy operations for Triang2D
   
    \ingroup triang2dmodule

    Overloads of the templates in \ref Gral/Base/construct-grid.h
    \see $GrALConstructGrid
    \see Gral base module \ref copyoperations
*/

  
namespace GrAL {

/*! \brief Copy any grid \c srcG to \c  Triang2D, including geometry

    \ingroup triang2dcopy
 */
template<class Geom, class G2, class Geom2>
extern  void 
ConstructGrid(Triang2D   & destG,
	      Geom       & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);

/*! \brief Copy any grid \c srcG to \c  Triang2D, including geometry and mapping

    \ingroup triang2dcopy
 */
template<class Geom, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(Triang2D      & destG,
	       Geom          & destGeom, 
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 
// VertexCorr  maps G2::vertex_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

/*! \brief Copy any grid \c srcG to \c  Triang2D, including geometry and mapping

    \ingroup triang2dcopy
 */
template<class Geom, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(Triang2D      & destG, 
		Geom          & destGeom, 
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 
// CellCorr  maps G2::cell_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

/*! \brief Copy any grid \c srcG to \c  Triang2D (without geometry)

    \ingroup triang2dcopy
 */
template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang2D      & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 

template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang2D      & destG, 
	       G2       const& srcG);


} // namespace GrAL 

// for inclusion model

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Triang2D/construct.C"
#endif

#endif
