#ifndef GRAL_GB_CARTESIANND_ARCHETYPES_H
#define GRAL_GB_CARTESIANND_ARCHETYPES_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"

namespace GrAL { namespace cartesiannd {

  template<class CARTGRID>
  class archetype_t;


  template<class CARTELEM>
  class archetype_element_t;
  template<class CARTIT>
  class archetype_sequence_iterator_t;
  template<class CARTIT>
  class archetype_incidence_iterator_t;


  template<class CARTGRID>
  struct grid_types_arch_base : public grid_types_detail::grid_types_root {
    // typedef typename archetype_of_grid<CARTGRID>::type   grid_type;
    typedef archetype_t<CARTGRID>    grid_type;
    enum { dim = CARTGRID::dim - 1 };
    typedef grid_dim_tag<dim>                            dimension_tag;
    typedef grid_types<CARTGRID>                bgt;

    typedef typename bgt::size_type         size_type;
    typedef typename bgt::vertex_index_type vertex_index_type;
    typedef typename bgt::cell_index_type   cell_index_type;
    typedef typename bgt::index_type        index_type;

    typedef typename bgt::facet_handle  cell_handle;
    typedef typename bgt::vertex_handle vertex_handle;
  
    typedef archetype_element_t<typename bgt::Vertex>     Vertex;
    typedef archetype_element_t<typename bgt::Facet>      Cell;
    typedef archetype_sequence_iterator_t <typename bgt::VertexOnCellIterator>  VertexIterator;
    typedef archetype_sequence_iterator_t <typename bgt::FacetOnCellIterator>   CellIterator;

    typedef archetype_incidence_iterator_t<typename bgt::VertexOnFacetIterator> VertexOnCellIterator;
    typedef archetype_incidence_iterator_t<typename bgt::VertexOnEdgeIterator>  VertexOnEdgeIterator;
    typedef archetype_incidence_iterator_t<typename GrAL::incidence_iterator_d<bgt,dim-1,dim>::type> FacetOnCellIterator;

    
    typedef typename bgt::edge_handle edge_handle;
    typedef archetype_element_t           <typename bgt::Edge>  Edge;
    typedef archetype_sequence_iterator_t <typename bgt::EdgeOnCellIterator>  EdgeIterator;

    typedef typename bgt::face_handle face_handle;
    typedef archetype_element_t           <typename bgt::Face>          Face;
    typedef archetype_sequence_iterator_t <typename bgt::FaceOnCellIterator>  FaceIterator;

    typedef typename element_handle_d<bgt,dim-1>::type facet_handle;
    //    typedef typename Facet::element_handle   facet_handle;
    typedef archetype_element_t           <typename element_d          <bgt,dim-1>::type>  Facet;
    typedef archetype_sequence_iterator_t <typename incidence_iterator_d<bgt,dim-1,dim+1>::type>  FacetIterator;
    //  typedef ??? archetype_type;
  };


  template<class CARTELEM>
  class archetype_element_t {
    typedef archetype_element_t<CARTELEM> self;    
  public:
    typedef CARTELEM                             base_element;
    typedef typename base_element::grid_type     base_grid_type;
    typedef grid_types_arch_base<base_grid_type> gt;

    enum { dim = base_element::dim };
    typedef grid_element_category_d<dim>           category;
    typedef typename int2etag<gt,dim>::type        element_type_tag;
    typedef archetype_t<base_grid_type>            grid_type;
    typedef typename base_element::element_handle  element_handle;
    typedef typename gt::size_type                 size_type;
  private:
    ref_ptr<grid_type const> g;
    base_element             e;
  public:
    archetype_element_t() {}
    archetype_element_t(        grid_type const& gg, element_handle h) : g(gg), e(gg .BaseGrid(),h) {}
    archetype_element_t(ref_ptr<grid_type const> gg, element_handle h) : g(gg), e(gg->BaseGrid(),h) {}
    archetype_element_t(        grid_type const& gg, base_element   b) : g(gg), e(b) {}
    archetype_element_t(ref_ptr<grid_type const> gg, base_element   b) : g(gg), e(b) {}
    
    grid_type    const& TheGrid() const { return *g;}
    base_element const& Base()    const { return e;}
    element_handle handle() const { return e.handle();}

    bool bound() const { return g != 0;}
    bool valid() const { return e.valid();}
    bool operator==(self const& rhs) const { return e == rhs.e;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };


  template<class CARTIT>
  class archetype_sequence_iterator_t {
    typedef archetype_sequence_iterator_t<CARTIT> self;
  public:
    typedef CARTIT                                base_iterator;
    typedef typename base_iterator::grid_type     base_grid_type;
    typedef typename base_iterator::value_type    base_element_type;
    
  public:
    typedef grid_sequence_iterator_category        category;
    typedef archetype_t<base_grid_type>            grid_type;
    typedef archetype_element_t<base_element_type> element_type;
    typedef typename element_type::element_handle  element_handle;
    typedef grid_type                              anchor_type;
    typedef element_type                           value_type;
  private:
    ref_ptr<grid_type const> g;
    base_iterator            it;
  public:
    archetype_sequence_iterator_t() {}
    archetype_sequence_iterator_t(        grid_type const& gg) : g(gg), it(gg .TheCell()) {}
    archetype_sequence_iterator_t(ref_ptr<grid_type const> gg) : g(gg), it(gg->TheCell()) {}
    archetype_sequence_iterator_t(        grid_type const& gg, base_iterator i) : g(gg), it(i) {}
    archetype_sequence_iterator_t(ref_ptr<grid_type const> gg, base_iterator i) : g(gg), it(i) {}
    
    self& operator++() { ++it; return *this;}
    bool  IsDone() const { return it.IsDone();}
    element_type operator*() const { return element_type(g,*it);}
    element_handle handle()  const { return it.handle();}

    grid_type   const& TheGrid()   const { return *g;}
    anchor_type const& TheAnchor() const { return *g;}
    
    bool operator==(self const& rhs) const { return it == rhs.it;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
   };


  template<class CARTIT>
  class archetype_incidence_iterator_t {
    typedef archetype_incidence_iterator_t<CARTIT> self;
  public:
    typedef CARTIT                                 base_iterator;
    typedef typename base_iterator::grid_type     base_grid_type;
    typedef typename base_iterator::value_type    base_element_type;
    typedef typename base_iterator::anchor_type   base_anchor_type;

    typedef grid_incidence_iterator_category       category;
    typedef archetype_t<base_grid_type>            grid_type;
    typedef archetype_element_t<base_element_type> element_type;
    typedef element_type                           value_type;
    typedef archetype_element_t<base_anchor_type>  anchor_type;
    typedef typename element_type::element_handle  element_handle;
  private:
    anchor_type        a;
    base_iterator      it;
  public:
    archetype_incidence_iterator_t() {}
    archetype_incidence_iterator_t(anchor_type const& aa) : a(aa), it(aa.Base()) {}
    archetype_incidence_iterator_t(anchor_type const& aa, base_iterator i) : a(aa), it(i) {}
    
    self& operator++() { ++it; return *this;}
    bool  IsDone() const { return it.IsDone();}
    element_type operator*() const { return element_type(TheGrid(),*it);}
    element_handle handle() const { return it.handle();}

    grid_type   const& TheGrid()   const { return a.TheGrid();}
    anchor_type        TheAnchor() const { return a;}
    
    bool operator==(self const& rhs) const { return it == rhs.it;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
   };




  /*! \brief Archetype type for n-dimensional Cartesian grids
      \ingroup cartesianndmodule
      
   */
  template<class CARTGRID>
  class archetype_t {
    typedef archetype_t<CARTGRID>      self;

    typedef grid_types_arch_base<CARTGRID>                gt;
    typedef grid_types<CARTGRID>                          bgt;

    CARTGRID g; // single D-dimensional cell
  public:
    enum { dim = CARTGRID::dim -1};
    typedef grid_dim_tag<dim>          dimension_tag;
    typedef GrAL::grid_category_d<dim> category;
    typedef self                       grid_type;

    typedef typename gt::size_type     size_type;
    typedef typename gt::Cell          Cell;
    typedef typename gt::Vertex        Vertex;
    typedef typename gt::Edge          Edge;
    typedef typename gt::Face          Face;
    typedef typename gt::Facet         Facet;

    typedef typename gt::cell_handle   cell_handle;
    typedef typename gt::vertex_handle vertex_handle;

    typedef typename gt::VertexIterator       VertexIterator; 
    typedef typename gt::CellIterator         CellIterator; 
    typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
    typedef typename gt::vertex_index_type     vertex_index_type;
    typedef typename gt::index_type            index_type;

    archetype_t() 
    {
      delta_map<CARTGRID::dim>::init();
      g.init(vertex_index_type(0), vertex_index_type(2));
      cg();
      REQUIRE_ALWAYS(g.NumOfCells() == 1, "g.NumOfCells()=" << g.NumOfCells(), 1);
    }
    void print(std::ostream& out) const { g.print(out);}

    void cg() const {
      REQUIRE(g.NumOfCells() == 1, "g.NumOfCells()=" << g.NumOfCells(), 1);
    }
    // for having grid functions on archetype ...
    operator CARTGRID const&() const { cg(); return g;}

    CARTGRID const& BaseGrid() const { cg(); return g;}
    typename bgt::Cell TheCell() const { return * g.FirstCell();}

    size_type NumOfVertices() const { cg(); return g.NumOfVertices();}
    size_type NumOfCells()    const { cg(); return g.NumOfFacets();}

    template<unsigned K>
    struct sequence_iter {
      typedef archetype_sequence_iterator_t<typename incidence_iterator_d<bgt,K,dim+1>::type> type;
    };

    template<unsigned K>
    typename sequence_iter<K>::type FirstElement() const 
    { cg(); return typename sequence_iter<K>::type(*this, TheCell().template FirstElement<K>());}
 
    template<unsigned K>
    typename sequence_iter<K>::type EndElement() const 
    { cg(); return typename sequence_iter<K>::type(*this, TheCell().template EndElement<K>());}

    template<class CARTIT>
    archetype_sequence_iterator_t<CARTIT> FirstElement() const 
    { cg(); return archetype_sequence_iterator_t<CARTIT>(*this, TheCell().template FirstElement<CARTIT::value_type::dim>());}
 
    template<class CARTIT>
    archetype_sequence_iterator_t<CARTIT> EndElement() const 
    { cg(); return archetype_sequence_iterator_t<CARTIT>(*this, TheCell().template EndElement<CARTIT::value_type::dim>());}

    template<unsigned K>
    size_type NumOfElements() const { return TheCell().template NumOfElements<K>();}
 
    template<class CARTIT>
    size_type NumOfElements() const { return TheCell().template NumOfElements<CARTIT::value_type::dim>();}


    // This grid has no boundary
    template<class CARTELEM>
    bool IsOnBoundary(archetype_element_t<CARTELEM>) const { return false;}
    template<class CARTIT>
    bool IsOnBoundary(archetype_incidence_iterator_t<CARTIT>) const { return false;}

    VertexIterator FirstVertex() const { cg(); return FirstElement<0>();}
    VertexIterator EndVertex()   const { cg(); return EndElement<0>();}
    CellIterator   FirstCell()   const { cg(); return FirstElement<dim>();}
    CellIterator   EndCell()     const { cg(); return EndElement<dim>();}


   
    void switch_vertex(Vertex      & v, Edge const& e)  const
    {
      typename bgt::Vertex vb(v.Base());
      g.switch_element(vb, e.Base());
      v = Vertex(v.TheGrid(), vb);
    }
    void switch_cell(Facet const& f, Cell & c) const {
      typename bgt::Facet bf(c.Base());
      g.switch_element(f.Base(),bf, TheCell());
      c = Cell(c.TheGrid(),bf);
    }
    void switch_edge(Vertex const& v, Edge& e, Face const& f)  const {
      typename bgt::Edge be(e.Base());
      g.switch_element(v.Base(), be, f.Base());
      e = Edge(e.TheGrid(), be);
    }

    Vertex switched_vertex(Vertex const& v, Cell const& e) const { Vertex w=v; switch_vertex(w,e); return w;}
    Edge   switched_edge  (Vertex const& v, Edge const& e, Face const& f) const { Edge ee=e; switch_edge(ee); return ee;}
    Cell   switched_cell  (Facet  const& f, Cell const& c) const { Cell cc(c); switch_cell(f,cc); return cc;}


    typedef typename CARTGRID::facet_grid_type            facet_grid_type;
    typedef grid_types<facet_grid_type>                   fgt;
    typedef typename fgt::archetype_iterator archetype_iterator;
    typedef typename fgt::archetype_handle   archetype_handle;
    typedef typename fgt::archetype_type     archetype_type;

    static archetype_type const&  ArchetypeOf (Cell const&) { return facet_grid_type::TheArchetype();}
    static archetype_type const&  ArchetypeOf (cell_handle) { return facet_grid_type::TheArchetype();}
    static archetype_handle       archetype_of(Cell const&) { return facet_grid_type::the_archetype();}
    static archetype_handle       archetype_of(cell_handle) { return facet_grid_type::the_archetype();}
 
    static archetype_type const&  Archetype(archetype_handle a) { return facet_grid_type::Archetype(a);}

    static archetype_iterator BeginArchetype() { return facet_grid_type::BeginArchetype();}
    static archetype_iterator EndArchetype()   { return facet_grid_type::EndArchetype();}
    static archetype_handle   handle(archetype_iterator it) { return facet_grid_type::handle(it);}
    static int                NumOfArchetypes() { return facet_grid_type::NumOfArchetypes();}
    
  };

} // namespace cartesiannd


template<class CART>
struct grid_types<cartesiannd::archetype_t<CART> >
  : public grid_types_base<cartesiannd::grid_types_arch_base<CART> > 
{
  typedef cartesiannd::archetype_t<CART>  grid_type;
  typedef cartesiannd::archetype_t<CART>  at;
  typedef typename at::archetype_iterator archetype_iterator;
  typedef typename at::archetype_handle   archetype_handle;
  typedef typename at::archetype_type     archetype_type;

};



  template<class CARTELEM>
  struct element_traits<cartesiannd::archetype_element_t<CARTELEM> >
    : public element_traits_base<cartesiannd::archetype_element_t<CARTELEM> > 
  {
    typedef element_traits_base<cartesiannd::archetype_element_t<CARTELEM> > base;
    typedef consecutive_integer_tag<0>                 consecutive_tag;
    typedef typename base::hasher_type_elem_base       hasher_type;
  };


  namespace cartesiannd {

    template<class CART, class CARTIT>
    archetype_sequence_iterator_t<CARTIT>
    gral_begin(archetype_t<CART> const& a, archetype_sequence_iterator_t<CARTIT>)
    { return a.template FirstElement<CARTIT>();}

    template<class CART, class CARTIT>
    archetype_sequence_iterator_t<CARTIT>
    gral_end  (archetype_t<CART> const& a, archetype_sequence_iterator_t<CARTIT>)
    { return a.template EndElement<CARTIT>();}
    
    template<class CART, class CARTIT>
    typename archetype_t<CART>::size_type
    gral_size (archetype_t<CART> const& a, archetype_sequence_iterator_t<CARTIT>)
    { return a.template NumOfElements<CARTIT>();}
    

    template<class CARTIT, class CARTELEM>
    archetype_incidence_iterator_t<CARTIT>
    gral_begin(archetype_element_t<CARTELEM> const& a, archetype_incidence_iterator_t<CARTIT>) {
      typedef archetype_incidence_iterator_t<CARTIT> it;
      return it(a, GrAL::begin<typename it::base_element_type>(a.Base()));
    }
    
    template<class CARTIT, class CARTELEM>
    archetype_incidence_iterator_t<CARTIT>
    gral_end  (archetype_element_t<CARTELEM> const& a, archetype_incidence_iterator_t<CARTIT>) {
      typedef archetype_incidence_iterator_t<CARTIT> it;
      return it(a, GrAL::end<typename it::base_element_type>(a.Base()));
    }
    
    template<class CARTIT, class CARTELEM>
    typename archetype_element_t<CARTELEM>::size_type
    gral_size  (archetype_element_t<CARTELEM> const& a, archetype_incidence_iterator_t<CARTIT>) {
      typedef archetype_incidence_iterator_t<CARTIT> it;
      return GrAL::size<typename it::base_element_type>(a.Base());
    }

} // namespace cartesiannd



/*! \brief Specialization for archetype elements of \c cartesiannd::grid<DIM>
    \ingroup cartesianndgf
 */
template<class CARTELEM, class T>
class grid_function<cartesiannd::archetype_element_t<CARTELEM>, T >
  : public grid_function_vector<cartesiannd::archetype_element_t<CARTELEM>, T >
{
  typedef grid_function_vector<cartesiannd::archetype_element_t<CARTELEM>, T > base;
public:
  typedef typename cartesiannd::archetype_element_t<CARTELEM>::grid_type       grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


} // namespace GrAL 

#endif
