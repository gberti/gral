#ifndef NMWR_GB_VERTEX_ON_EDGE2D_IT_H
#define NMWR_GB_VERTEX_ON_EDGE2D_IT_H

#include "Gral/Grids/Complex2D/complex2d.h"

class VertexOnEdge2D_Iterator : public complex2d_types {
  typedef VertexOnEdge2D_Iterator self;
private:
  Edge   e;
  int    ve;

public:
  //------------------- types ---------------------

  typedef Complex2D grid_type;
  typedef Vertex    element_type;
  typedef Vertex    value_type;
  typedef Edge2D    anchor_type;

  //----------------- construction -----------------

  VertexOnEdge2D_Iterator() : e(), ve(0) {}
  VertexOnEdge2D_Iterator(const Edge& ee) : e(ee), ve(1) {}
  VertexOnEdge2D_Iterator(const Edge& ee, int vv) : e(ee), ve(vv) {}

  //----------------- iteration --------------------

  Vertex operator*() const { return ( ve == 1 ? e.V1() : e.V2());}
  self& operator++()       { ++ve; return *this;}
  bool  IsDone()     const { return (ve > 2);}
  operator bool()    const { return !IsDone();}

  //--------------- anchor access -------------------

  const Complex2D& TheGrid()  const { return e.TheGrid();}
  const Edge&      TheEdge()  const { return e;}
  const Edge&      TheFacet() const { return e;}

  //--------------- comparisom ----------------------

  friend bool operator == (const self& ls, const self& rs) 
   { return ((ls.ve == rs.ve) && (ls.e == rs.e)); }

};

#endif
