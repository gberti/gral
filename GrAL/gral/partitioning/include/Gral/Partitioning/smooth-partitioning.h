#ifndef NMWR_GB_GRID_SMOOTH_PARTITIONING
#define NMWR_GB_GRID_SMOOTH_PARTITIONING

#include "Grids/common-grid-basics.h"
#include "Grids/partitioning.h"

#include <map.h>

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//   author: Holger Kache
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// CONTENTS:
// ---------
// [1]  class smooth<partitioning<GRID>>
//
// DESCRIPTION
// -----------
// [1] After partitioning Grid G smooth-partitioning algorithm should
// be used for optimization of partitioning. The algorithm trys to 
// minimalize the cut-size of partitioning. This value gives an 
// global amount of quality of an partitioning where all parts got
// equal size. The cut-size can change for the better when swapping 
// cells to neighbour partitions.     
//
//-----------------------------------------------------------------

template <class GRID>
class smooth_partitioning {

private:
  
  typedef grid_types<GRID>                 gt;
  typedef typename gt::Cell                Cell;
  typedef typename gt::CellIterator        CellIterator;
  typedef typename gt::CellOnCellIterator  CellOnCellIterator;

  grid_function<Cell,int> numC;
  hash_map<Cell, int> cells;

  partitioning<GRID> *P;

public:

  // CONSTRUCTION
       smooth_partitioning(partitioning<GRID>  & PP);
  
  // DESTRUCTION
       ~smooth_partitioning();
       
void write_partitioning(partitioning<GRID> & P);

void statistics(const partitioning<GRID> & P);

void NumOfNeighbours(const partitioning<GRID> & P,
		     const Cell& C);
};

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "generic/smooth-partitioning.C"      
#endif


#endif
