#ifndef NMWR_GB_REG2D_ELEMENT_TRAITS_H
#define NMWR_GB_REG2D_ELEMENT_TRAITS_H

// $LICENSE 

#include "Gral/Base/element-traits.h"
#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"

/*! \defgroup elementtraitsreggrid2d element traits for RegGrid2D

    Specialization of the template element_traits for the
    element types of RegGrid2D
 */


/*! \ingroup elementtraitsreggrid2d
 */
template<class E> 
struct hasher_for_reg2d {
  int operator()(const E& e) const { return e.GlobalNumber();}
};


/*! \ingroup elementtraitsreggrid2d
 */
template<>
struct element_traits<RegGrid2D::Vertex> 
: public element_traits_vertex_base<RegGrid2D>
{
  typedef hasher_for_reg2d<element_type> hasher_type;
  typedef consecutive_integer_tag<0>     consecutive_tag;
  static  RegGrid2D::Vertex  handle2element(grid_type const& g, handle_type h)
  { return (g.vertex(h));}
};


/*! \ingroup elementtraitsreggrid2d
 */
template<>
struct element_traits<RegGrid2D::Edge>
  : public element_traits_edge_base<RegGrid2D>
{
  typedef hasher_for_reg2d<element_type> hasher_type;
  typedef consecutive_integer_tag<0>     consecutive_tag;
  static  RegGrid2D::Edge  handle2element(grid_type const& g, handle_type h)
  { return (g.edge(h));}

};


/*! \ingroup elementtraitsreggrid2d
 */
template<>
struct element_traits<RegGrid2D::Cell> 
  : public element_traits_cell_base<RegGrid2D>
{
  typedef hasher_for_reg2d<element_type> hasher_type;
  typedef consecutive_integer_tag<0>     consecutive_tag;
  static  RegGrid2D::Cell  handle2element(grid_type const& g, handle_type h)
  { return (g.cell(h));}
};


#endif
