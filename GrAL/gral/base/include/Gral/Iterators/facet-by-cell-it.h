#ifndef GRAL_BASE_GB_FACET_BY_CELL_IT_H
#define GRAL_BASE_GB_FACET_BY_CELL_IT_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
// 
/*! \brief  Iterator over the facets of a set of cells.
    \ingroup iterators
    
    \templateparams
     - \c CellIt: Model of $GrAL CellIterator
     - \c CellIt::grid_type: Has \c Facet and \c Cell types.
     - \c CellIt::Cell: Model of $GrAL FacetRange

    <b> Implementation note: </b> <br>
    This uses a partial grid function to mark visited facets,
    hence this approach works also with other element types.

    
*/
//----------------------------------------------------------------

template<class CellIt>
class facet_set_of_cells_iterator {
public:
  typedef typename CellIt::grid_type grid_type;
  typedef grid_types<grid_type>      gt;
  typedef typename gt::Facet          Facet;
  typedef typename gt::Cell          Cell;
  typedef typename gt::FacetOnCellIterator  FacetOnCellIterator;

  typedef CellIt                     CellIterator;

private:
  //---------- DATA ---------------
  CellIterator       major;
  FacetOnCellIterator minor;
  partial_grid_function<Facet,bool> visited;
  //--------- END DATA -------------


  typedef facet_set_of_cells_iterator<CellIt> self;
public:
  facet_set_of_cells_iterator(const CellIterator& c)
    : visited(c.TheGrid())
    { 
      major = c;
      if (! major.IsDone()) 
	minor = (*major).FirstFacet(); 
    }

  self&   operator++() {
    advance();
    while(! IsDone() && visited(value()))
      advance();
    return *this;
  }
  Facet operator*() const { return value();}
  bool  IsDone()    const { return major.IsDone();}

  grid_type const& TheGrid() const { return major.TheGrid();}

private:
  Facet  value() const { return (*minor);}
    void advance() { 
    visited[value()] = true;
    ++minor; 
    if(minor.IsDone()) {
      ++major;
      if(! major.IsDone())
	minor = (*major).FirstFacet();
    } 
  }
};

#endif
