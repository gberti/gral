#ifndef GRAL_GB_COMPLEX2D_VERTEX_ON_CELL2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_VERTEX_ON_CELL2D_ITERATOR_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Grids/Complex2D/complex2d.h"

namespace GrAL {

class VertexOnCell2D_Iterator :  public complex2d_types {
  typedef VertexOnCell2D_Iterator  self;
  friend class Complex2D;
private:
  //---- DATA ------
  Cell C;
  int  lv;

public:
  typedef grid_incidence_iterator_category_d<0,2>  category;
  typedef Vertex value_type; 
  typedef Vertex element_type;
  typedef Cell   anchor_type;

  
  //--------- constructors -----------------------------------

  VertexOnCell2D_Iterator() {} 
  VertexOnCell2D_Iterator(int vv, const Cell& CC)
    : C(CC),  lv(vv) {}
  explicit VertexOnCell2D_Iterator(Cell const& CC) 
    : C(CC),  lv(0) {}
  
  ~VertexOnCell2D_Iterator() {}

  //----------- iterator operations --------------------------

  Vertex operator*() const { return Vertex2D(TheGrid(), C.vertex(lv));}

  self& operator++() { ++lv;  return (*this);}
  self& operator--() { --lv;  return (*this);}

  self  CyclicSucc() const 
  { return self((lv == TheCell().NumOfVertices()-1 ? 0   : lv+1), C);}
  self  CyclicPred() const 
  { return self((lv == 0 ? TheCell().NumOfVertices()-1   : lv-1), C);}

  bool IsDone() const { return (lv == C.NumOfVertices());}
  operator bool() const { return !IsDone();}

  int local_handle() const { return (lv);}


  //------- anchors ---------------------

  Cell      const& TheCell()   const { return  C;}
  Cell      const& TheAnchor() const { return  C;}
  Complex2D const& TheGrid()   const { return (C.TheGrid());}

  vertex_handle handle() const { return (C.vertex(lv));}

  //----------------- comparison  -----------------------

  friend bool operator==(const self& lhs, const self& rhs) 
  { return ((lhs.lv == rhs.lv) && (lhs.C == rhs.C));}
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

  bool valid()    const { return bound() && in_range();}
  bool bound()    const { return C.valid();}
  bool in_range() const { return (0 <= lv && lv < C.NumOfVertices());}
 
};

} // namespace GrAL {

#endif

