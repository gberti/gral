#ifndef GRAL_GB_CELL_ITERATOR_INT_H
#define GRAL_GB_CELL_ITERATOR_INT_H

// $LICENSE

/*! Generic cell / cell iterator type,
    based on vertices being numbered consecutively.

    \ingroup iterators
 */

template<class GRID>
class int_cell_iterator {
  typedef int_cell_iterator<GRID> self;
public:
  typedef GRID grid_type;
protected:
  grid_type const* g;
  int c;
public:
  int_cell_iterator() : g(0), c(-1) {}
  explicit
  int_cell_iterator(grid_type const& gg, int cc = 0) : g(&gg), c(cc) {}

  self      & operator++() { ++c; return *this;}
  self const& operator*() const { return *this;}
  grid_type const& TheGrid() const { return *g;}
  bool IsDone() const { return (c >= g->NumOfCells());}
  int  handle()  const { return c;}

  bool operator==(self const& rhs) const { return (rhs.c == c) && (rhs.g == g);}
  bool operator!=(self const& rhs) const { return !((*this) == rhs);}
};

#endif
