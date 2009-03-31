#ifndef NMWR_GB_GRAL_TEST_FACET_ON_CELL_ITERATOR_C
#define NMWR_GB_GRAL_TEST_FACET_ON_CELL_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-facet-on-cell-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class G, class GT>
bool test_facet_on_cell_iterator(G const& g, ::std::ostream & out, GT);

template<class G>
bool test_facet_on_cell_iterator(G const& g, ::std::ostream & out)
{ return test_facet_on_cell_iterator(g,out, grid_types<G>()); }

template<class G, class GT>
bool test_facet_on_cell_iterator(G const& g, ::std::ostream & out, GT)
{
  typedef grid_types<G> gt;
  typedef typename gt::CellIterator        CellIterator;
  typedef typename gt::FacetOnCellIterator FacetOnCellIterator;

  for(CellIterator c(g); ! c.IsDone(); ++c) {
    int v_cnt = 0;
    for(FacetOnCellIterator vc(*c); ! vc.IsDone(); ++vc, ++v_cnt) {
    ;
    }
    REQUIRE_ALWAYS(v_cnt == (int)(*c).NumOfFacets(), 
                   "v_cnt = " << v_cnt << " != c.NumOfFacets() = " 
                   << (*c).NumOfFacets() << '\n',1);

    FacetOnCellIterator v;
    v = (*c).FirstFacet();
    FacetOnCellIterator w = v;
    for( ; !v.IsDone(); ++v, ++w) {
      REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
      REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
    }
    REQUIRE_ALWAYS( (w.IsDone()), "", 1);
    REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
    if((*c).NumOfFacets() > 0) {
      v = (*c).FirstFacet();
      w = v;
      ++v;
      for( ; !v.IsDone(); ++v, ++w) 
        REQUIRE_ALWAYS( (*v != *w), "Iterators point to same facet!\n",1);
    }
  }
  return true;
}

} // namespace GrAL 

#endif
