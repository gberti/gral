#ifndef GRAL_GB_COMPLEX2D_BOUNDARY_VERTEX_COMPLEX2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_BOUNDARY_VERTEX_COMPLEX2D_ITERATOR_H



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

//----------------------------------------------------------------
//
//  This class takes a seed-triple (V,E,C) of pairwise
//  adjacent vertex V, edge E and cell C, where it is
//  supposed that E "lies on the boundary". 
//  Then the iterator BoundaryVertex2D_Iterator steps through the
//  vertices of the connected boundary component of E.
//  NOTE: if there are more than one boundary component, this
//  iterator will visit only that one containing E!
//
//  The test whether an edge lies on the boundary could be replaced
//  by another predicate partitioning the cells of the grid, such
//  that iteration occurs over one connected boundary component of
//  the partition containing C, namely that component containing E.
//
//  The component is supposed to be closed: every vertex has exactly
//  2 adjacent edge E1, E2 which lie on the boundary.
//
//----------------------------------------------------------------

class BoundaryComponent_Vertex2D_Iterator {
  typedef Complex2D grid_type;
  typedef grid_type gt; 
  typedef  gt::Vertex Vertex;
  typedef  gt::Edge   Edge;
  typedef  gt::Cell   Cell;

  typedef  BoundaryComponent_Vertex2D_Iterator self;

  //--- DATA ---
private:
  Vertex V;
  Edge   E, Estart;
  Cell   C;
  bool   done;
public:
  //----------  construction ------------------------------
  BoundaryComponent_Vertex2D_Iterator() : done(true) {}
  BoundaryComponent_Vertex2D_Iterator(const Edge&   E1)
    : V(E1.V2()), 
      E(E1), 
      Estart(E1), 
      C(E1.TheGrid().IsInside(E1.C1()) ? E1.C1() : E1.C2()), 
      done(false) 
    {}

  //--- copying ----
  BoundaryComponent_Vertex2D_Iterator(const self& rs) 
    : V(rs.V), E(rs.E), Estart(rs.Estart), C(rs.C), done(rs.done) {}
  self& operator=(const self& rs) {
    if(this != &rs) {
     V = rs.V; E = rs.E; Estart = rs.Estart; C = rs.C; done = rs.done;
    }
    return (*this);
  } 

  //------------------ iteration operators -----------------

  self& operator++() { advance(); return *this;}
  Vertex operator*() const { return V;}
  bool   IsDone() const { return done; }
  
  const grid_type& TheGrid() { return V.TheGrid();}

  //--------------------------------------------------------
private:

  void advance() {
    // find the other edge E' adj. to V and on the boundary,
    // by "rotating" E around V.
    C.FlipEdge(V,E); // E -> E' with E' adj. to V and C, and E' != E. ( E' is unique)
    while ( ! TheGrid().IsOnBoundary(E)) {
      E.FlipCell(C);  // E.C1 <-> E.C2
      C.FlipEdge(V,E);
    }
    E.FlipVertex(V);  // E.V1 <-> E.V2
    done = (E == Estart);
  }
};


} // namespace GrAL {

#endif
