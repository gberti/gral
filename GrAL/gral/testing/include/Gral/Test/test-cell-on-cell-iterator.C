#ifndef NMWR_GB_GRAL_TEST_CELL_ON_CELL_ITERATOR_C
#define NMWR_GB_GRAL_TEST_CELL_ON_CELL_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-cell-on-cell-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G, class GT>
bool test_cell_on_cell_iterator(G const& g, std::ostream & out, GT);

template<class G>
bool test_cell_on_cell_iterator(G const& g, std::ostream & out)
{ return test_cell_on_cell_iterator(g, out, grid_types<G>()); }

template<class G, class GT>
bool test_cell_on_cell_iterator(G const& g, std::ostream & out, GT)
{
  typedef GT gt;
  typedef typename gt::CellIterator        CellIterator;
  typedef typename gt::CellOnCellIterator  CellOnCellIterator;

  for(CellIterator c(g); ! c.IsDone(); ++c) {
    int v_cnt = 0;
    for(CellOnCellIterator vc(*c); ! vc.IsDone(); ++vc, ++v_cnt) {
      ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*c).NumOfCells(), 
                   "v_cnt = " << v_cnt << " != c.NumOfCells() = " 
                   << (*c).NumOfCells() << '\n',1);

    CellOnCellIterator v;
    v = (*c).FirstCell();
    CellOnCellIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*c).NumOfCells() > 0) {
      v = (*c).FirstCell();
      w = v;
      ++v;
      for( ; !v.IsDone(); ++v, ++w) 
        REQUIRE_ALWAYS( (*v != *w), "Iterators point to same cell!\n",1);
    }
  }
  return true;
}

#endif
