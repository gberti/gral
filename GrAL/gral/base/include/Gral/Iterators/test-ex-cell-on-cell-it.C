#ifndef NMWR_BG_TEST_EXT_CELL_ON_CELL_IT_C
#define NMWR_BG_TEST_EXT_CELL_ON_CELL_IT_C

// $LICENSE


#include <iostream.h>
#include "Grids/common-grid-basics.h"
#include "Grids/Iterators/ex-cell-on-cell-it.h"

template<class Grid>
void test_extended_cell_on_cell_iterator(const Grid& g)
{
  typedef grid_types<Grid> gt;
  typedef typename gt::CellIterator CellIterator;

  typedef typename gt::CellOnCellIterator  DirCellOnCellIterator;
  typedef ExtendedCellOnCellIterator<Grid> ExtCellOnCellIt;
  typedef IndirectCellOnCellIterator<Grid> IndCellOnCellIt;
 
  for(CellIterator C = g.FirstCell(); ! C.IsDone(); ++C) {
    cerr << "Cell " << g.handle(*C)  << ":\n"
	 << "extended nbs:  ";
    for(ExtCellOnCellIt ec(*C); ! ec.IsDone(); ++ec)
      cerr <<  g.handle(*ec) << ' ';
    cerr << '\n';
    cerr << "dir/indir nbs: ";
    for(DirCellOnCellIterator dc = (*C).FirstCell(); ! dc.IsDone(); ++dc)
      cerr <<  g.handle(*dc) << ' ';
    for(IndCellOnCellIt ic(*C); ! ic.IsDone(); ++ic)
      cerr <<  g.handle(*ic) << ' ';
    cerr << '\n';
  }
}

#endif
