#ifndef NMWR_GB_REG2D_ELEMENT_TRAITS_H
#define NMWR_GB_REG2D_ELEMENT_TRAITS_H

// $LICENSE 

#include "Gral/Base/element-traits.h"
#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"

namespace GrAL {

/*! \defgroup elementtraitsreggrid2d element traits for RegGrid2D
    \ingroup cartesian2dmodule
    
    Specialization of the template element_traits for the
    element types of RegGrid2D
 */



/*! \ingroup elementtraitsreggrid2d
 */
template<>
struct element_traits<cartesian2d::RegGrid2D::Vertex> 
: public element_traits_vertex_base<cartesian2d::RegGrid2D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>     consecutive_tag;
  static  cartesian2d::RegGrid2D::Vertex  handle2element(grid_type const& g, handle_type h)
  { return (g.vertex(h));}
};


/*! \ingroup elementtraitsreggrid2d
 */
template<>
struct element_traits<cartesian2d::RegGrid2D::Edge>
  : public element_traits_edge_base<cartesian2d::RegGrid2D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>     consecutive_tag;
  static  cartesian2d::RegGrid2D::Edge  handle2element(grid_type const& g, handle_type h)
  { return (g.edge(h));}

};


/*! \ingroup elementtraitsreggrid2d
 */
template<>
struct element_traits<cartesian2d::RegGrid2D::Cell> 
  : public element_traits_cell_base<cartesian2d::RegGrid2D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>     consecutive_tag;
  static  cartesian2d::RegGrid2D::Cell  handle2element(grid_type const& g, handle_type h)
  { return (g.cell(h));}
};

} // namespace GrAL 

#endif
