#ifndef GRAL_GB_SHRUNK_GRID_VIEW_H
#define GRAL_GB_SHRUNK_GRID_VIEW_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"

#include "Geometry/point-traits.h"

#include <iostream>

namespace GrAL {

/*! \defgroup shrunk_grid_view A shrunk view of a grid

    \ingroup gridviews

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

    \see Test in \ref test-shrunk-grid.C
    \see \ref gridviews module

    \todo add Edge and Facet elements
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
  class local_grid_types : public grid_types_detail::grid_types_root {
  public:
      typedef grid_view<GRID>        grid_type;
      typedef cell_iterator<GRID>    Cell;
      typedef cell_iterator<GRID>    CellIterator;
      typedef vertex_iterator<GRID>  Vertex;
      typedef vertex_iterator<GRID>  VertexIterator;
      typedef vertex_on_cell_iterator<GRID> VertexOnCellIterator;

      typedef grid_types<GRID>  bgt;
      typedef GRID              base_grid_type;
    typedef typename bgt::size_type size_type;
    typedef typename bgt::dimension_tag dimension_tag;
    
      typedef typename bgt::cell_handle cell_handle;
      typedef vertex_handle_t<GRID>     vertex_handle;

      typedef typename bgt::archetype_type     archetype_type;
      typedef typename bgt::archetype_handle   archetype_handle;
      typedef typename bgt::archetype_iterator archetype_iterator;
      typedef typename bgt::archgt             archgt;
    };


  /*  helper class to map element types of base grid to corresponding
      types of grid_view. 

      Repeated in grid_view as map_types.
   */
  namespace detail {
    template<class GRID, class element_tag>
      struct map_types_for_grid_view { };
    
    template<class GRID>
      struct map_types_for_grid_view<GRID, vertex_type_tag> {
	typedef shrink_grid_view::vertex_iterator<GRID> element_type;
	typedef shrink_grid_view::vertex_iterator<GRID> element_iterator;
      };
    
    template<class GRID>
      struct map_types_for_grid_view<GRID, cell_type_tag> {
	typedef shrink_grid_view::cell_iterator<GRID> element_type;
	typedef shrink_grid_view::cell_iterator<GRID> element_iterator;
      };
  } // namespace detail

  /*! \brief The combinatorial shrunk view

    \todo branch in grid_view::NumOfVertices() depending on availability
    of \c GRID::CellOnVertexIterator.
   */
  template<class GRID>
    class grid_view : public local_grid_types<GRID> {
      typedef grid_view<GRID>        self;
      typedef local_grid_types<GRID> gt;
    public:
      typedef self grid_type;
      enum { dim = gt::dimension_tag::dim };
      typedef grid_view_category_d<dim>       category;

      typedef typename gt::archetype_iterator archetype_iterator;
      typedef typename gt::archetype_type     archetype_type;
      typedef typename gt::archetype_handle   archetype_handle;
      typedef typename gt::base_grid_type     base_grid_type;
      typedef typename gt::Cell               Cell;
      typedef typename gt::cell_handle        cell_handle;
      typedef typename gt::bgt                bgt;
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
      cell_iterator<GRID>   EndCell()   const;
      vertex_iterator<GRID> EndVertex() const;
      base_grid_type const& BaseGrid() const { return *g;}


      template<class element_tag>
      struct map_types : public detail::map_types_for_grid_view<GRID,element_tag> {};

      archetype_iterator BeginArchetype() const 
	{ return BaseGrid().BeginArchetype();}  
      archetype_iterator EndArchetype() const 
	{ return BaseGrid().EndArchetype();}
      archetype_handle   handle(archetype_iterator it) const { return BaseGrid().handle(it);}

      archetype_type const& Archetype(archetype_handle a) const { return BaseGrid().Archetype(a);}
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
     typedef local_grid_types<GRID> base;
     typedef typename base::bgt bgt;
     typedef typename bgt::vertex_handle base_vertex_handle;
     typedef typename base::cell_handle  cell_handle;
     typedef grid_vertex_handle_category category;

     cell_handle        c;
     base_vertex_handle v;
   public:
     vertex_handle_t() {}
     vertex_handle_t(cell_handle cc, base_vertex_handle vv) : c(cc), v(vv) {}
   };

 template<class GRID>
   inline
    ::std::ostream& operator<<( ::std::ostream& out, vertex_handle_t<GRID> const& h)
   { return (out << h.c << ' ' << h.v);}

  //---- elements and sequence  iterators -----------
  
  template<class GRID>
    class cell_iterator : public local_grid_types<GRID> {
    private:
      typedef cell_iterator<GRID>    self; 
      typedef local_grid_types<GRID> gt;
      friend class vertex_on_cell_iterator<GRID>;
    public:
      typedef typename gt::bgt           bgt;
      typedef typename gt::archgt        archgt;
      typedef typename gt::grid_type     grid_type;
      typedef typename gt::cell_handle   cell_handle;
      typedef typename gt::vertex_handle vertex_handle;
      typedef typename gt::Vertex        Vertex;

      typedef cell_type_tag element_type_tag;
      struct category : 
	virtual grid_cell_category,
	virtual grid_cell_iterator_category {};

      typedef grid_type anchor_type;
      typedef self      value_type;
   private:
      grid_type  const*          g;
      typename bgt::CellIterator c;
    public:
      cell_iterator() : g(0) {}
      explicit
      cell_iterator(grid_type const& gg) : g(&gg), c(g->BaseGrid()) {}
      cell_iterator(grid_type const& gg,
		    cell_handle      cc) : g(&gg), c(g->BaseGrid(), cc) {}
      cell_iterator(grid_type const& gg,
		    typename bgt::CellIterator bc) : g(&gg), c(bc) {}
      self      & operator++()      { ++c; return *this;}
      self const& operator*() const { return *this;}
      bool IsDone() const { return c.IsDone();}
      cell_handle handle() const { return c.handle();}

      grid_type const& TheGrid()   const { return *g;}
      grid_type const& TheAnchor() const { return *g;}
     
      // Vertex-On-Cell Iteration
      vertex_on_cell_iterator<GRID> FirstVertex() const;
      vertex_on_cell_iterator<GRID> EndVertex()   const;
      unsigned NumOfVertices() const { return c.NumOfVertices();}
      vertex_iterator<GRID> V(typename archgt::Vertex av) const
	{ return Vertex(*g, vertex_handle(handle(), BaseCell().v(av.handle()) )); }
      //{ return Vertex(*this); } // *g, typename bgt::VertexOnCellIterator(c,av.handle())); }


      bool operator==(self const& rhs) const { return c == rhs.c;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}
      bool operator< (self const& rhs) const { return c <  rhs.c;}

      bool bound() const { return (g!= 0);}
      bool valid() const { return bound() && c.valid();}

      typename bgt::Cell const& BaseCell() const { return *c;}
      typename bgt::Cell const& Base    () const { return *c;}
    };
  

  template<class GRID>
    class vertex_iterator : public local_grid_types<GRID> {
    private:
      typedef vertex_iterator<GRID>  self;
      typedef local_grid_types<GRID> gt;
    public:
      typedef typename gt::bgt           bgt;
      typedef typename gt::archgt        archgt;
      typedef typename gt::grid_type     grid_type;
      typedef typename gt::cell_handle   cell_handle;
      typedef typename gt::vertex_handle vertex_handle;
      typedef typename gt::Cell          Cell;

      typedef vertex_type_tag element_type_tag;
      struct category : 
	virtual grid_vertex_category,
	virtual grid_vertex_iterator_category {};
      typedef grid_type anchor_type;
      typedef self      value_type;
    private:

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
		      typename bgt::CellIterator bc)
	: g(&gg), c(bc)
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
      grid_type const& TheGrid()   const { return *g;}
      grid_type const& TheAnchor() const { return *g;}

      bool operator==(self const& rhs) const { return (c == rhs.c && IsDone())  || (c == rhs.c && vc == rhs.vc);}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}
      bool operator< (self const& rhs) const { return c < rhs.c || (c == rhs.c && !IsDone() && vc < rhs.vc);}

      Cell Cell_()  const { return Cell(*g,c.handle());}
      typename bgt::Vertex BaseVertex() const { return *vc;}
      typename bgt::Vertex Base()       const { return *vc;}
    };

  //--------------- incidence iterators -----------------------

  template<class GRID>
    class vertex_on_cell_iterator : public local_grid_types<GRID> {
      typedef vertex_on_cell_iterator<GRID> self;
      typedef local_grid_types<GRID>        gt;
    public:
      typedef typename gt::bgt           bgt;
      typedef typename gt::grid_type     grid_type;
      typedef typename gt::vertex_handle vertex_handle;
      typedef typename gt::Vertex        Vertex;
      typedef typename gt::Cell          Cell;

      typedef grid_incidence_iterator_category_d<0,grid_type::dim> category;
      typedef Vertex value_type;
      typedef Cell   anchor_type;
    private:
      grid_type const* g;
      typename bgt::VertexOnCellIterator vc;
    public: 
      vertex_on_cell_iterator() : g(0) {}
      explicit
      vertex_on_cell_iterator(Cell const& c) 
	: g(&(c.TheGrid())), vc(c.c.FirstVertex()) {} 
      vertex_on_cell_iterator(Cell const& c, typename bgt::VertexOnCellIterator vvc)
	: g(&c.TheGrid()), vc(vvc) {}

      self& operator++() { ++vc; return *this;}
      Vertex operator*() const { return Vertex(*g,handle());}
      vertex_handle handle() const 
	{ return vertex_handle(vc.TheCell().handle(),
			       vc.handle());}
      bool IsDone() const { return vc.IsDone();}

      grid_type const& TheGrid() const { return *g;}
      Cell             TheAnchor() const { return Cell(TheGrid(), vc.TheCell());}

      bool operator==(self const& rhs) const { return vc == rhs.vc;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}
      bool operator< (self const& rhs) const { return vc < rhs.vc;}

    };


  //------------- inline functions -----------------------

  template<class GRID>
  inline vertex_on_cell_iterator<GRID> 
  cell_iterator<GRID>::FirstVertex() const 
  { return typename gt::VertexOnCellIterator(*this);}

  template<class GRID>
  inline vertex_on_cell_iterator<GRID> 
  cell_iterator<GRID>::EndVertex() const 
  { return typename gt::VertexOnCellIterator(*this, GrAL::end<typename bgt::Vertex>(Base()));}


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
    grid_view<GRID>::FirstVertex() const { return typename gt::VertexIterator(*this);}

  template<class GRID>
    inline
    cell_iterator<GRID> 
    grid_view<GRID>::FirstCell() const { return typename gt::CellIterator(*this);}

  template<class GRID>
    inline
    vertex_iterator<GRID> 
    grid_view<GRID>::EndVertex() const { return typename gt::VertexIterator(*this, BaseGrid().EndCell());}

  template<class GRID>
    inline
    cell_iterator<GRID> 
    grid_view<GRID>::EndCell() const { return typename gt::CellIterator(*this, BaseGrid().EndCell());}



  //---------------- grid geometry view ---------------------

  /*! \brief The geometric shrunk view

   */
  template<class GRID, class GEOM>
    class geom_view : public local_grid_types<GRID> {
      typedef GEOM                   base_geom_type;
      typedef local_grid_types<GRID> gt;
    public:
      typedef grid_geometry_category              category;
      typedef typename base_geom_type::coord_type coord_type;
      typedef point_traits<coord_type>            pt;
      typedef typename gt::bgt                    bgt;
      typedef typename gt::grid_type              grid_type;
      typedef typename gt::vertex_handle          vertex_handle;
      typedef typename gt::Vertex                 Vertex;
      typedef typename gt::Cell                   Cell;
      typedef typename gt::VertexOnCellIterator   VertexOnCellIterator;

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

} // namespace shrink_grid_view



//----- classes in global scope: grid_types<>, element_traits<>, grid_function<>  ---------


template<class GRID>
class grid_types<shrink_grid_view::grid_view<GRID> > :
  public grid_types_base<shrink_grid_view::local_grid_types<GRID> > {};


template<class GRID>
struct element_traits<shrink_grid_view::vertex_iterator<GRID> >
  : public element_traits_vertex_base<shrink_grid_view::grid_view<GRID> > 
{
  typedef element_traits_vertex_base<shrink_grid_view::grid_view<GRID> >  base;
  typedef typename base::element_type   element_type;
  typedef typename base::handle_type    handle_type;

  struct hasher_type : public base::hasher_type_elem_base {
    unsigned operator()(handle_type const&  v) const 
      { return (8*v.c + v.v);}
    unsigned operator()(element_type const&  v) const 
      { return (*this)(v.handle());}
  };
};


template<class GRID>
struct element_traits<shrink_grid_view::cell_iterator<GRID> >
  : public element_traits_cell_base<shrink_grid_view::grid_view<GRID> > 
{
  typedef element_traits_cell_base<shrink_grid_view::grid_view<GRID> >  base;
  typedef typename base::element_type   element_type;
  typedef typename base::handle_type    handle_type;

  typedef shrink_grid_view::local_grid_types<GRID> lgt; // gt of view
  typedef typename lgt::bgt                        bgt; // gt of GRID
  typedef element_traits<typename bgt::Cell>       bet;
  typedef typename bet::hasher_type                base_hasher_type;

  struct hasher_type : public base::hasher_type_elem_base {

    unsigned operator()(handle_type const&  v) const 
      { base_hasher_type h; return h(v);}
    unsigned operator()(element_type const&  v) const 
      { return (*this)(v.handle());}
  };
};


  namespace shrink_grid_view {
#define gt grid_types<grid_view<GRID> >

    template<class GRID>
    typename gt::VertexIterator gral_begin(grid_view<GRID> const& g, typename gt::VertexIterator)
    { return g.FirstVertex();}

    template<class GRID>
    typename gt::VertexIterator gral_end  (grid_view<GRID> const& g, typename gt::VertexIterator)
    { return g.EndVertex();}

    template<class GRID>
    typename gt::size_type      gral_size (grid_view<GRID> const& g, typename gt::VertexIterator)
    { return g.NumOfVertices();}

    template<class GRID>
    typename gt::CellIterator gral_begin(grid_view<GRID> const& g, typename gt::CellIterator)
    { return g.FirstCell();}

    template<class GRID>
    typename gt::CellIterator gral_end  (grid_view<GRID> const& g, typename gt::CellIterator)
    { return g.EndCell();}

    template<class GRID>
    typename gt::size_type      gral_size (grid_view<GRID> const& g, typename gt::CellIterator)
    { return g.NumOfCells();}


    template<class GRID>
    typename gt::VertexOnCellIterator 
    gral_begin(cell_iterator<GRID> const& c, typename gt::VertexOnCellIterator)
    { return c.FirstVertex();}

    template<class GRID>
    typename gt::VertexOnCellIterator 
    gral_end  (cell_iterator<GRID> const& c, typename gt::VertexOnCellIterator)
    { return c.EndVertex();}

    template<class GRID>
    typename gt::size_type      
    gral_size (cell_iterator<GRID> const& c, typename gt::VertexOnCellIterator)
    { return c.NumOfVertices();}

    
#undef gt
    
  } // namespace shrink_grid_view


template<class GRID, class T>
class grid_function<shrink_grid_view::vertex_iterator<GRID>, T>
  : public grid_function_hash<shrink_grid_view::vertex_iterator<GRID>, T>
{
  typedef grid_function_hash<shrink_grid_view::vertex_iterator<GRID>, T>  base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g,
		T         const& t0) : base(g,t0) {}
};

/*! 
   \todo This could reuse underlying grid function on base cells
 */
template<class GRID, class T>
class grid_function<shrink_grid_view::cell_iterator<GRID>, T>
  : public grid_function_hash<shrink_grid_view::cell_iterator<GRID>, T>
{
  typedef grid_function_hash<shrink_grid_view::cell_iterator<GRID>, T>  base;
public:
  typedef typename base::grid_type grid_type;

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
      typedef grid_function_view<GF>    self;
      typedef typename GF::grid_type    base_grid_type;
      typedef typename GF::element_type base_element_type;
      typedef element_traits<base_element_type> bet;
      typedef typename bet::element_type_tag    btag;
      typedef typename GF::const_iterator       base_const_iterator;
    public:
      typedef grid_function_view_category<typename category<GF>::type>  category;
      typedef typename GF::value_type      value_type;
      typedef typename GF::reference       reference;
      typedef typename GF::const_reference const_reference;
      typedef typename GF::size_type       size_type;

      typedef grid_view<base_grid_type>    grid_type;
      typedef typename grid_type::template map_types<btag> map_types;
      typedef typename map_types::element_type     element_type;
      typedef typename map_types::element_iterator element_iterator;
      typedef element_traits<element_type> et;
    private:
      grid_view<base_grid_type> const* g;
      GF                        const* gf;
    public:
      grid_function_view(grid_type const& g_,
			 GF        const& gf_) : g(&g_), gf(&gf_) {}

      const_reference   operator()(element_type const& e) const { return (*gf)(e.Base());}
      grid_type  const& TheGrid() const { return *g;}

      element_iterator FirstElement() const { return et::FirstElement(TheGrid());}
      element_iterator EndElement()   const { return et::EndElement  (TheGrid());}


      template<class GFV, class BASEIT> // , class REF>
      class const_iterator_t {
	typedef const_iterator_t<GFV,BASEIT> self;
	typedef GFV gf_type;
	typedef typename gf_type::element_iterator element_iterator;
	typedef std::iterator_traits<BASEIT> bt;
      public:
	typedef typename gf_type::const_reference  const_reference;

	typedef typename bt::reference        reference;
	typedef typename bt::value_type       value_type;
	typedef typename bt::pointer          pointer;
	typedef typename bt::difference_type  difference_type;
	typedef std::forward_iterator_tag     iterator_category;
      private:
        gf_type      const*  gfv;
	element_iterator     e;
      public:
	const_iterator_t() : gfv(0) {}
	const_iterator_t(gf_type const& gf, element_iterator const& ee)
	  : gfv(&gf), e(ee) {}

	const_reference operator*() const { return (*gfv)(*e);}
	self & operator++() { ++e; return *this;}
	bool operator==(self const& rhs) const { return e == rhs.e;}
	bool operator!=(self const& rhs) const { return e != rhs.e;}
      };

      typedef const_iterator_t<self,base_const_iterator> const_iterator;

      size_type      size()  const { return et::size(*g);}
      const_iterator begin() const { return const_iterator(*this, FirstElement());}
      const_iterator end()   const { return const_iterator(*this, EndElement());}

    };

} // namespace shrink_grid_view 

} // namespace GrAL 

#endif
