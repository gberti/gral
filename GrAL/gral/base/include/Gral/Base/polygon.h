#ifndef GRAL_GB_BASE_POLYGON1D_H
#define GRAL_GB_BASE_POLYGON1D_H

// $LICENSE_NEC
#include "Gral/Base/element-handle.h"
#include "Gral/Base/vertex-iterator-int.h"
#include "Gral/Base/cell-iterator-int.h"
#include "Gral/Base/archetype-0D.h"
#include "Gral/Base/construct-grid.h"


namespace polygon1d  { 
  
  //class VertexIterator1d;
  //class CellIterator1d;
  class VertexOnCellIterator1d;
  class polygon;

  struct grid_types_p1d {
    typedef polygon                    grid_type;
    typedef grid_dim_tag<1>            dimension_tag;

    typedef vertex_handle_int<polygon> vertex_handle;
    typedef cell_handle_int  <polygon> cell_handle;
 
    typedef vertex_iterator_int<grid_types_p1d>  VertexIterator;
    typedef cell_iterator_int  <grid_types_p1d>  CellIterator;
    typedef VertexIterator Vertex;
    typedef CellIterator   Cell;

   
    typedef VertexOnCellIterator1d     VertexOnCellIterator;

    // Cell <-> Edge, Facet <-> Vertex
    typedef vertex_handle              facet_handle;
    typedef cell_handle                edge_handle;
    typedef Vertex                     Facet;
    typedef Cell                       Edge;
    typedef VertexIterator             FacetIterator;
    typedef CellIterator               EdgeIterator;
    typedef VertexOnCellIterator       FacetOnCellIterator;
    typedef VertexOnCellIterator       VertexOnEdgeIterator;

  };

  /*! A 1D grid class
   
     An object of type polygon represents a simple polygon.

  */
  class polygon : public grid_types_p1d, public archetype_0D::grid_mixin<polygon, grid_types_p1d> {
  private:
    unsigned nv;
  public:
    explicit polygon(int nnv = 0) : nv(nnv) {}

    void set_number_of_vertices(int nnv) { nv = nnv;}
    
    unsigned NumOfVertices() const { return nv;}
    unsigned NumOfCells()    const { return nv;}

    unsigned NumOfEdges()  const { return NumOfCells();}
    unsigned NumOfFacets() const { return NumOfVertices();}

    VertexIterator FirstVertex() const;
    CellIterator   FirstCell()   const;

    EdgeIterator  FirstEdge()  const { return FirstCell();}
    FacetIterator FirstFacet() const { return FirstVertex();}

    unsigned NumOfVertices(Cell const&) const { return 2;}
    unsigned NumOfFacets  (Cell const&) const { return 2;}

    int next_handle(int h) const { return (h == (int) NumOfVertices()-1 ?  0                : h+1);}
    int prev_handle(int h) const { return (h == 0                 ? NumOfVertices()-1 : h-1);}

    inline void switch_vertex(Vertex      & v, Cell const& c) const;
    inline void switch_cell  (Vertex const& v, Cell      & c) const;

    inline Vertex switched_vertex(Vertex const& v, Cell const& c) const;
    inline Cell   switched_cell  (Vertex const& v, Cell const& c) const;

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
struct grid_types<polygon1d::polygon> 
 : public grid_types_base<polygon1d::grid_types_p1d>,
   public archetype_0D::grid_types_mixin
{
  // in order to allow derivation from this class
  grid_types() {}
};

template<>
struct element_traits<polygon1d::polygon::Vertex> 
  : public element_traits_vertex_base<polygon1d::polygon>
{
  typedef element_traits_vertex_base<polygon1d::polygon> base;
  typedef consecutive_integer_tag<0>                     consecutive_tag;
  typedef base::hasher_type_elem_base                    hasher_type;
};

template<>
struct element_traits<polygon1d::polygon::Cell> 
  : public element_traits_cell_base<polygon1d::polygon>
{
  typedef element_traits_cell_base<polygon1d::polygon> base;
  typedef consecutive_integer_tag<0>                     consecutive_tag;
  typedef base::hasher_type_elem_base                    hasher_type;
};


template<class T>
class grid_function<polygon1d::polygon::Vertex, T> 
  : public grid_function_vector<polygon1d::polygon::Vertex, T> 
{
  typedef grid_function_vector<polygon1d::polygon::Vertex, T> base;
public:
  typedef polygon1d::polygon                                  grid_type;
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};

template<class T>
class grid_function<polygon1d::polygon::Cell, T> 
  : public grid_function_vector<polygon1d::polygon::Cell, T> 
{
  typedef grid_function_vector<polygon1d::polygon::Cell, T> base;
public:
  typedef polygon1d::polygon                                  grid_type;
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};

/*
template<class VCORR, class CCORR>
void ConstructGrid0(polygon1d::polygon      & G_dst, 
		    polygon1d::polygon const& G_src,
		    VCORR & v_corr,
		    CCORR & c_corr)
{
  typedef grid_types<polygon1d::polygon> gtsrc;
  typedef grid_types<polygon1d::polygon> gtdst;

  G_dst.set_number_of_vertices(G_src.NumOfVertices());

  gtsrc::VertexIterator v_src = G_src.FirstVertex(); 
  gtdst::VertexIterator v_dst = G_dst.FirstVertex();
  for(; ! v_src.IsDone(); ++v_src, ++v_dst)
    v_corr[v_src.handle()] = v_dst.handle();

  gtsrc::CellIterator c_src = G_src.FirstCell(); 
  gtdst::CellIterator c_dst = G_dst.FirstCell();
  for(; ! c_src.IsDone(); ++c_src, ++c_dst)
    c_corr[c_src.handle()] = c_dst.handle();
}
*/

template<class GSRC, class VCORR, class CCORR>
void ConstructGrid0(polygon1d::polygon      & G_dst, 
		    GSRC               const& G_src,
		    VCORR & v_corr,
		    CCORR & c_corr)
{
  typedef grid_types<GSRC>               gtsrc;
  typedef grid_types<polygon1d::polygon> gtdst;

  G_dst.set_number_of_vertices(G_src.NumOfVertices());

  if(G_src.NumOfVertices() > 0) {
    // std::vector<typename gtsrc::vertex_handle> ordered_src_vertices(G_src.NumOfVertices());
    
    typename gtsrc::Cell   c_src = * G_src.FirstCell();
    typename gtsrc::Vertex v_src = * c_src.FirstVertex();
    gtdst         ::Cell   c_dst = * G_dst.FirstCell();
    gtdst         ::Vertex v_dst = * c_dst.FirstVertex();
    typename gtsrc::Vertex v_src_first = v_src;    

    v_corr[v_src.handle()] = v_dst.handle();
    c_corr[c_src.handle()] = c_dst.handle();
    unsigned cnt = 0;
    //    ordered_src_vertices[cnt] = v_src.handle();
    G_src.switch_vertex(v_src,c_src);
    G_src.switch_cell  (v_src,c_src);
    G_dst.switch_vertex(v_dst,c_dst);
    G_dst.switch_cell  (v_dst,c_dst);
    ++cnt;
    while ( v_src != v_src_first) {
      //ordered_src_vertices[cnt] = v.handle();
      v_corr[v_src.handle()] = v_dst.handle();
      c_corr[c_src.handle()] = c_dst.handle();
      G_src.switch_vertex(v_src,c_src);
      G_src.switch_cell  (v_src,c_src);
      G_dst.switch_vertex(v_dst,c_dst);
      G_dst.switch_cell  (v_dst,c_dst);
      ++cnt;
    }
    // this can happen only if more than 1 component or crossings allowed in G_src!
    if(cnt < G_src.NumOfVertices() -1) {
      ENSURE(false, 
	     "=> G_src is not a simple polygon! cnt=" << cnt 
	     << "  G_src.NumOfVertices()=" <<  G_src.NumOfVertices(), 1);  
    }

  }
  
}

namespace polygon1d {
  inline polygon::VertexIterator polygon::FirstVertex() const { return VertexIterator(*this);}
  inline polygon::CellIterator   polygon::FirstCell()   const { return CellIterator  (*this);}

  inline void polygon::switch_vertex(polygon::Vertex      & v, polygon::Cell const& c) const 
  { 
    v = ( (v == * c.FirstVertex()) 
	  ? Vertex(*this, next_handle(v.handle())) 
	  : Vertex(*this, prev_handle(v.handle())));
  }

  inline void polygon::switch_cell  (polygon::Vertex const& v, polygon::Cell      & c) const
  { 
    c = ( (v == * c.FirstVertex()) 
	  ? Cell(*this, prev_handle(c.handle())) 
	  : Cell(*this, next_handle(c.handle())));
  }

  inline polygon::Vertex polygon::switched_vertex(polygon::Vertex const& v, polygon::Cell const& c) const
  { Vertex res(v); switch_vertex(res,c); return res;}

  inline  polygon::Cell   polygon::switched_cell  (polygon::Vertex const& v, polygon::Cell const& c) const
  { Cell   res(c); switch_cell  (v,res); return res;}

} // namespace polygon1d

#endif
