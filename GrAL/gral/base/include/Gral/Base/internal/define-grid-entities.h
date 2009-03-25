#ifndef GRAL_GB_BASE_INTERNAL_DEFINE_GRID_ENTITIES_H
#define GRAL_GB_BASE_INTERNAL_DEFINE_GRID_ENTITIES_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */




// Define an entity for all types occuring in grid_types<>,
// using an arbitrary macro

#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_HANDLE_TYPES(MAKRO) \
  \
  MAKRO(vertex_handle); \
  MAKRO(edge_handle); \
  MAKRO(face_handle); \
  MAKRO(facet_handle); \
  MAKRO(cell_handle); 

#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_ELEMENT_TYPES(MAKRO) \
  \
  MAKRO(Vertex); \
  MAKRO(Edge); \
  MAKRO(Face); \
  MAKRO(Facet); \
  MAKRO(Cell); 

#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_SEQUENCE_ITERATOR_TYPES(MAKRO) \
  MAKRO(VertexIterator); \
  MAKRO(EdgeIterator); \
  MAKRO(FaceIterator); \
  MAKRO(FacetIterator); \
  MAKRO(CellIterator); 


#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_INCIDENCE_ITERATOR_TYPES(MAKRO) \
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
  MAKRO(FacetOnFaceIterator); \
  MAKRO(CellOnFaceIterator); \
  \
  MAKRO(VertexOnFacetIterator); \
  MAKRO(EdgeOnFacetIterator); \
  MAKRO(FaceOnFacetIterator); \
  MAKRO(CellOnFacetIterator); \
  \
  MAKRO(VertexOnCellIterator); \
  MAKRO(EdgeOnCellIterator); \
  MAKRO(FaceOnCellIterator); \
  MAKRO(FacetOnCellIterator); \
  MAKRO(CellOnCellIterator);  

#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_ARCHETYPE_TYPES(MAKRO) \
  MAKRO(archetype_type); \
  MAKRO(archetype_handle); \
  MAKRO(archetype_iterator); \
  MAKRO(archgt);  


#define GRAL_DEFINE_ENTITY_FOR_ALL_GRID_TYPES(MAKRO) \
  \
  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_HANDLE_TYPES(MAKRO) \
  \
  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_ELEMENT_TYPES(MAKRO) \
  \
  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_SEQUENCE_ITERATOR_TYPES(MAKRO)	\
  \
  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_INCIDENCE_ITERATOR_TYPES(MAKRO)	\
  \
  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_ARCHETYPE_TYPES(MAKRO)


#endif
