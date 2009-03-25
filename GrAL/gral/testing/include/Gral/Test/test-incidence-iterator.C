#ifndef NMWR_GB_GRAL_TEST_INCIDENCE_ITERATOR_C
#define NMWR_GB_GRAL_TEST_INCIDENCE_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-incidence-iterator.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

  template<class E, class A, class G, class GT>
  bool test_incidence_iterator(G const& g, std::ostream & out, GT);

  template<class E, class A, class G>
  bool test_incidence_iterator(G const& g, std::ostream & out)
  { return test_incidence_iterator<E,A>(g, out, grid_types<G>()); }


  template<class E, class A, class G, class GT>
  bool test_incidence_iterator(G const& g, std::ostream & out, GT)
  {
    typedef GT  gt;
    typedef typename element_traits<E>::element_type_tag etag;
    typedef typename element_traits<A>::element_type_tag atag;
    typedef typename element<gt,etag>::type element_type;
    typedef typename element<gt,atag>::type anchor_type;
    typedef typename incidence_iterator<gt,etag,atag>::type incidence_iterator_type;
    typedef typename sequence_iterator <gt,     atag>::type anchor_iterator;

    {
      typedef typename incidence_iterator_type::value_type  value_type;
      typedef typename incidence_iterator_type::anchor_type anchor_type;
      anchor_iterator         a(g);
      if(!a.IsDone()) {
	incidence_iterator_type e(*a);
	ref_ptr<anchor_type const> anch(e.TheAnchor());
      }
    }

    for(anchor_iterator a(g); ! a.IsDone(); ++a) {
      int e_cnt = 0;
      for(incidence_iterator_type ea(*a); ! ea.IsDone(); ++ea, ++e_cnt) {
	;
      }
      REQUIRE_ALWAYS(e_cnt == size<element_type>(*a),
		     "e_cnt = " << e_cnt << " != size<element_type>(*a) = "
		     << size<element_type>(*a) << '\n',1);

      incidence_iterator_type v, v_end;
      v = begin<element_type>(*a);
      incidence_iterator_type w = v;
      for( ; !v.IsDone(); ++v, ++w) {
	ref_ptr<G const> Gv(v.TheGrid());
	ref_ptr<G const> Gc(a.TheGrid());
	REQUIRE_ALWAYS( &(*Gv) == &(*Gc), "",1);
	REQUIRE_ALWAYS( ( v ==  w), "Iterators differ!\n",1);
	REQUIRE_ALWAYS( (*v == *w), "Iterator values differ!\n",1);
      }
      REQUIRE_ALWAYS( (w.IsDone()), "", 1);
      REQUIRE_ALWAYS( (v == w), "Past-the-end iterators differ!\n", 1);
  
      if(size<element_type>(*a) > 0) {
	v = begin<element_type>(*a);
	w = v;
	++v;
	for( ; !v.IsDone(); ++v, ++w) 
	  REQUIRE_ALWAYS( (*v != *w), "Iterators point to same element!\n",1);
      }

      v     = begin<element_type>(*a);
      v_end = end  <element_type>(*a);
      int e_cnt2 = 0;
      for( ; v != v_end; ++v)
	++e_cnt2;
      REQUIRE_ALWAYS( e_cnt == e_cnt2, "e_cnt=" << e_cnt << " != " << e_cnt2 << "=e_cnt2",1);
    }


    return true;
  }

} // namespace GrAL 

#endif
