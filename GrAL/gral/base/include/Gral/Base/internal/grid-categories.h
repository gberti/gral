#ifndef GRAL_GB_BASE_INTERNAL_GRID_CATEGORIES_H
#define GRAL_GB_BASE_INTERNAL_GRID_CATEGORIES_H


// $LICENSE_NEC_2006

#include "Gral/Base/type-tags.h"


#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/pair.hpp>

namespace GrAL { namespace grid_types_detail {


  /* get_entity_category<GT, E>
     if(has_E<GT>)
       if(has_category(E))
          E::category category(E)
       else
          default_category_for_E category(E)
  */
  
#define DEFINE_GET_ENTITY_CATEGORY(E, CAT)	\
  template<class GT, int HAS_E_CAT> \
  struct get_entity_category_aux2_##E : public GT					\
  { \
    typedef typename boost::mpl::insert<typename GT::category_list, \
					boost::mpl::pair<typename GT::E, typename GT::E::category> >::type category_list; \
    using GT::category; \
    static typename GT:: E ::category category( typename GT:: E const&) { return typename GT :: E ::category();} \
  }; \
  template<class GT> \
  struct get_entity_category_aux2_##E<GT,0> : public GT			       \
  { \
    typedef typename boost::mpl::insert<typename GT::category_list, \
					boost::mpl::pair<typename GT::E, CAT> >::type category_list; \
    using GT::category; \
    static CAT category( typename GT :: E const&) { return CAT(); } \
  }; \
  \
  template<class GT, int HAS_E> \
  struct get_entity_category_aux_##E : public get_entity_category_aux2_##E<GT, has_category< typename GT::E >::result > {}; \
  template<class GT> \
  struct get_entity_category_aux_##E<GT,0> : public GT {};	\
  \
  template<class GT> \
  struct get_entity_category_##E : public get_entity_category_aux_##E<GT, has_##E<GT>::result> {};


 
#define  GRAL_GRID_TYPES_USING_DECL(GT)   \
  using GT::category; \
  using GT::begin_aux; \
  using GT::end_aux; \
  using GT::size_aux; \
  using GT::NumOf; \
  using GT::FirstElement; \
  using GT::EndElement; 
  


  /* define a root class for the get_category hierarchy.
     the 2 purposes of this class are:
     - initialize type-to-category mapping in category_list
     - make sure predefined begin_aux/end_aux/category functions
     are not hidden
     Unfortunately, we cannot simply say <tt> using base::foo; </tt>
     if \c foo may not be defined in \c base. 
  */

  template<class GT, int GT_USES_ROOT_CLASS> 
  struct get_entity_category_root_aux : public GT 
  {
    GRAL_GRID_TYPES_USING_DECL(GT);
  };
  
  template<class GT>
  struct get_entity_category_root_aux<GT,0> : public GT 
  {
    // dummy definitions to permit using base::xxx in derived classes
    void begin_aux() {}
    void end_aux  () {}
    void size_aux  () {}
    void category () {}
    void NumOf    () {}
    void FirstElement() {}
    void EndElement() {}
  };
  
  template<class GT>
  struct get_entity_category_root 
    : public get_entity_category_root_aux<GT, has_uses_root_class<GT>::result >
  { 
    typedef boost::mpl::map0<> category_list;
  };
  
  struct grid_test_category {};
  DEFINE_GET_ENTITY_CATEGORY(test_type, grid_test_category);
DEFINE_GET_ENTITY_CATEGORY(grid_type, grid_range_category);

DEFINE_GET_ENTITY_CATEGORY(Vertex, grid_vertex_category);
DEFINE_GET_ENTITY_CATEGORY(Edge,   grid_edge_category);
DEFINE_GET_ENTITY_CATEGORY(Face,   grid_face_category);
DEFINE_GET_ENTITY_CATEGORY(Facet,  grid_facet_category);
DEFINE_GET_ENTITY_CATEGORY(Cell,   grid_cell_category);

DEFINE_GET_ENTITY_CATEGORY(VertexIterator, grid_sequence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(EdgeIterator,   grid_sequence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FaceIterator,   grid_sequence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FacetIterator,  grid_sequence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(CellIterator,   grid_sequence_iterator_category);

DEFINE_GET_ENTITY_CATEGORY(VertexOnVertexIterator, grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(EdgeOnVertexIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FaceOnVertexIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FacetOnVertexIterator,  grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(CellOnVertexIterator,   grid_incidence_iterator_category);


DEFINE_GET_ENTITY_CATEGORY(VertexOnEdgeIterator, grid_incidence_iterator_category);
//DEFINE_GET_ENTITY_CATEGORY(EdgeOnEdgeIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FaceOnEdgeIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FacetOnEdgeIterator,  grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(CellOnEdgeIterator,   grid_incidence_iterator_category);


DEFINE_GET_ENTITY_CATEGORY(VertexOnFaceIterator, grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(EdgeOnFaceIterator,   grid_incidence_iterator_category);
//DEFINE_GET_ENTITY_CATEGORY(FaceOnFaceIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FacetOnFaceIterator,  grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(CellOnFaceIterator,   grid_incidence_iterator_category);


DEFINE_GET_ENTITY_CATEGORY(VertexOnFacetIterator, grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(EdgeOnFacetIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FaceOnFacetIterator,   grid_incidence_iterator_category);
//DEFINE_GET_ENTITY_CATEGORY(FacetOnFacetIterator,  grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(CellOnFacetIterator,   grid_incidence_iterator_category);


DEFINE_GET_ENTITY_CATEGORY(VertexOnCellIterator, grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(EdgeOnCellIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FaceOnCellIterator,   grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(FacetOnCellIterator,  grid_incidence_iterator_category);
DEFINE_GET_ENTITY_CATEGORY(CellOnCellIterator,   grid_incidence_iterator_category);



#undef DEFINE_GET_ENTITY_CATEGORY


template<class GT>
struct get_grid_element_categories_0d 
  : public 
  get_entity_category_Vertex
  <get_entity_category_VertexIterator
   <get_entity_category_VertexOnVertexIterator
    <GT>
   >
  > 
{};

template<class GT>
struct get_grid_element_categories_1d 
  : public 
  get_entity_category_Edge
  <get_entity_category_EdgeIterator
   <get_entity_category_VertexOnEdgeIterator
    <get_entity_category_EdgeOnVertexIterator
     <get_entity_category_CellOnCellIterator
      <get_grid_element_categories_0d
       <GT> 
      >
     >
    >
   > 
  >
{};

template<class GT>
struct get_grid_element_categories_2d
  : public 
  get_entity_category_Face
  <get_entity_category_FaceIterator
   <get_entity_category_VertexOnFaceIterator
    <get_entity_category_EdgeOnFaceIterator
     <get_entity_category_FaceOnVertexIterator
      <get_entity_category_EdgeOnFaceIterator
       <get_grid_element_categories_1d
	<GT> 
       >
      >
     >
    >
   >  
  >
{};

template<class GT>
struct get_grid_element_categories_3d
  : public 
  get_entity_category_Cell
  <get_entity_category_CellIterator
   <get_entity_category_VertexOnCellIterator
    <get_entity_category_EdgeOnCellIterator
     <get_entity_category_FaceOnCellIterator
      <get_entity_category_CellOnVertexIterator
       <get_entity_category_CellOnEdgeIterator
	<get_entity_category_CellOnFaceIterator
	 <get_grid_element_categories_2d
	  <GT> 
         >
        >
       >
      >
     >
    >
   >
  > 
{};


template<class GT, int D>
struct get_grid_element_categories_aux 
  : public get_grid_element_categories_3d<GT> {};

template<class GT>
struct get_grid_element_categories_aux<GT,0> : public get_grid_element_categories_0d<GT> {};
template<class GT>
struct get_grid_element_categories_aux<GT,1> : public get_grid_element_categories_1d<GT> {};
template<class GT>
struct get_grid_element_categories_aux<GT,2> : public get_grid_element_categories_2d<GT> {};


template<class GT>
struct get_grid_element_categories : public get_grid_element_categories_aux<GT, GT::dimension_tag::dim> {};



template<class GT>
struct get_grid_categories_1 : 
  public 
  get_grid_element_categories
  <get_entity_category_grid_type
   <get_entity_category_test_type
    <get_entity_category_root
     <GT>
    >
   >
  >
{};

template<class GT>
class get_grid_categories : public get_grid_categories_1<GT>
{
  typedef get_grid_categories_1<GT> base;
public:
  template<class E>
  struct categories { typedef typename boost::mpl::at<typename base::category_list, E>::type type; };
};




} // namespace grid_types_detail 


template<class GRID> class grid_types;

  template<class T, int HAS_CAT> struct category_of_aux {};

  template<class T> struct category_of_aux<T,1> { typedef typename T::category type; };
  template<class T> struct category_of_aux<T,0> { 
    typedef grid_types<typename T::grid_type> gt;
    typedef typename gt::template categories<T>::type type; 
  };

  template<class T>
  struct category_of : public category_of_aux<T, has_category<T>::result> {};


  /*
  template<class X, class GT = grid_types<typename X::grid_type> >
  struct category_of : public grid_types_detail::get_grid_categories<GT>
  {
    typedef grid_types_detail::get_grid_categories<GT> base;
    typedef typename base::template categories<X>::type type;
  };

  template<class X>
  inline typename category_of<X>::type category(X const&) { return typename category_of<X>::type();}
  */

} // namespace GrAL 

#endif
