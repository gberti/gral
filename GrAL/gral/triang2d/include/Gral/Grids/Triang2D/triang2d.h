#ifndef NMWR_GB_GRIDS_TRIANG2D_H
#define NMWR_GB_GRIDS_TRIANG2D_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Utility/pre-post-conditions.h"
#include "Grids/common-grid-basics.h"
#include "Grids/Triang2D/grid-types.h"

#include "Grids/Iterators/facet-by-cell-it.h"
#include <algorithm>

//----------------------------------------------------------------
//
// Triang2D - a simple grid class for triangulations.
//
// This class may act as a wrapper with reference-semantics for
// a plain array containing the connectivity in FORTRAN style.
// Value semantics may be enforced by a call to the member 
// function DoCopy().
// A copy by assignment or copy-ctor will always entail
// value-semantics on the copied-to side, i.e. make a full copy.
//
//----------------------------------------------------------------


class Triang2D : public grid_types_base_Triang2D {
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

  // make a physical copy, if only referencing the grid,
  // thus gaining ownership of cells.
  void DoCopy();
  // set this to the connectivity given by c.
  // Assume ownership of c *without* doing a copy.
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
};


//------------ Cell / CellIterator ----------------

class Triang2D_Cell : public grid_types_base_Triang2D  {
  typedef Triang2D_Cell     self;
  friend class     Triang2D_VertexOnCellIterator;
  friend class     Triang2D_FacetOnCellIterator;
public:
  typedef Triang2D grid_type;
  typedef int       handle_type;
  typedef self      value_type;
private: 
  grid_type const* g;
  int              c;
public:
  Triang2D_Cell() : g(0), c(-1) {}
  Triang2D_Cell(grid_type const& gg) : g(&gg), c(0) {}
  ~Triang2D_Cell() {}  
  
  bool IsDone()   const { return (c == g->ncells); }
  operator bool() const { return (c <  g->ncells); }
  self const& operator*() const { return *this;}
  self & operator++() { ++c; return *this;}
  
  grid_type const& TheGrid() const { return *g;}
  handle_type handle() const { return c;}

  VertexOnCellIterator FirstVertex() const;
  FacetOnCellIterator  FirstFacet () const;

  unsigned NumOfVertices() const { return 3;}
  unsigned NumOfEdges()    const { return 3;}
  unsigned NumOfFacets()   const { return 3;}
};

//----------- Vertex / VertexIterator ----------------

class Triang2D_Vertex : public grid_types_base_Triang2D {
  typedef Triang2D_Vertex    self;
public:
  typedef Triang2D grid_type;
  typedef int       handle_type;
  typedef self      value_type;
private: 
  grid_type const* g;
  int              v;
public:
  Triang2D_Vertex() : g(0), v(-1) {}
  Triang2D_Vertex(grid_type const& gg) : g(&gg), v(0) {}
  Triang2D_Vertex(grid_type const& gg, int vv) : g(&gg), v(vv) {}
  ~Triang2D_Vertex() {}  
  
  bool IsDone()   const { return (v == g->nvertices); }
  operator bool() const { return (v <  g->nvertices); }
  self const& operator*() const { return *this;}
  self & operator++() { ++v; return *this;}
  
  grid_type const& TheGrid() const { return *g;}
  handle_type handle() const { return v;}
};


//----------------- VertexOnCellIterator ---------------------

class Triang2D_VertexOnCellIterator : public grid_types_base_Triang2D {
  typedef  Triang2D_VertexOnCellIterator self;
private:
  Cell c;
  int  vc;
public:
  Triang2D_VertexOnCellIterator() : vc(-1) {}
  Triang2D_VertexOnCellIterator(Cell const& cc) : c(cc), vc(0) {}
  
  self& operator++() { ++vc; return *this;}
  Vertex operator*() const { return Triang2D_Vertex(*(c.g), c.g->cells[3*c.c+vc]);}
  
  bool IsDone()   const { return (vc == 3);}
  operator bool() const { return (vc < 3);}

  vertex_handle handle() const { return c.g->cells[3*c.c+vc];}

  Cell const& TheCell() const { return c;}
  int LocalNumber()     const { return vc;}
};


//-------------- FacetOnCellIterator ---------------------

class Triang2D_FacetOnCellIterator : public grid_types_base_Triang2D {
  typedef  Triang2D_FacetOnCellIterator self;
private:
  Cell c;
  int  fc;
public:
  Triang2D_FacetOnCellIterator() : fc(-1) {}
  Triang2D_FacetOnCellIterator(Cell const& cc) : c(cc), fc(0) {}

  self&        operator++() { ++fc; return *this;}
  Facet        operator*()  const;
  
  bool IsDone()   const { return (fc == 3);}
  operator bool() const { return (fc < 3);}

  Vertex V1() const { return Vertex(*c.g, v1());}
  Vertex V2() const { return Vertex(*c.g, v2());}

  vertex_handle v1() const { return c.g->cells[3*c.c+ fc];}
  vertex_handle v2() const { return c.g->cells[3*c.c+ (fc+1)%3];}

  Cell const& TheCell() const { return c;}

  int LocalNumber() const { return fc;}
};

class Triang2D_Edge : public grid_types_base_Triang2D {
  typedef Triang2D_Edge  self;
private:
  Triang2D_FacetOnCellIterator fc;
public:
  Triang2D_Edge() {}
  Triang2D_Edge(Triang2D_FacetOnCellIterator ffc) : fc(ffc) {}

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
    vertex_handle max1(__STD::max(lhs.V1().handle(),lhs.V2().handle()));
    vertex_handle max2(__STD::max(rhs.V1().handle(),rhs.V2().handle()));
  
    return ( (max1 < max2)  
	     || 
	     ( (max1 == max2) && (  __STD::min(lhs.V1().handle(),lhs.V2().handle()) 
				    < __STD::min(rhs.V1().handle(),rhs.V2().handle()))));
  }

};


//------------------- inline functions ---------------

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


struct grid_types<Triang2D> : public grid_types_base_Triang2D 
{
  static vertex_handle handle(Vertex const& V) { return V.handle();}
  static cell_handle   handle(Cell   const& C) { return C.handle();}

  static int  local (FacetOnCellIterator  const& f) { return f.LocalNumber();}
  static int  local (VertexOnCellIterator const& v) { return v.LocalNumber();}

  static cell_handle invalid_cell_handle(Triang2D const&) { return -1;}
};

#include "Grids/Triang2D/partial-grid-functions.h"

class Triang2D_FacetIterator 
  : public facet_set_of_cells_iterator<Triang2D_Cell>
//    public grid_types_base_Triang2D
{
  typedef facet_set_of_cells_iterator<Triang2D_Cell> base;
 public: 
  Triang2D_FacetIterator() : base(CellIterator()) {}
  Triang2D_FacetIterator(CellIterator  const& c) : base(c) {}
  Triang2D_FacetIterator(grid_type const& g) : base(CellIterator(g)) {}
};


#endif
