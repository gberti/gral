#ifndef GRAL_BASE_GB_GRID_BOUNDARY_H
#define GRAL_BASE_GB_GRID_BOUNDARY_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

//----------------------------------------------------------------
/*! \brief  A reference to the combinatorial boundary of a grid.

   This must be specialized for concrete grid types:
   \code
   template<class Grid>
   class BoundaryRange {
     typedef VertexIterator;
     typedef EdgeIterator;
     typedef FacetIterator;

     VertexIterator FirstVertex() const;
     EdgeIterator   FirstEdge()   const;
     FacetIterator  FirstFacet()  const;

     BoundaryRange(const Grid&);
   };
   \endcode
*/
//----------------------------------------------------------------

template<class Grid>
class BoundaryRange {};

/*! \brief creator function for BoundaryRange
  \relates BoundaryRange
 */
template<class Grid>
inline
BoundaryRange<Grid> GetBoundaryRange(Grid const& G)
{ return BoundaryRange<Grid>(G); }

} // namespace GrAL 

#endif
