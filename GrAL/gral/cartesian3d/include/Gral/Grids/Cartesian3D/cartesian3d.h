#ifndef GRAL_GB_CARTESIAN3D_H
#define GRAL_GB_CARTESIAN3D_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"

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

  typedef int vertex_handle;
  typedef int cell_handle;

  typedef Vertex_Cartesian3D  Vertex;
  typedef Cell_Cartesian3D    Cell;
  typedef Vertex_Cartesian3D  VertexIterator;
  typedef Cell_Cartesian3D    CellIterator;
  typedef VertexOnCellIterator_Cartesian3D VertexOnCellIterator;
};



/*! \brief 3D Cartesian grid class. 

    At present, this implements only a 
    $GrAL CellVertexInputGrid.
  
    \todo Implement the full set of grid iterators.
    \todo CartesianGrid3D(1,1,1) gives an error in cell_map
          (division by 0). Check this in index_map_nd<>.
 */
class CartesianGrid3D : public grid_types_Cartesian3D {
public:
  typedef index_map_nd<3>            index_map_type;
  typedef index_map_type::index_type index_type;

private:
  index_map_type vertex_map; // maps vertex 3D integer coords to handles,
                             // and vice versa
  index_map_type cell_map;   // same for cells.

  static index_type corner_offset[8]; // get 3D vertex index from cell index
                                      // and local vertex number
  typedef int c_index[3];


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
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.h <  rhs.h);}

  bool valid() const { return base::valid() && !IsDone(); }
};



class Cell_Cartesian3D : public elem_base_Cartesian3D {
  typedef Cell_Cartesian3D       self;
  typedef elem_base_Cartesian3D  base;

  cell_handle              h;
  grid_type::index_type    I;

  //friend class VertexOnCellIterator_Cartesian3D;
public:
  Cell_Cartesian3D() {}  
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

  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.h == rhs.h);}
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
  int         lv;
public:
  VertexOnCellIterator_Cartesian3D() : c(0) {}
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
    return c->index() + grid_type::corner_offset[lv];
  }
  vertex_handle handle() const {
    REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);
    return TheGrid().vertex_map(index());
  }

  Cell      const& TheCell() const { return *c;}
  grid_type const& TheGrid() const { return c->TheGrid();}


  friend bool operator==(self const& lhs, self const& rhs)
  { return (*(lhs.c) == *(rhs.c)) && (lhs.lv == rhs.lv);}
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
};

template<>
struct element_traits<Cell_Cartesian3D>
  : public element_traits_cell_base<CartesianGrid3D>
{
 typedef std::hash<Cell_Cartesian3D> hasher_type;
};




#endif
