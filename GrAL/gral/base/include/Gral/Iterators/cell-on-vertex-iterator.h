#ifndef GRAL_GB_ITERATORS_CELL_ON_VERTEX_ITERATOR_H
#define GRAL_GB_ITERATORS_CELL_ON_VERTEX_ITERATOR_H

// $LICENSE_NEC


#include "Gral/Base/grid-functions.h"

#include "Utility/ref-ptr.h"
#include "Utility/pre-post-conditions.h"

#include <vector>
#include <map>

namespace GrAL {

/*! \brief A generic CellOnVertexIterator
    \ingroup iterators
    \see \ref iterators module

   \see Test in \ref test-cell-on-vertex-iterator.C
*/

template<class G, class GT = grid_types<G> >
class cell_on_vertex_iterator {
  typedef cell_on_vertex_iterator<G,GT> self;
public:
  typedef G  grid_type;
  typedef GT gt;
  typedef typename gt::Vertex      Vertex;
  typedef typename gt::Cell        Cell;
  typedef typename gt::cell_handle cell_handle;
private:
  typedef grid_function<Vertex,  ::std::vector<cell_handle> > cov_table;
  
  ref_ptr<cov_table const> inc;
  Vertex                   v;
  int                      lc;  

  static  ::std::map<grid_type const*, cov_table*> ctxt;


public:
  cell_on_vertex_iterator() {}
  cell_on_vertex_iterator(Vertex const& vv) : v(vv), lc(0) 
  {
    REQUIRE( (ctxt.find(&v.TheGrid()) != ctxt.end()), 
	    "No CellOnVertex incidence information registered for grid!\n",1);
    inc = ctxt[&v.TheGrid()];
  } 

  self& operator++() { cv(); ++lc; return *this;}
  Cell  operator*() const { cv(); return Cell(TheGrid(), handle());}
  bool  IsDone()    const { cb(); return lc >=  (int)(*inc)(v).size();}
  cell_handle handle() const { cv(); return (*inc)(v)[lc];}

  Vertex const& TheVertex() const { return v;}
  Vertex const& TheAnchor() const { return v;}
  grid_type const& TheGrid() const { return  v.TheGrid();}

  bool bound() const { return v.valid();}
  bool valid() const { return bound() && lc < (int)(*inc)(v).size();}
  void cb() const { REQUIRE(bound(), "", 1);}
  void cv() const { REQUIRE(valid(), "", 1);}


  static void init(grid_type const& g) { 
    cov_table* t = new cov_table(g);
    for(typename gt::CellIterator c(g); ! c.IsDone(); ++c)
      for(typename gt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc)
	(*t)[*vc].push_back(c.handle());
    ctxt[&g] = t;
  }

  static void remove(grid_type const& g) {
    typename cov_table::iterator it = ctxt.find(&g);
    if(it != ctxt.end()) {
      delete it->second;
      ctxt.erase(it);
    }

  }
  
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Iterators/cell-on-vertex-iterator.C"
#endif


#endif
