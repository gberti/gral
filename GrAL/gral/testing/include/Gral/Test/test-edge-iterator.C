#ifndef NMWR_GB_GRAL_TEST_EDGE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_EDGE_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-edge-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_edge_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
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
  v = g.FirstEdge();
  EdgeIterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  v = g.FirstEdge();
  w = v;
  if( ! v.IsDone()) {
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same edge!\n",1);
  }
  return true;
}

#endif
