#ifndef GRAL_GB_GRIDS_TRIANG2D_H
#define GRAL_GB_GRIDS_TRIANG2D_H

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Triang2D/grid-types.h"

#include "Gral/Iterators/facet-by-cell-it.h"
#include "Gral/Iterators/vertex-on-edge-iterator.h"
#include "Gral/Iterators/generic-edge-iterators.h"

#include <algorithm>


class grid_types_Triang2D :
  public grid_types_base_Triang2D 
{
  typedef vertex_on_edge_iterator<Triang2D, grid_types_Triang2D>
  VertexOnEdgeIterator;
  typedef VertexOnEdgeIterator VertexOnFacetIterator;;
};

#ifdef NMWR_DEBUG
#define DEBUG_ONLY(code)  if(true)  { code }
#else
#define DEBUG_ONLY(code)  if(false) { code }
#endif

/*! \brief A simple grid class for triangulations.

 This class may act as a wrapper with reference-semantics for
 a plain array containing the cell-vertex connectivity.
 Value semantics may be enforced by a call to the member 
 function DoCopy().

 A copy by assignment or copy-ctor will always entail
 value-semantics on the copied-to side, i.e. make a full copy.

*/

/*
class grid_types_Triang2D :
  public generic_edge::grid_types_edge<grid_types_base_Triang2D> {};
*/



class Triang2D : public grid_types_Triang2D {
private:
  int* cells;
  bool owned;
  int  ncells;
  int  nvertices;
public:
  Triang2D() 
    : cells(0), owned(false), ncells(0), nvertices(0) {}
  Triang2D(int* c, int nc) 
    : cells(c), owned(false), ncells(nc) 
  { nvertices = calc_num_of_vertices(); }
  Triang2D(int* c, int nc, int nv) 
    : cells(c), owned(false), ncells(nc), nvertices(nv) {} 

  // make physical copy
  Triang2D(Triang2D const& rhs);
  Triang2D& operator=(Triang2D const& rhs);

  ~Triang2D();

  /*! make a physical copy, if only referencing the grid,
    thus gaining ownership of cells. */
  void DoCopy();
  /*! set this to the connectivity given by c.
    Assume ownership of c *without* doing a copy. */
  void Steal(int* c, int nc, int nv);
private:
  void clear(); 
  void do_copy();
  int  calc_num_of_vertices();

public:

  friend class Triang2D_VertexOnCellIterator;
  friend class Triang2D_FacetOnCellIterator;
  friend class Triang2D_Cell;
  friend class Triang2D_Vertex;

  int NumOfCells   () const { return ncells   ;}
  int NumOfVertices() const { return nvertices;}

  inline VertexIterator FirstVertex() const;
  inline EdgeIterator   FirstEdge()   const;
  inline FacetIterator  FirstFacet()  const;
  inline CellIterator   FirstCell()   const;

};


//------------ Cell / CellIterator ----------------

/*
class Triang2D_Cell : public grid_types_Triang2D::cell_base_type
{
  typedef grid_types_Triang2D::cell_base_type base;
public:
  Triang2D_Cell() {};
  Triang2D_Cell(grid_type const& g) : base(g) {}  
  Triang2D_Cell(grid_type const& g, cell_handle h) : base(g,h) {}

  VertexOnCellIterator FirstVertex() const;
  unsigned NumOfVertices() const { return 3;}
};
*/

class Triang2D_Cell : public grid_types_Triang2D  {
  typedef Triang2D_Cell     self;
  friend class     Triang2D_VertexOnCellIterator;
  friend class     Triang2D_FacetOnCellIterator;
private: 
  grid_type const* g;
  cell_handle      c;
public:
  Triang2D_Cell() : g(0), c(-1) {}
  explicit
  Triang2D_Cell(grid_type const& gg) : g(&gg), c(0) {}
  Triang2D_Cell(grid_type const& gg,
		cell_handle      cc) : g(&gg), c(cc) {}
  ~Triang2D_Cell() {}  
  
  bool IsDone()   const { return (c == g->ncells); }
  self const& operator*() const { return *this;}
  self & operator++() { ++c; return *this;}
  
  grid_type const& TheGrid() const { return *g;}
  cell_handle   handle() const { return c;}

  FacetOnCellIterator  FirstFacet () const;
  VertexOnCellIterator FirstVertex() const;

  unsigned NumOfVertices() const { return 3;}
  unsigned NumOfEdges()    const { return 3;}
  unsigned NumOfFacets()   const { return 3;}

  // checking functions
  bool bound() const { return g != 0;}
  bool valid() const { return (bound() &&  (0 <= c) && (c < 3));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}
};


//----------- Vertex / VertexIterator ----------------

class Triang2D_Vertex : public grid_types_Triang2D {
  typedef Triang2D_Vertex    self;
public:
  typedef self      value_type;
private: 
  grid_type const* g;
  vertex_handle    v;
public:
  Triang2D_Vertex() : g(0), v(-1) {}
  explicit
  Triang2D_Vertex(grid_type const& gg) : g(&gg), v(0) {}
  Triang2D_Vertex(grid_type const& gg, int vv) : g(&gg), v(vv) {}
  ~Triang2D_Vertex() {}  
  
  bool IsDone()   const { cb(); return (v == g->nvertices); }
  self const& operator*() const { cv(); return *this;}
  self & operator++() { cv(); ++v; return *this;}
  
  grid_type const& TheGrid() const { cb(); return *g;}
  vertex_handle    handle()  const { cv(); return v;}

  // checking functions
  bool bound() const { return g != 0;}
  bool valid() const 
    { return (bound() &&  (0 <= v) && (v < TheGrid().NumOfVertices()));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}
};


//----------------- VertexOnCellIterator ---------------------

class Triang2D_VertexOnCellIterator : public grid_types_Triang2D {
  typedef  Triang2D_VertexOnCellIterator self;
private:
  Cell c;
  int  vc;
public:
  Triang2D_VertexOnCellIterator() : vc(-1) {}
  explicit
  Triang2D_VertexOnCellIterator(Cell const& cc) : c(cc), vc(0) {}
  
  self& operator++() { cv();  ++vc; return *this;}
  Vertex operator*() const 
    { cv(); return Triang2D_Vertex(TheGrid(), handle());}
  bool IsDone()   const { cb(); return (vc == 3);}
  
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}
  Cell const& TheCell() const { cb(); return c;}


  vertex_handle handle() const 
    { cv(); return TheGrid().cells[3*c.handle()+vc];}
  int local_handle()     const { cv(); return vc;}

  // checking functions
  bool bound() const { return c.valid();}
  bool valid() const { return (c.valid() &&  (0 <= vc) && (vc < 3));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}

};


//-------------- FacetOnCellIterator ---------------------

struct edge_handle_Triang2D : public grid_types_Triang2D {
  cell_handle c;
  int         lf;
  edge_handle_Triang2D() { DEBUG_ONLY( c=-1; lf = -1; ); }
  edge_handle_Triang2D(cell_handle cc, int llf) : c(cc), lf(llf) {}
};


class Triang2D_FacetOnCellIterator : public grid_types_Triang2D {
  typedef  Triang2D_FacetOnCellIterator self;
private:
  Cell c;
  int  fc;
public:
  Triang2D_FacetOnCellIterator() : fc(-1) {}
  explicit
  Triang2D_FacetOnCellIterator(Cell const& cc, int ffc = 0) 
    : c(cc), fc(ffc) 
    { REQUIRE( valid(), "invalid iterator!",1); }

  self&        operator++() { ++fc; return *this;}
  Facet        operator*()  const;
  bool IsDone()   const { cb(); return (fc == 3);}

  Vertex V1() const { cv(); return Vertex(*c.g, v1());}
  Vertex V2() const { cv(); return Vertex(*c.g, v2());}
  vertex_handle v1() const { cv(); return TheGrid().cells[3*c.c+ fc];}
  vertex_handle v2() const { cv(); return TheGrid().cells[3*c.c+ (fc+1)%3];}

  Cell      const& TheCell() const { cb(); return c;}
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}
  edge_handle  handle()     const 
    { cv(); return edge_handle(c.handle(), fc);} 
  int local_handle() const { cv(); return fc;}
  
  // checking functions

  bool bound() const { return c.bound();}
  bool valid() const { return (c.valid() && (0 <= fc) && (fc < 3));}
  void cv() const { REQUIRE(valid(), "", 1);}
  void cb() const { REQUIRE(bound(), "", 1);}
};



class Triang2D_Edge : public grid_types_Triang2D {
  typedef Triang2D_Edge  self;
private:
  Triang2D_FacetOnCellIterator fc;
public:
  Triang2D_Edge() {}
  explicit
  Triang2D_Edge(Triang2D_FacetOnCellIterator ffc) : fc(ffc) {}
  explicit
  Triang2D_Edge(grid_type const& g) 
    : fc(Cell(g,cell_handle(0)), 0) {}
  Triang2D_Edge(grid_type const& g, edge_handle e) 
    : fc(Cell(g,e.c), e.lf) {}

  edge_handle handle() const { return fc.handle();}

  unsigned NumOfVertices() const { return 2;}
  VertexOnEdgeIterator FirstVertex() const { return VertexOnEdgeIterator(*this);}
  vertex_handle v1() const { return fc.v1();}
  vertex_handle v2() const { return fc.v2();}
  Vertex V1() const { return fc.V1();}
  Vertex V2() const { return fc.V2();}

  Cell   TheCell() const { return fc.TheCell();}

  friend bool operator==(self const& lhs, self const& rhs)
  { 
    vertex_handle lv1 = lhs.V1().handle();
    vertex_handle lv2 = lhs.V2().handle();
    vertex_handle rv1 = rhs.V1().handle();
    vertex_handle rv2 = rhs.V2().handle();
    return (((lv1 == rv1) && (lv2 == rv2))
	    ||
	    ((lv2 == rv1) && (lv1 == rv2)));
  }

  friend bool operator<(self const& lhs, self const& rhs)
  {
    vertex_handle max1(std::max(lhs.V1().handle(),lhs.V2().handle()));
    vertex_handle max2(std::max(rhs.V1().handle(),rhs.V2().handle()));
  
    return ( ( max1 < max2)  
	     || 
	     ((max1 == max2) 
	       && (  std::min(lhs.V1().handle(),lhs.V2().handle()) 
		   < std::min(rhs.V1().handle(),rhs.V2().handle()))));
  }

};



struct grid_types<Triang2D> : public grid_types_Triang2D 
{

  static vertex_handle handle(Vertex const& V) { return V.handle();}
  static cell_handle   handle(Cell   const& C) { return C.handle();}

  static int  local (FacetOnCellIterator  const& f) { return f.local_handle();}
  static int  local (VertexOnCellIterator const& v) { return v.local_handle();}

  static cell_handle invalid_cell_handle(Triang2D const&) { return -1;}
};


#include "Gral/Grids/Triang2D/partial-grid-functions.h"

class Triang2D_FacetIterator 
  : public facet_set_of_cells_iterator<Triang2D_Cell>
{
  typedef facet_set_of_cells_iterator<Triang2D_Cell> base;
 public: 
  Triang2D_FacetIterator() : base(CellIterator()) {}
  Triang2D_FacetIterator(CellIterator  const& c) : base(c) {}
  Triang2D_FacetIterator(grid_type const& g) : base(CellIterator(g)) {}
};

//------------------- inline functions ---------------

inline
Triang2D::VertexIterator
Triang2D::FirstVertex() const { return VertexIterator(*this);}

inline
Triang2D::EdgeIterator
Triang2D::FirstEdge() const { return EdgeIterator(*this);}

inline
Triang2D::FacetIterator
Triang2D::FirstFacet() const { return FacetIterator(*this);}



inline
Triang2D::CellIterator
Triang2D::FirstCell() const { return CellIterator(*this);}

inline
Triang2D_VertexOnCellIterator
Triang2D_Cell::FirstVertex() const { return VertexOnCellIterator(*this);}


inline
Triang2D_FacetOnCellIterator
Triang2D_Cell::FirstFacet() const { return FacetOnCellIterator(*this);}

inline
Triang2D_Edge 
Triang2D_FacetOnCellIterator::operator*() const
{ return Triang2D_Edge(*this); }



#endif