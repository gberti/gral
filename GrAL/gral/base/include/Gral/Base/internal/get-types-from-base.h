#ifndef GRAL_GB_BASE_INTERNAL_GET_TYPES_FROM_BASE_H
#define GRAL_GB_BASE_INTERNAL_GET_TYPES_FROM_BASE_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/internal/define-grid-entities.h"


namespace GrAL {

#define DEFINE_GET_FROM_BASE_GT(T) \
  template<int NEW_HAS, int OLD_HAS, class NEW_GT, class OLD_GT>	   \
  struct get_from_base_gt_aux_##T : public NEW_GT {};		   \
  \
  template<class NEW_GT, class OLD_GT>					\
  struct get_from_base_gt_aux_##T<0,1,NEW_GT, OLD_GT> : public NEW_GT \
  { typedef typename OLD_GT:: T  T; }; \
  \
  template<class NEW_GT, class OLD_GT> \
  struct get_from_base_gt_##T : public get_from_base_gt_aux_##T <has_##T<NEW_GT>::result, \
                                                                 has_##T<OLD_GT>::result, \
                                                                 NEW_GT, OLD_GT> \
  {};


GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_GET_FROM_BASE_GT);

#undef DEFINE_GET_FROM_BASE_GT

template<class NEW_GT, class OLD_GT>
struct get_element_types_from_base
  :  public get_from_base_gt_Vertex<NEW_GT,OLD_GT>,
     public get_from_base_gt_Edge  <NEW_GT,OLD_GT>,
     public get_from_base_gt_Face  <NEW_GT,OLD_GT>,
     public get_from_base_gt_Facet <NEW_GT,OLD_GT>,
     public get_from_base_gt_Cell  <NEW_GT,OLD_GT>
{};

template<class NEW_GT, class OLD_GT>
struct get_element_handle_types_from_base
  :  public get_from_base_gt_vertex_handle<NEW_GT,OLD_GT>,
     public get_from_base_gt_edge_handle  <NEW_GT,OLD_GT>,
     public get_from_base_gt_face_handle  <NEW_GT,OLD_GT>,
     public get_from_base_gt_facet_handle <NEW_GT,OLD_GT>,
     public get_from_base_gt_cell_handle  <NEW_GT,OLD_GT>
{
  static typename  get_from_base_gt_cell_handle  <NEW_GT,OLD_GT>::cell_handle 
  invalid_cell_handle(typename NEW_GT::grid_type const& g) 
  { return OLD_GT::invalid_cell_handle(g.BaseGrid()); }
};


  template<class NEW_GT, class OLD_GT>
  struct get_downward_incidence_iterator_types_from_base
    :  public get_from_base_gt_VertexOnEdgeIterator <NEW_GT,OLD_GT>,
       public get_from_base_gt_VertexOnFaceIterator <NEW_GT,OLD_GT>,
       public get_from_base_gt_VertexOnFacetIterator<NEW_GT,OLD_GT>,
       public get_from_base_gt_VertexOnCellIterator <NEW_GT,OLD_GT>,
													 
       public get_from_base_gt_EdgeOnFaceIterator  <NEW_GT,OLD_GT>,
       public get_from_base_gt_EdgeOnFacetIterator <NEW_GT,OLD_GT>,
       public get_from_base_gt_EdgeOnCellIterator  <NEW_GT,OLD_GT>,
													 
       public get_from_base_gt_FaceOnFacetIterator <NEW_GT,OLD_GT>,
       public get_from_base_gt_FaceOnCellIterator  <NEW_GT,OLD_GT>,
													 
       public get_from_base_gt_FacetOnCellIterator <NEW_GT,OLD_GT>
  {};


  template<class NEW_GT, class OLD_GT>
  struct get_archetype_types_from_base :
    public get_from_base_gt_archetype_type    <NEW_GT, OLD_GT>,
    public get_from_base_gt_archetype_handle  <NEW_GT, OLD_GT>,
    public get_from_base_gt_archetype_iterator<NEW_GT, OLD_GT>,
    public get_from_base_gt_archgt            <NEW_GT, OLD_GT>
  {};

  template<class NEW_GT, class OLD_GT>
  struct get_local_types_from_base :
    public get_element_handle_types_from_base             <NEW_GT, OLD_GT>,
    public get_element_types_from_base                    <NEW_GT, OLD_GT>,
    public get_downward_incidence_iterator_types_from_base<NEW_GT, OLD_GT>,
    public get_archetype_types_from_base                  <NEW_GT, OLD_GT>
  { };


}

#endif
