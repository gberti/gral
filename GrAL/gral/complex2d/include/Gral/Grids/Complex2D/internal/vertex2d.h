#ifndef GRAL_GB_COMPLEX2D_VERTEX2D_H
#define GRAL_GB_COMPLEX2D_VERTEX2D_H



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

class Vertex2D : public complex2d_types {
  //---- DATA ----
private:
  vertex_handle _pos; 
  ComplexPtr    _cc;  // reference to grid

public:
  typedef Vertex2D             self;
  typedef CellOnVertexIterator CellIterator;

  typedef grid_element_category_d<0>  category;
  typedef vertex_type_tag             element_type_tag;


  //----------  construction -------------------

  Vertex2D() : _pos(-1), _cc(0) {}
  explicit Vertex2D(Complex2D const& cc);
  Vertex2D(Complex2D const& cc, vertex_handle  pos)
    : _pos(pos), _cc(&cc) {}


  ~Vertex2D() {}

  //--------------- iteration & sizes -------------------

  CellOnVertexIterator FirstCell() const;
  CellOnVertexIterator EndCell()   const;

  int NumOfCells()    const { return (base()._cells.size());}
  int NumOfEdges()    const { return NumOfCells();}
  int NumOfVertices() const { return NumOfCells();}



  //-------------- anchors ------------

  Complex const&  TheGrid()     const {return (*_cc);}
  Complex const&  TheAnchor()   const {return (*_cc);}
 
  vertex_handle handle() const { return _pos;}

  //----------------- comparision --------------

  friend bool operator<(const self& lhs, const self& rhs)
  { return (lhs._pos < rhs._pos);}
  friend bool operator==(const self& lhs, const self& rhs)
  { return (lhs._pos == rhs._pos);}
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}
  friend ::std::ostream& operator <<(::std::ostream& out, self const& rs)
    { return (out << rs._pos << ' ' << rs._cc); }

  inline bool valid() const; 
private:
  friend class Complex2D;
  friend class CellOnVertex2D_Iterator;

  inline cell_handle  cell(int lc) const;

  vertex_base const& base() const;
};

} // namespace GrAL {

#endif
