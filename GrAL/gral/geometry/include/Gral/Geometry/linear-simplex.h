#ifndef GRAL_GB_GEOMETRY_LINEAR_SIMPLEX_H
#define GRAL_GB_GEOMETRY_LINEAR_SIMPLEX_H

// $LICENSE_NEC_2003

#include "Gral/Base/common-grid-basics.h"
#include "Geometry/algebraic-primitives.h"
#include "Utility/ref-ptr.h"


// for simplices
template<class GEOM, class F, class GT = grid_types<typename GEOM::grid_type> >
class linear_simplex_interpolator {
public:
  typedef GEOM geom_type;
  typedef F    function_type;
  typedef typename geom_type::coord_type     coord_type;
  // should use archetype_geom here, then it works also
  // for surface meshes (except linear solve must be least-squares solve)
  typedef typename geom_type::coord_type     local_coord_type;
  typedef typename function_type::value_type result_type;
  typedef algebraic_primitives<coord_type>   ap;
  typedef point_traits<coord_type>           pt;

  typedef typename GT::Vertex  Vertex;
  typedef typename GT::Cell    Cell;
private:

  ref_ptr<geom_type const>     the_geom;
  ref_ptr<function_type const> f;
  Cell                         c;
public:
  linear_simplex_interpolator() {}
  linear_simplex_interpolator(geom_type const& geo,
			      function_type const& ff,
			      Cell cc)
    : the_geom(geo), f(ff), c(cc) {}


  result_type operator()(coord_type X) const {
    local_coord_type x = local(X);
    result_type res = (*f)(V(0)) * (1.0 - ap::dot(x, local_coord_type(1.0)));
    for(int v = 1; v < (int)c.NumOfVertices(); ++v)
      res +=  (*f)(V(v)) * x[v-1+pt::LowerIndex(x)];
    return res;
  }

  local_coord_type local(coord_type X) const {
    local_coord_type x;
    typename ap::matrix_type L2G;
    for(int v = 1; v < (int)c.NumOfVertices(); ++v)
      L2G[v-1+pt::LowerIndex(X)] = XV(v) - XV(0);
    ap::solve(L2G,x,X-XV(0));
    return x;
  }

  Vertex      V(int v) const { return TheCell()->V(v);}
  coord_type XV(int v) const { return TheGeometry()->coord(V(v));}

  ref_ptr<Cell const>      TheCell()     const { return ref_ptr<Cell const>(c);}
  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}
};



#endif
