#ifndef NMWR_GB_SPECIALIZATION_FOR_GRIDFUNCTION_REGGRID2D_H
#define NMWR_GB_SPECIALIZATION_FOR_GRIDFUNCTION_REGGRID2D_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


///////////////////////////////////////////////////////////
//
//  @author Guntram Berti, 1997
//
//
//  Work around lack of partial specialization by
//  total specialization. In theory, we could do 
//  partial specialization to the element-type E 
//  (first parameter of grid function template)
//
//  Usage:
//  SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Cell,  double);
//  SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Vertex,int);
//
///////////////////////////////////////////////////////////

#define SPECIALIZATION_GRIDFUNCTION_REGGRID2D(E,T)\
class grid_function<RegGrid2D::##E,T > \
  : public grid_function_reg2d<RegGrid2D::##E,T > { \
public: \
  grid_function() {} \
  grid_function(const Grid& gg) \
  : grid_function_reg2d<RegGrid2D::##E,T >(gg) {} \
  grid_function(const Grid& gg,size_type n) \
  : grid_function_reg2d<RegGrid2D::##E,T >(gg,n) {} \
}; 

SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Vertex,int);
SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Edge,  int);
SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Cell,  int);

#if !defined (__xlC__)
SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Vertex,bool);
SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Edge,  bool);
SPECIALIZATION_GRIDFUNCTION_REGGRID2D(Cell,  bool);
#endif

#endif
