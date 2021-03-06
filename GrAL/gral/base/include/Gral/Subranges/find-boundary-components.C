#ifndef NMWR_GB_GRID_FIND_BOUNDARY_COMPONENTS_C
#define NMWR_GB_GRID_FIND_BOUNDARY_COMPONENTS_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Container/dummy-container.h"
#include "Gral/Subranges/find-boundary-components.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Iterators/boundary-iterator.h"
#include "Gral/Base/grid-functors.h"

namespace GrAL {

template<class GRID>
int count_boundary_facets(GRID const& G)
{
  typedef GRID                          grid_type;
  typedef grid_types<GRID>              gt;
  typedef typename gt::FacetIterator    FacetIterator;

  int nf = 0;
  for(FacetIterator f = G.FirstFacet(); ! f.IsDone(); ++f)
    if(G.IsOnBoundary(*f)) 
      ++nf;
  return nf;
}

template<class GRID>
int count_boundary_components(GRID const& G) {

  typedef GRID                          grid_type;
  typedef grid_types<GRID>              gt;
  typedef typename gt::Facet            Facet;

  int nc = -1;
  int dummy;
  dummy_container<Facet> dummyC;
  find_boundary_component_germs(G,dummyC,nc,dummy);

  return nc;
}

template<class GRID, class FACETSET>
void find_boundary_component_germs(GRID const& G, FACETSET & C)
{ 
  int dummy;
  find_boundary_component_germs(G,C,dummy,dummy);
}

template<class GRID, class FACETSET>
void find_boundary_component_germs(GRID const& G, FACETSET & C,
				   int& num_comps, int& num_facets)

{
  typedef GRID                          grid_type;
  typedef grid_types<GRID>              gt;
  typedef typename gt::Facet            Facet;
  typedef typename gt::FacetIterator    FacetIterator;
  typedef iscellinside_pred<grid_type>  inside;
  typedef BoundaryComponentEdgeIterator2D<grid_type, inside> BCFacetIterator;

  partial_grid_function<Facet,bool> marked(G,false);

  num_comps = num_facets = 0;
  for(FacetIterator f = G.FirstFacet(); ! f.IsDone(); ++f)
    if( (! marked(*f))  && (G.IsOnBoundary(*f))) {
      C.push_back(*f);
      num_comps++;
      BCFacetIterator bf(*f,inside(G));
      while(! bf.IsDone()) {
	num_facets++;
	marked[*bf] = true;
	++bf;
      }
    }
}

} // namespace GrAL 

#endif
