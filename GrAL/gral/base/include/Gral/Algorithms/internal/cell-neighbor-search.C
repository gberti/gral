#ifndef NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_C
#define NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_C


// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Algorithms/cell-neighbor-search.h"

#include "Gral/Base/vertex-set.h"

#include "Container/my-hash-map.h"

namespace GrAL {

//----------------------------------------------------------------
//
// Outline of algorithm:
//
// Loop over all facets of all cells in cell_set,
// look for the corresponding sorted vertex set in facet_map.
// If it is there, a pair of adjacent cells has been found.
// Else, the vertex set is inserted into the map.
//
//----------------------------------------------------------------

template<class NBF, class CELLSET, class FACETMAP, class CGT>
void CalculateNeighborCells(NBF           &  NB, 
			    CELLSET  const&  cell_set,
			    FACETMAP      &  facet_map,
			    CGT      const&);


template<class NBF, class CELLSET, class FACETMAP>
void CalculateNeighborCells(NBF             &  Nb,         // out
			    CELLSET    const&  cell_set,   // in
			    FACETMAP        &  facet_map)  // inout
{ CalculateNeighborCells(Nb,cell_set,facet_map, grid_types<CELLSET>()); }

template<class NBF, class CELLSET>
void CalculateNeighborCells(NBF             &  Nb,          // out
			    CELLSET    const&  cell_set)    // in
{ 
  typedef grid_types<CELLSET> gt;
  typedef vertex_set<typename gt::Facet> vertex_set_type;
  typedef ::STDHASH::hash_map<vertex_set_type,
                              typename gt::FacetOnCellIterator> FACETMAP;
  FACETMAP facet_map;
  CalculateNeighborCells(Nb,cell_set,facet_map, gt()); 

  for(typename FACETMAP::const_iterator f = facet_map.begin();
      f != facet_map.end(); ++f) {
    // gt::outer_cell() ?
    Nb[(*f).second] = gt::invalid_cell_handle(cell_set);
  }
}


template<class NBF, class CELLSET, class FACETMAP, class CGT>
void CalculateNeighborCells(NBF           &  NB, 
			    CELLSET  const&  cell_set,
			    FACETMAP      &  facet_map,
			    CGT      const&)
{

  //  typedef typename CGT::CellIterator        CellIt;
  typedef typename CELLSET::CellIterator    CellIt;
  typedef typename CGT::Cell                Cell;
  typedef typename CGT::FacetOnCellIterator FacetOnCellIt;

  typedef typename FACETMAP::iterator        MapIt;

  typedef vertex_set<typename CGT::Facet>    vtuple;
  // must be equal to FACETMAP::key_type 

  CellIt c(cell_set.FirstCell());
  for(; !c.IsDone(); ++c){
    Cell C(*c);
    FacetOnCellIt f(C);
    for(; !f.IsDone();++f) {
      vtuple facet(*f);
      MapIt nb;
      if((nb = facet_map.find(facet)) != facet_map.end()){ 
        // facet found: nb has already been visited
        // do appropriate entries in the neighborlists
        //  & remove facet from the map.
        FacetOnCellIt NbIt((*nb).second);
        // NB[NbIt] = 0;
	typename CGT::cell_handle fh = f.TheCell().handle();
        NB[NbIt]    = fh;
        NB[f   ]    = NbIt.TheCell().handle();

        facet_map.erase(nb);
      }
      else // 1st time this facet is encountered: add it to map
        facet_map[facet] = f ;
    } // for(; !f.IsDone(), ...
  } // for(; !c.IsDone(), ...

  // all remaining map entries are on the boundary of cell_set
  // because they have been encountered exactly once.

}

} // namespace GrAL 

#endif
