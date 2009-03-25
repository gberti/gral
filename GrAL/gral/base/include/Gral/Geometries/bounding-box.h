#ifndef GRAL_GB_BASE_GEOMETRIES_BOUNDING_BOX_H
#define GRAL_GB_BASE_GEOMETRIES_BOUNDING_BOX_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/box.h"

namespace GrAL {

/*! \defgroup gridboundingbox  Bounding Boxes of Grids

    \brief Routines for calculating bounding boxes of grids

    One might be tempted to think that bounding boxes of grids
    can be calculated just by taking the bounding boxes of
    their sequence of vertex coordinate. While this is done
    in the functions below, it is neither correct nor the most
    efficient way in every case:
    
    - if the geometric dimension is the same as the combinatoric
      dimension, it suffices to calculate the bounding box of the
      boundary
    - if the geometry is curved, looking at vertex coordinates
      does not suffice.

   \ingroup geometricgridtools

   \todo routines should check at compile time whether geometries are
   linear.
   \see Module geometricgridtools
*/


/*! Get the bounding box of the range of a grid vertex iterator
 
    \ingroup gridboundingbox
    \see Module gridboundingbox
 */
template<class VERTEXITER, class GEOM>
inline
box<typename GEOM::coord_type>
get_grid_iter_bounding_box(VERTEXITER V, GEOM const& Geom)
{
  box<typename GEOM::coord_type> b(Geom.coord(*V));
  for(; ! V.IsDone(); ++V)
    b |= Geom.coord(*V);
  return b;
}

/*! \brief convenience wrapper for get_grid_iter_bounding_box

    \ingroup gridboundingbox

    \todo Specialize to the case that there is a boundary representation
    of the grid available. (The bounding box of a grid is that of the
    grid's boundary.)
    \see Module gridboundingbox
 */
template<class GRID, class GEOM>
box<typename GEOM::coord_type>
inline
get_grid_bounding_box(GRID const& G, GEOM const& Geom)
{
  return get_grid_iter_bounding_box(G.FirstVertex(), Geom);
} 

/*! \brief Get bounding box of a cell

    \ingroup gridboundingbox

    Convenience wrapper for get_grid_iter_bounding_box.

    \note This works only if the bounding box of a cell is the bounding
    box of its vertices, i.e. for linear or d-linear geometries. 
  
    \see Module gridboundingbox
 */
template<class CELL, class GEOM>
box<typename GEOM::coord_type>
inline
get_cell_bounding_box(CELL const& C, GEOM const& Geom)
{
  return get_grid_iter_bounding_box(C.FirstVertex(), Geom);
} 

} // namespace GrAL 

#endif
