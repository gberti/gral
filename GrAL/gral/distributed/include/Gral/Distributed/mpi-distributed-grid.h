#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_H   // -*- c++ -*-
#define NMWR_GB_MPI_DISTRIBUTED_GRID_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include <mpi.h>

#include "Gral/Base/grid-functions.h"
#include "Gral/Distributed/overlapping-grid.h"
#include "Gral/Distributed/connector.h"

/*! \defgroup mpidistributedgrids Distributed Grids using MPI 
    \ingroup  distributedgrids
  
*/

/*! \brief Helper class bundling MPI per-process entities.
    \ingroup mpidistributedgrids
 */
struct mpi_proc {
 MPI_Comm the_communicator;
 int      the_rank;
  mpi_proc(MPI_Comm c, int r) : the_communicator(c), the_rank(r) {}
  mpi_proc() : the_rank(-1)
    { cerr << "mpi_proc::mpi_proc() called!\n"; }
};


/*! \brief Distributed grid class using MPI for communication
    \ingroup mpidistributedgrid
   
 */
template<class CoarseG, class FineG>
class MPIDistributedGrid {
public:
  typedef FineG                            fine_grid_type;
  typedef CoarseG                          coarse_grid_type;
  typedef OverlappingGrid<coarse_grid_type,
                          fine_grid_type>  ovrlp_grid_type;
 
  typedef grid_types<coarse_grid_type> cgt;
  typedef grid_types<  fine_grid_type> fgt;

  typedef typename   cgt::Cell                       CoarseCell;
  typedef typename   cgt::CellIterator               CoarseCellIterator;
  typedef typename   ovrlp_grid_type::overlap_type   overlap_type;
  typedef typename   overlap_type::NbIterator        NeighbourIterator;
  
  typedef typename ovrlp_grid_type::range_type_ref   range_type;
  //typedef ExtendedCellOnCellIterator<coarse_grid_type> NeighbourIterator;
private:
  //----------  DATA -------------
  coarse_grid_type                           the_coarse;
  ovrlp_grid_type                            the_ovrlp_grid;

  // keep it here, so it can be _referenced_
  range_type                                 the_local_range;

  MPI_Comm                                   the_communicator;
  grid_function<CoarseCell,int>              cell2rank;
  vector<CoarseCell>                         rank2cell;
  int                                        my_rank;
  CoarseCell                                 my_cell;
  bool                                       initialized;
  //----------- END DATA ----------

  // FORBIDDEN -- NOT IMPLEMENTED
  typedef MPIDistributedGrid<CoarseG,FineG> self;
  MPIDistributedGrid(self const&); 
  self&  operator=  (self const&);
public:
  //------------------- construction -------------------------------

  MPIDistributedGrid(); 
  MPIDistributedGrid(const coarse_grid_type& cg); 

  void set_coarse_grid(const coarse_grid_type& cg);

  void init();
private:
  void init_unstructured();
  void init_cartesian();

public:

  //-------------------- component access ----------------------------

  coarse_grid_type      & TheCoarseGrid()       { return the_coarse;}
  coarse_grid_type const& TheCoarseGrid() const { return the_coarse;}

  ovrlp_grid_type      &  TheOvrlpGrid()       { return the_ovrlp_grid;}
  ovrlp_grid_type const&  TheOvrlpGrid() const { return the_ovrlp_grid;}

  overlap_type const& TheOverlap() const { return TheOvrlpGrid().TheOverlap();}
  overlap_type      & TheOverlap()       { return TheOvrlpGrid().TheOverlap();}

  fine_grid_type       &  TheGrid()       { return the_ovrlp_grid.TheGrid();}
  fine_grid_type  const&  TheGrid() const { return the_ovrlp_grid.TheGrid();}

  NeighbourIterator FirstNeighbour() const 
    //  { return NeighbourIterator(my_cell);}
    { return TheOverlap().FirstNeighbour();}

  range_type  const&  LocalRange() const  { 
    (range_type&)the_local_range = the_ovrlp_grid.LocalRange();
    return the_local_range;
  } 

  // this cuases problems if some user class forwards LocalRange() as reference,
  // like all other grid data structures!
  //  range_type  LocalRange() const  { return the_ovrlp_grid.LocalRange() ;} 

  CoarseCell MyCell() const { return my_cell;}
  int        MyRank() const { return my_rank;}  

  int        NumOfParts() const { return TheCoarseGrid().NumOfCells();}

  //------------------------ MPI related -----------------------------

  bool isMaster() const { return (my_rank == 0);}

  mpi_proc Proc(const CoarseCell& Nb) const 
    { return mpi_proc(the_communicator,cell2rank(Nb)); }

  MPI_Comm const& TheCommunicator() const { return the_communicator;}

};

/*! \brief Creator function for MPI-based connector for data send.
    \ingroup mpidistributedgrid connectors
 */
template<class SenderIt>
Connector GetSendConnector(SenderIt sb, SenderIt se, unsigned sz, const mpi_proc& Receiver);

/*! \brief Creator function for MPI-based connector for data send.
    \ingroup mpidistributedgrid connectors
 */
template<class Range>
inline Connector GetSendConnector(const Range& R, unsigned sz, const mpi_proc& Receiver)
{ return GetSendConnector(R.begin(),R.end(), sz,Receiver); }

/*! \brief Creator function for MPI-based connector for data receive.
    \ingroup mpidistributedgrid connectors
 */
template<class ReceiverIt>
Connector GetRecvConnector(ReceiverIt rb, ReceiverIt re,  unsigned sz, const mpi_proc& Sender);

/*! \brief Creator function for MPI-based connector for data receive.
    \ingroup mpidistributedgrid connectors
 */
template<class Range>
inline Connector GetRecvConnector(const Range& R,  unsigned sz, const mpi_proc& Sender)
{ return GetRecvConnector(R.begin(),R.end(),sz,Sender); }


/*! \brief Creator function for MPI-based connector for data receive.
    \ingroup mpidistributedgrid connectors
    Data is received and combined with the local data, using op.
    
    Typically, AssignOp is just assigment, or add-assignment (often on shared ranges).
 */
template<class ReceiverIt, class AssignOp>
Connector GetRecvConnector(ReceiverIt rb, ReceiverIt re, unsigned sz,
			   const mpi_proc& Sender, const AssignOp& op);

/*! \brief Creator function for MPI-based connector for data send.
    \ingroup mpidistributedgrid connectors
    Data is received and combined with the local data, using op.
 */
template<class Range, class AssignOp>
inline Connector GetRecvConnector(const Range& R, unsigned sz,
				  const mpi_proc& Sender, const AssignOp& op)
{ return GetRecvConnector(R.begin(),R.end(), sz, Sender,op); }


//r = op(r,s), 
// typically  op is assigment: r = s.
// possible also addition: r += s


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mpi-distributed-grid.C"
#endif


#endif
