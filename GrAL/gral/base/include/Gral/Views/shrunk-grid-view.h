#ifndef GRAL_GB_SHRUNK_GRID_VIEW_H
#define GRAL_GB_SHRUNK_GRID_VIEW_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"

#include "Geometry/point-traits.h"

#include <iostream>


/*! \defgroup shrunk_grid_view A shrunk view of a grid

    \ingroup gridviews
    \see gridviews module

    A shrunk (or imploded) view of a grid can be useful 
    for visualizing grid interior.

    Typical usage example:
    \code
    grid_type G;
    geom_type GeomG(G);
    // ...
    namespace sgv = shrink_grid_view;
    sgv::grid_view<grid_type>           ShrunkG(G);
    sgv::geom_view<grid_type,geom_type> ShrunkGeom(ShrunkG, GeomG);
    ShrunkGeom.set_shrink_factor(0.5);

    Require(ShrunkG.NumOfCells() == G.NumOfCells(), "", 1);
    \endcode

    \see test-shrunk-grid.C
 */

/*! \brief An ``exploded'' view of a grid

    \ingroup shrunk_grid_view

    Combinatorially, each vertex gets copied for each incident cell.

    Geometrically, each cell is shrunk towards its center.
 */
namespace shrink_grid_view {



  template<class GRID>
    class grid_view;

  template<class GRID>
    class cell_iterator;

  template<class GRID>
    class vertex_iterator;
      
  template<class GRID>
  class vertex_on_cell_iterator;

  template<class GRID>
    class vertex_handle_t;

  template<class GRID>
    class local_grid_types {
      typedef grid_view<GRID>        grid_type;
      typedef cell_iterator<GRID>    Cell;
      typedef cell_iterator<GRID>    CellIterator;
      typedef vertex_iterator<GRID>  Vertex;
      typedef vertex_iterator<GRID>  VertexIterator;
      typedef vertex_on_cell_iterator<GRID> VertexOnCellIterator;

      typedef grid_types<GRID>  bgt;
      typedef GRID              base_grid_type;

      typedef typename bgt::cell_handle cell_handle;
      typedef vertex_handle_t<GRID>     vertex_handle;

      typedef typename bgt::dimension_tag     dimension_tag;
      typedef typename bgt::archetype_type    archetype_type;
      typedef typename bgt::archetype_handle  archetype_handle;
      typedef typename bgt::ArchetypeIterator ArchetypeIterator;
      typedef typename bgt::archgt            archgt;
    };


  template<class GRID, class element_tag>
    struct map_types_for_grid_view { };

  template<class GRID>
    struct map_types_for_grid_view<GRID, vertex_type_tag> {
      typedef shrink_grid_view::vertex_iterator<GRID> element_type;
    };

  template<class GRID>
    struct map_types_for_grid_view<GRID, cell_type_tag> {
      typedef shrink_grid_view::cell_iterator<GRID> element_type;
    };


  /*! \brief The combinatorial shrunk view
   */
  template<class GRID>
    class grid_view : public local_grid_types<GRID> {
    private:
      base_grid_type const* g; // ref

      mutable int  num_of_vertices;
      mutable bool num_of_vertices_valid;
    public:
      grid_view(base_grid_type const& gg) 
	: g(&gg), num_of_vertices(0), num_of_vertices_valid(false) {}

      unsigned NumOfCells() const { return BaseGrid().NumOfCells();}
      unsigned NumOfVertices() const {
	if(! num_of_vertices_valid)
	  calculate_num_of_vertices();
	return num_of_vertices;
      }
      cell_iterator<GRID>   FirstCell()   const;
      vertex_iterator<GRID> FirstVertex() const;
      base_grid_type const& BaseGrid() const { return *g;}

      template<class element_tag>
      struct map_types : public map_types_for_grid_view<GRID,element_tag> {};

      ArchetypeIterator BeginArchetype() const 
	{ return BaseGrid().BeginArchetype();}  
      ArchetypeIterator EndArchetype() const 
	{ return BaseGrid().EndArchetype();}
      archetype_type const& ArchetypeOf(Cell const& c) const
	{ return BaseGrid().ArchetypeOf(c.BaseCell()); }
      archetype_handle archetype_of(Cell const& c) const
	{ return BaseGrid().archetype_of(c.BaseCell());}
      archetype_handle archetype_of(cell_handle c) const
	{ return BaseGrid().archetype_of(c);}

    private:
      void calculate_num_of_vertices() const {
	num_of_vertices = 0;
	for(typename bgt::CellIterator c(BaseGrid()); ! c.IsDone(); ++c)
	  num_of_vertices += (*c).NumOfVertices();
	num_of_vertices_valid = true;
      }


    };

 template<class GRID>
   struct vertex_handle_t : public local_grid_types<GRID> {
     typedef typename bgt::vertex_handle base_vertex_handle;

     cell_handle        c;
     base_vertex_handle v;
   public:
     vertex_handle_t() {}
     vertex_handle_t(cell_handle cc, base_vertex_handle vv) : c(cc), v(vv) {}
   };

 template<class GRID>
   inline
   ostream& operator<<(ostream& out, vertex_handle_t<GRID> const& h)
   { return (out << h.c << ' ' << h.v);}

  template<class GRID>
    class cell_iterator : public local_grid_types<GRID> {
    private:
      typedef cell_iterator<GRID> self; 
      friend class vertex_on_cell_iterator<GRID>;

      grid_type  const*          g;
      typename bgt::CellIterator c;
    public:
      cell_iterator() : g(0) {}
      explicit
      cell_iterator(grid_type const& gg) : g(&gg), c(g->BaseGrid()) {}
      cell_iterator(grid_type const& gg,
		    cell_handle      cc) : g(&gg), c(g->BaseGrid(), cc) {}
      self      & operator++()      { ++c; return *this;}
      self const& operator*() const { return *this;}
      bool IsDone() const { return c.IsDone();}
      cell_handle handle() const { return c.handle();}

      grid_type const& TheGrid() const { return *g;}
     
      // Vertex-On-Cell Iteration
      vertex_on_cell_iterator<GRID> FirstVertex() const;
      unsigned NumOfVertices() const { return c.NumOfVertices();}
      vertex_iterator<GRID> V(typename archgt::Vertex av) const
	{ return Vertex(*g, vertex_handle(handle(), BaseCell().v(av.handle()) )); }
      //{ return Vertex(*this); } // *g, typename bgt::VertexOnCellIterator(c,av.handle())); }


      bool operator==(self const& rhs) const { return c == rhs.c;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}

      bool bound() const { return (g!= 0);}
      bool valid() const { return bound() && c.valid();}

      typename bgt::Cell const& BaseCell() const { return *c;}
      typename bgt::Cell const& Base    () const { return *c;}
    };
  

  template<class GRID>
    class vertex_iterator : public local_grid_types<GRID> {
    private:
      typedef vertex_iterator<GRID> self;

      grid_type const* g;
      typename bgt::CellIterator c;
      typename bgt::VertexOnCellIterator vc;
    public:
      vertex_iterator() {}
      explicit
      vertex_iterator(grid_type const& gg) 
	: g(&gg), c(g->BaseGrid().FirstCell())
	{ if (!c.IsDone()) vc = c.FirstVertex(); }
      vertex_iterator(grid_type const& gg, 
		      vertex_handle h) 
	: g(&gg), c(gg.BaseGrid(),h.c) 
	{
	  vc = c.FirstVertex();
	  while(vc.handle() != h.v) ++vc;
	  REQUIRE(vc.handle() == h.v, 
		  "invalid handle h = " << h << '\n' ,1);
	}
      explicit
      vertex_iterator(vertex_on_cell_iterator<GRID> const& vvc);

      self& operator++() { 
	++vc;
	if(vc.IsDone()) {
	  ++c;
	  if(! c.IsDone())
	    vc = c.FirstVertex();
	}
	return *this;
      }
      self const& operator*() const { return *this;}
      bool IsDone() const { return c.IsDone();}
      vertex_handle handle() const 
	{ return vertex_handle(c.handle(),vc.handle());}
      grid_type const& TheGrid() const { return *g;}

      bool operator==(self const& rhs) const { return vc == rhs.vc;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}


      Cell Cell_()  const { return Cell(*g,c.handle());}
      typename bgt::Vertex BaseVertex() const { return *vc;}
      typename bgt::Vertex Base()       const { return *vc;}
    };


  template<class GRID>
    class vertex_on_cell_iterator : public local_grid_types<GRID> {
      typedef vertex_on_cell_iterator<GRID> self;
    private:
      grid_type const* g;
      typename bgt::VertexOnCellIterator vc;
    public: 
      vertex_on_cell_iterator() : g(0) {}
      explicit
      vertex_on_cell_iterator(Cell const& c) 
	: g(&(c.TheGrid())), vc(c.c.FirstVertex()) {} 

      self& operator++() { ++vc; return *this;}
      Vertex operator*() const { return Vertex(*g,handle());}
      vertex_handle handle() const 
	{ return vertex_handle(vc.TheCell().handle(),
			       vc.handle());}
      bool IsDone() const { return vc.IsDone();}

      grid_type const& TheGrid() const { return *g;}

      bool operator==(self const& rhs) const { return vc == rhs.vc;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}

    };


  template<class GRID>
    inline
    vertex_on_cell_iterator<GRID> 
    cell_iterator<GRID>::FirstVertex() const 
    { return VertexOnCellIterator(*this);}


  /*
   template<class GRID>
     inline
     vertex_iterator<GRID> 
     cell_iterator<GRID>::V(typename archgt::Vertex av) const
     { return Vertex(*g, vertex_handle(handle(), BaseCell().v(av.handle()) )); }
  */ 
  // *g, typename bgt::VertexOnCellIterator(c,av.handle())); }

  template<class GRID>
    inline
    vertex_iterator<GRID> 
    grid_view<GRID>::FirstVertex() const { return VertexIterator(*this);}

  template<class GRID>
    inline
    cell_iterator<GRID> 
    grid_view<GRID>::FirstCell() const { return CellIterator(*this);}




  /*! \brief The geometric shrunk view

   */
  template<class GRID, class GEOM>
    class geom_view : public local_grid_types<GRID> {
      typedef GEOM base_geom_type;
    public:
      typedef typename base_geom_type::coord_type coord_type;
      typedef point_traits<coord_type> pt;
    private:
      grid_type      const* g;
      base_geom_type const* geom;
      double t; // shrink factor, 1 = no shrink, 0 = shrink cell to point (cell center)
    public:
      geom_view(grid_type const& gg,
		base_geom_type const& ggeom,
		double tt = 1.0) 
	: g(&gg), geom(&ggeom), t(tt) {}
      void set_shrink_factor(double tt) { t = tt;}
      
      coord_type coord(Vertex const& v) const 
	{ 
	  coord_type ctr = barycenter(v.Cell_());
	  return ctr + t*(geom->coord(v.BaseVertex()) - ctr);
	}

      coord_type barycenter(Cell const& c) const {
	int nv = c.NumOfVertices();
	coord_type sum(pt::Origin());
	for(VertexOnCellIterator vc(c); ! vc.IsDone(); ++vc)
	  sum += geom->coord((*vc).BaseVertex());
	return sum * (1.0/(double)nv);
      }
  };

}


template<class GRID>
class grid_types<shrink_grid_view::grid_view<GRID> > :
  public shrink_grid_view::local_grid_types<GRID> {};

template<class GRID>
struct element_traits<shrink_grid_view::vertex_iterator<GRID> >
  : public element_traits_vertex_base<shrink_grid_view::grid_view<GRID> > 
{
  typedef shrink_grid_view::vertex_handle_t<GRID> element_handle;
  struct hasher_type {
    unsigned operator()(element_handle const&  v) const 
      { return (8*v.c + v.v);}
    unsigned operator()(element_type const&  v) const 
      { return (*this)(v.handle());}
  };
};


template<class GRID>
struct element_traits<shrink_grid_view::cell_iterator<GRID> >
  : public element_traits_cell_base<shrink_grid_view::grid_view<GRID> > 
{
  typedef shrink_grid_view::local_grid_types<GRID> lgt; // gt of view
  typedef typename lgt::bgt                        bgt; // gt of GRID
  typedef typename lgt::cell_handle                element_handle;
  typedef element_traits<typename bgt::Cell>       bet;
  typedef typename bet::hasher_type                base_hasher_type;

  struct hasher_type {
    unsigned operator()(element_handle const&  v) const 
      { base_hasher_type h; return h(v);}
    unsigned operator()(element_type const&  v) const 
      { return (*this)(v.handle());}
  };
};



template<class GRID, class T>
class grid_function<shrink_grid_view::vertex_iterator<GRID>, T>
  : public grid_function_hash<shrink_grid_view::vertex_iterator<GRID>, T>
{
  typedef grid_function_hash<shrink_grid_view::vertex_iterator<GRID>, T> 
  base;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g,
		T         const& t0) : base(g,t0) {}
};


namespace shrink_grid_view {
  /*! \brief adapter for grid functions on base grid
      
       This adapter forwards accesses through elements of 
       shrink_grid_view::grid_view to accesses through their base
       elements to the adaptee.


   */

  template<class GF>
    class grid_function_view {
    private:
      typedef typename GF::grid_type    base_grid_type;
      typedef typename GF::element_type base_element_type;
      typedef element_traits<base_element_type> bet;
      typedef typename bet::element_type_tag    btag;
    public:
      typedef typename GF::value_type   value_type;
      typedef grid_view<base_grid_type> grid_type;
      typedef typename grid_type::template map_types<btag> map_types;
      typedef typename map_types::element_type element_type;
      typedef element_traits<element_type> et;
    private:
      grid_view<base_grid_type> const* g;
      GF                        const* gf;
    public:
      grid_function_view(grid_type const& g_,
			 GF        const& gf_) : g(&g_), gf(&gf_) {}

      value_type const& operator()(element_type const& e) const { return (*gf)(e.Base());}
      grid_type  const& TheGrid() const { return *g;}
      size_t             size() const { return et::size(*g);}
    };

};

#endif
