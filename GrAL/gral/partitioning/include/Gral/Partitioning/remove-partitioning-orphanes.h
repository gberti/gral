#ifndef NMWR_GB_REMOVE_PARTITIONING_ORPHANES_H
#define NMWR_GB_REMOVE_PARTITIONING_ORPHANES_H


// $LICENSE

//----------------------------------------------------------------
//
// void remove_orphanes(partitioning<GRID>& P):
//
//  make all partitions in P facet-connected, that is,
//  for any cell c that has no other cell of the same partition
//  as neighbor, change the partition of c to that of one of its
//  neighbors.
//
//----------------------------------------------------------------

template<class GRID> class partitioning;

template<class GRID>
extern void
remove_orphanes(partitioning<GRID>& P);

#ifdef  NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/remove-partitioning-orphanes.C"
#endif


#endif
