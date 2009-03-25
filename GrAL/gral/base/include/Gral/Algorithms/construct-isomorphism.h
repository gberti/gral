#ifndef GRAL_GB_ALGORITHM_CONSTRUCT_ISOMORPHISM_H
#define GRAL_GB_ALGORITHM_CONSTRUCT_ISOMORPHISM_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Algorithms/internal/construct-isomorphism.C"

#else

namespace GrAL {

/*! \defgroup construct_isomorphism_mod Construct Grid Isomorphisms

    \ingroup gridmorphisms
    \ingroup combinatoric_grid_algorithms
    
    \brief Algorithms for finding an isomorphism between two grids.

    Grid Isomorphisms are bijective mappings between grids, respecting
    incidence relationships. Two isomorphic grids are indistinguishable
    from a combinatorial point of view.
 */

/*! \brief Construct an isomorphism between two grids.
    \ingroup construct_isomorphism_mod
    \partof  construct_isomorphism_mod

    Return value is true on successful construction,
    else false.

   \see \ref test-construct-isomorphism.C for an example

    \bug It does not really try all possible isomorphisms.
    If an isomorphism of 2 cells does not extend to an isomorphism
    of the whole grid, it does not try other isomorphisms of the same
    pair of cell. Thus, it cannot construct an isomorphism of a
    Cartesian 2x1 grid to a 1x2 grid.
*/
template<class GRID1, class GRID2, 
         class GT1,   class GT2,
         class PHI>
bool construct_isomorphism(GRID1 const& G1,
			   GRID2 const& G2,
			   GT1   gt1,                // only type used
			   GT2   gt2,                //    
			   PHI        & phi);   // OUT

/*! \brief Convenience wrapper for general construct_isomorphism 

    \ingroup construct_isomorphism_mod
    \partof  construct_isomorphism_mod
 */
template<class GRID1, class GRID2, 
         class PHI>
bool construct_isomorphism(GRID1 const& G1,
			   GRID2 const& G2,
			   PHI        & phi);  // OUT


/*! \brief Extend a given isomorphism to the whole grid.
    \ingroup construct_isomorphism_mod
    \partof  construct_isomorphism_mod

    Return value is true on successful extension to the whole
    grid (more precisely, to the component containing c1).
    It is false if this failed. In this case, phi contains
    the partial morphism constructed so far.
 */

template<class CELL1, class CELL2, class GRID1, class GRID2, class PHI>
bool extend_isomorphism(CELL1 const& c1,
			CELL2 const& c2,
			GRID1 const& G1,
			GRID2 const& G2,
			PHI        & phi);

} // namespace GrAL 

#endif // NMWR_INCLUDE_TEMPLATE_DEFS

#endif
