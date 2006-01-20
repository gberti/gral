#ifndef GRAL_GB_BASE_ITERATORS_DUMMY_ITERATOR_H
#define GRAL_GB_BASE_ITERATORS_DUMMY_ITERATOR_H

// $LICENSE_NEC_2003

//#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-traits.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class GRID, class ETAG>
class dummy_element {
  typedef dummy_element<GRID, ETAG> self;
public:
  typedef GRID     grid_type;
  typedef ETAG     element_type_tag;
  typedef int      handle_type;
  
  dummy_element() { fail();}
  dummy_element(grid_type const&) { fail();}
  dummy_element(grid_type const&, handle_type) { fail();}

  handle_type handle() const { fail(); return handle_type();}
  grid_type   TheGrid()   const { fail(); return grid_type();}

  bool operator==(self const& rhs) const { fail(); return false;}
  bool operator!=(self const& rhs) const { fail(); return false;}
private:
  void  fail() const { REQUIRE_ALWAYS(false, "Attempt to use dummy iterator!!",1);}
};


  template<class GRID, class ETAG>
  struct element_traits<dummy_element<GRID, ETAG> > 
    : public element_traits_base<dummy_element<GRID, ETAG> >
  {};

/*! \brief Dummy sequence iterator
  \ingroup iterators
  
   This can be used with generic components formally requiring certain iterators 
   which will not be used in the concrete case.

   \see \ref iterators module
*/
template<class ELEM, class ANCHOR>
class dummy_sequence_iterator {
  typedef dummy_sequence_iterator<ELEM, ANCHOR> self;
public:
  typedef ELEM     element_type;
  typedef ELEM     value_type;
  typedef ANCHOR   anchor_type;
  typedef typename anchor_type::grid_type grid_type;

  typedef element_traits<element_type> et;
  typedef typename et::handle_type     handle_type;
  //  typedef typename element_type::handle_type handle_type;

  dummy_sequence_iterator() { fail();}
  dummy_sequence_iterator(anchor_type const&) { fail();}

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



/*! \brief Dummy incidence iterator
  \ingroup iterators
  
   This can be used with generic components formally requiring certain iterators 
   which will not be used in the concrete case.

   \see \ref iterators module
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

} // namespace GrAL 

#endif
