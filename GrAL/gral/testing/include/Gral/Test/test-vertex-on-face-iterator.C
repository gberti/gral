#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_FACE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_VERTEX_ON_FACE_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-vertex-on-face-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G, class GT>
bool test_vertex_on_face_iterator(G const& g, std::ostream & out, GT);

template<class G>
bool test_vertex_on_face_iterator(G const& g, std::ostream & out)
{ return test_vertex_on_face_iterator(g, out, grid_types<G>()); }

template<class G, class GT>
bool test_vertex_on_face_iterator(G const& g, std::ostream & out, GT)
{
  typedef grid_types<G> gt;
  typedef typename gt::FaceIterator         FaceIterator;
  typedef typename gt::VertexOnFaceIterator VertexOnFaceIterator;

  for(FaceIterator c(g); ! c.IsDone(); ++c) {
    int v_cnt = 0;
    for(VertexOnFaceIterator vc(*c); ! vc.IsDone(); ++vc, ++v_cnt) {
    ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*c).NumOfVertices(), 
                   "v_cnt = " << v_cnt << " != c.NumOfVertices() = " 
                   << (*c).NumOfVertices() << '\n',1);

    VertexOnFaceIterator v;
    v = (*c).FirstVertex();
    VertexOnFaceIterator w = v;
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

#endif
