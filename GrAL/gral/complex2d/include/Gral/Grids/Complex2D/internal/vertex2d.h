#ifndef NMWR_GB_VERTEX2D_H
#define NMWR_GB_VERTEX2D_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Grids/Complex2D/complex2d.h"

class Vertex2D : public complex2d_types {
  //---- DATA ----
private:
  vertex_handle _pos; 
  ComplexPtr    _cc;  // reference to grid

public:
  typedef Vertex2D             self;
  typedef CellOnVertexIterator CellIterator;

  //----------  construction -------------------

  Vertex2D() : _pos(-1), _cc(0) {}
  Vertex2D(vertex_handle  pos, Complex2D const& cc) 
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

  friend ostream& operator <<(ostream& out, self const& rs)
    { return (out << rs._pos << ' ' << rs._cc); }

private:
  friend class Complex2D;
  friend class CellOnVertex2D_Iterator;

  inline cell_handle  cell(int lc) const;

  vertex_base const& base() const;
};


#endif
