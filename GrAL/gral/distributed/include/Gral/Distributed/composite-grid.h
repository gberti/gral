#ifndef NMWR_GB_COMPOSITE_GRID_H
#define NMWR_GB_COMPOSITE_GRID_H

// $LICENSE



#include "Gral/Base/common-grid-basics.h"
#include "Gral/Distributed/overlapping-grid.h"

/*! \defgroup compositegrids Composite Grids
    \ingroup distributedgrids
    
*/


/*! \brief A grid composed of separate pieces
    \ingroup compositegrids
    
    An object of type CompositeGrid represents a  (global) grid
    which is partitioned into several parts.
    These parts form the cells of a coarse (quotient) grid.
    To each of this parts is associated an 
    OverlappingGrid. The union of these overlapping grids
    (with proper identification of the overlap) is
    isomorphic to the underlying global grid.
    
   \see composite_grid_function,
   \partof compositegrids
 */
template<class CoarseG, class FineG>
class CompositeGrid {
public:
  typedef FineG                            fine_grid_type;
  typedef CoarseG                          coarse_grid_type;
  typedef OverlappingGrid<coarse_grid_type,
                          fine_grid_type>  ovrlp_grid_type;
 
  typedef grid_types<coarse_grid_type> cgt;
  typedef grid_types<  fine_grid_type> fgt;

  typedef typename   cgt::Cell                         CoarseCell;
  typedef typename   ovrlp_grid_type::overlap_type     overlap_type;
  typedef typename   overlap_type::CellNbIterator      CellNbIterator;
  typedef typename   overlap_type::VertexNbIterator    VertexNbIterator;
  typedef typename  overlap_type::NbIterator           NeighbourIterator;

  //  typedef ExtendedCellOnCellIterator<coarse_grid_type> NeighbourIterator;
  typedef typename ovrlp_grid_type::range_type_ref   range_type;
private:
  coarse_grid_type                           the_coarse;
  grid_function<CoarseCell,ovrlp_grid_type>  local_grids;

public:
  //------------------- construction -------------------------------

  CompositeGrid() {}

  void update() { local_grids.set_grid(TheCoarseGrid());}

  //-------------------- component access ----------------------------

  coarse_grid_type      & TheCoarseGrid()       { return the_coarse;}
  coarse_grid_type const& TheCoarseGrid() const { return the_coarse;}

  ovrlp_grid_type      &  OvrlpGrid(const CoarseCell& c)       { return local_grids[c];}
  ovrlp_grid_type const&  OvrlpGrid(const CoarseCell& c) const { return local_grids(c);}

  fine_grid_type       &  Grid(const CoarseCell& c)       { return local_grids[c].TheGrid();}
  fine_grid_type  const&  Grid(const CoarseCell& c) const { return local_grids[c].TheGrid();}

  overlap_type const& Overlap(const CoarseCell& c) const { return OvrlpGrid(c).TheOverlap();}
  overlap_type      & Overlap(const CoarseCell& c)       { return OvrlpGrid(c).TheOverlap();}

  CellNbIterator FirstCellNeighbour(const CoarseCell& c) const 
    { return Overlap(c).FirstCellNeighbour();}
  VertexNbIterator FirstVertexNeighbour(const CoarseCell& c) const 
    { return Overlap(c).FirstVertexNeighbour();}

  NeighbourIterator  FirstNeighbour(const CoarseCell& c) const
    //{ return NeighbourIterator(c);}
    {return Overlap(c).FirstNeighbour();}
  
  range_type    LocalRange(const CoarseCell& c)   { return local_grids[c].LocalRange();}


};


template<class CoarseG, class FineG>
void check_composite_grid(const CompositeGrid<CoarseG,FineG>& CG);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/composite-grid.C"
#endif

#endif
