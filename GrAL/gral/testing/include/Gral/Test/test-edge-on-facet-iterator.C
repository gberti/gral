#ifndef NMWR_GB_GRAL_TEST_EDGE_ON_FACET_ITERATOR_C
#define NMWR_GB_GRAL_TEST_EDGE_ON_FACET_ITERATOR_C

// $LICENSE

#include "Gral/Test/test-edge-on-facet-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G, class GT>
extern bool test_edge_on_facet_iterator(G const& g, ::std::ostream & out, GT);

template<class G>
bool test_edge_on_facet_iterator(G const& g, ::std::ostream & out)
{  return test_edge_on_facet_iterator(g, out, grid_types<G>()); }


template<class G, class GT>
extern bool test_edge_on_facet_iterator(G const& g, ::std::ostream & out, GT)
{
  typedef grid_types<G> gt;
  typedef typename gt::FacetIterator         FacetIterator;
  typedef typename gt::EdgeOnFacetIterator EdgeOnFacetIterator;

  for(FacetIterator f(g); ! f.IsDone(); ++f) {
    int v_cnt = 0;
    for(EdgeOnFacetIterator ef(*f); ! ef.IsDone(); ++ef, ++v_cnt) {
    ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*f).NumOfEdges(), 
                   "v_cnt = " << v_cnt << " != f.NumOfEdges() = " 
                   << (*f).NumOfEdges() << '\n',1);

    EdgeOnFacetIterator v;
    v = (*f).FirstEdge();
    EdgeOnFacetIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      // REQUIRE_ALWAYS( &(v.TheGrid()) == &(f.TheGrid()), "",1);
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*f).NumOfEdges() > 0) {
      v = (*f).FirstEdge();
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
