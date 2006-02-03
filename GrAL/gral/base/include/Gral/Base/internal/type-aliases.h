#ifndef GRAL_GB_BASE_INTERNAL_TYPE_ALIASES_H
#define GRAL_GB_BASE_INTERNAL_TYPE_ALIASES_H

// $LICENSE_NEC_2006

#include "Gral/Base/type-tags.h"
#include "Gral/Base/internal/compile-time-reflection.h"
#include "Gral/Base/internal/iterator-type-maps.h"

namespace GrAL { 

  namespace grid_types_detail { namespace type_aliases {

    template<class GT, int D> struct  dim_dep_defs_dim : public GT {};


    // specializations for D = 0 - 3

    template<class GT> 
    struct dim_dep_defs_dim<GT,0> : public GT {

      typedef typename element_handle_d    <GT,0>  ::type vertex_handle;
      typedef typename element_handle_d    <GT,0>  ::type cell_handle;

      typedef typename element_d           <GT,0>  ::type Vertex;
      typedef typename element_d           <GT,0>  ::type Cell;

      typedef typename sequence_iterator_d <GT,0>  ::type VertexIterator;
      typedef typename sequence_iterator_d <GT,0>  ::type CellIterator;

      typedef typename incidence_iterator_d<GT,0,0>::type VertexOnVertexIterator;
      typedef typename incidence_iterator_d<GT,0,0>::type VertexOnCellIterator;
      typedef typename incidence_iterator_d<GT,0,0>::type CellOnVertexIterator;
      typedef typename incidence_iterator_d<GT,0,0>::type CellOnCellIterator;
    };




    template<class GT> 
    struct dim_dep_defs_dim<GT,1> : public GT {

      typedef typename element_handle_d    <GT,0>  ::type vertex_handle;
      typedef typename element_handle_d    <GT,0>  ::type facet_handle;
      typedef typename element_handle_d    <GT,1>  ::type edge_handle;
      typedef typename element_handle_d    <GT,1>  ::type cell_handle;

      typedef typename element_d           <GT,0>  ::type Vertex;
      typedef typename element_d           <GT,0>  ::type Facet;
      typedef typename element_d           <GT,1>  ::type Edge;
      typedef typename element_d           <GT,1>  ::type Cell;

      typedef typename sequence_iterator_d <GT,0>  ::type VertexIterator;
      typedef typename sequence_iterator_d <GT,0>  ::type FacetIterator;
      typedef typename sequence_iterator_d <GT,1>  ::type EdgeIterator;
      typedef typename sequence_iterator_d <GT,1>  ::type CellIterator;

      typedef typename incidence_iterator_d<GT,0,0>::type VertexOnVertexIterator;
      typedef typename incidence_iterator_d<GT,0,0>::type VertexOnFacetIterator;
      typedef typename incidence_iterator_d<GT,0,0>::type FacetOnVertexIterator;
      typedef typename incidence_iterator_d<GT,0,0>::type FacetOnFacetIterator;

      typedef typename incidence_iterator_d<GT,0,1>::type VertexOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,0,1>::type VertexOnCellIterator;
      typedef typename incidence_iterator_d<GT,0,1>::type FacetOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,0,1>::type FacetOnCellIterator;

      typedef typename incidence_iterator_d<GT,1,0>::type EdgeOnVertexIterator;
      typedef typename incidence_iterator_d<GT,1,0>::type EdgeOnFacetIterator;
      typedef typename incidence_iterator_d<GT,1,0>::type CellOnVertexIterator;
      typedef typename incidence_iterator_d<GT,1,0>::type CellOnFacetIterator;

      typedef typename incidence_iterator_d<GT,1,1>::type EdgeOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,1,1>::type EdgeOnCellIterator;
      typedef typename incidence_iterator_d<GT,1,1>::type CellOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,1,1>::type CellOnCellIterator;
    };



    template<class GT> 
    struct dim_dep_defs_dim<GT,2> : public GT {

      typedef typename element_handle_d    <GT,0>  ::type vertex_handle;
      typedef typename element_handle_d    <GT,1>  ::type edge_handle;
      typedef typename element_handle_d    <GT,2>  ::type face_handle;
      typedef typename element_handle_d    <GT,1>  ::type facet_handle;
      typedef typename element_handle_d    <GT,2>  ::type cell_handle;

      typedef typename element_d           <GT,0>  ::type Vertex;
      typedef typename element_d           <GT,1>  ::type Edge;
      typedef typename element_d           <GT,2>  ::type Face;
      typedef typename element_d           <GT,1>  ::type Facet;
      typedef typename element_d           <GT,2>  ::type Cell;

      typedef typename sequence_iterator_d <GT,0>  ::type VertexIterator;
      typedef typename sequence_iterator_d <GT,1>  ::type EdgeIterator;
      typedef typename sequence_iterator_d <GT,2>  ::type FaceIterator;
      typedef typename sequence_iterator_d <GT,1>  ::type FacetIterator;
      typedef typename sequence_iterator_d <GT,2>  ::type CellIterator;


      typedef typename incidence_iterator_d<GT,0,1>::type VertexOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,0,1>::type VertexOnFacetIterator;

      typedef typename incidence_iterator_d<GT,0,2>::type VertexOnFaceIterator;
      typedef typename incidence_iterator_d<GT,0,2>::type VertexOnCellIterator;


      typedef typename incidence_iterator_d<GT,1,0>::type EdgeOnVertexIterator;
      typedef typename incidence_iterator_d<GT,1,0>::type FacetOnVertexIterator;

      typedef typename incidence_iterator_d<GT,1,2>::type EdgeOnFaceIterator;
      typedef typename incidence_iterator_d<GT,1,2>::type EdgeOnCellIterator;
      typedef typename incidence_iterator_d<GT,1,2>::type FacetOnFaceIterator;
      typedef typename incidence_iterator_d<GT,1,2>::type FacetOnCellIterator;


      typedef typename incidence_iterator_d<GT,2,0>::type CellOnVertexIterator;
      typedef typename incidence_iterator_d<GT,2,0>::type FaceOnVertexIterator;

      typedef typename incidence_iterator_d<GT,2,1>::type FaceOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,2,1>::type FaceOnFacetIterator;
      typedef typename incidence_iterator_d<GT,2,1>::type CellOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,2,1>::type CellOnFacetIterator;

      typedef typename incidence_iterator_d<GT,2,2>::type FaceOnFaceIterator;
      typedef typename incidence_iterator_d<GT,2,2>::type FaceOnCellIterator;
      typedef typename incidence_iterator_d<GT,2,2>::type CellOnFaceIterator;
      typedef typename incidence_iterator_d<GT,2,2>::type CellOnCellIterator;
    };




    template<class GT> 
    struct dim_dep_defs_dim<GT,3> : public GT {

      typedef typename element_handle_d    <GT,0>  ::type vertex_handle;
      typedef typename element_handle_d    <GT,1>  ::type edge_handle;
      typedef typename element_handle_d    <GT,2>  ::type face_handle;
      typedef typename element_handle_d    <GT,2>  ::type facet_handle;
      typedef typename element_handle_d    <GT,3>  ::type cell_handle;

      typedef typename element_d           <GT,0>  ::type Vertex;
      typedef typename element_d           <GT,1>  ::type Edge;
      typedef typename element_d           <GT,2>  ::type Face;
      typedef typename element_d           <GT,2>  ::type Facet;
      typedef typename element_d           <GT,3>  ::type Cell;

      typedef typename sequence_iterator_d <GT,0>  ::type VertexIterator;
      typedef typename sequence_iterator_d <GT,1>  ::type EdgeIterator;
      typedef typename sequence_iterator_d <GT,2>  ::type FaceIterator;
      typedef typename sequence_iterator_d <GT,2>  ::type FacetIterator;
      typedef typename sequence_iterator_d <GT,3>  ::type CellIterator;



      typedef typename incidence_iterator_d<GT,0,2>::type VertexOnFaceIterator;
      typedef typename incidence_iterator_d<GT,0,2>::type VertexOnFacetIterator;


      typedef typename incidence_iterator_d<GT,1,2>::type EdgeOnFaceIterator;
      typedef typename incidence_iterator_d<GT,1,2>::type EdgeOnFacetIterator;


      typedef typename incidence_iterator_d<GT,2,0>::type FaceOnVertexIterator;
      typedef typename incidence_iterator_d<GT,2,0>::type FacetOnVertexIterator;

      typedef typename incidence_iterator_d<GT,2,1>::type FaceOnEdgeIterator;
      typedef typename incidence_iterator_d<GT,2,1>::type FacetOnEdgeIterator;

      typedef typename incidence_iterator_d<GT,2,3>::type FaceOnCellIterator;
      typedef typename incidence_iterator_d<GT,2,3>::type FacetOnCellIterator;


      typedef typename incidence_iterator_d<GT,3,2>::type CellOnFaceIterator;
      typedef typename incidence_iterator_d<GT,3,2>::type CellOnFacetIterator;
    };


  }} //   namespace grid_types_detail { namespace type_aliases {

  template<class GT>
  struct dim_dep_defs : grid_types_detail::type_aliases::dim_dep_defs_dim<GT, dimgt<GT>::value> {};


} // namespace GrAL

#endif
