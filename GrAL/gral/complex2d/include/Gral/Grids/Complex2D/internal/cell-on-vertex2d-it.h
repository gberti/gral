#ifndef NMWR_GB_CELL_ON_VERTEX2D_ITERATOR_H
#define NMWR_GB_CELL_ON_VERTEX2D_ITERATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Gral/Grids/Complex2D/complex2d.h"

class CellOnVertex2D_Iterator : public complex2d_types {
  friend class Complex2D;
  typedef CellOnVertex2D_Iterator   self;

   //------- DATA -----
  int    lc;   // local cell 
  Vertex V;   // anchor

public:

  typedef Cell   value_type;
  typedef Cell   element_type;
  typedef Vertex anchor_type;

  //--------- constructors -----------------------

  CellOnVertex2D_Iterator() 
     {}
  CellOnVertex2D_Iterator(const Vertex& VV)
    : lc(0), V(VV) {}
  CellOnVertex2D_Iterator(int c, const Vertex& VV)
    :  lc(c), V(VV) {}

  ~CellOnVertex2D_Iterator() {}


  //------------ iterator operations -------------------

  Cell operator*() const {return Cell(V.cell(lc), TheGrid());}

  self& operator++()   { ++lc;  return (*this);}
  self& operator--()   { --lc;  return (*this);}

  //  inline self  CyclicSucc() const;
 
  bool IsDone()      const { return (lc >= V.NumOfCells());}
  operator bool()    const { return !IsDone();}
  int  LocalNumber() const { return (lc+1);}

  //----------------- anchor access ---------------------

  Vertex  const& TheVertex()  const {return V;}
  Vertex  const& TheAnchor()  const {return V;}
  Complex const& TheGrid()    const {return (V.TheGrid());}

  //----------------- comparison  -----------------------

  friend bool operator==(const self& lhs, const self& rhs) 
  {return ((lhs.lc == rhs.lc) && (lhs.V == rhs.V));}

};


#endif
