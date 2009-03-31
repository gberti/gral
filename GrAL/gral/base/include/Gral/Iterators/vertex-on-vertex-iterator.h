#ifndef GRAL_GB_ITERATORS_VERTEX_ON_VERTEX_ITERATOR_H
#define GRAL_GB_ITERATORS_VERTEX_ON_VERTEX_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-functions.h"
#include "Gral/Base/cell-archetype-map.h"

#include "Utility/ref-ptr.h"
#include "Utility/pre-post-conditions.h"
#include "Container/sequence-algorithms.h"

#include <vector>
#include <map>
#include <algorithm>

namespace GrAL {

  template<class G, class GT = grid_types<G> >
  class vertex_on_vertex_iterator;

  namespace detail {
    template<int HasVoV, class G, class GT>
    struct select_vertex_on_vertex_iterator {};

    template<class G, class GT> 
    struct select_vertex_on_vertex_iterator<0,G, GT>
    { 
      typedef typename GT::archetype_type A;
      typedef typename GT::archgt         AGT;
      typedef vertex_on_vertex_iterator<A,AGT> type;
    };

    template<class G, class GT> 
    struct select_vertex_on_vertex_iterator<1,G, GT>
    {
      typedef typename GT::archgt archgt;
      typedef typename archgt::VertexOnVertexIterator type;
    };
  }

/*! \brief A generic VertexOnVertexIterator
    \ingroup iterators
    \see \ref iterators module

   \see Test in \ref test-vertex-on-vertex-iterator.C
*/

template<class G, class GT>
class vertex_on_vertex_iterator {
  typedef vertex_on_vertex_iterator<G,GT> self;
public:
  typedef grid_incidence_iterator_category_d<0,0> category;
  typedef G  grid_type;
  typedef GT gt;
  typedef typename gt::Vertex      Vertex;
  typedef typename gt::Cell        Cell;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::archetype_type archetype_type;
  typedef typename gt::archgt         archgt;

  typedef typename detail::select_vertex_on_vertex_iterator<
    has_VertexOnVertexIterator<archgt>::result, G, GT>::type   ArchVertexOnVertexIterator;
private:
  typedef grid_function<Vertex, std::vector<vertex_handle> > vov_table;
  
  ref_ptr<vov_table const> inc;
  Vertex                   v;
  int                      lv;  

  static  std::map<grid_type const*, ref_ptr<vov_table> > ctxt;


public:
  vertex_on_vertex_iterator() {}
  vertex_on_vertex_iterator(Vertex const& vv) : v(vv), lv(0) 
  {
    // REQUIRE( (ctxt.find(&v.TheGrid()) != ctxt.end()), 
    //	    "No CellOnVertex incidence information registered for grid!\n",1);
    if(ctxt.find(&v.TheGrid()) == ctxt.end())
      init(v.TheGrid());
    inc = ctxt[&v.TheGrid()];
  } 

  self& operator++() { cv(); ++lv; return *this;}
  Vertex  operator*() const { cv(); return Vertex(TheGrid(), handle());}
  bool  IsDone()    const { cb(); return lv >=  (int)(*inc)(v).size();}
  vertex_handle handle() const { cv(); return (*inc)(v)[lv];}

  Vertex const& TheVertex() const { return v;}
  Vertex const& TheAnchor() const { return v;}
  grid_type const& TheGrid() const { return  v.TheGrid();}

  bool bound() const { return v.valid();}
  bool valid() const { return bound() && lv < (int)(*inc)(v).size();}
  void cb() const { REQUIRE(bound(), "", 1);}
  void cv() const { REQUIRE(valid(), "", 1);}


  static void init(grid_type const& g) { 
    ref_ptr<vov_table> t(new vov_table(g), ref_ptr_base::shared);
    for(typename gt::CellIterator c(g); ! c.IsDone(); ++c) {
      cell_archetype_map<grid_type> cXa(*c);
      for(typename gt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc) {
	for(ArchVertexOnVertexIterator vvc(cXa.V(vc)); !vvc.IsDone(); ++vvc) {
	  (*t)[*vc].push_back(cXa(vvc.handle()));
	}
      }
    }
    for(typename gt::VertexIterator v(g); !v.IsDone(); ++v) {
      sequence::sort_and_make_unique((*t)[*v]);
      /*
     std::sort  ((*t)[*v].begin(), (*t)[*v].end());
      typename std::vector<vertex_handle>::iterator last =
	std::unique((*t)[*v].begin(), (*t)[*v].end());
      (*t)[*v].erase(last, (*t)[*v].end());
      */
    }
    ctxt[&g] = t;
  }

  static void remove(grid_type const& g) {
    typename vov_table::iterator it = ctxt.find(&g);
    if(it != ctxt.end()) {
      it->second.clear();
      ctxt.erase(it);
    }

  }
  
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Iterators/vertex-on-vertex-iterator.C"
#endif


#endif
