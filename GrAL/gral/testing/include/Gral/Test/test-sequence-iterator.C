#ifndef NMWR_GB_GRAL_TEST_SEQUENCE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_SEQUENCE_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-sequence-iterator.h"
#include "Gral/Base/common-grid-basics.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

  template<class E,class G, class GT>
  bool test_sequence_iterator(G const& g, std::ostream & out, GT);

  template<class E,class G>
  bool test_sequence_iterator(G const& g, std::ostream & out)
  //  { return test_sequence_iterator(g, out, grid_types<G>()); }
{
  //typedef GT gt;
  typedef grid_types<G> gt;

  typedef element_traits<E> et;
  typedef typename et::element_type_tag etag;
  typedef typename element<gt,etag>::type        element_type;
  typedef typename element_handle<gt,etag>::type element_handle_type;
  typedef typename sequence_iterator<gt,etag>::type element_iterator;

  {
    typedef typename element_iterator::value_type  value_type;
    typedef typename element_iterator::anchor_type anchor_type;
    element_iterator e = GrAL::begin_x(g);
    ref_ptr<anchor_type const> a(e.TheAnchor());
  }

  int cnt = 0;
  for(element_iterator e = GrAL::begin_x(g); ! e.IsDone(); ++e, ++cnt) {
    element_handle_type h = e.handle();
    element_type        ee (e.TheGrid(), h);

    REQUIRE_ALWAYS(ee == *e, "Element constructed from handle differs!\n",1);

  }
 
  REQUIRE_ALWAYS(cnt ==  size<element_type>(g),
                 "cnt = " << cnt << " != size<element_type>(g) = " << size<element_type>(g) << '\n',1);

 
  element_iterator v, v_end;
  v     = GrAL::begin<element_type>(g);
  v_end = GrAL::end  <element_type>(g);
  int cnt2 = 0;
  for( ; v != v_end; ++v, ++cnt2)
    ;
  REQUIRE_ALWAYS(cnt == cnt2, "cnt=" << cnt << " != " << cnt2 << "=cnt2",1);

  v = begin<element_type>(g);
  element_iterator w = v;
  for( ; !v.IsDone(); ++v, ++w) {
    REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
    REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
  }
  REQUIRE_ALWAYS( (w.IsDone()), "", 1);
  REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);

  v = begin<element_type>(g);
  w = v;
  if (! v.IsDone()) {
    ++v;
    for( ; !v.IsDone(); ++v, ++w) 
      REQUIRE_ALWAYS( (*v != *w), "Iterators point to same element!\n",1);
  }

  return true;
}

} // namespace GrAL 

#endif
