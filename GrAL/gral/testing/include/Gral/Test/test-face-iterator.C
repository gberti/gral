#ifndef NMWR_GB_GRAL_TEST_FACE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_FACE_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-face-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G, class GT>
bool test_face_iterator(G const& g, std::ostream & out, GT);


template<class G>
bool test_face_iterator(G const& g, std::ostream & out)
{
  return test_face_iterator(g,out, grid_types<G>());
}

template<class G, class GT>
bool test_face_iterator(G const& g, std::ostream & out, GT)
{
  typedef GT       gt;
  typedef typename gt::FaceIterator FaceIterator;

  int v_cnt = 0;
  for(FaceIterator v(g); ! v.IsDone(); ++v, ++v_cnt) {
    ;
  }
  /*
  REQUIRE_ALWAYS(v_cnt == (int)g.NumOfFaces(), 
                 "v_cnt = " << v_cnt << " != g.NumOfFaces() = " << g.NumOfFaces() << '\n',1);
  */
  FaceIterator v;
  v = FaceIterator(g); // g.FirstFace();
  FaceIterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  v = FaceIterator(g); // g.FirstFace();
  w = v;
  if( ! v.IsDone()) {
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same face!\n",1);
  }
  return true;
}

#endif
