#ifndef GRAL_GB_GRIDS_TRIANG3D_H
#define GRAL_GB_GRIDS_TRIANG3D_H

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Triang3D/grid-types.h"

#include "Gral/Iterators/generic-edge-iterators.h"
#include "Gral/Iterators/generic-facet-iterators.h"
#include "Gral/Iterators/vertex-on-edge-iterator.h"

#include <algorithm>

struct grid_types_Triang3D
  :  public virtual
    generic_facet::grid_types_facet
      < generic_edge::grid_types_edge<
         grid_types_base_Triang3D>
   >
{
  typedef facet_handle          face_handle;
  typedef Facet                 Face;
  typedef FacetIterator         FaceIterator;
  typedef FacetOnCellIterator   FaceOnCellIterator;
  typedef VertexOnFacetIterator VertexOnFaceIterator;
  typedef EdgeOnFacetIterator   EdgeOnFaceIterator;

  typedef vertex_on_edge_iterator<Triang3D> VertexOnEdgeIterator;
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


class Triang3D : public grid_types_Triang3D {
private:
  int* cells;
  bool owned;
  int  ncells;
  int  nvertices;
public:
  enum { dim = 3};
  unsigned dimension() const { return dim;}

  Triang3D() 
    : cells(0), owned(false), ncells(0), nvertices(0) {}
  /*! Initialize with reference semantic with respect to \c c
      Number of vertices will be calculated. 
  */
  Triang3D(int* c, int nc) 
    : cells(c), owned(false), ncells(nc) 
  { nvertices = calc_num_of_vertices(); }

  /*! Initialize with reference semantic with respect to \c c
   */
  Triang3D(int* c, int nc, int nv) 
    : cells(c), owned(false), ncells(nc), nvertices(nv) {} 

  // make physical copy
  Triang3D(Triang3D const& rhs);
  Triang3D& operator=(Triang3D const& rhs);

  ~Triang3D();

  /*! make a physical copy, if only referencing the grid,
    thus gaining ownership of cells. */
  void DoCopy();
  /*! set this to the connectivity given by c.
    Assume ownership of c *without* doing a copy. 
    \code
      int * cells = new int[3*nc];
      // fill cells ...
      Triang3D T; 
      T.Steal(c, nc, nv);
      // T now owns the array c 
   \endcode 
  */
  void Steal(int* c, int nc, int nv);
private:
  void clear(); 
  void do_copy();
  int  calc_num_of_vertices();

public:

  friend class Triang3D_VertexOnCellIterator;
  //friend class Triang3D_FacetOnCellIterator;
  friend class Triang3D_Cell;
  friend class Triang3D_Cell_base;
  friend class Triang3D_Vertex;

  int NumOfCells   () const { return ncells   ;}
  int NumOfVertices() const { return nvertices;}
  //  int NumOfFaces()    const { return NumOfCells();}

  inline VertexIterator FirstVertex() const;
  inline EdgeIterator   FirstEdge()   const;
  inline FaceIterator   FirstFace()   const;
  inline FacetIterator  FirstFacet()  const;
  inline CellIterator   FirstCell()   const;

  bool valid(vertex_handle v) const { return 0 <= v && v < nvertices;}
  bool valid(cell_handle   c) const { return 0 <= c && c < ncells;}

  // inline void switch_vertex(Vertex      & v, Edge const& e) const { return (v == e.V1() ? e.V2() : e.V1());}
  // inline void switch_edge  (Vertex const& v, Edge      & e, Face const& f) const;
  // inline void switch_face  (Edge   const& e, Face      & f, Cell const& c) const;
  // inline Vertex switched_vertex(Vertex const& v, Edge const& e) const;
  // inline Edge   switched_edge  (Vertex const& v, Edge const& e, Face const& c) const;
  // inline Face   switched_face  (Edge   const& e, Face const& f, Cell const& c) const;


  struct SD {
    typedef Triang3D::archetype_type archetype_type;

    archetype_type the_archetype[1];
    SD();
  };
  static SD sd;
  
   /*! \name Archetype handling
   */
  /*@{*/
  static archetype_type const& Archetype(archetype_handle a) {
    REQUIRE(a == 0, "a = " << a,1);
    return *BeginArchetype();
  }
  static archetype_type   const& ArchetypeOf (Cell const&) { return *BeginArchetype();}
  static archetype_type   const& ArchetypeOf (cell_handle) { return *BeginArchetype();}
  static archetype_handle        archetype_of(cell_handle) { return 0;}
  static archetype_handle        archetype_of(Cell const&) { return 0;}

  static archetype_iterator BeginArchetype()
    { return archetype_iterator(sd.the_archetype);}
  static archetype_iterator EndArchetype()  { return BeginArchetype() +1;}
  static unsigned NumOfArchetypes() { return 1;}
  static archetype_handle handle(archetype_iterator a)
    { return a - BeginArchetype();}
  /*@}*/

};


//------------ Cell / CellIterator ----------------

class Triang3D_Cell_base : public grid_types_base_Triang3D {
public:
  typedef Triang3D grid_type;
private: 
  grid_type const* g;
  cell_handle      c;
public:
  Triang3D_Cell_base() : g(0), c(-1) {}
  explicit
  Triang3D_Cell_base(grid_type const& gg) : g(&gg), c(0) {}
  Triang3D_Cell_base(grid_type const& gg,
		     cell_handle      cc) : g(&gg), c(cc) {}
protected:
  void init(grid_type const& gg, cell_handle cc)
    { g = &gg; c = cc;}
public: 


  grid_type const& TheGrid() const { return *g;}
  cell_handle   handle() const { return c;}

  grid_type::archetype_type const& TheArchetype() const { return * g->BeginArchetype();}

  bool IsDone()   const { return (c == g->ncells); }
  void incr() { ++c;}

  vertex_handle v(int lv) const;
  Vertex        V(int lv) const;
  Vertex        V(archgt::Vertex) const;

  // checking functions
  bool bound() const { return g != 0;}
  bool valid() const { return (bound() &&  (0 <= c) && (c < g->ncells));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}
  // compatibility with generic_edge/facet
  void c_() const { cv();}
};


class Triang3D_Cell : public grid_types_Triang3D::cell_base_type {
  typedef Triang3D_Cell                       self;
  typedef grid_types_Triang3D::cell_base_type base;
  friend class     Triang3D_VertexOnCellIterator;
public:
  Triang3D_Cell() {}
  explicit
  Triang3D_Cell(grid_type const& gg) { base::init(gg,0);}
  Triang3D_Cell(grid_type const& gg,
		cell_handle      cc) { base::init(gg,cc);}
  ~Triang3D_Cell() {}  
  
  self const& operator*() const { return *this;}
  self & operator++() { base::incr(); return *this;}
  
  /*
  FacetOnCellIterator  FirstFacet () const;
  FaceOnCellIterator   FirstFace  () const;
  EdgeOnCellIterator   FirstEdge  () const;
  */
  VertexOnCellIterator FirstVertex() const;
  VertexOnCellIterator EndVertex  () const;
  typedef grid_types_Triang3D::FaceOnCellIterator FaceOnCellIterator;
  FaceOnCellIterator FirstFace() const { return FirstFacet();}

  unsigned NumOfVertices() const { return 4;}
  unsigned NumOfEdges()    const { return 6;}
  unsigned NumOfFacets()   const { return 4;}
  unsigned NumOfFaces ()   const { return 4;}

  friend bool operator==(self const& lhs, self const& rhs) { return lhs.handle() == rhs.handle();}
  friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs) { return lhs.handle() <  rhs.handle();}
};



//----------- Vertex / VertexIterator ----------------

class Triang3D_Vertex : public grid_types_Triang3D {
  typedef Triang3D_Vertex    self;
public:
  typedef self      value_type;
private: 
  grid_type const* g;
  vertex_handle    v;
public:
  Triang3D_Vertex() : g(0), v(-1) {}
  explicit
  Triang3D_Vertex(grid_type const& gg) : g(&gg), v(0) {}
  Triang3D_Vertex(grid_type const& gg, int vv) : g(&gg), v(vv) {}
  ~Triang3D_Vertex() {}  
  
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

  friend bool operator==(self const& lhs, self const& rhs) { return lhs.v == rhs.v;}
  friend bool operator!=(self const& lhs, self const& rhs) { return lhs.v != rhs.v;}
  friend bool operator< (self const& lhs, self const& rhs) { return lhs.v <  rhs.v;}
};


//----------------- VertexOnCellIterator ---------------------

class Triang3D_VertexOnCellIterator : public grid_types_Triang3D {
  typedef  Triang3D_VertexOnCellIterator self;
private:
  Cell c;
  int  vc;
public:
  Triang3D_VertexOnCellIterator() : vc(-1) {}
  explicit
  Triang3D_VertexOnCellIterator(Cell const& cc, int vcc = 0) : c(cc), vc(vcc) {}
  
  self& operator++() { cv();  ++vc; return *this;}
  Vertex operator*() const 
    { cv(); return Triang3D_Vertex(TheGrid(), handle());}
  bool IsDone()   const { cb(); return (vc == (int)c.NumOfVertices());}
  
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}
  Cell const& TheCell() const { cb(); return c;}


  vertex_handle handle() const 
    { cv(); return TheGrid().cells[c.NumOfVertices()*c.handle()+vc];}
  int local_handle()     const { cv(); return vc;}

  // checking functions
  bool bound() const { return c.valid();}
  bool valid() const { return (c.valid() &&  (0 <= vc) && (vc < (int)c.NumOfVertices()));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}

  friend bool operator==(self const& lhs, self const& rhs) { return lhs.vc == rhs.vc;}
  friend bool operator!=(self const& lhs, self const& rhs) { return lhs.vc != rhs.vc;}
};


template<>
struct grid_types<Triang3D> : public grid_types_base<grid_types_Triang3D>
{

  static vertex_handle handle(Vertex const& V) { return V.handle();}
  static cell_handle   handle(Cell   const& C) { return C.handle();}

  // static int  local (FacetOnCellIterator  const& f) { return f.local_handle();}
  static int  local (VertexOnCellIterator const& v) { return v.local_handle();}

  static cell_handle invalid_cell_handle(Triang3D const&) { return -1;}
};


//------------------- inline functions ---------------

inline
Triang3D::VertexIterator
Triang3D::FirstVertex() const { return VertexIterator(*this);}

inline
Triang3D::EdgeIterator
Triang3D::FirstEdge() const { return EdgeIterator(*this);}

inline
Triang3D::FacetIterator
Triang3D::FirstFacet() const { return FacetIterator(*this);}

inline
Triang3D::FacetIterator
Triang3D::FirstFace()  const { return FacetIterator(*this);}

inline
Triang3D::CellIterator
Triang3D::FirstCell() const { return CellIterator(*this);}

//inline 
//void Triang3D::switch_vertex(Triang3D::Vertex & v, Triang3D::Edge const& e) const
//{ v = (v == e.V1() ? e.V2() : e.V1());}

/*
inline 
void Triang2D::switch_edge(Triang2D::Vertex const& v, Triang2D::Edge & e, Triang2D::Cell const& c) const
{
  if(v == e.V1())
    e = Edge(FacetOnCellIterator(c, (e.fc.fc  == 0 ? 2 : e.fc.fc-1)));
  else
    e = Edge(FacetOnCellIterator(c, (e.fc.fc  == 2 ? 0 : e.fc.fc+1)));
}
*/

//inline 
//Triang3D::Vertex
//Triang3D::switched_vertex(Triang3D::Vertex const& v, Triang3D::Edge const& e) const
//{ Vertex v1(v); switch_vertex(v1, e); return v1;}

/*
inline 
Triang2D::Edge
Triang2D::switched_edge(Triang2D::Vertex const& v, Triang2D::Edge const& e, Triang2D::Cell const& c) const
{ Edge e1(e); switch_edge(v,e1,c); return e1; }
*/




inline
Triang3D::VertexOnCellIterator
Triang3D_Cell::FirstVertex() const { return VertexOnCellIterator(*this);}

inline
Triang3D::VertexOnCellIterator
Triang3D_Cell::EndVertex() const { return VertexOnCellIterator(*this, 4);}

/*
inline
Triang3D::EdgeOnCellIterator
Triang3D_Cell::FirstEdge() const { return  EdgeOnCellIterator(*this);}

inline
Triang3D::FacetOnCellIterator
Triang3D_Cell::FirstFacet() const { return FacetOnCellIterator(*this);}

inline
Triang3D::FaceOnCellIterator
Triang3D_Cell::FirstFace()  const { return FaceOnCellIterator(*this);}
*/



inline 
Triang3D::vertex_handle 
Triang3D_Cell_base::v(int lv) const { return vertex_handle(g->cells[(grid_type::dim+1)*c+lv]);}

inline 
Triang3D_Vertex
Triang3D_Cell_base::V(int lv) const { return Vertex(TheGrid(), v(lv));}

inline 
Triang3D_Vertex
Triang3D_Cell_base::V(Triang3D_Cell_base::archgt::Vertex lv) const { return Vertex(TheGrid(), v(lv.handle()));}

//inline
//Triang3D_Edge 
//Triang3D_FacetOnCellIterator::operator*() const
//{ return Triang3D_Edge(*this); }



#endif
