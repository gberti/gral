#ifndef NMWR_GB_COMPLEX2D_ELEMENT_TRAITS_H
#define NMWR_GB_COMPLEX2D_ELEMENT_TRAITS_H

// $LICENSE

#include "Gral/Base/element-traits.h"
#include "Gral/Grids/Complex2D/complex2d.h"

/* specializations of element_traits for element
   types of Complex2D
*/

template<class E> 
struct hasher_for_complex2d {};



struct hasher_for_complex2d<Vertex2D> 
  : public hash_vertex2d {};
struct hasher_for_complex2d<Edge2D> 
  : public hash_edge2d {};
struct hasher_for_complex2d<Cell2D> 
  : public hash_cell2d {};


struct element_traits<Vertex2D> 
 : public element_traits_vertex_base<Complex2D> 
{
  typedef hasher_for_complex2d<element_type> hasher_type;
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
  typedef hasher_for_complex2d<element_type> hasher_type;
  typedef consecutive_integer_tag<0>         consecutive_tag;
};


#endif
