#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_EDGE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ON_EDGE_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-vertex-on-edge-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_vertex_on_edge_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::EdgeIterator         EdgeIterator;
  typedef typename gt::VertexOnEdgeIterator VertexOnEdgeIterator;

  for(EdgeIterator f(g); ! f.IsDone(); ++f) {
    int v_cnt = 0;
    for(VertexOnEdgeIterator vf(*f); ! vf.IsDone(); ++vf, ++v_cnt) {
      ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*f).NumOfVertices(), 
                   "v_cnt = " << v_cnt << " != f.NumOfVertices() = " 
                   << (*f).NumOfVertices() << '\n',1);

    VertexOnEdgeIterator v;
    v = (*f).FirstVertex();
    VertexOnEdgeIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      REQUIRE_ALWAYS( &(v.TheGrid()) == &(f.TheGrid()), "",1);
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*f).NumOfVertices() > 0) {
      v = (*f).FirstVertex();
      w = v;
      ++v;
      for( ; !v.IsDone(); ++v, ++w) 
        REQUIRE_ALWAYS( (*v != *w), "Iterators point to same vertex!\n",1);
    }
  }
  return true;
}

#endif