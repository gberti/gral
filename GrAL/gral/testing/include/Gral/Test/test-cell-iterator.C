#ifndef NMWR_GB_GRAL_TEST_CELL_ITERATOR_C
#define NMWR_GB_GRAL_TEST_CELL_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-cell-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G>
bool test_cell_iterator(G const& g, ::std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::CellIterator CellIterator;

  int cnt = 0;
  for(CellIterator v(g); ! v.IsDone(); ++v, ++cnt) {
    typename gt::cell_handle h = v.handle();
    typename gt::Cell vv(v.TheGrid(), h);
    //    typename gt::Cell vv(v.TheGrid(), v.handle());

    REQUIRE_ALWAYS(vv == *v, "Cell constructed from handle differs!\n",1);

  }
 
  REQUIRE_ALWAYS(cnt == (int)g.NumOfCells(), 
                 "cnt = " << cnt << " != g.NumOfCells() = " << g.NumOfCells() << '\n',1);
 
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

} // namespace GrAL 

#endif
