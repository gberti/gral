#ifndef NMWR_GB_REMOVE_PARTITIONING_ORPHANES_C
#define NMWR_GB_REMOVE_PARTITIONING_ORPHANES_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Grids/Partitioning/remove-partitioning-orphanes.h"
#include "Grids/Partitioning/partitioning.h"

#include <iostream>

namespace GrAL {


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
	std::cerr << "orphane removed : " << P.TheGrid().handle(*C) << '\n'; 
       int pnb = P.partition(*(*C).FirstCell());
       P.set_partition(*C,pnb);
      }
    }
  }
}


} // namespace GrAL 


#endif
