#ifndef NMWR_GB_OVERLAP_PATTERN_H
#define NMWR_GB_OVERLAP_PATTERN_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <string>
#include <queue> // STL

#include "Gral/Base/type-tags.h"

namespace GrAL {

//----------------------------------------------------------------
//
/*! \brief class for storing an overlap (incidence) pattern
  
   The pattern may be, for example,  the local stencil of a 
   numeric function.
*/
//----------------------------------------------------------------

class overlap_pattern {
private:
 std::queue<grid_element_tag, std::deque<grid_element_tag> > q;
public:
  overlap_pattern();
  overlap_pattern(std::string const& pat);

  void push(grid_element_tag t)  { q.push(t);}
  void pop()                     { q.pop();}
  grid_element_tag front() const { return q.front();}

  bool empty() const { return q.empty();}
};

} // namespace GrAL 

#endif

