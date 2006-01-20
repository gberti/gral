#ifndef GRAL_GB_GRIDS_TRIANG2D_H
#define GRAL_GB_GRIDS_TRIANG2D_H

// $LICENSE

/*! \file
 */

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Triang2D/grid-types.h"

#include "Gral/Iterators/vertex-on-edge-iterator.h"

#include <algorithm>


namespace GrAL {

struct grid_types_Triang2D :
  public grid_types_base_Triang2D 
{
  typedef vertex_on_edge_iterator<Triang2D, grid_types_Triang2D>
  VertexOnEdgeIterator;
  typedef VertexOnEdgeIterator VertexOnFacetIterator;
};

#ifdef NMWR_DEBUG
#define DEBUG_ONLY(code)  if(true)  { code }
#else
#define DEBUG_ONLY(code)  if(false) { code }
#endif

/*! \brief A simple grid class for 2D triangulations.

   \ingroup triang2dmodule 

   Model of $GrAL VertexGridRange, $GrAL EdgeGridRange, $GrAL CellGridRange,
   $GrAL ArchetypedGrid.

   This class may act as a wrapper with reference-semantics for
   a plain array containing the cell-vertex connectivity.
   Value semantics may be enforced by a call to the member 
   function \c DoCopy().

   A copy by assignment or copy-constructor will always entail
   value-semantics on the copied-to side, i.e. make a full copy.

    As this class also serves as a tutorial on how to adapt
    existing data structures to GrAL (see also $GrAL AdaptingToInterface),
    the technical level was kept low, i.e. there are no complicated 
    constructions for building element and iterators types out of generic components.

   \see Test in \ref test-triang2d-construct.C
*/


class Triang2D : public grid_types_Triang2D {
private:
  size_type* cells;
  bool owned;
  size_type  ncells;
  size_type  nvertices;

  mutable size_type  nedges;
  mutable bool       nedges_valid;
public:

  enum { dim = 2};
  unsigned dimension() const { return dim;}

  //@{
  /*! \name Construction 
   */
  //! \brief Empty grid
  Triang2D() 
    : cells(0), owned(false), ncells(0), nvertices(0), nedges(0), nedges_valid(false) {}
  /*! \brief Construct using cell-vertex incidence information in \c c
      \pre
         - \c c is of size 3*nc, and <tt> c[3*n], c[3*n+1], c[3*n+2]</tt>
            are the vertex indices of the cell no. \c n. 
         -  The range of values in \c c is an interval \f$ [0,n_v[ \f$ for some  \f$n_v > 0\f$.
      \post 
         - <tt> NumOfCells() == nc </tt>
         - <tt> NumOfVertices() </tt> is the number \f$n_v\f$ of different vertices in \c c.
         - The data in \c c is only referenced.
   */
  Triang2D(size_type* c, size_type nc) 
    : cells(c), owned(false), ncells(nc), nedges(-1), nedges_valid(false) 
  { nvertices = calc_num_of_vertices(); }
  /*!  \brief Construct using cell-vertex incidence information in \c c

       \pre
         - \c c is of size 3*nc, and <tt> c[3*n], c[3*n+1], c[3*n+2]</tt>
            are the vertex indices of the cell no. \c n.  
         -  \c c[n] \In \c [0,nv-1] for 0 \Le \c n \Lt 3*nc  
       \post        
         - <tt> NumOfCells() == nc </tt>
         - <tt> NumOfVertices() == nv </tt>
         - The data in \c is only referenced.

   */
  Triang2D(size_type* c, size_type nc, size_type nv) 
    : cells(c), owned(false), ncells(nc), nvertices(nv), nedges(-1), nedges_valid(false) {} 

  /*! \brief  make physical copy of \c rhs
   */
  Triang2D(Triang2D const& rhs);
  /*! \brief  make physical copy of \c rhs
   */
  Triang2D& operator=(Triang2D const& rhs);

  ~Triang2D();
 

  /*! \brief  make a physical copy
    \post
      The storage pointing to the cells array is owned 
  */
  void DoCopy();
  /*! \brief Initialize using the connectivity given by c.
     
      Assume ownership of c \e without doing a copy.
      The typical example for this use is the following:
      \code
      {
       // beginning of block
        int * c = new int[3*nc];
	// ... fill c somehow ...
	Triang2D T;
	T.Steal(c,nc,nv);
        // DO NOT delete [] c in the sequel, it is owned by T
       } // end of block, destructor of T will delete c
       \endcode 
 
  */
  void Steal(size_type* c, size_type nc, size_type nv);

  /*! \brief Initialize by copying \c c
  
      This could be parameterized over the type of the cell vector \c c.
   */
  void Copy (size_type const* c, size_type nc, size_type nv, int offset = 0);


  /*! \brief Initialize using the connectivity given by c, 
       \e without assuming ownership.

   */
  void init (size_type* c, size_type nc, size_type nv) { Steal(c,nc,nv); owned = false; }
  //@}
private:
  void clear(); 
  void do_copy();
  size_type  calc_num_of_vertices();
  size_type  calc_num_of_edges() const;

  struct SD {
    typedef Triang2D::archetype_type      archetype_type;
    typedef Triang2D::archetype_geom_type archetype_geom_type;

    archetype_type      the_archetype[1];
    archetype_geom_type the_archetype_geom[1];
    SD();
  };
  static SD sd;

public:

  friend class Triang2D_VertexOnCellIterator;
  friend class Triang2D_FacetOnCellIterator;
  friend class Triang2D_Cell;
  friend class Triang2D_Vertex;

  /*! \name Sequence iteration
      \todo STL-style EndXXX() 
  */
  //@{
  size_type NumOfCells   () const { return ncells   ;}
  size_type NumOfVertices() const { return nvertices;}
  size_type NumOfFaces()    const { return NumOfCells();}
  size_type NumOfEdges()    const;

  inline VertexIterator FirstVertex() const;
  inline EdgeIterator   FirstEdge()   const;
  inline FaceIterator   FirstFace()   const;
  inline FacetIterator  FirstFacet()  const;
  inline CellIterator   FirstCell()   const;

  inline VertexIterator EndVertex() const;
  inline CellIterator   EndCell()   const;
  //@}

  //@{
  /*! \name Switch operator
      \note \c switch_cell(edge,cell) cannot be implemented without CellOnCellIterator.
   */
  inline void switch_vertex(Vertex      & v, Edge const& e) const;
  inline void switch_edge  (Vertex const& v, Edge      & e, Cell const& c) const;
  inline Vertex switched_vertex(Vertex const& v, Edge const& e) const;
  inline Edge   switched_edge  (Vertex const& v, Edge const& e, Cell const& c) const;
  //@}

  //@{
  /*! \name Validity check
       \todo Implement for edge handles
   */
  bool valid(vertex_handle v) const { return 0 <= v && v < nvertices;}
  bool valid(cell_handle   c) const { return 0 <= c && c < ncells;}
  //@}

  /*@{*/
   /*! \name Archetype handling
     
       \see $GrAL ArchetypedGrid concept
   */
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

  static archetype_geom_type const& ArchetypeGeom(archetype_handle   a) { return sd.the_archetype_geom[a];}
  // these are quite generic ...
  static archetype_geom_type const& ArchetypeGeom(archetype_iterator a) { return ArchetypeGeom(archetype_handle(a));}
  static archetype_geom_type const& ArchetypeGeom(Cell        const& c) { return ArchetypeGeom(archetype_of(c));}
  static archetype_geom_type const& ArchetypeGeom(cell_handle const& c) { return ArchetypeGeom(archetype_of(c));}
  /*@}*/

  // FIXME: ad hoc! ... job of archetype??
  void swap_orientation(Cell const& c);
};


//------------ Cell / CellIterator ----------------


class Triang2D_Cell : public grid_types_Triang2D  {
  typedef Triang2D_Cell     self;
  friend class     Triang2D_VertexOnCellIterator;
  friend class     Triang2D_FacetOnCellIterator;
private: 
  ref_ptr<grid_type const> g;
  cell_handle      c;
public:
  typedef grid_type anchor_type;
  typedef self      value_type;

  Triang2D_Cell() : g(0), c(-1) {}

  explicit
  Triang2D_Cell(grid_type const& gg,
		cell_handle      cc = 0) : g(gg), c(cc) {}
  explicit
  Triang2D_Cell(ref_ptr<grid_type const> gg,
		cell_handle      cc = 0) : g(gg), c(cc) {}

  ~Triang2D_Cell() {}  
  
  bool IsDone()   const { return (c == g->ncells); }
  self const& operator*() const { return *this;}
  self & operator++() { ++c; return *this;}
  
  grid_type const& TheGrid  () const { return *g;}
  grid_type const& TheAnchor() const { return *g;}
  cell_handle   handle() const { return c;}

  FacetOnCellIterator  FirstFacet () const;
  EdgeOnCellIterator   FirstEdge  () const;
  VertexOnCellIterator FirstVertex() const;
  VertexOnCellIterator EndVertex  () const;

  unsigned NumOfVertices() const { return 3;}
  unsigned NumOfEdges()    const { return 3;}
  unsigned NumOfFacets()   const { return 3;}

  vertex_handle v(int lv) const;
  Vertex        V(int lv) const;

  archetype_type const& TheArchetype() const { return TheGrid().ArchetypeOf(*this);}

  // checking functions
  bool bound() const { return g != 0;}
  bool valid() const { return (bound() &&  (0 <= c) && (c < g->ncells));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}

  friend bool operator==(self const& lhs, self const& rhs) { return lhs.c == rhs.c;}
  friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs) { return lhs.c <  rhs.c;}
};



//----------- Vertex / VertexIterator ----------------

class Triang2D_Vertex : public grid_types_Triang2D {
  typedef Triang2D_Vertex    self;
public:
  typedef self      value_type;
  typedef grid_type anchor_type;
private: 
  ref_ptr<grid_type const> g;
  vertex_handle            v;
public:
  Triang2D_Vertex() : g(0), v(-1) {}
  explicit
  Triang2D_Vertex(ref_ptr<grid_type const> gg, int vv = 0) : g(gg), v(vv) {}
  explicit
  Triang2D_Vertex(grid_type const&         gg, int vv = 0) : g(gg), v(vv) {}
  ~Triang2D_Vertex() {}  
  
  bool IsDone()   const { cb(); return (v == g->nvertices); }
  self const& operator*() const { cv(); return *this;}
  self & operator++() { cv(); ++v; return *this;}
  
  grid_type const& TheGrid  () const { cb(); return *g;}
  grid_type const& TheAnchor() const { cb(); return *g;}
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

class Triang2D_VertexOnCellIterator : public grid_types_Triang2D {
  typedef  Triang2D_VertexOnCellIterator self;
private:
  Cell c;
  int  vc;
public:
  typedef Vertex value_type;
  typedef Cell   anchor_type;

  Triang2D_VertexOnCellIterator() : vc(-1) {}
  explicit
  Triang2D_VertexOnCellIterator(Cell const& cc, int vvc = 0) : c(cc), vc(vvc) {}
  
  self& operator++() { cv();  ++vc; return *this;}
  Vertex operator*() const 
    { cv(); return Triang2D_Vertex(TheGrid(), handle());}
  bool IsDone()   const { cb(); return (vc == 3);}
  
  grid_type const& TheGrid  () const { cb(); return c.TheGrid();}
  Cell      const& TheAnchor() const { cb(); return c;}
  Cell      const& TheCell  () const { cb(); return c;}


  vertex_handle handle() const 
    { cv(); return TheGrid().cells[3*c.handle()+vc];}
  int local_handle()     const { cv(); return vc;}

  // checking functions
  bool bound() const { return c.valid();}
  bool valid() const { return (c.valid() &&  (0 <= vc) && (vc < 3));}
  void cb()    const { REQUIRE (bound(), "",1);}
  void cv()    const { REQUIRE (valid(), "",1);}

  friend bool operator==(self const& lhs, self const& rhs) { return lhs.vc == rhs.vc;}
  friend bool operator!=(self const& lhs, self const& rhs) { return lhs.vc != rhs.vc;}
  friend bool operator< (self const& lhs, self const& rhs) { return lhs.vc <  rhs.vc;}
};


//-------------- FacetOnCellIterator ---------------------

struct edge_handle_Triang2D : public grid_types_Triang2D {
  cell_handle c;
  int         lf;
  edge_handle_Triang2D() { DEBUG_ONLY( c=-1; lf = -1; ); }
  edge_handle_Triang2D(cell_handle cc, int llf) : c(cc), lf(llf) {}
};


inline
std::ostream& operator<<(std::ostream& out, edge_handle_Triang2D e)
{ return (out << e.c << ' ' << e.lf);} 

class Triang2D_FacetOnCellIterator : public grid_types_Triang2D {
  typedef  Triang2D_FacetOnCellIterator self;
  friend class Triang2D;
private:
  Cell c;
  int  fc;
public:
  typedef Facet value_type;
  typedef Cell  anchor_type;

  Triang2D_FacetOnCellIterator() : fc(-1) {}
  explicit
  Triang2D_FacetOnCellIterator(Cell const& cc, int ffc = 0) 
    : c(cc), fc(ffc) 
  {}
  //    { REQUIRE( valid(), "invalid iterator!",1); }

  self&        operator++() { ++fc; return *this;}
  Facet        operator*()  const;
  bool IsDone()   const { cb(); return (fc == 3);}

  Vertex V1() const { cv(); return Vertex(*c.g, v1());}
  Vertex V2() const { cv(); return Vertex(*c.g, v2());}
  vertex_handle v1() const { cv(); return TheGrid().cells[3*c.c+ fc];}
  vertex_handle v2() const { cv(); return TheGrid().cells[3*c.c+ (fc+1)%3];}

  Cell      const& TheCell()   const { cb(); return c;}
  Cell      const& TheAnchor() const { cb(); return c;}
  grid_type const& TheGrid()   const { cb(); return c.TheGrid();}

  edge_handle  handle()     const 
    { cv(); return edge_handle(c.handle(), fc);} 
  int local_handle() const { cv(); return fc;}
  
  // checking functions
  archgt::Cell ArchetypeCell() const { return archgt::Cell(TheCell().TheArchetype(), fc);}

  bool bound() const { return c.bound();}
  bool valid() const { return (c.valid() && (0 <= fc) && (fc < 3));}
  void cv() const { REQUIRE(valid(), "", 1);}
  void cb() const { REQUIRE(bound(), "", 1);}

  friend bool operator==(self const& lhs, self const& rhs) { return lhs.c == rhs.c && lhs.fc == rhs.fc;}
  friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
};



class Triang2D_Edge : public grid_types_Triang2D {
  typedef Triang2D_Edge  self;
  friend class Triang2D;
private:
  Triang2D_FacetOnCellIterator fc;
public:
  Triang2D_Edge() {}
  explicit
  Triang2D_Edge(Triang2D_FacetOnCellIterator ffc) : fc(ffc) {}
  explicit
  Triang2D_Edge(grid_type const& g) 
    : fc(Cell(g,cell_handle(0)), 0) {}
  explicit 
  Triang2D_Edge(ref_ptr<grid_type const> g) 
    : fc(Cell(g,cell_handle(0)), 0) {}

  Triang2D_Edge(grid_type const& g, edge_handle e) 
    : fc(Cell(g,e.c), e.lf) {}
  Triang2D_Edge(ref_ptr<grid_type const> g, edge_handle e) 
    : fc(Cell(g,e.c), e.lf) {}

  edge_handle handle() const { return fc.handle();}

  unsigned NumOfVertices() const { return 2;}
  VertexOnEdgeIterator FirstVertex() const { return VertexOnEdgeIterator(*this);}
  vertex_handle v1() const { return fc.v1();}
  vertex_handle v2() const { return fc.v2();}
  Vertex V1() const { return fc.V1();}
  Vertex V2() const { return fc.V2();}

  grid_type const& TheGrid  () const { return fc.TheGrid();}
  grid_type const& TheAnchor() const { return fc.TheGrid();}
  Cell             TheCell  () const { return fc.TheCell();}

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
  friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}

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



template<>
struct grid_types<Triang2D> : public grid_types_base<grid_types_Triang2D>
{

  static vertex_handle handle(Vertex const& V) { return V.handle();}
  static cell_handle   handle(Cell   const& C) { return C.handle();}

  static int  local (FacetOnCellIterator  const& f) { return f.local_handle();}
  static int  local (VertexOnCellIterator const& v) { return v.local_handle();}

  static cell_handle invalid_cell_handle(Triang2D const&) { return -1;}
};

} // namespace GrAL

#include "Gral/Grids/Triang2D/partial-grid-functions.h"

namespace GrAL {

//------------------- inline functions ---------------

inline
void Triang2D::swap_orientation(Triang2D::Cell const& c) { std::swap(cells[3*c.handle()], cells[3*c.handle()+1]);}


Triang2D::size_type
inline Triang2D::NumOfEdges()    const { 
  if(!nedges_valid) {
    calc_num_of_edges();
    nedges_valid = true;
  }
  return nedges;
}

inline
Triang2D::VertexIterator
Triang2D::FirstVertex() const { return VertexIterator(*this);}
inline
Triang2D::VertexIterator
Triang2D::EndVertex()  const { return VertexIterator(*this, NumOfVertices());}

inline
Triang2D::EdgeIterator
Triang2D::FirstEdge() const { return EdgeIterator(*this);}

inline
Triang2D::FacetIterator
Triang2D::FirstFacet() const { return FacetIterator(*this);}


inline 
void Triang2D::switch_vertex(Triang2D::Vertex & v, Triang2D::Edge const& e) const
{ v = (v == e.V1() ? e.V2() : e.V1());}


inline 
void Triang2D::switch_edge(Triang2D::Vertex const& v, Triang2D::Edge & e, Triang2D::Cell const& c) const
{
  if(v == e.V1())
    e = Edge(FacetOnCellIterator(c, (e.fc.fc  == 0 ? 2 : e.fc.fc-1)));
  else
    e = Edge(FacetOnCellIterator(c, (e.fc.fc  == 2 ? 0 : e.fc.fc+1)));
}


inline 
Triang2D::Vertex
Triang2D::switched_vertex(Triang2D::Vertex const& v, Triang2D::Edge const& e) const
{ Vertex v1(v); switch_vertex(v1, e); return v1;}

inline 
Triang2D::Edge
Triang2D::switched_edge(Triang2D::Vertex const& v, Triang2D::Edge const& e, Triang2D::Cell const& c) const
{ Edge e1(e); switch_edge(v,e1,c); return e1; }


inline
Triang2D::FaceIterator
Triang2D::FirstFace() const { return FaceIterator(*this);}

inline
Triang2D::CellIterator
Triang2D::FirstCell() const { return CellIterator(*this);}

inline
Triang2D::CellIterator
Triang2D::EndCell() const { return CellIterator(*this,NumOfCells());}


inline
Triang2D_VertexOnCellIterator
Triang2D_Cell::FirstVertex() const { return VertexOnCellIterator(*this);}

inline
Triang2D_VertexOnCellIterator
Triang2D_Cell::EndVertex()  const { return VertexOnCellIterator(*this, NumOfVertices());}


inline
Triang2D_FacetOnCellIterator
Triang2D_Cell::FirstFacet() const { return FacetOnCellIterator(*this);}

inline
Triang2D_FacetOnCellIterator
Triang2D_Cell::FirstEdge() const { return FacetOnCellIterator(*this);}


inline 
Triang2D::vertex_handle 
Triang2D_Cell::v(int lv) const { return vertex_handle(g->cells[3*c+lv]);}

inline 
Triang2D_Vertex
Triang2D_Cell::V(int lv) const { return Vertex(TheGrid(), v(lv));}

inline
Triang2D_Edge 
Triang2D_FacetOnCellIterator::operator*() const
{ return Triang2D_Edge(*this); }


#define gt grid_types<Triang2D>

  inline gt::VertexIterator   gral_begin(gt::grid_type const& G, gt::VertexIterator) { return G.FirstVertex();}
  inline gt::VertexIterator   gral_end  (gt::grid_type const& G, gt::VertexIterator) { return G.EndVertex();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::VertexIterator) { return G.NumOfVertices();}

  inline gt::EdgeIterator     gral_begin(gt::grid_type const& G, gt::EdgeIterator)   { return G.FirstEdge();}
  inline gt::EdgeIterator     gral_end  (gt::grid_type const& G, gt::EdgeIterator)   { return gt::EdgeIterator(G.EndCell());}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::EdgeIterator)   { return G.NumOfEdges();}

  inline gt::CellIterator     gral_begin(gt::grid_type const& G, gt::CellIterator)   { return G.FirstCell();}
  inline gt::CellIterator     gral_end  (gt::grid_type const& G, gt::CellIterator)   { return G.EndCell();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::CellIterator)   { return G.NumOfCells();}


  inline gt::VertexOnCellIterator   gral_begin(gt::Cell   a, gt::VertexOnCellIterator) { return a.FirstVertex();}
  inline gt::VertexOnCellIterator   gral_end  (gt::Cell   a, gt::VertexOnCellIterator) { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::VertexOnCellIterator) { return a.NumOfVertices();}

  inline gt::VertexOnEdgeIterator   gral_begin(gt::Edge   a, gt::VertexOnEdgeIterator) { return a.FirstVertex();}
  inline gt::VertexOnEdgeIterator   gral_end  (gt::Edge   a, gt::VertexOnEdgeIterator) { return gt::VertexOnEdgeIterator(a,2);}
  inline gt::size_type              gral_size (gt::Edge   a, gt::VertexOnEdgeIterator) { return a.NumOfVertices();}

  inline gt::EdgeOnCellIterator     gral_begin(gt::Cell   a, gt::EdgeOnCellIterator) { return a.FirstEdge();}
  inline gt::EdgeOnCellIterator     gral_end  (gt::Cell   a, gt::EdgeOnCellIterator) { return gt::EdgeOnCellIterator(a,3);}
  inline gt::size_type              gral_size (gt::Cell   a, gt::EdgeOnCellIterator) { return a.NumOfEdges();}


#undef gt

} // namespace GrAL 

#endif
