#ifndef NMWR_GB_OVERLAP_PATTERN_H
#define NMWR_GB_OVERLAP_PATTERN_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "forward/string.h"
#include <queue> // STL
#include "Grids/type-tags.h"

//----------------------------------------------------------------
//
// class for storing an overlap (adjacency) pattern, for example
// for characterizing the local stencil of a numeric function.
//
//----------------------------------------------------------------

class overlap_pattern {
private:
 queue<grid_element_tag,deque<grid_element_tag> > q;
public:
  overlap_pattern();
  overlap_pattern(string const& pat);

  void push(grid_element_tag t)  { q.push(t);}
  void pop()                     { q.pop();}
  grid_element_tag front() const { return q.front();}

  bool empty() const { return q.empty();}
};

#endif

