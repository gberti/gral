#ifndef GRAL_GB_COMPLEX2D_EDGE2D_INLINE_H
#define GRAL_GB_COMPLEX2D_EDGE2D_INLINE_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex2D/internal/edge2d.h"

namespace GrAL {

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

} // namespace GrAL {

#endif
