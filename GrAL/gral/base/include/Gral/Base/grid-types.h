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
  struct sequence_iterator_aux<GT, face_type_tag>   { typedef typename GTCGT::cell_handle>::FaceIterator   type;};
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
class grid_types_base : public GTBASE
{
  template<class ETAG>
  struct sequence_iterator : public grid_types_detail::sequence_iterator_aux<GTBASE,ETAG> {};
  
  template<class ETAG_VALUE, class ETAG_ANCHOR>
  struct incidence_iterator : public grid_types_detail::incidence_iterator_aux<GTBASE, ETAG_VALUE, ETAG_ANCHOR> {};
};


#endif
