#ifndef GRAL_GB_VERTEX_ITERATOR_INT_H
#define GRAL_GB_VERTEX_ITERATOR_INT_H

// $LICENSE

/*! Generic vertex / vertex iterator type,
    based on vertices being numbered consecutively.
   
    \ingroup iterators
 */

template<class GRID>
class int_vertex_iterator {
  typedef int_vertex_iterator<GRID> self;
public:
  typedef GRID grid_type;
protected:
  grid_type const* g;
  int v;
public:
  int_vertex_iterator() : g(0), v(-1) {}
  explicit 
  int_vertex_iterator(grid_type const& gg, int vv = 0) : g(&gg), v(vv) {}

  self      & operator++() { ++v; return *this;}
  self const& operator*() const { return *this;}
  grid_type const& TheGrid() const { return *g;}
  bool IsDone() const { return (v >= g->NumOfVertices());}

  int handle() const { return v;}
  bool operator==(self const& rhs) const { return (rhs.v == v) && (rhs.g == g);}
  bool operator!=(self const& rhs) const { return !((*this) == rhs);}
};


#endif
