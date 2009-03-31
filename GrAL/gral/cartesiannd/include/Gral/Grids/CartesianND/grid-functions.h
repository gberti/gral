#ifndef GRAL_GB_CARTESIANND_GRID_FUNCTIONS_H
#define GRAL_GB_CARTESIANND_GRID_FUNCTIONS_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Gral/Base/grid-function-vector.h"


namespace GrAL {

/*!  \defgroup cartesianndgf Total grid functions for CartesianND
 
      \ingroup cartesianndmodule

      A partial specialization of the primary \c grid_function template is used.
      \see $GrAL TotalGridFunction
      \see Test in \ref test-cartesiannd-gf.C
 */

/*! \brief Specialization for elements of \c cartesiannd::grid<DIM>
    \ingroup cartesianndgf
 */
template<unsigned DIM, unsigned K, class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM> ,K>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,K>, T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,K>, T > base;
public:
  typedef cartesiannd::grid<DIM>                                                               grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};

} // namespace GrAL 

#endif
