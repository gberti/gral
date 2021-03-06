#ifndef GRAL_GB_CARTESIANND_MAPPED_GEOMETRY_H
#define GRAL_GB_CARTESIANND_MAPPED_GEOMETRY_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Utility/pre-post-conditions.h"
#include "Gral/Geometries/point-location-base.h"

#include "Geometry/algebraic-primitives.h"
#include "Geometry/array-operations.h"
#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"

namespace GrAL {


namespace cartesiannd {

  template<class CARTGRID>
  struct default_coord {
    // could use CARTGRID::index_type::rebind<double>
    enum { dim = CARTGRID::dim };
    typedef tuple<double, dim> type;
  };


  /*! \brief A geometry for a Cartesian grid defined by an analytic mapping
      \ingroup cartesianndmodule

     
     The mapping is assumed to be defined on \f$ [0,1]^d \f$.
     Each vertex gets first assigned a unit coordinate in \f$ [0,1]^d \f$. 

     By default, these unit coordinates are chosen such that cells are isotropic hypercubes.
     This means that in the case of grids
     with different number of vertices in each direction, only the vertices
     with the largest integer indices will have a corresponding unit coordinate of 1.
     For example, in a grid with 4x2 cells, the right upper vertex (index \f$(4,2)\f$) 
     will be mapped to \f$(1, 0.5) \f$.

     In addition, it is possible to prescribe a Cartesian subrange which is mapped onto \f$ [0,1]^d \f$.
     In this case, an anisotropic (non-hypercubic) subrange leads of course to anisotropic coordinates.

     \see Test in \ref test-cartesiannd-geometry.C

     \note In the case of a \e nonlinear mapping, some predicates do not work correctly.

   */
  template<class CARTGRID, class MAP> //  = identity<cartesiannd::default_coord<CARTGRID>::type > >
  class mapped_geometry {
    typedef mapped_geometry<CARTGRID, MAP> self;
  public:
    typedef CARTGRID grid_type;
    typedef MAP      mapping_type;

    typedef typename mapping_type::result_type coord_type;
    typedef point_traits<coord_type>           pt;
    typedef algebraic_primitives<coord_type>   ap;
    typedef typename pt::component_type        scalar_type;

    typedef grid_types<grid_type>   gt;
    typedef typename gt::Vertex     Vertex;
    typedef typename gt::Edge       Edge;
    typedef typename gt::Cell       Cell;
    typedef typename gt::index_type        index_type;
    typedef typename gt::cell_index_type   cell_index_type;
    typedef typename gt::vertex_index_type vertex_index_type;

    typedef point_location_result<gt, coord_type> location_result_type;
  private:
    ref_ptr<grid_type    const> g;
    ref_ptr<mapping_type const> f;     // defined on unit cube (at least).
    ref_ptr<mapping_type const> f_inverse;

    coord_type         low_;
    coord_type         high_; // [low_, high_] will be first mapped to unit cube and
                              // then mapped by f.
    coord_type         delta;
  public:

    //@{
    //! \name Constructors
    //! \brief Empty geometry
    mapped_geometry() {}
 
   /*! \brief Initialize with mapping \c ff

   The unit coordinates are chosen such that cells are isotropic hypercubes.
   
   If \c mapped_geometry(gg) is called, the initialization 
   is complete only if \c MAP has a sensible default constructor.
   */
    mapped_geometry(grid_type const& gg, mapping_type ff = mapping_type::identity())
      : g(gg) 
    { 
      set_isotropic_box();
      f = new_ref_ptr(new mapping_type(ff));
      init();
    }

    void set_isotropic_box() {
      index_type diff = g->high_vertex_index() - g->low_vertex_index();
      high_ = low_ + coord_type(* std::max_element(diff.begin(), diff.end()));
    }

    /*! \brief Initialize with mapping \c ff
         
    The unit coordinates are chosen such that cells are isotropic hypercubes, 
    and the extent of the grid (in unit coordinates) is \e contained in \f$[0,1]^d\f$.
    
    If \c mapped_geometry(gg) is called, the initialization 
    is complete only if \c MAP has a sensible default constructor.
    */
    mapped_geometry(ref_ptr<grid_type const> gg, ref_ptr<mapping_type> ff)
      : g(gg)
    { 
      set_isotropic_box();
      f = ff;
      init();
    }


    /*! \brief Initialize with mapping \c ff, mapping the range \c [lw,hgh] onto \f$ ff([0,1]^d)\f$
         
        The mapping of \c [lw,hgh] into \f$[0,1]^d\f$ is isotropic \e only if [lw,hgh] is a
	hypercube.

        If \c mapped_geometry(gg, lw, hgh) is called, the initialization 
	is complete only if \c MAP has a sensible default constructor.
     */
    mapped_geometry(grid_type const& gg, 
		    coord_type lw, coord_type hgh, 
		    mapping_type ff = mapping_type::identity()) 
      : g(gg),
	low_(lw), high_(hgh) 
    { 
      f = new_ref_ptr(new mapping_type(ff));
      init();
    }
    /*! \brief Initialize with mapping \c ff, mapping the range \c [lw,hgh] onto \f$ ff([0,1]^d)\f$
         
        The mapping of \c [lw,hgh] \e onto \f$[0,1]^d\f$ is isotropic \e only if [lw,hgh] is a
	hypercube.

     */
    mapped_geometry(ref_ptr<grid_type const> gg, 
		    coord_type lw, coord_type hgh, 
		    ref_ptr<mapping_type> ff)
      : g(gg),
	low_(lw), high_(hgh) 
    { 
      f = ff;
      init();
  }


    /*! \brief Initialize using the unit coordinates of a master geometry

        This is useful e.g. for hierarchical grids (esp. with anisotropic refinement).
        Using standard unit coordinates would result in different extents of the hierarchies.

	\pre  \c TheGrid() is a refinement of \c master.TheGrid() 
	\post Extent of geometry equal to extent of master geometry.
     */
    mapped_geometry(grid_type const& gg, self const& master)
      : g(gg)
    {
      init_from_master(master);
    }
    /*! \brief Initialize using the unit coordinates of a master geometry

        See mapped_geometry(grid_type const& gg, self const& master)
     */
    mapped_geometry(ref_ptr<grid_type const> gg, 
		    ref_ptr<self const>      master)
      : g(gg)
    {
      init_from_master(*master);
    }

    //@}
  private:
    void init() {
      delta = GrAL::quotient(coord_type(1.0), coord_type(high_ - low_));
    }

    void init_from_master(self const& m) {
      coord_type size_refined =   g->high_vertex_index() -   g->low_vertex_index();
      coord_type size_master  = m.g->high_vertex_index() - m.g->low_vertex_index();
      coord_type refinement_factor = quotient(size_refined, size_master);

      coord_type g_low   =   g->low_vertex_index();
      coord_type m_g_low = m.g->low_vertex_index();
      low_  = g_low + product(m.low_  - m_g_low, refinement_factor);  
      high_ = g_low + product(m.high_ - m_g_low, refinement_factor);  
      f = new_ref_ptr(new mapping_type(* m.f));
      if(m.has_inverse())
	f_inverse = new_ref_ptr(new mapping_type(* m.f_inverse));
      init();
      // this may fail because of rounding errors ...
      //REQUIRE_ALWAYS(coord(g-> low_vertex_index()) == m.coord(m.g-> low_vertex_index()), "", 1);
      //REQUIRE_ALWAYS(coord(g->high_vertex_index()) == m.coord(m.g->high_vertex_index()), "", 1);
    }

  public:
    //! \brief Set the mapping 
    void set_mapping(mapping_type const& ff) {
      f.make_shared(new mapping_type(ff));
    }
    //! \brief Ditto
    void set_mapping(ref_ptr<mapping_type const> ff) {
      f = ff;
    }
    //! \brief Set the inverse mapping (for point location) 
    void set_inverse_mapping(mapping_type const& inv) { 
      f_inverse.make_shared(new mapping_type(inv));
    }
    //! \brief Ditto
    void set_inverse_mapping(ref_ptr<mapping_type const> inv) { 
      f_inverse = inv;
    }

    //! \brief Query inverse mapping
    bool has_inverse() const { return f_inverse != 0;}

    //! \brief Re-initialize, keeping the mapping
    void rebind(grid_type const& gg) {
      g = ref_ptr<grid_type const>(gg);
      low_  = g->low_vertex_index();
      high_ = g->high_vertex_index();
      init();
    }
    //! \brief Re-initialize
    void rebind(grid_type const& gg, mapping_type const& ff) {
      f.make_shared(new mapping_type(ff));
      rebind(gg);
    }

    //@{
    //! \name Accessors
    //! \brief Anchor grid
    ref_ptr<grid_type const> TheGrid() const { cb(); return g;}
    //! \brief Get mapping
    ref_ptr<mapping_type const> TheMapping() const { return f;}
    //! \brief Get inverse mapping
    ref_ptr<mapping_type const> TheInverse() const { return f_inverse;}
    //@}

    //! \brief Dimension of embedding space
    unsigned space_dimension() const { return pt::Dim(delta);}

  private:
    //! \brief Anisotropic unit coordinate. Inverse of unit2index.
    coord_type unit_coord(index_type idx) const { 
      coord_type r(convert_point<coord_type>(idx) - low_);
      return product(r, delta);
    }
    //! \brief Index i such that local_p is in cell with index i. Inverse of unit_coord.
    cell_index_type unit2index(coord_type local_p) const {
      //coord_type rational_index = quotient(local_p, delta);
      //index_type integral_index = floor_tuple(rational_index);
      return convert_point<cell_index_type>(floor_tuple(quotient(local_p, delta)) + low_);

    }
  public:
    //! Coordinate of vertex
    coord_type coord (Vertex const& v) const { cb(); return (*f)(unit_coord(v.index()));}
    coord_type coord (index_type    v) const { cb(); return (*f)(unit_coord(v));}

    /*! \brief length of an edge.
        
       \note The result is only correct in the case of an affine mapping
     */
    scalar_type length(Edge   const& e) const { 
      cb(); 
      return ap::distance(coord(e.Vl(typename Edge::local_index_type(0))), 
			  coord(e.Vl(typename Edge::local_index_type(1))));
    }
    /*! \brief Result is the mapping of the center of c with respect to unit coordinates \f$ \in [0,1]^d \f$
        
        This is the center of inertia only if the mapping \c f is affine. 
    */
    coord_type center(Cell   const& c) const { cb(); return (*f)(unit_coord(c.index()) + 0.5*delta);} 
    coord_type barycenter(Cell const& c) const { 
      cb();
      coord_type res(0.0);
      for(typename gt::VertexOnCellIterator vc(c); !vc.IsDone(); ++vc)
	res += coord(*vc);
      res *= 1.0/c.NumOfVertices();
      return res;
    }

    bool bound() const { return g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);}

    /*! \brief locate point \c pp in grid

       \note Points on the high boundary will be reported as projected, not as inside the grid.
     */
    template<class COORD>
    location_result_type locate(COORD const& pp) const;

  }; // mapped_geometry<CARTGRID,MAP>


  template<class CARTGRID, class MAP>
    template<class COORD>
    typename mapped_geometry<CARTGRID,MAP>::location_result_type
    mapped_geometry<CARTGRID,MAP>::locate(COORD const& pp) const 
    {
      REQUIRE( (f_inverse != 0), "Must specifiy inverse mapping!", 1);
      coord_type p; assign_point(p,pp);
      coord_type local_p = (*f_inverse)(p); // in [0,1]^d iff p in some grid cell
      cell_index_type cell_of_p = unit2index(local_p);
      if(TheGrid()->valid_cell_index(cell_of_p))
	return location_result_type(Cell(TheGrid(), cell_of_p), p);
      else {
	//index_type grid_low  = TheGrid()->low_cell_index();
	//index_type grid_high = TheGrid()->high_cell_index();
	index_type projected_cell    = clamp_tuple(TheGrid()->low_cell_index(), TheGrid()->high_cell_index(), cell_of_p);
	coord_type projected_local_p = clamp_tuple(unit_coord(TheGrid()->low_vertex_index()), 
						   unit_coord(TheGrid()->high_vertex_index()), local_p);
        // FIXME: Problems occur for points at 'upper' grid boundary (at least one coordinate 
	// = highest grid coordinate in this direction)
	// These will be rounded to outside cell => should measure relative difference of local_p and projected_local_p 
	// This is a real problem only for exact arithmetic, though.
	return location_result_type(Cell(TheGrid(), projected_cell), (*f)(projected_local_p), 
				    location_result_type::projection_tag);
      }
    }

} // namespace cartesiannd

} // namespace GrAL 

#endif
