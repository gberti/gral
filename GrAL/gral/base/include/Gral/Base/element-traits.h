#ifndef GRAL_BASE_GB_ELEMENT_TRAITS_H
#define GRAL_BASE_GB_ELEMENT_TRAITS_H


// $LICENSE

//----------------------------------------------------------------
/*!  \brief traits classes for enabling uniform treatment
      of different element types (vertex, edge etc).
      \ingroup traits

      Used for example in the class template grid_function<E,T>.

      This has to be specialized for concrete element classes, e.g.:
      \code
      class MyVertex;
      struct element_traits<MyVertex> {
        typedef  grid_type;
        typedef  element_type;
        typedef  ElementIterator;
        typedef  element_handle;
        typedef  hasher_type;
        typedef  element_type_tag;

        static ElementIterator FirstElement(const grid_type& g);
        static unsigned        size        (const grid_type& g);
        static handle_type     handle      (element_type const& e);
      };
      \endcode
*/
//----------------------------------------------------------------


template<class E>
struct element_traits {};

/*! \brief basic definition to derive from for actual specializations
     of element_traits<>
  \ingroup traits
  \relates element_traits
*/
template<class GRID>
struct element_traits_vertex_base {
  typedef GRID                           grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Vertex            element_type;
  typedef typename gt::VertexIterator    ElementIterator;
  typedef typename gt::vertex_handle     handle_type;
  typedef vertex_type_tag                element_type_tag;


  static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstVertex();}
  static unsigned        size        (grid_type    const& g)    { return g.NumOfVertices();}
  static handle_type     handle      (element_type const& e)    { return e.handle();}
  static element_type    handle2element(grid_type const& g, handle_type h)
  { 
    return element_type(g,h); // could be factored out.
  }
};


/*! \brief basic definition to derive from for actual specializations
     of element_traits<>
  \ingroup traits
  \relates element_traits
*/
template<class GRID>
struct element_traits_edge_base {
  typedef GRID grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Edge              element_type;
  typedef typename gt::EdgeIterator      ElementIterator;
  typedef typename gt::edge_handle       handle_type;
  typedef edge_type_tag                  element_type_tag;


  static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstEdge();}
  static unsigned        size        (grid_type    const& g)    { return g.NumOfEdges();}
  static handle_type     handle      (element_type const& e)    { return e.handle();}
  static element_type    handle2element(grid_type const& g, handle_type h)
  {
    return element_type(g,h); // could be factored out.
  }
};


/*! \brief basic definition to derive from for actual specializations
     of element_traits<>
  \ingroup traits
  \relates element_traits
*/

template<class GRID>
struct element_traits_edge2d_base 
  : public element_traits_edge_base<GRID> {
  typedef edge2d_type_tag element_type_tag;
};



/*! \brief basic definition to derive from for actual specializations
     of element_traits<>
  \ingroup traits
  \relates element_traits
*/
template<class GRID>
struct element_traits_facet_base {
  typedef GRID grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Facet              element_type;
  typedef typename gt::FacetIterator      ElementIterator;
  typedef typename gt::facet_handle       handle_type;
  typedef facet_type_tag                  element_type_tag;


  static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstFacet();}
  static unsigned        size        (grid_type    const& g)    { return g.NumOfFacets();}
  static handle_type     handle      (element_type const& e)    { return e.handle();}
  static element_type    handle2element(grid_type const& g, handle_type h)
  { 
    return element_type(g,h); // could be factored out.
  }
};


/*! \brief basic definition to derive from for actual specializations
     of element_traits<>
  \ingroup traits
  \relates element_traits
*/
template<class GRID>
struct element_traits_cell_base {
  typedef GRID grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Cell              element_type;
  typedef typename gt::CellIterator      ElementIterator;
  typedef typename gt::cell_handle       handle_type;
  typedef cell_type_tag                  element_type_tag;


  static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstCell();}
  static unsigned        size        (grid_type    const& g)    { return g.NumOfCells();}
  static handle_type     handle      (element_type const& e)    { return e.handle();}
  static element_type    handle2element(grid_type const& g, handle_type h)
  { 
    return element_type(g,h); // could be factored out.
  }
};


/*! \brief basic definition to derive from for actual specializations
     of element_traits<>
  \ingroup traits
  \relates element_traits
*/
template<class GRID>
struct element_traits_cell2d_base
  : public element_traits_cell_base<GRID>
{
   typedef cell2d_type_tag                element_type_tag;
};


#endif
