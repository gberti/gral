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

     \see Tested in \ref test-incidence-hull.C
*/
class incidence_stencil {
  typedef incidence_stencil self;

private:
   std::queue<grid_element_tag> q;
public:
  incidence_stencil() {}
  incidence_stencil(std::string const& s)
  { init(s,1);}

  /*! \brief Initialize by repeating the stencil.

      Repeating a stencil means repeating forming the incidence hull,
      starting from the last layer as a seed set. This means that
      the first and last element code of the stencil must coincide (repeatable stencil).
      If this is not the case, the first element code is implicitely
      appended to the stencil. Consider the following examples:
      - ("CVC",2) -> "CVCVC"
      - ("CC", 3) -> "CCCC"
      - ("VC", 3) -> "VCVCVC"
      - ("CVCC", 2) -> "CVCCVCC"

      This leads to
      <tt>stencil(s,n).size() == 1 + n*(stencil(s).size() -1)</tt>,
      if the stencil is repeatable. 
  */
  incidence_stencil(std::string const& s, int n)
  { init(s,n);}

  void init(std::string const& s, int n) 
  {
    for(int k = 0; k < n; ++k) {
      int start = (s[0] == s[s.size() -1] && k > 0 ? 1 : 0);
      
      for(unsigned i = start; i < s.size(); ++i)
	switch(s[i]) {
	case 'V' : q.push(vertex_tag); break;
	case 'E' : q.push(edge_tag);   break;
	case 'F' : q.push(facet_tag);  break;
	case 'C' : q.push(cell_tag);   break;
	default: 
	  std::cerr << "Unknown element code " << s[i] << " found!\n";
	  break;
	}
    }
  }

  void             pop()         { q.pop();}
  grid_element_tag front() const { return q.front();}
  bool     empty() const { return q.empty();}
  unsigned size()  const { return q.size();}
  bool operator==(self const& rhs) const { return rhs.q == q;}
  bool operator!=(self const& rhs) const { return rhs.q != q;}
};

} // namespace GrAL

#endif
