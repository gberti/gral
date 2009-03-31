#ifndef GRAL_GB_BASE_ITERATORS_SINGLE_ELEMENT_ITERATOR_H
#define GRAL_GB_BASE_ITERATORS_SINGLE_ELEMENT_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Utility/ref-ptr.h"

namespace GrAL {

  /*! \brief An iterator iterating over a single element


     \ingroup iterators

   Model of $GrAL GridSequenceIterator.
   This class is handy when an iterator / range is formally required. 

   \templateparams
      - E: a model of $GrAL GridElement

   Tested in \ref test-single-element-iterator.C
  */
  template<class E>
  class single_element_iterator {
    typedef single_element_iterator<E> self;
  public:
    typedef E element_type;
    typedef element_traits<E>           et;
    typedef typename et::grid_type      grid_type;
    typedef typename et::handle_type    handle_type;
    typedef element_type                value_type;
    
    typedef typename get_sequence_iterator_category<typename category<E>::type>::type category;
  private:
    element_type e;
    int          cnt;
  public:
    single_element_iterator() : cnt(1) {}
    single_element_iterator(element_type const& ee) : e(ee), cnt(0) {}
    
    self& operator++() { ++cnt; return *this;}
    bool IsDone() const { return cnt > 0;}
    element_type const& operator*() const { return e;}
    handle_type  handle() const { return e.handle();}

    ref_ptr<grid_type const> TheGrid()   const { return ref_ptr<grid_type const>(e.TheGrid());}
    ref_ptr<grid_type const> TheAnchor() const { return ref_ptr<grid_type const>(e.TheGrid());}

    bool operator==(self const& rhs) const { return cnt == rhs.cnt;}
    bool operator!=(self const& rhs) const { return cnt != rhs.cnt;}
    bool operator< (self const& rhs) const { return cnt <  rhs.cnt;}
  };
  
} // namespace GrAL

#endif
