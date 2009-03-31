#ifndef GRAL_GB_COMPLEX2D_ADJACENCY_C
#define GRAL_GB_COMPLEX2D_ADJACENCY_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Grids/Complex2D/internal/adjacency.h"
#include "Gral/Base/vtuple2d.h"

namespace GrAL {

template<class CellSet, class FacetMap>
void CalculateNeighbourCells(        Complex2D&  G, 
			       const CellSet  &  cell_set,
			             FacetMap &  facet_map)
{

  typedef typename CellSet::CellIterator  SetCellIt;

  typedef typename FacetMap::iterator     MapIt;
  typedef grid_types<Complex2D>           gt;
  typedef typename gt::Cell               Cell;
  typedef typename gt::FacetOnCellIterator FacetOnCellIt;
  //  typedef typename gt::CellOnCellIterator CellNeighbourIt;
  
  friend_for_input gg(G); // gg == G + access to private routines
  
  typedef vtuple_2d<Complex2D> vtuple;
  SetCellIt c = cell_set.FirstCell();
  for(c= cell_set.FirstCell(); !c.IsDone(); ++c){
    Cell C(*c);
    FacetOnCellIt f(C.FirstFacet());
    for(; !f.IsDone();++f) {
      vtuple  facet(get_vertices(f));
      MapIt nb;
      if((nb = facet_map.find(facet)) != facet_map.end()){ 
        // facet found: nb has already been visited
        // do appropriate entries in the neighbourlists
        //  & remove facet from the map.
        FacetOnCellIt NbIt((*nb).second);
        gg.set_neighbour(f,    NbIt.TheCell());
        gg.set_neighbour(NbIt, f.   TheCell());
        //(int&)(*f._nb)      = G.handle(NbIt.TheCell()); // replace with call to
        //(int&)(*(NbIt._nb)) = G.handle(f.TheCell());    // internal fct of Complex2D
        facet_map.erase(nb);
      }
      else // 1st time this facet is encountered: add it to map
        facet_map[facet] = f ;
    } // for(f=C.FirstNeighbour();...
  } // for(c=FirstCell();...

  // all remaining map entries are on the boundary of cell_set
  // because they have been encountered exactly once.

}

} // namespace GrAL 

#endif
