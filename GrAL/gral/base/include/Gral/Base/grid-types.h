#ifndef GRAL_BASE_GB_GRID_TYPES_H
#define GRAL_BASE_GB_GRID_TYPES_H


// $LICENSE

//----------------------------------------------------------------
/*! \brief Empty traits class for parameterized namespaces associated 
    with a grid.
    \ingroup traits

   The specializations for a concrete grid type provide access to
   grid related types like
    - Vertex, Edge, ....
    - VertexIterator, ....
    - vertex_handle, ...

 \b Example:
 \code
 typedef grid_types<Complex2D> gt;
 for(gt::VertexIterator v = G.FirstVertex(); ...)
    do_something(*v);
 \endcode
*/
//----------------------------------------------------------------


template<class Grid>
struct grid_types {};


#endif
