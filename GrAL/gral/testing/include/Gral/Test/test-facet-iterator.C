#ifndef NMWR_GB_GRAL_TEST_FACET_ITERATOR_C
#define NMWR_GB_GRAL_TEST_FACET_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-facet-iterator.h"
#include "Utility/pre-post-conditions.h"

template<class G>
bool test_facet_iterator(G const& g, std::ostream & out)
{
  typedef grid_types<G> gt;
  typedef typename gt::FacetIterator FacetIterator;

  int v_cnt = 0;
  for(FacetIterator v(g); ! v.IsDone(); ++v, ++v_cnt) {
    ;
  }
  /*
  REQUIRE_ALWAYS(v_cnt == (int)g.NumOfFacets(), 
                 "v_cnt = " << v_cnt << " != g.NumOfFacets() = " << g.NumOfFacets() << '\n',1);
  */
  FacetIterator v;
  v = g.FirstFacet();
  FacetIterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  v = g.FirstFacet();
  w = v;
  if (! v.IsDone()) {
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same facet!\n",1);
  }
  return true;
}

#endif
