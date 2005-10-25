#ifndef GRAL_BASE_GB_ELEMENT_TRAITS_H
#define GRAL_BASE_GB_ELEMENT_TRAITS_H

#include <cstddef>

#include "Utility/ref-ptr.h"

// $LICENSE

namespace GrAL {

/*! \defgroup elementtraits Traits classes for grid elements

      \ingroup traits
      \ingroup elements

      These traits classes enable uniform treatment
      of different element types (vertex, edge etc).

      Used for example in the class template <tt> grid_function <E,T> </tt>.

      This has to be specialized for concrete element classes, e.g.:

      \code
      class MyVertex { ... };

      template<>
      struct element_traits<MyVertex> {
        typedef  grid_type;
        typedef  element_type;
        typedef  ElementIterator;
        typedef  element_handle;
        typedef  hasher_type;
        typedef  element_type_tag;
	typedef  consecutive_tag; // elements consecutively numbered?
	typedef  grid_dimension_tag;
	typedef  element_dimension_tag;
	typedef  element_codimension_tag;

        static ElementIterator FirstElement(const grid_type& g);
        static ElementIterator EndElement  (const grid_type& g);
        static unsigned        size        (const grid_type& g);
        static handle_type     handle      (element_type const& e);
        static element_type    element     (grid_type const& g, 
                                            handle_type      h);
      };
      \endcode

      All of these (except hasher_type) can be obtained
      by inheriting from a default definition, 
      in this case for vertex types:

      \code
      template<>
      struct element_traits<MyVertex> 
         : public element_traits_vertex_base<MyVertex::grid_type> 
      {
             typedef xxx hasher_type;
      };
      \endcode
*/

/*! \brief Primary template to be specialized
    \ingroup elementtraits
 */
template<class E>
struct element_traits {};


/*! \brief Tag for providing info on numbering of elements

    \ingroup elementtraits

    Define <tt> element_traits<MyElement>::consecutive_tag </tt> to be \c non_consecutive_tag
    if this element type is not numbered consecutively.
    (This is the default.)
 */
struct non_consecutive_tag {};


/*! \brief Tag for providing info on numbering of elements

    \ingroup elementtraits

    Define <tt> element_traits<MyElement>::consecutive_tag </tt> to be \c consecutive_integer_tag<OFFSET>
    if this element type is  numbered consecutively starting from \c OFFSET:

    \code
      template<>
      struct element_traits<MyVertex> 
         : public element_traits_vertex_base<MyVertex::grid_type> 
      {
         typedef consecutive_integer_tag<0> consecutive_tag;
         // ...
      };
    \endcode
 */
template<int OFFSET>
struct consecutive_integer_tag 
{ enum { offset = OFFSET }; };


  namespace et_detail {

    template<class GRID>
    struct element_traits_base 
    {
      typedef GRID             grid_type;
      typedef grid_types<GRID> gt;
      // default: elements not numbered consecutively
      typedef non_consecutive_tag            consecutive_tag; 
      
      template<class G, class E, class H>
      struct hasher_type_base {
	typedef E      key_type;
	typedef E      argument_type;
	typedef size_t result_type;
	
	// default implementation
	result_type operator()(key_type const& k) const { return k.handle();}
	result_type operator()(H               h) const { return h;}
      };
    };


    template<class ET, bool HasVertexIterator>
    struct et_vertex_iterator_mixin : public ET {};
    template<class ET>
    struct et_vertex_iterator_mixin<ET, true> : public ET 
    {
      typedef typename ET::gt        gt;
      typedef typename gt::grid_type grid_type;

      typedef typename gt::VertexIterator      ElementIterator;
      static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstVertex();}
      static ElementIterator EndElement  (grid_type    const& g)    { return g.EndVertex();}
      static unsigned        size        (grid_type    const& g)    { return g.NumOfVertices();}
      static ElementIterator FirstElement(ref_ptr<grid_type    const> g)    { return g->FirstVertex();}
      static ElementIterator EndElement  (ref_ptr<grid_type    const> g)    { return g->EndVertex();}
      static unsigned        size        (ref_ptr<grid_type    const> g)    { return g->NumOfVertices();}
    };


    template<class ET, bool HasEdgeIterator>
    struct et_edge_iterator_mixin : public ET {};
    template<class ET>
    struct et_edge_iterator_mixin<ET, true> : public ET 
    {
      typedef typename ET::gt        gt;
      typedef typename gt::grid_type grid_type;

      typedef typename gt::EdgeIterator      ElementIterator;
      static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstEdge();}
      static ElementIterator EndElement  (grid_type    const& g)    { return g.EndEdge();}
      static unsigned        size        (grid_type    const& g)    { return g.NumOfEdges();}
      static ElementIterator FirstElement(ref_ptr<grid_type    const> g)    { return g->FirstEdge();}
      static ElementIterator EndElement  (ref_ptr<grid_type    const> g)    { return g->EndEdge();}
      static unsigned        size        (ref_ptr<grid_type    const> g)    { return g->NumOfEdges();}
    };


    template<class ET, bool HasFaceIterator>
    struct et_face_iterator_mixin : public ET {};
    template<class ET>
    struct et_face_iterator_mixin<ET, true> : public ET 
    {
      typedef typename ET::gt        gt;
      typedef typename gt::grid_type grid_type;

      typedef typename gt::FaceIterator      ElementIterator;
      static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstFace();}
      static ElementIterator EndElement  (grid_type    const& g)    { return g.EndFace();}
      static unsigned        size        (grid_type    const& g)    { return g.NumOfFaces();}
      static ElementIterator FirstElement(ref_ptr<grid_type    const> g)    { return g->FirstFace();}
      static ElementIterator EndElement  (ref_ptr<grid_type    const> g)    { return g->EndFace();}
      static unsigned        size        (ref_ptr<grid_type    const> g)    { return g->NumOfFaces();}
    };



    template<class ET, bool HasFacetIterator>
    struct et_facet_iterator_mixin : public ET {};
    template<class ET>
    struct et_facet_iterator_mixin<ET, true> : public ET 
    {
      typedef typename ET::gt        gt;
      typedef typename gt::grid_type grid_type;

      typedef typename gt::FacetIterator      ElementIterator;
      static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstFacet();}
      static ElementIterator EndElement  (grid_type    const& g)    { return g.EndFacet();}
      static unsigned        size        (grid_type    const& g)    { return g.NumOfFacets();}
      static ElementIterator FirstElement(ref_ptr<grid_type    const> g)    { return g->FirstFacet();}
      static ElementIterator EndElement  (ref_ptr<grid_type    const> g)    { return g->EndFacet();}
      static unsigned        size        (ref_ptr<grid_type    const> g)    { return g->NumOfFacets();}
    };


    template<class ET, bool HasCellIterator>
    struct et_cell_iterator_mixin : public ET {};
    template<class ET>
    struct et_cell_iterator_mixin<ET, true> : public ET 
    {
      typedef typename ET::gt        gt;
      typedef typename gt::grid_type grid_type;

      typedef typename gt::CellIterator      ElementIterator;
      static ElementIterator FirstElement(grid_type    const& g)    { return g.FirstCell();}
      static ElementIterator EndElement  (grid_type    const& g)    { return g.EndCell();}
      static unsigned        size        (grid_type    const& g)    { return g.NumOfCells();}
      static ElementIterator FirstElement(ref_ptr<grid_type    const> g)    { return g->FirstCell();}
      static ElementIterator EndElement  (ref_ptr<grid_type    const> g)    { return g->EndCell();}
      static unsigned        size        (ref_ptr<grid_type    const> g)    { return g->NumOfCells();}
    };




  } //  namespace et_detail

/*! \brief basic definition to derive from for actual specializations
     of \c element_traits for vertex types
  \ingroup elementtraits
*/
template<class GRID>
struct element_traits_vertex_base 
  : public  et_detail::et_vertex_iterator_mixin<et_detail::element_traits_base<GRID>, 
						has_VertexIterator<grid_types<GRID> >::result >  
{
  typedef GRID                           grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Vertex            element_type;
  typedef typename gt::vertex_handle     handle_type;
  typedef vertex_type_tag                element_type_tag;
  typedef typename gt::dimension_tag     grid_dimension_tag;
  typedef typename grid_dimension_tag::template elem_codim<0>::tag element_codimension_tag;
  typedef element_dim_tag<0>                                       element_dimension_tag;
  enum { dim = element_dimension_tag::dim, codim = element_codimension_tag::dim };

  static unsigned dimension  (element_type const& e) { return dim;}
  static unsigned codimension(element_type const&)   { return codim;}

  typedef typename et_detail::element_traits_base<GRID>::  template 
    hasher_type_base<GRID, element_type, handle_type> hasher_type_elem_base;
  
  static handle_type     handle        (element_type const& e)                     { return e.handle();}
  static element_type    handle2element(grid_type         const& g, handle_type h) { return element_type(g,h); }
  static element_type    handle2element(ref_ptr<grid_type const> g, handle_type h) { return element_type(*g,h); }
};


/*! \brief basic definition to derive from for actual specializations
     of \c element_traits for edge types

  \ingroup elementtraits
*/
template<class GRID>
struct element_traits_edge_base 
  : public et_detail::et_edge_iterator_mixin<et_detail::element_traits_base<GRID>, 
					     has_EdgeIterator<grid_types<GRID> >::result >  {
  typedef GRID                           grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Edge              element_type;
  typedef typename gt::edge_handle       handle_type;
  typedef edge_type_tag                  element_type_tag;
  typedef typename gt::dimension_tag     grid_dimension_tag;
  typedef element_codim_tag<(int)(grid_dimension_tag::dim)-1> 
                                         element_codimension_tag;
  typedef element_dim_tag<1>             element_dimension_tag;
  enum { dim = element_dimension_tag::dim, codim = element_codimension_tag::dim };
  static unsigned dimension  (element_type const& e) { return dim;}
  static unsigned codimension(element_type const&)   { return codim;}

  typedef typename et_detail::element_traits_base<GRID>::template 
    hasher_type_base<GRID, element_type, handle_type> hasher_type_elem_base;

  static handle_type     handle        (element_type const& e)                     { return e.handle();}
  static element_type    handle2element(grid_type const& g, handle_type h)         { return element_type(g,h);}
  static element_type    handle2element(ref_ptr<grid_type const> g, handle_type h) { return element_type(*g,h); }
};

/*! \brief basic definition to derive from for actual specializations
     of \c element_traits for face types

  \ingroup elementtraits
*/
template<class GRID>
struct element_traits_face_base 
 : public et_detail::et_face_iterator_mixin<et_detail::element_traits_base<GRID>, 
					    has_FaceIterator<grid_types<GRID> >::result > 
{
  typedef GRID                           grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Face              element_type;
  typedef typename gt::face_handle       handle_type;
  typedef face_type_tag                  element_type_tag;
  typedef typename gt::dimension_tag     grid_dimension_tag;
  typedef element_codim_tag<(int)(grid_dimension_tag::dim)-2> 
                                         element_codimension_tag;
  typedef element_dim_tag<2>             element_dimension_tag;
  enum { dim = element_dimension_tag::dim, codim = element_codimension_tag::dim };

  static unsigned dimension  (element_type const& e) { return dim;}
  static unsigned codimension(element_type const&)   { return codim;}

  typedef typename et_detail::element_traits_base<GRID>::template 
    hasher_type_base<GRID, element_type, handle_type> hasher_type_elem_base;

  static handle_type     handle        (element_type const& e)                     { return e.handle();}
  static element_type    handle2element(grid_type         const& g, handle_type h) { return element_type(g,h); }
  static element_type    handle2element(ref_ptr<grid_type const> g, handle_type h) { return element_type(*g,h); }
};


/*! \brief basic definition to derive from for actual specializations
     of \c element_trait for facet types

  \ingroup elementtraits
*/
template<class GRID>
struct element_traits_facet_base 
  : public et_detail::et_facet_iterator_mixin<et_detail::element_traits_base<GRID>, 
					      has_FacetIterator<grid_types<GRID> >::result > 
{
  typedef GRID                            grid_type;
  typedef grid_types<grid_type>           gt;
  typedef typename gt::Facet              element_type;
  typedef typename gt::facet_handle       handle_type;
  typedef facet_type_tag                  element_type_tag;
  typedef typename gt::dimension_tag      grid_dimension_tag;
  typedef element_dim_tag<(int)(grid_dimension_tag::dim)-1> 
                                          element_dimension_tag;
  typedef element_codim_tag<1>            element_codimension_tag;
  enum { dim = element_dimension_tag::dim, codim = element_codimension_tag::dim };

  static unsigned dimension  (element_type const& e) { return dim;}
  static unsigned codimension(element_type const&)   { return codim;}

  typedef typename et_detail::element_traits_base<GRID>::template  
    hasher_type_base<GRID, element_type, handle_type> hasher_type_elem_base;

  static handle_type     handle        (element_type const& e)                     { return e.handle();}
  static element_type    handle2element(grid_type         const& g, handle_type h) { return element_type(g,h); }
  static element_type    handle2element(ref_ptr<grid_type const> g, handle_type h) { return element_type(*g,h); }
};


/*! \brief basic definition to derive from for actual specializations
     of \c element_traits for cell types

  \ingroup elementtraits
*/
template<class GRID>
struct element_traits_cell_base 
 : public et_detail::et_cell_iterator_mixin<et_detail::element_traits_base<GRID>, 
					    has_CellIterator<grid_types<GRID> >::result > 
{
  typedef GRID                           grid_type;
  typedef grid_types<grid_type>          gt;
  typedef typename gt::Cell              element_type;
  typedef typename gt::cell_handle       handle_type;
  typedef cell_type_tag                  element_type_tag;
  typedef typename gt::dimension_tag     grid_dimension_tag;
  typedef element_dim_tag<grid_dimension_tag::dim> element_dimension_tag;
  typedef element_codim_tag<0>                     element_codimension_tag;
  enum { dim = element_dimension_tag::dim, codim = element_codimension_tag::dim };

  static unsigned dimension  (element_type const& e) { return dim;}
  static unsigned codimension(element_type const&)   { return codim;}


  typedef typename et_detail::element_traits_base<GRID>::template 
    hasher_type_base<GRID, element_type, handle_type> hasher_type_elem_base;

  static handle_type     handle        (element_type const& e)                     { return e.handle();}
  static element_type    handle2element(grid_type         const& g, handle_type h) { return element_type(g,h); }
  static element_type    handle2element(ref_ptr<grid_type const> g, handle_type h) { return element_type(*g,h); }
};

namespace et_detail {

  template<class E, class TAG> struct  et_aux {};
  template<class E> struct et_aux<E, vertex_type_tag> : public element_traits_vertex_base<typename E::grid_type> {};
  template<class E> struct et_aux<E, edge_type_tag>   : public element_traits_edge_base  <typename E::grid_type> {};
  template<class E> struct et_aux<E, face_type_tag>   : public element_traits_face_base  <typename E::grid_type> {};
  template<class E> struct et_aux<E, facet_type_tag>  : public element_traits_facet_base <typename E::grid_type> {};
  template<class E> struct et_aux<E, cell_type_tag>   : public element_traits_cell_base  <typename E::grid_type> {};


} // namespace et_detail

/*! \brief Dispatching elements to basic predefined traits

    \ingroup elementtraits

    \templateparams
     - \c typedef E::grid_type
     - \c typedef E::element_type_tag

     \todo Extend the mechanism to the case where not \c element_type_tag but dim/codim are given
 */
template<class E> struct element_traits_base : public et_detail::et_aux<E, typename E::element_type_tag> {};

} // namespace GrAL 

#endif
