#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_CELL_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ON_CELL_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-vertex-on-cell-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G, class GT>
bool test_vertex_on_cell_iterator(G const& g, ::std::ostream & out, GT);

template<class G>
bool test_vertex_on_cell_iterator(G const& g, ::std::ostream & out)
{ return  test_vertex_on_cell_iterator(g, out, grid_types<G>()); }

template<class G, class GT>
bool test_vertex_on_cell_iterator(G const& g, ::std::ostream & out, GT)
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
      ref_ptr<G const> Gv(v.TheGrid());
      ref_ptr<G const> Gc(c.TheGrid());
      REQUIRE_ALWAYS( &(*Gv) == &(*Gc), "",1);
      //    REQUIRE_ALWAYS( &(v.TheGrid()) == &(c.TheGrid()), "",1);
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

} // namespace GrAL 

#endif
