#ifndef NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_C
#define NMWR_GB_GRIDS_ALGORITHM_CELL_NEIGHBOR_SEARCH_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/common-grid-basics.h"
#include "Grids/Algorithms/cell-neighbor-search.h"
#include "Grids/vtuple2d.h"
#include "hash_map.h"

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
  typedef hash_map<vtuple_2d<CELLSET>, 
                   typename gt::FacetOnCellIterator> FACETMAP;
  FACETMAP facet_map;
  CalculateNeighborCells(Nb,cell_set,facet_map, gt()); 

  for(typename FACETMAP::const_iterator f = facet_map.begin();
      f != facet_map.end(); ++f)
    Nb[(*f).second] = gt::invalid_cell_handle(cell_set);
}


template<class NBF, class CELLSET, class FACETMAP, class CGT>
void CalculateNeighborCells(NBF           &  NB, 
			    CELLSET  const&  cell_set,
			    FACETMAP      &  facet_map,
			    CGT      const&)
{

  typedef typename CGT::CellIterator        CellIt;
  typedef typename CGT::Cell                Cell;
  typedef typename CGT::FacetOnCellIterator FacetOnCellIt;

  typedef typename FACETMAP::iterator        MapIt;
  typedef vtuple_2d<typename CGT::grid_type> vtuple;
  // must be equal to FACETMAP::data_type / result_type;

  CellIt c(cell_set);
  for(; !c.IsDone(); ++c){
    Cell C(*c);
    FacetOnCellIt f(C);
    for(; !f.IsDone();++f) {
      vtuple facet(get_vertices(f));
      MapIt nb;
      if((nb = facet_map.find(facet)) != facet_map.end()){ 
        // facet found: nb has already been visited
        // do appropriate entries in the neighborlists
        //  & remove facet from the map.
        FacetOnCellIt NbIt((*nb).second);
        NB[NbIt]    = CGT::handle(f.TheCell());
        NB[f   ]    = CGT::handle(NbIt.TheCell());

        facet_map.erase(nb);
      }
      else // 1st time this facet is encountered: add it to map
        facet_map[facet] = f ;
    } // for(; !f.IsDone(), ...
  } // for(; !c.IsDone(), ...

  // all remaining map entries are on the boundary of cell_set
  // because they have been encountered exactly once.

}

#endif
