#ifndef NMWR_GB_OVERLAPPING_GRID_H
#define NMWR_GB_OVERLAPPING_GRID_H 

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Distributed/overlap.h"

//----------------------------------------------------------------
//
// template<class CoarseGrid, class FineGrid>
// class overlapping_grid;
//
// A overlapping grid augments a base grid with the notion of
// overlap ranges that capture its relationship to other parts
// of a (conceptual) global grid.
// However, a overlapping grid has no idea of the context it is
// contained in, i.e. a really distributed environment with message
// passing or just a part of a composite grid.
//
//
//----------------------------------------------------------------


//template<class FineGrid, class overlap_type>

template<class CoarseGrid, class FineGrid>
class OverlappingGrid  {
public:
  typedef OverlappingGrid<CoarseGrid,FineGrid> self;
  MAKE_DEFAULT_OPS_SELF
  typedef FineGrid    fine_grid_type;
  typedef CoarseGrid  coarse_grid_type;
  //typedef grid_types<coarse_grid_type> cgt;
  typedef grid_types<fine_grid_type> fgt;

  // typedef typename cgt::Cell           CoarseCell;
  typedef overlap<coarse_grid_type, fine_grid_type> overlap_type;
  //typedef overlap<fine_grid_type> overlap_type;
  typedef typename overlap_type::range_type_ref     range_type_ref;

  //-------- owned DATA ---------------
private:

  fine_grid_type     the_fine_grid;
  overlap_type       the_overlap;

  //CoarseCell         myCell; // position in coarse grid

  //-------- referenced DATA ----------
  
  const coarse_grid_type* the_coarse_grid;

public:
  //----------------------   construction   ----------------------
  
  OverlappingGrid() : the_coarse_grid(0) {} 

  void set_coarse_grid(const coarse_grid_type& cg)
    { the_coarse_grid = &cg;}

  void init_overlap() {
    the_overlap.set_coarse_grid(*the_coarse_grid);
    the_overlap.set_fine_grid(the_fine_grid);
  }

  //---------------------- component access ----------------------

  coarse_grid_type const& TheCoarseGrid() const { return *the_coarse_grid;}

  //  fine_grid_type const& TheBaseGrid() const { return the_fine_grid;}
  //  fine_grid_type&       TheBaseGrid()       { return the_fine_grid;}
  fine_grid_type const& TheGrid() const { return the_fine_grid;}
  fine_grid_type&       TheGrid()       { return the_fine_grid;}


  overlap_type const& TheOverlap() const { return the_overlap;}
  overlap_type&       TheOverlap()       { return the_overlap;}

  //range_type_ref   TheRange()   const { return the_overlap.local_range();}
  //range_type_ref   OwnedRange() const { return the_overlap.local();}  
  range_type_ref   LocalRange() const { return the_overlap.local_range();}  
};

#endif
