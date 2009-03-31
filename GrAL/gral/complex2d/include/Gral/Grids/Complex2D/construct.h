#ifndef GRAL_GB_COMPLEX2D_CONSTRUCT_H
#define GRAL_GB_COMPLEX2D_CONSTRUCT_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file 
 */

/*! \defgroup complex2dcopy Generic copy operations for Complex2D
   
    \ingroup complex2dmodule

    Overloads of the templates in \ref Gral/Base/construct-grid.h
    \see $GrALConstructGrid
    \see Gral base module \ref copyoperations
*/

#include "Gral/Base/construct-grid.h"
#include "Gral/Grids/Complex2D/complex2d.h"


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/complex2d-construct.C"
#else

namespace GrAL {

/*! \ingroup complex2dcopy
 */
template<class Geom1, class G2, class Geom2>
extern  void 
ConstructGrid(Complex2D  & destG,
              Geom1      & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);

/*! \ingroup complex2dcopy
 */
template<class Geom1, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(Complex2D     & destG, 
               Geom1         & destGeom,
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 

/*! \ingroup complex2dcopy
 */
template<class Geom1, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(Complex2D     & destG, 
                Geom1         & destGeom,
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 

/*! \ingroup complex2dcopy
 */
template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Complex2D     & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 


} // namespace GrAL 


#endif

#endif
