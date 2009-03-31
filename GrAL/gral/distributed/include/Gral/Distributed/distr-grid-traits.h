#ifndef NMWR_GB_DISTR_GRID_TRAITS_H
#define NMWR_GB_DISTR_GRID_TRAITS_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

/*! Traits class for distributed grids.
   \ingroup distributedgrids

   Specializations must define:
   \code
    typedef  range_type;
    typedef  base_grid_type;
    
    static   range_type     const& LocalRange(DG const&);
    static   base_grid_type const& BaseGrid  (DG const&);
   \endcode
 */
template<class DG>
struct distr_grid_traits {};

/*! General template for distributed grid functions, to be specialized.
  \ingroup distributedgrids
 */
template<class E, class T, class DG>
class distr_grid_function {};

} // namespace GrAL 

#endif
