#ifndef GRAL_GB_CARTESIAN3D_H
#define GRAL_GB_CARTESIAN3D_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Complex2D/complex2d.h"

#include "Container/index-map-nd.h"

#include "Utility/pre-post-conditions.h"


// forward decl. of related types

namespace cartesian3d {

class CartesianGrid3D;
class Vertex_Cartesian3D;
class Edge_Cartesian3D;
class Facet_Cartesian3D;
class Cell_Cartesian3D;
class VertexOnCellIterator_Cartesian3D;


// collect related types in struct,
// to avoid retyping in every one of the above classes.

struct grid_types_Cartesian3D {

  typedef CartesianGrid3D         grid_type;

  typedef vertex_handle_int<grid_type> vertex_handle;
  typedef edge_handle_int<grid_type>   edge_handle;
  typedef facet_handle_int<grid_type>  facet_handle;
  typedef cell_handle_int<grid_type>   cell_handle;
  typedef int                          archetype_handle;

  typedef Vertex_Cartesian3D  Vertex;
  typedef Edge_Cartesian3D    Edge;
  typedef Facet_Cartesian3D   Facet;
  typedef Cell_Cartesian3D    Cell;

  typedef Vertex_Cartesian3D  VertexIterator;
  typedef Edge_Cartesian3D    EdgeIterator;
  typedef Facet_Cartesian3D   FacetIterator;
  typedef Cell_Cartesian3D    CellIterator;

  typedef VertexOnCellIterator_Cartesian3D VertexOnCellIterator;

  typedef grid_dim_tag<3>     dimension_tag;
};



/*! \brief 3D Cartesian grid class. 

    At present, this implements only a 
    $GrAL Cell-VertexInputGridRange.
  
    \todo Implement the full set of grid elements and iterators,
    in particular Edge and Facet types.
    \todo CartesianGrid3D(1,1,1) gives an error in cell_map
          (division by 0). Check this in index_map_nd<>.
    \todo Implement validity checks for iterators
 */
class CartesianGrid3D : public grid_types_Cartesian3D {
public:
  typedef index_map_nd<3>            index_map_type;
  typedef index_map_type::index_type index_type;

  typedef Complex2D             archetype_type;
  typedef archetype_type const* archetype_iterator;

  //! edge directions
  struct edge_direction   { enum dir { x  = 0, y  = 1, z = 2, size = 3 };};
  //! facet directions
  struct  facet_direction { enum dir { xy = 0, xz = 1, yz= 2, size = 3 };};

private:
  index_map_type vertex_map; // maps vertex 3D integer coords to handles,
                             // and vice versa
  index_map_type cell_map;   // same for cells.
  index_map_type edge_maps [edge_direction::size];
  index_map_type facet_maps[facet_direction::size];

  int edge_handle_offset [edge_direction::size]; // offset[x] = 0; offset[y] = NumOfXEdges(); ...
  int facet_handle_offset[facet_direction::size];


private:
  // static data
  class SD {
  public:
    typedef CartesianGrid3D grid_type;
    SD();
    // get 3D vertex index from cell index
    // and local vertex number. Acts as geometry for the archetype below.
    grid_type::index_type corner_offset[8]; 

    index_type edge_index_offset [edge_direction::size]; // { offset[x]=index_type(1,0,0); ...}
    index_type facet_index_offset[facet_direction::size][2]; // facet has 2 dimensions
                                          
    // 2D grid of a cell surface.
    // Although there is only one archetype, we
    // use an array to easily get an iterator type.
    grid_type::archetype_type  the_archetype[1]; 
  };
  static SD sd;

  class archetype_geom {
  public:
    typedef CartesianGrid3D::index_type coord_type;
   
    typedef CartesianGrid3D::archetype_type grid_type;
    typedef grid_types<archetype_type>      agt;
  private:
    grid_type const* g;
  public:
    archetype_geom(grid_type const& gg) : g(&gg) {}

    coord_type const& coord(agt::Vertex const& v) const
      { return CartesianGrid3D::sd.corner_offset[v.handle()];}
  };

public:
  typedef archetype_geom archetype_geom_type;

  friend class archetype_geom;
  friend class Vertex_Cartesian3D;
  friend class Edge_Cartesian3D;
  friend class Facet_Cartesian3D;
  friend class Cell_Cartesian3D;
  friend class VertexOnCellIterator_Cartesian3D;

public:
  /*! \name Constructors */
  /*@{*/ 
  //! Empty grid
  CartesianGrid3D();
  //! Grid with \f$ vx \times vy \times vz \f$ vertices.
  CartesianGrid3D(int vx, int vy, int vz);
  /*@}*/
private:
  void init(int vx, int vy, int vz);
public:

  bool empty() const { return (NumOfVertices() == 0);}

  /*! \name Element size functions */
  /*@{*/ 
  unsigned NumOfVertices() const { return vertex_map.max_flat_index()+1;}
  unsigned NumOfCells   () const { return cell_map  .max_flat_index()+1;}
  unsigned NumOfEdges   () const { return NumOfXDirEdges()+NumOfYDirEdges()+NumOfZDirEdges(); }
  unsigned NumOfFacets  () const { return NumOfXYDirFacets()+NumOfXZDirFacets()+NumOfYZDirFacets();}
  /*@}*/

  /*! \name Sequence iteration */
  /*@{*/ 
  inline VertexIterator FirstVertex() const;
  inline EdgeIterator   FirstEdge()   const;
  inline FacetIterator  FirstFacet()  const;
  inline CellIterator   FirstCell()   const;
  /*@}*/

  /*! \name 1D element numbers
       Number of vertices, cells along a grid dimension */
  /*@{*/ 
  //! equal to vx arg of constructor
  unsigned NumOfXVertices() const { return vertex_map.max_tuple()[0]+1;}
  //! equal to vy arg of constructor
  unsigned NumOfYVertices() const { return vertex_map.max_tuple()[1]+1;}
  //! equal to vz arg of constructor
  unsigned NumOfZVertices() const { return vertex_map.max_tuple()[2]+1;}
  //! equal to NumOfXVertices()-1
  unsigned NumOfXCells   () const { return cell_map.max_tuple()[0]+1  ;}
  //! equal to NumOfYVertices()-1
  unsigned NumOfYCells   () const { return cell_map.max_tuple()[1]+1  ;}
  //! equal to NumOfZVertices()-1
  unsigned NumOfZCells   () const { return cell_map.max_tuple()[2]+1  ;}
  /*@}*/

  /*! \name Directional edge,facet element numbers
       Number of Edges/Facets with a given direction.
       Note that the meaning is different from NumOfXVertices() / NumOfXCells() etc. !! */
  /*@{*/ 
  unsigned NumOfXDirEdges() const { return NumOfDirEdges(edge_direction::x);}
  unsigned NumOfYDirEdges() const { return NumOfDirEdges(edge_direction::y);}
  unsigned NumOfZDirEdges() const { return NumOfDirEdges(edge_direction::z);}
  unsigned NumOfDirEdges(edge_direction::dir dir) const { return edge_maps[dir].max_flat_index()+1;}

  unsigned NumOfXYDirFacets() const { return NumOfDirFacets(facet_direction::xy);}
  unsigned NumOfXZDirFacets() const { return NumOfDirFacets(facet_direction::xz);}
  unsigned NumOfYZDirFacets() const { return NumOfDirFacets(facet_direction::yz);}
  unsigned NumOfDirFacets(facet_direction::dir dir) const { return facet_maps[dir].max_flat_index()+1;}
  /*@}*/


  /*! \name Element map accessors.
      Access to element maps, mapping flat indices to 3D and vice versa */
  /*@{*/ 
  index_map_type const& VertexMap()                   const { return vertex_map;}
  index_map_type const& CellMap()                     const { return cell_map;}
  index_map_type const& EdgeMap (edge_direction::dir dir)  const { return edge_maps [dir];}
  index_map_type const& FacetMap(facet_direction::dir dir) const { return facet_maps[dir];}
  /*@}*/

  /*! \name direction extraction from handles
   */
  /*@{*/ 
  edge_direction::dir get_direction(edge_handle e) const { cv(e);  
  return (e < (int) NumOfXDirEdges() ? edge_direction::x
	  : e <  (int) (NumOfXDirEdges() + NumOfYDirEdges()) ? edge_direction::y
	  : edge_direction::z);
  }
  facet_direction::dir get_direction(facet_handle f) const
  {
    cv(f);  
    return (f < (int)NumOfXYDirFacets() ? facet_direction::xy
		: f < (int) (NumOfXYDirFacets() + NumOfXZDirFacets()) ? facet_direction::xz
		    : facet_direction::yz);
  }
  /*@}*/

  /*! \name Checking functions */
 /*@{*/ 
  void cv(vertex_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(vertex_handle e) const { return (0 <= e && e < (int)NumOfVertices()); }
  void cv(edge_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(edge_handle e) const { return (0 <= e && e < (int)NumOfEdges()); }
  void cv(facet_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(facet_handle e) const { return (0 <= e && e < (int)NumOfFacets()); }
  void cv(cell_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(cell_handle e) const { return (0 <= e && e < (int)NumOfCells()); }
  /*@}*/

  /*! \name Archetype handling
   */
  /*@{*/ 
  archetype_type const& Archetype(int a) const {
    REQUIRE(a == 0, "a = " << a,1);
    return *BeginArchetype();
  }

  archetype_type   const& ArchetypeOf (Cell const&) const 
    { return *BeginArchetype();}
  archetype_type   const& ArchetypeOf (cell_handle) const 
    { return *BeginArchetype();}

  archetype_handle        archetype_of(cell_handle) const 
    { return 0;}
  archetype_handle        archetype_of(Cell const&) const 
    { return 0;}

  static archetype_iterator BeginArchetype() 
    { return archetype_iterator(sd.the_archetype);}
  static archetype_iterator EndArchetype()  { return BeginArchetype() +1;}
  unsigned NumOfArchetypes() const { return 1;}


  archetype_geom_type ArchetypeGeom(archetype_type const& A 
				    = *BeginArchetype()) const 
    { return archetype_geom_type(A); }

  archetype_handle handle(archetype_iterator a) const 
    { return a - BeginArchetype();}
  /*@}*/
};


class elem_base_Cartesian3D : public grid_types_Cartesian3D 
{
 private:
   grid_type const* g;
 public:
  elem_base_Cartesian3D() : g(0) {}
  elem_base_Cartesian3D(grid_type const& gg) : g(&gg) {}

  grid_type const& TheGrid() const { return *g;}
 
  bool valid() const { return g != 0;}
};


  /*! \brief Vertex class for CartesianGrid3D */
class Vertex_Cartesian3D : public elem_base_Cartesian3D {
  typedef Vertex_Cartesian3D     self;
  typedef elem_base_Cartesian3D  base;

  vertex_handle            h;
public:
  Vertex_Cartesian3D() {}  
  explicit
  Vertex_Cartesian3D(grid_type const&  gg, vertex_handle v = 0)
    : base(gg), h(v) {}
  Vertex_Cartesian3D(grid_type const&  gg, grid_type::index_type Iv)
    : base(gg)
  { h = TheGrid().vertex_map(Iv);}
  Vertex_Cartesian3D(grid_type const& gg, int x, int y, int z)
    : base(gg)
  {
    h = TheGrid().VertexMap()(grid_type::index_type(x,y,z));
  }

  self & operator++() { ++h; return *this;}
  self const& operator*() const { return *this;}
  bool IsDone() const { return h >= static_cast<int>(TheGrid().NumOfVertices());}
  vertex_handle handle() const { return h;}
  grid_type::index_type  index() const { return TheGrid().VertexMap()(h);}

  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.h <  rhs.h);}

  bool valid() const { return base::valid() && !IsDone(); }
};




 /*! \brief Edge class for CartesianGrid3D */
  class Edge_Cartesian3D : public elem_base_Cartesian3D {
    typedef Edge_Cartesian3D       self;
    typedef elem_base_Cartesian3D  base;
    
    edge_handle h;
  public:
    Edge_Cartesian3D() {}
    Edge_Cartesian3D(grid_type const& gg, edge_handle e = 0) 
      : base(gg), h(e) {}
    //Edge_Cartesian3D(grid_type const& gg, index_type v1, index_type v2);
    //Edge_Cartesian3D(grid_type const& gg, index_type v1, direction_type d);
    
    self & operator++() { cv(); ++h; return *this;}
    self const& operator*() const { cv();  return *this;}
    bool IsDone() const { return h >= static_cast<int>(TheGrid().NumOfEdges());}
    edge_handle handle() const { cv(); return h;}
    grid_type::index_type  index() const { 
      cv(); 
      grid_type::edge_direction::dir d = dir();
      return TheGrid().EdgeMap(d)(h - TheGrid().edge_handle_offset[d]);
    }
    grid_type::edge_direction::dir dir() const { cv(); return TheGrid().get_direction(h);}

    friend bool operator==(self const& lhs, self const& rhs)
    { return (lhs.h == rhs.h);}
    friend bool operator!=(self const& lhs, self const& rhs)
    { return !(lhs == rhs);}
    friend bool operator< (self const& lhs, self const& rhs)
    { return (lhs.h <  rhs.h);}
    
    bool valid() const { return base::valid() && !IsDone(); }
    void cv()    const { REQUIRE(valid(), "invalid Edge: h = " << h , 1)};
  };



 /*! \brief Facet class for CartesianGrid3D */
class Facet_Cartesian3D : public elem_base_Cartesian3D {
  typedef Facet_Cartesian3D       self;
  typedef elem_base_Cartesian3D  base;

  facet_handle h;
public:
  Facet_Cartesian3D() {}
  Facet_Cartesian3D(grid_type const& gg, facet_handle e = 0) 
    : base(gg), h(e) {}
  //Facet_Cartesian3D(grid_type const& gg, index_type v1, index_type v2);
  //Facet_Cartesian3D(grid_type const& gg, index_type v1, direction_type d);

  self & operator++() { cv(); ++h;  return *this; }
  self const& operator*() const { cv();  return *this;}
  bool IsDone() const { return h >= static_cast<int>(TheGrid().NumOfFacets());}
  facet_handle handle() const { cv(); return h;}
  grid_type::index_type index() const { 
    cv(); 
    grid_type::facet_direction::dir d = dir();
    return TheGrid().FacetMap(d)(h - TheGrid().facet_handle_offset[d]);
  }
  grid_type::facet_direction::dir dir() const { cv(); return TheGrid().get_direction(h);}


  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.h <  rhs.h);}

  bool valid() const { return base::valid() && !IsDone(); }
  void cv()    const { REQUIRE(valid(), "invalid Facet: h = " <<h , 1)};

};





 /*! \brief Cell class for CartesianGrid3D */
class Cell_Cartesian3D : public elem_base_Cartesian3D {
  typedef Cell_Cartesian3D       self;
  typedef elem_base_Cartesian3D  base;
  typedef grid_type::archetype_type archetype_type;
  typedef grid_types<archetype_type> archgt;

  cell_handle              h;
  grid_type::index_type    I;

  //friend class VertexOnCellIterator_Cartesian3D;
public:
  Cell_Cartesian3D() {}  
  explicit
  Cell_Cartesian3D(grid_type const&  gg, cell_handle c = 0)
    : base(gg), h(c) 
    { if(valid()) I = TheGrid().cell_map(h);}
  Cell_Cartesian3D(grid_type const&  gg, grid_type::index_type Ic)
    : base(gg), I(Ic) 
    { 
      if(TheGrid().CellMap().valid(I))
	h = TheGrid().cell_map(I);
    }

  // could be done more efficiently: cell_map.incr(I)
  self & operator++() { ++h; if(valid()) { I = TheGrid().cell_map(h);}  return *this;}
  self const& operator*() const { return *this;}
  bool IsDone() const { return h >= static_cast<int>(TheGrid().NumOfCells());}
  cell_handle handle() const { return h;}
  grid_type::index_type const& index() const { return I;}

  inline VertexOnCellIterator FirstVertex() const;
  unsigned NumOfVertices() const { return 8;}
  Vertex V(archgt::vertex_handle lh) const { // local2global
    return Vertex(TheGrid(), I+grid_type::sd.corner_offset[lh]);}
  vertex_handle v(archgt::vertex_handle lh) const 
    { return V(lh).handle();} // inefficient
  Vertex        V(archgt::Vertex lv) const { return V(lv.handle());}
  vertex_handle v(archgt::Vertex lv) const { return v(lv.handle());}

  archetype_type const& TheArchetype() const { return TheGrid().ArchetypeOf(*this);}

  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.h <  rhs.h);}

  bool valid() const { return base::valid() && !IsDone(); }
};




class VertexOnCellIterator_Cartesian3D 
  : public grid_types_Cartesian3D 
{
  typedef VertexOnCellIterator_Cartesian3D self;
private:
  Cell   c;
  int    lv; // archetype::vertex_handle?
public:
  VertexOnCellIterator_Cartesian3D()  {}
  explicit
  VertexOnCellIterator_Cartesian3D(Cell const& cc, int llv = 0) 
    : c(cc), lv(llv) {}  


  self& operator++() {
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    ++lv; return *this;
  }
  Vertex operator*() const { 
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return Vertex(c.TheGrid(), index());
  }
  bool IsDone() const { return lv >= 8;}

  grid_type::index_type   index()  const { 
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return c.index() + grid_type::sd.corner_offset[lv];
  }
  vertex_handle handle() const {
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return TheGrid().vertex_map(index());
  }
  int local_handle() const { return lv;}

  Cell      const& TheCell() const { return c;}
  grid_type const& TheGrid() const { return c.TheGrid();}


  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.c == rhs.c) && (lhs.lv == rhs.lv);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.c < rhs.c) || ((lhs.c == rhs.c) && (lhs.lv <  rhs.lv));}

  bool valid() const { return c.valid() && (0 <= lv) && (lv <= 7);}
};


// inline member functions of Cartesian3D

inline Vertex_Cartesian3D
CartesianGrid3D::FirstVertex() const
{ return Vertex(*this);}

inline Edge_Cartesian3D
CartesianGrid3D::FirstEdge() const
{ return Edge(*this);}

inline Facet_Cartesian3D
CartesianGrid3D::FirstFacet() const
{ return Facet(*this);}

inline Cell_Cartesian3D
CartesianGrid3D::FirstCell() const
{ return Cell(*this);}


// inline member functions of Cell_Cartesian3D

inline
VertexOnCellIterator_Cartesian3D
Cell_Cartesian3D::FirstVertex() const
{ return VertexOnCellIterator(*this);}


}; // namespace cartesian3d

// these must be in global namespace

// grid_types<>

template<>
struct grid_types<cartesian3d::CartesianGrid3D> 
  : public cartesian3d::grid_types_Cartesian3D
{
  typedef grid_type::archetype_type      archetype_type;
  //typedef grid_type::archetype_handle  archetype_handle;
  typedef grid_type::archetype_iterator  ArchetypeIterator;
  typedef grid_type::archetype_geom_type archetype_geom_type;
  typedef grid_types<archetype_type>     archgt;
};



// element traits

template<>
struct element_traits<cartesian3d::Vertex_Cartesian3D>
  : public element_traits_vertex_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>    consecutive_tag;
};

template<>
struct element_traits<cartesian3d::Edge_Cartesian3D>
  : public element_traits_edge_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>  consecutive_tag;
};

template<>
struct element_traits<cartesian3d::Facet_Cartesian3D>
  : public element_traits_facet_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>  consecutive_tag;
};

template<>
struct element_traits<cartesian3d::Cell_Cartesian3D>
  : public element_traits_cell_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>  consecutive_tag;
};




#endif
