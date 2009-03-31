#ifndef GRAL_GB_COLLAPSED_GRID_VIEW_H
#define GRAL_GB_COLLAPSED_GRID_VIEW_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Iterators/cell-on-vertex-iterator.h"
#include "Gral/Base/extend-grid-types.h"
#include "Gral/Subranges/enumerated-element-range.h"

#include "Geometry/point-traits.h"
#include "Container/union-find.h"

#include <iostream>

namespace GrAL {

/*! \defgroup collapsed_grid_view   A view of a grid with vertices collapsed

    \ingroup gridviews

    A collapsed view of a grid is essentially a decimation of a grid.
    It is assumed that the underlying grid is simplicial.

    Typical usage example:
    \code
    grid_type G;
    geom_type GeomG(G);
    // ...
    namespace cgv = collapsed_grid_view;
    cgv::grid_view<grid_type>           CollapsedG(G);
    cgv::geom_view<grid_type,geom_type> CollapsedGeom(CollapsedG, GeomG);
    typedef enumerated_vertex_range<grid_type> vertex_set_type;
    vector<vertex_set_type> v_sets;
    // ... populate vertex_clusters with sets of adjacent vertices to be identified ...
    CollapsedG.collapse_vertex_sets(v_sets.begin(), v_sets.end());

    // set coords for each set
    for(vector<vertex_set_type>::iterator s = v_sets.begin(); s != v_sets.end(); ++s)
      CollapsedGeom(* s->FirstVertex()) = average_m(s->FirstVertex(), s->EndVertex(), vertex2coord(Geom));
    \endcode

    \see Test in \ref test-collapsed-grid-view.C
    \see \ref gridviews module

    \todo add Edge and Facet elements
 */


namespace collapsed_grid_view {

  template<class GRID>  class grid_view;
  template<class GRID>  class cell_iterator;
  template<class GRID>  class vertex_iterator;
  template<class GRID>  class vertex;
  template<class GRID>  class vertex_on_cell_iterator;



  template<class GRID>
  class local_grid_types : public grid_types_detail::grid_types_root {
  public:
    typedef grid_view<GRID>        grid_type;
    typedef cell_iterator<GRID>    Cell;
    typedef cell_iterator<GRID>    CellIterator;
    typedef vertex<GRID>           Vertex;
    typedef vertex_iterator<GRID>  VertexIterator;
    typedef vertex_on_cell_iterator<GRID> VertexOnCellIterator;
    
    // TODO: do this only of no cell_on_vertex_iterator available
    typedef cell_on_vertex_iterator<GRID>  base_c_o_v_iterator;
    typedef xgt<grid_types<GRID>, 
		override_CellOnVertexIterator<base_c_o_v_iterator> >   bgt;
    // typedef xgt<grid_types<GRID>, override_ifno_CellOnVertexIterator<base_c_o_v_iterator> >   bgt;
    static void init_iterators(grid_type const& g);

    typedef GRID              base_grid_type;
    typedef typename bgt::size_type     size_type;
    typedef typename bgt::cell_handle   cell_handle;
    typedef typename bgt::vertex_handle vertex_handle;
    
    typedef typename bgt::dimension_tag      dimension_tag;
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
	typedef collapsed_grid_view::vertex<GRID> element_type;
	typedef collapsed_grid_view::vertex<GRID> element_iterator;
      };
    
    template<class GRID>
      struct map_types_for_grid_view<GRID, cell_type_tag> {
	typedef collapsed_grid_view::cell_iterator<GRID> element_type;
	typedef collapsed_grid_view::cell_iterator<GRID> element_iterator;
      };
  } // namespace detail


/*! \brief An view of a grid with some sets of vertices collapsed (identified)

    \ingroup collapsed_grid_view

    Combinatorially, each vertex set is replaced by a new vertex, and the vertices 
    in the set are removed. Also, all cells containing two or more  vertices of the same set
    (in other words, they contain duplicates of new vertices) are removed.
     

    Geometrically, each new vertex is placed at some coordinate near the coordinates of its underlying set.
    Here, different options are possible; the default location is the barycenter of the set.

 */
  template<class GRID>
    class grid_view : public local_grid_types<GRID> {
      typedef grid_view<GRID>        self;
      typedef local_grid_types<GRID> gt;
    public:
      typedef grid_view_category_d<gt::dimension_tag::dim> category;

      typedef typename gt::size_type          size_type;
      typedef typename gt::archetype_iterator archetype_iterator;
      typedef typename gt::archetype_type     archetype_type;
      typedef typename gt::archetype_handle   archetype_handle;
      typedef typename gt::base_grid_type     base_grid_type;
      typedef typename gt::Cell               Cell;
      typedef typename gt::cell_handle        cell_handle;
      typedef typename gt::Vertex             Vertex;
      typedef typename gt::vertex_handle      vertex_handle;
      typedef typename gt::bgt                bgt;

      //! \brief Vertex of the base grid
      typedef typename bgt::Vertex BaseVertex;

    private:
      ref_ptr<base_grid_type const> g; 

      mutable size_type  num_of_vertices;
      mutable size_type  num_of_cells;

      // initially identity on vertex_handle. handle_v[v] points to the representant of the set of v.
      union_find<vertex_handle> handle_v; 
      // all original vertices which have been removed
      partial_grid_function<typename bgt::Vertex, bool>  collapsed_v;
      // cells contains two or more vertices from same set (i.e. contains duplicate vertices)
      partial_grid_function<typename bgt::Cell,   bool>  collapsed_c;
      // cell contains at most one collapsed vertex from each vertex set (i.e. no duplicates)
      // partial_grid_function<typename bgt::Cell,   bool>  affected_c; 

      typedef enumerated_cell_range<base_grid_type> cell_range_type;
      partial_grid_function<Vertex, cell_range_type> incident_cells;
    public:

      grid_view(base_grid_type const& gg) 
	: g(gg), collapsed_v(g, false), collapsed_c(g, false),
	  incident_cells(*this, cell_range_type(gg))
      { init();}


      template<class IT>
      grid_view(base_grid_type const& gg, IT begin_vertex_set, IT end_vertex_set) 
	: g(&gg), collapsed_v(g), collapsed_c(g), 
	  incident_cells(*this, cell_range_type(g))
      {
	init();
	collapse_vertex_sets(begin_vertex_set, end_vertex_set); 
      }
      
      void init() { 
	init_iterators(*this);
	num_of_vertices = BaseGrid()->NumOfVertices();
	num_of_cells    = BaseGrid()->NumOfCells();
      }

      /*! \brief Collapse the vertices of each set in <tt>[begin_vertex_set, end_vertex_set)</tt>
	
         <tt>[begin_vertex_set, end_vertex_set)</tt> is a range of vertex sets of the original grid.
	 This method can be called repeatedly.
	 
         \precondition
          For each set \f$ S \f$ in <tt>[begin_vertex_set, end_vertex_set)</tt>,
	  all vertices of  \f$ S \f$ are different in the collapsed view:
          <tt> CollapsedVertex(v_i) != CollapsedVertex(v_j) </tt> \f$ \forall v_i \neq v_j \in S \f$.
       */
      template<class IT>
      void collapse_vertex_sets(IT begin_vertex_set, IT end_vertex_set);

      template<class R>
      void collapse_vertex_set(R const& r) {
	std::vector<R> v;
	v.push_back(r);
	collapse_vertex_sets(v.begin(), v.end());
      }
      void collapse_vertices(BaseVertex v1, BaseVertex v2) {
	enumerated_vertex_range<base_grid_type> v(*BaseGrid());
	v.push_back(v1);  v.push_back(v2); 
	collapse_vertex_set(v);
      }

      size_type NumOfCells()    const { return num_of_cells;}
      size_type NumOfVertices() const { return num_of_vertices;}

      cell_iterator<GRID>   FirstCell()   const;
      vertex_iterator<GRID> FirstVertex() const;
      cell_iterator<GRID>   EndCell()   const;
      vertex_iterator<GRID> EndVertex() const;

      ref_ptr<base_grid_type const> BaseGrid() const { return g;}

      bool valid(typename bgt::Vertex v) const { return (v.handle() == handle_v(v.handle()));}
      bool valid(typename bgt::Cell   c) const { return (!collapsed_c(c)); }
      // bool collapsed(Vertex v) const { return collapsed_v(v); }
      bool collapsed(typename bgt::Vertex v) const { return collapsed_v(v);}
      bool collapsed(typename bgt::Cell   c) const { return collapsed_c(c);}
      vertex_handle handle(vertex_handle v)  const { return handle_v(v);}

      //! \brief Convert vertex of base grid into representant of collapsed set.      
      Vertex CollapsedVertex(BaseVertex v) const;


      template<class element_tag>
      struct map_types : public detail::map_types_for_grid_view<GRID,element_tag> {};

      archetype_iterator BeginArchetype() const 
	{ return BaseGrid()->BeginArchetype();}  
      archetype_iterator EndArchetype() const 
	{ return BaseGrid()->EndArchetype();}
      archetype_handle   handle(archetype_iterator it) const { return BaseGrid()->handle(it);}

      archetype_type const& Archetype(archetype_handle a) const { return BaseGrid()->Archetype(a);}
      archetype_type const& ArchetypeOf(Cell const& c) const
	{ return BaseGrid()->ArchetypeOf(c.BaseCell()); }
      archetype_handle archetype_of(Cell const& c) const
	{ return BaseGrid()->archetype_of(c.BaseCell());}
      archetype_handle archetype_of(cell_handle c) const
	{ return BaseGrid()->archetype_of(c);}

    };


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
      typedef typename gt::Cell          Cell;

      typedef cell_type_tag      element_type_tag;
      struct category: virtual grid_cell_category, virtual grid_cell_iterator_category {};
      typedef grid_type          anchor_type;
      typedef Cell               value_type;
   private:
      ref_ptr<grid_type  const>  g;
      typename bgt::CellIterator c;
    public:
      cell_iterator() : g(0) {}
      explicit
      cell_iterator(grid_type const& gg) : g(&gg), c(*g->BaseGrid()) 
      { advance_till_valid(); }
      cell_iterator(grid_type const& gg,
		    cell_handle      cc) : g(&gg), c(*g->BaseGrid(), cc) 
      { advance_till_valid(); }
      cell_iterator(grid_type const& gg,
		    typename bgt::CellIterator bc) : g(&gg), c(bc) 
      { advance_till_valid(); }

      self      & operator++()      { ++c; advance_till_valid(); return *this;}
      self const& operator*() const { return *this;}
      bool IsDone() const { return c.IsDone();}
      cell_handle handle() const { return c.handle();}

      grid_type const& TheGrid  () const { return *g;}
      grid_type const& TheAnchor() const { return *g;}
     
      // Vertex-On-Cell Iteration
      vertex_on_cell_iterator<GRID> FirstVertex() const;
      vertex_on_cell_iterator<GRID> EndVertex()   const;
      unsigned NumOfVertices() const { return GrAL::size<typename bgt::Vertex>(*c);}

      Vertex V(typename archgt::Vertex av) const
      { return Vertex(*g, BaseCell().v(av.handle())); }

      bool operator==(self const& rhs) const { return c == rhs.c;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}
      bool operator< (self const& rhs) const { return c <  rhs.c;}

      bool bound() const { return (g!= 0);}
      bool valid() const { return bound() && c.valid();}

      typename bgt::Cell  BaseCell() const { return *c;}
      typename bgt::Cell  Base    () const { return *c;}
    private:
      void advance_till_valid() {
	while(!c.IsDone() && !TheGrid().valid(*c))
	  ++c;
      }
    };




  template<class GRID>
  class vertex : public local_grid_types<GRID> {
  private:
    typedef vertex<GRID>           self;
    typedef local_grid_types<GRID> base;
  public:
    typedef vertex_type_tag              element_type_tag;
    typedef grid_vertex_category         category;
    typedef typename base::bgt           bgt;
    typedef typename base::grid_type     grid_type;
    typedef typename base::vertex_handle vertex_handle;
  private:
    
    ref_ptr<grid_type const>  g;
    vertex_handle             h; // always handle of representant
  public:
    vertex() {}
    
    vertex(grid_type const& gg, vertex_handle hh)       : g(gg)  { h = g->handle(hh);}
    vertex(grid_type const& gg, typename bgt::Vertex v) : g(gg)  { h = g->handle(v.handle());}
    

    vertex_handle handle() const { return h;}
    grid_type const& TheGrid() const { return *g;}

    bool operator==(self const& rhs) const { return h == rhs.h;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    bool operator< (self const& rhs) const { return h < rhs.h;}

    // conversion to base. This is of course not unique for collapsed vertices.
    typename bgt::Vertex BaseVertex() const { return typename bgt::Vertex(* g->BaseGrid(), h);}
    typename bgt::Vertex Base()       const { return BaseVertex(); }
  };



  

  template<class GRID>
    class vertex_iterator : public local_grid_types<GRID> {
    private:
      typedef vertex_iterator<GRID>  self;
      typedef local_grid_types<GRID> base;
    public:
      typedef typename base::bgt           bgt;
      typedef typename base::archgt        archgt;
      typedef typename base::grid_type     grid_type;
      typedef typename base::cell_handle   cell_handle;
      typedef typename base::vertex_handle vertex_handle;
      typedef typename base::Cell          Cell;
      typedef typename base::Vertex        Vertex;

      typedef grid_vertex_iterator_category   category;
      typedef grid_type                       anchor_type;
      typedef Vertex                          value_type;
    private:

      ref_ptr<grid_type const>     g;
      typename bgt::VertexIterator v;
    public:
      vertex_iterator() {}

      explicit
      vertex_iterator(grid_type const& gg) 
	: g(&gg), v(g->BaseGrid()->FirstVertex())
      { advance_till_valid();}

      vertex_iterator(grid_type const& gg,
		      typename bgt::VertexIterator bv)
	: g(&gg), v(bv)
      { advance_till_valid(); }

      vertex_iterator(grid_type const& gg, 
		      vertex_handle h) 
	: g(&gg), v(gg.BaseGrid(),h) 
      { advance_till_valid();}


      self& operator++() { ++v; advance_till_valid(); return *this;}
      Vertex operator*() const { return Vertex(*g, handle());}
      bool IsDone() const { return v.IsDone();}
      vertex_handle handle() const { return g->handle(v.handle());}

      grid_type const& TheGrid  () const { return *g;}
      grid_type const& TheAnchor() const { return *g;}

      bool operator==(self const& rhs) const { return v == rhs.v;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}
      bool operator< (self const& rhs) const { return v < rhs.v;}

      typename bgt::Vertex BaseVertex() const { return *v;}
      typename bgt::Vertex Base()       const { return *v;}
    private:
      void advance_till_valid() {
	while(!v.IsDone() && !TheGrid().valid(*v))
	  ++v;
      }
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

      typedef grid_incidence_iterator_category category;
      typedef Cell   anchor_type;
      typedef Vertex value_type;
    private:
      ref_ptr<grid_type const> g;
      typename  bgt::VertexOnCellIterator vc;
    public: 
      vertex_on_cell_iterator() : g(0) {}
      explicit
      vertex_on_cell_iterator(Cell const& c) 
      	: g(c.TheGrid()), vc(GrAL::begin<typename bgt::Vertex>(*(c.c))) {} 
      vertex_on_cell_iterator(Cell const& c, typename  bgt::VertexOnCellIterator vvc) 
	: g(c.TheGrid()), vc(vvc) {} 

      self& operator++() { ++vc; return *this;}
      Vertex operator*() const { return Vertex(*g,handle());}
      vertex_handle handle() const { return g->handle(vc.handle());}
      bool IsDone() const { return vc.IsDone();}

      grid_type const& TheGrid()   const { return *g;}
      Cell             TheAnchor() const { return Cell(TheGrid(),vc.TheAnchor().handle());}

      bool operator==(self const& rhs) const { return vc == rhs.vc;}
      bool operator!=(self const& rhs) const { return !(*this == rhs);}
      bool operator< (self const& rhs) const { return vc < rhs.vc;}
    };


  //------------- inline functions -----------------------

  template<class GRID>
  inline
  vertex_on_cell_iterator<GRID> 
  cell_iterator<GRID>::FirstVertex() const 
  { return typename gt::VertexOnCellIterator(*this);}

  template<class GRID>
  inline
  vertex_on_cell_iterator<GRID> 
  cell_iterator<GRID>::EndVertex() const 
  { return typename gt::VertexOnCellIterator(*this, GrAL::end<typename bgt::Vertex>(c));}


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
  grid_view<GRID>::EndVertex() const { return typename gt::VertexIterator(*this, BaseGrid()->EndVertex());}

  template<class GRID>
  inline
  cell_iterator<GRID> 
  grid_view<GRID>::EndCell() const { return typename gt::CellIterator(*this, BaseGrid()->EndCell());}


  template<class GRID>
  inline
  typename grid_view<GRID>::Vertex 
  grid_view<GRID>::CollapsedVertex(typename grid_view<GRID>::BaseVertex v) const { return Vertex(*this,v);}



  template<class GRID>
  template<class IT>
  void grid_view<GRID>::collapse_vertex_sets(IT begin_vertex_set, IT end_vertex_set)
  {
    typedef typename std::iterator_traits<IT>::value_type vertex_set_type;
    typedef grid_types<vertex_set_type>                   setgt;
    size_type vcnt = 0;
    size_type ccnt = 0;
    for(IT s = begin_vertex_set; s != end_vertex_set; ++s) {
      partial_grid_function<typename bgt::Cell, int> num_collapsed_vertices(*BaseGrid(), 0);
      typename setgt::VertexIterator v = (*s).FirstVertex();
      vertex_handle         repr_s = handle_v(v.handle());
      // Join all vertices (vertex sets) in the set s into a single one, represented by repr_s
      for( ; !v.IsDone(); ++v) {
	Vertex V = CollapsedVertex(*v);
	// initialize incident cells for V. This only happens if v is an original vertex.
	if(incident_cells(V).empty())
	  for(typename bgt::CellOnVertexIterator cv(*v); !cv.IsDone(); ++cv)
	    incident_cells[V].push_back(*cv);
	// Cells with >= 2 vertices in this set disappear, cells with 1 vertex in s will form
	// the incident_cells of the collapsed vertex.	
	typedef grid_types<cell_range_type> rgt;
	for(typename rgt::CellIterator ic(incident_cells(V).FirstCell()); !ic.IsDone(); ++ic)
	  num_collapsed_vertices[*ic]++;
	incident_cells[V].clear();

	if(v != (*s).FirstVertex()) {
	  REQUIRE( handle_v(repr_s) != handle_v(v.handle()), 
		   "identifying already identified vertices: " << repr_s << " and " << v.handle() 
		<< " in set " << handle_v(repr_s), 1);
	  handle_v.join(repr_s, v.handle());
	}
	collapsed_v[*v] = true; 
	++vcnt;
      }
      --vcnt; // one vertex remains from s
      // mark collapsed cells & remove them from incident_cells(Repr) 
      Vertex ReprS(*this, repr_s);
      for(typename  partial_grid_function<typename bgt::Cell, int>::ElementIterator 
	    c = num_collapsed_vertices.FirstElement(); !c.IsDone(); ++c) {
	if(! collapsed_c(*c)) {
	  if(num_collapsed_vertices(*c) >= 2) {
	    collapsed_c[*c] = true;
	    ++ccnt;
	  }
	  if(num_collapsed_vertices(*c) == 1)
	    incident_cells[ReprS].push_back(*c);
	}
      }
    }
    // this would be not correct if vertex sets could contain duplicates,
    // i.e. vertices already identified before.
    num_of_vertices -= vcnt;
    num_of_cells    -= ccnt;
  }
  
  //---------------- grid geometry view ---------------------

  /*! \brief The geometric collapsed view

   */
  template<class GRID, class GEOM>
  /*  class geom_view : public linear_geometry<geom_view<GRID,GEOM> > */
  class geom_view : public local_grid_types<GRID>
  {
    typedef GEOM                   base_geom_type;
    typedef local_grid_types<GRID> gt;
  public:
    typedef typename base_geom_type::coord_type coord_type;
    typedef point_traits<coord_type>            pt;
    typedef typename gt::bgt                    bgt;
    typedef typename gt::grid_type              grid_type;
    typedef typename gt::vertex_handle          vertex_handle;
    typedef typename gt::Vertex                 Vertex;
    typedef typename gt::Cell                   Cell;
    typedef typename gt::VertexOnCellIterator   VertexOnCellIterator;
    
  private:
    ref_ptr<grid_type      const> g;
    ref_ptr<base_geom_type      > geom;
    partial_grid_function<typename bgt::Vertex, coord_type> new_coords;
    coord_type dummy;
  public:
    geom_view(grid_type      const& gg,
	      base_geom_type      & ggeom)
      : g(&gg), geom(&ggeom), new_coords(* gg.BaseGrid()) {}
    
    // either original coordinate or new coordinate
    coord_type  coord (Vertex const& v) const 
    { return (g->collapsed(v.Base()) ? new_coords(v.Base()) : geom->coord(v.Base()));}
    coord_type& coord (Vertex const& v)       
    { 
      if(g->collapsed(v.Base()))
	return new_coords[v.Base()];
      else {
	std::cout << "Warning: return dummy!\n";
	dummy = geom->coord(v.Base());
	return dummy;
      }
    }

    coord_type  coord(typename bgt::Vertex const& v) const { 
      return (g->collapsed(v) ? new_coords(v) : geom->coord(v));
    }

    // cannot write to original coords
    coord_type& coord(typename bgt::Vertex const& v) { 
      if(g->collapsed(v))
	return new_coords[Vertex(*g,v).Base()];
      else {
	std::cout << "Warning: return dummy!\n";
	dummy = geom->coord(v);
	return dummy;
      }
    }
  };


  template<class GRID>
  void local_grid_types<GRID>::init_iterators(grid_view<GRID> const& g) {
    base_c_o_v_iterator::init(* g.BaseGrid());
  };

} // namespace collapsed_grid_view



//----- classes in global scope: grid_types<>, element_traits<>, grid_function<>  ---------


template<class GRID>
class grid_types<collapsed_grid_view::grid_view<GRID> > :
  public grid_types_base<collapsed_grid_view::local_grid_types<GRID> > {};


template<class GRID>
struct element_traits<collapsed_grid_view::vertex<GRID> >
  : public element_traits_vertex_base<collapsed_grid_view::grid_view<GRID> > 
{
  typedef element_traits_vertex_base<collapsed_grid_view::grid_view<GRID> >  base;
  typedef typename base::element_type   element_type;
  typedef typename base::handle_type    handle_type;
  typedef collapsed_grid_view::local_grid_types<GRID> lgt; // gt of view
  typedef typename lgt::bgt                        bgt; // gt of GRID
  typedef element_traits<typename bgt::Vertex>     bet;
  typedef typename bet::hasher_type                base_hasher_type;

  struct hasher_type : public base::hasher_type_elem_base {
    //unsigned operator()(handle_type const&  v) const 
    //{ base_hasher_type h; return h(v);}
    unsigned operator()(element_type const&  v) const 
    { base_hasher_type h; return h(v.Base());}
  };
};


template<class GRID>
struct element_traits<collapsed_grid_view::cell_iterator<GRID> >
  : public element_traits_cell_base<collapsed_grid_view::grid_view<GRID> > 
{
  typedef element_traits_cell_base<collapsed_grid_view::grid_view<GRID> >  base;
  typedef typename base::element_type   element_type;
  typedef typename base::handle_type    handle_type;

  typedef collapsed_grid_view::local_grid_types<GRID> lgt; // gt of view
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



  namespace collapsed_grid_view {

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

#undef  gt

  }// namespace collapsed_grid_view


template<class GRID, class T>
class grid_function<collapsed_grid_view::vertex<GRID>, T>
  : public grid_function_hash<collapsed_grid_view::vertex<GRID>, T>
{
  typedef grid_function_hash<collapsed_grid_view::vertex<GRID>, T>  base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g,
		T         const& t0) : base(g,t0) {}
};


template<class GRID, class T>
class grid_function<collapsed_grid_view::cell_iterator<GRID>, T>
  : public grid_function_hash<collapsed_grid_view::cell_iterator<GRID>, T>
{
  typedef grid_function_hash<collapsed_grid_view::cell_iterator<GRID>, T>  base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g,
		T         const& t0) : base(g,t0) {}
};



namespace collapsed_grid_view {

  /*! \brief adapter for grid functions on base grid
      
       This adapter forwards accesses through elements of 
       collapsed_grid_view::grid_view to accesses through their base
       elements to the adaptee. 


       \todo Specialize for vertices.
       Different strategies could be used to deal with collapsed vertices.
       Currently, the value of the representant of a collapse set is returned.
       At least, there should be a possibility to define the value explicitely for
       collapsed vertices.

   */

  template<class GF>
    class grid_function_view {
    private:
      typedef grid_function_view<GF>            self;
      typedef typename GF::grid_type            base_grid_type;
      typedef typename GF::element_type         base_element_type;
      typedef element_traits<base_element_type> bet;
      typedef typename bet::element_type_tag    btag;
      typedef typename GF::const_iterator       base_const_iterator;
    public:
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

} // namespace collapsed_grid_view 

} // namespace GrAL 

#endif
