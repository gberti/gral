#ifndef GRAL_BASE_GB_GRID_TYPES_H
#define GRAL_BASE_GB_GRID_TYPES_H


// $LICENSE

#include "Gral/Base/type-tags.h"


//----------------------------------------------------------------
/*! \brief Empty traits class for parameterized namespaces associated 
    with a grid.
    \ingroup traits

   The specializations for a concrete grid type provide access to
   grid related types like
    - Vertex, Edge, ....
    - VertexIterator, ....
    - vertex_handle, ...

   See grid_types_base for advice and an example of specialization.

   \b Example for code using grid types:
   \code
    typedef grid_types<Complex2D> gt;
    for(gt::VertexIterator v = G.FirstVertex(); ...)
       do_something(*v);
    \endcode
  
   \b Dimension-independent access to sequence and incidence iterators:
   \code
   // same as gt::VertexIterator
   typedef gt::sequence_iterator<vertex_type_tag>::type VIterator;

   // same as gt::VertexOnCellIterator
   typedef gt::incidence_iterator<vertex_type_tag, cell_type_tag> VoCIterator;
   \endcode

  This is useful in context which are generic over the element category, that is,
  are intended to work uniformly for vertices, edges etc.

  
*/
//----------------------------------------------------------------


template<class Grid>
struct grid_types {};

namespace grid_types_detail 
{

  template<class GT, class ETAG>
  struct sequence_iterator_aux {};

  // specializations 
  template<class GT>
  struct sequence_iterator_aux<GT, vertex_type_tag> { typedef typename GT::VertexIterator type;};
  template<class GT>
  struct sequence_iterator_aux<GT, edge_type_tag>   { typedef typename GT::EdgeIterator   type;};
  template<class GT>
  struct sequence_iterator_aux<GT, face_type_tag>   { typedef typename GT::FaceIterator   type;};
  template<class GT>
  struct sequence_iterator_aux<GT, facet_type_tag>  { typedef typename GT::FacetIterator  type;};
  template<class GT>
  struct sequence_iterator_aux<GT, cell_type_tag>   { typedef typename GT::CellIterator   type;};


template<class GT, class ETAG_VALUE, class ETAG_ANCHOR>
struct incidence_iterator_aux {};

  // specializations

  // VertexOnXXX

  template<class GT> struct incidence_iterator_aux<GT, vertex_type_tag, vertex_type_tag>
  { typedef typename GT::VertexOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, vertex_type_tag, edge_type_tag>
  { typedef typename GT::VertexOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, vertex_type_tag, face_type_tag>
  { typedef typename GT::VertexOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, vertex_type_tag, facet_type_tag>
  { typedef typename GT::VertexOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, vertex_type_tag, cell_type_tag>
  { typedef typename GT::VertexOnCellIterator type;};

  // EdgeOnXXX

  template<class GT> struct incidence_iterator_aux<GT, edge_type_tag, vertex_type_tag>
  { typedef typename GT::EdgeOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, edge_type_tag, edge_type_tag>
  { typedef typename GT::EdgeOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, edge_type_tag, face_type_tag>
  { typedef typename GT::EdgeOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, edge_type_tag, facet_type_tag>
  { typedef typename GT::EdgeOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, edge_type_tag, cell_type_tag>
  { typedef typename GT::EdgeOnCellIterator type;};

  // FaceOnXXX

  template<class GT> struct incidence_iterator_aux<GT, face_type_tag, vertex_type_tag>
  { typedef typename GT::FaceOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, face_type_tag, edge_type_tag>
  { typedef typename GT::FaceOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, face_type_tag, face_type_tag>
  { typedef typename GT::FaceOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, face_type_tag, facet_type_tag>
  { typedef typename GT::FaceOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, face_type_tag, cell_type_tag>
  { typedef typename GT::FaceOnCellIterator type;};

  // FacetOnXXX

  template<class GT> struct incidence_iterator_aux<GT, facet_type_tag, vertex_type_tag>
  { typedef typename GT::FacetOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, facet_type_tag, edge_type_tag>
  { typedef typename GT::FacetOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, facet_type_tag, face_type_tag>
  { typedef typename GT::FacetOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, facet_type_tag, facet_type_tag>
  { typedef typename GT::FacetOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, facet_type_tag, cell_type_tag>
  { typedef typename GT::FacetOnCellIterator type;};

  // CellOnXXX

  template<class GT> struct incidence_iterator_aux<GT, cell_type_tag, vertex_type_tag>
  { typedef typename GT::CellOnVertexIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, cell_type_tag, edge_type_tag>
  { typedef typename GT::CellOnEdgeIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, cell_type_tag, face_type_tag>
  { typedef typename GT::CellOnFaceIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, cell_type_tag, facet_type_tag>
  { typedef typename GT::CellOnFacetIterator type;};
  template<class GT> struct incidence_iterator_aux<GT, cell_type_tag, cell_type_tag>
  { typedef typename GT::CellOnCellIterator type;};


  struct size1 { char dummy;};
  struct size2 { char dummy[2];};

#define DEFINE_TESTFOR(T) \
  template<class X> size1 test_for_##T(typename X:: T *); \
  template<class X> size2 test_for_##T(...);


  DEFINE_TESTFOR(vertex_handle);
  DEFINE_TESTFOR(edge_handle);
  DEFINE_TESTFOR(face_handle);
  DEFINE_TESTFOR(facet_handle);
  DEFINE_TESTFOR(cell_handle);

  DEFINE_TESTFOR(Vertex);
  DEFINE_TESTFOR(Edge);
  DEFINE_TESTFOR(Face);
  DEFINE_TESTFOR(Facet);
  DEFINE_TESTFOR(Cell);

  DEFINE_TESTFOR(VertexIterator);
  DEFINE_TESTFOR(EdgeIterator);
  DEFINE_TESTFOR(FaceIterator);
  DEFINE_TESTFOR(FacetIterator);
  DEFINE_TESTFOR(CellIterator);

  DEFINE_TESTFOR(VertexOnVertexIterator);
  DEFINE_TESTFOR(EdgeOnVertexIterator);
  DEFINE_TESTFOR(FaceOnVertexIterator);
  DEFINE_TESTFOR(FacetOnVertexIterator);
  DEFINE_TESTFOR(CellOnVertexIterator);

  DEFINE_TESTFOR(VertexOnEdgeIterator);
  DEFINE_TESTFOR(FaceOnEdgeIterator);
  DEFINE_TESTFOR(FacetOnEdgeIterator);
  DEFINE_TESTFOR(CellOnEdgeIterator);

  DEFINE_TESTFOR(VertexOnFaceIterator);
  DEFINE_TESTFOR(EdgeOnFaceIterator);
  DEFINE_TESTFOR(CellOnFaceIterator);

  DEFINE_TESTFOR(VertexOnFacetIterator);
  DEFINE_TESTFOR(EdgeOnFacetIterator);
  DEFINE_TESTFOR(CellOnFacetIterator);

  DEFINE_TESTFOR(VertexOnCellIterator);
  DEFINE_TESTFOR(EdgeOnCellIterator);
  DEFINE_TESTFOR(FaceOnCellIterator);
  DEFINE_TESTFOR(FacetOnCellIterator);
  DEFINE_TESTFOR(CellOnCellIterator);


  /*
  template<class X>
  size1 test_for_Vertex(typename X::mytype *);
  template<class X>
  size2 test_for_Vertex(...);
  */

#undef DEFINE_TESTFOR

} // namespace grid_types_detail 


#define DEFINE_HAS_MEMBER(T) \
  template<class X> \
  struct has_##T { \
    enum { result = (sizeof(grid_types_detail::test_for_##T<X>(0)) == sizeof(grid_types_detail::size1) ? 1 : 0)}; \
  };
 

DEFINE_HAS_MEMBER(vertex_handle);
DEFINE_HAS_MEMBER(edge_handle);
DEFINE_HAS_MEMBER(face_handle);
DEFINE_HAS_MEMBER(facet_handle);
DEFINE_HAS_MEMBER(cell_handle);

DEFINE_HAS_MEMBER(Vertex);
DEFINE_HAS_MEMBER(Edge);
DEFINE_HAS_MEMBER(Face);
DEFINE_HAS_MEMBER(Facet);
DEFINE_HAS_MEMBER(Cell);

DEFINE_HAS_MEMBER(VertexIterator);
DEFINE_HAS_MEMBER(EdgeIterator);
DEFINE_HAS_MEMBER(FaceIterator);
DEFINE_HAS_MEMBER(FacetIterator);
DEFINE_HAS_MEMBER(CellIterator);

DEFINE_HAS_MEMBER(VertexOnVertexIterator);
DEFINE_HAS_MEMBER(EdgeOnVertexIterator);
DEFINE_HAS_MEMBER(FaceOnVertexIterator);
DEFINE_HAS_MEMBER(FacetOnVertexIterator);
DEFINE_HAS_MEMBER(CellOnVertexIterator);

DEFINE_HAS_MEMBER(VertexOnEdgeIterator);
DEFINE_HAS_MEMBER(FaceOnEdgeIterator);
DEFINE_HAS_MEMBER(FacetOnEdgeIterator);
DEFINE_HAS_MEMBER(CellOnEdgeIterator);

DEFINE_HAS_MEMBER(VertexOnFaceIterator);
DEFINE_HAS_MEMBER(EdgeOnFaceIterator);
DEFINE_HAS_MEMBER(CellOnFaceIterator);

DEFINE_HAS_MEMBER(VertexOnFacetIterator);
DEFINE_HAS_MEMBER(EdgeOnFacetIterator);
DEFINE_HAS_MEMBER(CellOnFacetIterator);

DEFINE_HAS_MEMBER(VertexOnCellIterator);
DEFINE_HAS_MEMBER(EdgeOnCellIterator);
DEFINE_HAS_MEMBER(FaceOnCellIterator);
DEFINE_HAS_MEMBER(FacetOnCellIterator);
DEFINE_HAS_MEMBER(CellOnCellIterator);



/*
template<class X>
struct has_Vertex {
  enum { result = (sizeof(grid_types_detail::test_for_Vertex<X>(0)) == sizeof(size1) ? 1 : 0)};
};
*/

#undef HAS_MEMBER

namespace grid_types_detail {

#define DEFINE_SELECT_GT(T) \
    template<int TEST, typename GT1, typename GT2> \
    struct select_##T              { typedef typename GT1:: T result;}; \
    \
    template<          typename GT1, typename GT2> \
    struct select_##T<0, GT1, GT2> { typedef typename GT2:: T result;};


DEFINE_SELECT_GT(vertex_handle);
DEFINE_SELECT_GT(edge_handle);
DEFINE_SELECT_GT(face_handle);
DEFINE_SELECT_GT(facet_handle);
DEFINE_SELECT_GT(cell_handle);

DEFINE_SELECT_GT(Vertex);
DEFINE_SELECT_GT(Edge);
DEFINE_SELECT_GT(Face);
DEFINE_SELECT_GT(Facet);
DEFINE_SELECT_GT(Cell);

DEFINE_SELECT_GT(VertexIterator);
DEFINE_SELECT_GT(EdgeIterator);
DEFINE_SELECT_GT(FaceIterator);
DEFINE_SELECT_GT(FacetIterator);
DEFINE_SELECT_GT(CellIterator);

DEFINE_SELECT_GT(VertexOnVertexIterator);
DEFINE_SELECT_GT(EdgeOnVertexIterator);
DEFINE_SELECT_GT(FaceOnVertexIterator);
DEFINE_SELECT_GT(FacetOnVertexIterator);
DEFINE_SELECT_GT(CellOnVertexIterator);

DEFINE_SELECT_GT(VertexOnEdgeIterator);
DEFINE_SELECT_GT(FaceOnEdgeIterator);
DEFINE_SELECT_GT(FacetOnEdgeIterator);
DEFINE_SELECT_GT(CellOnEdgeIterator);

DEFINE_SELECT_GT(VertexOnFaceIterator);
DEFINE_SELECT_GT(EdgeOnFaceIterator);
DEFINE_SELECT_GT(CellOnFaceIterator);

DEFINE_SELECT_GT(VertexOnFacetIterator);
DEFINE_SELECT_GT(EdgeOnFacetIterator);
DEFINE_SELECT_GT(CellOnFacetIterator);

DEFINE_SELECT_GT(VertexOnCellIterator);
DEFINE_SELECT_GT(EdgeOnCellIterator);
DEFINE_SELECT_GT(FaceOnCellIterator);
DEFINE_SELECT_GT(FacetOnCellIterator);
DEFINE_SELECT_GT(CellOnCellIterator);

#undef DEFINE_SELECT_GT

} // namespace grid_types_detail

/*! \brief Plugin-in base class for grid_types<> specializations
    
   \b Usage example:
   \code
   class MyGrid { ... };
   struct grid_types_mygrid { ... };

   template<>
   struct grid_types<MyGrid> : public grid_types_base<grid_types_mygrid> {};
   \endcode

   \todo This is not yet used in all grid_types<> specializations!
*/
template<class GTBASE>
struct grid_types_base : public GTBASE
{
  template<class ETAG>
  struct sequence_iterator : public grid_types_detail::sequence_iterator_aux<GTBASE,ETAG> {};
  
  template<class ETAG_VALUE, class ETAG_ANCHOR>
  struct incidence_iterator : public grid_types_detail::incidence_iterator_aux<GTBASE, ETAG_VALUE, ETAG_ANCHOR> {};
};





#endif
