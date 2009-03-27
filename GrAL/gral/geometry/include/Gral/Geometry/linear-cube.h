#ifndef GRAL_GB_GEOMETRY_LINEAR_CUBE_H
#define GRAL_GB_GEOMETRY_LINEAR_CUBE_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/CartesianND/all.h"

#include "Geometry/algebraic-primitives.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"


#include "Container/index-map-nd.h"
#include "Container/combinatorial.h"

#include "Utility/ref-ptr.h"

#include <vector>

namespace GrAL {


template<class COORD>
class midpoint_cubature_cube {
public:
  typedef COORD coord_type;
  typedef point_traits<coord_type> pt;
  typedef typename pt::component_type scalar_type;
 
  enum { dim = pt::dimension};
private:
  /*static*/ scalar_type weight_[1];
  /*static*/ coord_type  point_ [1];
public:
  midpoint_cubature_cube() 
  {
    weight_[0] = 1.0;
    point_ [0] = coord_type(0.5);
  }

  unsigned size() const { return 1;}
  coord_type   point (int i) const { cv(i); return point_[i];}
  scalar_type  weight(int i) const { cv(i); return weight_[i];}

  bool valid(int i) const { return (0 <= i && i < (int)size());}
  void cv   (int i) const { REQUIRE(valid(i), "  i=" << i, 1);}
};


template<class COORD>
class uniform_cubature_cube {
public:
  typedef COORD coord_type;
  typedef point_traits<coord_type> pt;
  typedef typename pt::component_type scalar_type;
 
  enum { dim = pt::dimension};
private:
  std::vector<scalar_type> weight_;
  std::vector<coord_type>  point_;
public:
  uniform_cubature_cube(int n = 1) 
  { init(n);}

  void init(int n)
  {
    typedef cartesiannd::grid<dim>     cart_grid_type;
    typedef grid_types<cart_grid_type> gt;
    typedef typename gt::index_type    index_type;
    typedef matrix<dim,dim,0> matrix_type;
    typedef affine_mapping<matrix_type, coord_type>  mapping_type;
    cart_grid_type  G(index_type(0), index_type(n+1));
    cartesiannd::mapped_geometry<cart_grid_type, mapping_type> GeomG(G, mapping_type::identity());
 
    scalar_type w = 1.0/G.NumOfCells();
    weight_ = std::vector<scalar_type>(G.NumOfCells(),w);
    point_  = std::vector<coord_type> (G.NumOfCells());
    int ccnt=0;
    for(typename gt::CellIterator c(G); !c.IsDone(); ++c, ++ccnt)
      point_[ccnt] = GeomG.center(*c);
  }

  unsigned size() const { return point_.size();}
  coord_type   point (int i) const { cv(i); return point_[i];}
  scalar_type  weight(int i) const { cv(i); return weight_[i];}

  bool valid(int i) const { return (0 <= i && i < (int)size());}
  void cv   (int i) const { REQUIRE(valid(i), "  i=" << i, 1);}
};



/*! \brief Linear cube finite element
 
   \ingroup interpolation   
   \see test-linear-cube.C 
*/
template<class GEOM, class F, class GT = grid_types<typename GEOM::grid_type> >
class linear_cube_interpolator {
  typedef linear_cube_interpolator<GEOM,F,GT> self;
public:
  typedef GEOM geom_type;
  typedef F    function_type;
  typedef typename GT::grid_type             grid_type;
  typedef typename geom_type::coord_type     coord_type;
  typedef typename function_type::value_type result_type;
  typedef algebraic_primitives<coord_type>   ap;
  typedef point_traits<coord_type>           pt;
  typedef typename pt::component_type        scalar_type;
  typedef typename ap::matrix_type           matrix_type;

  // should use archetype_geom here, then it works also
  // for surface meshes (except linear solve must be least-squares solve)
  typedef typename geom_type::coord_type     local_coord_type;
  typedef point_traits<local_coord_type>     ptl;

  typedef typename GT::Vertex  Vertex;
  typedef typename GT::Cell    Cell;
  typedef typename GT::VertexOnCellIterator VertexOnCellIterator;
  // could be template parameter
  //  typedef midpoint_cubature_cube<local_coord_type> cubature_type;
  typedef uniform_cubature_cube<local_coord_type> cubature_type;
private:

  ref_ptr<geom_type const>     the_geom;
  ref_ptr<function_type const> f;
  Cell                         c;
  static cubature_type         cubature_rule;

  std::vector<scalar_type>  jacdet_cub;
  std::vector<coord_type>   global_cub;
public:
  enum { dim = grid_type::dim};
  enum { two_power_dim = combinatorial::compile_time_functions::power<2,dim>::value};

  typedef index_map_nd<dim>                   index_map_type;
  typedef typename index_map_type::index_type index_type;

  static index_map_type  cart_index_map;
  static std::vector<int> cart2mesh;
  static std::vector<int> mesh2cart;
  //  static int cart2mesh[two_power_dim];
  //  static int mesh2cart[two_power_dim];

  static void init() { 
    cart_index_map = index_map_type(index_type(0), index_type(2)); 
    cart2mesh = std::vector<int>(two_power_dim);
    mesh2cart = std::vector<int>(two_power_dim);
    // this could be done by archetype mapping
    for(int i = 0; i < two_power_dim; ++i)
      cart2mesh[i] = mesh2cart[i] = i;
  }
  static int        index2mesh(index_type idx) { 
    return cart2mesh[cart_index_map(idx)];
}
  static index_type mesh2index(int v)          { cv(v); return cart_index_map(mesh2cart[v]);}

  static bool valid(int v) { return (0 <= v && v < (int)mesh2cart.size());}
  static void cv   (int v) { REQUIRE(valid(v), " v=" << v, 1); }

  static void print(std::ostream& out) 
  {
    out << "cart_index_map: ["
	<< cart_index_map.min_tuple() << "," << cart_index_map.beyond_tuple() << "]" << std::endl;
    out << "cart2mesh: ";
    for(unsigned i = 0; i < cart2mesh.size(); ++i)
      out << cart2mesh[i] << std::endl;
    out << "mesh2cart: ";
    for(unsigned i = 0; i < mesh2cart.size(); ++i)
      out << mesh2cart[i] << std::endl;
  }

public:
  linear_cube_interpolator() {}
  linear_cube_interpolator(geom_type     const& geo,
			   function_type const& ff,
			   Cell cc)
    : the_geom(geo), f(ff), c(cc) 
  { REQUIRE_ALWAYS(cc.valid()," cc=" << cc.handle() << "," << & (cc.TheGrid()),1); }

  void init_integration() {
    jacdet_cub = std::vector<scalar_type>(cubature_rule.size());
    global_cub = std::vector<coord_type> (cubature_rule.size());
    for(int i = 0; i < (int)cubature_rule.size(); ++i) {
      jacdet_cub[i] = jacdet(cubature_rule.point(i));
      global_cub[i] = global(cubature_rule.point(i));
    }
  }
  bool cub_initialized() const { return (jacdet_cub.size() == cubature_rule.size());}

  result_type operator()(coord_type X) const { return eval_local(local(X)); }

  result_type eval_local(local_coord_type x) const {
    result_type res(0.0); 
    //    for(int  v= 0; v < c.NumOfVertices(); ++v) {
    int vcnt = 0;
    for(VertexOnCellIterator v(c); !v.IsDone(); ++v, ++vcnt) {
      result_type prod = (*f)(*v);
      index_type idx = mesh2index(vcnt);
      for(int i = 0; i < dim; ++i)
	prod *= (1.0 - idx[i]) + (2*idx[i]-1.0)*x[i+ptl::LowerIndex(x)];
      res += prod;
    }
    return res;

  }

  bool is_inside_local(local_coord_type x) const {
    bool res = true;
    for(int i = 0; i < dim; ++i) {
      res = res && (0 <= x[i+ptl::LowerIndex(x)] && x[i+ptl::LowerIndex(x)] <= 1);
    }
    return res;
  }

  local_coord_type local(coord_type X) const {
    local_coord_type res;
    matrix_type L2G;
    coord_type X0 = XV(index2mesh(index_type(0)));
    for(int i = 0; i < dim; ++i) {
      index_type ei(0); // = ap::unit_vector(i);
      ei[i] = 1;
      L2G[i+pt::LowerIndex(X0)] = XV(index2mesh(ei)) - X0;
    }
    ap::solve(L2G,res,X-X0);
    scalar_type dist = ap::distance(X, global(res));
    scalar_type eps = 0.00001 * ap::distance(X0, XV(index2mesh(index_type(1))));
    while(dist > eps) {
      coord_type delta;
      ap::solve(jacobian(res), delta, global(res)-X);
      res = res - delta;
      dist = ap::distance(X, global(res));
    }
    return res;
  }

  coord_type global(local_coord_type x) const {
    coord_type res(0.0);
    int vcnt = 0;
    for(VertexOnCellIterator v(c); !v.IsDone(); ++v, ++vcnt) {
       coord_type prod = coord(*v);
       index_type idx = mesh2index(vcnt);
       for(int i = 0; i < dim; ++i)
	 prod *= (1.0-idx[i]) + (2*idx[i]-1)*x[i+ptl::LowerIndex(x)];
       res += prod;
    }
    return res;
  }
  coord_type cubature_global_coord(int i) const { REQUIRE(cub_initialized(), "", 1); return global_cub[i];}


  Vertex      V(int v)       const { 
    return TheCell()->V(v);
  }
  coord_type XV(int v)       const { 
    return TheGeometry()->coord(V(v));
  }
  coord_type coord(Vertex v) const { return TheGeometry()->coord(v);}

  ref_ptr<Cell const>      TheCell()     const { return ref_ptr<Cell const>(c);}
  ref_ptr<geom_type const> TheGeometry() const { return the_geom;}
  static ref_ptr<cubature_type>       TheCubature()       { return ref_ptr<cubature_type >     (cubature_rule);}


  /*! \brief Jacobian  of the shape function (local-to-global mapping) 
   */
  matrix_type jacobian(local_coord_type x) const 
  {
    matrix_type res(0.0);
    for(int j = 0; j < dim; ++j) {
      int vcnt = 0;
      for(VertexOnCellIterator v(c); !v.IsDone(); ++v, ++vcnt) {
 	coord_type prod = coord(*v);
	index_type idx = mesh2index(vcnt);
	for(int i = 0; i < dim; ++i) 
	  if(i != j)
	    prod *= (1.0-idx[i]) + (2*idx[i]-1) * x[i+ptl::LowerIndex(x)];
	  else
	    prod *= (2.0*idx[i]-1);
	res[j+pt::LowerIndex(prod)] += prod;
      }
    }
    return res;
  }

  /*! \brief Determinant of the Jacobian
   */
  scalar_type jacdet(local_coord_type x) const { return ap::det(jacobian(x));}
  scalar_type jacdet(int i)              const { REQUIRE(cub_initialized(), "", 1); return jacdet_cub[i];}

  /*! \brief Integrate a function defined in global coordinates over the simplex
      
  */
  template<class GLOBF>
  typename GLOBF::result_type integrate_global(GLOBF const& f) const 
  {
    if(! cub_initialized())
      const_cast<self *>(this)->init_integration();

    typename GLOBF::result_type res(0.0);
    for(int i = 0; i < (int)cubature_rule.size(); ++i)
      res += f(cubature_global_coord(i)) * jacdet(i) * cubature_rule.weight(i);
    return res;
  }

}; // class linear_cube_interpolator


template<class GEOM, class F, class GT>
typename linear_cube_interpolator<GEOM,F,GT>::index_map_type
linear_cube_interpolator<GEOM,F,GT>::cart_index_map;

/*
template<class GEOM, class F, class GT>
int 
linear_cube_interpolator<GEOM,F,GT>::cart2mesh[linear_cube_interpolator<GEOM,F,GT>::two_power_dim];

template<class GEOM, class F, class GT>
int 
linear_cube_interpolator<GEOM,F,GT>::mesh2cart[linear_cube_interpolator<GEOM,F,GT>::two_power_dim];
*/
template<class GEOM, class F, class GT>
std::vector<int> 
linear_cube_interpolator<GEOM,F,GT>::cart2mesh;

template<class GEOM, class F, class GT>
std::vector<int> 
linear_cube_interpolator<GEOM,F,GT>::mesh2cart;


template<class GEOM, class F, class GT>
typename linear_cube_interpolator<GEOM,F,GT>::cubature_type
linear_cube_interpolator<GEOM,F,GT>::cubature_rule;

} // namespace GrAL 

#endif
