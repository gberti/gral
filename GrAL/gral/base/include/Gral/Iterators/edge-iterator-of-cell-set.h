#ifndef GRAL_BASE_GB_EDGE_ITERATOR_BY_CELL_IT_H
#define GRAL_BASE_GB_EDGE_ITERATOR_BY_CELL_IT_H


// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"

//----------------------------------------------------------------
// 
/*! \brief  Iterator over the edges incident to a set of cells.
    \ingroup iterators
    
    \templateparams
     - \c CellIt: Model of $GrAL CellIterator
     - \c CellIt::grid_type: Has \c Edge and \c Cell types.
     - \c CellIt::Cell: Model of $GrAL EdgeRange

    \test in  test-edge-iterator-of-cell-set.C  
 
    \see vertex_iterator_of_cell_set 
    \see facet_iterator_of_cell_set
*/
//----------------------------------------------------------------

template<class CELLIT, class GT = grid_types<typename CELLIT::grid_type> >
class edge_iterator_of_cell_set {
  typedef edge_iterator_of_cell_set<CELLIT, GT> self;
public:
  typedef typename CELLIT::grid_type        grid_type;
  typedef GT                                gt;
  typedef typename gt::Edge               Edge;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::EdgeOnCellIterator EdgeOnCellIterator;
  typedef typename gt::edge_handle        edge_handle;

  typedef CELLIT                            CellIterator;

private:
  //---------- DATA ---------------
  partial_grid_function<Edge,bool> visited;
  CellIterator         major;
  EdgeOnCellIterator minor;
  //--------- END DATA -------------


public:
  edge_iterator_of_cell_set() {}
  edge_iterator_of_cell_set(grid_type const& g) 
    : visited(g)
  {
    major = g.FirstCell();
    if(! major.IsDone())
      minor = EdgeOnCellIterator(*major);
  }
  edge_iterator_of_cell_set(const CellIterator& c)
    : visited(c.TheGrid())
    { 
      major = c;
      if (! major.IsDone()) 
	minor = EdgeOnCellIterator(*major);
    }

  self&   operator++() {
    cv();
    advance();
    while(! IsDone() && visited(value()))
      advance();
    return *this;
  }

  Edge operator*()     const { return value();}
  bool   IsDone()        const { cb(); return major.IsDone();}
  edge_handle handle() const { return minor.handle();}

  grid_type const& TheGrid() const { cb(); return major.TheGrid();}

  friend bool operator==(self const& lhs, self const& rhs) 
  { lhs.cb(); rhs.cb(); return (lhs.major == rhs.major) && (lhs.minor == rhs.minor);}
  friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs==rhs);}

  bool bound() const { return major.bound();}
  bool valid() const { return bound() && minor.valid();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "", 1);}
private:
  Edge  value() const { cv(); return (*minor);}
  void advance() { 
    visited[value()] = true;
    ++minor; 
    if(minor.IsDone()) {
      ++major;
      if(! major.IsDone())
	minor = EdgeOnCellIterator(*major);
    } 
  }
};

#endif
