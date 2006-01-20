#ifndef GRAL_GB_BASE_INTERNAL_GET_RANGE_OPERATIONS_H
#define GRAL_GB_BASE_INTERNAL_GET_RANGE_OPERATIONS_H

// $LICENSE_NEC_2006

#include "Gral/Base/type-tags.h"
#include "Gral/Base/internal/compile-time-reflection.h"
#include "Gral/Base/internal/iterator-type-maps.h"

namespace GrAL { namespace grid_types_detail {



  // define template functions NumOf, FirstElement, EndElement in grid_types


#define DEFINE_MIXIN_RANGE_OPS(E,S)		\
  template<class GT, int HAS_E> \
  struct mixin_range_ops_aux_##E : public GT \
  { \
    using GT::NumOf; \
    using GT::FirstElement; \
    using GT::EndElement; \
    template<class A> static int NumOf(A const& a, tp<typename GT::E>)   { return a.NumOf##S ();}  \
    template<class A> static typename GrAL::iterator<GT, typename GT::E, A>::type \
    FirstElement(A const& a, tp<typename GT::E>) { return a.First##E();} \
    template<class A> static typename GrAL::iterator<GT, typename GT::E, A>::type \
    EndElement  (A const& a, tp<typename GT::E>) { return a.End##E  ();} \
  }; \
  template<class GT>  \
  struct mixin_range_ops_aux_##E <GT,0> : public GT {};\
  \
  template<class GT> struct mixin_range_ops_##E : public mixin_range_ops_aux_##E <GT, has_##E<GT>::result > {};

  
  DEFINE_MIXIN_RANGE_OPS(Vertex,Vertices);
  DEFINE_MIXIN_RANGE_OPS(Edge,  Edges);
  DEFINE_MIXIN_RANGE_OPS(Face,  Faces);
  DEFINE_MIXIN_RANGE_OPS(Facet, Facets);
  DEFINE_MIXIN_RANGE_OPS(Cell,  Cells);


#undef DEFINE_MIXIN_RANGE_OPS

  template<class GT>
  struct mixin_range_ops_0d : public mixin_range_ops_Vertex<GT> {};
  template<class GT>
  struct mixin_range_ops_1d : public  mixin_range_ops_Edge<mixin_range_ops_0d<GT> > {};
  template<class GT>
  struct mixin_range_ops_2d : public  mixin_range_ops_Face<mixin_range_ops_1d<GT> > {};
  template<class GT>
  struct mixin_range_ops_3d : public  mixin_range_ops_Cell<mixin_range_ops_2d<GT> > {};

  template<class GT, int D>
  struct mixin_range_ops_aux       : public mixin_range_ops_3d<GT> {};
  template<class GT>
  struct mixin_range_ops_aux<GT,0> : public mixin_range_ops_0d<GT> {};
  template<class GT>
  struct mixin_range_ops_aux<GT,1> : public mixin_range_ops_1d<GT> {};
  template<class GT>
  struct mixin_range_ops_aux<GT,2> : public mixin_range_ops_2d<GT> {};

  template<class GT>
  struct mixin_range_ops : public mixin_range_ops_aux<GT, GT::dimension_tag::dim> {};



}} // namespace GrAL { namespace grid_types_detail


#endif
