#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_H
#define NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include <list>  // STL

#include "Gral/Base/type-tags.h"

#include "Gral/Distributed/connector.h"
#include "Gral/Distributed/overlapping-grid-function.h"


/*! \brief Grid function for a MPIDistributedGrid
    \ingroup mpidistributedgrids
   
    \partof mpidistributedgrids
    \see module gridfunctions
    \see compositegrids
 */
template<class E, class T, class DistributedG>
class distributed_grid_function {
  typedef DistributedG                            distributed_grid_type;
  typedef DistributedG                            grid_type;

  typedef typename DistributedG::coarse_grid_type coarse_grid_type;
  typedef typename DistributedG::fine_grid_type   fine_grid_type;
  typedef typename DistributedG::ovrlp_grid_type  ovrlp_grid_type;
  typedef grid_types<coarse_grid_type>            cgt;
  typedef grid_types<fine_grid_type>              fgt;

  typedef typename cgt::Cell                                CoarseCell;  
  //  typedef ExtendedCellOnCellIterator<coarse_grid_type>  CoarseExtdCellOnCellIterator;
  typedef typename distributed_grid_type::NeighbourIterator   NeighbourIterator;
  typedef typename cgt::CellIterator                        CoarseCellIterator;

  typedef overlapping_grid_function<E,T,ovrlp_grid_type>    ovrlp_grid_function;
  typedef typename ovrlp_grid_function::overlap_type        overlap_type;

  typedef tp<E>                                             tpE;

public:
  typedef E   element_type;
  typedef T   value_type;
  typedef     typename ovrlp_grid_function::size_type       size_type;
  typedef     typename ovrlp_grid_function::const_iterator  const_iterator;
private:
  //------- DATA -----------

  const distributed_grid_type*                                 the_distributed;
  overlapping_grid_function<E,T,ovrlp_grid_type>               the_local_gf;

  list<Connector> shared_senders;
  list<Connector> shared_receivers; // only for buffered send/receive necessary
  list<Connector> exposed_senders;
  list<Connector> copied_receivers; // only for buffered send/receive necessary

  bool added_on_shared;
  bool sync_on_shared;
public:
  //--------------------------- construction -------------------------------
  distributed_grid_function() : added_on_shared(false), sync_on_shared(false) {}
  distributed_grid_function(const distributed_grid_type& cg);
  distributed_grid_function(const distributed_grid_type& cg, const T& t);

  void set_grid(const distributed_grid_type& cg);

  void set_added_on_shared(); 
  void set_no_sync_on_shared();

  void do_shared_ranges();
  void do_exported_ranges();

  //--------------------------- data access --------------------------------

  T      & operator[](const E& e)       { return the_local_gf[e];}
  T const& operator()(const E& e) const { return the_local_gf[e];}

  //--------------------------- container ----------------------------------
 
  size_type size() const { return the_local_gf.size();}
  const_iterator begin() const { return the_local_gf.begin();}
  const_iterator end  () const { return the_local_gf.end  ();}
  
  //--------------------------- component access ----------------------------

  distributed_grid_type const& TheDistributedGrid()  const { return *the_distributed;}
  coarse_grid_type      const& TheCoarseGrid()       const { return the_distributed->TheCoarseGrid();} 

  overlap_type const& TheOverlap() const { return the_local_gf.TheOverlap();}
  NeighbourIterator   FirstNeighbour() const 
    { return TheDistributedGrid().FirstNeighbour();}

  grid_function<E,T>  const& TheGridFunction() const
    { return the_local_gf.TheBaseGridFunction();}
  grid_function<E,T>       & TheGridFunction()
    { return the_local_gf.TheBaseGridFunction();}

  //--------------------------- data transfer  --------------------------

  void synchronize() { begin_synchronize(); end_synchronize(); }

  void begin_synchronize();
  void end_synchronize();

  //  void begin_calculation();
  //  void end_calculation();

};

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mpi-distributed-grid-function.C"
#endif


#endif
