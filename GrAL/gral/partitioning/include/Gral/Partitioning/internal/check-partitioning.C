#ifndef NMWR_GB_CHECK_PARTITIONING_C
#define NMWR_GB_CHECK_PARTITIONING_C

// $LICENSE

/*! \file
 */

#include "Gral/Partitioning/partitioning.h"


// return the number of orphanes, that is, cells that are not
// connected to their partitions via at least one neighbour.
// Routine such as CreateCoarsePartitionGrid depend on a partition
// without orphanes. Unluckily, some partitioning tools seem to create
// partitions with orphanes.
// NOTE: a partition consisting of only one cell will be NOT be counted
// as orphane.
template<class Grid>
int check_partition_for_orphanes(partitioning<Grid> const& P)
{
  typedef Grid                            grid_type;
  typedef grid_types<Grid>                gt;
  typedef typename gt::CellIterator       CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;

  int orphane_cnt = 0;
  for(CellIterator C = P.TheGrid().FirstCell(); ! C.IsDone(); ++C) {
    bool orphane = true;
    int p = P.partition(*C);
    if( P.Range(p).size() != 1) { // skip one-cell partitions
      for(CellOnCellIterator CC = (*C).FirstCell(); ! CC.IsDone(); ++CC)
	if(P.partition(*CC) == p) {
	  orphane = false;
	  break;
	}
      if(orphane)
	orphane_cnt++;
    }
  }
  return orphane_cnt;
}

#endif
