#ifndef GRAL_GB_BASE_VERTEX_SET_H
#define GRAL_GB_BASE_VERTEX_SET_H

// $LICENSE-NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/predicates.h" // incidence

#include "Utility/pre-post-conditions.h"


#include <vector>
#include <algorithm>



/*! \brief sorted vertex set of a grid element
    \ingroup elements  
    vertex_set can be used as keys in hash tables of elements.

    \todo specialize to vertex set of edges
 */
template<class E, class EDIM = typename element_traits<E>::element_dimension_tag>
class vertex_set {
private:
  typedef vertex_set<E,EDIM> self;
  typedef element_traits<E>  et;
  typedef grid_types<typename et::grid_type> gt;
  typedef typename gt::vertex_handle         vertex_handle;

  std::vector<vertex_handle> v;
public:
  vertex_set(E const& e) 
    { 
      v.reserve(e.NumOfVertices());
      for(typename VertexOn<E>::Iterator ve(e.FirstVertex()); ! ve.IsDone(); ++ve) {
	v.push_back(ve.handle());
      }
      std::sort(v.begin(), v.end());
    }

  vertex_handle operator[](int n) const { check_range(n); return v[n];}
  unsigned      size() const { return v.size();}

  // FIXME: lexicographical_compare_3way is not in std::
  // (only a SGI extension)
  bool operator==(self const& rhs) const { 
    return (0 == std::lexicographical_compare_3way
	    (v    .begin(),    v.end(),
	     rhs.v.begin(),rhs.v.end()));
  }
  bool operator< (self const& rhs) const { 
    return (0  >  std::lexicographical_compare_3way
	    (v    .begin(),    v.end(),
	     rhs.v.begin(),rhs.v.end()));
  }
private:
  void check_range(unsigned i) const {
    REQUIRE( ((0 <= i ) && (i < v.size())), 
	     " i = " << i << " must be in [0," << v.size()-1 << "]\n",1);
          
  }
};


namespace std {

  template<class T>
    class hash;
  
  template<class E>
    class hash<vertex_set<E> >
    {
    public:
      size_t operator()(vertex_set<E> const& v) const 
	{
	  size_t n = v.size();
	  // should use ct-branch here.
	  if(n == 2) 
	    return 6*v[1] + v[0];
	  else // not unique for cells in 3D
	    return 24*v[n-1] + 8*v[n-2] + v[n-3];
	}
    };
  // specialization for E = edge case (EDIM = 1): only 2 vertices
}



#endif
