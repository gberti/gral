#ifndef GRAL_GB_BASE_ITERATORS_SEQUENCE_ELEMENT_ITERATOR_H
#define GRAL_GB_BASE_ITERATORS_SEQUENCE_ELEMENT_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

/*! \brief Sequence iterator adaptor for a sequence of element handles

   \ingroup iterators

   \templateparams
   \c X is an instance of \ sequence_element_iterator.
    - \c E  Element type = \c X::value_type, constructible from \c G and \c IT::value_type 
    - \c G  Grid type
    - \c IT forward iterator over element handles
    - \c GT \c grid_types specialization for \c G
*/

  template<class E, class G, class IT, class GT = grid_types<G> >
  class sequence_element_iterator {
    typedef sequence_element_iterator<E,G,IT,GT> self;
  public:
    typedef typename get_sequence_iterator_category<typename category<E>::type>::type category;
    typedef G grid_type;
    typedef G anchor_type;
    typedef E value_type;
    typedef E element_type;
    typedef element_traits<E> et;
    typedef typename et::handle_type element_handle;
  private:
    ref_ptr<grid_type const> g;
    IT  beg, end;
  public:
    sequence_element_iterator() {}
    sequence_element_iterator(        grid_type const& gg) 
      : g(gg), beg(gg. begin(tp<E>())), end(gg. end(tp<E>())) {}
    sequence_element_iterator(ref_ptr<grid_type const> gg) 
      : g(gg), beg(gg->begin(tp<E>())), end(gg->end(tp<E>())) {}

    sequence_element_iterator(        grid_type const& gg, IT b, IT e) : g(gg), beg(b), end(e) {}
    sequence_element_iterator(ref_ptr<grid_type const> gg, IT b, IT e) : g(gg), beg(b), end(e) {}

    self&      operator++() { cv(); ++beg; return *this;}
    value_type operator*() const { cv(); return  value_type(TheGrid(), *beg);}
    bool       IsDone()    const { cb(); return  beg == end;}
    element_handle handle() const { cv(); return *beg;}

    anchor_type const& TheAnchor() const { cb(); return *g;}
    grid_type   const& TheGrid()   const { cb(); return *g;}

    bool bound() const { return g != 0;}
    bool valid() const { return bound() && beg != end;}
    void cb()    const { REQUIRE(bound(), "",1);}
    void cv()    const { REQUIRE(valid(), "",1);}

    bool operator==(self const& rhs) const { cb(); return beg == rhs.beg;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };

} // namespace GrAL

#endif
