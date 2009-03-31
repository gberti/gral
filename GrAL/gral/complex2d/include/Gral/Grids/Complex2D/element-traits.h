#ifndef GRAL_GB_COMPLEX2D_ELEMENT_TRAITS_H
#define GRAL_GB_COMPLEX2D_ELEMENT_TRAITS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/element-traits.h"
#include "Gral/Grids/Complex2D/complex2d.h"

/* specializations of element_traits for element
   types of Complex2D
*/

namespace GrAL {

template<class E> 
struct hasher_for_complex2d {};

template<>  
struct hasher_for_complex2d<Edge2D> 
  : public hash_edge2d {};


template<>  
struct element_traits<Vertex2D> 
 : public element_traits_vertex_base<Complex2D> 
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>         consecutive_tag;
};


template<>
struct element_traits<Edge2D> 
  : public element_traits_edge_base<Complex2D> 
{
  typedef hasher_for_complex2d<element_type> hasher_type;
};


template<>
struct element_traits<Cell2D>
  : public element_traits_cell_base<Complex2D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>         consecutive_tag;
};

} // namespace GrAL 

#endif
