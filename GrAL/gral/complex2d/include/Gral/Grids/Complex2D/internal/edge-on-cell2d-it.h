#ifndef NMWR_GB_EDGE_ON_CELL2D_ITERATOR_H
#define NMWR_GB_EDGE_ON_CELL2D_ITERATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Utility/pre-post-conditions.h"
#include "Gral/Grids/Complex2D/complex2d.h"

class EdgeOnCell2D_Iterator : public complex2d_types {
  friend class CellOnCell2D_Iterator;
  friend class Complex2D;
  friend class Cell2D;
  friend class Edge2D;
  friend class Edge2D_Iterator;

  typedef EdgeOnCell2D_Iterator   self;
private:
  //----- DATA --------
  Cell            C;  // anchor
  int             lf; // local facet, \in [0, TheCell().NumOfFacets()]

public:
  typedef Cell   anchor_type;
  typedef Edge   element_type;
  typedef Edge   value_type;

  //--------- constructors -----------------------------------

  EdgeOnCell2D_Iterator() 
    : C(),   lf(0)  {}
  explicit EdgeOnCell2D_Iterator(const Cell& CC)
    : C(CC), lf(0)   {}
  EdgeOnCell2D_Iterator(int cnt, const Cell& CC)
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

  self& operator++()      { ++lf;   return (*this);}
  self& operator--()      { --lf;   return (*this);}

  
  inline self  CyclicSucc() const;
  inline self  CyclicPred() const;

  // inline operator Cell2D_Iterator() const;
 
  bool IsDone() const {return ((lf < 0) || (lf >= TheCell().NumOfEdges()));}
  operator bool() const { return !IsDone();}
  int LocalNumber() const { return (lf+1);}
  Cell const& TheCell()  const;
  Cell        OtherCell() const;

  //---------------- anchor access ------------------

  anchor_type const&  TheAnchor() const { return C;}
  Complex     const&  TheGrid()   const { return C.TheGrid();}

  //----------------- comparison  -----------------------

  friend bool operator==(const self& ls, const self& rs) {
    return ((ls.C == rs.C) && (ls.lf == rs.lf));
  }

  void print(ostream& out)  const;
  friend ostream& operator<<(ostream& out, const EdgeOnCell2D_Iterator& it)
    { it.print(out); return out; }
};



#endif
