#ifndef NMWR_GB_NEUMANN_TRIANG2D_H
#define NMWR_GB_NEUMANN_TRIANG2D_H

// $LICENSE

#include "Config/compiler-config.h"
#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
/*! \brief  A triangulation view of a grid with quadrangular cells.

  It fullfils the requirements of a $GrAL Cell-VertexInputGridRange.

  It is assumed that the vertices of cells are visited in order
  by GRID::VertexOnCellIterator, that is, any to consecutive values
  of this iterator are adjacent.
 
  In principle, this belongs to the functionality of the archetype
  (comb. model) of the cells.
  A similar approach in 3D breaks down, because there is no such
  'natural' archetype that can be implicetly assumed.
  Therefore, we need an explicit archetype with a default triangulation.
*/
//----------------------------------------------------------------


template<class GRID>
class neumann_triang2d {
public:
  typedef GRID                        grid_type;
  typedef grid_types<grid_type>       gt;
  typedef typename gt::Vertex         Vertex1; // suffix "1" used to avoid g++ - confusion.
  typedef typename gt::VertexIterator VertexIterator1;
  typedef typename gt::vertex_handle  vertex_handle1;

  neumann_triang2d(const grid_type& G) : the_grid(&G) {}

  //---------------- ELEMENTS & ITERATORS  ----------------------------
  //
  // new types for Cell, CellIterator, VertexOnCellIterator
  //
  //-------------------------------------------------------------------

  class VertexOnCellIterator1;


/*

 4       3
  ------+           von Neumann subdivision of a rectangle
  | C2 /|
  |   / |
  |  /  |
  | /   |
  |/ C1 |
  +-----+
 1       2

*/

  class Cell1 {
  public: 
    typedef typename gt::Cell gtCell;
    typedef typename gtCell::corner corner1;
    Cell1(typename gt::Cell C,int t = 1) : c(C), triang(t) {}

    Vertex1 vertex1(int v) const {
      if( triang == 1) // vertices 1,2,3
        return c.vertex(corner1(--v));
      else {           // vertices 1,3,4
        return (v == 1 ? c.vertex(corner1(--v)) : c.vertex(corner1(v)));
      }
    }
    VertexOnCellIterator1 FirstVertex() const;
    unsigned NumOfVertices() const { return 3;}
    //    const neumann_triang2d& TheGrid() const;
    int handle() const { return (2* c.handle() + triang -1);}

  private:
    gtCell c;
    int triang;  // 1 or 2
    // const neuman_triangulation2d* triang_grid;
  };
  
  //-------------------------------------------------------------------


  class VertexOnCellIterator1 {
    //typedef VertexOnCellIterator self;
  private:
    Cell1 c;
    int v;
  public:
    VertexOnCellIterator1(const Cell1& C) : c(C), v(1) {}
    Vertex1 operator*() const { return c.vertex1(v);}
    VertexOnCellIterator1& operator++() { ++v; return *this;}
    bool IsDone() const { return (v>3);}
    int handle() const { return c.vertex1(v).handle();} 
  };

  //-------------------------------------------------------------------

  class CellIterator1 {
    //  typedef CellIterator self;
  public:
    CellIterator1(typename gt::CellIterator C, int t = 1) : c(C), triang(t) {}

    Cell1 operator*() const { return  Cell1(*c,triang);}
    CellIterator1& operator++() { 
      if(triang == 1)
        ++triang;
      else {
        triang =1;
        ++c;
      }
      return *this;
    }
    bool IsDone() const { return c.IsDone(); }
    int handle() const { return (2* c.handle() + triang -1);}
  private:
    typename gt::CellIterator c;
    int triang;
  };


  //-------------------------------------------------------------------

  VertexIterator1 FirstVertex() const { return TheGrid().FirstVertex();}
  CellIterator1   FirstCell()   const { return CellIterator1(TheGrid().FirstCell(),1);}
  
  unsigned  NumOfVertices()   const { return TheGrid().NumOfVertices();}
  unsigned  NumOfCells()      const { return 2*TheGrid().NumOfCells();}

  const grid_type& TheGrid() const { return *the_grid;}

  typedef typename gt::vertex_handle vertex_handle;
  vertex_handle handle(const Vertex1&         v) const { return TheGrid().handle(v);}
  vertex_handle handle(const VertexIterator1& v) const { return TheGrid().handle(v);}
  vertex_handle handle(const VertexOnCellIterator1& v) const { return TheGrid().handle(*v);}

  // map own handles to handles of base grid
  // a better name: father() ? ancestor() ? 
  vertex_handle base_vertex_corr(const vertex_handle& v) const { return v;}
    //  cell_handle   base_cell_corr(const cell_handle& c) const {
    //  Cell cell(cell_handle h) const { return Cell(h/2, (h%2) +1);}

  //-------------------------------------------------------------------

private:
  //-- DATA --//
  const grid_type* the_grid;
};

template<class GRID>
inline
typename neumann_triang2d<GRID>::VertexOnCellIterator1
neumann_triang2d<GRID>::Cell1::FirstVertex() const
{ return VertexOnCellIterator1(*this);}


template<class GRID>
struct grid_types_nt2d  {
  typedef neumann_triang2d<GRID>      nt;

  typedef typename nt::Vertex1               Vertex;
  typedef typename nt::Cell1                 Cell;
  typedef typename nt::VertexIterator1       VertexIterator;
  typedef typename nt::CellIterator1         CellIterator;
  typedef typename nt::VertexOnCellIterator1 VertexOnCellIterator;


  typedef grid_types<GRID>                   reggt;
  typedef typename reggt::vertex_handle      vertex_handle;
  typedef typename reggt::cell_handle        cell_handle;
};


// this has to be done 'by hand' if partial specialization
// is not available.

template<class GRID>
struct grid_types<neumann_triang2d<GRID> > 
  : public grid_types_nt2d<GRID>  {};


#endif
