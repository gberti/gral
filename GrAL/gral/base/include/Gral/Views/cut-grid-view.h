#ifndef GRAL_GB_BASE_VIEWS_CUT_GRID_VIEW_H
#define GRAL_GB_BASE_VIEWS_CUT_GRID_VIEW_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/wrap-elements.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Base/grid-function-hash.h"

#include "Gral/Iterators/iterator-base.h"

#include "Utility/ref-ptr.h"
#include "Container/sequence-algorithms.h"
#include "Container/counting-iterator.h"
#include "Geometry/point-traits.h"

#include <vector>

namespace GrAL {

  /*! \brief A view to a grid with some components cut off
 
  This view duplicates vertices (and all other lower dimensional elements) 
  along cut interfaces. These interfaces are determined by a classification of
  cells into components, which are to be separated from each other.
  It is possible to specify partial cuts that allow components to stay connected
  to the rest of the grid by leaving out cells from the classification: 
  An interface with the unclassified component is not considered a cut interface.
       
  \b Example:
  \code
  Grid G;
      
  partial_grid_function<gt::Cell, int> comps(G, -1);
  // set comps for a number of cells ...
  cut_grid_view::grid_view<cut_grid_view::cfg<Grid> > cut_view(G, comps);
       
  \endcode
  */
  namespace cut_grid_view {

    template<class GT> struct cfg {
      typedef GT                     gt;
      typedef typename gt::grid_type grid_type;

    };


    template<class CFG> class grid_view;
    template<class CFG> class vertex_handle_t;
    template<class CFG> class vertex_t;
    template<class CFG> class vertex_iterator_t;
    template<class CFG> class vertex_on_cell_iterator_t;
    template<class CFG> class cell_t;

    template<class CFG> struct grid_types_base :
      public grid_types_detail::grid_types_root,
      public archetypes_from_base_grid_types<typename CFG::gt>
    {
      typedef typename CFG::gt               bgt;
      typedef typename CFG::grid_type        base_grid_type;
      
      typedef typename bgt::dimension_tag    dimension_tag;
      typedef grid_view<CFG>                 grid_type;

      typedef vertex_handle_t<CFG>           vertex_handle;
      typedef vertex_t<CFG>                  Vertex;
      typedef vertex_iterator_t<CFG>         VertexIterator;
      typedef vertex_on_cell_iterator_t<CFG> VertexOnCellIterator;

      typedef wrapped_element_handle   <grid_type, typename bgt::cell_handle>         cell_handle;
      //      typedef wrapped_element          <grid_type, typename bgt::Cell, cell_handle>   Cell;
      typedef cell_t<CFG>                                                             Cell;
      typedef wrapped_sequence_iterator<grid_type, Cell, cell_handle, 
					typename bgt::CellIterator>                   CellIterator;

    //! Returns the id of a dummy component for unsplit vertices.
    static size_type unsplit_component() { return -1;}


  }; // struct grid_types_base

} // namespace cut_grid_view



template<class CFG>
struct grid_types<cut_grid_view::grid_view<CFG> >
  : public grid_types_base<cut_grid_view::grid_types_base<CFG> >
{};

template<class CFG>
struct element_traits<cut_grid_view::vertex_t<CFG> > 
  : public element_traits_base<cut_grid_view::vertex_t<CFG> > 
{
  typedef element_traits_base<cut_grid_view::vertex_t<CFG> >  base;
  typedef cut_grid_view::vertex_t<CFG>                        element_type;

  struct hasher_type : public base::hasher_type_elem_base {
    size_t operator()(element_type v) const {
      typedef element_traits<typename element_type::baseVertex> bet;
      typedef typename bet::hasher_type bh_type;
      bh_type bh;
      // FIXME: this is a very bad hash function.
      return 5 * bh(v.Base()) + v.handle().local();
    }
  };  
};

template<class CFG>
struct element_traits<cut_grid_view::cell_t<CFG> > 
  : public element_traits_base<cut_grid_view::cell_t<CFG> > 
{
  typedef element_traits_base<cut_grid_view::cell_t<CFG> >  base;
  typedef cut_grid_view::cell_t<CFG>                        element_type;

  struct hasher_type : public base::hasher_type_elem_base {
    size_t operator()(element_type c) const {
      typedef element_traits<typename element_type::baseCell> bet;
      typedef typename bet::hasher_type bh_type;
      bh_type bh;
      return bh(c.Base());
    }
  };  
};

namespace cut_grid_view {


  template<class CFG>
  class grid_view 
    : public archetypes_from_base<grid_view<CFG>, typename CFG::grid_type, typename CFG::gt>
  {
    typedef grid_view<CFG>   self;
    typedef grid_types<self> gt;
    typedef typename gt::bgt bgt;
  public:
    typedef self             grid_type;
    enum { dim = gt::dimension_tag::dim };

    typedef grid_view_category_d<dim>  category;

    typedef typename gt::VertexIterator VertexIterator;
    typedef typename gt::CellIterator   CellIterator;
    typedef typename gt::vertex_handle  vertex_handle;
    typedef typename gt::cell_handle    cell_handle;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::Cell           Cell;
    typedef typename gt::size_type      size_type;

    typedef typename bgt::grid_type     base_grid_type;
    typedef typename bgt::Vertex        baseVertex;
    typedef typename bgt::Cell          baseCell;

    typedef grid_function<baseCell, int>    component_map_type;
    //      typedef typename CFG::component_map_type  component_map_type;
  private:
    ref_ptr<base_grid_type const>                      g;
    ref_ptr<component_map_type const>                  m_cell_component;
    // may be more efficient to use a partial grid function here
    // but we need a total grid function in init (at least if CellOnVertexIterator is not defined).
    grid_function<baseVertex, std::vector<size_type> > m_vertex_components;
    size_type                                          m_num_new_vertices;
    std::vector<size_type>                             m_non_cut_components;
  public:

    /*! \brief Construct from base grid and a component map.
        Each component is treated as a cut component, that is,
	the cells of each component will be separated from the rest of the grid.
     */
    grid_view(base_grid_type const& gg, component_map_type const& comps)
      : g(gg), 
	m_cell_component(comps), 
	m_vertex_components(gg),
	m_num_new_vertices(0)
    { init();}
    /*! \brief Construct from base grid, a component map, and an enumeration of non-cutting components.

        Non-cutting components are not considered when deciding whether a vertex has to be split:
	There have to be at least two cut components incident to the vertex to trigger splitting.
        However, once a vertex is split, it is split into a separate vertex for each incident 
	component, whether the component is cutting or not.
  
        \todo If a vertex is split, maybe there should be only one vertex for the complete set of incident
	non-cutting components.
     */
    template<class InputIterator>
    grid_view(base_grid_type const& gg, component_map_type const& comps,
	      InputIterator begin_non_cut_comps, InputIterator end_non_cut_comps) 
      : g(gg), 
	m_cell_component(comps), 
	m_vertex_components(gg),
	m_num_new_vertices(0),
	m_non_cut_components(begin_non_cut_comps,end_non_cut_comps)
    { init();}

    //    void add_non_cut_component(size_type i) { m_non_cut_components.push_back(i);}

    base_grid_type const& BaseGrid() const { return *g;}

    size_type NumOfVertices() const { return BaseGrid().NumOfVertices() + m_num_new_vertices;}
    size_type NumOfCells()    const { return BaseGrid().NumOfCells(); }

    VertexIterator FirstVertex() const;
    VertexIterator EndVertex()   const;
    CellIterator   FirstCell()   const;
    CellIterator   EndCell()     const;

    /*! \brief Returns the number of duplicates of \c v

    This returns 1 if \c v is not duplicated.
  
    \invariant
    \$f \sum_{v \in G^0} <tt> G.NumOfDuplicates(v)  == G.NumOfVertices() </tt>
    */
    int NumOfDuplicates(baseVertex v) const { 
      return m_vertex_components(v).size();
    }

    /*! \brief Returns the component of the \c n th duplicate of \c v
      
    \pre <tt> NumOfDuplicates(v) == 0 || (0 <= n && n < NumOfDuplicates(v)) </tt> 
    */
    size_type Component(baseVertex v, size_type n) const { 
      REQUIRE(0 <= n  && n < (size_type)m_vertex_components(v).size(), "n=" << n, 1); 
      return m_vertex_components(v)[n];
    }
      
    size_type Component(baseCell c) const { return (*m_cell_component)(c); }

    bool valid(vertex_handle v) const;
    bool valid(cell_handle   c) const { return BaseGrid().valid(c);}
  private:
    void init();

  }; // class grid_view



  template<class CFG>
  class vertex_handle_t {
    typedef vertex_handle_t<CFG>        self;
    typedef grid_types_base<CFG>        gt;
    typedef typename gt::bgt            bgt;
    typedef typename bgt::vertex_handle base_handle_type;
    typedef typename gt::size_type      size_type;

    base_handle_type  base_v;
    size_type         comp_v;
  public:
    vertex_handle_t() {}
    explicit vertex_handle_t(base_handle_type bh, 
			     size_type         c = gt::unsplit_component())
      : base_v(bh), comp_v(c) {}

    base_handle_type Base()  const { return base_v;}
    size_type        local() const { return comp_v;}

    bool operator==(self rhs) const { return base_v == rhs.base_v && comp_v == rhs.comp_v;}
    bool operator!=(self rhs) const { return !(*this == rhs);}
    bool operator< (self rhs) const { return base_v < rhs.base_v || (base_v == rhs.base_v && comp_v < rhs.comp_v);}
  };


  template<class CFG>
  class vertex_iterator_t : public sequence_iter_base<grid_types<grid_view<CFG> >, vertex_type_tag> {
    typedef vertex_iterator_t<CFG>             self;
  public:
    typedef grid_vertex_iterator_category      category;
    typedef grid_view<CFG>                     grid_type;
    typedef grid_types<grid_type>              gt;

    typedef typename gt::Vertex                Vertex;
    typedef typename gt::vertex_handle         vertex_handle;
    typedef typename gt::size_type             size_type;

    typedef typename gt::bgt                   bgt;
    typedef typename bgt::VertexIterator       baseVertexIterator;
      
  private:
    ref_ptr<grid_type const> g;
    baseVertexIterator       v;
    size_type                curr_duplicate; // invariant: 0 <= curr_duplicate <= TheGrid.NumOfDuplicates(v)

  public:
    vertex_iterator_t() {}
    explicit 
    vertex_iterator_t(grid_type const& gg)
      : g(gg),  v(GrAL::begin<baseVertexIterator>(gg.BaseGrid())), curr_duplicate(0) {}
    vertex_iterator_t(grid_type const& gg, baseVertexIterator vv)
      : g(gg),  v(vv), curr_duplicate(0) {}
      
    self&         operator++()      { cv(); advance(); return *this;}
    bool          IsDone()    const { cb(); return v.IsDone();}
    vertex_handle handle()    const { cv(); return vertex_handle(v.handle(), TheGrid().Component(*v, curr_duplicate)); }
    Vertex        operator*() const { cv(); return Vertex(TheGrid(), handle()); }

    grid_type const& TheGrid()   const { return *g;}
    grid_type const& TheAnchor() const { return *g;}

    bool operator==(self rhs) const { cb(); rhs.cb(); return v == rhs.v && curr_duplicate == rhs.curr_duplicate;}
    bool operator!=(self rhs) const { return !(*this==rhs);}
    bool operator< (self rhs) const { cb(); rhs.cb(); return v < rhs.v || ( v == rhs.v &&  curr_duplicate < rhs.curr_duplicate);}
      
    bool bound() const { return g != 0; }
    bool valid() const { return bound() && v.valid();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}
 
  private:
    void advance() {
      ++curr_duplicate;
      if(curr_duplicate >= TheGrid().NumOfDuplicates(*v)) {
	curr_duplicate = 0;
	++v;
      }
    }
  }; // class vertex_iterator_t



  template<class CFG>
  class vertex_t {
    typedef vertex_t<CFG> self;
  public:
    typedef grid_view<CFG>                     grid_type;
    typedef grid_types<grid_type>              gt;
    typedef grid_vertex_category               category;
    typedef vertex_type_tag                    element_type_tag;

    typedef typename gt::vertex_handle         vertex_handle;
    typedef typename gt::bgt                   bgt;
    typedef typename bgt::Vertex               baseVertex;
  private:
    ref_ptr<grid_type const> g;
    vertex_handle            h;
  public:
    vertex_t() {}
    vertex_t(grid_type const& gg, vertex_handle hh) : g(gg), h(hh) {}

    vertex_handle    handle()  const { cv(); return h;}
    grid_type const& TheGrid() const { cb(); return *g;}

    baseVertex Base()     const { return baseVertex(TheGrid().BaseGrid(), h.Base());}
    operator baseVertex() const { return Base();}


    bool operator==(self rhs) const { cb(); rhs.cb(); return h == rhs.h;}
    bool operator!=(self rhs) const { return !(*this==rhs);}
    bool operator< (self rhs) const { cb(); rhs.cb(); return h < rhs.h; }
      
    bool bound() const { return g != 0; }
    bool valid() const { return bound() && g->valid(h);}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}

  }; // class vertex_t


  template<class CFG>
  class cell_t {
    typedef cell_t<CFG> self;
  public:
    typedef grid_view<CFG>                     grid_type;
    typedef grid_types<grid_type>              gt;
    typedef grid_cell_category                 category;
    typedef cell_type_tag                      element_type_tag;

    typedef typename gt::vertex_handle         vertex_handle;
    typedef typename gt::cell_handle           cell_handle;
    typedef typename gt::size_type             size_type;
    typedef typename gt::Vertex                Vertex;

    typedef typename gt::bgt                   bgt;
    typedef typename bgt::Vertex               baseVertex;
    typedef typename bgt::Cell                 baseCell;
    typedef typename bgt::vertex_handle        base_vertex_handle;
    typedef typename bgt::VertexOnCellIterator baseVertexOnCellIterator;

    typedef typename gt::archgt                archgt;
  private:
    ref_ptr<grid_type const> g;
    cell_handle              h;
  public:
    cell_t() {}
    cell_t(grid_type         const& gg, cell_handle hh) : g(gg), h(hh) {}
    cell_t(ref_ptr<grid_type const> gg, cell_handle hh) : g(gg), h(hh) {}
    
    grid_type const& TheGrid() const { return *g;}
    baseCell         Base()    const { return baseCell(g->BaseGrid(), h);}
    operator baseCell    ()    const { return Base();}
    cell_handle      handle()  const { return h;}

    vertex_on_cell_iterator_t<CFG> FirstVertex() const;
    vertex_on_cell_iterator_t<CFG> EndVertex()   const;
    size_type NumOfVertices() const { cb(); return Base().NumOfVertices();}

    bool operator==(self rhs) const { cb(); rhs.cb(); return h == rhs.h;}
    bool operator!=(self rhs) const { cb(); rhs.cb(); return h != rhs.h;}
    bool operator< (self rhs) const { cb(); rhs.cb(); return h < rhs.h;}

    vertex_handle v(typename archgt::vertex_handle av) const
    {
      base_vertex_handle bh = Base().v(av);
      return vertex_handle(bh, component(baseVertex(TheGrid().BaseGrid(),bh)));
    }
    Vertex V(typename archgt::Vertex av) const
    { return Vertex(*g, v(av.handle())); }

    size_type component(baseVertexOnCellIterator vc) const {
      REQUIRE(Base() == vc.TheCell(), "", 1);
      return component(*vc);
    }
    size_type component(baseVertex v) const {
      return (TheGrid().NumOfDuplicates(v) == 1 ?
	      TheGrid().Component(v,0) :
	      TheGrid().Component(*this));
    }


    bool bound() const { return g != 0;}
    void cb()    const { REQUIRE(bound(), "" ,1);}
  }; // class cell_t




  template<class CFG>
  class vertex_on_cell_iterator_t : public incidence_iter_base<grid_types<grid_view<CFG> >, 
							       vertex_type_tag, cell_type_tag>
  {
    typedef vertex_on_cell_iterator_t<CFG>     self;
  public:
    typedef grid_view<CFG>                     grid_type;
    typedef grid_types<grid_type>              gt;

    typedef grid_incidence_iterator_category_d<0,grid_type::dim>  category;

    typedef typename gt::Vertex                Vertex;
    typedef typename gt::vertex_handle         vertex_handle;
    typedef typename gt::Cell                  Cell;

    typedef typename gt::bgt                   bgt;
    typedef typename bgt::VertexOnCellIterator baseVertexOnCellIterator;

  private:
    Cell                      c;
    baseVertexOnCellIterator vc;
  public:
    vertex_on_cell_iterator_t() {}
    explicit 
    vertex_on_cell_iterator_t(Cell cc)
      : c(cc), vc(GrAL::begin<baseVertexOnCellIterator>(cc.Base())) {}
    vertex_on_cell_iterator_t(Cell cc, baseVertexOnCellIterator vcc) 
      : c(cc), vc(vcc) {}

    self&         operator++()      { cv(); ++vc; return *this; }
    bool          IsDone()    const { cb(); return vc.IsDone(); }
    Vertex        operator*() const {       return Vertex(TheGrid(), handle());}
    vertex_handle handle()    const { cv(); 
      /*
      typename gt::size_type component = (TheGrid().NumOfDuplicates(*vc) == 1 ?
					  TheGrid().Component(*vc,0) :
					  TheGrid().Component(c));
      */
      return vertex_handle(vc.handle(), c.component(vc));
    }
  
    Cell const& TheCell  () const { return c;}
    Cell const& TheAnchor() const { return c;}
    grid_type const& TheGrid() const { return c.TheGrid();}

    bool operator==(self rhs) const { cb(); rhs.cb(); return vc == rhs.vc;}
    bool operator!=(self rhs) const { cb(); rhs.cb(); return !(*this == rhs);}
    bool operator< (self rhs) const { cb(); rhs.cb(); return vc <  rhs.vc;}

    bool bound() const { return c.bound();}
    bool valid() const { return bound() && vc.valid();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}
 
  };


  //----------- inline functions ------------

  template<class CFG>
  inline vertex_on_cell_iterator_t<CFG> cell_t<CFG>::FirstVertex() const 
  { return  vertex_on_cell_iterator_t<CFG>(*this, GrAL::begin<baseVertex>(Base()));}

  template<class CFG>
  inline vertex_on_cell_iterator_t<CFG> cell_t<CFG>::EndVertex() const 
  { return  vertex_on_cell_iterator_t<CFG>(*this, GrAL::end<baseVertex>(Base()));}


  template<class CFG>
  inline typename grid_view<CFG>::VertexIterator grid_view<CFG>::FirstVertex() const 
  { return VertexIterator(*this, BaseGrid().FirstVertex()); }
 
  template<class CFG>
  inline typename grid_view<CFG>::VertexIterator grid_view<CFG>::EndVertex()   const 
  { return VertexIterator(*this, BaseGrid().EndVertex()); }

  template<class CFG>
  inline typename grid_view<CFG>::CellIterator grid_view<CFG>::FirstCell() const 
  { return CellIterator(*this, BaseGrid().FirstCell()); }
 
  template<class CFG>
  inline typename grid_view<CFG>::CellIterator grid_view<CFG>::EndCell()   const 
  { return CellIterator(*this, BaseGrid().EndCell()); }

  template<class CFG>
  inline bool grid_view<CFG>::valid(typename grid_view<CFG>::vertex_handle v) const 
  { return BaseGrid().valid(v.Base());}
    

  template<class CFG>
  void grid_view<CFG>::init() 
  {
    // m_vertex_components.clear();
    // could be more efficient if base_grid has CellOnVertexIterator
    for(typename bgt::CellIterator c = GrAL::begin_x(BaseGrid()); !c.IsDone(); ++c)
      for(typename bgt::VertexOnCellIterator vc = GrAL::begin_x(*c); !vc.IsDone(); ++vc)
	m_vertex_components[*vc].push_back(Component(*c));
    // now check if there are >= 2 split components incident to v
    // if yes, keep all components of v,
    // else, keep only the first one
    for(typename bgt::VertexIterator v = GrAL::begin_x(BaseGrid()); !v.IsDone(); ++v) {
      /// std::cout << "v=" << v.handle() << ", comps: [";
      sequence::sort_and_make_unique(m_vertex_components[*v]);
      ///      for(std::vector<int>::const_iterator m = m_vertex_components[*v].begin(); m != m_vertex_components[*v].end(); ++m)
	///	std::cout << *m << " ";
	///std::cout << "] ";

      int num_cut_comps = * std::set_difference(m_vertex_components[*v].begin(),  m_vertex_components[*v].end(),
						m_non_cut_components.begin(),     m_non_cut_components.end(),
						counter(0));

      /// std::cout << num_cut_comps << " components\n";

      if(num_cut_comps < 2) {
	// no vertex splitting occurs:
	// keep only one component (does not matter which one)
	m_vertex_components[*v].erase(m_vertex_components[*v].begin()+1,
				      m_vertex_components[*v].end());
      }
      else
	// keep all components: v is split into a new one for each component
	m_num_new_vertices += m_vertex_components[*v].size()-1;
    }
  }

#define gt grid_types<grid_view<CFG> > 
    
  template<class CFG>
  inline typename gt::VertexIterator gral_begin(grid_view<CFG> const& g, typename gt::VertexIterator)
  { return g.FirstVertex();}

  template<class CFG>
  inline typename gt::VertexIterator gral_end  (grid_view<CFG> const& g, typename gt::VertexIterator)
  { return g.EndVertex();}

  template<class CFG>
  inline typename gt::size_type      gral_size (grid_view<CFG> const& g, typename gt::VertexIterator)
  { return g.NumOfVertices();}




  template<class CFG>
  inline typename gt::VertexOnCellIterator gral_begin(typename gt::Cell const& a, vertex_on_cell_iterator_t<CFG>)
  { 
    typedef typename gt::bgt bgt;
    return vertex_on_cell_iterator_t<CFG>(a, GrAL::begin<typename bgt::Vertex>(a.Base()));
  }

  template<class CFG>
  inline typename gt::VertexOnCellIterator gral_end  (typename gt::Cell const& a, vertex_on_cell_iterator_t<CFG>)
  {  
    typedef typename gt::bgt bgt;
    return vertex_on_cell_iterator_t<CFG>(a, GrAL::end  <typename bgt::Vertex>(a.Base()));
  }

  template<class CFG>
  inline typename gt::size_type            gral_size (typename gt::Cell const& a, vertex_on_cell_iterator_t<CFG>)
  { 
    typedef typename gt::bgt bgt;
    return GrAL::size<typename bgt::Vertex>(a.Base());
  }

    
#undef  gt

} //  namespace cut_grid_view 


  // a better implementation might use a total grid function for the bulk of unsplit vertices
  // and a partial grid function for the rest.
  // Or we use a vertex index aka vertex_enumeration.
  // This should be easy to implement, parallel to m_vertex_components.
  // Probably it's more efficient to uses consecutive duplicate numbers 
  // instead of component ids in vertex_handle_t.
template<class CFG, class T>
class grid_function<cut_grid_view::vertex_t<CFG>, T> 
  : public grid_function_hash<cut_grid_view::vertex_t<CFG>, T> 
{
  typedef grid_function_hash<cut_grid_view::vertex_t<CFG>, T> base;
public:
  typedef cut_grid_view::grid_view<CFG>                 grid_type;
  grid_function() {}
  grid_function(grid_type         const& g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(grid_type         const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


  template<class CFG, class T>
  class grid_function<cut_grid_view::cell_t<CFG>, T>
    : public grid_function<typename cut_grid_view::cell_t<CFG>::baseCell, T>
               ::template apply<cut_grid_view::cell_t<CFG>, T>::type
  {
    typedef typename cut_grid_view::cell_t<CFG>::baseCell baseCell;
    typedef cut_grid_view::cell_t<CFG>                    Cell;
    typedef typename grid_function<baseCell, T>::template apply<Cell, T>::type base;
  public:
    typedef cut_grid_view::grid_view<CFG>                 grid_type;
    grid_function() {}
    grid_function(grid_type         const& g) : base(g) {}
    grid_function(ref_ptr<grid_type const> g) : base(g) {}
    grid_function(grid_type         const& g, T const& t) : base(g,t) {}
    grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
  };

namespace cut_grid_view {

  /*! \brief The geometric grid view

     \todo should allow assigment to coordinates of split vertices
     \todo could offer the local outward normal vector at split vertices
     as an aid to defining new coordinates.
     \todo could somehow automatically inherit all geometric stuff from GEOM 
      ... but only if no changes to coordinates are allowed! 
  */
  template<class CFG, class GEOM>
  class geom_view {
    typedef GEOM                                base_geom_type;
  public:
    typedef grid_geometry_category              category;
    typedef typename base_geom_type::coord_type coord_type;
    typedef point_traits<coord_type>            pt;

    typedef grid_view<CFG>                      grid_type;
    typedef grid_types<grid_type>               gt;
    typedef typename gt::Vertex                 Vertex;

  private:
    ref_ptr<grid_type const>      g;
    ref_ptr<base_geom_type const> base_geom;
  public:
    geom_view() {}
    geom_view(grid_type         const& gg, base_geom_type         const& geom) : g(gg), base_geom(geom) {}
    geom_view(ref_ptr<grid_type const> gg, ref_ptr<base_geom_type const> geom) : g(gg), base_geom(geom) {}

    int space_dimension() const { return base_geom->space_dimension();}
    grid_type const& TheGrid() const { return *g;}

    coord_type coord(Vertex v) const { return base_geom->coord(v.Base()); }
    // coord_type& coord(Vertex v)   { ... }
  }; // geom_view  

} // namespace cut_grid_view
} // namespace GrAL

#endif
