#ifndef GRAL_GB_BASE_INTERNAL_COMPILE_TIME_REFLECTION_H
#define GRAL_GB_BASE_INTERNAL_COMPILE_TIME_REFLECTION_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <boost/mpl/int.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include "Gral/Base/internal/define-grid-entities.h"
#include "Gral/Base/type-tags.h"

namespace GrAL {

  /* compile-time reflection
   */
  namespace grid_types_detail {
  // the following technique for detecting member types is described
  // e.g. in Josuttis & Vandevoorde, C++ Templates, Addison-Wesley 2002

  // on SX, a definition of size2 with dummy[2] is of the same size as size1!
  struct size1 { char dummy;};
  struct size2 { char dummy[8];};

#define DEFINE_TESTFOR(T) \
  template<class X> size1 test_for_##T(typename X:: T *); \
  template<class X> size2 test_for_##T(...);

GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_TESTFOR);
  DEFINE_TESTFOR(category);
  DEFINE_TESTFOR(uses_root_class);
  DEFINE_TESTFOR(grid_type);
  DEFINE_TESTFOR(element_type_tag);
  DEFINE_TESTFOR(test_type);
  DEFINE_TESTFOR(EdgeOnEdgeIterator);
  DEFINE_TESTFOR(FaceOnFaceIterator);
  DEFINE_TESTFOR(FacetOnFacetIterator);

#undef DEFINE_TESTFOR

  template<class X> struct is_undefined 
  { enum { value = boost::is_same<X, tags::undefined>::value }; };
	                          

  } // namespace grid_types_detail 

/*! \brief Compile-time mechanism to detect presence of typedefs in grid_types<> specializations
 
    \ingroup gridtypesreflection

   Analogous classes exist for other element types and iterators, 
   e.g. \c has_edge_handle, \c has_Cell<>, \c has_EdgeOnCellIterator<>, \c has_FacetIterator

 */
template<class X> struct has_Vertex;
// dummy declaration to anchor docu.


#define DEFINE_HAS_MEMBER(T) \
  template<class X> \
  struct has_type_##T { \
    enum { value = (sizeof(grid_types_detail::test_for_##T<X>(0)) == sizeof(grid_types_detail::size1) ? 1 : 0)}; \
  };									\
  template<class X, int HAS_TYPE> \
  struct has_ ## T ## _aux { \
    enum { value  = has_type_##T<X>::value };  \
    enum { result = has_type_##T<X>::value };  \
  }; \
  template<class X> \
  struct has_ ## T ## _aux<X,1> { \
    enum { type_is_undefined = grid_types_detail::is_undefined<typename X::T>::value }; \
    enum { result = has_type_##T<X>::value && ! type_is_undefined };	\
    enum { value  = has_type_##T<X>::value && ! type_is_undefined };	\
  }; \
  template<class X> \
  struct has_##T  { \
    enum { value = has_ ## T ## _aux<X, has_type_##T<X>::value>::value }; \
    enum { result = value }; \
    typedef boost::mpl::int_<value> type; \
  }; 


  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_HAS_MEMBER);

  DEFINE_HAS_MEMBER(category);
  DEFINE_HAS_MEMBER(uses_root_class);
  DEFINE_HAS_MEMBER(grid_type);
  DEFINE_HAS_MEMBER(element_type_tag);
  DEFINE_HAS_MEMBER(test_type);
  DEFINE_HAS_MEMBER(EdgeOnEdgeIterator);
  DEFINE_HAS_MEMBER(FaceOnFaceIterator);
  DEFINE_HAS_MEMBER(FacetOnFacetIterator);

#undef DEFINE_HAS_MEMBER



#define DEFINE_GET_MEMBER(T) \
  template<class GT, class FALLBACK, int HAS = 0> struct get_ ## T ## _aux                 { typedef FALLBACK type; }; \
  template<class GT, class FALLBACK             > struct get_ ## T ## _aux<GT, FALLBACK,1> { typedef typename GT::T type; }; \
    template<class GT, class FALLBACK> struct get_ ## T : \
    public get_ ## T ## _aux<GT,FALLBACK, has_ ## T <GT>::result> {}; 
  
  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_GET_MEMBER);
  DEFINE_GET_MEMBER(grid_type);

#undef  DEFINE_GET_MEMBER



  namespace tags {

    template<class GT, class E> struct get_raw   { typedef undefined type;};
    template<class GT, class E> struct has_raw : boost::mpl::int_<0> {}; 
    
#define DEFINE_GET_RAW(T) template<class GT> struct get_raw<GT,T> { typedef typename GT::T type;};
#define DEFINE_HAS_RAW(T) template<class GT> struct has_raw<GT,T> : has_ ## T <GT> {};

    GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_GET_RAW);
    GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_HAS_RAW);

#undef  DEFINE_GET_RAW
    

  /*! \brief Templated version of has_XXX

      \templateparams
      - \c GT grid_types specialization
      - \c E: one of the dummy types \c tags::Vertex etc.
   */
  template<class GT, class E> struct has : tags::has_raw<GT,E> {};
  /*! \brief Templated version of get_XXX
   */
  template<class GT, class E> struct get { 
    typedef typename boost::mpl::eval_if<typename has<GT,E>::type,
					 tags::get_raw<GT,E>,
					 boost::mpl::identity<tags::undefined> >::type type;
  };

  } // namespace tags


/*! Print out which types are supported by a \c grid_types<> specialization 

    \ingroup gridtypesreflection

    \templateparams
     - \c GT is a specialization of \c grid_types<>
     - OSTREAM supports <tt> operator<<( ::std::string) </tt>, e.g. \c  ::std::ostream
*/
template<class GT, class OSTREAM>
inline void checkgt(OSTREAM& out)
{
  typedef GT gt;

#define PRINT_HAS_MEMBER(T)  out << "has" << ( has_##T<gt>::result ? "   " : " no") << " type " << #T << "\n";

GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(PRINT_HAS_MEMBER);

#undef PRINT_HAS_MEMBER

}

template<class GT, class OSTREAM>
inline void checkgt_terse(OSTREAM& out)
{
  typedef GT gt;

#define PRINT_HAS_MEMBER(T)  if(has_##T<gt>::result)  { out << "has  type " << #T << "\n"; }

GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(PRINT_HAS_MEMBER);

#undef PRINT_HAS_MEMBER

}
   

  template<class GT, class ETAG> struct has_element { enum { result = 0 }; };
  template<class GT> struct has_element<GT, vertex_type_tag> : public has_Vertex<GT> {};
  template<class GT> struct has_element<GT, edge_type_tag  > : public has_Edge  <GT> {};
  template<class GT> struct has_element<GT, face_type_tag  > : public has_Face  <GT> {};
  template<class GT> struct has_element<GT, facet_type_tag > : public has_Facet <GT> {};
  template<class GT> struct has_element<GT, cell_type_tag  > : public has_Cell  <GT> {};


  template<class GT, class ETAG> struct has_sequence_iterator { enum { result = 0 }; };
  template<class GT> struct has_sequence_iterator<GT, vertex_type_tag> : public has_VertexIterator<GT> {};
  template<class GT> struct has_sequence_iterator<GT, edge_type_tag  > : public has_EdgeIterator  <GT> {};
  template<class GT> struct has_sequence_iterator<GT, face_type_tag  > : public has_FaceIterator  <GT> {};
  template<class GT> struct has_sequence_iterator<GT, facet_type_tag > : public has_FacetIterator <GT> {};
  template<class GT> struct has_sequence_iterator<GT, cell_type_tag  > : public has_CellIterator  <GT> {};


  template<class GT, class ETAG, class ATAG> struct has_incidence_iterator { enum { result = 0 }; };
  template<class GT> struct has_incidence_iterator<GT, vertex_type_tag, vertex_type_tag> : public has_VertexOnVertexIterator<GT> {};
  template<class GT> struct has_incidence_iterator<GT, vertex_type_tag, edge_type_tag  > : public has_VertexOnEdgeIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT, vertex_type_tag, face_type_tag  > : public has_VertexOnFaceIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT, vertex_type_tag, facet_type_tag > : public has_VertexOnFacetIterator <GT> {};
  template<class GT> struct has_incidence_iterator<GT, vertex_type_tag, cell_type_tag  > : public has_VertexOnCellIterator  <GT> {};

  template<class GT> struct has_incidence_iterator<GT,   edge_type_tag, vertex_type_tag> : public has_EdgeOnVertexIterator<GT> {};
  //  template<class GT> struct has_incidence_iterator<GT,   edge_type_tag, edge_type_tag  > : public has_EdgeOnEdgeIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   edge_type_tag, face_type_tag  > : public has_EdgeOnFaceIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   edge_type_tag, facet_type_tag > : public has_EdgeOnFacetIterator <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   edge_type_tag, cell_type_tag  > : public has_EdgeOnCellIterator  <GT> {};

  template<class GT> struct has_incidence_iterator<GT,   face_type_tag, vertex_type_tag> : public has_FaceOnVertexIterator<GT> {};
  template<class GT> struct has_incidence_iterator<GT,   face_type_tag, edge_type_tag  > : public has_FaceOnEdgeIterator  <GT> {};
  //  template<class GT> struct has_incidence_iterator<GT,   face_type_tag, face_type_tag  > : public has_FaceOnFaceIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   face_type_tag, facet_type_tag > : public has_FaceOnFacetIterator <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   face_type_tag, cell_type_tag  > : public has_FaceOnCellIterator  <GT> {};

  template<class GT> struct has_incidence_iterator<GT,  facet_type_tag, vertex_type_tag> : public has_FacetOnVertexIterator<GT> {};
  template<class GT> struct has_incidence_iterator<GT,  facet_type_tag, edge_type_tag  > : public has_FacetOnEdgeIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT,  facet_type_tag, face_type_tag  > : public has_FacetOnFaceIterator  <GT> {};
  //  template<class GT> struct has_incidence_iterator<GT,  facet_type_tag, facet_type_tag > : public has_FacetOnFacetIterator <GT> {};
  template<class GT> struct has_incidence_iterator<GT,  facet_type_tag, cell_type_tag  > : public has_FacetOnCellIterator  <GT> {};

  template<class GT> struct has_incidence_iterator<GT,   cell_type_tag, vertex_type_tag> : public has_CellOnVertexIterator<GT> {};
  template<class GT> struct has_incidence_iterator<GT,   cell_type_tag, edge_type_tag  > : public has_CellOnEdgeIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   cell_type_tag, face_type_tag  > : public has_CellOnFaceIterator  <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   cell_type_tag, facet_type_tag > : public has_CellOnFacetIterator <GT> {};
  template<class GT> struct has_incidence_iterator<GT,   cell_type_tag, cell_type_tag  > : public has_CellOnCellIterator  <GT> {};

}

#endif
