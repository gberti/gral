#ifndef GRAL_BASE_GB_FACET_ITERATOR_H
#define GRAL_BASE_GB_FACET_ITERATOR_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
//
/*! \brief A generic iterator over the facets of a grid.
  \ingroup iterators

  This works by iterating over the cells of the grid
  and over the facets of the cells, skipping facets
  that already have been visited from the other side, 
  i.e. the opposite cell. To this end we require the 
  cells to be ordered by a relation < and define that
  facets will be visited from the smaller cells.

  \partof Module iterators
*/
//----------------------------------------------------------------

template<class FacOnCellIt>
class FacetIterator  {
public:
  typedef typename FacOnCellIt::grid_type grid_type;
  typedef grid_types<grid_type>           gt;
  typedef FacOnCellIt                     local_it;
  typedef typename gt::CellIterator       global_it;

  typedef typename gt::Facet              Facet;
private:
  //----- DATA --------
  global_it         c;  // current cell
  local_it          fc; // current facet on c
  // invariant:  ! IsDone() => *c == fc.TheCell()

  typedef FacetIterator<FacOnCellIt> self;
public:
  typedef grid_type anchor_type;
  typedef Facet     value_type;

  //--------- constructors -------------------------

  FacetIterator() {}
  FacetIterator(grid_type const& g) { init(g.FirstCell()); }
  FacetIterator(const global_it& cc) { init(cc) ;}

private:
  void init(const global_it& cc)
    {
      c = cc;
      if(!c.IsDone()) 
	fc = (*c).FirstFacet();
      while(! IsDone() && ! is_new_facet())
	advance();
    }

public:  
  //---------- iterator operations ------------------

  inline self& operator++() {
    advance();
    while( ! IsDone() && ! is_new_facet())
      advance();
    return *this;
  }

  Facet operator*() const {return (*fc);}
  bool  IsDone() const {return (c.IsDone());}

  const grid_type& TheGrid() {return (c.TheGrid());}

  friend bool operator==(const self& ls, const self& rs) {
    // relies on shortcircuit to prevent (ls._nb == rs._nb) from
    // being executed if (ls._c != rs._c), thus violating the precondition
    // of the first comparision.
    // Also, this relies on Edge2D_Iterator visiting edges always from
    // one and the same side -- the inverse iterator won't be compare to equal.
    return (    (ls.IsDone() && rs.IsDone()) 
	     || ((ls.c == rs.c) && (ls.fc == rs.fc)));
  }
  friend bool operator<(const self& ls, const self& rs) {
    return ((ls.c < rs.c) || ((ls.c == rs.c) && (ls.fc < rs.fc )));
  }

private:
  //---------- helper functions for iteration -------

  inline void advance();

  // check, if the current edge (facet) is not
  // already visited from the other side.
  bool is_new_facet() { 
    return (   TheGrid().IsOnBoundary(fc)
	       || (fc.other_cell() < c.handle()));
  }

};


template<class FacOnCellIt>
inline void
FacetIterator<FacOnCellIt>::advance()
{ 
  ++fc;
  if(fc.IsDone()) {
    ++c;
    if(! c.IsDone()){
      fc = (*c).FirstFacet();
    }
  }
}




#endif
