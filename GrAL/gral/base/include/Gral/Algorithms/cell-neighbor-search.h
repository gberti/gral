#ifndef NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_H
#define NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_H


// $LICENSE

/*! \defgroup  cell_neighbor_search
   \ingroup combinatoric_grid_algorithms
   \see combinatoric_grid_algorithms
    
   A collection of routines for finding
   cell neighborhood relationsships and boundary facets.
*/

/*! \brief calculate the neighborhood relationship on a set of cells.
   \ingroup cell_neighbor_search

  
  The algorithm works by inserting new facets into the table
  and extracting the corresponding counterparts from the table,
  if they are found.

  A sample use can be found in test-triang2d-construct.C.

  \param   nbf [OUT]: A mapping Cell x int -> cell_handle
  \param   cell_set [IN]:  set of cells on which to calculate neighbors
  \param   facet_map [INOUT]: a (possibly empty) mapping
    from facet vertex sets to FacetOnCellIterators
  \param   CGT [TYPE ONLY]  a traits type providing iterator types
                              connected to  type \c CELLSET 


 \b Input
   - \c facet_map may already contain entries for facets
     which must have at most one adjacent cell in 'cell_set' .
     The case that exacly one cell is in 'cell_set' is of course
     the interesting one and corresponds to facets on the boundary.
     If \c cell_set contains all cells of a grid 
     then  \c facet_map should be empty.
   - \c cell_set contains a subset of the cells of a grid on which
     the algorithm will be executed.
     The grid underlying \c cell_set must have unique vertex sets for facets.
     The algorithm is incremental in the sense that, given a cell_set C,
     and a disjoint partition \f$ C_1 \cup ... \cup C_n = C \f$, 
     calling the algorithm n times with \f$ C_1, ..., C_n \f$ instead of \f$ C \f$
     yields the same result with the same time and memory complexity.
 
 \b Output
    - \c nbf: the neighbor relations for cells in \c cell_set that
          correspond to internal facets are set in \c nbf, as well as those
          corresponding to facets already present in 'facet_map'.
    - \c facet_map contains all unmatched facets of F(cell_set), that
         is, facets on the boundary of  \c cell_set that have not been 
         contained in \c facet_map before.

 More formally: Let
   - FC  = all facets of cell_set
   - I   = internal facets of cell_set
   - B   = boundary facets of cell_set (i.e. \f$ I \cup B = FC \f$)
   - FM1 = facets in facet_map before
   - FM2 = facets in facet_map after

 Then
  <UL> 
   <LI>  PRECONDITION:
     <UL>
       <LI>  \f$ FM1 \cap I = \emptyset \f$ 
     </UL>
   <LI>  POSTCONDITION:
     <UL>
       <LI> \f$ FM2 \cap I = \emptyset \f$
       <LI> \f$ FM2 = (FM1 \ B) \cup (B \ FM1) \f$
     </UL>
  </UL>


 \templateparams

  - \c NBF
     - <tt> nb_type& operator[](CGT::Cell C) </tt>
        where nb_type satisfies:
         <tt>  cell_handle& operator[](int nb) </tt>
         \f$ nb \in [0, C.NumOfFacets() [ \f$
     - \b Examples: <tt> grid_function<CGT::Cell, cell_handle[3]> </tt>
     (for a triangulation, <tt> c.NumOfFacets() == 3 </tt> \f$ \forall \f$ cells c)
  - \c CGT (default: \c grid_types<CELLSET>)
     -  type \c Cell 
     -  type \c CellIterator
     -  type \c FacetOnCellIterator
     - <tt>   int  local (FacetOnCellIterator fc)  </tt>
      (get local number n of facet fc, 
       n \f$ \in \f$ [0, fc.TheCell().NumOfFacets() [ )
     - \b Examples: grid_types<CELLSET>
  - \c CELLSET
     - type \c CellIterator 
     - <tt>  CellIterator FirstCell() </tt>
     - \b Examples: Grids, Grid subranges
  - \c FACETMAP
     - type \c  iterator  
     - <tt> iterator end() </tt>
     - <tt> iterator find(vtuple) </tt>
     - <tt> void     erase(iterator) </tt>
     - <tt> CGT::FacetOnCellIterator& operator[](vtuple) </tt>
     - \b Examples: \c hash_map<vtuple, CGT::FacetOnCellIterator>
     - \b Note: \c vtuple is short for \c vtuple<CELLSET>

*/

template<class NBF, class CELLSET, class FACETMAP, class CGT>
void CalculateNeighborCells(NBF             &  Nb,         // out
			    CELLSET    const&  cell_set,   // in
			    FACETMAP        &  facet_map,  // inout
			    CGT        const&);            // in (only type used) 

/*! \brief convenience wrapper for CalculateNeighborCells
     \ingroup combinatoric_grid_algorithms
 
 */
template<class NBF, class CELLSET, class FACETMAP>
void CalculateNeighborCells(NBF             &  Nb,         // out
			    CELLSET    const&  cell_set,   // in
			    FACETMAP        &  facet_map); // inout

/*! \brief convenience wrapper for CalculateNeighborCells
   \ingroup cell_neighbor_search

 */
template<class NBF, class CELLSET>
void CalculateNeighborCells(NBF             &  Nb,          // out
			    CELLSET    const&  cell_set);   // in

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Algorithms/internal/cell-neighbor-search.C"
#endif

#endif
