#ifndef GRAL_BASE_GB_VERTEX_ITERATOR_BY_CELL_IT_H
#define GRAL_BASE_GB_VERTEX_ITERATOR_BY_CELL_IT_H


// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"


namespace GrAL {
//----------------------------------------------------------------
// 
/*! \brief  Iterator over the vertices incident to a set of cells.
    \ingroup iterators
    
    \templateparams
     - \c CellIt: Model of $GrAL CellIterator
     - \c CellIt::grid_type: Has \c Vertex and \c Cell types.
     - \c CellIt::Cell: Model of $GrAL VertexRange


    Example:
    \code
    partial_grid_function<Cell,bool> marked(grid, false);
    // ... mark some cells ...
    typedef partial_grid_function<Cell,bool>::CellIterator pgfCellIterator;
    typedef vertex_iterator_of_cell_set<pgfCellIterator>   pgfVertexIterator;
    
    // pgfVertexIterator(marked); // ERROR: does not compile
    for(pgfVertexIterator v(marked.FirstCell()); !v.IsDone(); ++v) {
      something(*v);
    }
    \endcode

     \c vertex_iterator_of_cell_set::TheGrid() does not necessarily return
     the grid range covered by the object of type \c CELLIT.
     In the example above, it refers to the underlying grid, not to the grid function \c marked.
     In this case, a call to the constructor 
     <tt> vertex_iterator_of_cell_set(grid_type const&) </tt>  will not compile.
     This can be changed by providing an adapted \c GT parameter.

    \see \ref iterators module 
    \see Test in \ref test-vertex-iterator-of-cell-set.C

    \note This is very similar to edge_iterator_of_cell_set and facet_iterator_of_cell_set.
    We could use a similar strategy as that one used for partial_grid_function.
*/
//----------------------------------------------------------------

template<class CELLIT, class GT = grid_types<typename CELLIT::grid_type> >
class vertex_iterator_of_cell_set {
  typedef vertex_iterator_of_cell_set<CELLIT, GT> self;
public:
  typedef typename CELLIT::grid_type        grid_type;
  typedef GT                                gt;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
  typedef typename gt::vertex_handle        vertex_handle;

  typedef CELLIT                            CellIterator;

private:
  //---------- DATA ---------------
  partial_grid_function<Vertex,bool> visited;
  CellIterator         major;
  VertexOnCellIterator minor;
  //--------- END DATA -------------


public:
  vertex_iterator_of_cell_set() {}
  vertex_iterator_of_cell_set(grid_type const& g) 
    : visited(g)
  {
    major = g.FirstCell();
    if(! major.IsDone())
      minor = VertexOnCellIterator(*major);
  }
  vertex_iterator_of_cell_set(const CellIterator& c)
    : visited(c.TheGrid())
    { 
      major = c;
      if (! major.IsDone()) 
	minor = VertexOnCellIterator(*major);
    }

  self&   operator++() {
    cv();
    advance();
    while(! IsDone() && visited(value()))
      advance();
    return *this;
  }

  Vertex operator*()     const { return value();}
  bool   IsDone()        const { cb(); return major.IsDone();}
  vertex_handle handle() const { return minor.handle();}

  grid_type const& TheGrid() const { cb(); return major.TheGrid();}

  friend bool operator==(self const& lhs, self const& rhs) 
  { lhs.cb(); rhs.cb(); return (lhs.major == rhs.major) && (lhs.minor == rhs.minor);}
  friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs==rhs);}
  friend bool operator< (self const& lhs, self const& rhs) { 
    lhs.cb(); rhs.cb(); 
    return (lhs.major < rhs.major) || ((lhs.major == rhs.major) && (lhs.minor < rhs.minor));
  }

  bool bound() const { return major.bound();}
  bool valid() const { return bound() && minor.valid();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "", 1);}
private:
  Vertex  value() const { cv(); return (*minor);}
  void advance() { 
    visited[value()] = true;
    ++minor; 
    if(minor.IsDone()) {
      ++major;
      if(! major.IsDone())
	minor = VertexOnCellIterator(*major);
    } 
  }
};


} // namespace GrAL

#endif
