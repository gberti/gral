#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_CELL_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ON_CELL_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-vertex-on-cell-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_vertex_on_cell_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::CellIterator         CellIterator;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  for(CellIterator c(g); ! c.IsDone(); ++c) {
    int v_cnt = 0;
    for(VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc, ++v_cnt) {
    ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*c).NumOfVertices(), 
                   "v_cnt = " << v_cnt << " != c.NumOfVertices() = " 
                   << (*c).NumOfVertices() << '\n',1);

    VertexOnCellIterator v;
    v = (*c).FirstVertex();
    VertexOnCellIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      REQUIRE_ALWAYS( &(v.TheGrid()) == &(c.TheGrid()), "",1);
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*c).NumOfVertices() > 0) {
      v = (*c).FirstVertex();
      w = v;
      ++v;
      for( ; !v.IsDone(); ++v, ++w) 
        REQUIRE_ALWAYS( (*v != *w), "Iterators point to same vertex!\n",1);
    }
  }
  return true;
}

#endif
