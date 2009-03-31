#ifndef GRAL_GB_BASE_SUBRANGES_INTERNAL_ELEMENT_RANGE_AUX_H
#define GRAL_GB_BASE_SUBRANGES_INTERNAL_ELEMENT_RANGE_AUX_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"

namespace GrAL { namespace detail {

  template<class G> struct map_elem_names_base {
    typedef typename G::Element         Element;
    typedef typename G::ElementIterator ElementIterator;
    
  };

  template<class G, class ETAG> struct map_elem_names {};

  template<class G> struct map_elem_names<G, vertex_type_tag> 
    : public map_elem_names_base<G>
  {
    typedef typename G::element_handle  vertex_handle;
    typedef typename G::Element         Vertex;
    typedef typename G::ElementIterator VertexIterator;
  };

  template<class G> struct map_elem_names<G, edge_type_tag> 
    : public map_elem_names_base<G>
  {
    typedef typename G::element_handle  edge_handle;
    typedef typename G::Element         Edge;
    typedef typename G::ElementIterator EdgeIterator;
  };

  template<class G> struct map_elem_names<G, face_type_tag> 
    : public map_elem_names_base<G>
  {
    typedef typename G::element_handle  face_handle;
    typedef typename G::Element         Face;
    typedef typename G::ElementIterator FaceIterator;
  };

  template<class G> struct map_elem_names<G, facet_type_tag> 
    : public map_elem_names_base<G>
  {
    typedef typename G::element_handle  facet_handle;
    typedef typename G::Element         Facet;
    typedef typename G::ElementIterator FacetIterator;
  };

  template<class G> struct map_elem_names<G, cell_type_tag> 
    : public map_elem_names_base<G>
  {
    typedef typename G::element_handle  cell_handle;
    typedef typename G::Element         Cell;
    typedef typename G::ElementIterator CellIterator;
  };



} } // namespace GrAL { namespace detail 

#endif
