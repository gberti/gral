#ifndef NMWR_GB_COMPOSITE_GRID_FUNCTION_H
#define NMWR_GB_COMPOSITE_GRID_FUNCTION_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <list>  // STL

#include "Gral/Distributed/connector.h"
#include "Gral/Distributed/overlapping-grid-function.h"

namespace GrAL {


/*! \brief Grid function for a composite grid.
    \ingroup compositegrids
   
    \see CompositeGrid
 */
template<class E, class T, class CompositeG>
class composite_grid_function {
  typedef CompositeG                            composite_grid_type;
  typedef typename CompositeG::coarse_grid_type coarse_grid_type;
  typedef typename CompositeG::fine_grid_type   fine_grid_type;
  typedef typename CompositeG::ovrlp_grid_type  ovrlp_grid_type;
  typedef grid_types<coarse_grid_type>          cgt;
  typedef grid_types<fine_grid_type>            fgt;

  typedef typename cgt::Cell                                CoarseCell;  
  //  typedef ExtendedCellOnCellIterator<coarse_grid_type>  CoarseExtdCellOnCellIterator;
  typedef typename composite_grid_type::NeighbourIterator   NeighbourIterator;
  typedef typename cgt::CellIterator                        CoarseCellIterator;

  typedef overlapping_grid_function<E,T,ovrlp_grid_type>    ovrlp_grid_function;
  typedef typename ovrlp_grid_function::overlap_type        overlap_type;

  typedef tp<E>                                             tpE;
private:
  //------- DATA -----------

  const composite_grid_type*                                 the_composite;
  grid_function<CoarseCell, overlapping_grid_function<E,T,ovrlp_grid_type> > local_gfs;

  std::list<Connector> the_senders;
  std::list<Connector> the_receivers; // only for buffered send/receive necessary

public:
  //--------------------------- construction -------------------------------

  composite_grid_function(const composite_grid_type& cg);


  //--------------------------- data access --------------------------------

  ovrlp_grid_function      & operator[](const CoarseCell& C)       { return local_gfs[C];}
  ovrlp_grid_function const& operator()(const CoarseCell& C) const { return local_gfs[C];}

  //--------------------------- component access ----------------------------

  composite_grid_type const& TheCompositeGrid()  const { return *the_composite;}
  coarse_grid_type    const& TheCoarseGrid() const { return the_composite->TheCoarseGrid();} 

  overlap_type const& Overlap(const CoarseCell& C) const { return local_gfs(C).TheOverlap();}
  NeighbourIterator   FirstNeighbour(const CoarseCell& C) const 
    { return TheCompositeGrid().FirstNeighbour(C);}

  grid_function<E,T>  const& GridFunction(const CoarseCell& C) const 
    { return local_gfs(C).TheBaseGridFunction();}
  grid_function<E,T>       & GridFunction(const CoarseCell& C)       
    { return local_gfs[C].TheBaseGridFunction();}

  //--------------------------- data transfer  --------------------------

  void synchronize();

  void begin_synchronize() { synchronize();}
  void end_synchronize() {}

  //  void begin_calculation();
  //  void end_calculation();

};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/composite-grid-function.C"
#endif


#endif
