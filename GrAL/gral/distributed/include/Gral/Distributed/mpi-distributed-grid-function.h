#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_H
#define NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <list>  // STL

#include "Gral/Base/type-tags.h"

#include "Gral/Distributed/connector.h"
#include "Gral/Distributed/overlapping-grid-function.h"
#include "Gral/Distributed/mpi-distributed-grid-function-adapter.h"

namespace GrAL {

/*! \brief Grid function for a MPIDistributedGrid
    \ingroup mpidistributedgrids
   
    \partof mpidistributedgrids
    \see module gridfunctions
    \see compositegrids
 */

template<class E1, class T1, class DistributedG>
class distributed_grid_function 
  : public distributed_grid_function_adapter
           <
            overlapping_grid_function<E1,T1, typename DistributedG::ovrlp_grid_type>,
            DistributedG 
           >
{
private:
  typedef distributed_grid_function<E1,T1,DistributedG>  self;
  typedef distributed_grid_function_adapter
  <
    overlapping_grid_function<E1,T1, typename DistributedG::ovrlp_grid_type>,
    DistributedG
  >        base;
public:
  typedef typename base::distributed_grid_type distributed_grid_type;
  distributed_grid_function() {}
  distributed_grid_function(distributed_grid_type const& dg) : base(dg) {}
  distributed_grid_function(distributed_grid_type const& dg,
			    T1                    const& t ) : base(dg,t) {}

  distributed_grid_function(self const& rhs) : base(rhs) {}
  self& operator=(self const& rhs) { 
    if (this != &rhs) { base::operator=(rhs);} 
    return *this;
  }
 
};

} // namespace GrAL 

#endif
