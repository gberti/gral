#ifndef GRAL_GB_BASE_INCIDENCE_STENCIL_H
#define GRAL_GB_BASE_INCIDENCE_STENCIL_H

// $LICENSE_NEC_2003


#include <queue>
#include <string>
#include <iostream>

#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

/*! \brief Representation of incidence stencils

     \ingroup incidence_hull
*/
class incidence_stencil {
   ::std::queue<grid_element_tag> q;
public:
  incidence_stencil() {}
  incidence_stencil( ::std::string const& s)
  { init(s);}

  void init( ::std::string const& s) 
  {
    for(unsigned i = 0; i < s.size(); ++i)
      switch(s[i]) {
      case 'V' : q.push(vertex_tag); break;
      case 'E' : q.push(edge_tag);   break;
      case 'F' : q.push(facet_tag);  break;
      case 'C' : q.push(cell_tag);   break;
      default: 
	 ::std::cerr << "Unknown element code " << s[i] << " found!\n";
	break;
      }
  }

  void             pop()         { q.pop();}
  grid_element_tag front() const { return q.front();}
  bool     empty() const { return q.empty();}
  unsigned size()  const { return q.size();}
};

} // namespace GrAL

#endif
