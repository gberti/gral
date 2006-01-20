#ifndef GRAL_GB_BASE_INTERNAL_TYPE_ALIASES_H
#define GRAL_GB_BASE_INTERNAL_TYPE_ALIASES_H

// $LICENSE_NEC_2006

#include "Gral/Base/internal/compile-time-reflection.h"

namespace GrAL { namespace grid_types_detail {

  // Automatically define dimension dependent aliases for types

#define DEFINE_A_AS_B(A,B) \
  template<class GT,int HAS_A, int HAS_B> \
  struct map_ ## A ## _as_ ## B ## _aux          : public GT {}; \
  template<class GT> \
  struct map_ ## A ## _as_ ## B ## _aux <GT,1,0> : public GT { typedef typename GT::A B;}; \
  \
  template<class GT> struct map_ ## A ## _as_ ## B \
   : public map_ ## A ## _as_ ## B ## _aux<GT, has_##A<GT>::result, has_##B<GT>::result> {}; 

#define DEFINE_A_IS_B(A,B) \
  DEFINE_A_AS_B(A,B); \
  DEFINE_A_AS_B(B,A); \
  template<class GT> class  A ## _is_ ## B \
   : public map_ ## A ## _as_ ## B < map_ ## B ## _as_ ## A <GT> > {};

#define DEFINE_A_IS_B_ALL_INC(A,B) \
  DEFINE_A_IS_B(A ## OnVertexIterator, B ## OnVertexIterator); \
  DEFINE_A_IS_B(A ## OnEdgeIterator,   B ## OnEdgeIterator); \
  DEFINE_A_IS_B(A ## OnFaceIterator,   B ## OnFaceIterator); \
  DEFINE_A_IS_B(A ## OnFacetIterator,  B ## OnFacetIterator); \
  DEFINE_A_IS_B(A ## OnCellIterator,   B ## OnCellIterator); \
  DEFINE_A_IS_B(VertexOn ## A ## Iterator, VertexOn ## B ## Iterator); \
  DEFINE_A_IS_B(EdgeOn   ## A ## Iterator, EdgeOn   ## B ## Iterator); \
  DEFINE_A_IS_B(FaceOn   ## A ## Iterator, FaceOn   ## B ## Iterator); \
  DEFINE_A_IS_B(FacetOn  ## A ## Iterator, FacetOn  ## B ## Iterator); \
  DEFINE_A_IS_B(CellOn   ## A ## Iterator, CellOn   ## B ## Iterator); \



  // 0D
  DEFINE_A_IS_B(Vertex,Cell);
  DEFINE_A_IS_B(vertex_handle, cell_handle);
  DEFINE_A_IS_B(VertexIterator,CellIterator);
  DEFINE_A_IS_B_ALL_INC(Vertex,Cell);



  // 1D
  DEFINE_A_IS_B(Edge,Cell);
  DEFINE_A_IS_B(edge_handle,cell_handle);
  DEFINE_A_IS_B(EdgeIterator,CellIterator);
  DEFINE_A_IS_B_ALL_INC(Edge,Cell);


  DEFINE_A_IS_B(Vertex,Facet);
  DEFINE_A_IS_B(vertex_handle, facet_handle);
  DEFINE_A_IS_B(VertexIterator,FacetIterator);
  DEFINE_A_IS_B_ALL_INC(Vertex,Facet);

  // 2D
  DEFINE_A_IS_B(Cell,Face);
  DEFINE_A_IS_B(cell_handle,  face_handle);
  DEFINE_A_IS_B(CellIterator, FaceIterator);
  //DEFINE_A_IS_B(CellOnCellIterator, CellOnFaceIterator);
  DEFINE_A_IS_B_ALL_INC(Cell,Face);


  DEFINE_A_IS_B(Edge,Facet);
  DEFINE_A_IS_B(edge_handle,facet_handle);
  DEFINE_A_IS_B(EdgeIterator,FacetIterator);
  DEFINE_A_IS_B_ALL_INC(Edge,Facet);

  // 3D
  DEFINE_A_IS_B(Facet,Face);
  DEFINE_A_IS_B(facet_handle,  face_handle);
  DEFINE_A_IS_B(FacetIterator, FaceIterator);
  DEFINE_A_IS_B_ALL_INC(Facet,Face);


#undef DEFINE_A_IS_B
#undef DEFINE_A_IS_B_ALL_INC
  
#define DEFINE_A_IS_B_INC_ITERS(A,B) \
    A ## OnVertexIterator ## _is_ ## B  ## OnVertexIterator \
    <A ## OnEdgeIterator   ## _is_ ## B  ## OnEdgeIterator \
     <A ## OnFaceIterator ## _is_ ## B  ## OnFaceIterator \
      <A ## OnFacetIterator ## _is_ ## B  ## OnFacetIterator \
       <A ## OnCellIterator ## _is_ ## B  ## OnCellIterator \
        <VertexOn ## A ## Iterator_is_VertexOn ## B ## Iterator \
	 <EdgeOn   ## A ## Iterator_is_EdgeOn   ## B ## Iterator	\
	  <FaceOn   ## A ## Iterator_is_FaceOn   ## B ## Iterator	\
	   <FacetOn  ## A ## Iterator_is_FacetOn  ## B ## Iterator	\
	    <CellOn   ## A ## Iterator_is_CellOn   ## B ## Iterator	\
	     <GT>							\
            > \
           > \
          > \
         > \
        > \
       > \
      > \
     > \
    > \



#define DEFINE_A_IS_B_ALL(A,a,B,b) \
  template<class GT> struct a ## _is_ ## b : public \
  A ## _is_ ## B \
  < a ## _handle_is_ ## b ## _handle				\
   < A ## Iterator_is_ ## B ## Iterator			\
     < DEFINE_A_IS_B_INC_ITERS(A,B) >			\
   > \
  > \
  {};

  

  // 0D
  DEFINE_A_IS_B_ALL(Vertex,vertex,Cell,cell);
  // 1D 
  DEFINE_A_IS_B_ALL(Vertex,vertex,Facet,facet);
  DEFINE_A_IS_B_ALL(Edge,  edge,  Cell, cell);
  // 2D
  DEFINE_A_IS_B_ALL(Cell,cell,Face,face);
  DEFINE_A_IS_B_ALL(Edge,edge,Facet,facet);
  // 3D
  DEFINE_A_IS_B_ALL(Facet, facet, Face, face);


  template<class GT, int D> struct dim_dep_defs_aux : public GT {};
  template<class GT> struct dim_dep_defs_aux<GT,0> : public vertex_is_cell              <GT>   {};
  template<class GT> struct dim_dep_defs_aux<GT,1> : public edge_is_cell<vertex_is_facet<GT> > {};
  template<class GT> struct dim_dep_defs_aux<GT,2> : public cell_is_face<edge_is_facet  <GT> > {};
  template<class GT> struct dim_dep_defs_aux<GT,3> : public facet_is_face               <GT>   {};


  template<class GT> struct dim_dep_defs : public dim_dep_defs_aux<GT, GT::dimension_tag::dim> {};

}} // namespace GrAL { namespace grid_types_detail

#endif
