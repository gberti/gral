#ifndef GRAL_GB_COMPLEX2D_CELL2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_CELL2D_ITERATOR_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Grids/Complex2D/complex2d.h"

namespace GrAL {

class Cell2D_Iterator : public complex2d_types {
private:

  typedef Cell2D_Iterator self;
  typedef Complex2D Complex;
  friend class Complex2D;

public:
  typedef grid_sequence_iterator_category_d<2>  category;
  typedef grid_type anchor_type;
  typedef Cell      value_type;

  //-------- DATA --------------
  // public for access by grid_function<Cell2D,T>
  cell_handle _pos;    // current cell position
private:
  ComplexPtr  _cc;     // reference to grid
  cell_handle _endpos; // past-the-end cell position

public:
  //--------------- constructors -----------------------------

  Cell2D_Iterator() : _pos(-1),  _cc(0), _endpos(-1) {}

  explicit Cell2D_Iterator(Complex2D const& CC) 
    : _pos(0), _cc(&CC), _endpos(CC.NumOfCells())
  {}

  Cell2D_Iterator(const cell_handle& c, Complex2D const& CC)
    : _pos(c),  _cc(&CC),  _endpos(CC.NumOfCells())
  {}
  Cell2D_Iterator(Complex2D const& CC, cell_handle c)
    : _pos(c),  _cc(&CC),  _endpos(CC.NumOfCells())
  {}
  
  Cell2D_Iterator(const Cell2D& C) 
    : _pos(C._pos), _cc(& C.TheGrid()), _endpos(C.TheGrid().NumOfCells())
  {}
 
  // copying
  Cell2D_Iterator(const self& s)
    { _pos=s._pos; _endpos = s._endpos; _cc = s._cc;}
  self& operator=(const self& s)
    { if (this != &s) { _pos=s._pos; _endpos = s._endpos; _cc = s._cc;}  return (*this);}
    

  //------------ iterator operations ----------------
  self& operator++() { ++_pos; return (*this);}
  Cell  operator*() const {return (_cc->cell(_pos));}
  bool IsDone() const { return (_pos == _endpos);}
  //   operator bool() const { return !IsDone();}


  //-------------- anchor ---------------------------

  const Complex2D& TheGrid  () const { return (*_cc);}
  const Complex2D& TheAnchor() const { return (*_cc);}

  cell_handle handle() const { return _pos;}

  friend bool operator==(const self& ls, const self& rs)
    { 
      REQUIRE((ls._cc == rs._cc), "comparsion between iterators from different grids!\n",1);
      return ( ls._pos == rs._pos);
    }
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}
  friend bool operator<(const self& ls, const self& rs)
    { 
      REQUIRE((ls._cc == rs._cc), "comparsion between iterators from different grids!\n",1);
      return ( ls._pos < rs._pos);
    }

  bool bound()    const { return (_cc != 0);}
  bool in_range() const { return (0 <= _pos && _pos < _endpos);}
  bool valid()    const { return ( bound() && in_range());}
};



/*
inline CellOnCell2D_Iterator::operator Cell2D_Iterator() const
{
  return Cell2D_Iterator(*_nb,_cc);
}
*/

} // namespace GrAL {

#endif
