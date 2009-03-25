#ifndef NMWR_GB_COMPOSITE_GRID_C
#define NMWR_GB_COMPOSITE_GRID_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Distributed/composite-grid.h"
#include <iostream>
//#include "IO/iomgr.h"

namespace GrAL {

template<class Grid, class GF>
void enumerate(const Grid& G, GF& f, int offset /* = 0 */)
{
  typedef typename GF::element_type elt_t;
  //  typedef tp<elt_t>                 tpE;
  typedef element_traits<elt_t>         et;
  typedef typename  et::ElementIterator Eit;

  int cnt = offset;
  for(Eit e = et::FirstElement(G); ! e.IsDone(); ++e) {
   f[*e] = cnt;
   ++cnt;
  }
  
}

template<class CoarseG, class FineG>
void check_composite_grid(const CompositeGrid<CoarseG,FineG>& CG)
{
  typedef CompositeGrid<CoarseG,FineG> cg_type;
  //  typedef typename cg_type::coarse_grid_type coarse_grid_type;
  typedef CoarseG coarse_grid_type;
  typedef grid_types<coarse_grid_type> cgt;
  typedef typename cgt::Cell         Cell;
  typedef typename cgt::CellIterator CellIterator; 
  typedef typename cg_type::NeighbourIterator NeighbourIterator;

  grid_function<Cell,int> Num(CG.TheCoarseGrid());
  enumerate(CG.TheCoarseGrid(),Num,0);
  for(CellIterator C = CG.TheCoarseGrid().FirstCell(); ! C.IsDone(); ++C) {
    std::cout << "-------------------------------------------------\n"
		  << "checking cell " << Num(*C) << "\n\n";
    for(NeighbourIterator Nb = CG.FirstNeighbour(*C); ! Nb.IsDone(); ++Nb) {
       std::cout << "   |CopiedVertices(" << Num(*Nb) << "," << Num(*C) << ")| = " 
		    <<  CG.Overlap(*Nb).vertices(*C ).copied() .size() << " = "
		    <<  CG.Overlap(*C).vertices(*Nb ).exposed() .size() << " = "
		    << "|ExposedVertices(" << Num(*C ) << "," << Num(*Nb) << ")|\n"; 
      
       std::cout << "   |CopiedCells(" << Num(*Nb) << "," << Num(*C) << ")| = " 
		    <<  CG.Overlap(*Nb).cells(*C ).copied() .size() << " = "
		    <<  CG.Overlap(*C).cells(*Nb ).exposed() .size() << " = "
		    << "|ExposedCells(" << Num(*C ) << "," << Num(*Nb) << ")|\n"; 
      
       std::cout << "   |SharedVertices(" << Num(*Nb) << "," << Num(*C) << ")| = " 
		    <<  CG.Overlap(*Nb).vertices(*C ).shared() .size() << " = "
		    <<  CG.Overlap(*C).vertices(*Nb ).shared() .size() << " = "
		    << "|SharedVertices(" << Num(*C ) << "," << Num(*Nb) << ")|\n"; 
      
       std::cout << "   |SharedCells(" << Num(*Nb) << "," << Num(*C) << ")| = " 
		    <<  CG.Overlap(*Nb).cells(*C ).shared() .size() << " = "
		    <<  CG.Overlap(*C).cells(*Nb ).shared() .size() << " = "
		    << "|SharedCells(" << Num(*C ) << "," << Num(*Nb) << ")|\n"; 


    }
     std::cout << "-------------------------------------------------\n";

  }
  
}

} // namespace GrAL 

#endif
