#ifndef GRAL_GB_BASE_POLYGON1D_H
#define GRAL_GB_BASE_POLYGON1D_H

// $LICENSE_NEC
#include "Gral/Base/element-handle.h"
#include "Gral/Base/vertex-iterator-int.h"
#include "Gral/Base/cell-iterator-int.h"

namespace polygon1d {
  
  //class VertexIterator1d;
  //class CellIterator1d;
  class VertexOnCellIterator1d;
  class polygon;

  struct grid_types_p1d {
    typedef polygon                    grid_type;
    typedef grid_dim_tag<1>            dimension_tag;

    typedef vertex_handle_int<polygon> vertex_handle;
    typedef cell_handle_int  <polygon> cell_handle;
 
    typedef vertex_iterator_int<polygon, grid_types_p1d> VertexIterator;
    typedef cell_iterator_int<polygon, grid_types_p1d>   CellIterator;
    typedef VertexIterator Vertex;
    typedef CellIterator   Cell;

   
    typedef VertexOnCellIterator1d     VertexOnCellIterator;

    typedef vertex_handle              facet_handle;
    typedef cell_handle                edge_handle;
    typedef Vertex                     Facet;
    typedef Cell                       Edge;
    typedef VertexIterator             FacetIterator;
    typedef CellIterator               EdgeIterator;
    typedef VertexOnCellIterator       FacetOnCellIterator;
    typedef VertexOnCellIterator       VertexOnEdgeIterator;

  };

  class polygon : public grid_types_p1d {
  private:
    unsigned nv;
  public:
    explicit polygon(int nnv = 0) : nv(nnv) {}
    
    unsigned NumOfVertices() const { return nv;}
    unsigned NumOfCells()    const { return nv;}

    unsigned NumOfVertices(Cell const&) const { return 2;}

    VertexIterator FirstVertex() const;
    CellIterator   FirstCell()   const;

  };

  class VertexOnCellIterator1d : public grid_types_p1d {
    typedef VertexOnCellIterator1d self;
  private:
    Cell      c;
    unsigned  h;
  public:
    VertexOnCellIterator1d() {}
    VertexOnCellIterator1d(Cell const& cc) : c(cc), h(0) {}

    self& operator++() { cv(); ++h; return *this;}
    Vertex operator*() const { cv(); return Vertex(TheGrid(),handle());} 
    vertex_handle handle() const { 
      cv(); 
      unsigned v = c.handle();
      return (h == 0 ? vertex_handle(v) : vertex_handle( (v+1)%TheGrid().NumOfVertices()));
    }
    bool IsDone() const { cb(); return h >= 2;}

    Cell      const& TheCell()   const { cb(); return c;}
    Cell      const& TheAnchor() const { cb(); return c;}
    grid_type const& TheGrid()   const { cb(); return c.TheGrid();}

    friend bool operator==(self const& lhs, self const& rhs) { lhs.cb(); rhs.cb(); return (lhs.h == rhs.h);}
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}

    bool bound() const { return c.valid();}
    bool valid() const { return bound() && h < 2;}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "h=" << h, 1);}
  };

} // namespace polygon1d

template<>
struct grid_types<polygon1d::polygon> : public grid_types_base<polygon1d::grid_types_p1d>
{
  // in order to allow derivation from this class
  grid_types() {}
};


namespace polygon1d {
  inline polygon::VertexIterator polygon::FirstVertex() const { return VertexIterator(*this);}
  inline polygon::CellIterator   polygon::FirstCell()   const { return CellIterator  (*this);}

} // namespace polygon1d

#endif
