#ifndef NMWR_GB_DISTR_GRID_TRAITS_H
#define NMWR_GB_DISTR_GRID_TRAITS_H


// $LICENSE

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
