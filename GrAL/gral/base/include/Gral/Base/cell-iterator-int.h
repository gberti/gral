#ifndef GRAL_GB_CELL_ITERATOR_INT_H
#define GRAL_GB_CELL_ITERATOR_INT_H

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

template<class GT>
class cell_iterator_int;

template<class GT>
inline bool operator==(cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs);
template<class GT>
inline bool operator< (cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs);


// mixin for vertex-on-cell-iteration
template<class GT, unsigned HASVTXONCELL>
class cell_iterator_int_voc_aux {};

// mixin for edge-on-cell-iteration
template<class GT, unsigned HASVTXONCELL>
class cell_iterator_int_eoc_aux {};

// mixin for facet-on-cell-iteration
template<class GT, unsigned HASVTXONCELL>
class cell_iterator_int_foc_aux {};



template<class GT>
class cell_iterator_int_voc_aux<GT,1>
{
  typedef cell_iterator_int<GT> cell_iter_type;
  cell_iter_type const& to_derived() const { return static_cast<cell_iter_type const&>(*this);}
public:
  typedef typename GT::VertexOnCellIterator VertexOnCellIterator;
  VertexOnCellIterator FirstVertex() const { return VertexOnCellIterator(to_derived());}
  unsigned NumOfVertices() const { return to_derived().TheGrid().NumOfVertices(to_derived());}
};


template<class GT>
class cell_iterator_int_foc_aux<GT,1>
{
  typedef cell_iterator_int<GT> cell_iter_type;
  cell_iter_type const& to_derived() const { return static_cast<cell_iter_type const&>(*this);}
public:
  typedef typename GT::FacetOnCellIterator FacetOnCellIterator;
  FacetOnCellIterator FirstFacet() const { return FacetOnCellIterator(to_derived());}
  unsigned NumOfFacets() const { return to_derived().TheGrid().NumOfFacets(to_derived());}
};

template<class GT>
class cell_iterator_int_eoc_aux<GT,1>
{
  typedef cell_iterator_int<GT> cell_iter_type;
  cell_iter_type const& to_derived() const { return static_cast<cell_iter_type const&>(*this);}
public:
  typedef typename GT::EdgeOnCellIterator EdgeOnCellIterator;
  EdgeOnCellIterator FirstEdge() const { return EdgeOnCellIterator(to_derived());}
  unsigned NumOfEdges() const { return to_derived().TheGrid().NumOfEdges(to_derived());}
};


/*! \brief Generic cell / cell iterator type,
    based on cells being numbered consecutively starting from 0.

    \ingroup iterators

    This implementation assumes that \c GT::Cell is constructible from \c GT::CellIterator.
    (Often, \c GT::Cell and \c GT::CellIterator will be even the same type, namely \c cell_iterator_int<GT>.)

    It adds support for incidence iteration if the appropriate types are defined in \c GT.
    For instance, if \c GT defines a type \c VertexOnCellIterator,
    cell_iterator_int<GT> defines \c FirstVertex() and \c NumOfVertices() members.
    In this case, the type \c GT::grid_type  must also provide a member NumOfVertices(Cell),
    where \c GT::Cell is assumed to be constructible from \c cell_iterator_int<GT> 
    (which in turn should be equal to \c GT::CellIterator).

   \see polygon1d::polygon for a usage of this template.

 */

template<class GT>
class cell_iterator_int : 
  public GT, 
  public cell_iterator_int_voc_aux<GT, has_VertexOnCellIterator<GT>::result>,
  public cell_iterator_int_eoc_aux<GT, has_EdgeOnCellIterator  <GT>::result>,
  public cell_iterator_int_foc_aux<GT, has_FacetOnCellIterator <GT>::result> 

{
  typedef cell_iterator_int< GT>  self;
  typedef GT                      gt;
public:
  typedef typename gt::grid_type    grid_type;
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

  friend bool operator==<>(cell_iterator_int<GT> const& lhs, 
			   cell_iterator_int<GT> const& rhs);
  friend bool operator< <>(cell_iterator_int<GT> const& lhs, 
			   cell_iterator_int<GT> const& rhs);

  bool bound() const { return g != 0;}
  bool valid() const { return bound() && c < (int)g->NumOfCells();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "c=" << c, 1);}
};

template<class GT>
inline bool operator==(cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs) { return (rhs.c == lhs.c);}
template<class GT>
inline bool operator< (cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs) { return (rhs.c <  lhs.c);}

// #ifndef GRAL_INCLUDES_RELOPS
template<class GT>
inline bool operator!=(cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs) { return !(lhs == rhs);}

template<class GT>
inline bool operator> (cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs) { return  (rhs < lhs);}

template<class GT>
inline bool operator<=(cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs) { return !(lhs > rhs);}

template<class GT>
inline bool operator>=(cell_iterator_int<GT> const& lhs, 
		       cell_iterator_int<GT> const& rhs) { return !(lhs < rhs);}
//#endif

} // namespace GrAL 

#endif
