#ifndef NMWR_GB_GRAL_TEST_CELL_ITERATOR_C
#define NMWR_GB_GRAL_TEST_CELL_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-cell-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_cell_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::CellIterator CellIterator;

  int v_cnt = 0;
  for(CellIterator v(g); ! v.IsDone(); ++v, ++v_cnt) {
    ;
  }
 
  REQUIRE_ALWAYS(v_cnt == (int)g.NumOfCells(), 
                 "v_cnt = " << v_cnt << " != g.NumOfCells() = " << g.NumOfCells() << '\n',1);
 
  CellIterator v;
  v = g.FirstCell();
  CellIterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  v = g.FirstCell();
  w = v;
  if (! v.IsDone()) {
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same cell!\n",1);
  }
  return true;
}

#endif
