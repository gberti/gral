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

   See \c grid_types_base<> for advice and an example of specialization.

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
   typedef gt::incidence_iterator<vertex_type_tag, cell_type_tag>::type VoCIterator;
   \endcode

  This is useful in context which are generic over the element category, that is,
  are intended to work uniformly for vertices, edges etc.

  
*/
//----------------------------------------------------------------


template<class Grid>
struct grid_types {};

// Define an entity for all types occuring in grid_types<>,
// using an arbitrary macro

#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(MAKRO) \
  \
  MAKRO(vertex_handle); \
  MAKRO(edge_handle); \
  MAKRO(face_handle); \
  MAKRO(facet_handle); \
  MAKRO(cell_handle); \
  \
  MAKRO(Vertex); \
  MAKRO(Edge); \
  MAKRO(Face); \
  MAKRO(Facet); \
  MAKRO(Cell); \
  \
  MAKRO(VertexIterator); \
  MAKRO(EdgeIterator); \
  MAKRO(FaceIterator); \
  MAKRO(FacetIterator); \
  MAKRO(CellIterator); \
  \
  MAKRO(VertexOnVertexIterator); \
  MAKRO(EdgeOnVertexIterator); \
  MAKRO(FaceOnVertexIterator); \
  MAKRO(FacetOnVertexIterator); \
  MAKRO(CellOnVertexIterator); \
  \
  MAKRO(VertexOnEdgeIterator); \
  MAKRO(FaceOnEdgeIterator); \
  MAKRO(FacetOnEdgeIterator); \
  MAKRO(CellOnEdgeIterator); \
  \
  MAKRO(VertexOnFaceIterator); \
  MAKRO(EdgeOnFaceIterator); \
  MAKRO(CellOnFaceIterator); \
  \
  MAKRO(VertexOnFacetIterator); \
  MAKRO(EdgeOnFacetIterator); \
  MAKRO(CellOnFacetIterator); \
  \
  MAKRO(VertexOnCellIterator); \
  MAKRO(EdgeOnCellIterator); \
  MAKRO(FaceOnCellIterator); \
  MAKRO(FacetOnCellIterator); \
  MAKRO(CellOnCellIterator);  \
  \
  MAKRO(archetype_type); \
  MAKRO(archetype_handle); \
  MAKRO(archetype_iterator);   






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


  // the following technique for detecting member types is described
  // e.g. in Josuttis & Vandevoorde, C++ Templates, Addison-Wesley 2002

  struct size1 { char dummy;};
  struct size2 { char dummy[2];};

#define DEFINE_TESTFOR(T) \
  template<class X> size1 test_for_##T(typename X:: T *); \
  template<class X> size2 test_for_##T(...);

GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_TESTFOR);

#undef DEFINE_TESTFOR

} // namespace grid_types_detail 


/*! \defgroup gridtypesreflection Compile time reflection mechanism for grid_types<> specializations
*/

/*! \brief Compile-time mechanism to detect presence of typedefs in grid_types<> specializations
 
    \ingroup gridtypesreflection

   Analogous classes exist for other element types and iterators, 
   e.g. \c has_edge_handle, \c has_Cell<>, \c has_EdgeOnCellIterator<>, \c has_FacetIterator

 */
template<class X> struct has_Vertex;
// dummy declaration to anchor docu.

#define DEFINE_HAS_MEMBER(T) \
  template<class X> \
  struct has_##T { \
    enum { result = (sizeof(grid_types_detail::test_for_##T<X>(0)) == sizeof(grid_types_detail::size1) ? 1 : 0)}; \
  };
 
GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(DEFINE_HAS_MEMBER);

#undef HAS_MEMBER

/*! Print out which types are supported by a \c grid_types<> specialization 

    \ingroup gridtypesreflection

    \templateparams
     - \c GT is a specialization of \c grid_types<>
     - OSTREAM supports <tt> operator<<(std::string) </tt>, e.g. \c std::ostream
*/
template<class GT, class OSTREAM>
inline void checkgt(OSTREAM& out)
{
  typedef GT gt;

#define PRINT_HAS_MEMBER(T)  out << "has" << ( has_##T<gt>::result ? "   " : " no") << " type " << #T << "\n";

GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(PRINT_HAS_MEMBER);

#undef PRINT_HAS_MEMBER

}



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
