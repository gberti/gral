#ifndef NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-vertex-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_vertex_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::VertexIterator VertexIterator;

  int v_cnt = 0;
  for(VertexIterator v(g); ! v.IsDone(); ++v, ++v_cnt) {
    ;
  }
  REQUIRE_ALWAYS(v_cnt == (int)g.NumOfVertices(), 
                 "v_cnt = " << v_cnt << " != g.NumOfVertices() = " << g.NumOfVertices() << '\n',1);

  VertexIterator v;
  v = g.FirstVertex();
  VertexIterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  if(g.NumOfVertices() > 0) {
    v = g.FirstVertex();
    w = v;
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same vertex!\n",1);
  }
  return true;
}

#endif
