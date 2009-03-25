#ifndef GRAL_GB_GEOMETRY_LINEAR_SIMPLEX_H
#define GRAL_GB_GEOMETRY_LINEAR_SIMPLEX_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

#include "Gral/Base/common-grid-basics.h"
#include "Geometry/algebraic-primitives.h"
#include "Container/combinatorial.h"
#include "Utility/ref-ptr.h"

namespace GrAL {

template<class COORD>
class midpoint_cubature_simplex {
public:
  typedef COORD coord_type;
  typedef point_traits<coord_type> pt;
  typedef typename pt::component_type scalar_type;
 
  enum { dim = pt::dimension};
private:
  /*static*/ scalar_type weight_[1];
  /*static*/ coord_type  point_ [1];
public:
  midpoint_cubature_simplex() 
  {
    weight_[0] = 1.0/combinatorial::factorial((int)dim);
    point_ [0] = coord_type(1.0/(dim+1.0));
  }

  unsigned size() const { return 1;}
  coord_type   point (int i) const { cv(i); return point_[i];}
  scalar_type  weight(int i) const { cv(i); return weight_[i];}

  bool valid(int i) const { return (0 <= i && i < (int)size());}
  void cv   (int i) const { REQUIRE(valid(i), "  i=" << i, 1);}
};


/*! \brief Linear simplex finite element
    
   \ingroup interpolation   
   \see test-linear-simplex.C 
*/
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
  typedef typename pt::component_type        scalar_type;

  typedef typename GT::Vertex  Vertex;
  typedef typename GT::Cell    Cell;

  typedef midpoint_cubature_simplex<local_coord_type> cubature_type;
private:

  ref_ptr<geom_type const>     the_geom;
  ref_ptr<function_type const> f;
  Cell                         c;
  /* static */ cubature_type   cubature_rule;
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
  coord_type global(local_coord_type x) const {
    coord_type res = XV(0);
    for(int v = 1; v < (int)TheCell()->NumOfVertices(); ++v)
      res += (XV(v)-XV(0))*x[v-1+pt::LowerIndex(x)];
    return res;
  }
  coord_type cubature_global_coord(int i) const { return global(cubature_rule.point(i));}


  Vertex      V(int v) const { return TheCell()->V(v);}
  coord_type XV(int v) const { return TheGeometry()->coord(V(v));}

  ref_ptr<Cell const>      TheCell()     const { return ref_ptr<Cell const>(c);}
  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}
  ref_ptr<cubature_type const> TheCubature() const { return ref_ptr<cubature_type const>(cubature_rule);}


  /*! \brief Jacobian  of the shape function (local-to-global mapping) 
   */
  scalar_type jacobian(local_coord_type) const 
  {
    typename ap::matrix_type L2G;
    for(int v = 1; v < (int)c.NumOfVertices(); ++v)
      L2G[v-1+pt::LowerIndex()] = XV(v) - XV(0);
    return ap::det(L2G);
  }

  /*! \brief Integrate a function defined in global coordinates over the simplex
      
  */
  template<class GLOBF>
  typename GLOBF::result_type integrate_global(GLOBF const& f) const 
  {
    typename GLOBF::result_type res = 0.0;
    for(int i = 0; i < (int)cubature_rule.size(); ++i)
      res += f(cubature_global_coord(i)) * jacobian(cubature_rule.point(i)) * cubature_rule.weight(i);
    return res;
  }

};

} // namespace GrAL 

#endif
