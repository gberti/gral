#ifndef NMWR_GB_COMPOSITE_MARK_LAYERS_C
#define NMWR_GB_COMPOSITE_MARK_LAYERS_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Distributed/mark-ranges.h"


#include "Grids/multi-grid-functions.h"
#include "Grids/layered-subrange.h"
#include "Grids/Algorithms/adjacency-hulls.h"
#include "Grids/Algorithms/reduction.h"

//----------------------------------------------------------------
// 
// Mark cells on partition boundary according to pattern.
// The facets in boundary_f contain the complete boundary of
// the partition with respect to other partitions (i.e. they
// are not part of the underlying grids boundary).
// The Predicate inside controls if cells are to be collected
// inside (exposed) or outside (copied)  the partition.
//
// PRECONDITIONS:
// --------------
// inside(f.C1()) <=>  ! inside(f.C2()) \forall f \in boundary_f
//
// POSTCONDITIONS:
// ---------------
// \forall C \in c_layers : inside(C) = true.
//
// ALGORITHM:
// ---------
// This routine essentially branches to mark_on_vertices or
// mark_on_cells, according to pattern, after generating the
// proper starting sets ("seeds").
//
//----------------------------------------------------------------

template<class FacetRange, 
         class CellRange, 
         class OvlpPattern, class Pred>
void mark_layers(FacetRange    boundary_f,      
		 CellRange  &  c_layer,
		 OvlpPattern const &  ovlpp,
		 Pred          inside)
{
  typedef typename FacetRange::grid_type grid_type;
   OvlpPattern ovlppattern=ovlpp;
   grid_element_tag ovtag = ovlppattern.front();
   ovlppattern.pop();
   
   partial_multi_grid_function<grid_type,int> visited(boundary_f.TheGrid(),0);
   int level = 1;
   layered_vertex_range<grid_type> dummy_v_layer(c_layer.TheGrid()); // the vertex range to be calculated
   // is just the vertex ajaceny hull of c_layer minus the shared vertices
   layered_cell_range<grid_type>   tmp_c_layer(c_layer.TheGrid());
  
   if    (ovtag == vertex_tag) {
     range_assign(visited,VerticesOfFacets(boundary_f).FirstVertex(),level);
     ++level;
     mark_on_vertices(VerticesOfFacets(boundary_f).FirstVertex(),
		      dummy_v_layer, tmp_c_layer,
		      visited,ovlppattern,level,inside);
   }
   else if (ovtag == cell_tag) {
     // seed cells are those cells adjacent to facets in seed_f, that
     // do NOT satisfy inside(), such that their opposite cells (w/r to seed_f)
     // will be the first cell layer determined.
     CellRange seed_cells(boundary_f.TheGrid());
     for(typename FacetRange::ElementIterator f = boundary_f.FirstElement(); !f.IsDone(); ++f) {
       REQUIRE((! boundary_f.TheGrid().IsOnBoundary(*f)),"Partition facet on boundary!\n",1);
       if(inside((*f).C1())) {
	 seed_cells.push_back((*f).C2());
	 visited[(*f).C2()] = level;
       }
       else {
	 seed_cells.push_back((*f).C1());
	 visited[(*f).C1()] = level;
       }
     }
     ++level;
     mark_on_cells(seed_cells.FirstElement(),
		   dummy_v_layer,tmp_c_layer,
		   visited, ovlppattern,level,inside);
   }
  
   // there could be "make_layered(range)" to add layer functionality
   // making this tmp & copy approach superflous
   copy(tmp_c_layer.begin(), tmp_c_layer.end(), back_inserter(c_layer.TheContainer()));

}

// collect the private elements:
// G := priv.TheGrid() = exposed.TheGrid() = ... = copied.TheGrid()
// priv = Elt(G) \ ( exposed \cup shared \cup copied )
template<class ERange, class Eiter>
void mark_private_range(ERange& priv, Eiter eit,
			const ERange& exposed, const ERange& shared, const ERange& copied)
{
  typedef typename ERange::Element Element;
  //typedef element_traits<Element>  et;
  //typedef typename et::ElementIterator ElementIterator;

  partial_grid_function<Element,bool> is_private(priv.TheGrid(),true); 
  range_assign(is_private, exposed.FirstElement(),false);
  range_assign(is_private, shared .FirstElement(),false);
  range_assign(is_private, copied .FirstElement(),false);
  //  for(ElementIterator e = et::FirstElement(priv.TheGrid()); ! e.IsDone(); ++e)
  while (! eit.IsDone()) {
    if(is_private(*eit))
      priv.push_back(*eit);
    ++eit;
  }

}

#endif
