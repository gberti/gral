#ifndef GRAL_BASE_GB_BOUNDARY_ITERATOR_H
#define GRAL_BASE_GB_BOUNDARY_ITERATOR_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-functors.h"

//----------------------------------------------------------------
/*! \defgroup boundarycompiter Boundary Component Iterators

  \brief A set of general iterators for the boundary of a (subset of) a 2D grid
         described by a predicate on cells.
  \ingroup iterators
  \ingroup boundary
  
  \contents
   - class BoundaryComponentIterator<Grid,CellPred>;
   - class BoundaryComponentVertexIterator<Grid,CellPred>;
   - class BoundaryComponentEdgeIterator<Grid,CellPred>;

  \description
   - class BoundaryComponentIterator2D<Grid,CellPred>:
    This class takes a seed-triple (V,E,C) of pairwise
    incident vertex V, edge E and cell C, and a predicate "inside",
    where it is supposed that E "lies on the boundary" with
    respect to "inside" 
    (<tt> inside(E.C1()) != inside(E.C2()) </tt>)
    and <tt> C == E.Ci()</tt>  \f$ \iff\f$ <tt>inside(C) == true </tt>. <BR>
    BoundaryComponentIterator2D steps through the  vertices, edges 
    and adjacent cells of the connected boundary component of E. <BR>
    \b Note: if there is more than one boundary component, this
      iterator will visit only that one containing E!
   -  BoundaryComponentVertexIterator2D and BoundaryComponentEdgeIterator2D
   are just wrappers forBoundaryComponentIterator2D  
   that provide the correct   \c operator* . <BR> 
   An analogon for cells would have to check if the
   current cell changed when incrementing the base iterator.

   \templateparams
   - Grid: grid_types<Grid> is defined.
   - CellPred: model of Unary Function Cell --> bool: <BR>
    <tt> bool operator()(Cell) const </tt>


   \todo
   A 3D analogue requires more work, as the boundary isn't
   linearly ordered any more.
   \todo
   Could use partial specialization based on the 
   dimension of Grid to remove the syntactic 2D dependency
   in the name.
  
  \see Module \ref iterators
  \see Module \ref boundaryrange
*/
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

/*! \brief Iterator visiting vertices and edges of a boundary component
    of a 2D grid 
    \ingroup boundarycompiter
  
   \see Module boundarycompiter
   \see boundary_range_generic
 */
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

  /*! \pre ins(E1.C1()) != ins(E1.C2()) (that is, E1 is boundary facet)
   */
  BoundaryComponentIterator2D(const Edge&   E1, const CellPred& ins)
   : inside(ins),
      V(E1.V2()), 
      E(E1), 
      Estart(E1), 
      done(false) 
    {
      // why necessary?
      if(! TheGrid().IsInside(E1.C1()))
       C = E1.C2();
      else 
       C = (! inside(E1.C1()) ? E1.C2() : E1.C1());
    }

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


/*! \brief Iterator visiting the vertices  of a boundary component
    of a 2D grid 
    \ingroup boundarycompiter
  
   \see Module boundarycompiter
   \see boundary_range_generic
   \see BoundaryComponentIterator2D
 */
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


/*! \brief Iterator visiting the edges of a boundary component
    of a 2D grid 
    \ingroup boundarycompiter
  
   \see Module boundarycompiter
   \see boundary_range_generic
   \see BoundaryComponentIterator2D
 */
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
