#ifndef NMWR_GB_OVERLAPPING_GRID_FUNCTION_H
#define NMWR_GB_OVERLAPPING_GRID_FUNCTION_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <vector>

#include "Config/compiler-config.h"
#include "Gral/Base/grid-functions.h"

#include "Gral/Distributed/overlapping-grid-function-adapter.h"

namespace GrAL {

//----------------------------------------------------------------
/*! \brief A grid function that knows which parts are owned by other parts.
    \ingroup overlappinggrids
    \partof  overlappinggrids

    \templateparams
    - E element type
    - T value type
    - DGrid: overlapping grid, for example overlapping_grid

   \see module gridfunctions
 */
//----------------------------------------------------------------


template<class E1, class T1, class DGrid>
class overlapping_grid_function 
  : public  overlapping_grid_function_adapter<grid_function<E1,T1>, DGrid> 
{
  
  typedef overlapping_grid_function<E1,T1,DGrid>                          self;
  typedef overlapping_grid_function_adapter<grid_function<E1,T1>, DGrid>  base;
  
public:
  typedef typename base::ovrlp_grid_type ovrlp_grid_type;
  // typedef typename base::ovrlp_grid_type grid_type;
  typedef T1 value_type;

  overlapping_grid_function() {}
  overlapping_grid_function(const ovrlp_grid_type& og) 
    : base(og) {}
  overlapping_grid_function(const ovrlp_grid_type& og, const value_type& t) 
    : base(og,t) {}

  overlapping_grid_function(self const& rhs) : base(rhs) {}
  self& operator=(self const& rhs) { 
    if (this != &rhs) { base::operator=(rhs); } 
    return *this;
  }

};

} // namespace GrAL 

#endif
