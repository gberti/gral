#ifndef GRAL_GB_BASE_VERTEX_SET_H
#define GRAL_GB_BASE_VERTEX_SET_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/predicates.h" // incidence

#include "Utility/pre-post-conditions.h"


#include <vector>
#include <algorithm>

#include "Container/my-hash-map.h"

namespace GrAL {

/*! \brief sorted vertex set of a grid element
    \ingroup elements  
    \see \ref elements module 
    vertex_set can be used as key  in hash tables of elements.

    \todo specialize to vertex set of edges
    \todo Rename to element_vertex_set ?
 */
template<class E, class EDIM = typename element_traits<E>::element_dimension_tag>
class vertex_set {
private:
  typedef vertex_set<E,EDIM> self;
  typedef element_traits<E>  et;
  typedef grid_types<typename et::grid_type> gt;
  typedef typename gt::vertex_handle         vertex_handle;
  typedef typename gt::Vertex                Vertex;
  typedef std::vector<vertex_handle>         table_type;
  
  table_type v;
public:
  vertex_set(E const& e) 
    { 
      typedef typename GrAL::iterator<Vertex,E>::type VertexOnEIterator;
      v.reserve(GrAL::size<Vertex>(e)); 
      for(VertexOnEIterator ve = GrAL::begin_x(e); ! ve.IsDone(); ++ve) {
	v.push_back(ve.handle());
      }
      std::sort(v.begin(), v.end());
    }
  vertex_set(unsigned n) : v(n) {}

  vertex_handle  operator[](int n) const { check_range(n); return v[n];}
  vertex_handle& operator[](int n)       { check_range(n); return v[n];}

  typedef typename table_type::size_type      size_type;
  typedef typename table_type::const_iterator const_iterator;
  size_type      size()  const { return v.size();}
  bool           empty() const { return v.empty();}
  const_iterator begin() const { return v.begin();}
  const_iterator end()   const { return v.end();}


  bool operator==(self const& rhs) const { 
    return ( ::std::equal(v    .begin(),    v.end(),
		       rhs.v.begin()));
  }
  bool operator!=(self const& rhs) const { return !((*this) == rhs);}
  bool operator< (self const& rhs) const { 
    return ( ::std::lexicographical_compare
	    (v    .begin(),    v.end(),
	     rhs.v.begin(),rhs.v.end()));
  }
private:
  void check_range(int i) const {
    REQUIRE( ((0 <= i ) && (i < (int) v.size())), 
	     " i = " << i << " must be in [0," << v.size()-1 << "]\n",1);
          
  }
};




/*! \brief sorted general vertex subset of a grid
    \ingroup elements  
    \see \ref elements module 
    general_vertex_set can be used as key in hash tables of elements,
    or for otherwise arbitrary sets of vertices

 */
template<class GRID>
class general_vertex_set {
private:
  typedef general_vertex_set<GRID> self;
  typedef GRID                  grid_type;
  typedef grid_types<grid_type> gt;
  typedef typename gt::vertex_handle     vertex_handle;
  typedef typename gt::Vertex            Vertex;

public:
  typedef std::vector<vertex_handle>          table_type;
  typedef typename table_type::size_type      size_type;
  typedef typename table_type::iterator       iterator;
  typedef typename table_type::const_iterator const_iterator;

private:
   table_type v;
public:
  general_vertex_set() {}

  explicit general_vertex_set(size_type n) : v(n) {}

  template<class IT>
  general_vertex_set(IT b, IT e) : v(b,e) 
  { init(); }

  template<class E>
  explicit general_vertex_set(E const& e) 
    { 
      v.reserve(GrAL::size<Vertex>(e)); 
      for(typename VertexOn<E>::type ve = GrAL::begin(e); ! ve.IsDone(); ++ve) {
	v.push_back(ve.handle());
      }
      init();
    }
  void init() { std::sort(v.begin(), v.end()); }

  void push_back(vertex_handle h) { v.push_back(h);}
  void push_back(Vertex       vv) { v.push_back(vv.handle());}


  vertex_handle  operator()(int n) const { check_range(n); return v[n];}
  vertex_handle& operator[](int n)       { check_range(n); return v[n];}

  size_type      size()  const { return v.size();}
  bool           empty() const { return v.empty();}
  const_iterator begin() const { return v.begin();}
  const_iterator end  () const { return v.end  ();}
  iterator       begin()       { return v.begin();}
  iterator       end  ()       { return v.end  ();}

  bool operator==(self const& rhs) const { 
    // std::equal() returns true also if one sequence is a strict
    // subsequence of another.
    return (size() == rhs.size() && std::equal(v    .begin(),    v.end(),
					       rhs.v.begin()));
  }
  bool operator!=(self const& rhs) const { return !((*this) == rhs);}
  bool operator< (self const& rhs) const { 
    return ( ::std::lexicographical_compare
	    (v    .begin(),    v.end(),
	     rhs.v.begin(),rhs.v.end()));
  }
private:
  void check_range(int i) const {
    REQUIRE( ((0 <= i ) && (i < (int) v.size())), 
	     " i = " << i << " must be in [0," << v.size()-1 << "]\n",1);
          
  }
};



} // namespace GrAL

namespace STDEXT {

  template<class T>
    class hash;
  
  template<class E>
    class hash<GrAL::vertex_set<E> >
    {
    public:
      typedef GrAL::vertex_set<E> key_type;
      typedef GrAL::vertex_set<E> argument_type;
      typedef size_t        result_type;

      size_t operator()(GrAL::vertex_set<E> const& v) const 
	{
	  size_t n = v.size();
	  // should use ct-branch here.
	  if(n == 2) 
	    return 6*v[1] + v[0];
	  else // not unique for cells in 3D
	    return 24*v[n-1] + 8*v[n-2] + v[n-3];
	}
    };
  //TODO: specialization for E = edge case (EDIM = 1): only 2 vertices


  template<class GRID>
  class hash<GrAL::general_vertex_set<GRID> >
    {
    public:
      typedef GrAL::general_vertex_set<GRID> key_type;
      typedef GrAL::general_vertex_set<GRID> argument_type;
      typedef size_t        result_type;

      size_t operator()(GrAL::general_vertex_set<GRID> const& v) const 
	{
	  // typedef element_traits<Vertex> et;
	  // typename et::hasher_type h;
	  int base = 17;
	  int factor = 1;
	  size_t sum = 0;
	  for(unsigned i = 0; i < v.size(); ++i) {
	    sum = factor * (v(i)+1);
	    factor *= base;
	  }
	  return sum;
	}
    };

}



#endif
