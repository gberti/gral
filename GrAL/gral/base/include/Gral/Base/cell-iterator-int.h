#ifndef GRAL_GB_CELL_ITERATOR_INT_H
#define GRAL_GB_CELL_ITERATOR_INT_H

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Gral/Base/common-grid-basics.h"


template<class GRID, class GT = grid_types<GRID> >
class cell_iterator_int;

template<class GRID, class GT>
inline bool operator==(cell_iterator_int<GRID,GT> const& lhs, 
		       cell_iterator_int<GRID,GT> const& rhs);

template<class GRID, class GT, unsigned HASVTXONCELL>
class cell_iterator_int_voc_aux {};

template<class GRID, class GT>
class cell_iterator_int_voc_aux<GRID,GT,1>
{
  typedef cell_iterator_int<GRID,GT> cell_iter_type;
  cell_iter_type const& to_derived() const { return static_cast<cell_iter_type const&>(*this);}
public:
  typedef typename GT::VertexOnCellIterator VertexOnCellIterator;
  VertexOnCellIterator FirstVertex() const { return VertexOnCellIterator(to_derived());}
  unsigned NumOfVertices() const { return to_derived().TheGrid().NumOfVertices(to_derived());}
};


/*! Generic cell / cell iterator type,
    based on vertices being numbered consecutively.

    This implementation assumes that Cell is constructible from a CellIterator.
    \ingroup iterators
 */

template<class GRID, class GT>
class cell_iterator_int : 
  public GT, 
  public cell_iterator_int_voc_aux<GRID,GT, has_VertexOnCellIterator<GT>::result>  
{
  typedef cell_iterator_int<GRID, GT>  self;
  typedef GT                           gt;
public:
  typedef GRID                      grid_type;
  typedef typename gt::cell_handle  cell_handle;
  typedef typename gt::Cell         Cell;
protected:
  grid_type const* g;
  int c;
public:
  cell_iterator_int() : g(0), c(-1) {}
  explicit
  cell_iterator_int(grid_type const&         gg, int cc = 0) : g(&gg), c(cc) {}
  explicit
  cell_iterator_int(ref_ptr<grid_type const> gg, int cc = 0) : g( gg), c(cc) {}

  self      & operator++()      { cv();  ++c; return *this;}
  self        operator*() const { cv(); return Cell(*this);}
  grid_type const& TheGrid() const { cb(); return *g;}


  bool IsDone()  const { cb(); return (c >= (int)g->NumOfCells());}
  int  handle()  const { cv(); return c;}

  friend bool operator==<>(cell_iterator_int<GRID,GT> const& lhs, 
			   cell_iterator_int<GRID,GT> const& rhs);
  /*
  bool operator==(self const& rhs) const { return (rhs.c == c) && (rhs.g == g);}
  bool operator!=(self const& rhs) const { return !((*this) == rhs);}
  bool operator< (self const& rhs) const { return (c < rhs.c);}
  */

  bool bound() const { return g != 0;}
  bool valid() const { return bound() && c < (int)g->NumOfCells();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "c=" << c, 1);}
};

template<class GRID, class GT>
inline bool operator==(cell_iterator_int<GRID,GT> const& lhs, 
		       cell_iterator_int<GRID,GT> const& rhs) { return (rhs.c == lhs.c);}
template<class GRID, class GT>
inline bool operator!=(cell_iterator_int<GRID,GT> const& lhs, 
		       cell_iterator_int<GRID,GT> const& rhs) { return !(lhs == rhs);}


#endif
