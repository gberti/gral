
// $LICENSE

#ifndef NMWR_GB_EDGE2D_INLINE_H
#define NMWR_GB_EDGE2D_INLINE_H

#include "Gral/Grids/Complex2D/internal/edge2d.h"

inline Edge2D::Edge2D(Complex2D const& cc)
    { *this = *(cc.FirstEdge());} 
inline Edge2D::Edge2D(Complex2D const& cc, edge_handle eh)
    { *this = cc.edge(eh);} 

inline VertexOnEdge2D_Iterator Edge2D::FirstVertex() const
{ return VertexOnEdge2D_Iterator(*this); }

inline VertexOnEdge2D_Iterator Edge2D::EndVertex() const
{ return VertexOnEdge2D_Iterator(*this,3); }

inline CellOnEdge2D_Iterator Edge2D::FirstCell() const
{ return CellOnEdge2D_Iterator(*this); }

inline CellOnEdge2D_Iterator Edge2D::EndCell() const
{ return CellOnEdge2D_Iterator(*this,3); }

#endif
