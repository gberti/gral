#ifndef GRAL_GB_COMPLEX2D_EDGE_ON_CELL2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_EDGE_ON_CELL2D_ITERATOR_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/pre-post-conditions.h"
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Iterators/iterator-base.h"


namespace GrAL { 

class EdgeOnCell2D_Iterator 
  : public complex2d_types, 
    public incidence_iter_base<complex2d_types, edge_type_tag, cell_type_tag>
{
  friend class CellOnCell2D_Iterator;
  friend class Complex2D;
  friend class Cell2D;
  friend class Edge2D;
  friend class Edge2D_Iterator;

  typedef EdgeOnCell2D_Iterator   self;
  typedef archgt::Cell            archetype_cell;
private:
  //----- DATA --------
  Cell            C;  // anchor
  int             lf; // local facet, \in [0, TheCell().NumOfFacets()]

public:
  typedef grid_incidence_iterator_category_d<1,2>  category;
  typedef Cell   anchor_type;
  typedef Edge   element_type;
  typedef Edge   value_type;

  //--------- constructors -----------------------------------

  EdgeOnCell2D_Iterator() 
    : C(),   lf(0)  {}
  explicit 
  EdgeOnCell2D_Iterator(const Cell& CC)
    : C(CC), lf(0)   {}
  EdgeOnCell2D_Iterator(int cnt, const Cell& CC)
    : C(CC), lf(cnt) 
  {
    REQUIRE( (0 <= cnt), 
	    "EdgeOnCell2DIterator: invalid cnt = " << cnt << " specified!\n",1);
  }
  EdgeOnCell2D_Iterator(const Cell& CC, int cnt)
    : C(CC), lf(cnt) 
  {
    REQUIRE( (0 <= cnt), 
	    "EdgeOnCell2DIterator: invalid cnt = " << cnt << " specified!\n",1);
  }
  /*
  EdgeOnCell2D_Iterator(cell_handle c, int cnt, const Complex2D& cc)
    : _cc(&cc), _c(c), lf(cnt) {
    REQUIRE( (0 <= cnt), // cnt == NumOfEdges() possible 
	    "EdgeOnCell2DIterator: invalid cnt = " << cnt << " specified!\n",1);
  }
  */
  EdgeOnCell2D_Iterator(CellOnCell2D_Iterator const& nb);

  ~EdgeOnCell2D_Iterator() {}

  //----------- iterator operations --------------------------

  inline Edge operator*() const;
  edge_handle handle() const { return edge_handle(C.handle(), lf);}

  self& operator++()      { ++lf;   return (*this);}
  self& operator--()      { --lf;   return (*this);}

  
  inline self  CyclicSucc() const;
  inline self  CyclicPred() const;

  // inline operator Cell2D_Iterator() const;
 
  bool IsDone() const {return ((lf < 0) || (lf >= TheCell().NumOfEdges()));}
  operator bool() const { return !IsDone();}
  int local_handle() const { return lf;}

  inline vertex_handle v1() const;
  inline vertex_handle v2() const;

  inline cell_handle   the_cell()   const;
  inline cell_handle   other_cell() const;
  inline Cell const&   TheCell()   const;
  inline Cell          OtherCell() const;

  //---------------- anchor access ------------------

  anchor_type const&  TheAnchor() const { return C;}
  Complex     const&  TheGrid()   const { return C.TheGrid();}

  archetype_cell ArchetypeCell() const { return archetype_cell(C.TheArchetype(),lf);}

  //----------------- comparison  -----------------------

  // note this is different from *lhs == *rhs !
  // *lhs != *rhs => lhs != rhs, but not the other way around.
  friend bool operator==(const self& lhs, const self& rhs) {
    return ((lhs.C == rhs.C) && (lhs.lf == rhs.lf));
  }
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

  // note this is different from *lhs < *rhs !
  // Comparison of edges is based on vertices
  friend bool operator< (const self& lhs, const self& rhs) {
    return ( (lhs.C < rhs.C) || ((lhs.C == rhs.C) && (lhs.lf < rhs.lf)));
  }


  void print(::std::ostream& out)  const;
  friend ::std::ostream& operator<<(::std::ostream& out, const EdgeOnCell2D_Iterator& it)
    { it.print(out); return out; }

  bool bound() const { return C.valid();}
  bool valid() const { return C.valid() && (0 <= lf) && (lf < C.NumOfEdges());}
};


} // namespace GrAL

#endif
