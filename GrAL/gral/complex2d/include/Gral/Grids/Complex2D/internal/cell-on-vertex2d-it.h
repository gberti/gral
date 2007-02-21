#ifndef GRAL_GB_COMPLEX2D_CELL_ON_VERTEX2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_CELL_ON_VERTEX2D_ITERATOR_H



// $LICENSE


#include "Gral/Grids/Complex2D/complex2d.h"

namespace GrAL { 

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

  Cell operator*() const {return Cell(TheGrid(),V.cell(lc));}

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
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

};

} // namespace GrAL

#endif
