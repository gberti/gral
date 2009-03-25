#ifndef GRAL_GB_BASE_PREDICATES_H
#define GRAL_GB_BASE_PREDICATES_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"

namespace GrAL {
	
  namespace detail {

    template<class tag1, class tag2, class E1, class E2, class GT>
    struct incidence_aux {};

    // XXXOnCell

    template<class E1, class E2, class GT>
    struct incidence_aux<vertex_type_tag, cell_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::VertexOnCellIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<edge_type_tag, cell_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::EdgeOnCellIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<face_type_tag, cell_type_tag,E1,E2,GT> 
    { typedef typename GT::FaceOnCellIterator iterator; };

    template<class E1, class E2, class GT>
    struct incidence_aux<facet_type_tag, cell_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FacetOnCellIterator iterator;
    };
    
    // XXXOnFace

    template<class E1, class E2, class GT>
    struct incidence_aux<vertex_type_tag, face_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::VertexOnFaceIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<edge_type_tag, face_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::EdgeOnFaceIterator   iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<facet_type_tag, face_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FacetOnFaceIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<cell_type_tag, face_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::CellOnFaceIterator iterator;
    };


    // XXXOnFacet 

    template<class E1, class E2, class GT>
    struct incidence_aux<vertex_type_tag, facet_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::VertexOnFacetIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<edge_type_tag, facet_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::EdgeOnFacetIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<face_type_tag, facet_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FaceOnFacetIterator iterator;
    };
    template<class E1, class E2, class GT>
    struct incidence_aux<cell_type_tag, facet_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::CellOnFacetIterator iterator;
    };

    // XXXOnEdge

    template<class E1, class E2, class GT>
    struct incidence_aux<vertex_type_tag, edge_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::VertexOnEdgeIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<face_type_tag, edge_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FaceOnEdgeIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<facet_type_tag, edge_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FacetOnEdgeIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<cell_type_tag, edge_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::CellOnEdgeIterator iterator;
    };

    // XXXOnVertex
    template<class E1, class E2, class GT>
    struct incidence_aux<edge_type_tag, vertex_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::EdgeOnVertexIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<face_type_tag, vertex_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FaceOnVertexIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<facet_type_tag, vertex_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::FacetOnVertexIterator iterator;
    };

    template<class E1, class E2, class GT>
    struct incidence_aux<cell_type_tag, vertex_type_tag,E1,E2,GT> 
    { 
      typedef typename GT::CellOnVertexIterator iterator;
    };


  } // namespace detail



  /*! \brief Map element types pairs to corresponding incidence iterator types
   */
  template<class E1, class E2, class GT = grid_types<typename element_traits<E1>::grid_type> >
  struct incidence : public detail::incidence_aux<typename element_traits<E1>::element_type_tag,
						  typename element_traits<E2>::element_type_tag,
						  E1, E2, GT>
  {};

/*! \brief Meta-function to get VertexOn<E> incidence iterator

   \ingroup  traits
   \see \ref traits
 */
template<class E>
struct VertexOn {
  typedef element_traits<E> et;
  typedef grid_types<typename et::grid_type> gt;
  typedef typename gt::Vertex Vertex;
  // REQUIRE vt::element_type_tag = vertex_type_tag
  typedef typename incidence<Vertex,E>::iterator Iterator;
  typedef typename incidence<Vertex,E>::iterator type;

};



/*! \brief Predicate to decide whether two elements are incident.
    \ingroup predicates
    \see \ref predicates    

    
    \see Tested in \ref test-incident.C
    \todo branch on iterator capabilities:
          Use either E1OnE2Iter or E2OnE1Iter.
*/
template<class E1, class E2>
bool incident(E1 const& e1, E2 const& e2)
{ 
  typedef typename incidence<E1,E2>::iterator E1OnE2Iterator;
  for(E1OnE2Iterator e1e2(e2); ! e1e2.IsDone(); ++e1e2)
    if(*e1e2 == e1)
      return true;
  return false;
}

/*! \brief Predicate to decide whether two elements are incident.
    \ingroup predicates
    \see \ref predicates    

    This take an extra template paramter GT which must be specified explicitely.
    It can be use to plug in additional incidence iterators definitions:

    \code
    typedef xgt<gt, override_CellOnVertexIterator<my_cell_on_vertex_iterator> > mygt;
    // ...
    if(incident<mygt>(c,v))
       // do something
    \endcode
    
    \see Tested in \ref test-incident.C
    \todo branch on iterator capabilities:
          Use either E1OnE2Iter or E2OnE1Iter.
*/
template<class GT, class E1, class E2>
bool incident(E1 const& e1, E2 const& e2)
{ 
  typedef typename incidence<E1,E2, GT>::iterator E1OnE2Iterator;
  for(E1OnE2Iterator e1e2(e2); ! e1e2.IsDone(); ++e1e2)
    if(*e1e2 == e1)
      return true;
  return false;
}



} // namespace GrAL 

#endif
