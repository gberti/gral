#ifndef NMWR_GB_GRAL_TEST_EDGE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_EDGE_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-edge-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G, class GT>
bool test_edge_iterator(G const& g, ::std::ostream & out, GT);


template<class G>
bool test_edge_iterator(G const& g, ::std::ostream & out)
{
  return test_edge_iterator(g,out, grid_types<G>());
}

template<class G, class GT>
bool test_edge_iterator(G const& g, ::std::ostream & out, GT)
{
  typedef GT       gt;
  typedef typename gt::EdgeIterator EdgeIterator;

  int v_cnt = 0;
  for(EdgeIterator v(g); ! v.IsDone(); ++v, ++v_cnt) {
    ;
  }
  /*
  REQUIRE_ALWAYS(v_cnt == (int)g.NumOfEdges(), 
                 "v_cnt = " << v_cnt << " != g.NumOfEdges() = " << g.NumOfEdges() << '\n',1);
  */
  EdgeIterator v;
  v = EdgeIterator(g); // g.FirstEdge();
  EdgeIterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  v = EdgeIterator(g); // g.FirstEdge();
  w = v;
  if( ! v.IsDone()) {
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same edge!\n",1);
  }
  return true;
}

} // namespace GrAL 

#endif
