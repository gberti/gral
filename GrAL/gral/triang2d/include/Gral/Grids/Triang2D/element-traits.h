#ifndef GRAL_GB_GRIDS_TRIANG2D_ELEMENT_TRAITS_H
#define GRAL_GB_GRIDS_TRIANG2D_ELEMENT_TRAITS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Gral/Grids/Triang2D/triang2d.h"

namespace std {
  template<class T> struct hash;
  

  
  
 
  
} // namespace std
 
template<>
struct element_traits<Triang2D_Vertex> 
  : public element_traits_vertex_base<Triang2D> 
{ 
  struct hasher_type {
    size_t operator()(Triang2D_Vertex const& v) const { return v.handle();}
  };
  typedef consecutive_integer_tag<0> consecutive_tag;
};

template<>
struct element_traits<Triang2D_Cell> 
  : public element_traits_cell_base<Triang2D> 
{ 
  struct hasher_type {
    size_t operator()(Triang2D_Cell const& c) const { return c.handle();}
  };
  typedef consecutive_integer_tag<0> consecutive_tag;
};



template<>
struct element_traits<Triang2D::Edge> 
  : public element_traits_edge_base<Triang2D> 
{
  struct hasher_type : public grid_types_base_Triang2D {
    enum { p= 37};
    size_t operator()(Triang2D::Edge const& e) const { 
      vertex_handle v1 = e.V1().handle();
      vertex_handle v2 = e.V2().handle();
      return (v1 > v2 ? p*v1 + (v2%p) : p*v2 + (v1%p));
    }
  };

};


#endif


