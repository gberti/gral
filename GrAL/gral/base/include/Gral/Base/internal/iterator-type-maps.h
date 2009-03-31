#ifndef GRAL_GB_BASE_INTERNAL_ITERATOR_TYPE_MAPS_H
#define GRAL_GB_BASE_INTERNAL_ITERATOR_TYPE_MAPS_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/type-tags.h"
#include "Gral/Base/internal/compile-time-reflection.h"
#include "Gral/Base/internal/grid-categories.h"
#include "Gral/Iterators/dummy-iterator.h"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/placeholders.hpp>

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


namespace GrAL { 

  namespace tags {
    namespace mpl = boost::mpl;

    // helper class, to work around bug in gcc 4.0.x
    template<class GT> struct get_entity {
      template<class E> struct map   { typedef typename get<GT,E>::type type;};
      template<class E> struct apply { typedef typename get<GT,E>::type type;};
    };

    /*! \brief Helper algorithm to select any defined type of \c Seq
   */
  template<class GT, class Seq> struct any_of {
    struct mapper {
      template<class E> struct apply {
	typedef typename get<GT,E>::type type;
      };
    };
   
    typedef undefined forbidden;
    typedef undefined fallback;
    typedef mpl::placeholders::_1 _1;

    // mpl::protect is necessary for gcc4, because else it decends into lambda
    // for template args of mapper (and gets screwed up sometimes),
    // although mapper is a perfect metafunction class already.
    typedef mpl::transform_view<Seq, mpl::protect<mapper> > types;
    typedef typename mpl::end <types>::type it_end;
    typedef typename mpl::find_if<types, mpl::not_<boost::is_same<forbidden,_1> > >::type first_def;
    typedef typename mpl::eval_if<typename boost::is_same<first_def,it_end>::type,
				  mpl::identity<fallback>,
				  mpl::deref<first_def>
				  >::type   type;
   };


//-------------------- metafunctions for grid element and iterators -------------------------

//----- elements -------

// DE / CE - dimension / codimension of element, D - dimension of grid

template<class GT, int DE, int CE, int D> struct element_d_dim { typedef undefined type; };

template<class GT, int CE, int D> struct element_d_dim<GT, 0, CE, D> : get<GT,Vertex> {};
template<class GT, int CE, int D> struct element_d_dim<GT, 1, CE, D> : get<GT,Edge>   {};
template<class GT, int CE, int D> struct element_d_dim<GT, 2, CE, D> : get<GT,Face>   {};

template<class GT, int DE, int D> struct element_d_dim<GT, DE, 1, D> : get<GT,Facet>  {};
template<class GT, int DE, int D> struct element_d_dim<GT, DE, 0, D> : get<GT,Cell>   {};

// 0D
template<class GT> struct element_d_dim<GT,0,0, 0> : any_of<GT, mpl::vector2<Vertex, Cell> > {};
// 1D
template<class GT> struct element_d_dim<GT,0,1, 1> : any_of<GT, mpl::vector2<Vertex, Facet> > {};
template<class GT> struct element_d_dim<GT,1,0, 1> : any_of<GT, mpl::vector2<Edge, Cell> > {};
// 2D
template<class GT> struct element_d_dim<GT,1,1, 2> : any_of<GT, mpl::vector2<Edge, Facet> > {};
template<class GT> struct element_d_dim<GT,2,0, 2> : any_of<GT, mpl::vector2<Face, Cell> > {};
// 3D
template<class GT> struct element_d_dim<GT,2,1, 3> : any_of<GT, mpl::vector2<Face, Facet> > {};


//------ element handles -----

template<class GT, int DE, int CE, int D> struct element_handle_d_dim { typedef undefined type; };

// general case
template<class GT, int CE, int D> struct element_handle_d_dim<GT, 0, CE, D> : get<GT,vertex_handle> {};
template<class GT, int CE, int D> struct element_handle_d_dim<GT, 1, CE, D> : get<GT,edge_handle>   {};
template<class GT, int CE, int D> struct element_handle_d_dim<GT, 2, CE, D> : get<GT,face_handle>   {};

template<class GT, int DE, int D> struct element_handle_d_dim<GT, DE, 1, D> : get<GT,facet_handle>  {};
template<class GT, int DE, int D> struct element_handle_d_dim<GT, DE, 0, D> : get<GT,cell_handle>   {};

// 0D
template<class GT> struct element_handle_d_dim<GT,0,0, 0> : any_of<GT, mpl::vector2<vertex_handle, cell_handle> > {};
// 1D
template<class GT> struct element_handle_d_dim<GT,0,1, 1> : any_of<GT, mpl::vector2<vertex_handle, facet_handle> > {};
template<class GT> struct element_handle_d_dim<GT,1,0, 1> : any_of<GT, mpl::vector2<edge_handle,   cell_handle> > {};
// 2D
template<class GT> struct element_handle_d_dim<GT,1,1, 2> : any_of<GT, mpl::vector2<edge_handle, facet_handle> > {};
template<class GT> struct element_handle_d_dim<GT,2,0, 2> : any_of<GT, mpl::vector2<face_handle, cell_handle> > {};
// 3D
template<class GT> struct element_handle_d_dim<GT,2,1, 3> : any_of<GT, mpl::vector2<face_handle, facet_handle> > {};



//----- Sequence iterators ------


template<class GT, int DE, int CE, int D> struct sequence_iterator_d_dim { typedef undefined type; };

// general case
template<class GT, int CE, int D> struct sequence_iterator_d_dim<GT, 0, CE, D> : get<GT,VertexIterator> {};
template<class GT, int CE, int D> struct sequence_iterator_d_dim<GT, 1, CE, D> : get<GT,EdgeIterator>   {};
template<class GT, int CE, int D> struct sequence_iterator_d_dim<GT, 2, CE, D> : get<GT,FaceIterator>   {};

template<class GT, int DE, int D> struct sequence_iterator_d_dim<GT, DE, 1, D> : get<GT,FacetIterator>  {};
template<class GT, int DE, int D> struct sequence_iterator_d_dim<GT, DE, 0, D> : get<GT,CellIterator>   {};

// 0D
template<class GT> struct sequence_iterator_d_dim<GT,0,0, 0> : any_of<GT, mpl::vector2<VertexIterator, CellIterator> > {};
// 1D
template<class GT> struct sequence_iterator_d_dim<GT,0,1, 1> : any_of<GT, mpl::vector2<VertexIterator, FacetIterator> > {};
template<class GT> struct sequence_iterator_d_dim<GT,1,0, 1> : any_of<GT, mpl::vector2<EdgeIterator,   CellIterator> > {};
// 2D
template<class GT> struct sequence_iterator_d_dim<GT,1,1, 2> : any_of<GT, mpl::vector2<EdgeIterator, FacetIterator> > {};
template<class GT> struct sequence_iterator_d_dim<GT,2,0, 2> : any_of<GT, mpl::vector2<FaceIterator, CellIterator> > {};
// 3D
template<class GT> struct sequence_iterator_d_dim<GT,2,1, 3> : any_of<GT, mpl::vector2<FaceIterator, FacetIterator> > {};



//----- Incidence iterators -----

// DE/CE - dim/codim of element, DA/CA - dim/codim of anchor, D - dim of grid

template<class GT, int DE, int CE, int DA, int CA, int D> struct incidence_iterator_d_dim { typedef undefined type; };

// general case
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 0, CE, 0, CA, D> : get<GT,VertexOnVertexIterator> {};
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 0, CE, 1, CA, D> : get<GT,VertexOnEdgeIterator>   {};
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 0, CE, 2, CA, D> : get<GT,VertexOnFaceIterator>   {};
template<class GT, int CE, int DA, int D> struct incidence_iterator_d_dim<GT, 0, CE, DA, 1, D> : get<GT,VertexOnFacetIterator>  {};
template<class GT, int CE, int DA, int D> struct incidence_iterator_d_dim<GT, 0, CE, DA, 0, D> : get<GT,VertexOnCellIterator>   {};

template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 1, CE, 0, CA, D> : get<GT,EdgeOnVertexIterator>   {};
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 1, CE, 1, CA, D> : get<GT,EdgeOnEdgeIterator>     {};
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 1, CE, 2, CA, D> : get<GT,EdgeOnFaceIterator>     {};
template<class GT, int CE, int DA, int D> struct incidence_iterator_d_dim<GT, 1, CE, DA, 1, D> : get<GT,EdgeOnFacetIterator>    {};
template<class GT, int CE, int DA, int D> struct incidence_iterator_d_dim<GT, 1, CE, DA, 0, D> : get<GT,EdgeOnCellIterator>     {};

template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 2, CE, 0, CA, D> : get<GT,FaceOnVertexIterator>   {};
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 2, CE, 1, CA, D> : get<GT,FaceOnEdgeIterator>     {};
template<class GT, int CE, int CA, int D> struct incidence_iterator_d_dim<GT, 2, CE, 2, CA, D> : get<GT,FaceOnFaceIterator>     {};
template<class GT, int CE, int DA, int D> struct incidence_iterator_d_dim<GT, 2, CE, DA, 1, D> : get<GT,FaceOnFacetIterator>    {};
template<class GT, int CE, int DA, int D> struct incidence_iterator_d_dim<GT, 2, CE, DA, 0, D> : get<GT,FaceOnCellIterator>     {};

template<class GT, int DE, int CA, int D> struct incidence_iterator_d_dim<GT, DE, 1, 0, CA, D> : get<GT,FacetOnVertexIterator>  {};
template<class GT, int DE, int CA, int D> struct incidence_iterator_d_dim<GT, DE, 1, 1, CA, D> : get<GT,FacetOnEdgeIterator>    {};
template<class GT, int DE, int CA, int D> struct incidence_iterator_d_dim<GT, DE, 1, 2, CA, D> : get<GT,FacetOnFaceIterator>    {};
template<class GT, int DE, int DA, int D> struct incidence_iterator_d_dim<GT, DE, 1, DA, 1, D> : get<GT,FacetOnFacetIterator>   {};
template<class GT, int DE, int DA, int D> struct incidence_iterator_d_dim<GT, DE, 1, DA, 0, D> : get<GT,FacetOnCellIterator>    {};

template<class GT, int DE, int CA, int D> struct incidence_iterator_d_dim<GT, DE, 0, 0, CA, D> : get<GT,CellOnVertexIterator>   {};
template<class GT, int DE, int CA, int D> struct incidence_iterator_d_dim<GT, DE, 0, 1, CA, D> : get<GT,CellOnEdgeIterator>     {};
template<class GT, int DE, int CA, int D> struct incidence_iterator_d_dim<GT, DE, 0, 2, CA, D> : get<GT,CellOnFaceIterator>     {};
template<class GT, int DE, int DA, int D> struct incidence_iterator_d_dim<GT, DE, 0, DA, 1, D> : get<GT,CellOnFacetIterator>    {};
template<class GT, int DE, int DA, int D> struct incidence_iterator_d_dim<GT, DE, 0, DA, 0, D> : get<GT,CellOnCellIterator>     {};




// 0D
template<class GT> struct incidence_iterator_d_dim<GT,0,0, 0,0, 0> : any_of<GT, mpl::vector<VertexOnVertexIterator, VertexOnCellIterator,
											    CellOnVertexIterator,   CellOnCellIterator> > {};
// 1D
template<class GT> struct incidence_iterator_d_dim<GT,0,1, 0,1, 1> : any_of<GT, mpl::vector4<VertexOnVertexIterator, VertexOnFacetIterator, 
											    FacetOnVertexIterator, FacetOnFacetIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,0,1, 1,0, 1> : any_of<GT, mpl::vector4<VertexOnEdgeIterator,  VertexOnCellIterator,
											    FacetOnEdgeIterator,   FacetOnCellIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,1,0, 0,1, 1> : any_of<GT, mpl::vector4<EdgeOnVertexIterator,  EdgeOnFacetIterator,
											    CellOnVertexIterator,  CellOnFacetIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,1,0, 1,0, 1> : any_of<GT, mpl::vector4<EdgeOnEdgeIterator,    EdgeOnCellIterator,
											    CellOnEdgeIterator,    CellOnCellIterator> > {};

// 2D
template<class GT> struct incidence_iterator_d_dim<GT,0,2, 1,1, 2> : any_of<GT, mpl::vector2<VertexOnEdgeIterator, VertexOnFacetIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,0,2, 2,0, 2> : any_of<GT, mpl::vector2<VertexOnFaceIterator, VertexOnCellIterator> > {};

template<class GT> struct incidence_iterator_d_dim<GT,1,1, 0,2, 2> : any_of<GT, mpl::vector2<EdgeOnVertexIterator, FacetOnVertexIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,2,0, 0,2, 2> : any_of<GT, mpl::vector2<FaceOnVertexIterator, CellOnVertexIterator > > {};


template<class GT> struct incidence_iterator_d_dim<GT,1,1, 2,0, 2> : any_of<GT, mpl::vector4<EdgeOnFaceIterator,  EdgeOnCellIterator,
											    FacetOnFaceIterator, FacetOnCellIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,2,0, 1,1, 2> : any_of<GT, mpl::vector4<FaceOnEdgeIterator,  FaceOnFacetIterator,
											    CellOnEdgeIterator,  CellOnFacetIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,2,0, 2,0, 2> : any_of<GT, mpl::vector4<FaceOnFaceIterator,  FaceOnCellIterator,
											    CellOnFaceIterator,  CellOnCellIterator> > {};

// 3D
template<class GT> struct incidence_iterator_d_dim<GT,0,3, 2,1, 3> : any_of<GT, mpl::vector2<VertexOnFaceIterator, VertexOnFacetIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,1,2, 2,1, 3> : any_of<GT, mpl::vector2<EdgeOnFaceIterator,   EdgeOnFacetIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,3,0, 2,1, 3> : any_of<GT, mpl::vector2<CellOnFaceIterator,   CellOnFacetIterator> > {};

template<class GT> struct incidence_iterator_d_dim<GT,2,1, 0,3, 3> : any_of<GT, mpl::vector2<FaceOnVertexIterator, FacetOnVertexIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,2,1, 1,2, 3> : any_of<GT, mpl::vector2<FaceOnEdgeIterator,   FacetOnEdgeIterator> > {};
template<class GT> struct incidence_iterator_d_dim<GT,2,1, 3,0, 3> : any_of<GT, mpl::vector2<FaceOnCellIterator, FacetOnCellIterator> > {};

  } // namespace tags



  template<class GT, int DE> struct element_d           : tags::element_d_dim          <GT,DE, dim<GT>::value-DE, dim<GT>::value> {};
  template<class GT, int DE> struct element_handle_d    : tags::element_handle_d_dim   <GT,DE, dim<GT>::value-DE, dim<GT>::value> {};
  template<class GT, int DE> struct sequence_iterator_d : tags::sequence_iterator_d_dim<GT,DE, dim<GT>::value-DE, dim<GT>::value> {};
  template<class GT, int DE, int DA> struct incidence_iterator_d 
  : tags::incidence_iterator_d_dim<GT,
				  DE,dim<GT>::value-DE,
				  DA,dim<GT>::value-DA,
				  dim<GT>::value> {};

  template<class GT, class TAG> struct element           : element_d          <GT, etag2dim<GT,TAG>::value > {};
  template<class GT, class TAG> struct element_handle    : element_handle_d   <GT, etag2dim<GT,TAG>::value > {};
  template<class GT, class TAG> struct sequence_iterator : sequence_iterator_d<GT, etag2dim<GT,TAG>::value > {};
  template<class GT, class ETAG, class ATAG> struct incidence_iterator 
  : incidence_iterator_d<GT, etag2dim<GT,ETAG>::value, etag2dim<GT,ATAG>::value> {};


  template<class GT, int CE> struct element_cd           : tags::element_d_dim          <GT, dim<GT>::value-CE, CE, dim<GT>::value> {};
  template<class GT, int CE> struct element_handle_cd    : tags::element_handle_d_dim   <GT, dim<GT>::value-CE, CE, dim<GT>::value> {};
  template<class GT, int CE> struct sequence_iterator_cd : tags::sequence_iterator_d_dim<GT, dim<GT>::value-CE, CE, dim<GT>::value> {};
  template<class GT, int CE, int CA> struct incidence_iterator_cd 
  : tags::incidence_iterator_d_dim<GT, dim<GT>::value-CE, CE, dim<GT>::value-CA, CA, dim<GT>::value> {};
  template<class GT, int DE, int CA> struct incidence_iterator_d_cd 
  : tags::incidence_iterator_d_dim<GT, DE, dim<GT>::value-DE, dim<GT>::value-CA, CA, dim<GT>::value> {};
  template<class GT, int CE, int DA> struct incidence_iterator_cd_d
  : tags::incidence_iterator_d_dim<GT, dim<GT>::value-CE, CE, DA, dim<GT>::value-DA, dim<GT>::value> {};





  namespace grid_types_detail { namespace iterator_map {

    // E is a sequence iterator: return E
    template<class GT, class E, int IS_SEQ> struct sequence_iterator_aux2 { typedef E type; };
    // E is no sequence iterator: compute result
    template<class GT, class E> struct sequence_iterator_aux2<GT,E,0>
      : public tags::sequence_iterator_d_dim<GT,
					     dim  <GT,E>::value,
					     codim<GT,E>::value,
					     dim  <GT>  ::value>
    {};

    template<class GT, class E> struct sequence_iterator_aux 
      : public  sequence_iterator_aux2<GT,E, is_grid_sequence_iterator<E>::value> {};
    
    // E is an incidence iterator: return E
    template<class GT, class E, class A, int IS_INC> struct incidence_iterator_aux2 { typedef E type; };
    // E is no incidence iterator: compute result
    template<class GT, class E, class A> struct incidence_iterator_aux2<GT,E,A,0> 
      : public tags::incidence_iterator_d_dim<GT, 
					      dim  <GT,E>::value,
					      codim<GT,E>::value,
					      dim  <GT,A>::value,
					      codim<GT,A>::value,
					      dim  <GT>  ::value>
    {};


    template <class GT, class E, class A> struct incidence_iterator_aux
      : public incidence_iterator_aux2<GT,E,A, is_grid_incidence_iterator<E>::value> {};


    template<class GT, class E, class A, int IS_GRID_RANGE> 
    struct iterator_aux {};

    // A is a grid range, return sequence iterator corresponding to E
    template<class GT, class E, class A>
    struct iterator_aux<GT,E,A,1>  : sequence_iterator_aux<GT, E> {};
    
    // A is not a grid range, return incidence iterator corresponding to A and E
    template<class GT, class E, class A>
    struct iterator_aux<GT,E,A,0>  : incidence_iterator_aux<GT, E, A>  {};
    
    template<class GT, class E, class A>
    struct iterator : iterator_aux<GT, E, A, is_grid_range<A>::value> {};

  }} // namespace grid_types_detail { namespace iterator_map {



  /*! \brief Meta-function returning iterator with anchor \c A and value \c E

      \ingroup gridtypes
  */
  template<class E, class A, class GT = typename grid_types_of<A>::type> 
  struct iterator : public grid_types_detail::iterator_map::iterator<GT, E,A> {};

  /*! \brief Type map of iterator with anchor \c A and value of dimension \c D

  */
  template<int D, class A, class GT = typename grid_types_of<A>::type> 
  struct iterator_d 
  {
    typedef typename element_d<GT,D> ::type  E;
    typedef typename iterator<E,A,GT>::type  type;
  };


  /*! \brief Type map of iterator with anchor \c A and value of codimension \c CD

  */
  template<int CD, class A, class GT = typename grid_types_of<A>::type> 
  struct iterator_cd 
  {
    typedef typename element_cd<GT,CD>::type  E;
    typedef typename iterator<E,A,GT> ::type  type;
  };


} // namespace GrAL 

#endif
