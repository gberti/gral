#ifndef GRAL_BASE_GB_GRID_BOUNDARY_H
#define GRAL_BASE_GB_GRID_BOUNDARY_H



// $LICENSE

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


#endif
