#ifndef GRAL_GB_BASE_ITERATORS_DUMMY_ITERATOR_H
#define GRAL_GB_BASE_ITERATORS_DUMMY_ITERATOR_H

// $LICENSE_NEC_2003

//#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-traits.h"
#include "Utility/pre-post-conditions.h"

/*! \brief Dummy incidence iterators

   This can be used with generic components formally requiring certain iterators 
   which will not be used in the concrete case.
*/
template<class ELEM, class ANCHOR>
class dummy_incidence_iterator {
  typedef dummy_incidence_iterator<ELEM, ANCHOR> self;
public:
  typedef ELEM     element_type;
  typedef ELEM     value_type;
  typedef ANCHOR   anchor_type;
  typedef typename anchor_type::grid_type grid_type;

  typedef element_traits<element_type> et;
  typedef typename et::handle_type     handle_type;
  //  typedef typename element_type::handle_type handle_type;

  dummy_incidence_iterator() { fail();}
  dummy_incidence_iterator(anchor_type const&) { fail();}

  self& operator++() { fail(); return *this;}
  bool  IsDone() const { fail(); return true;}
  value_type  operator*() const { fail(); return value_type();}
  handle_type handle() const { fail(); return handle_type();}

  grid_type   TheGrid()   const { fail(); return grid_type();}
  anchor_type TheAnchor() const { fail(); return anchor_type();}

  bool operator==(self const& rhs) const { fail(); return false;}
  bool operator!=(self const& rhs) const { fail(); return false;}
private:
  void  fail() const { REQUIRE_ALWAYS(false, "Attempt to use dummy iterator!!",1);}
};


#endif
