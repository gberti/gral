#ifndef NMWR_GB_SPECIALIZATION_FOR_PARTIAL_GRIDFUNCTION_REG2D_H
#define NMWR_GB_SPECIALIZATION_FOR_PARTIAL_GRIDFUNCTION_REG2D_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#define SPECIALIZATION_PARTIAL_GRIDFUNCTION_REGGRID2D(E,T)\
class partial_grid_function<RegGrid2D::##E,T> \
  : public partial_grid_function_reg2d<RegGrid2D::##E,T> { \
public: \
  partial_grid_function() {} \
  partial_grid_function(const Grid& gg) \
   : partial_grid_function_reg2d<RegGrid2D::##E,T>(gg) {} \
  partial_grid_function(const Grid& gg, const T& t) \
   : partial_grid_function_reg2d<RegGrid2D::##E,T>(gg,t) {} \
}; 


#endif
