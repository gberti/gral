#ifndef NMWR_GB_BOUNDARY_ITERATOR_H
#define NMWR_GB_BOUNDARY_ITERATOR_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/common-grid-basics.h"
#include "Grids/grid-functors.h"

//----------------------------------------------------------------
//
//  A general iterator for the boundary of a (subset of) a 2D grid
//  described by a predicate on cells.
//  
//  Contents:
//  ---------
//  [1] class BoundaryComponentIterator<Grid,CellPred>;
//  [2] class BoundaryComponentVertexIterator<Grid,CellPred>;
//  [3] class BoundaryComponentEdgeIterator<Grid,CellPred>;
//
//  Description:
//  ------------
//  [1] class BoundaryComponentIterator<Grid,CellPred>
//    This class takes a seed-triple (V,E,C) of pairwise
//    adjacent vertex V, edge E and cell C, and a predicate "inside",
//    where it is supposed that E "lies on the boundary" with
//    respect to "inside" : 
//        inside(E.C1()) != inside(E.C2()),
//    and C == E.Ci() <=> inside(C) == true.
//  BoundaryComponentIterator2D<> steps through the  vertices, edges 
//  and adjacent cells of the connected boundary component of E.
//  NOTE: if there are more than one boundary component, this
//  iterator will visit only that one containing E!
//
//  [2], [3] : These are just wrappers for [1] that provide the correct
//   operator* . An analogue for cells would have to check if the
//   current cell changed when incrementing the base iterator [1].
//
//  Constraints on template parameters:
//  -----------------------------------
//   - Grid: grid_types<Grid> is defined.
//   - CellPred: model of unary function Cell --> bool:
//       bool operator()(Cell) const
//
//  Preconditions:
//  -------------
//  - inside(E.C1()) != inside(E.C2())
//
//  ToDo:
//  -----
//  - A 3D analogue requires more work, as the boundary isn't
//    linearly ordered any more.
//  - There could be used partial specialization based on the 
//    dimension of Grid to remove the syntactic 2D dependency
//    in the name.
//
//----------------------------------------------------------------

/*----------------------------------------------------------------

template<class Grid, class CellPred>
class BoundaryComponentIterator 
  : public BoundaryComponentIterator_ND<Grid,Pred, typename Grid::dim_tag> 
{ ...};

template<class Grid, class CellPred, class dim>
BoundaryComponentIterator_ND {};


template<class Grid, class CellPred>
BoundaryComponentIterator_ND<tag2d> { ... code for 2D ...};

template<class Grid, class CellPred>
BoundaryComponentIterator_ND<tag3d> { ... code for 3D ...};

----------------------------------------------------------------*/

template<class Grid, class CellPred = iscellinside_pred<Grid> >
class BoundaryComponentIterator2D {
protected:
  typedef Grid                grid_type;
  typedef grid_type           gt; 
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Cell   Cell;

  typedef  BoundaryComponentIterator2D<Grid, CellPred> self;

  //--- DATA ---

  CellPred   inside;
  Vertex V;
  Edge   E, Estart;
  Cell   C;
  bool   done;
public:
  //----------  construction ------------------------------
  BoundaryComponentIterator2D() : done(true) {}
  // precondition: E1 is boundary facet, i.e.
  // inside(E1.C1()) != inside(E1.C2())

  BoundaryComponentIterator2D(const Edge&   E1, const CellPred& ins)
    : inside(ins),
      V(E1.V2()), 
      E(E1), 
      Estart(E1), 
      C(! inside(E1.C1()) ? E1.C2() : E1.C1()), 
      done(false) 
    {}

  //--- copying ----
  BoundaryComponentIterator2D(const self& rs) 
    : inside(rs.inside), V(rs.V), E(rs.E), Estart(rs.Estart), C(rs.C), done(rs.done) {}
  self& operator=(const self& rs) {
    if(this != &rs) {
      inside = rs.inside; V = rs.V; 
      E = rs.E; Estart = rs.Estart; C = rs.C; 
      done = rs.done;
    }
    return (*this);
  } 

  //------------------ iteration operators -----------------

  self& operator++() { advance(); return *this;}
  //  Vertex operator*() const { return V;}
  const Vertex& curr_vertex()  const { return V;}
  const Edge&   curr_edge( )   const { return E;}
  const Edge&   curr_facet()   const { return E;}
  const Cell&   curr_cell()    const { return C;}

  bool  IsDone() const { return done; }
  
  const grid_type& TheGrid() { return V.TheGrid();}

  //--------------------------------------------------------
private:
  bool IsOnBoundary(const Edge& Eb)
    { 
      return( Eb.TheGrid().IsOnBoundary(Eb) 
	      || (inside(Eb.C1()) ? ! inside(Eb.C2()) : inside(Eb.C2()))); 
    }
  void advance() {
    // find the other edge E' adj. to V and on the boundary,
    // by "rotating" E around V.
    C.FlipEdge(V,E); // E -> E' with E' adj. to V and C, and E' != E. ( E' is unique)
    //   while ( ! inside.IsOnBoundary(E)) {
    while ( ! IsOnBoundary(E)) {
      E.FlipCell(C);  // E.C1 <-> E.C2
      C.FlipEdge(V,E);
    }
    E.FlipVertex(V);  // E.V1 <-> E.V2
    done = (E == Estart);
  }
};


template<class Grid, class CellPred = iscellinside_pred<Grid> >
class BoundaryComponentVertexIterator2D 
  : public  BoundaryComponentIterator2D<Grid,CellPred> 
{
    //private:
  typedef BoundaryComponentIterator2D<Grid,CellPred> base;
public:
  // typedef typename base_it::Vertex   Vertex;
  typedef Vertex                     value_type;

  BoundaryComponentVertexIterator2D() {}
  BoundaryComponentVertexIterator2D(const base& bs) : base(bs) {}
  BoundaryComponentVertexIterator2D(const Edge& e)
    : base(e,IsCellInside(e.TheGrid())) {}
  BoundaryComponentVertexIterator2D(const Edge& e, const CellPred& ins)  
    : base(e,ins) {}

  const Vertex& operator*() const { return curr_vertex();}
};


template<class Grid, class CellPred = iscellinside_pred<Grid> >
class BoundaryComponentEdgeIterator2D 
  : public  BoundaryComponentIterator2D<Grid,CellPred>
{
private:
  typedef BoundaryComponentIterator2D<Grid,CellPred> base;
public:
  typedef Edge                                 value_type;

  BoundaryComponentEdgeIterator2D() {}
  BoundaryComponentEdgeIterator2D(const base& bs) : base(bs) {}
  BoundaryComponentEdgeIterator2D(const Edge& e)
    : base(e,IsCellInside(e.TheGrid())) {}
  BoundaryComponentEdgeIterator2D(const Edge& e,const CellPred& ins)    
    : base(e,ins) {}

  const Edge& operator*() const { return curr_edge();}

};



#endif
