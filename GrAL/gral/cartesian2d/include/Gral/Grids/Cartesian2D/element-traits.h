#ifndef NMWR_GB_REG2D_ELEMENT_TRAITS_H
#define NMWR_GB_REG2D_ELEMENT_TRAITS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/element-traits.h"
#include "Grids/Reg2D/cartesian-grid2d.h"

//----------------------------------------------------------------
//
//  traits classes for enabling uniform treatment
//  of different element types (vertex, edge etc)
//  for example in the template class grid_function<E,T>.
//
//----------------------------------------------------------------


template<class E> 
struct hasher_for_reg2d {
  int operator()(const E& e) const { return e.GlobalNumber();}
};


template<>
struct element_traits<RegGrid2D::Vertex> 
: public element_traits_vertex_base<RegGrid2D>
{
  typedef hasher_for_reg2d<element_type> hasher_type;
};


template<>
struct element_traits<RegGrid2D::Edge>
  : public element_traits_edge2d_base<RegGrid2D>
{
  typedef hasher_for_reg2d<element_type> hasher_type;
};


template<>
struct element_traits<RegGrid2D::Cell> 
  : public element_traits_cell2d_base<RegGrid2D>
{
  typedef hasher_for_reg2d<element_type> hasher_type;
};


#endif
