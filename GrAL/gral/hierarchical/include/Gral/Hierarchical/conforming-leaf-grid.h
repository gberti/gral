#ifndef GRAL_GB_CONFORMING_LEAF_GRID_H
#define GRAL_GB_CONFORMING_LEAF_GRID_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */




#include "Gral/Hierarchical/octree.h"            
#include "Gral/Hierarchical/hierarchical-grid.h" 
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Base/partial-grid-function-hash.h"

#include "Gral/Iterators/closure-iterator.h"

namespace GrAL { namespace octree { namespace conforming_leaf_grid {

  template<class OCTREE> class grid;


  template<class ANCHOR, class ELEM, class ELEMITER>  class side_iterator_t;

  template<class OCTREE>                 class element_base_t;
  template<class OCTREE>                 class vertex_t;
  template<class OCTREE>                 class cell_t;
  template<class OCTREE, class HIERELEM> class element_t; 


  template<class OCTREE> class grid_types_b0 : public grid_types_detail::grid_types_root {
  public:
    typedef grid<OCTREE>                            grid_type;
    typedef OCTREE                                  octree_type;
    typedef typename octree_type::hier_grid_type    hier_grid_type;
    typedef typename hier_grid_type::flat_grid_type flat_grid_type;
    typedef typename hier_grid_type::flatgt         flatgt;
    typedef typename hier_grid_type::level_handle   level_handle;

    typedef typename flatgt::dimension_tag          dimension_tag;
    enum { dim = dimension_tag::dim };
    typedef typename octree_type::size_type         size_type;
    
    typedef hierarchical::h_element_handle_t<grid_type, typename flatgt::cell_handle>   cell_handle;
    typedef hierarchical::h_element_handle_t<grid_type, typename flatgt::facet_handle>  facet_handle;
    typedef hierarchical::h_element_handle_t<grid_type, typename flatgt::edge_handle>   edge_handle;
    typedef hierarchical::h_element_handle_t<grid_type, typename flatgt::vertex_handle> vertex_handle;

    // should come as nested templates from hier_grid_type / octree_type
    typedef hierarchical::h_vertex_t <element_base_t<octree_type> > vertex_base_type;
    //  typedef hierarchical::h_cell_t   <element_base_t<octree_type> > cell_base_type;

    template<class FLATELEM>
    struct element_base {
      typedef hierarchical::h_element_t<element_base_t<octree_type>, FLATELEM>   type;
      // typedef hierarchical::h_element_t<element_base_t<octree_type> > oct_elem_type;
      typedef typename hier_grid_type::template flat2hier<FLATELEM>   flat2hier;
      typedef typename flat2hier::type                       oct_elem_type;
      typedef leaf_elem_iterator<octree_type, oct_elem_type> leaf_iterator;
    };


    typedef vertex_t<octree_type>                                            Vertex;
    typedef element_t<octree_type, typename hier_grid_type::hier_cell_type>  Cell;
    typedef element_t<octree_type, typename hier_grid_type::hier_facet_type> Facet;
    typedef element_t<octree_type, typename hier_grid_type::hier_edge_type>  Edge;
    // ...
    
    typedef side_iterator_t<Cell, Facet, typename flatgt::FacetOnCellIterator> FacetOnCellIterator;
    typedef side_iterator_t<Facet, Edge, typename flatgt::EdgeOnFacetIterator> EdgeOnFacetIterator;



    typedef leaf_cell_iterator_t<element_base_t<octree_type> >                 CellIterator;
    //    typedef facet_iterator_of_cell_set<CellIterator, grid_types_b0<OCTREE> >    FacetIterator;
    typedef closure_iterator<CellIterator, FacetOnCellIterator, grid_types_b0<OCTREE> > FacetIterator;

    // does not work - facets without ancestors on root grid are not found.
    // typedef side_iterator_t<grid_type, Facet, typename flatgt::FacetIterator>  FacetIterator;
  };

  template<class GT, unsigned D> class grid_types_b_mixin_dim       : public GT {
    typedef side_iterator_t<typename GT::Facet, typename GT::Edge, typename GT::flatgt::EdgeOnFacetIterator> EdgeOnFacetIterator;
  };
  template<class GT>             class grid_types_b_mixin_dim<GT,2> : public GT {
  public:
    typedef typename GT::FacetIterator EdgeIterator;
  };
  template<class GT>             class grid_types_b_mixin_dim<GT,3> : public GT {
    typedef grid_types_b_mixin_dim<GT,3> self;
  public:
    typedef typename GT::flatgt flatgt;

    typedef side_iterator_t<typename GT::Facet, typename GT::Edge, typename flatgt::EdgeOnFacetIterator> EdgeOnFacetIterator;
    typedef closure_iterator<typename GT::FacetOnCellIterator, EdgeOnFacetIterator, self>                EdgeOnCellIterator;
    typedef closure_iterator<typename GT::CellIterator, EdgeOnCellIterator, self>                        EdgeIterator;
  };


  template<class OCTREE>
  class grid_types_b : public grid_types_b_mixin_dim<grid_types_b0<OCTREE>, grid_types_b0<OCTREE>::dim > {};



  template<class OCTREE>
  class element_base_t
  {
    typedef element_base_t<OCTREE> self;
  public:
    typedef grid_element_category       category;

    typedef grid_types_b<OCTREE>        gt;

    typedef typename gt::grid_type      grid_type;
    typedef grid_type                   leaf_grid_type;
    typedef typename gt::octree_type    octree_type;
    typedef typename gt::hier_grid_type hier_grid_type;
    typedef hier_grid_type              hgt;

    // types related to 'current' grid type
    typedef typename gt::Cell           Cell;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::cell_handle    cell_handle;
    typedef typename gt::vertex_handle  vertex_handle;
    
    // types related to hierarchical grid type
    typedef typename gt::level_handle   level_handle;
    typedef typename gt::flatgt         flatgt;
    typedef typename gt::flat_grid_type flat_grid_type;

    // types related to octree type
    typedef typename gt::octree_type ogt;
    typedef typename ogt::Cell       oct_cell_type;
    //    typedef typename gt::oct_cell_type  oct_cell_type;
  protected:
    ref_ptr<const grid_type> the_grid;
    level_handle             lev;

    void set(level_handle newlev) { lev = newlev;}
  public:
    element_base_t() {}
    element_base_t(grid_type const& g)
      : the_grid(g), lev(g.TheOctree()->finest_level()) {}
    element_base_t(ref_ptr<const grid_type> g)
      : the_grid(g), lev(g->TheOctree()->finest_level()) {}
    element_base_t(grid_type const& g, level_handle l)
      : the_grid(g), lev(l) {}
    element_base_t(ref_ptr<const grid_type> g, level_handle l)
      : the_grid(g), lev(l) {}

    bool bound() const { return the_grid != 0;}
    void cb()    const { REQUIRE(bound(), "", 1); }
    level_handle level() const { return lev;}

    ref_ptr<const flat_grid_type> TheFlatGrid() const { return TheHierGrid()->FlatGrid(level());}
    ref_ptr<const hier_grid_type> TheHierGrid() const { return TheOctree()->TheHierGrid();}
    ref_ptr<const octree_type>    TheOctree()   const { return TheGrid()->TheOctree();}
    ref_ptr<const grid_type>      TheGrid()     const { return the_grid;}
    //const grid_type&   TheGrid() const { return *the_grid;}
    ref_ptr<const leaf_grid_type> TheLeafGrid() const { return the_grid;}

    ref_ptr<const flat_grid_type> Grid(tp<flat_grid_type>) const { return TheFlatGrid();}
    ref_ptr<const hier_grid_type> Grid(tp<hier_grid_type>) const { return TheHierGrid();} 
    ref_ptr<const octree_type>    Grid(tp<octree_type>)    const { return TheOctree();} 
    ref_ptr<const grid_type>      Grid(tp<grid_type>)      const { return the_grid;}
  }; // class element_base_t<>




  template<class OCTREE>
  class vertex_t { 
    typedef vertex_t<OCTREE>                            self;
    typedef grid_types_b<OCTREE>                        gt;
    typedef typename gt::flatgt                         flatgt;
  public:
    typedef grid_element_category             category;
    typedef vertex_type_tag                   element_type_tag;
    typedef typename gt::grid_type            grid_type;

    typedef typename gt::vertex_handle        vertex_handle;
    typedef typename flatgt::Vertex           flat_vertex_type;
    typedef typename flatgt::vertex_handle    flat_vertex_handle;
    typedef typename gt::vertex_handle        element_handle;
    typedef typename flatgt::Vertex           flat_element_type;
    typedef typename flatgt::vertex_handle    flat_element_handle;

    typedef typename gt::level_handle         level_handle;

    typedef typename gt::octree_type                  octree_type;
    typedef typename gt::hier_grid_type               hier_grid_type;
    typedef typename hier_grid_type::hier_vertex_type hier_vertex_type;
    typedef hier_vertex_type                          hier_element_type;

  private:
    ref_ptr<grid_type const> g;
    hier_element_type        he;

  public:
    vertex_t() {}
    vertex_t(grid_type const& gg, element_handle hh) :  g(gg), he(TheHierGrid(), hh.flat(), hh.level()) 
    { normalize(); }
    vertex_t(grid_type const& gg, flat_element_type   f, level_handle lev) :  g(gg), he(TheHierGrid(), f, lev) 
    { normalize(); }
    vertex_t(grid_type const& gg, flat_element_handle f, level_handle lev) :  g(gg), he(TheHierGrid(), f, lev) 
    { normalize(); }
    vertex_t(ref_ptr<grid_type const> gg, element_handle hh) :  g(gg), he(TheHierGrid(), hh.flat(), hh.level()) 
    { normalize(); }
    vertex_t(ref_ptr<grid_type const> gg, flat_element_type   f, level_handle lev) :  g(gg), he(TheHierGrid(), f, lev)
    { normalize(); }
    vertex_t(ref_ptr<grid_type const> gg, flat_element_handle f, level_handle lev) :  g(gg), he(TheHierGrid(), f, lev)
    { normalize();}

    operator hier_element_type() const { return he;}
    hier_element_type Base()  const { return he;}
    flat_element_type Flat()  const { return Base().Flat();}
    level_handle      level() const { return Base().level();}
    
    ref_ptr<grid_type   const>    TheGrid()   const { return g;}
    ref_ptr<octree_type const>    TheOctree() const { return g->TheOctree();}
    ref_ptr<hier_grid_type const> TheHierGrid() const { return g->TheOctree()->TheHierGrid();}


    friend bool operator==(self const& lhs, self const& rhs) 
    { lhs.cv(); rhs.cv(); return lhs.level() == rhs.level() && lhs.Flat().handle() == rhs.Flat().handle();}
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
    friend bool operator< (self const& lhs, self const& rhs) { 
      lhs.cv(); rhs.cv(); 
      return lhs.level() < rhs.level() 
	|| ((lhs.level() == rhs.level()) && (lhs.Flat().handle() < rhs.Flat().handle()));
    }

  private:
    void cv() const { REQUIRE(he.valid(), "", 1);}
    //   void set(vertex_handle cv) { he = hier_element_type(TheHierGrid(),cv.flat(), cv.level());}
    void normalize() { he = TheHierGrid()->CoarsestParent(Base()); }
  };




  template<class OCTREE, class HIERELEM>
  class element_t  {
    typedef element_t<OCTREE,HIERELEM>         self;
  public:
    typedef grid_element_category              category;
    typedef grid_types_b<OCTREE>               gt;
    typedef typename gt::grid_type             grid_type;
    typedef typename gt::level_handle          level_handle;
    typedef typename gt::octree_type           octree_type;
    typedef typename gt::hier_grid_type        hier_grid_type;

    typedef HIERELEM                                        hier_element_type;
    typedef typename hier_element_type::element_handle      element_handle;
    typedef typename hier_element_type::flat_element_type   flat_element_type;
    typedef typename hier_element_type::flat_element_handle flat_element_handle;
    typedef element_traits<flat_element_type>               fet;
    typedef typename fet::element_type_tag                  element_type_tag;

  private:
    ref_ptr<grid_type const> g;
    hier_element_type        he;
  public:
    element_t() {}
    element_t(grid_type const& gg,         element_handle hh)                       : g(gg), he(TheHierGrid(), hh)   { cl(); }
    element_t(grid_type const& gg,         flat_element_type   f, level_handle lev) : g(gg), he(TheHierGrid(),f,lev) { cl(); }
    element_t(grid_type const& gg,         flat_element_handle f, level_handle lev) : g(gg), he(TheHierGrid(),f,lev) { cl(); }
    element_t(ref_ptr<grid_type const> gg, element_handle hh)                       : g(gg), he(TheHierGrid(), hh)   { cl(); }
    element_t(ref_ptr<grid_type const> gg, flat_element_type   f, level_handle lev) : g(gg), he(TheHierGrid(),f,lev) { cl(); }
    element_t(ref_ptr<grid_type const> gg, flat_element_handle f, level_handle lev) : g(gg), he(TheHierGrid(),f,lev) { cl(); }

    void cl() const { REQUIRE(is_leaf(), "Underlying hier. element is not a leaf!",1);}
    bool is_leaf() const { return TheOctree()->isLeaf(he);}

    operator hier_element_type() const { return he;}
    hier_element_type Base() const { return he;}
    flat_element_type Flat()  const { return Base().Flat();}
    level_handle      level() const { return Base().level();}

    ref_ptr<grid_type   const>    TheGrid()   const { return g;}
    ref_ptr<octree_type const>    TheOctree() const { return g->TheOctree();}
    ref_ptr<hier_grid_type const> TheHierGrid() const { return g->TheOctree()->TheHierGrid();}

    friend bool operator==(self const& lhs, self const& rhs) { return lhs.he == rhs.he; }
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
    friend bool operator< (self const& lhs, self const& rhs) { return lhs.he <  rhs.he; }

    //    side_iterator FirstSide() const;
  };


  namespace detail {

    template<class ET> 
    class et_hash_mixin : public ET {
      typedef ET bet;
    public: 
      class hasher_type {
	typedef typename bet::element_type                              element_type;
	typedef typename element_type::hier_element_type                base_element_type;
	typedef typename element_traits<base_element_type>::hasher_type base_hasher_type;
      public:
	typedef element_type                           key_type;
	typedef element_type                           argument_type;
	typedef typename base_hasher_type::result_type result_type;

	result_type operator()(element_type e) const {
	  base_hasher_type h;
	  return h(e.Base());
	}
      };
    };

    template<class E> class et_base 
      : public et_hash_mixin<element_traits_base<E> > {};

    
  } // namespace detail

  template<class OCTREE>
  class grid {
  public:
    //typedef grid_category               category;
    typedef grid_range_category               category;

    typedef grid_types_b<OCTREE>        gt;
    typedef typename gt::grid_type      grid_type;
    typedef typename gt::octree_type    octree_type;
    typedef typename gt::hier_grid_type hier_grid_type;
    typedef typename gt::level_handle   level_handle;

    //   typedef typename gt::CellIterator   CellIterator;
  private:
    ref_ptr<octree_type const> oct;
  public:
    enum { dim = octree_type::dim};

    grid() {}
    grid(octree_type const& o)         : oct(o) {}
    grid(ref_ptr<octree_type const> o) : oct(o) {}

    ref_ptr<grid_type      const> TheGrid()     const { return ref_ptr<grid_type   const>(*this);}
    ref_ptr<octree_type    const> TheOctree()   const { return oct;}
    ref_ptr<hier_grid_type const> TheHierGrid() const { return oct->TheHierGrid();}

    typename gt::CellIterator  FirstCell () const { return typename gt::CellIterator (*this);}
    typename gt::FacetIterator FirstFacet() const { return typename gt::FacetIterator(*this);}
    //    typename gt::FacetIterator FirstFacet() const { return typename gt::FacetIterator(*this);}
  };


  // with flat_side_iterator a template parameter,
  // we could use this to implement also other iterators.
  // EdgeIterator: anchor=grid, side_iterator=flat_edge_iterator, side=edge, leaf default.
  // Problems: 
  //  - does get only edges with ancestors on root grid => Cannot use it!
  //  - copy or ref anchor? (Copy-to-ref-ptr when "light")
  //  - level() ? Add. arg
  //  - falt_iterator i must have some leafs ... e.g. flatgt::FacetIterator over non-active facets?

  template<class ANCHOR, class ELEM, class ELEMITER>  // , class LEAFITER>
  class side_iterator_t {
    typedef side_iterator_t<ANCHOR,ELEM, ELEMITER> self;
  public:
    typedef ANCHOR       anchor_type;
    typedef ELEM         element_type;
    typedef ELEMITER     flat_iterator;
    
    typedef element_type value_type;

    typedef typename anchor_type::gt         gt;
    typedef typename gt::grid_type           grid_type;
    typedef typename gt::octree_type         octree_type;
    typedef typename gt::hier_grid_type      hier_grid_type;

    typedef typename gt::level_handle        level_handle;

    //    typedef LEAFITERATOR leaf_iterator;
    typedef typename element_type::hier_element_type           hier_element_type;
    typedef leaf_elem_iterator<octree_type, hier_element_type> leaf_iterator;
    
    typedef typename element_type::flat_element_type flat_element_type;
    // typedef typename anchor_type::flat_side_iterator flat_iterator;
    typedef typename element_type::element_handle    element_handle;

  private:
    ref_ptr<anchor_type const> anchor;
    flat_iterator       side;
    leaf_iterator       leaf;
    level_handle        lev;  
  public:
    side_iterator_t() {}
    // anchor type is a "light" type (element)
    side_iterator_t(anchor_type const& a) { init(a); }
    //      : anchor(copy_to_ref_ptr(a)), side(a.Flat()), lev(a.level()) { init(); }
   
    //side_iterator_t(anchor_type const& a, flat_iterator i) 
    // : anchor(copy_to_ref_ptr(a)), side(i), lev(a.level())             { init();}

    void init(anchor_type const& a) { init(a, typename anchor_type::category());}
    void init(anchor_type const& a, grid_element_category) { 
      anchor = copy_to_ref_ptr(a); 
      lev    = a.level();
      side   = flat_iterator(a.Flat()); 
      init();
    }
    void init(anchor_type const& a, grid_range_category   ) {
      anchor = const_ref_to_ref_ptr(a); 
      lev    = a.TheHierGrid()->coarsest_level();
      side   = flat_iterator(a.TheOctree()->LevelGrid(lev)); 
      init();
    }

    // anchor type is a "heavy" type (grid)
    side_iterator_t(anchor_type         const& a, flat_iterator i, level_handle l) : anchor(a), side(i), lev(l) { init();}
    side_iterator_t(ref_ptr<anchor_type const> a, flat_iterator i, level_handle l) : anchor(a), side(i), lev(l) { init();}

    element_type   operator*() const { return element_type(TheGrid(), handle());}
    element_handle handle()    const { return (*leaf).handle();}
    bool           IsDone()    const { return side.IsDone();}
    
    self& operator++()  {
      ++leaf;
      if(leaf.IsDone()) {
	++side;
	if(!side.IsDone())
	  set_leaf(); 
      }
      return *this;
    }
    GrAL::ref_ptr<grid_type   const>     TheGrid()     const { return anchor->TheGrid();}
    GrAL::ref_ptr<octree_type const>     TheOctree()   const { return anchor->TheOctree();}
    GrAL::ref_ptr<hier_grid_type const>  TheHierGrid() const { return anchor->TheHierGrid();}
    level_handle level() const { return lev;}

    bool bound() const { return anchor != 0; }
    bool valid() const { return bound() && side.valid();}
    // bool operator==(self const& rhs) const { return side == rhs.side && leaf == rhs.leaf;}
  private:
    void init() { set_leaf();}
    void set_leaf() { leaf = leaf_iterator(TheOctree(), hier_element_type(TheHierGrid(),*side, level())); }
    };

}}} // namespace GrAL { namespace octree { namespace conforming_leaf_grid {




namespace GrAL {

  template<class OCTREE>
  struct grid_types<octree::conforming_leaf_grid::grid<OCTREE> >
    : public grid_types_base<octree::conforming_leaf_grid::grid_types_b<OCTREE> >
  {};


  template<class OCTREE, class HIERELEM>
  struct element_traits<octree::conforming_leaf_grid::element_t<OCTREE,HIERELEM> >
    : public octree::conforming_leaf_grid::detail::et_base<octree::conforming_leaf_grid::element_t<OCTREE,HIERELEM> > {};

  template<class OCTREE>
  struct element_traits<octree::conforming_leaf_grid::vertex_t<OCTREE> >
    : public octree::conforming_leaf_grid::detail::et_base<octree::conforming_leaf_grid::vertex_t<OCTREE> > {};


  template<class OCTREE, class HIERELEM, class T>
  class grid_function<octree::conforming_leaf_grid::element_t<OCTREE,HIERELEM>, T>
    : public grid_function_hash<octree::conforming_leaf_grid::element_t<OCTREE,HIERELEM>, T>
  {
    typedef grid_function_hash<octree::conforming_leaf_grid::element_t<OCTREE,HIERELEM>, T> base;
  public:
    typedef typename base::grid_type grid_type;
 
    grid_function() {}
    grid_function(grid_type const& g)             : base(g) {}
    grid_function(grid_type const& g, T const& t) : base(g,t) {}
    grid_function(ref_ptr<grid_type const> g)             : base(g) {}
    grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
  };

  template<class OCTREE, class T>
  class grid_function<octree::conforming_leaf_grid::vertex_t<OCTREE>, T>
    : public grid_function_hash<octree::conforming_leaf_grid::vertex_t<OCTREE>, T>
  {
    typedef grid_function_hash<octree::conforming_leaf_grid::vertex_t<OCTREE>, T> base;
  public:
    typedef typename base::grid_type grid_type;
 
    grid_function() {}
    grid_function(grid_type const& g)             : base(g) {}
    grid_function(grid_type const& g, T const& t) : base(g,t) {}
    grid_function(ref_ptr<grid_type const> g)             : base(g) {}
    grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
  };



} // namespace GrAL




#endif
