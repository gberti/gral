#ifndef GRAL_GB_GRAL_GEOMETRY_BUCKETING_H
#define GRAL_GB_GRAL_GEOMETRY_BUCKETING_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include "Utility/ref-ptr.h"
#include "Container/sequence-algorithms.h"

#include "Gral/Grids/CartesianND/all.h"

#include <vector>

namespace GrAL {


  template<class T, int N>
  class bucket_table {
  public:
    typedef std::vector<T>                                             value_type;

    typedef cartesiannd::grid<N>                                       grid_type;
    typedef grid_types<grid_type>                                      gt;
    typedef matrix<N, N, 0>                                            matrix_type;
    typedef typename cartesiannd::default_coord<grid_type>::type       coord_type;
    typedef affine_mapping<matrix_type, coord_type>                    mapping_type;
    typedef cartesiannd::mapped_geometry<grid_type, mapping_type>      geom_type;
                                                                       
    typedef typename grid_type::index_type                             index_type;
    typedef typename gt::Cell                                          Cell;
    typedef typename gt::cartesian_subrange_type                       cart_subrange_type;

    typedef partial_grid_function<Cell, value_type>                    table_type;
  private:
    ref_ptr<grid_type>                                   g;
    ref_ptr<geom_type>                                   geom;
    ref_ptr<table_type>                                  b;

    coord_type m_cell_width;
  
  public:
    bucket_table() {}
    bucket_table(index_type n, coord_type min_x, coord_type max_x, double adjust_factor = 0.5) 
      { init(n, min_x, max_x, adjust_factor); }

    template<class COORD>
    bucket_table(index_type n, COORD min_x, COORD max_x, double adjust_factor = 0.5) 
      { init(n, min_x, max_x, adjust_factor);}

    void init(index_type n, coord_type min_x, coord_type max_x, double adjust_factor = 0.5);
 
    template<class COORD>
    void init(index_type n, COORD min_x, COORD max_x, double adjust_factor = 0.5) 
    { init(n, convert_point<coord_type>(min_x), convert_point<coord_type>(max_x), adjust_factor);}


    void clear_table() { b->clear(); }

    template<class COORD>
    Cell locate(COORD x) const { 
      REQUIRE(inside(x), "could not find point " << x, 1);
      return geom->locate(x).TheCell(); 
    }

    template<class COORD>
    bool inside(COORD x) const { return geom->locate(x).found();}

    /*! \brief Access values at bucket of \c x 
     */
    template<class COORD>
    value_type & operator[](COORD x) {
      return (*b)[locate(x)];
    }
    
    /*! \brief Access values at bucket of \c x (read only)
     */
    template<class COORD>
    value_type const& operator()(COORD x) {
      return get(locate(x));
    }
    
    value_type const& get(Cell c) const {
      return (*b)(c);
    }
     
    template<class COORD>    
    value_type get_neighborhood(COORD x, int width = 1) const {
      return get_neighborhood(locate(x), width);
    }
    
    value_type get_neighborhood(Cell c, int width = 1) const {
      index_type w(width);
      cart_subrange_type nbs(g, 
			     Cell(g, clamp_tuple(g->low_cell_index(), g->high_cell_index(), c.index() - w)),
			     Cell(g, clamp_tuple(g->low_cell_index(), g->high_cell_index(), c.index() + w)));
			     
      
      value_type res;
      typedef grid_types<cart_subrange_type> rgt;
      for(typename rgt::CellIterator nb = GrAL::begin_x(nbs); !nb.IsDone(); ++nb) {
	value_type const& nbs_i = get(*nb);
	res.insert(res.end(), nbs_i.begin(), nbs_i.end());
      }
      GrAL::sequence::sort_and_make_unique(res);
      return res;
    }

    grid_type const& TheGrid() const { return *g;}
    geom_type const& TheGeom() const { return *geom;}

    coord_type min() const { return geom->coord(typename gt::Vertex(g, g->low_vertex_index()));}
    coord_type max() const { return geom->coord(typename gt::Vertex(g, g->high_vertex_index()));}
    coord_type cell_width() const { return m_cell_width; }
    
  }; // class bucket_table


  template<class T, int N>
  void bucket_table<T,N>::init(typename bucket_table<T,N>::index_type n,
			       typename bucket_table<T,N>::coord_type min_x,
			       typename bucket_table<T,N>::coord_type max_x,
			       double adjust_factor)
 
    { 
      // make the window a bit larger, to avoid boundary situations
      // and dist values of zero.
      n = max_tuple(n, index_type(1));
      coord_type adjust = quotient(coord_type(1.0), coord_type(n));
      min_x = min_x - adjust_factor*adjust;
      max_x = max_x + adjust_factor*adjust;
      coord_type dist  = max_x - min_x;
      m_cell_width = quotient(dist, n);

      g    = new_ref_ptr(new grid_type(n+index_type(1)));
      typedef mapping_type mt;
      mapping_type A     = mt::translation(min_x) (mt::scaling(dist));
      mapping_type A_inv = mt::scaling(inverse(dist))(mt::translation(-min_x));
      geom = new_ref_ptr(new geom_type(*g, g->low_vertex_index(), g->high_vertex_index(), A));
      geom->set_inverse_mapping(A_inv);
      b = new_ref_ptr(new table_type(g));
    }
  


}


#endif
