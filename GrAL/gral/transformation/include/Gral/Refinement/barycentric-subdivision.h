#ifndef GRAL_GB_BASE_BARYCENTRIC_SUBDIVISION_H
#define GRAL_GB_BASE_BARYCENTRIC_SUBDIVISION_H



/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/combinatorial.h"
#include "Container/range.h"

#include "Gral/Transformation/pattern-data.h"

#include "Gral/Grids/Triang2D/all.h"
#include "Gral/Grids/Triang3D/all.h"

#include <iostream>
#include <vector>
#include <algorithm>


namespace GrAL {


  namespace barycentric_subdivision_detail {

    template<unsigned D>
    struct triangulation_grid {};
    
    template<>
    struct triangulation_grid<2> { 
      typedef Triang2D                 grid_type;
      typedef stored_geometry_triang2d geom_type;
    };
    
    template<>
    struct triangulation_grid<3> {
      typedef Triang3D                 grid_type;
      typedef stored_geometry_triang3d geom_type;
    };

  } // barycentric_subdivision_detail



  /*! \brief  D-dimensional barycentric subdivision of a simplex

  */
  template<unsigned D>
  class barycentric_subdivision {
    enum { Dim = D,
	   num_cells    = GrAL::compile_time_functions::factorial<D+1>::value,
	   num_vertices = GrAL::compile_time_functions::power<2,D+1>::value - 1,
	   conn_size = (D+1)*num_cells
    };
    typedef barycentric_subdivision_detail::triangulation_grid<D> get_pattern_grid;
    typedef typename get_pattern_grid::grid_type triang_grid_type;
    typedef typename get_pattern_grid::geom_type triang_geom_type;
    typedef grid_types<triang_grid_type>     gt;
    typedef typename triang_geom_type::coord_type  coord_type;
    typedef typename gt::Vertex              Vertex;
    typedef typename gt::Cell                Cell;

  private:

    // valid range: [0, Dim]
    class corner_index_t {
      unsigned i;
    public:
      corner_index_t() : i(-1) {}
      explicit corner_index_t(unsigned ii) : i(ii) { REQUIRE( 0 <= i  && i <= D, "i=" << i, 1); }
      operator unsigned() const { return i;}
    };
    typedef std::vector<corner_index_t> corner_index_set;

    /*! \brief Identifier for a vertex of a barycentric subdivision.

    Each subset of the corners uniquely corresponds to one vertex.
    A single corner \c c  corresponds to vertex_index_type \f$ 2^c \f$.
    A set \f$ \{ c_i \} \f$ corresponds to \f$ \sum_i 2^{c_i}\f$.
 
    Thus, the valid range for vertex_index_t is \f$ [1, 2^{D+1}-1] \f$.
    */
    class vertex_index_t {
      unsigned v;
    public:
      vertex_index_t() : v(-1) {}
      explicit vertex_index_t(unsigned vv) : v(vv) { cv();  }
      explicit vertex_index_t(corner_index_t   c) : v( (1 << c) -1) { cv();}
      explicit vertex_index_t(corner_index_set c) 
      {
	REQUIRE(! c.empty(), "", 1);
	v = 0;
	for(unsigned i=0; i < c.size(); ++i)
	  v += (1 << c[i]);
	--v;
	cv();
      }
      void cv() const { REQUIRE( 0 <= v && v < num_vertices, "v=" << v, 1); }
      operator unsigned() const { cv(); return v;}

      /*! \brief number of corners contributing to this vertex
          
          Equivalent to get_corners().size().
      */
      unsigned  num_corners() const;

      /*! \brief Set of corners contributing to this vertex
      */
      corner_index_set get_corners() const;
    };

    // Vertex            vertex (vertex_index_t v) const { return Vertex(*the_grid, v);}
    static corner_index_set corners(vertex_index_t v)  { return v.get_corners();}

  public:
    typedef corner_index_t corner_index_type;
    typedef vertex_index_t vertex_index_type;



    template<class CFG>
    struct pattern_data_type : public transformation_pattern_data<CFG>
    {
    private:
      typedef pattern_data_type<CFG>           self;
      typedef transformation_pattern_data<CFG> base;
      typedef barycentric_subdivision<Dim>     subdivision_type;
    public:
      typedef typename base::Vertex Vertex;
      typedef typename base::vertex_range_type vertex_range_type;

      grid_function<Vertex, int>  trans2vertex_index;
      Vertex corner2orig          [Dim+1];

      // corners contributing to v 
      vertex_range_type corners(Vertex v) const
      {
	corner_index_set local_corners = subdivision_type::corners(vertex_index_type(trans2vertex_index(v)));
	vertex_range_type res(this->orig_patch);
	for(unsigned c = 0; c < local_corners.size(); ++c)
	  res.push_back(corner2orig[local_corners[c]]);
	return res;
      }

      template<class CELL>
      static self trivial_pattern_data(CELL  hc) {
	self res;
	static_cast<base&>(res) = base::trivial_pattern_data(hc);
	// TODO: add corner2orig / trans2vertex_index
	return res;
      }
    }; // struct pattern_data_type

    template<class CFG>
    static void init_pattern(pattern_data_type<CFG> & p);

  private:
    typedef std::vector<int>    permutation_type;
    static permutation_type identical_permutation();

    //! \f$ \sum_k=0^i 2^{p_i} \f$
    static vertex_index_t vertex_index(permutation_type const& p, int i); 

    //    typedef transformation_pattern_data_cfg<grid_type, geom_type> cfg;
    
    struct static_data {
      int        conn  [conn_size];
      coord_type coords[num_vertices];

      int        corner_conn  [Dim+1];
      coord_type corner_coords[Dim+1];

      std::vector<std::vector<int> > vertices_by_dim;
      std::vector<int>               non_corner_vertices;  
    };

    static static_data * sd;

    static void init();
  public:


    barycentric_subdivision() { init();}
  
    //! Return the unique corner to which the cell  \c c is incident
    corner_index_type  corner(int cell) const {
      REQUIRE(0 <= cell && cell < num_cells, "cell=" << cell, 1);
      return corner_index_type(cell*(Dim+1)/NumOfCells());
    }

  }; // barycentric_subdivision


  template<unsigned D>
  inline
  typename barycentric_subdivision<D>::corner_index_set
  barycentric_subdivision<D>::vertex_index_t::get_corners() const 
  {
    cv();
    corner_index_set res;
    unsigned c = 0;
    unsigned i = v+1;
    while(i) {
      if(i & 0x1u)
	res.push_back(corner_index_t(c));
      ++c;
      i >>= 1;
    }
    return res;
  }


  template<unsigned D>
  inline unsigned
  barycentric_subdivision<D>::vertex_index_t::num_corners() const 
  {
    cv();
    int res = 0; 
    int vv = v+1;
    for(unsigned k = 0; k <= Dim; ++k) 
      res += ( (vv & (1 << k)) >> k); 
    return res;
  }



  template<unsigned D>
  inline
  typename barycentric_subdivision<D>::permutation_type
  barycentric_subdivision<D>::identical_permutation()
  {
    permutation_type p(Dim+1);
    for(int i = 0; i <= Dim; ++i)
      p[i] = i;
    return p;
  }

  template<unsigned D>
  inline 
  typename barycentric_subdivision<D>::vertex_index_t
  barycentric_subdivision<D>::vertex_index(typename barycentric_subdivision<D>::permutation_type const& p, int i) 
  {
    REQUIRE(0 <= i && i <= Dim, "i=" << i << " ", 1);
    unsigned res = 0;
    for(int k = 0; k <= i; ++k)
      res += (1 << p[k]);
    return vertex_index_type(res-1);
  }

  

  template<unsigned D>
  void barycentric_subdivision<D>::init()
  {
    static int count = 0;
    if(count > 0) return;
    ++count;
    sd = new static_data();  

    // set up data for orig patch (a single unit simplex)
    typedef algebraic_primitives<coord_type> ap;

    sd->corner_conn  [0] = 0;
    sd->corner_coords[0] = coord_type(0.0);
    for(int c = 1; c <= Dim; ++c) {
      sd->corner_conn  [c] = c;
      sd->corner_coords[c] = ap::unit_vector(c-1 + ap::LowerIndex()); 
    }

    // set up data for barycentric subdivision of unit simplex
    permutation_type p = identical_permutation();
    int cell = 0;
    do {
      for(int i = 0; i <= Dim; ++i) {
	sd->conn[cell + i] = vertex_index(p,i);
      }
      cell += (Dim+1);
    } while(std::next_permutation(p.begin(), p.end()));



    for(unsigned v = 0; v < num_vertices; ++v) {
      vertex_index_type vi(v);
      corner_index_set corners_v = corners(vi);
      coord_type sum(0.0);
      for(unsigned c = 0; c < corners_v.size(); ++c)
	sum += sd->corner_coords[corners_v[c]];
      sd->coords[v] = sum/corners_v.size();
    }
  

    sd->vertices_by_dim.resize(Dim+1);
    for(unsigned v = 0; v < num_vertices; ++v) {
      vertex_index_type vi(v);
      sd->vertices_by_dim[vi.num_corners()-1].push_back(vi);
    }
    for(unsigned d = 1; d < sd->vertices_by_dim.size(); ++d) {
      for(unsigned i = 0; i < sd->vertices_by_dim[d].size(); ++i) {
	sd->non_corner_vertices.push_back(sd->vertices_by_dim[d][i]);
      }
    }
  }
  

  template<unsigned D>
  template<class CFG>
  void barycentric_subdivision<D>::init_pattern(typename barycentric_subdivision<D>::pattern_data_type<CFG> & p)
  {
    init();

    typedef pattern_data_type<CFG> pattern_type;
    typedef typename pattern_type::grid_type pattern_grid_type;
    typedef grid_types<pattern_grid_type>    pgt;

    triang_grid_type origT(sd->corner_conn, 1, Dim+1);
    triang_geom_type origGeomT(origT,&(sd->corner_coords[0][0]));
    triang_grid_type T(sd->conn, num_cells, num_vertices);
    triang_geom_type GeomT(T, &(sd->coords[0][0]));

    vertex_morphism<triang_grid_type, pattern_grid_type> Phi_orig (origT, p.orig_patch);
    vertex_morphism<triang_grid_type, pattern_grid_type> Phi_trans(T,     p.transformed_patch);
    ConstructGridV(p.orig_patch,        p.orig_geom,        origT, origGeomT, Phi_orig);
    ConstructGridV(p.transformed_patch, p.transformed_geom, T,     GeomT,     Phi_trans); 

    p.removed_vertices.set_grid(p.orig_patch);
    p.removed_vertex  .init    (p.orig_patch, false);
    p.new_vertices    .set_grid(p.transformed_patch);
    p.new_vertex      .init    (p.transformed_patch, false);

    for(unsigned d = 1; d < sd->vertices_by_dim.size(); ++d) {
      for(unsigned i = 0; i < sd->vertices_by_dim[d].size(); ++i) {
	Vertex v(T, sd->vertices_by_dim[d][i]);
	p.new_vertices.push_back(Phi_trans(v));
	p.new_vertex[Phi_trans(v)] = true;
      }
    }
    p.local.set_grid(p.transformed_patch);
    for(typename pgt::VertexIterator v(p.transformed_patch); !v.IsDone(); ++v)
      p.local[v] = typename pattern_type::local_coord_type(* p.orig_patch.FirstCell(), 
							   p.transformed_geom.coord(*v));

    // set up mapping of orig corners to transformed patch 
    p.transformed2orig.set_grids(p.transformed_patch,
				 p.orig_patch);
    for(unsigned i = 0; i < sd->vertices_by_dim[0].size(); ++i) {
      p.transformed2orig[Phi_trans(Vertex(T,sd->vertices_by_dim[0][i]))] = Phi_orig(Vertex(origT, i));
    }


    // these are additions for barycentric_subdivision 
    for(int c = 0; c <= Dim; ++c)
      p.corner2orig[c] = Phi_orig(Vertex(origT, c));

    p.trans2vertex_index.set_grid(p.transformed_patch);
    for(int v = 0; v < num_vertices; ++v)
      p.trans2vertex_index[Phi_trans(Vertex(T, v))] = v;

    // here we could also map cells to the unique incident corner
  }

  template<unsigned D>
  typename  barycentric_subdivision<D>::static_data * barycentric_subdivision<D>::sd;


} // namespace GrAL 

#endif
