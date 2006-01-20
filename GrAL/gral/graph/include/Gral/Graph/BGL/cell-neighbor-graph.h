#ifndef GRAL_GB_GRAPHS_CELL_CELL_GRAPH_H
#define GRAL_GB_GRAPHS_CELL_CELL_GRAPH_H

// $LICENSE_NEC_2005

#include <boost/config.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

#include <boost/ref.hpp>

#include "Gral/Graph/base.h"
#include "Gral/Graph/BGL/bgl.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-numbering.h"
#include "Container/tuple.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL { namespace graph {

  /*! \defgroup cell_neighbor_graph  Grid as graph via cell neighborhood relationship

     \ingroup gral_grid_graphs

      In a cell-neighbor graph view of a grid, grid cells play the roles of graph vertices,
      and grid facets joining two neighbor cells are the graph edges.
      The two possible flavors of the cell-neighbor graphs are 
      the undirected variant
      (each grid facet gives rise to one undirected graph edge) 
      and the directed variant
      (each grid facet gives rise to two directed graph edges).
   */


  template<class GRID, class GT>
  struct graph_view_cng_base
  {
  public:
    typedef GRID grid_type;
    typedef GT   gt;
    typedef typename gt::Facet              GridFacet;
    typedef typename gt::CellIterator       GridCellIterator;
    typedef typename gt::CellOnCellIterator GridCellOnCellIterator;
    //    typedef typename gt::Cell               vertex_type;
    typedef typename gt::CellIterator       vertex_iterator;
  private:
    ref_ptr<grid_type const> g;
  protected:
    mutable int  m_num_edges;
    mutable bool m_num_edges_valid;
  public:
    graph_view_cng_base() {}
    graph_view_cng_base(grid_type const& gg)         : g(gg), m_num_edges(0), m_num_edges_valid(false) {}
    graph_view_cng_base(ref_ptr<grid_type const> gg) : g(gg), m_num_edges(0), m_num_edges_valid(false) {}
    virtual ~graph_view_cng_base() {}

    ref_ptr<grid_type const> TheGrid() const { return g;}
    int num_edges()    const { return (m_num_edges_valid ? m_num_edges : compute_num_edges());}
    int num_vertices() const { return TheGrid()->NumOfCells();}

    struct vertex_type {
      typedef vertex_type self;
      typedef typename gt::Cell GridCell;
      typedef GridCell          base_type;
      base_type base;

      vertex_type() {}
      vertex_type(GridCell c) : base(c) {}

      // Without this conversion operator, some BGL algorithms don't work (e.g. push_relabel_max_flow).      
      operator int() const { return base.handle();}
      //operator base_type() const { return base;}
      base_type const& Base() const { return base;}
      
      bool operator==(self const& rhs) const { return base == rhs.base;}
      bool operator!=(self const& rhs) const { return base != rhs.base;}
    };

    GridCellOnCellIterator first_neighbor(vertex_type v) const { return GridCellOnCellIterator::begin(v.Base());}
    GridCellOnCellIterator end_neighbor  (vertex_type v) const { return GridCellOnCellIterator::end  (v.Base());}
        


    struct directed_graph_skip_edge_pred {
      bool operator()(GridCellOnCellIterator) const { return false;}
    };
    struct undirected_graph_skip_edge_pred {
      bool operator()(GridCellOnCellIterator cc) const { return cc.TheCell().handle() > (*cc).handle();}
    };

    template<class SkipPred, class EDGE> 
    struct edge_iterator {
      typedef edge_iterator<SkipPred, EDGE> self;
      typedef EDGE edge_type;
    private:
      GridCellIterator       c;
      GridCellOnCellIterator nb;
      SkipPred               skip_nb;
    public:
      edge_iterator() {}
      edge_iterator(GridCellIterator c_, SkipPred skip = SkipPred()) : c(c_),  skip_nb(skip)
      { 
	if(!c.IsDone()) nb = GridCellOnCellIterator(*c);
	advance_till_valid();
      }
      void increment() { advance(); advance_till_valid(); }
      self& operator++() { increment(); return *this;}
      edge_type operator*() const { return edge_type(nb);}
      bool IsDone() const { return c.IsDone();}
      bool operator==(self rhs) const { return c == rhs.c && (c.IsDone() || nb == rhs.nb);}
    private:   
      bool valid() const { return !nb.IsDone() && !skip_nb(nb);}
      void advance() {
	++nb;
	if(nb.IsDone()) {
	  ++c;
	  if(!c.IsDone())
	    nb = GridCellOnCellIterator(*c);
	}
      }
      void advance_till_valid() {
	while(! IsDone() && ! valid())
	  advance();
      }

    };
   
    

    GridCellOnCellIterator reverse(GridCellOnCellIterator c) const {
      GridCellOnCellIterator res(*c); // return(std::find((*c).FirstCell(), (*c).EndCell(), c.TheCell()));
      while(*res != c.TheCell()) {
	++res;
	ENSURE(!res.IsDone(), "",1);
      }
      return res;
    }
    /*! \brief distinguish between (u,v) and (v,u) by introducing 2 equivalence classes
      
       \invariant <tt> direction(c) == 1 - direction(reverse(c)) </tt>      
    */
    int direction(GridCellOnCellIterator c) const { return (c.TheCell() < *c ? 0 : 1);}

  protected:
    virtual int compute_num_edges() const = 0;

    int compute_num_cell_on_cell_iterators() const {
      m_num_edges = 0;
      for(GridCellIterator c(*TheGrid()); !c.IsDone(); ++c)
	for(GridCellOnCellIterator cc(*c); !cc.IsDone(); ++cc)
	  ++m_num_edges;
      m_num_edges_valid = true;
      return m_num_edges;
    }
  };


  /*! \brief Directed cell-neighbor graph view

      \ingroup cell_neigbor_graph
   */
  template<class GRID, class GT>
  struct graph_view<GRID, cell_neighbor_graph_tag, directed_graph_tag, GT> 
    : public graph_view_cng_base<GRID,GT>
  {
    typedef graph_view_cng_base<GRID,GT> base;
  public:
    typedef typename base::grid_type grid_type;
    typedef typename base::GridFacet GridFacet;

    graph_view() {}
    graph_view(grid_type const& gg)         : base(gg) {}
    graph_view(ref_ptr<grid_type const> gg) : base(gg) {}

    typedef typename base::GridCellOnCellIterator edge_type;
    GridFacet facet(edge_type e) const { return e.TheFacet();}
    typedef typename base::template edge_iterator<typename base::directed_graph_skip_edge_pred, edge_type>
    edge_iterator_base;

  protected:
    virtual int compute_num_edges() const { return base::compute_num_cell_on_cell_iterators();}
  }; 


  /*! \brief Undirected cell-neighbor graph view

      \ingroup cell_neigbor_graph
   */
  template<class GRID, class GT>
  struct graph_view<GRID, cell_neighbor_graph_tag, undirected_graph_tag, GT> 
    : public graph_view_cng_base<GRID,GT>
  {
    typedef graph_view_cng_base<GRID,GT> base;
  public:
    typedef typename base::GridCellOnCellIterator GridCellOnCellIterator;
    typedef typename base::GridFacet              GridFacet;
    typedef typename base::grid_type              grid_type;
    
    graph_view() {}
    graph_view(grid_type const& gg)         : base(gg) {}
    graph_view(ref_ptr<grid_type const> gg) : base(gg) {}

    struct edge_type {
    private:
      GridCellOnCellIterator base;
    public:
      edge_type() {}
      edge_type(GridCellOnCellIterator c) : base(c) {}
      
      bool operator==(edge_type const& rhs) const { 
	// base.TheFacet() == rhs.TheFacet()
	return 
	  (base.TheCell() == rhs.base.TheCell() && *base == *(rhs.base))
	  || 
	  (base.TheCell() == *(rhs.base) && *base = rhs.base.TheCell());
      }
      GridCellOnCellIterator Base() const { return base;}
    };

    GridFacet facet(edge_type e) const { return e.Base().TheFacet();}
    typedef typename base::template edge_iterator<typename base::undirected_graph_skip_edge_pred, edge_type>
    edge_iterator_base;


  protected:
    virtual int compute_num_edges() const {  
      base::compute_num_cell_on_cell_iterators();
      this->m_num_edges /= 2; 
      return this->m_num_edges;
    }
  }; 


  template<class E, class T, class GRID, class DIRECTED_TAG, class GT, class TAG>
  class vertex_property_map<E,T, graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG, GT>, TAG> 
    : public GrAL::grid_function<typename E::base_type, T>
  {
    typedef GrAL::grid_function<typename E::base_type, T> base;
    typedef graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG, GT> graph_type;
  public:
    typedef typename base::value_type       value_type;
    typedef typename base::reference        reference;
    typedef typename base::const_reference  const_reference;
    typedef E                               key_type;
    typedef boost::lvalue_property_map_tag  category;
    vertex_property_map() {}
    vertex_property_map(graph_type const& G)      : base(*G.TheGrid()) {}
    vertex_property_map(graph_type const& G, T t) : base(*G.TheGrid(),t) {}
    
    const_reference operator()(key_type e) const { return base::operator()(e.Base());}
    reference       operator[](key_type e)       { return base::operator[](e.Base());}
    const_reference operator[](key_type e) const { return base::operator[](e.Base());}
  };


  template<class E, class T, class GRID, class DIRECTED_TAG, class GT>
  class vertex_property_map<E,T, graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG, GT>, vertex_index_map_tag> 
    : public GrAL::element_numbering<typename E::base_type>
  {
    typedef GrAL::element_numbering<typename E::base_type> base;
    //    typedef GRAPH                      graph_type;
    typedef graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG, GT> graph_type;
  public:
    typedef typename base::value_type value_type;
    typedef value_type                reference;
    typedef E                         key_type;
    //    typedef boost::read_write_property_map_tag category;
    typedef boost::lvalue_property_map_tag category;
    vertex_property_map() {}
    vertex_property_map(graph_type const& G)      : base(*G.TheGrid()) {}
    
    value_type operator()(key_type e) const { return base::operator()(e.Base());}
    value_type operator[](key_type e) const { return base::operator()(e.Base());}
  };




  

  template<class E, class T, class GRID, class GT> 
  class edge_property_map<E,T, graph_view<GRID, cell_neighbor_graph_tag, undirected_graph_tag,GT>, void>
    : public GrAL::grid_function<typename GrAL::grid_types<GRID>::Facet, T>
  {
    typedef GrAL::grid_function<typename GrAL::grid_types<GRID>::Facet, T> base;
  public:
    typedef graph_view<GRID, cell_neighbor_graph_tag, undirected_graph_tag, GT> graph_type;
    typedef typename base::value_type          value_type;
    typedef typename base::reference           reference;
    typedef typename base::const_reference     const_reference;
    typedef E                                  key_type;
    //    typedef boost::read_write_property_map_tag category;
    typedef boost::lvalue_property_map_tag category;

    edge_property_map() {}
    edge_property_map(graph_type const& G)      : base(*G.TheGrid()) {}
    edge_property_map(graph_type const& G, T t) : base(*G.TheGrid(),t) {}

    const_reference  operator()(key_type e) const { return base::operator()(e.TheFacet());}
    reference        operator[](key_type e)       { return base::operator[](e.TheFacet());}
  };


  template<class E, class T, class GRID, class GT> 
  class edge_property_map<E,T,graph_view<GRID, cell_neighbor_graph_tag, directed_graph_tag,GT >, void>
    : public GrAL::grid_function<typename GrAL::grid_types<GRID>::Facet, GrAL::tuple<T,2> >
  {
    typedef GrAL::grid_function<typename GrAL::grid_types<GRID>::Facet, GrAL::tuple<T,2> > base;
  public:
    typedef graph_view<GRID, cell_neighbor_graph_tag, directed_graph_tag, GT> graph_type;
    typedef GrAL::tuple<T,2> tuple2;
    typedef T                                value_type;
    typedef typename tuple2::reference       reference;
    typedef typename tuple2::const_reference const_reference;
    typedef E        key_type;
    //    typedef boost::read_write_property_map_tag category;
    typedef boost::lvalue_property_map_tag category;

    edge_property_map() {}
    edge_property_map(graph_type const& gg)      : base(*gg.TheGrid()),   g(gg) {}
    edge_property_map(graph_type const& gg, T t) : base(*gg.TheGrid(),GrAL::tuple<T,2>(t)), g(gg) {}
 
    const_reference   operator()(key_type e) const { return base::operator()(e.TheFacet())[g->direction(e)];}
    reference         operator[](key_type e)       { return base::operator[](e.TheFacet())[g->direction(e)];}
    const_reference   operator[](key_type e) const { return base::operator[](e.TheFacet())[g->direction(e)];}
  private:
    GrAL::ref_ptr<graph_type const> g;
  };


  template<class E, class T, class GRID, class DIRECTED_TAG, class GT>
  class edge_property_map<E,T,graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG,GT>, GrAL::graph::reverse_map_tag> {
  public:
    typedef graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG, GT > graph_type;
    // typedef typename GRAPH::edge_descriptor  value_type;
    //typedef typename GRAPH::edge_descriptor  key_type;
    typedef E             value_type;
    typedef E             key_type;
    typedef value_type      &   reference;
    //  typedef value_type const&   reference;
    //    typedef boost::readable_property_map_tag category;
    typedef boost::lvalue_property_map_tag category;

    edge_property_map() {}
    edge_property_map(graph_type const& gg)    : g(gg) {}

    value_type operator()(key_type e) const { return g->reverse(e);}
    value_type operator[](key_type e) const { return g->reverse(e);}
  private:
    GrAL::ref_ptr<graph_type const> g;    
  };

}} // namespace GrAL { namespace graph



namespace boost {
     

  template<class GRID, class DIRECTED_TAG, class GT> 
  struct graph_traits<GrAL::graph::graph_view<GRID, GrAL::graph::cell_neighbor_graph_tag, DIRECTED_TAG, GT> >
  {
    typedef GrAL::graph::graph_view<GRID, GrAL::graph::cell_neighbor_graph_tag, DIRECTED_TAG, GT> graph_type;
    typedef typename graph_type::gt                 grid_types;
    typedef typename graph_type::vertex_type        vertex_descriptor;
    typedef typename graph_type::edge_type          edge_descriptor;
    typedef typename graph_type::vertex_iterator    vertex_iterator_base;
    typedef typename graph_type::edge_iterator_base edge_iterator_base;

    // static vertex_descriptor null_vertex(graph_type const& g) { return g.TheGrid()->invalid_vertex();}
    static vertex_descriptor null_vertex(graph_type const& g) { return vertex_descriptor();}

    typedef typename gral2bgl<DIRECTED_TAG>::type  directed_category;
    typedef disallow_parallel_edge_tag               edge_parallel_category;
    struct traversal_category : incidence_graph_tag,  vertex_list_graph_tag, edge_list_graph_tag {};

    typedef int vertices_size_type;
    typedef int edges_size_type;
    typedef int degree_size_type;


    class out_edge_iterator 
      : public iterator_facade<out_edge_iterator,
                               edge_descriptor,
                               forward_traversal_tag,
                               const edge_descriptor&,
                               const edge_descriptor*>
    {
    public:
               out_edge_iterator() {}
      explicit out_edge_iterator(edge_descriptor edge) : base(edge) {}
    private:
      const edge_descriptor& dereference() const { return base; }
      void increment() { ++base;}
      bool equal(const out_edge_iterator& other) const { return base == other.base; }
      edge_descriptor base;
      friend class iterator_core_access;
    };
    typedef std::pair<out_edge_iterator,out_edge_iterator> out_edge_iterator_range;



    class vertex_iterator : public iterator_facade<vertex_iterator,
						   vertex_descriptor,
						   forward_traversal_tag,
						   vertex_descriptor, // iterator operator* returns temporary
						   const vertex_descriptor*>
    {
    public:
               vertex_iterator() {}
      explicit vertex_iterator(vertex_iterator_base c) : base(c) {}
    private:
      vertex_descriptor dereference() const { return *base; }
      bool equal(const vertex_iterator& other) const { return base == other.base; }
      void increment() { ++base;}

      vertex_iterator_base  base;
      friend class iterator_core_access;
    };
    typedef std::pair<vertex_iterator, vertex_iterator>  vertex_iterator_range;


    class edge_iterator : public iterator_facade<edge_iterator,
						 edge_descriptor,
						 forward_traversal_tag,
						 edge_descriptor,
						 const edge_descriptor*>
    {
    public:
      edge_iterator() {}
      edge_iterator(edge_iterator_base b) : base(b) {}
    private:
      edge_descriptor dereference() const { return *base;}
      void increment() { ++base;}
      bool equal(edge_iterator const& rhs) const { return base == rhs.base;}
     
      edge_iterator_base base;
      friend class iterator_core_access;
    };
    typedef std::pair<edge_iterator, edge_iterator> edge_iterator_range;

    template<class T> 
    struct vertex_property_map { typedef GrAL::graph::vertex_property_map<vertex_descriptor,T, graph_type, void> type;};
    template<class T>
    struct edge_property_map   { typedef GrAL::graph::edge_property_map  <edge_descriptor,  T, graph_type, void> type;};

    typedef GrAL::graph::vertex_property_map<vertex_descriptor, vertices_size_type, graph_type, GrAL::graph::vertex_index_map_tag>
    vertex_index_map_type;
    // FIXME: this is potentially a large object!
    static vertex_index_map_type vertex_index_map(graph_type const& g) { return vertex_index_map_type(g);}

    typedef GrAL::graph::edge_property_map<edge_descriptor, void, graph_type, GrAL::graph::reverse_map_tag>  
    reverse_edge_map_type;
    static reverse_edge_map_type reverse_edge_map(graph_type const& g) { return reverse_edge_map_type(g);}
  };  // graph_traits<>


} // namespace boost

namespace GrAL { namespace graph {

#ifdef GRAPH_TYPE
#error "Macro GRAPH_TYPE already defined, change macro name below!"
#else
#define GRAPH_TYPE graph_view<GRID, cell_neighbor_graph_tag, DIRECTED_TAG, GT>


  template<class GRID, class DIRECTED_TAG, class GT>
  typename boost::graph_traits<GRAPH_TYPE >::vertex_descriptor
  source(typename boost::graph_traits<GRAPH_TYPE >::edge_descriptor e,
	 GRAPH_TYPE const& g) { return e.TheCell();}

  template<class GRID, class DIRECTED_TAG, class GT>
  typename boost::graph_traits<GRAPH_TYPE >::vertex_descriptor
  target(typename boost::graph_traits<GRAPH_TYPE >::edge_descriptor e,
	 GRAPH_TYPE const& g) { return *e;}

  template<class GRID, class DIRECTED_TAG, class GT>
  int num_vertices(GRAPH_TYPE const& g) { return g.num_vertices();}

  template<class GRID, class DIRECTED_TAG, class GT>
  int num_edges(GRAPH_TYPE const& g) { return g.num_edges();}
  
  template<class GRID, class DIRECTED_TAG, class GT>
  typename boost::graph_traits<GRAPH_TYPE >::degree_size_type
  out_degree(typename boost::graph_traits<GRAPH_TYPE >::vertex_descriptor const& v, GRAPH_TYPE const& g)
  { return v.Base().NumOfCells();}

  template<class GRID, class DIRECTED_TAG, class GT>
  typename boost::graph_traits<GRAPH_TYPE >::out_edge_iterator_range
  out_edges(typename boost::graph_traits<GRAPH_TYPE >::vertex_descriptor const&u,
	    GRAPH_TYPE const&                       g) 
  {
    typedef typename boost::graph_traits<GRAPH_TYPE >::edge_descriptor ed;
    return typename boost::graph_traits<GRAPH_TYPE >::out_edge_iterator_range
      (typename boost::graph_traits<GRAPH_TYPE >::out_edge_iterator(g.first_neighbor(u)),
       typename boost::graph_traits<GRAPH_TYPE >::out_edge_iterator(g.end_neighbor  (u)));
  }
	

  template<class GRID, class DIRECTED_TAG, class GT>
  typename boost::graph_traits<GRAPH_TYPE >::vertex_iterator_range
  vertices(GRAPH_TYPE const& g) 
  { return typename boost::graph_traits<GRAPH_TYPE >::vertex_iterator_range
      ( typename boost::graph_traits<GRAPH_TYPE >::vertex_iterator(g.TheGrid()->FirstCell()),
	typename boost::graph_traits<GRAPH_TYPE >::vertex_iterator(g.TheGrid()->EndCell  ()));
  }

  template<class GRID, class DIRECTED_TAG, class GT>
  typename boost::graph_traits<GRAPH_TYPE >::edge_iterator_range
  edges(GRAPH_TYPE const& g) 
  { return typename boost::graph_traits<GRAPH_TYPE >::edge_iterator_range
      ( typename boost::graph_traits<GRAPH_TYPE >::edge_iterator(g.TheGrid()->FirstCell()),
	typename boost::graph_traits<GRAPH_TYPE >::edge_iterator(g.TheGrid()->EndCell  ()));
  }


#undef GRAPH_TYPE
#endif  // ifdef GRAPH_TYPE 

}} // namespace GrAL { namespace graph

#endif
