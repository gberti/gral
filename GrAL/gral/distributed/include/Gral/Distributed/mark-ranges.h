#ifndef NMWR_GB_COMPOSITE_MARK_LAYERS_H
#define NMWR_GB_COMPOSITE_MARK_LAYERS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
// 
//  Routines that help constructing a composite grid
//  from a partitioned grid.
// 
//  CONTENTS:
//  --------- 
//
//  template<class FacetRange, class VertexRange, class CellRange, 
//           class OvlpPattern, class Pred>
//  void mark_layers(FacetRange    boundary_f,      // in
//     	 	     VertexRange&  v_layer,         // out
//		     CellRange  &  c_layer,         // out
// 		     OvlpPattern   ovlppattern,     // in
//		     Pred          inside)          // in
//
//----------------------------------------------------------------


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
		 OvlpPattern const &  ovlppattern,
		 Pred          inside);

//----------------------------------------------------------------
//
// collect the private elements:
// G := priv.TheGrid() = exposed.TheGrid() = ... = copied.TheGrid()
// priv = range(eit) \setminus  ( exposed \cup shared \cup copied )
//
//----------------------------------------------------------------

template<class ERange, class Eiter>
void mark_private_range(ERange& priv, 
			Eiter eit,
			const ERange& exposed, 
			const ERange& shared, 
			const ERange& copied);

#ifdef  NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mark-ranges.C"
#endif

#endif
