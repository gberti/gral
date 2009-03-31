#ifndef NMWR_GB_GRAL_TEST_EDGE_ON_FACE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_EDGE_ON_FACE_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-edge-on-face-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G, class GT>
bool test_edge_on_face_iterator(G const& g, ::std::ostream & out, GT);

template<class G>
bool test_edge_on_face_iterator(G const& g, ::std::ostream & out)
{  return test_edge_on_face_iterator(g, out, grid_types<G>()); }

template<class G, class GT>
bool test_edge_on_face_iterator(G const& g, ::std::ostream & out, GT)
{
  typedef grid_types<G> gt;
  typedef typename gt::FaceIterator         FaceIterator;
  typedef typename gt::EdgeOnFaceIterator   EdgeOnFaceIterator;

  for(FaceIterator c(g); ! c.IsDone(); ++c) {
    int v_cnt = 0;
    for(EdgeOnFaceIterator vc(*c); ! vc.IsDone(); ++vc, ++v_cnt) {
    ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*c).NumOfEdges(), 
                   "v_cnt = " << v_cnt << " != c.NumOfEdges() = " 
                   << (*c).NumOfEdges() << '\n',1);

    EdgeOnFaceIterator v;
    v = (*c).FirstEdge();
    EdgeOnFaceIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*c).NumOfVertices() > 0) {
      v = (*c).FirstEdge();
      w = v;
      ++v;
      for( ; !v.IsDone(); ++v, ++w) 
        REQUIRE_ALWAYS( (*v != *w), "Iterators point to same edge!\n",1);
    }
  }
  return true;
}

} // namespace GrAL 

#endif
