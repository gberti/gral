#ifndef GRAL_GB_GRIDS_TRIANG3D_H
#define GRAL_GB_GRIDS_TRIANG3D_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Triang3D/grid-types.h"

#include "Gral/Iterators/generic-edge-iterators.h"
#include "Gral/Iterators/generic-facet-iterators.h"
#include "Gral/Iterators/vertex-on-edge-iterator.h"

#include <algorithm>

namespace GrAL {

struct grid_types_Triang3D
  :  public virtual
    generic_facet::grid_types_facet
      < generic_edge::grid_types_edge<
         grid_types_base_Triang3D>
   >
{
private:
  typedef  grid_types_Triang3D  self;
public:
  typedef facet_handle          face_handle;
  typedef Facet                 Face;
  typedef FacetIterator         FaceIterator;
  typedef FacetOnCellIterator   FaceOnCellIterator;
  typedef VertexOnFacetIterator VertexOnFaceIterator;
  typedef EdgeOnFacetIterator   EdgeOnFaceIterator;

  typedef vertex_on_edge_iterator<Triang3D,self> VertexOnEdgeIterator;
};


class  Triang3D_grid_base : public grid_types_Triang3D {};


#ifdef NMWR_DEBUG
#define DEBUG_ONLY(code)  if(true)  { code }
#else
#define DEBUG_ONLY(code)  if(false) { code }
#endif



/*! \brief A simple grid class for 3D triangulations (tetrahedrizations).
    \ingroup triang3dmodule 
 
   This class may act as a wrapper with reference-semantics for
 a plain array containing the cell-vertex connectivity.
 Value semantics may be enforced by a call to the member 
 function DoCopy().

 A copy by assignment or copy-ctor will always entail
 value-semantics on the copied-to side, i.e. make a full copy.

 \see Test in \ref test-triang3d-construct.C
 \todo Implement switch operators for vertex, edge, and facet.
*/


  class Triang3D : public grid_types_Triang3D::grid_base_type {
    typedef Triang3D self;
private:
  size_type* cells;
  bool owned;
  size_type  ncells;
  size_type  nvertices;
public:
    typedef self                        grid_type;
    typedef triangulation_category_d<3> category;

  //@{
  /*! \name Dimension information
      \brief Combinatorial dimension is 3
     
      Geometric dimension may be higher, 
      see \c stored_geometry_triang3d::space_dimension(). 
  */
  enum { dim = 3 /*< Combinatorial dimension */ };
  unsigned dimension() const { return dim;}
  //@}
    //@{
  /*! \name Construction 
   */
  //! \brief Empty grid
  Triang3D() 
    : cells(0), owned(false), ncells(0), nvertices(0) { sd.initialize(); }
  /*! \brief Initialize with reference semantic with respect to \c c

      The number of vertices will be calculated. 
      \pre
         - \c c is of size 4*nc, and <tt> c[4*n], c[4*n+1], c[4*n+2], c[4*n+3]</tt>
            are the vertex indices of the cell no. \c n. 
         -  The range of values in \c c is an interval \f$ [0,n_v[ \f$ for some  \f$n_v > 0\f$.
      \post 
         - <tt> NumOfCells() == nc </tt>
         - <tt> NumOfVertices() </tt> is the number \f$n_v\f$ of different vertices in \c c.
         - The data in \c is only referenced.

  */
  Triang3D(size_type* c, size_type nc) 
    : cells(c), owned(false), ncells(nc) 
  { sd.initialize(); nvertices = calc_num_of_vertices(); }

  /*! Initialize with reference semantic with respect to \c c

       \pre
         - \c c is of size 4*nc, and <tt> c[4*n], c[4*n+1], c[4*n+2], c[4*n+3]</tt>
            are the vertex indices of the cell no. \c n.  
         -  \c c[n] \In \c [0,nv-1] for 0 \Le \c n \Lt 4*nc  
       \post        
         - <tt> NumOfCells() == nc </tt>
         - <tt> NumOfVertices() == nv </tt>
         - The data in \c is only referenced.
   */
  Triang3D(size_type* c, size_type nc, size_type nv) 
    : cells(c), owned(false), ncells(nc), nvertices(nv) { sd.initialize(); } 

  /*! \brief  make physical copy of \c rhs
   */
  Triang3D(Triang3D const& rhs);
  /*! \brief  make physical copy of \c rhs
   */
  Triang3D& operator=(Triang3D const& rhs);
  //! Destructor
  ~Triang3D();

  /*! \brief  make a physical copy
    \post
      The storage pointing to the cells array is owned 
  */
  void DoCopy();
  /*! \brief Initialize using the connectivity given by c.
     
      Assume ownership of c \e without doing a copy.
      The typical example for this use is the following:
      \code
      { // beginning of block
        //  ...
        size_type * c = new size_type[4*nc];
	// ... fill c somehow ...
	Triang2D T;
	T.Steal(c,nc,nv);
        // DO NOT delete [] c in the sequel, it is owned by T
      } // end of block, destructor of T will delete c
      \endcode 
 
  */ 
  void Steal(size_type* c, size_type nc, size_type nv);
 
  /*! \brief Initialize by copying \c c

  */
  void Copy (size_type const* c, size_type nc, size_type nv, int offset = 0) {
    size_type * cc = new size_type[nc*4];
    for(size_type cell = 0; cell < 4*nc; ++cell)  
      cc[cell] = c[cell] - offset;
    Steal(cc, nc, nv);
    owned = true;
  }
  //@}
private:
  void clear(); 
  void do_copy();
  size_type  calc_num_of_vertices();

public:

  friend class Triang3D_VertexOnCellIterator;
  //friend class Triang3D_FacetOnCellIterator;
  friend class Triang3D_Cell;
  friend class Triang3D_Cell_base;
  friend class Triang3D_Vertex;

  //@{
  /*! \name Sequence iteration
      \todo STL-style EndXXX() 
  */
  size_type NumOfCells   () const { return ncells   ;}
  size_type NumOfVertices() const { return nvertices;}

  inline VertexIterator FirstVertex() const;
  inline VertexIterator EndVertex()   const;

  inline EdgeIterator   FirstEdge()   const;
  inline FaceIterator   FirstFace()   const;
  inline FacetIterator  FirstFacet()  const;

  inline CellIterator   FirstCell()   const;
  inline CellIterator   EndCell()     const;
  //@}

  //@{
  /*! \name Validity check
       \todo Implement for edge and facet handles
   */
  bool valid(vertex_handle v) const { return 0 <= v && v < nvertices;}
  bool valid(cell_handle   c) const { return 0 <= c && c < ncells;}
  bool valid_vertex(vertex_handle v) const { return 0 <= v && v < nvertices;}
  bool valid_cell  (cell_handle   c) const { return 0 <= c && c < ncells;}
  //@}

  // inline void switch_vertex(Vertex      & v, Edge const& e) const { return (v == e.V1() ? e.V2() : e.V1());}
  // inline void switch_edge  (Vertex const& v, Edge      & e, Face const& f) const;
  // inline void switch_face  (Edge   const& e, Face      & f, Cell const& c) const;
  // inline Vertex switched_vertex(Vertex const& v, Edge const& e) const;
  // inline Edge   switched_edge  (Vertex const& v, Edge const& e, Face const& c) const;
  // inline Face   switched_face  (Edge   const& e, Face const& f, Cell const& c) const;


  struct SD {
    typedef Triang3D::archetype_type archetype_type;

    archetype_type the_archetype[1];
    void initialize();
    SD();
  };
  static SD sd;
  
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
  /*@}*/


  // FIXME: ad hoc! ... job of archetype??
  void swap_orientation(Cell const& c);
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
  typedef cell_type_tag element_type_tag;
  struct category : grid_cell_category, grid_cell_iterator_category {};
  typedef grid_type anchor_type;
  typedef self      value_type;
public:
  Triang3D_Cell() {}
  explicit
  Triang3D_Cell(grid_type const& gg) { base::init(gg,0);}
  Triang3D_Cell(grid_type const& gg,
		cell_handle      cc) { base::init(gg,cc);}
  ~Triang3D_Cell() {}  
  
  self const& operator*() const { return *this;}
  self & operator++() { base::incr(); return *this;}

  anchor_type const& TheAnchor() const { return TheGrid();}

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
  typedef vertex_type_tag element_type_tag;
  struct category : grid_vertex_category, grid_vertex_iterator_category {};
  typedef self      value_type;
  typedef grid_type anchor_type;
private: 
  grid_type const* g;
  vertex_handle    v;
public:
  Triang3D_Vertex() : g(0), v(-1) {}
  explicit
  Triang3D_Vertex(grid_type const& gg) : g(&gg), v(0) {}
  Triang3D_Vertex(grid_type const& gg, size_type vv) : g(&gg), v(vv) {}
  ~Triang3D_Vertex() {}  
  
  bool IsDone()   const { cb(); return (v == g->nvertices); }
  self const& operator*() const { cv(); return *this;}
  self & operator++() { cv(); ++v; return *this;}
  
  grid_type const& TheGrid()   const { cb(); return *g;}
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

class Triang3D_VertexOnCellIterator : public grid_types_Triang3D {
  typedef  Triang3D_VertexOnCellIterator self;
private:
  Cell c;
  int  vc;
public:
  typedef grid_incidence_iterator_category_d<0,3> category;
  typedef Vertex value_type;
  typedef Cell   anchor_type;
public:
  Triang3D_VertexOnCellIterator() : vc(-1) {}
  explicit
  Triang3D_VertexOnCellIterator(Cell const& cc, int vcc = 0) : c(cc), vc(vcc) {}
  
  self& operator++() { cv();  ++vc; return *this;}
  Vertex operator*() const 
    { cv(); return Triang3D_Vertex(TheGrid(), handle());}
  bool IsDone()   const { cb(); return (vc == (int)c.NumOfVertices());}
  
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}
  Cell const& TheCell()   const { cb(); return c;}
  Cell const& TheAnchor() const { cb(); return c;}


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




#define gt grid_types<Triang3D>

  inline gt::VertexIterator   gral_begin(gt::grid_type const& G, gt::VertexIterator) { return G.FirstVertex();}
  inline gt::VertexIterator   gral_end  (gt::grid_type const& G, gt::VertexIterator) { return G.EndVertex();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::VertexIterator) { return G.NumOfVertices();}

  inline gt::EdgeIterator     gral_begin(gt::grid_type const& G, gt::EdgeIterator)   { return G.FirstEdge();}
  inline gt::EdgeIterator     gral_end  (gt::grid_type const& G, gt::EdgeIterator)   { return gt::EdgeIterator(G.EndCell());}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::EdgeIterator)   { return G.NumOfEdges();}

  inline gt::FacetIterator    gral_begin(gt::grid_type const& G, gt::FacetIterator)  { return G.FirstFacet();}
  inline gt::FacetIterator    gral_end  (gt::grid_type const& G, gt::FacetIterator)  { return gt::FacetIterator(G.EndCell());}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::FacetIterator)  { return G.NumOfFacets();}

  inline gt::CellIterator     gral_begin(gt::grid_type const& G, gt::CellIterator)   { return G.FirstCell();}
  inline gt::CellIterator     gral_end  (gt::grid_type const& G, gt::CellIterator)   { return G.EndCell();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::CellIterator)   { return G.NumOfCells();}


  inline gt::VertexOnCellIterator   gral_begin(gt::Cell   a, gt::VertexOnCellIterator)  { return a.FirstVertex();}
  inline gt::VertexOnCellIterator   gral_end  (gt::Cell   a, gt::VertexOnCellIterator)  { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::VertexOnCellIterator)  { return a.NumOfVertices();}

  inline gt::VertexOnEdgeIterator   gral_begin(gt::Edge   a, gt::VertexOnEdgeIterator)  { return gt::VertexOnEdgeIterator(a);}
  inline gt::VertexOnEdgeIterator   gral_end  (gt::Edge   a, gt::VertexOnEdgeIterator)  { return gt::VertexOnEdgeIterator(a,2);}
  inline gt::size_type              gral_size (gt::Edge   a, gt::VertexOnEdgeIterator)  { return 2;}

  inline gt::VertexOnFacetIterator  gral_begin(gt::Facet  a, gt::VertexOnFacetIterator) { return a.FirstVertex();}
  inline gt::VertexOnFacetIterator  gral_end  (gt::Facet  a, gt::VertexOnFacetIterator) { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Facet  a, gt::VertexOnFacetIterator) { return a.NumOfVertices();}

  inline gt::EdgeOnFacetIterator    gral_begin(gt::Facet  a, gt::EdgeOnFacetIterator) { return a.FirstEdge();}
  inline gt::EdgeOnFacetIterator    gral_end  (gt::Facet  a, gt::EdgeOnFacetIterator) { return a.EndEdge();}
  inline gt::size_type              gral_size (gt::Facet  a, gt::EdgeOnFacetIterator) { return a.NumOfEdges();}


  inline gt::EdgeOnCellIterator     gral_begin(gt::Cell   a, gt::EdgeOnCellIterator)  { return a.FirstEdge();}
  inline gt::EdgeOnCellIterator     gral_end  (gt::Cell   a, gt::EdgeOnCellIterator)  { return a.EndEdge();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::EdgeOnCellIterator)  { return a.NumOfEdges();}

  inline gt::FacetOnCellIterator    gral_begin(gt::Cell   a, gt::FacetOnCellIterator) { return a.FirstFacet();}
  inline gt::FacetOnCellIterator    gral_end  (gt::Cell   a, gt::FacetOnCellIterator) { return a.EndFacet();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::FacetOnCellIterator) { return a.NumOfFacets();}


#undef gt



//------------------- inline functions ---------------

inline
void Triang3D::swap_orientation(Triang3D::Cell const& c) { std::swap(cells[4*c.handle()], cells[4*c.handle()+1]);}


inline
Triang3D::VertexIterator
Triang3D::FirstVertex() const { return VertexIterator(*this);}

inline
Triang3D::VertexIterator
Triang3D::EndVertex() const { return VertexIterator(*this, NumOfVertices());}


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

inline
Triang3D::CellIterator
Triang3D::EndCell() const { return CellIterator(*this, NumOfCells());}

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

} // namespace GrAL 

#endif
