#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_ADAPTER_H
#define NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_ADAPTER_H


// $LICENSE

#include <list>  // STL

#include "Gral/Base/type-tags.h"

#include "Gral/Distributed/connector.h"
#include "Gral/Distributed/overlapping-grid-function.h"


/*! \brief Grid function adapter for a MPIDistributedGrid
    \ingroup mpidistributedgrids
   
    Makes a distributed grid function from any local grid function 
    type.
    \partof mpidistributedgrids
    \see module gridfunctions
    \see compositegrids
 */
template<class GF, class DistributedG>
class distributed_grid_function_adapter {
  typedef distributed_grid_function_adapter<GF,DistributedG> self;

public:
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

  // typedef overlapping_grid_function<E,T,ovrlp_grid_type>    ovrlp_grid_function;
  typedef GF                                                ovrlp_grid_function;
  typedef typename ovrlp_grid_function::overlap_type        overlap_type;
  typedef typename ovrlp_grid_function::local_grid_function base_grid_function;

  typedef typename  ovrlp_grid_function::element_type   element_type;
  typedef typename  ovrlp_grid_function::value_type     value_type;
  typedef typename  ovrlp_grid_function::element_type   E;
  typedef typename  ovrlp_grid_function::value_type     T;

  typedef typename ovrlp_grid_function::size_type       size_type;
  typedef typename ovrlp_grid_function::const_iterator  const_iterator;

  typedef tp<E>                                             tpE;

private:
  //------- DATA -----------

  const distributed_grid_type*      the_distributed;
  ovrlp_grid_function               the_local_gf;

  std::list<Connector> shared_senders;
  std::list<Connector> shared_receivers; // only for buffered send/receive necessary
  std::list<Connector> exposed_senders;
  std::list<Connector> copied_receivers; // only for buffered send/receive necessary

  bool added_on_shared;
  bool sync_on_shared;
  bool initialized;
public:
  //--------------------------- construction -------------------------------

  distributed_grid_function_adapter();

  // constructors for the value-semantics case 
  distributed_grid_function_adapter(distributed_grid_type const& cg);
  distributed_grid_function_adapter(distributed_grid_type const& cg,
				    T                     const& t);
  // constructor for the reference-semantics case
  distributed_grid_function_adapter(ovrlp_grid_function   const& ogf,
				    distributed_grid_type const& cg);

  distributed_grid_function_adapter(self const& rhs);
  self& operator=(self const& rhs);

private:
  void clear();
  void init();
  void init(self const& rhs);

public:
  void set_grid(const distributed_grid_type& cg);

  void set_added_on_shared(); 
  void set_no_sync_on_shared();

  void do_shared_ranges  ();
  void do_exported_ranges();

  //--------------------------- data access --------------------------------

  T      & operator[](const E& e)       { return the_local_gf[e];}
  T const& operator()(const E& e) const { return the_local_gf(e);}

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

  
  base_grid_function  const& TheGridFunction() const
    { return the_local_gf.TheBaseGridFunction();}
  base_grid_function       & TheGridFunction()
    { return the_local_gf.TheBaseGridFunction();}

  //--------------------------- data transfer  --------------------------

  void synchronize() { begin_synchronize(); end_synchronize(); }

  void begin_synchronize();
  void end_synchronize();

  //  void begin_calculation();
  //  void end_calculation();

};

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mpi-distributed-grid-function-adapter.C"
#endif


#endif
