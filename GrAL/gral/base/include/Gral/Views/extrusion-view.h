#ifndef GRAL_GB_BASE_EXTRUSION_VIEW_H
#define GRAL_GB_BASE_EXTRUSION_VIEW_H



/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"
// for archtypes
#include "Gral/IO/stream-grid-adapter.h"
#include "Gral/Grids/Complex2D/construct.h"

#include "Geometry/point-traits.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include <utility> // pair
#include <vector>

namespace GrAL {

/*! \brief Extrusion of a 2D triangular grid
    \ingroup gridviews

    \see \ref gridviews module
    \see Test in \ref test-extrusion-view.C
*/
namespace extrusion_view {

  /*! \brief Extrusion grid type

      The grid type is a model of $GrAL Cell-VertexInputGridRange.
      Grid functions are defined for vertex and cell types.
   */
  template<class GRID2D>
  class grid;
  template<class GRID2D>
  class prism_subdivision;
  template<class GRID2D>
  class elem_base;
  template<class GRID2D>
  class cell_iterator_t;
  template<class GRID2D>
  class vertex_iterator_t;
  template<class GRID2D>
  class vertex_on_cell_iterator_t;

  /*! \brief Extrusion geometry type

   */
  template<class GEOM2, class F>
  class geometry;

  template<class GRID2D>
  struct grid_types_base : public grid_types_detail::grid_types_root {
    typedef grid<GRID2D> grid_type;
    typedef grid_types<GRID2D> bgt;
    typedef typename bgt::size_type size_type;
    typedef grid_dim_tag<3> dimension_tag;

    // OK only if base grid has (0-base) consecutive integer vertex handle
    // typedef vertex_handle_int<grid_type> vertex_handle;
    // we should really branch on consecutive_tag of base element handles
    typedef  ::std::pair<typename bgt::vertex_handle, int> vertex_handle;
    typedef  ::std::pair<typename bgt::cell_handle,   int> cell_handle;

    typedef cell_iterator_t<GRID2D>   CellIterator;
    typedef CellIterator              Cell;
    typedef vertex_iterator_t<GRID2D> VertexIterator;
    typedef VertexIterator            Vertex;

    typedef  vertex_on_cell_iterator_t<GRID2D> VertexOnCellIterator;

    typedef Complex2D                         archetype_type;
    typedef std::vector<archetype_type>::const_iterator archetype_iterator;
    typedef int                               archetype_handle;
    typedef grid_types<archetype_type>        archgt;
  };


  template<class GRID2D>
  class grid : public grid_types_base<GRID2D> {
    typedef grid_types_base<GRID2D> gt;
    typedef typename gt::bgt        bgt;
  public:
    typedef typename gt::grid_type  grid_type;
    typedef typename gt::size_type  size_type;

    enum { dim = gt::dimension_tag::dim }; 
    typedef grid_view_category_d<dim>       category;

    typedef typename gt::archetype_type     archetype_type;
    typedef typename gt::archetype_iterator archetype_iterator;
    typedef typename gt::archetype_handle   archetype_handle;
  private:
    ref_ptr<GRID2D const>  bg;
    std::vector<archetype_type> the_archetypes;
  public:
    grid() { init(); }
    grid(GRID2D const&         gg) : bg(gg) { init(); }
    grid(ref_ptr<GRID2D const> gg) : bg(gg) { init(); }

    // init archetypes 
    void init();

    archetype_iterator BeginArchetype() const { return the_archetypes.begin();}
    archetype_iterator EndArchetype()   const { return the_archetypes.end();}
    unsigned NumOfArchetypes() const { return the_archetypes.size();}
    archetype_handle handle(archetype_iterator  it) const { return it - BeginArchetype();}
    archetype_type const& Archetype(archetype_handle a) const { return the_archetypes[a];}

    archetype_type const& ArchetypeOf(typename gt::Cell const& c) const { return Archetype(archetype_of(c));}
    archetype_type const& ArchetypeOf(typename gt::cell_handle c) const { return Archetype(archetype_of(c));}
    // we allow only one archetype
    archetype_handle archetype_of(typename gt::Cell const& c) const { return 0;}
    archetype_handle archetype_of(typename gt::cell_handle c) const { return 0;}


    size_type NumOfVertices() const { return 2*BaseGrid()->NumOfVertices();}
    size_type NumOfCells()    const { return 3*BaseGrid()->NumOfCells();}

    ref_ptr<GRID2D const> BaseGrid() const { return bg;}

    typename gt::VertexIterator FirstVertex() const;
    typename gt::VertexIterator EndVertex() const;
    typename gt::CellIterator   FirstCell() const;
    typename gt::CellIterator   EndCell() const;

    typename gt::vertex_handle handle(typename bgt:: vertex_handle bv, int lv)  const { 
      REQUIRE(0<= lv && lv < 2,"",1);
      return  typename gt::vertex_handle(bv,lv);
    }
    typename gt::cell_handle   handle(typename bgt:: cell_handle bc, int lc)  const { 
      REQUIRE(0<= lc && lc < 3,"",1);
      return  typename gt::cell_handle(bc,lc);
    }

  };
  
  template<class I>
  struct tetra {
    I v[4];
  public:
    tetra(I v0, I v1, I v2, I v3) { v[0]=v0;v[1]=v1;v[2]=v2;v[3]=v3;}
    I  operator[](int i) const { return v[i];}
    I& operator[](int i)       { return v[i];}
  };



  template<class GRID2D>
  class elem_base : public grid_types_base<GRID2D> {
  public:  
    typedef grid_types_base<GRID2D> gt;
    typedef grid<GRID2D> grid_type;
    typedef grid_type    anchor_type;
  private:
    ref_ptr<grid_type const> g;
  public:
    elem_base() {}
    elem_base(grid_type const&         gg) : g(gg) {}
    elem_base(ref_ptr<grid_type const> gg) : g(gg) {}

    ref_ptr<grid_type const> TheGrid()   const { cb(); return g;}
    ref_ptr<grid_type const> TheAnchor() const { cb(); return g;}

    bool bound() const { return g != 0;}
    void cb() const { REQUIRE(bound(), "" ,1);}
  };

  template<class GRID2D>
  class prism_subdivision : public elem_base<GRID2D> {
    typedef elem_base<GRID2D> base;
    typedef typename base::gt gt;
    typedef typename gt::bgt  bgt;
    typedef typename gt::grid_type grid_type;

    typename gt::vertex_handle v[6];
    // v[0], v[1], v[2] base triangle
    // v[3], v[4], v[5] top  triangle
    // Invariant: v[0] < v[1] < v[2]
  public:
    using base::TheGrid;
    prism_subdivision() {}

    prism_subdivision(ref_ptr<grid_type const> g, typename bgt::Cell const& tri) : base(g)
    { set_base_triangle(tri);}
    prism_subdivision(grid_type const&         g, typename bgt::Cell const& tri) : base(g)
    { set_base_triangle(tri);}
    void set_base_triangle(typename bgt::Cell const& tri) 
    {
      REQUIRE(tri.NumOfVertices() == 3, "tri.NumOfVertices()=" << tri.NumOfVertices(), 1);
      int i = 0;
      for(typename bgt::VertexOnCellIterator vc(tri); !vc.IsDone(); ++vc, ++i) {
	v[i]   = TheGrid()->handle(vc.handle(), 0);
	v[i+3] = TheGrid()->handle(vc.handle(), 1);
      }
      sort_vertices();
    }
    /*
    prism_subdivision(int v0, int v1, int v2, int v3, int v4, int v5)
    { 
      v[0]=v0;v[1]=v1;v[2]=v2;v[3]=v3;v[4]=v4;v[5]=v5;
      sort_vertices();
    }
    prism_subdivision(int* w) { 
      for(int i = 0; i < 6; ++i) v[i] = w[i];
      sort_vertices();
    }
    */
    // could move this knowledge into static arrays
    typedef tetra<typename gt::vertex_handle> tet_type;
    tet_type tet0() const { return tet_type(v[0], v[3],v[4],v[5]);}
    tet_type tet1() const { return tet_type(v[0], v[1],v[4],v[5]);}
    tet_type tet2() const { return tet_type(v[0], v[1],v[2],v[5]);}
    tet_type tet(int i) const { REQUIRE( ((0 <= i) && (i < 3)),"",1); return (i == 0 ? tet0() : (i == 1 ? tet1() : tet2()));}

    unsigned NumOfCells()    const { return 3;}
    unsigned NumOfVertices() const { return 6;}

  private:
    void sort_vertices() {
      if(v[1] < v[0]) {  ::std::swap(v[0],v[1]);  ::std::swap(v[3],v[4]);}
      if(v[2] < v[1]) {  ::std::swap(v[1],v[2]);  ::std::swap(v[4],v[5]);}
      if(v[1] < v[0]) {  ::std::swap(v[0],v[1]);  ::std::swap(v[3],v[4]);}
      ENSURE((v[0] < v[1] && v[1] < v[2]), "", 1); // "v[0]="<<v[0] << " v[1]=" << v[1] << " v[2]=" << v[2], 1);
    }
  };
  








  template<class GRID2D>
  class vertex_iterator_t : public elem_base<GRID2D> {
  private:
    typedef elem_base<GRID2D>         base;
    typedef vertex_iterator_t<GRID2D> self;
    typedef typename base::bgt        bgt;
  public:
    typedef vertex_type_tag              element_type_tag;
    struct category : 
      virtual grid_vertex_category, 
      virtual grid_vertex_iterator_category {};
    typedef typename base::vertex_handle vertex_handle;
    typedef typename base::grid_type     grid_type;
    typedef self                         value_type;

    typename bgt::VertexIterator v;
    int                          lv;
  public:
    using base::cb;
    using base::bound;
    using base::TheGrid;

    vertex_iterator_t() {}
    vertex_iterator_t(grid_type const&         gg)                   : base(gg), v(* TheGrid()->BaseGrid()), lv(0) {}
    vertex_iterator_t(grid_type const&         gg, vertex_handle vv) : base(gg), v(* TheGrid()->BaseGrid(), vv.first), lv(vv.second) {}    
    vertex_iterator_t(ref_ptr<grid_type const> gg)                   : base(gg), v(* TheGrid()->BaseGrid()), lv(0) {}
    vertex_iterator_t(ref_ptr<grid_type const> gg, vertex_handle vv) : base(gg), v(* TheGrid()->BaseGrid(), vv.first), lv(vv.second) {}

    self& operator++() { cv(); ++lv; if(lv == 2) { lv = 0; ++v;}  return *this;}
    bool IsDone() const { cb(); return v.IsDone();}
    self const& operator*() const { cv(); return *this;}
    vertex_handle handle()  const { cv(); return TheGrid()->handle(v.handle(), lv); }
    typename bgt::Vertex Base() const { return *v;}
    operator typename bgt::Vertex() const { return Base();}

    int level() const { return lv;}
    bool valid() const { return bound() && v.valid() && (0<=lv && lv < 2);}
    void cv()    const { REQUIRE(valid(), "", 1);}
    bool equal_to(self const& rhs) const { return (v == rhs.v && lv == rhs.lv);}
    // bool equal_to(self const& rhs) const { return (v.IsDone() && rhs.v.IsDone()) || (v == rhs.v && lv == rhs.lv);}
 };
    



  template<class GRID2D>
  class cell_iterator_t : public elem_base<GRID2D> {
    friend class vertex_on_cell_iterator_t<GRID2D>;
    typedef elem_base<GRID2D>       base;
    typedef cell_iterator_t<GRID2D> self;
  public:
    typedef cell_type_tag              element_type_tag;
    struct category : 
      virtual grid_cell_category, 
      virtual grid_cell_iterator_category {};

    typedef typename base::grid_type   grid_type;
    typedef typename base::cell_handle cell_handle;
    typedef typename base::archgt      archgt;
    typedef self                       value_type;

  private:
    typedef typename base::bgt bgt;
    typename bgt::CellIterator tri;
    int                        lc;
    prism_subdivision<GRID2D>  sub;
  public:
    using base::cb;
    using base::bound;
    using base::TheGrid;

    cell_iterator_t() {}
    cell_iterator_t(grid_type const&         g) 
      : base(g), tri(* TheGrid()->BaseGrid()), lc(0) { init();}
    cell_iterator_t(ref_ptr<grid_type const> g) 
      : base(g), tri(* TheGrid()->BaseGrid()), lc(0) { init();}
    cell_iterator_t(grid_type const&         g, cell_handle cc) 
      : base(g), tri(* TheGrid()->BaseGrid(), cc.first), lc(cc.second) { init(); }
    cell_iterator_t(ref_ptr<grid_type const> g, cell_handle cc) 
      : base(g), tri(* TheGrid()->BaseGrid(), cc.first), lc(cc.second) { init();}


    self const& operator* () const { cv(); return *this;}
    self&       operator++()       { cv();  advance(); return *this; }
    bool IsDone() const { cb(); return tri.IsDone();}
    cell_handle handle() const { cv(); return cell_handle(tri.handle(), lc);}

    vertex_iterator_t<GRID2D> V(typename archgt::Vertex av) const 
    { return  vertex_iterator_t<GRID2D>(TheGrid(), sub.tet(lc)[av.handle()]);}

    unsigned NumOfVertices() const { cv(); return 4;}
    vertex_on_cell_iterator_t<GRID2D>  FirstVertex() const;
    vertex_on_cell_iterator_t<GRID2D>  EndVertex()   const;

    bool valid() const { return bound() && tri.valid() && (0<=lc && lc < (int)sub.NumOfCells());}
    void cv()    const { REQUIRE(valid(), "", 1);}

    bool equal_to(self const& rhs) const { return tri == rhs.tri && lc == rhs.lc;}
  private:
    void init() { 
      if(!IsDone()) 
	sub = prism_subdivision<GRID2D>(TheGrid(), *tri);
    }
    void advance() {
      ++lc; 
      if(lc == 3) { 
	lc = 0;
	++tri; 
	if(!tri.IsDone())
	  sub.set_base_triangle(*tri);
      } 
    }
  };



  template<class GRID2D>
  class vertex_on_cell_iterator_t : public grid_types_base<GRID2D> {
    typedef grid_types_base<GRID2D>           gt;
    typedef vertex_on_cell_iterator_t<GRID2D> self;
  public:
    typedef typename gt::grid_type            grid_type;
    typedef typename gt::vertex_handle        vertex_handle;
    typedef typename gt::Cell                 anchor_type;
    typedef typename gt::Vertex               value_type;
    typedef grid_incidence_iterator_category_d<0, grid_type::dim> category;
  private:
    typename gt::Cell c;
    int               lv;
  public:

    vertex_on_cell_iterator_t() {}
    vertex_on_cell_iterator_t(typename gt::Cell const& cc, int llv = 0) : c(cc), lv(llv) {}

    vertex_handle handle() const { return vertex_handle(c.sub.tet(c.lc)[lv]);}
    typename gt::Vertex operator*() const { return typename gt::Vertex(TheGrid(), handle());}
    self&  operator++() { cv(); ++lv; return *this;}
    bool IsDone() const { cb(); return lv >= 4;}

    ref_ptr<grid_type   const> TheGrid() const { cb(); return c.TheGrid();}
    ref_ptr<anchor_type const> TheCell() const { cb(); return ref_ptr<typename gt::Cell const>(c);}
    ref_ptr<anchor_type const> TheAnchor() const { return TheCell();}

    bool bound() const { return c.valid();}
    bool valid() const { return bound() && 0 <= lv && lv < 4;}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}

    bool equal_to(self const& rhs) const { return lv == rhs.lv;}
  };

  template<class GRID2D>
  inline bool operator==(vertex_on_cell_iterator_t<GRID2D> const& lhs,
			 vertex_on_cell_iterator_t<GRID2D> const& rhs) { return lhs.equal_to(rhs);}
  template<class GRID2D>
  inline bool operator==(vertex_iterator_t<GRID2D> const& lhs,
			 vertex_iterator_t<GRID2D> const& rhs) { return lhs.equal_to(rhs);}
  template<class GRID2D>
  inline bool operator==(cell_iterator_t<GRID2D> const& lhs,
			 cell_iterator_t<GRID2D> const& rhs) { return lhs.equal_to(rhs);}
#ifndef GRAL_INCLUDES_RELOPS
  template<class GRID2D>
  inline bool operator!=(vertex_on_cell_iterator_t<GRID2D> const& lhs,
			 vertex_on_cell_iterator_t<GRID2D> const& rhs) { return !(lhs == rhs);}
  template<class GRID2D>
  inline bool operator!=(vertex_iterator_t<GRID2D> const& lhs,
			 vertex_iterator_t<GRID2D> const& rhs) { return !(lhs == rhs);}
  template<class GRID2D>
  inline bool operator!=(cell_iterator_t<GRID2D> const& lhs,
			 cell_iterator_t<GRID2D> const& rhs) { return !(lhs == rhs);}
#endif


  template<class GRID2D>
  inline vertex_on_cell_iterator_t<GRID2D> 
  cell_iterator_t<GRID2D>::FirstVertex() const { return vertex_on_cell_iterator_t<GRID2D>(*this);}

  template<class GRID2D>
  inline vertex_on_cell_iterator_t<GRID2D> 
  cell_iterator_t<GRID2D>::EndVertex() const  { return vertex_on_cell_iterator_t<GRID2D>(*this, NumOfVertices());}


  template<class GRID2D>
    void grid<GRID2D>::init() 
    {
      int tet[4*4] = {
	3, 0, 1, 2,
	3, 0, 1, 3,
	3, 0, 2, 3,
	3, 1, 2, 3
      };
      stream_grid_mask<int *> arch(4,4,tet);
      the_archetypes.resize(1);
      ConstructGrid0(the_archetypes[0], arch);
    }

  template<class GRID2D>
  inline typename grid_types_base<GRID2D>::VertexIterator // vertex_iterator_t<GRID2D> 
  grid<GRID2D>::FirstVertex() const { return vertex_iterator_t<GRID2D>(*this);}

  template<class GRID2D>
  inline typename grid_types_base<GRID2D>::VertexIterator // vertex_iterator_t<GRID2D> 
  grid<GRID2D>::EndVertex() const { return vertex_iterator_t<GRID2D>(*this, handle(BaseGrid()->EndVertex().handle(), 0));}
 
  template<class GRID2D>
  inline typename grid_types_base<GRID2D>::CellIterator // cell_iterator_t<GRID2D> 
  grid<GRID2D>::FirstCell() const { return cell_iterator_t<GRID2D>(*this);}

  template<class GRID2D>
  inline typename grid_types_base<GRID2D>::CellIterator // cell_iterator_t<GRID2D> 
  grid<GRID2D>::EndCell() const { return cell_iterator_t<GRID2D>(*this, handle(BaseGrid()->EndCell().handle(), 0));}


} // namespace extrusion_view


template<class GRID2D>
struct grid_types<extrusion_view::grid<GRID2D> > 
  : public grid_types_base<extrusion_view::grid_types_base<GRID2D> >
{
};



template<class GRID2D>
struct element_traits<extrusion_view::vertex_iterator_t<GRID2D> >
  : public element_traits_vertex_base<extrusion_view::grid<GRID2D> > 
{
  typedef element_traits_vertex_base<extrusion_view::grid<GRID2D> > base;

  struct hasher_type {
    typedef extrusion_view::vertex_iterator_t<GRID2D> key_type;
    typedef key_type                                  argument_type;
    typedef size_t                                    result_type;
    size_t operator()(key_type const& e) const {
      typedef element_traits<typename grid_types<GRID2D>::Vertex> bet;
      typename bet::hasher_type bh;
      return bh(e.Base()) * e.handle().second;
    }
  };

  typedef non_consecutive_tag consecutive_tag;
};


template<class GRID2D>
struct element_traits<extrusion_view::cell_iterator_t<GRID2D> >
  : public element_traits_cell_base<extrusion_view::grid<GRID2D> > 
{
  typedef element_traits_cell_base<extrusion_view::grid<GRID2D> > base;

  struct hasher_type {
    typedef extrusion_view::cell_iterator_t<GRID2D> key_type;
    typedef key_type                                  argument_type;
    typedef size_t                                    result_type;
    size_t operator()(key_type const& e) const {
      typedef element_traits<typename grid_types<GRID2D>::Cell> bet;
      typename bet::hasher_type bh;
      return bh(e.Base()) * e.handle().second;
    }
  };

  typedef non_consecutive_tag consecutive_tag;
};


  namespace extrusion_view {

#define gt grid_types<grid<GRID2D> >

    template<class GRID2D>
    inline typename gt::VertexIterator
    gral_begin(grid<GRID2D> const&g, vertex_iterator_t<GRID2D> ) { return g.FirstVertex();}
    //  gral_begin(grid<GRID2D> const&g,  typename gt::VertexIterator) { return g.FirstVertex();}
   
    template<class GRID2D>
    inline typename gt::VertexIterator
    gral_end  (grid<GRID2D> const&g,  vertex_iterator_t<GRID2D>) { return g.EndVertex();}
    // gral_end  (grid<GRID2D> const&g,  typename gt::VertexIterator) { return g.EndVertex();}
 
    template<class GRID2D>
    inline typename gt::size_type
    gral_size  (grid<GRID2D> const&g,  vertex_iterator_t<GRID2D>) { return g.NumOfVertices();}
    //  gral_size  (grid<GRID2D> const&g,  typename gt::VertexIterator) { return g.NumOfVertices();}

    template<class GRID2D>
    inline typename gt::CellIterator
    gral_begin(grid<GRID2D> const&g,  cell_iterator_t<GRID2D> ) { return g.FirstCell();}
   
    template<class GRID2D>
    inline typename gt::CellIterator
    gral_end  (grid<GRID2D> const&g,  cell_iterator_t<GRID2D> ) { return g.EndCell();}
 
    template<class GRID2D>
    inline typename gt::size_type
    gral_size  (grid<GRID2D> const&g, cell_iterator_t<GRID2D> ) { return g.NumOfCells();}

    template<class GRID2D>
    inline typename gt::VertexOnCellIterator
    gral_begin(cell_iterator_t<GRID2D> const&g,  vertex_on_cell_iterator_t<GRID2D>  ) { return g.FirstVertex();}
   
    template<class GRID2D>
    inline typename gt::VertexOnCellIterator
    gral_end  (cell_iterator_t<GRID2D> const&g,  vertex_on_cell_iterator_t<GRID2D>  ) { return g.EndVertex();}
 
    template<class GRID2D>
    inline typename gt::size_type
    gral_size  (cell_iterator_t<GRID2D> const&g, vertex_on_cell_iterator_t<GRID2D>  ) { return g.NumOfVertices();}

#undef gt

  } // namespace extrusion_view

template<class GRID2D, class T>
class grid_function<extrusion_view::vertex_iterator_t<GRID2D>, T>
  : public grid_function_hash<extrusion_view::vertex_iterator_t<GRID2D>, T> 
{

  typedef grid_function_hash<extrusion_view::vertex_iterator_t<GRID2D>,T> base;
public:
  typedef extrusion_view::grid<GRID2D> grid_type;
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


namespace extrusion_view {

  /*! \brief A geometry type for extruded grids

      \templateparam
       - \c GEOM2D grid geometry
       - \c F mapping from \c GEOM2D::Vertex to \c result_type

     \see Tested in test-extrusion-view.C
  */  
  template<class GEOM2D, class F>
  class geometry {
 public:
    typedef grid_geometry_category             category;
    typedef GEOM2D                             base_geom_type;
    typedef typename base_geom_type::grid_type base_grid_type;
    typedef grid_types<base_grid_type>         bgt;
    typedef grid<base_grid_type>  grid_type;
    typedef grid_types<grid_type> gt;

    typedef F mapping_type;
    typedef typename F::result_type coord_type;
    typedef point_traits<coord_type> pt;
    typedef typename pt::component_type scalar_type;

    enum extrusion_dir { symmetric, keep_original };
  private:
    ref_ptr<grid_type const>      g;
    ref_ptr<base_geom_type const> bgeom;
    ref_ptr<mapping_type const>   f;

    //  coord_type delta;
    extrusion_dir dir; // if dir = symmetric, original vertices are moved into the opposite direction
  public:
    geometry() {}
    geometry(grid_type const& gg, base_geom_type const& geo, 
	     mapping_type const& ff, extrusion_dir d = keep_original) 
      : g(gg), bgeom(geo), f(ff), dir(d) {}

    ref_ptr<grid_type const> TheGrid() const { return g;}
    unsigned space_dimension() const { return pt::Dim(coord(* TheGrid()->FirstVertex()));}
    coord_type coord(typename gt::Vertex const& v) const 
    { 
      typedef point_traits<typename base_geom_type::coord_type> pt2;
      typename base_geom_type::coord_type cv = (bgeom->coord(v.Base()));
      coord_type res(pt2::x(cv), pt2::y(cv), pt2::z(cv));
      scalar_type delta = (dir==keep_original ? v.level() : v.level() - 0.5); 
      return res + delta * (*f)(v);
    }
  };

} // namespace extrusion_view

} // namespace GrAL 

#endif
