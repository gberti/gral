#ifndef GRAL_GB_CARTESIAN3D_H
#define GRAL_GB_CARTESIAN3D_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Complex2D/complex2d.h"

#include "Gral/Grids/Cartesian3D/index-map-nd.h"

#include "Utility/pre-post-conditions.h"


// forward decl. of related types

class CartesianGrid3D;
class Vertex_Cartesian3D;
class Cell_Cartesian3D;
class VertexOnCellIterator_Cartesian3D;


// collect related types in struct,
// to avoid retyping in every one of the above classes.

struct grid_types_Cartesian3D {

  typedef CartesianGrid3D         grid_type;

  typedef vertex_handle_int<grid_type> vertex_handle;
  typedef cell_handle_int<grid_type>   cell_handle;
  typedef int                          archetype_handle;

  typedef Vertex_Cartesian3D  Vertex;
  typedef Cell_Cartesian3D    Cell;
  typedef Vertex_Cartesian3D  VertexIterator;
  typedef Cell_Cartesian3D    CellIterator;
  typedef VertexOnCellIterator_Cartesian3D VertexOnCellIterator;

  typedef grid_dim_tag<3>     dimension_tag;
};



/*! \brief 3D Cartesian grid class. 

    At present, this implements only a 
    $GrAL CellVertexInputGrid.
  
    \todo Implement the full set of grid elements and iterators,
    in particular Edge and Facet types.
    \todo CartesianGrid3D(1,1,1) gives an error in cell_map
          (division by 0). Check this in index_map_nd<>.
 */
class CartesianGrid3D : public grid_types_Cartesian3D {
public:
  typedef index_map_nd<3>            index_map_type;
  typedef index_map_type::index_type index_type;

  typedef Complex2D archetype_type;
  typedef archetype_type const* archetype_iterator;
private:
  index_map_type vertex_map; // maps vertex 3D integer coords to handles,
                             // and vice versa
  index_map_type cell_map;   // same for cells.

  class SD {
  public:
    typedef CartesianGrid3D grid_type;
    SD();
    // get 3D vertex index from cell index
    // and local vertex number. Acts as geometry for the archetype below.
    grid_type::index_type corner_offset[8]; 
                                        
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
  friend class Cell_Cartesian3D;
  friend class VertexOnCellIterator_Cartesian3D;

public:
  CartesianGrid3D()
    // : cell_map(index_type(c_index({0,0,0})))
    {
      int I[3] = {0,0,0};
      vertex_map = index_map_type(I); 
      cell_map   = index_map_type(I); 
    }
  CartesianGrid3D(int vx, int vy, int vz)
    // : vertex_map(index_type(vx,  vy,  vz)),
    //  cell_map  (index_type(vx-1,vy-1,vz-1))
    {
      int v[3] = {vx,vy,vz};
      vertex_map = index_map_type(v);
      int c[3] = {vx-1,vy-1,vz-1};
      cell_map   = index_map_type(c); 
    }

  unsigned NumOfVertices() const { return vertex_map.max_flat_index()+1;}
  unsigned NumOfCells   () const { return cell_map  .max_flat_index()+1;}
  inline VertexIterator FirstVertex() const;
  inline CellIterator   FirstCell()   const;

  unsigned NumOfXVertices() const { return vertex_map.max_tuple()[0]+1;}
  unsigned NumOfYVertices() const { return vertex_map.max_tuple()[1]+1;}
  unsigned NumOfZVertices() const { return vertex_map.max_tuple()[2]+1;}
  unsigned NumOfXCells   () const { return vertex_map.max_tuple()[0]  ;}
  unsigned NumOfYCells   () const { return vertex_map.max_tuple()[1]  ;}
  unsigned NumOfZCells   () const { return vertex_map.max_tuple()[2]  ;}


  static archetype_iterator BeginArchetype() 
    { return archetype_iterator(sd.the_archetype);}
  static archetype_iterator EndArchetype()  { return BeginArchetype() +1;}
  unsigned NumOfArchetypes() const { return 1;}

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

  archetype_geom_type ArchetypeGeom(archetype_type const& A 
				    = *BeginArchetype()) const 
    { return archetype_geom_type(A); }

  archetype_handle handle(archetype_iterator a) const 
    { return a - BeginArchetype();}

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



class Vertex_Cartesian3D : public elem_base_Cartesian3D {
  typedef Vertex_Cartesian3D     self;
  typedef elem_base_Cartesian3D  base;

  vertex_handle            h;
  grid_type::index_type    I;
public:
  Vertex_Cartesian3D() {}  
  explicit
  Vertex_Cartesian3D(grid_type const&  gg, vertex_handle v = 0)
    : base(gg), h(v) 
    { I = TheGrid().vertex_map(h);}
  Vertex_Cartesian3D(grid_type const&  gg, grid_type::index_type Iv)
    : base(gg), I(Iv) 
    { h = TheGrid().vertex_map(I);}

  self & operator++() { ++h; I = TheGrid().vertex_map(h); return *this;}
  self const& operator*() const { return *this;}
  bool IsDone() const { return h >= static_cast<int>(TheGrid().NumOfVertices());}
  vertex_handle handle() const { return h;}
  grid_type::index_type const& index() const { return I;}

  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.h <  rhs.h);}

  bool valid() const { return base::valid() && !IsDone(); }
};



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
    { I = TheGrid().cell_map(h);}
  Cell_Cartesian3D(grid_type const&  gg, grid_type::index_type Ic)
    : base(gg), I(Ic) 
    { h = TheGrid().cell_map(I);}

  // could be done more efficiently: cell_map.incr(I)
  self & operator++() { ++h; I = TheGrid().cell_map(h); return *this;}
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
  Cell const* c;
  int         lv; // archetype::vertex_handle?
public:
  VertexOnCellIterator_Cartesian3D() : c(0) {}
  explicit
  VertexOnCellIterator_Cartesian3D(Cell const& cc, int llv = 0) 
    : c(&cc), lv(llv) {}  


  self& operator++() {
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    ++lv; return *this;
  }
  Vertex operator*() const { 
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return Vertex(c->TheGrid(), index());
  }
  bool IsDone() const { return lv >= 8;}

  grid_type::index_type   index()  const { 
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return c->index() + grid_type::sd.corner_offset[lv];
  }
  vertex_handle handle() const {
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return TheGrid().vertex_map(index());
  }
  int local_handle() const { return lv;}

  Cell      const& TheCell() const { return *c;}
  grid_type const& TheGrid() const { return c->TheGrid();}


  friend bool operator==(self const& lhs, self const& rhs)
  { return (*(lhs.c) == *(rhs.c)) && (lhs.lv == rhs.lv);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (*(lhs.c) < *(rhs.c)) || ((*(lhs.c) == *(rhs.c)) && (lhs.lv <  rhs.lv));}

  bool valid() const { return c->valid() && (0 <= lv) && (lv <= 7);}
};


// inline member functions of Cartesian3D

inline Vertex_Cartesian3D
CartesianGrid3D::FirstVertex() const
{ return Vertex(*this);}

inline Cell_Cartesian3D
CartesianGrid3D::FirstCell() const
{ return Cell(*this);}


// inline member functions of Cell_Cartesian3D

inline
VertexOnCellIterator_Cartesian3D
Cell_Cartesian3D::FirstVertex() const
{ return VertexOnCellIterator(*this);}



// grid_types<>

template<>
struct grid_types<CartesianGrid3D> 
  : public grid_types_Cartesian3D
{
  typedef grid_type::archetype_type      archetype_type;
  //typedef grid_type::archetype_handle  archetype_handle;
  typedef grid_type::archetype_iterator  ArchetypeIterator;
  typedef grid_type::archetype_geom_type archetype_geom_type;
};



// element traits

namespace std {
  template<class T> struct hash;
  
  
  template<>
  struct hash<Vertex_Cartesian3D> {
    size_t operator()(Vertex_Cartesian3D const& e) const { return e.handle();}
  };
  template<>
  struct hash<Cell_Cartesian3D> {
    size_t operator()(Cell_Cartesian3D const& e) const { return e.handle();}
  };
};

template<>
struct element_traits<Vertex_Cartesian3D>
  : public element_traits_vertex_base<CartesianGrid3D>
{
  typedef std::hash<Vertex_Cartesian3D> hasher_type;
  typedef consecutive_integer_tag<0>    consecutive_tag;
};

template<>
struct element_traits<Cell_Cartesian3D>
  : public element_traits_cell_base<CartesianGrid3D>
{
  typedef std::hash<Cell_Cartesian3D> hasher_type;
  typedef consecutive_integer_tag<0>  consecutive_tag;
};




#endif
