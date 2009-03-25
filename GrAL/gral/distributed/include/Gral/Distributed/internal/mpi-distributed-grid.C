#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_C
#define NMWR_GB_MPI_DISTRIBUTED_GRID_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/mpi-distributed-grid.h"
#include "Gral/Distributed/grid-to-mpi-graph.h"

#include <vector>
#include "Container/assignment-operators.h"

// #include "IO/iomgr.h"

namespace GrAL {

template<class CoarseG, class FineG>
MPIDistributedGrid<CoarseG,FineG>::MPIDistributedGrid()
 : initialized(false)
{}

template<class CoarseG, class FineG>
MPIDistributedGrid<CoarseG,FineG>::MPIDistributedGrid(CoarseG const& cg)
  : the_coarse(cg), 
    cell2rank(the_coarse),  
    rank2cell(the_coarse.NumOfCells()),
    initialized(false)
{ 
   init(); 
}



template<class CoarseG, class FineG>
void  MPIDistributedGrid<CoarseG,FineG>::set_coarse_grid(CoarseG const& cg)
{
  REQUIRE( (! initialized), "topology changes not possible!\n",1);
  the_coarse = cg;
  coarse_grid_complete();
}

template<class CoarseG, class FineG>
void  MPIDistributedGrid<CoarseG,FineG>::coarse_grid_complete()
{
  cell2rank.set_grid(the_coarse);
  rank2cell = std::vector<CoarseCell>(the_coarse.NumOfCells());

  the_ovrlp_grid.init(TheCoarseGrid());
  init();
}

template<class CoarseG, class FineG>
void  MPIDistributedGrid<CoarseG,FineG>::init_cartesian() {}

template<class CoarseG, class FineG>
void  MPIDistributedGrid<CoarseG,FineG>::init() 
{ 
  // this way it always works. There should be a compile time branch
  // on the type of CoarseG to init_cartesian.
  REQUIRE( (! initialized), "double initialization!\n",1);
  init_unstructured();
  initialized = true;
}

template<class CoarseG, class FineG>
void  MPIDistributedGrid<CoarseG,FineG>::init_unstructured()
{
  coarse_grid_type& CG = TheCoarseGrid();

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size != (int)CG.NumOfCells()) {
    // IOMgr::Error()
      std::cerr << "size " << size 
		<<" != num of cells of coarse grid = " << CG.NumOfCells() << " !\n" ;
    exit(1);
  }


  // create processor topology from coarse grid
  if(CG.NumOfCells() > 1) {
    int ni = mpi_graph_size_of_index_array(CG);
    int* index = new int[ni];
    int ne = mpi_graph_size_of_edges_array(CG);
    int* edges = new int[ne];
    mpi_graph_format(CG,index, index+ni,edges, edges+ne);
    MPI_Graph_create(MPI_COMM_WORLD,
		     CG.NumOfCells(),index,edges, 0,  &the_communicator);
    
    delete [] index;
    delete [] edges;
  }
  else
   MPI_Comm_dup(MPI_COMM_WORLD, &the_communicator);

  // create mapping cells <-> ranks
  int r = 0;
  for(CoarseCellIterator C = CG.FirstCell(); ! C.IsDone(); ++C, ++r) {
    cell2rank[*C] = r;
    rank2cell[r] = *C;
  }  
  my_rank = 0;
  MPI_Comm_rank(the_communicator,&my_rank);
  my_cell = rank2cell[my_rank];

  the_local_range = the_ovrlp_grid.LocalRange();

  MPI_Barrier(the_communicator);

  if(isMaster())
    // IOMgr::Info()
      std::cerr << "initialized processor grid ( "
		<< CG.NumOfCells() << " processes)\n";
}




template<class SenderIt>
class mpi_send_connector : public connector_impl {
  typedef typename SenderIt::value_type T;
private:
  //------ DATA --------
  SenderIt  src_b, src_e;
  mpi_proc  p;
  T*        buffer;
  int       sz;
  int       tag;


  MPI_Request curr_request;
  MPI_Status  curr_stat;
  //----- END DATA ------

  typedef mpi_send_connector<SenderIt> self;
  mpi_send_connector(self const&) { ENSURE(false, "mpi_send_connector: copy ctor called!\n",1);}
public:
  mpi_send_connector(SenderIt sb, SenderIt se, unsigned n, const mpi_proc& P) 
    : src_b(sb), src_e(se), p(P), sz(n), tag(0)
    { 
      buffer = new T[sz];
    }

  ~mpi_send_connector() { delete [] buffer;}
  virtual connector_impl* clone() const { return new mpi_send_connector<SenderIt>(*this);}

  void send_data_begin() 
    {
      my_copy(src_b,src_e,buffer);
      MPI_Isend(buffer, sz*sizeof(T),MPI_BYTE, 
		p.the_rank, ++tag, p.the_communicator, &curr_request);
    }
  void send_data_end() {
    MPI_Wait(&curr_request,&curr_stat); 
  }
  void send_data()     { send_data_begin(); send_data_end();}
  void recv_data() {}
};


template<class SenderIt>
Connector GetSendConnector(SenderIt sb, SenderIt se, unsigned sz, const mpi_proc& Receiver)
{
  return Connector(new mpi_send_connector<SenderIt>(sb, se, sz, Receiver));
}


template<class ReceiverIt,class AssignOp>
class mpi_receive_connector : public connector_impl {
  typedef typename ReceiverIt::value_type T;
private:
  ReceiverIt  dest_b, dest_e;
  mpi_proc    p;
  T*          buffer;
  int         sz;

  MPI_Request curr_request;
  MPI_Status  curr_stat;

  AssignOp    assign_op;

  typedef mpi_receive_connector<ReceiverIt,AssignOp> self;
  mpi_receive_connector(self const& ) { ENSURE(false, "mpi_receive_connector: copy ctor called!\n",1);}
public:
  mpi_receive_connector(ReceiverIt sb, ReceiverIt se, unsigned n,
			const mpi_proc& P, const AssignOp& op) 
    : dest_b(sb), dest_e(se), p(P), sz(n), assign_op(op)
    { 
      //      sz = dest_e - dest_b;
      buffer = new T[sz];
    }

  ~mpi_receive_connector() { delete [] buffer;}
  virtual connector_impl* clone() const { return new mpi_receive_connector<ReceiverIt,AssignOp>(*this);}

  void recv_data_begin() 
    {
       MPI_Irecv(buffer, sz*sizeof(T),MPI_BYTE, 
		 p.the_rank, MPI_ANY_TAG, p.the_communicator, &curr_request);
    }

  void recv_data_end() 
    { 
      MPI_Wait(&curr_request,&curr_stat); 
      my_copy_op(buffer,buffer+sz,dest_b, assign_op);
    }

  void recv_data() { recv_data_begin(); recv_data_end();}
  void send_data() {}
};






template<class ReceiverIt>
Connector GetRecvConnector(ReceiverIt rb, ReceiverIt re, unsigned sz, const mpi_proc& Sender)
{ 
  typedef typename ReceiverIt::value_type T;
  return Connector(new mpi_receive_connector<ReceiverIt,assignment_operators::assign>
		   (rb,re,sz,Sender, assignment_operators::assign())); 
}

template<class ReceiverIt, class AssignOp>
Connector GetRecvConnector(ReceiverIt rb, ReceiverIt re, unsigned sz,
			      const mpi_proc& Sender, const AssignOp& op)
{ 
  return Connector(new mpi_receive_connector<ReceiverIt,AssignOp>(rb,re,sz,Sender, op)); 
}

/*
template<class CG1, class FG1, class CG2, class FG2>
void ConstructDistributed(MPIDistributedGrid<CG1,FG1>     &  MpiG,         // out
			  CG2                        const&  src_coarse,   // in
			  FG2                        const&  src_fine,     // in
			  CellCorr                        &  src2dest_c)   // out
{

}
*/

} // namespace GrAL 

#endif

