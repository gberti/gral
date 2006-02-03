#ifndef NMWR_GB_GRIDS_COMPLEX2D_H
#define NMWR_GB_GRIDS_COMPLEX2D_H



// $LICENSE

/*! \file

*/

#include <list>
#include <vector> 

#include "Utility/pre-post-conditions.h"
#include "Gral/Grids/Complex2D/point.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-handle.h"

#include "Gral/Base/polygon.h"

// many, many forward declarations ...

namespace GrAL {

class Complex2D;
class Vertex2D;
class Edge2D;
class Cell2D;
 
class Vertex2D_Iterator;
class Edge2D_Iterator;
class Cell2D_Iterator;
class BoundaryFacet2D_Iterator;
class BoundaryComponent_Vertex2D_Iterator;

class VertexOnEdge2D_Iterator;
class CellOnEdge2D_Iterator;

class VertexOnCell2D_Iterator;
class EdgeOnCell2D_Iterator;
class CellOnCell2D_Iterator;


class CellOnVertex2D_Iterator;
struct edge_handle_complex2d;


  struct complex2d_types : public grid_types_detail::grid_types_root { 
    typedef complex2d_types self;
  typedef Complex2D Complex;
  typedef Complex2D grid_type;
  typedef const Complex2D* ComplexPtr;

  typedef Vertex2D  Vertex;
  typedef Edge2D    Edge;
  typedef Edge2D    Facet;
  typedef Cell2D    Cell;
  typedef Cell2D    Face;


    typedef vertex_handle_int<Complex2D,self> vertex_handle;
    typedef cell_handle_int<Complex2D,self>   cell_handle;
    typedef edge_handle_complex2d             edge_handle;
    typedef edge_handle_complex2d             facet_handle;

  // sequence iterators
  typedef Vertex2D_Iterator       GridVertexIterator;
  typedef Edge2D_Iterator         GridEdgeIterator;
  typedef Edge2D_Iterator         GridFacetIterator;
  typedef Cell2D_Iterator         GridCellIterator;

  typedef VertexOnEdge2D_Iterator VertexOnEdgeIterator;
  typedef VertexOnEdge2D_Iterator VertexOnFacetIterator;
  typedef CellOnEdge2D_Iterator   CellOnEdgeIterator;
  typedef CellOnEdge2D_Iterator   CellOnFacetIterator;


  // X-on-Cell incidence
  typedef VertexOnCell2D_Iterator VertexOnCellIterator;
  typedef EdgeOnCell2D_Iterator   EdgeOnCellIterator;
  typedef EdgeOnCell2D_Iterator   FacetOnCellIterator;
  typedef CellOnCell2D_Iterator   CellNeighbourIterator;
  typedef CellOnCell2D_Iterator   CellOnCellIterator;

  typedef CellOnVertex2D_Iterator CellOnVertexIterator;

  typedef polygon1d::polygon                 archetype_type;
  typedef std::vector<archetype_type>        archetype_sequence;
  typedef archetype_sequence::const_iterator archetype_iterator;
  typedef int                                archetype_handle;
  typedef grid_types<archetype_type>         archgt;

};

// the next two classes (vertex_base and cell_connectivity) are
// the *internal* representations of vertices and cells and are
// *not* intended for public use



class vertex_base {
public:
  typedef point2 CoordType;
  typedef complex2d_types::size_type size_type;

  typedef std::vector<size_type> cell_list;
private:

  //--- DATA ------
  CoordType _coord;       // this could be moved to external geometry.
  cell_list _cells;       // information about adjacent cells.
                          // no ordering of the cells is assumed, albeit possible in 2D.
  void reserve(unsigned n) { _cells.reserve(n);}

  typedef vertex_base self;
public:
  //-------- constructors ----------------------
  vertex_base() : _coord(0.0), _cells() {}
  vertex_base(unsigned n) : _coord(0.0), _cells() { reserve(n);} // NumOfCells() == 0
  vertex_base(const CoordType& coord)                 // NumOfCells() == 0
    : _coord(coord), _cells() {}
 
  // copying 
  vertex_base(const vertex_base& rs) : _coord(rs._coord), _cells(rs._cells) {}
  vertex_base& operator=(const vertex_base& rs) { 
    if (this != &rs) { 
      _coord = rs._coord; 
      _cells = rs._cells;
    }
    return *this;
  }

private:
  friend class Complex2D;
  friend class Vertex2D;
};



class cell2d_connectivity {
  typedef complex2d_types::size_type size_type;
public:
  typedef std::vector<size_type> vertex_list;
  typedef std::vector<size_type> cell_list;

  //private:
  //-------------- DATA -----------
  // the assumption is that the 2 vertices corresponding to
  // the edge separating (*this) from nb_i (at postion i in _neighbours)
  // are at pos. i and i+1 (mod Length(_vertices)) in _vertices:
  // v1 --------- v2 ---------- v3 --- ... --- vn [ -------- v1 ]
  //      nb1            nb2         ...              nb_n
  cell_list      _neighbours;
  vertex_list    _vertices; 

  void resize(size_type n) {
    _neighbours = cell_list(n);
    _vertices   = vertex_list(n);
  }


  typedef cell2d_connectivity self;
public:
  //------------ constructors -----------------------
  cell2d_connectivity()  {}
  cell2d_connectivity(size_type n) : _neighbours(n), _vertices(n) {} // NumOfVertices() == n
  ~cell2d_connectivity() {}

  // copying
  cell2d_connectivity(const self& rs) 
    : _neighbours(rs._neighbours), _vertices(rs._vertices) {}
  self& operator=(const self& rs) {
    if(this != &rs) {
      _neighbours = rs._neighbours;
      _vertices   = rs._vertices;
    }
   return (*this);
  }
  
private:
  friend class friend_for_input; // work around, see below.
  friend class EdgeOnCell2D_Iterator;
  friend class Complex2D;
  friend class Cell2D;
};




typedef std::vector<cell2d_connectivity>   cell_list_complex2d;
typedef std::vector<vertex_base>           vertex_list_complex2d;

struct edge_handle_complex2d {
  typedef complex2d_types::cell_handle cell_handle;
  cell_handle c;
  // int c;  // cell
  int le; // local position in cell
  edge_handle_complex2d() {}
  edge_handle_complex2d(cell_handle cc, int lle) : c(cc), le(lle) {}

  int local_facet() const { return le;}

  typedef edge_handle_complex2d self;
  friend bool operator==(const self& ls, const self& rs)
    { return ((ls.c == rs.c) && (ls.le == rs.le)); }
  friend bool operator!=(const self& ls, const self& rs)
    { return !(ls == rs);}
  friend bool operator< (const self& ls, const self& rs)
    { return ((ls.c <rs.c)  || ((ls.c == rs.c) &&  (ls.le < rs.le))); }

  friend ::std::ostream& operator<< (::std::ostream& out, const self& e)
    { return (out << e.c << ' ' << e.le);}
  friend ::std::istream& operator>>(::std::istream& in,         self& e)
    { return (in >> e.c >> e.le);}
};

} // namespace GrAL

namespace STDEXT  {
  template<class T> class hash;

  template<>
  struct hash<GrAL::edge_handle_complex2d> {
  public:
    typedef GrAL::edge_handle_complex2d key_type;
    typedef GrAL::edge_handle_complex2d argument_type;
    typedef size_t                result_type;
    
    size_t operator()(const GrAL::edge_handle_complex2d& e) const
      { return (6*e.c + e.le);}
  };
  
} // namespace STDEXT

namespace GrAL {





#include "Gral/Grids/Complex2D/internal/vertex2d.h"
#include "Gral/Grids/Complex2D/internal/cell2d.h"
#include "Gral/Grids/Complex2D/internal/cell-on-cell2d-it.h"
#include "Gral/Grids/Complex2D/internal/cell-on-vertex2d-it.h"
#include "Gral/Grids/Complex2D/internal/edge-on-cell2d-it.h"

/*! \brief General 2D grid class
     \ingroup complex2dmodule

    An object of type Complex2D can represent a general two-dimensional
    grid of homogeneous dimension. 
    The cells can be arbitrary simple polygons. 
    The vertex sets of facets must be unique, else the facet-cell incidence
    relation will not be calculated correctly.
       

    Complex2D is a model of 
    $GrAL Grid-With-Boundary,
    $GrAL ArchetypedGrid,
    $GrAL  VertexRange, $GrAL EdgeRange,
    $GrAL  FacetRange, and $GrAL CellRange.

    Its $GrAL Cell type (Cell2D) is a model of $GrAL VertexRange,
    $GrAL EdgeRange, and $GrAL CellRange. 
    That is, there are types for the $GrAL GridIncidenceIterator's 
    \c VertexOnCellIterator (model of $GrAL VertexOnCellIterator),
    \c EdgeOnCellIterator (model of $GrAL EdgeOnCellIterator),
    and \c CellOnCellIterator (model of $GrAL CellOnCellIterator).

    Its $GrAL Vertex type (Vertex2D) is a model of 
    $GrAL CellRange.
    That is, there is a type \c CellOnVertexIterator 
    (model of $GrAL CellOnVertexIterator).

    Its $GrAL Edge type (Edge2D) is a model of 
    $GrAL VertexRange and $GrAL CellRange.
    That is, there are types \c VertexOnEdgeIterator
    and \c CellOnEdgeIterator.

    Its $GrAL Facet type is a typedef to its $GrAL Edge type.
 */
class Complex2D : public complex2d_types  {
  typedef Complex2D    self;

  typedef std::list<EdgeOnCell2D_Iterator>  boundary_facet_list;
  typedef vertex_list_complex2d        v_list;
  typedef cell_list_complex2d          c_list;


  //----- DATA -------------------------------------
public: // for benchmark only
  cell_list_complex2d            _cells;
  vertex_list_complex2d          _vertices;
  boundary_facet_list            _boundary;

  mutable size_type              num_of_edges_cache;
  archetype_sequence             archetypes;
  std::vector<archetype_handle>  arch_for_n_vertices;
public:
  typedef self               grid_type;
  typedef grid_category_d<2> category;

  //--------- types --------------------

  // sequence iterators
  typedef Vertex2D_Iterator       VertexIterator;
  typedef Edge2D_Iterator         EdgeIterator;
  typedef Edge2D_Iterator         FacetIterator;
  typedef Cell2D_Iterator         CellIterator;
  typedef Cell2D_Iterator         FaceIterator;

  typedef BoundaryFacet2D_Iterator            BoundaryFacetIterator;
  typedef BoundaryComponent_Vertex2D_Iterator BoundaryVertexIterator;

  typedef vertex_base::CoordType    CoordType;
 
  //--------------- constructors --------------
  //@{ @name Constructors 
  Complex2D();
  Complex2D(const Complex2D& rhs);
  Complex2D& operator=(const Complex2D& rhs);
  ~Complex2D();
  //@}
private:
  void clear();
public:
  //-------------- iteration ------------------

  //@{ @name Dimension information
  enum { dim = 2};
  unsigned dimension() const { return 2;}
  //@}

  //@{ @name STL half-open ranges
  inline VertexIterator   FirstVertex() const;
  inline VertexIterator   EndVertex()   const;

  inline EdgeIterator     FirstEdge()    const;
  inline EdgeIterator     EndEdge()      const;

  inline FacetIterator    FirstFacet()   const;
  inline FacetIterator    EndFacet()     const;
 
  inline CellIterator     FirstCell()    const;
  inline CellIterator     EndCell()      const;

  inline FaceIterator     FirstFace()    const;
  inline FaceIterator     EndFace()      const;
 //@}

  //@{ @name switch operations
  inline void switch_vertex(Vertex& v, Edge const& e) const;
  inline void switch_edge(Vertex const& v, Edge & e, Cell const& c) const;
  inline void switch_facet(Vertex const& v, Edge & e, Cell const& c) const
    { switch_edge(v,e,c);}
  inline void switch_cell(Edge const& e, Cell & c) const;

  inline Vertex switched_vertex(Vertex const& v, Edge const& e) const;
  inline Edge   switched_edge  (Vertex const& v, Edge const& e, Cell const& c) const;
  inline Facet  switched_facet (Vertex const& v, Edge const& e, Cell const& c) const;
  inline Cell   switched_cell  (Edge   const& e, Cell const& c) const;
  //@}


  //@{ @name Boundary iteration
  // boundary iteration -- still somewhat incomplete.
  // there could be access to the 1D boundary grid as a whole,
  // and to its connected components as entities of their own.
  inline BoundaryFacetIterator  FirstBoundaryFacet() const;
  inline BoundaryFacetIterator  FirstBoundaryEdge () const;
  inline BoundaryVertexIterator FirstBoundaryVertex() const; // works only for one bd. component!

  size_type NumOfBoundaryVertices() const { return NumOfBoundaryFacets();}
  size_type NumOfBoundaryFacets() const { return (_boundary.size());}

  // now here some work has still to be done ...
  // int NumOfBoundaryCells() const 
  //   { return NumOfBoundaryFacets() - # cells w. multiple bd}

  // FIXME: only correct for genus = genus(S^2) / genus [0,1]^2
  //  int NumOfBoundaryComponents() const { return (NumOfBoundaryFacets() > 0 ? 1 : 0);} 
  //@}

  //---------------- size information ------------

  //@{ @name Size information
  size_type NumOfVertices() const  {return (_vertices.size());}
  size_type NumOfEdges()    const  
  { 
    if(num_of_edges_cache < 0)
      calculate_num_of_edges();
    return num_of_edges_cache;
  }
  //  {return (NumOfCells() == 0 ? 0 : -2 + NumOfVertices() + NumOfCells() + NumOfBoundaryComponents());}
  size_type NumOfFacets()   const { return NumOfEdges();}
  size_type NumOfFaces()    const  {return NumOfCells();}
  size_type NumOfCells()    const  {return (_cells.size());}
  //@}
  
  //-----------------------------------------------------
  //@{ @name Boundary predicates
  //! invalid cell_handle for marking "outside"
  cell_handle outer_cell_handle() const { return cell_handle(-1);}

  //  inline bool IsOnBoundary(CellNeighbourIterator const& nb) const;
  inline bool IsOnBoundary(EdgeOnCellIterator    const& e ) const;
  inline bool IsOnBoundary(Edge                  const& e ) const;

  bool IsInside(cell_handle c) const { return (c != outer_cell_handle());}
  bool IsInside(const Cell& c) const { return IsInside(c.handle());}
  //@}

  /** @name geometry 
      \todo: Remove this functionality (It's the job of geometry classes).
   */
  //@{ 
  inline const CoordType& Coord(const Vertex& v)   const;
  inline       CoordType& Coord(const Vertex& v);
  //@}

  /*! \name Archetype handling
   */
  /*@{*/
  archetype_iterator BeginArchetype() const { return archetypes.begin();}
  archetype_iterator EndArchetype()   const { return archetypes.end();}
  archetype_handle   handle(archetype_iterator it) const { return it - BeginArchetype();}

  archetype_type const& Archetype(archetype_handle a) const { return archetypes[a];}
  archetype_type      & Archetype(archetype_handle a)       { return archetypes[a];}
  archetype_type const& ArchetypeOf (Cell const& c) const 
  { return Archetype(archetype_of(c));}
  archetype_type   const& ArchetypeOf (cell_handle c) const 
    { return ArchetypeOf(cell(c));}
  archetype_handle        archetype_of(cell_handle c) const 
    { return archetype_of(cell(c)); }
  archetype_handle        archetype_of(Cell const& c) const 
    { return arch_for_n_vertices[c.NumOfVertices()];}
  unsigned NumOfArchetypes() const { return archetypes.size(); }


  /*! \brief Add an archetype to the archetype list.
 
     If there is already an archetype with nv vertices, the handle of the old
     archetype is returned. Else the handle of the newly added archetype is returned.
   */
  archetype_handle add_archetype(archetype_type const& A, int nv);
  /*@}*/
  
private:
  friend class friend_for_input; // work-around class
  friend class Vertex2D;
  friend class Edge2D;
  friend class Cell2D;

  friend class VertexOnCell2D_Iterator;
  friend class EdgeOnCell2D_Iterator;
  friend class BoundaryFacet2D_Iterator; 
  friend class CellOnCell2D_Iterator;
  friend class CellOnVertex2D_Iterator;

  friend class Vertex2D_Iterator;
  friend class Edge2D_Iterator;
  friend class Cell2D_Iterator;
  
   

  inline cell_handle   _new_cell(int num_of_v);
  inline vertex_handle _new_vertex(const CoordType& coo = CoordType());

  // make n1_it point to n2
  inline void set_neighbour(const FacetOnCellIterator& n1_it, const Cell& n2);
  inline void add_cell_on_vertex(const Vertex& V, const Cell& C);
  inline void add_cell_on_vertex(const Vertex& V, const cell_handle& C);

  void add_archetype_of(Cell const& c);
  void calculate_num_of_edges() const;
  void calculate_archetypes();
  // calculate required connectivity information
  void  calculate_neighbour_cells();
  void  calculate_vertex_cells();
  void  calculate_adjacencies() {
    calculate_neighbour_cells();
    calculate_vertex_cells();
  }

public:

  //------------------------- elements -> handles ------------------

  //@{ @name Element <-> Handle conversion
  inline cell_handle   handle(Cell                  const& C) const;
  inline cell_handle   handle(CellIterator          const& c) const;
  inline cell_handle   handle(CellNeighbourIterator const& c) const;

  inline vertex_handle handle(Vertex               const& V)  const;
  inline vertex_handle handle(VertexIterator       const& v)  const;
  inline vertex_handle handle(VertexOnCellIterator const& v)  const;

  inline bool is_valid_vertex(vertex_handle v) const { return ((0 <= v) && (v <  NumOfVertices()));}
  inline bool is_valid_cell  (cell_handle c)   const { return ((0 <= c) && (c <  NumOfCells   ()));}
  bool valid(vertex_handle v) const { return ((0 <= v) && (v < NumOfVertices()));}
  bool valid(cell_handle   c) const { return ((0 <= c) && (c < NumOfCells   ()));}
  bool valid_vertex(vertex_handle v) const { return valid(v);}
  bool valid_cell  (cell_handle   c) const { return valid(c);}

  inline edge_handle handle(const Edge& E) const;

  //------------------------- handles -> elements  ------------------

  Vertex vertex(const vertex_handle& v) const { return Vertex(*this,v);}
  Cell   cell  (const cell_handle&   c) const { return Cell  (*this,c);}
  inline Edge   edge  (const edge_handle& e) const;
  inline Edge   facet (const edge_handle& e) const;
  //@}
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

  // an ugly hack used to workaround the former lack of template
  // member functions or template friends.
  // the sole purpose of this class is to make public
  // some private functions/data of Complex2D, that are
  // needed to build it from some connectivity information
  // FIXME: this should not be necessary any more for modern compilers.
  
class friend_for_input : public complex2d_types {
public:
  //  typedef  Complex2D::v_list vertex_list;
  //  typedef  Complex2D::c_list   cell_list;
  typedef  Complex2D::CoordType CoordType;
  typedef  Complex2D::boundary_facet_list boundary_facet_list;

  friend_for_input(Complex2D& cc) : _cc(cc) {}

  void clear() { _cc.clear();}
  boundary_facet_list&    boundary()  {return _cc._boundary;} 
  vertex_list_complex2d& _vertices()  {return _cc._vertices;}
  cell_list_complex2d&   _cells()     {return _cc._cells;}

  void calculate_neighbour_cells()    {_cc.calculate_neighbour_cells();}
  void calculate_vertex_cells()       {_cc.calculate_vertex_cells();}
  void calculate_archetypes()         {_cc.calculate_archetypes();}
  void add_archetype_of(Cell const& c) { _cc.add_archetype_of(c);}

  cell_handle   _new_cell(int i)                                { return _cc._new_cell(i);}
  vertex_handle _new_vertex(const CoordType& coo = CoordType(0,0)) { return _cc._new_vertex(coo);}
  Complex2D::archetype_handle add_archetype(Complex2D::archetype_type const& A, int nv) { return _cc.add_archetype(A, nv);}
  
  CoordType& coord(const Vertex2D& V) { return _cc.Coord(V);}

  void set_neighbour(const EdgeOnCell2D_Iterator& n1_it, const Cell2D& n2)
    { _cc.set_neighbour(n1_it,n2);}
  void add_cell_on_vertex(const Vertex2D& V, const Cell2D& C) 
    {_cc.add_cell_on_vertex(V,C);}
  void add_cell_on_vertex(const Vertex2D& V, const Complex2D::cell_handle& c)
    {_cc.add_cell_on_vertex(V,c);}


  cell_handle  outer_cell_handle() const { return _cc.outer_cell_handle();}
  
  cell2d_connectivity::vertex_list& _cell_vertices  (const cell_handle& c) 
  { return (_cc._cells[c]._vertices);}
  cell2d_connectivity::cell_list&   _cell_neighbours(const cell_handle& c) 
  { return (_cc._cells[c]._neighbours);}


private:
  Complex2D& _cc;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include "Gral/Grids/Complex2D/internal/vertex-on-cell2d-it.h" 
#include "Gral/Grids/Complex2D/internal/vertex2d-it.h"
#include "Gral/Grids/Complex2D/internal/cell2d-it.h"

#include "Gral/Grids/Complex2D/internal/edge2d.h"
#include "Gral/Grids/Complex2D/internal/edge2d-it.h"
#include "Gral/Grids/Complex2D/internal/vertex-on-edge2d-it.h"
#include "Gral/Grids/Complex2D/internal/cell-on-edge2d-it.h"
#include "Gral/Grids/Complex2D/internal/boundary-facet-it.h"
#include "Gral/Grids/Complex2D/internal/boundary-vertex-iterator.h"

#include "Gral/Grids/Complex2D/internal/cell-on-cell2d-it.C"
#include "Gral/Grids/Complex2D/internal/edge-on-cell2d-it.C"
#include "Gral/Grids/Complex2D/internal/vertex2d.C"
#include "Gral/Grids/Complex2D/internal/edge2d.C"
#include "Gral/Grids/Complex2D/internal/cell2d.C"
#include "Gral/Grids/Complex2D/internal/complex2d.C"


//-----------------------------------------------------------------------------
//------------  parameterized namespace specialization for Complex2D  ---------
//-----------------------------------------------------------------------------

struct hash_vertex2d;
struct hash_edge2d;
struct hash_cell2d;

struct grid_types_Complex2D  : public complex2d_types {

  typedef Vertex2D_Iterator       VertexIterator;
  typedef Edge2D_Iterator         EdgeIterator;
  typedef Edge2D_Iterator         FacetIterator;
  typedef Cell2D_Iterator         CellIterator;
 
  typedef  Complex2D::BoundaryFacetIterator  BoundaryFacetIterator;
  typedef  Complex2D::BoundaryVertexIterator BoundaryVertexIterator;

  // 2D: Cell <-> Face
  typedef cell_handle             face_handle;
  typedef Cell                    Face;
  typedef CellIterator            FaceIterator;
  typedef VertexOnCellIterator    VertexOnFaceIterator;
  typedef CellOnVertexIterator    FaceOnVertexIterator;
  typedef EdgeOnCellIterator      EdgeOnFaceIterator;
  typedef CellOnEdgeIterator      FaceOnEdgeIterator;

  static size_t hash(const Vertex& V) { return V.handle();}
  static size_t hash(const Cell&   C) { return C.handle();}
  static size_t hash(const Edge&   E) 
  {
    // problems if E is on boundary: one Cell does not exist.
    // bug detected by  __STL_DEBUG
    int c1 =  hash(E.V1()); // hash(E.C1());
    int c2 = hash(E.V2()); // hash(E.C2());
    return (c1 > c2 ? c1 * 4 /*GridOf(E).NumOfCells()*/ + c2 // besser : local_number(c2)
	            : c2 * 4 /*GridOf(E).NumOfCells()*/ + c1);
  }
 
  typedef hash_vertex2d hash_vertex;
  typedef hash_edge2d   hash_edge;
  typedef hash_edge2d   hash_facet;
  typedef hash_cell2d   hash_cell;

  typedef  vertex_base::CoordType CoordType;

  typedef grid_dim_tag<2> dimension_tag;
  static int dimension(const Cell& ) { return 2;}
  static int dimension(const Facet&) { return 1;} 
   
  static grid_type const&  GridOf(const Cell& C) {return C.TheGrid();}
  static grid_type const&  GridOf(const Edge& E) {return E.TheGrid();}
  static grid_type const&  GridOf(const Vertex& V) {return V.TheGrid();}

  static CoordType Coord(const Vertex& V) { return GridOf(V).Coord(V);}

  static cell_handle outer_cell_handle(grid_type const& G) 
  { return G.outer_cell_handle();}
  static cell_handle invalid_cell_handle(grid_type const& G) 
  { return G.outer_cell_handle();}
};

/*! \brief Specialization of the grid_types primary template.
   \ingroup complex2dmodule
 */
template<>
struct grid_types<Complex2D> : public grid_types_base<grid_types_Complex2D> {};


#define gt grid_types<Complex2D>

  inline gt::VertexIterator   gral_begin(gt::grid_type const& G, gt::VertexIterator) { return G.FirstVertex();}
  inline gt::VertexIterator   gral_end  (gt::grid_type const& G, gt::VertexIterator) { return G.EndVertex();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::VertexIterator) { return G.NumOfVertices();}

  inline gt::EdgeIterator     gral_begin(gt::grid_type const& G, gt::EdgeIterator)   { return G.FirstEdge();}
  inline gt::EdgeIterator     gral_end  (gt::grid_type const& G, gt::EdgeIterator)   { return G.EndEdge(); }
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::EdgeIterator)   { return G.NumOfEdges();}

  inline gt::CellIterator     gral_begin(gt::grid_type const& G, gt::CellIterator)   { return G.FirstCell();}
  inline gt::CellIterator     gral_end  (gt::grid_type const& G, gt::CellIterator)   { return G.EndCell();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::CellIterator)   { return G.NumOfCells();}


  inline gt::VertexOnCellIterator   gral_begin(gt::Cell   a, gt::VertexOnCellIterator) { return a.FirstVertex();}
  inline gt::VertexOnCellIterator   gral_end  (gt::Cell   a, gt::VertexOnCellIterator) { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::VertexOnCellIterator) { return a.NumOfVertices();}

  inline gt::VertexOnEdgeIterator   gral_begin(gt::Edge   a, gt::VertexOnEdgeIterator) { return a.FirstVertex();}
  inline gt::VertexOnEdgeIterator   gral_end  (gt::Edge   a, gt::VertexOnEdgeIterator) { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Edge   a, gt::VertexOnEdgeIterator) { return a.NumOfVertices();}

  inline gt::EdgeOnCellIterator     gral_begin(gt::Cell   a, gt::EdgeOnCellIterator) { return a.FirstEdge();}
  inline gt::EdgeOnCellIterator     gral_end  (gt::Cell   a, gt::EdgeOnCellIterator) { return a.EndEdge(); }
  inline gt::size_type              gral_size (gt::Cell   a, gt::EdgeOnCellIterator) { return a.NumOfEdges();}

  inline gt::CellOnCellIterator     gral_begin(gt::Cell   a, gt::CellOnCellIterator) { return a.FirstCell();}
  inline gt::CellOnCellIterator     gral_end  (gt::Cell   a, gt::CellOnCellIterator) { return a.EndCell();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::CellOnCellIterator) { return a.NumOfCells();}


#undef  gt

struct hash_vertex2d {
  typedef grid_types<Complex2D> gt;
  typedef gt::Vertex   Vertex;
  typedef Vertex key_type;
  typedef Vertex argument_type;
  typedef size_t result_type;

  result_type operator()(const Vertex& V) const { return gt::hash(V);}

  void operator=(hash_vertex2d const&) {} // suppress warnings about statement w/o effect
};

struct hash_edge2d {
  typedef grid_types<Complex2D> gt;
  typedef  gt::Edge     Edge;
  typedef Edge   key_type;
  typedef Edge   argument_type;
  typedef size_t result_type;
  result_type operator()(const Edge& E) const { return gt::hash(E);}

  void operator=(hash_edge2d const&) {} // suppress warnings about statement w/o effect
};

struct hash_cell2d {
  typedef grid_types<Complex2D> gt;
  typedef gt::Cell     Cell;
  typedef Cell   key_type;
  typedef Cell   argument_type;
  typedef size_t result_type;

  result_type operator()(const Cell& C) const { return gt::hash(C);}

  void operator=(hash_cell2d const&) {} // suppress warnings about statement w/o effect
};

} // namespace GrAL 

namespace STDEXT {
  template<class T>
    struct hash;

  template<>  
  struct hash<GrAL::Vertex2D> 
    : public GrAL::hash_vertex2d {};
  template<>  
  struct hash<GrAL::Edge2D> 
    : public GrAL::hash_edge2d {};
  template<>  
  struct hash<GrAL::Cell2D> 
    : public GrAL::hash_cell2d {};
}

#include "Gral/Grids/Complex2D/element-traits.h"

#endif
