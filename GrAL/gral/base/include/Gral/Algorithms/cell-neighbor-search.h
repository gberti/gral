#ifndef NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_H
#define NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
// template<class NBF, class CELLSET, class FACETMAP, class CGT>
// void CalculateNeighborCells(NBF             &  Nb,         // out
//	     		        CELLSET    const&  cell_set,   // in
//			        FACETMAP        &  facet_map,  // inout
//			        CGT        const&);            // in (only type used) 
//
// This template function calculates the neighborhood relations
// on a set of cells.
//
// Input:
//  * a map 'nbf': Cell x int -> cell_handle
//  * a set of cells  'cell_set' 
//  * a (possibly empty) mapping 'facet_map'
//    that maps vertex pairs to FacetOnCellIterators
//  * a traits type 'CGT' providing iterator types connected to 'CELLSET' type
//
// The algorithm works by inserting new facets into the table
// and extracting the corresponding counterparts from the table,
// if they are found.
//
// The FACETMAP  'facet_map' may already contain entries for facets
// which must have at most one adjacent cell in 'cell_set' .
// The case that exacly one cell is in 'cell_set' is of course
// the interesting one and corresponds to facets on the boundary.
// If called for 'cell_set' == C(G) then 'facet_map' == \emptyset.
//
// The algorithm is incremental in the sense that, given a cell_set C,
// and a disjoint partition C_1 \cup ... \cup C_n = C, 
// calling the algorithm n times with C_1, ..., C_n instead of C 
// yields the same result with the same time and memory complexity.
// 
// Output:
//   * nbf: the neighbor relations for cells in 'cell_set' that
//          correspond to internal facets are set in 'nbf', as well as those
//          corresponding to facets already present in 'facet_map'.
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
//   PRECONDITION:
//    FM1 \cap I = \emptyset 
//   POSTCONDITION:
//    FM2 \cap I = \emptyset
//    FM2 = (FM1 \ B) \cup (B \ FM1)
//
// CONSTRAINTS on template parameters:
//
//  * NBF
//     + nb_type& operator[](CGT::Cell C)
//       where nb_type satisfies:
//       - cell_handle& operator[](int nb)
//         nb \in [0, C.NumOfFacets() [
//     Examples: grid_function<CGT::Cell, cell_handle[3]>
//     (for a triangulation, C.NumOfFacets() == 3 \forall C)
//
//  * CGT (default: grid_types<CELLSET>)
//    + typedef Cell
//    + typedef CellIterator
//      - constructor CellIterator(CELLSET)
//    + typedef FacetOnCellIterator
//    + cell_handle handle(Cell)
//    + int         local (FacetOnCellIterator fc) 
//      (get local number n of facet fc, n \in [0, fc.TheCell().NumOfFacets() [ )
//    Examples: grid_types<CELLSET>
//
//  * CELLSET
//     + typedef CellIterator;
//     + CellIterator FirstCell();
//     Examples: Grids, Grid subranges
//
//  * FACETMAP
//     + typedef iterator
//     + iterator end();
//     + iterator find(FacetOnIterator);
//     + void     erase(iterator);
//     + FacetOnCellIterator& operator[](vtuple_2d);
//     Examples: hash_map<vtuple_2d<CELLSET>, FacetOnCellIterator>
//
//----------------------------------------------------------------

template<class NBF, class CELLSET, class FACETMAP, class CGT>
void CalculateNeighborCells(NBF             &  Nb,         // out
			    CELLSET    const&  cell_set,   // in
			    FACETMAP        &  facet_map,  // inout
			    CGT        const&);            // in (only type used) 

template<class NBF, class CELLSET, class FACETMAP>
void CalculateNeighborCells(NBF             &  Nb,         // out
			    CELLSET    const&  cell_set,   // in
			    FACETMAP        &  facet_map); // inout

template<class NBF, class CELLSET>
void CalculateNeighborCells(NBF             &  Nb,          // out
			    CELLSET    const&  cell_set);   // in

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Algorithms/generic/cell-neighbor-search.C"
#endif

#endif
