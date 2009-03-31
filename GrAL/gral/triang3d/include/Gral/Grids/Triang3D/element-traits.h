#ifndef GRAL_GB_GRIDS_TRIANG3D_ELEMENT_TRAITS_H
#define GRAL_GB_GRIDS_TRIANG3D_ELEMENT_TRAITS_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Triang3D/triang3d.h"

/*! \file
 */


namespace GrAL {
 
template<>
struct element_traits<Triang3D_Vertex> 
  : public element_traits_vertex_base<Triang3D> 
{ 
  typedef element_traits_vertex_base<Triang3D> base;
  struct hasher_type : public base::hasher_type_elem_base {
    size_t operator()(Triang3D_Vertex const& v) const { return v.handle();}
  };
  // vertices are numbered consecutively 
  typedef consecutive_integer_tag<0> consecutive_tag;
};

template<>
struct element_traits<Triang3D::Edge>
  : public element_traits_edge_base<Triang3D>
{
  typedef Triang3D::Edge::edge_hasher_type edge_hasher_type;
  struct hasher_type :  public hasher_type_elem_base,
                        public edge_hasher_type
  {
    typedef Triang3D::Edge key_type;
    typedef Triang3D::Edge argument_type;
    // shouldn't this work??
    // using Triang3D::Edge::edge_hasher_type::operator();
    size_t operator()(key_type const& k) const {
      return Triang3D::Edge::edge_hasher_type::operator()(k);
    }
  };
};


template<>
struct element_traits<Triang3D::Facet>
  : public element_traits_facet_base<Triang3D>
{
  typedef Triang3D::Facet::facet_hasher_type facet_hasher_type;
  struct hasher_type : public hasher_type_elem_base,
                       public facet_hasher_type
  {
    typedef Triang3D::Facet key_type;
    typedef Triang3D::Facet argument_type;

    //??  using Triang3D::Facet::facet_hasher_type::operator();
    size_t operator()(key_type const& k) const {
      return Triang3D::Facet::facet_hasher_type::operator()(k);
    }
  };
};

template<>
struct element_traits<Triang3D_Cell> 
  : public element_traits_cell_base<Triang3D> 
{
  typedef element_traits_cell_base<Triang3D>  base;
  struct hasher_type : public base::hasher_type_elem_base {
    size_t operator()(Triang3D_Cell const& c) const { return c.handle();}
  };
  // cells are numbered consecutively
  typedef consecutive_integer_tag<0> consecutive_tag;
};


/*
template<>
struct element_traits<Triang3D::Edge> 
  : public element_traits_edge_base<Triang3D> 
{
  struct hasher_type : public grid_types_base_Triang3D, 
		       public hasher_type_base<Triang3D, Triang3D_Cell> {
    enum { p= 37};
    size_t operator()(Triang3D::Edge const& e) const { 
      vertex_handle v1 = e.V1().handle();
      vertex_handle v2 = e.V2().handle();
      return (v1 > v2 ? p*v1 + (v2%p) : p*v2 + (v1%p));
    }
  };

};
*/

} // namespace GrAL 


#endif


