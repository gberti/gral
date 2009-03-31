#ifndef GRAL_BASE_GB_GRID_FIND_BOUNDARY_COMPONENTS_H
#define GRAL_BASE_GB_GRID_FIND_BOUNDARY_COMPONENTS_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file find-boundary-components.h
    \see Module boundaryalgorithms
 */

namespace GrAL {

/*! \defgroup boundaryalgorithms Algorithms for exploring Grid Boundaries
    \ingroup boundary
    
    The algorithms in this module count and find boundary facets and connected components.

    \todo The algorithms could take an additional parameter \c IsOnBd
    which is a predicate deciding whether a $GrAL Facet is on the boundary.
    Then one could handle also internal boundaries, for example
    between two partitions.
    \todo Test

    \see Module \ref boundaryrange
    \see Module \ref boundarycompiter
 */

/*! \ingroup boundaryalgorithms 
    \brief Count the number of boundary facets.

    Runtime is linear in the number of facets of \c G.
    \see \ref boundaryalgorithms  module
 */    
template<class GRID>
int count_boundary_facets(GRID const& G);

/*! \ingroup boundaryalgorithms
    \brief Count the number of boundary components

    Runtime is linear in the number of facets of \c G.
    \see \ref boundaryalgorithms  module
 */    
template<class GRID>
int count_boundary_components(GRID const& G);


/*! \ingroup boundaryalgorithms 
    \brief Find a facet for each component of the boundary of G
    \see \ref boundaryalgorithms  module
*/
template<class GRID, class FACETSET>
void find_boundary_component_germs(GRID const& G, FACETSET & C);

/*! \ingroup boundaryalgorithms 
    \brief Find a facet for each component of the boundary of G,
    and number of boundary components and facets.
    \see \ref boundaryalgorithms  module
  
*/
template<class GRID, class FACETSET>
void find_boundary_component_germs(GRID const& G, FACETSET & C,
				   int& num_comps, int& num_facets);
} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Subranges/find-boundary-components.C"
#endif

#endif
