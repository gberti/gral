#ifndef GRAL_GB_MEASUREMENT_CHECK_EDGE_FACET_INTERSECTIONS_C
#define GRAL_GB_MEASUREMENT_CHECK_EDGE_FACET_INTERSECTIONS_C

// $LICENSE_NEC

#include "Gral/Measurement/check-edge-facet-intersections.h"
#include "Gral/Geometries/intersections.h"

#include "Gral/Base/predicates.h" // incident

#include <iostream>

template<class GRID, class GEOM, class GT>
void
check_edge_facet_intersections<GRID,GEOM,GT>::
find_non_incident_facet_edge_pairs()
{

  // for each facet of each cell, we find edges of the same cell
  // not incident to the facet.
  // It suffices to do so for each archetype once.
  for(typename gt::ArchetypeIterator  
      a = TheGrid().BeginArchetype();
      a != TheGrid().EndArchetype(); 
      ++a) {
    arch_non_incident_edges.push_back(arch_cell_edge_range(*a));
    for(typename agt::CellIterator aC(*a); ! aC.IsDone(); ++aC) {
      arch_non_incident_edges.back()[*aC] = arch_edge_range(*a);
      for(typename agt::EdgeIterator aE(*a); !aE.IsDone(); ++aE)
	if(! incident((*aE).V1(), *aC) && ! incident((*aE).V2(), *aC) )
	  arch_non_incident_edges.back()[*aC].push_back(*aE);
    }
  }
}


template<class GRID, class GEOM, class GT>
void 
check_edge_facet_intersections<GRID,GEOM,GT>::check_all_cells()
{
  find_non_incident_facet_edge_pairs();

  // Would be more consistent: typedef grid_types<edge_range_type> rangegt;
  typedef typename arch_edge_range::EdgeIterator RangeEdgeIterator;
 

  edge_facet_intersection<geom_type> intersection(*geom);


  for(typename gt::CellIterator c(TheGrid()); !c.IsDone(); ++c) {
    typename gt::archetype_handle a = TheGrid().archetype_of(c.handle());
      arch_cell_edge_range const&  ni_edges = arch_non_incident_edges[a];    

    for(typename gt::FacetOnCellIterator fc(*c); ! fc.IsDone(); ++fc) {
      for(RangeEdgeIterator ef = ni_edges(fc.ArchetypeCell()).FirstEdge(); ! ef.IsDone(); ++ef) {
        typename gt::Edge e = (*c).E(*ef);
	if(intersection(e,*fc)) {
	  num_intersections_c[*c ]++;
	  num_intersections_e[e]++;
	  num_intersections_f[*fc]++;
	  std::cout << "intersection found: " << intersection.location() << "\n";
	}
      }
      std::cout << "\n";
    }	
  }

}

#endif



