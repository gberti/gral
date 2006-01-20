#ifndef GRAL_GB_BASE_INTERNAL_ITERATOR_TYPE_MAPS_H
#define GRAL_GB_BASE_INTERNAL_ITERATOR_TYPE_MAPS_H

// $LICENSE_NEC_2006

#include "Gral/Base/type-tags.h"
#include "Gral/Base/internal/compile-time-reflection.h"
#include "Gral/Base/internal/grid-categories.h"

namespace GrAL {

  template<class G> class grid_types;
  template<class E> class element_traits;

  template<class GT, class ETAG>             struct element;
  template<class GT, int DE>                 struct element_d;
  template<class GT, class ETAG>             struct element_handle;
  template<class GT, int DE>                 struct element_handle_d;
  template<class GT, class ETAG>             struct sequence_iterator;
  template<class GT, int DE>                 struct sequence_iterator_d;
  template<class GT, class ETAG, class ATAG> struct incidence_iterator;
  template<class GT, int DE,     int DA>     struct incidence_iterator_d;

} // namespace GrAL


namespace GrAL { namespace grid_types_detail {


  template<class GT, class ETAG, int HAS>
  struct element_type_aux1 { typedef void type; };

  // specializations 
  template<class GT>
  struct element_type_aux1<GT, vertex_type_tag,1> { typedef typename GT::Vertex type;};
  template<class GT>
  struct element_type_aux1<GT, edge_type_tag,1>   { typedef typename GT::Edge   type;};
  template<class GT>
  struct element_type_aux1<GT, face_type_tag,1>   { typedef typename GT::Face   type;};
  template<class GT>
  struct element_type_aux1<GT, facet_type_tag,1>  { typedef typename GT::Facet  type;};
  template<class GT>
  struct element_type_aux1<GT, cell_type_tag,1>   { typedef typename GT::Cell   type;};

  template<class GT, class ETAG>
  struct element_type_aux  : public element_type_aux1<GT,ETAG, has_element<GT,ETAG>::result> {};



  template<class GT, class ETAG, int HAS>
  struct element_handle_aux1 { typedef void type; };

  // specializations 
  template<class GT>
  struct element_handle_aux1<GT, vertex_type_tag,1> { typedef typename GT::vertex_handle type;};
  template<class GT>
  struct element_handle_aux1<GT, edge_type_tag,1>   { typedef typename GT::edge_handle   type;};
  template<class GT>
  struct element_handle_aux1<GT, face_type_tag,1>   { typedef typename GT::face_handle   type;};
  template<class GT>
  struct element_handle_aux1<GT, facet_type_tag,1>  { typedef typename GT::facet_handle  type;};
  template<class GT>
  struct element_handle_aux1<GT, cell_type_tag,1>   { typedef typename GT::cell_handle   type;};

  template<class GT, class ETAG>
  struct element_handle_aux  : public element_handle_aux1<GT,ETAG, has_element<GT,ETAG>::result> {};



  template<class GT, class ETAG, int HAS>
  struct sequence_iterator_aux1 {};

  // specializations 
  template<class GT>
  struct sequence_iterator_aux1<GT, vertex_type_tag,1> { typedef typename GT::VertexIterator type;};
  template<class GT>
  struct sequence_iterator_aux1<GT, edge_type_tag,1>   { typedef typename GT::EdgeIterator   type;};
  template<class GT>
  struct sequence_iterator_aux1<GT, face_type_tag,1>   { typedef typename GT::FaceIterator   type;};
  template<class GT>
  struct sequence_iterator_aux1<GT, facet_type_tag,1>  { typedef typename GT::FacetIterator  type;};
  template<class GT>
  struct sequence_iterator_aux1<GT, cell_type_tag,1>   { typedef typename GT::CellIterator   type;};


  template<class ELEM, class ANCHOR>
  class dummy_incidence_iterator;

  template<class GT, class ETAG_VALUE, class ETAG_ANCHOR, int HAS>
  struct incidence_iterator_aux1 { typedef void type; };

  // specializations

  // VertexOnXXX
  

  template<class GT> struct incidence_iterator_aux1<GT, vertex_type_tag, vertex_type_tag,1> 
  { typedef typename GT::VertexOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, vertex_type_tag, edge_type_tag,1>
  { typedef typename GT::VertexOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, vertex_type_tag, face_type_tag,1>
  { typedef typename GT::VertexOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, vertex_type_tag, facet_type_tag,1>
  { typedef typename GT::VertexOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, vertex_type_tag, cell_type_tag,1>
  { typedef typename GT::VertexOnCellIterator type;};

  // EdgeOnXXX

  template<class GT> struct incidence_iterator_aux1<GT, edge_type_tag, vertex_type_tag,1>
  { typedef typename GT::EdgeOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, edge_type_tag, edge_type_tag,1>
  // { typedef typename GT::EdgeOnEdgeIterator type;};
  { typedef dummy_incidence_iterator<typename GT::Edge, typename GT::Edge> type;};
  template<class GT> struct incidence_iterator_aux1<GT, edge_type_tag, face_type_tag,1>
  { typedef typename GT::EdgeOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, edge_type_tag, facet_type_tag,1>
  { typedef typename GT::EdgeOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, edge_type_tag, cell_type_tag,1>
  { typedef typename GT::EdgeOnCellIterator type;};

  // FaceOnXXX

  template<class GT> struct incidence_iterator_aux1<GT, face_type_tag, vertex_type_tag,1>
  { typedef typename GT::FaceOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, face_type_tag, edge_type_tag,1>
  { typedef typename GT::FaceOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, face_type_tag, face_type_tag,1>
  { typedef typename GT::FaceOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, face_type_tag, facet_type_tag,1>
  { typedef typename GT::FaceOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, face_type_tag, cell_type_tag,1>
  { typedef typename GT::FaceOnCellIterator type;};

  // FacetOnXXX

  template<class GT> struct incidence_iterator_aux1<GT, facet_type_tag, vertex_type_tag,1>
  { typedef typename GT::FacetOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, facet_type_tag, edge_type_tag,1>
  { typedef typename GT::FacetOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, facet_type_tag, face_type_tag,1>
  { typedef typename GT::FacetOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, facet_type_tag, facet_type_tag,1>
  { typedef typename GT::FacetOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, facet_type_tag, cell_type_tag,1>
  { typedef typename GT::FacetOnCellIterator type;};

  // CellOnXXX

  template<class GT> struct incidence_iterator_aux1<GT, cell_type_tag, vertex_type_tag,1>
  { typedef typename GT::CellOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, cell_type_tag, edge_type_tag,1>
  { typedef typename GT::CellOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, cell_type_tag, face_type_tag,1>
  { typedef typename GT::CellOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, cell_type_tag, facet_type_tag,1>
  { typedef typename GT::CellOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux1<GT, cell_type_tag, cell_type_tag,1>
  { typedef typename GT::CellOnCellIterator type;};



 /*
     If A is range
         (Result must be sequence iterator)
         If E is a sequence iterator 
            return E
         Else (E must be an element type)
            return computed seq. iter for E
     Else (A must be an element type)
         (Result must be an incidence iterator)
         If E is an incidence iterator
            return E
         Else 
            return computed inc. iter for E
   */


  template<class GT, class E, int HAS_ELEM_TAG> struct element_tag_of_aux {};
  template<class GT, class E> struct element_tag_of_aux<GT,E,1> { typedef typename E::element_type_tag type; };
  template<class GT, class E> struct element_tag_of_aux<GT,E,0> 
    : public category2element_tag<GT, typename category_of<E>::type> {};

  

  template<class GT, class E>
  struct element_tag_of : public element_tag_of_aux<GT, E, has_element_type_tag<E>::result> {};


  template<class GT, class E>
  struct element_dim_of {  enum { value = element_traits<E>::dim }; };



  template<class GT, class E, class A, int IS_INC_ITERATOR> struct incidence_iterator_aux2 {};
  // could check that anchor type of E corresponds to A
  template<class GT, class E, class A> struct incidence_iterator_aux2<GT,E,A,1> { typedef E type;  };



  template<class GT, class E, class A> struct incidence_iterator_aux2<GT,E,A,0> 
    : public incidence_iterator_d<GT, 
				  element_dim_of<GT,E>::value,
				  element_dim_of<GT,A>::value>
  {};




  template <class GT, class E, class A> struct incidence_iterator_aux
    : public incidence_iterator_aux2<GT,E,A,
				     is_base_or_same<grid_incidence_iterator_category,
						     typename category_of<E>::type >
						     ::value>
						     
  {  };
   

  template<class GT, class E, int IS_SEQ_ITERATOR> struct sequence_iterator_aux2 {};
  template<class GT, class E> struct sequence_iterator_aux2<GT,E,1> { typedef E type; };



  template<class GT, class E> struct sequence_iterator_aux2<GT,E,0>
    : public sequence_iterator_d<GT,
				 element_dim_of<GT,E>::value>
  {};



  template<class GT, class E> struct sequence_iterator_aux 
    : public  sequence_iterator_aux2<GT,E, 
				     is_base_or_same<grid_sequence_iterator_category,
						     typename category_of<E>::type>
						     ::value>
						     {};

  template<class GT, class E, class A, int IS_GRID_RANGE> 
  struct iterator_aux {};

  // A is a grid range, return sequence iterator corresponding to E
  template<class GT, class E, class A>
  struct iterator_aux<GT,E,A,1>  : public sequence_iterator_aux<GT, E> {};

  // A is not a grid range, return incidence iterator corresponding to A and E
  template<class GT, class E, class A>
  struct iterator_aux<GT,E,A,0>  : public incidence_iterator_aux<GT, E, A>  {};



  template<class GT, class E, class A>
  struct iterator 
    : public iterator_aux<GT, E, A, 
			  is_base_or_same<grid_range_category, 
					  typename category_of<A>::type>::value > {};



} // namespace grid_types_detail 


  template<class T> struct grid_types_of;


  /*! \brief Meta-function returning iterator with anchor \c A and value \c E

      \ingroup gridtypes
  */
  template<class E, class A, class GT = typename grid_types_of<A>::type> 
  struct iterator : public grid_types_detail::iterator<GT, E,A> {};

  /*! \brief Type map of iterator with anchor \c A and value of dimension \c D

  */
  template<int D, class A, class GT = typename grid_types_of<A>::type> 
  struct iterator_d 
  {
    typedef typename element_d<GT,D> ::type  E;
    typedef typename iterator<E,A,GT>::type  type;
  };




  /*! \brief Meta-function return element corresponding to tag
    
     \ingroup gridtypes
  */
  template<class GT, class ETAG>  struct element   
    : public grid_types_detail::element_type_aux<GT,ETAG> {};
  /*! \brief Meta-function return element corresponding to dim
    
     \ingroup gridtypes
  */
  template<class GT, int   DE  >  struct element_d 
    : public grid_types_detail::element_type_aux<GT,typename int2etag<GT,DE>::tag> {};

  /*! \brief Meta-function return element handle corresponding to tag
    
     \ingroup gridtypes
  */
  template<class GT, class ETAG>  struct element_handle   
    : public grid_types_detail::element_handle_aux<GT,ETAG> {};

  /*! \brief Meta-function return element handle corresponding to dim
    
     \ingroup gridtypes
  */
  template<class GT, int   DE  >  struct element_handle_d 
    : public grid_types_detail::element_handle_aux<GT,typename int2etag<GT,DE>::tag> {};

 
  /*! \brief Meta-function return sequence iterator corresponding to tag
    
     \ingroup gridtypes
  */
  template<class GT, class ETAG>  struct sequence_iterator 
    : public grid_types_detail::sequence_iterator_aux1<GT,ETAG,1> {};

  /*! \brief Meta-function return sequence iterator corresponding to dim \c DE
    
     \ingroup gridtypes
  */
   template<class GT, int   DE>    struct sequence_iterator_d
    : public grid_types_detail::sequence_iterator_aux1<GT,typename int2etag<GT,DE>::tag,1> {};

  /*! \brief Meta-function return sequence iterator corresponding to tags

     \c ETAG is the tag of the \c value_type and \c ATAG of the \c anchor_type of the iterator.    

     \ingroup gridtypes
  */

  template<class GT, class ETAG, class ATAG> struct incidence_iterator
    :  public grid_types_detail::incidence_iterator_aux1<GT,
							 ETAG,
							 ATAG,
							 1> 
  {};

  /*! \brief Meta-function return sequence iterator corresponding to dims \c DE and \c DA
    
     \c DE is the dimension of the \c value_type and \c DA of the \c anchor_type of the iterator.   

     \ingroup gridtypes
  */
  template<class GT, int DE,     int DA>     struct incidence_iterator_d
    : public grid_types_detail::incidence_iterator_aux1<GT,
							typename int2etag<GT, DE>::tag,
							typename int2etag<GT, DA>::tag,
							1> 
  {};

  
} // namespace GrAL 

#endif
