#ifndef NMWR_GB_REMOVE_PARTITIONING_ORPHANES_C
#define NMWR_GB_REMOVE_PARTITIONING_ORPHANES_C


// $LICENSE

/*! \file
 */

#include "Grids/Partitioning/remove-partitioning-orphanes.h"
#include "Grids/Partitioning/partitioning.h"


template<class GRID>
void
remove_orphanes(partitioning<GRID>& P)
{
  typedef GRID                            grid_type;
  typedef grid_types<GRID>                gt;
  typedef typename gt::CellIterator       CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;

  for(CellIterator C = P.TheGrid().FirstCell(); ! C.IsDone(); ++C) {
    bool orphane = true;
    int p = P.partition(*C);
    if( P.Range(p).size() != 1) { // skip one-cell partitions
      for(CellOnCellIterator CC = (*C).FirstCell(); ! CC.IsDone(); ++CC)
        if(P.partition(*CC) == p) {
          orphane = false;
          break;
        }
      if(orphane) {
	cerr << "orphane removed : " << P.TheGrid().handle(*C) << '\n'; 
       int pnb = P.partition(*(*C).FirstCell());
       P.set_partition(*C,pnb);
      }
    }
  }
}



#endif
