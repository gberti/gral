#ifndef NMWR_GB_COMPLEX2D_ADJACENCY_H
#define NMWR_GB_COMPLEX2D_ADJACENCY_H


// $LICENSE

#include "Gral/Grids/Complex2D/complex2d.h"

//----------------------------------------------------------------
//
// This template function calculates the neighborhood relations
// on a set of cells.
//
// Input:
//  * a grid 'G' of type Complex2D
//  * a set of cells  'cell_set' \subset C(G)
//  * a (possibly empty) mapping 'facet_map'
//    that maps vertex pairs to CellOnCellIterators
//
// The algorithm works by inserting new facets into the table
// and extracting the corresponding counterparts from the table,
// if they are found.
//
// The FacetMap  'facet_map' may already contain entries for facets
// which have at most one adjacent cell in 'cell_set'.
// The case that exacly one cell is in 'cell_set' is of course
// the interesting one and corresponds to facets on the boundary.
// If called for 'cell_set' == C(G) then 'facet_map' == \emptyset.
// 
// Output:
//   * G: the neighbor relations for cells in 'cell_set' that
//        correspond to internal facets are set in 'G', as well as those
//        corresponding to facets in 'facet_map'.
//   * cell_set: unchanged
//   * facet_map contains all unmatched facets of F(cell_set), that
//     is facets on the boundary of 'cell_set' that have not been 
//     contained before.
//
// more formally:
// let
//   FC  = all facets of cell_set
//   I   = internal facets of cell_set
//   B   = boundary facets of cell_set (i.e. I \cup B = FC)
//   FM1 = facets in facet_map before
//   FM2 = facets in facet_map after
// Then
//   FM1 \cap I = \emptyset
//   FM2 \cap I = \emptyset
//   FM2 = (FM1 \ B) \cup (B \ FM1)
//
// CONSTRAINTS on template parameters:
//
//  * CellSet
//     + typedef CellIterator;
//     + CellIterator FirstCell();
//  * FacetMap
//     + typedef iterator
//     + iterator end();
//     + iterator find(CellOnCell2D_Iterator);
//     + void     erase(iterator);
//     + CellOnCell2D_Iterator& operator[](vtuple_2d);
//
//----------------------------------------------------------------

template<class CellSet, class FacetMap>
void CalculateNeighbourCells(Complex2D      &  G,          // inout
			     const CellSet  &  cell_set,   // out
			     FacetMap       &  facet_map); // inout

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/adjacency.C"
#endif

#endif
