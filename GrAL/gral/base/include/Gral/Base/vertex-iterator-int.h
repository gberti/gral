#ifndef GRAL_GB_VERTEX_ITERATOR_INT_H
#define GRAL_GB_VERTEX_ITERATOR_INT_H

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Gral/Base/common-grid-basics.h"

template<class GT>
class vertex_iterator_int;

template<class GT>
inline bool operator==(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs);

/*! Generic vertex / vertex iterator type,
    based on vertices being numbered consecutively.
   
    \ingroup iterators
 */

template<class GT>
class vertex_iterator_int : public GT {
  typedef vertex_iterator_int< GT>   self;
  typedef GT                         gt;
public:
  typedef typename gt::grid_type     grid_type;
  typedef typename gt::vertex_handle vertex_handle;
protected:
  ref_ptr<grid_type const> g;
  int                      v;
public:
  vertex_iterator_int() : g(0), v(-1) {}
  explicit 
  vertex_iterator_int(grid_type const&         gg, int vv = 0) : g(&gg), v(vv) {}
  explicit 
  vertex_iterator_int(ref_ptr<grid_type const> gg, int vv = 0) : g(gg), v(vv) {}

  self      & operator++() { cv(); ++v; return *this;}
  self const& operator*() const { cv(); return *this;}
  grid_type const& TheGrid() const { cb(); return *g;}
  bool IsDone() const { cb(); return (v >= (int)g->NumOfVertices());}

  friend bool operator== <> (vertex_iterator_int<GT> const& lhs, 
			     vertex_iterator_int<GT> const& rhs);

  vertex_handle handle() const { cv(); return vertex_handle(v);}

  bool bound() const { return g != 0;}
  bool valid() const { return bound() && v < (int) g->NumOfVertices();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "v=" << v, 1);}
};

  
template<class GT>
inline bool operator==(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return (rhs.v == lhs.v);}
template<class GT>
inline bool operator< (vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return (rhs.v < lhs.v);}

#ifndef GRAL_INCLUDES_RELOPS
template<class GT>
inline bool operator!=(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return !(lhs == rhs);}

template<class GT>
inline bool operator> (vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return  (rhs < lhs);}

template<class GT>
inline bool operator<=(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return !(lhs > rhs);}

template<class GT>
inline bool operator>=(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return !(lhs < rhs);}

#endif

#endif
