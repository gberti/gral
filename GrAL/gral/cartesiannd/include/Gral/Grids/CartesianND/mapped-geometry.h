#ifndef GRAL_GB_CARTESIANND_MAPPED_GEOMETRY_H
#define GRAL_GB_CARTESIANND_MAPPED_GEOMETRY_H

// $LICENSE_NEC

#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Utility/pre-post-conditions.h"

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"


namespace cartesiannd {

  template<class CARTGRID>
  struct default_coord {
    // could use CARTGRID::index_type::rebind<double>
    enum { dimension = CARTGRID::dim };
    typedef tuple<double, dimension> type;
  };


  template<class CARTGRID, class MAP> //  = identity<cartesiannd::default_coord<CARTGRID>::type > >
  class mapped_geometry {
  public:
    typedef CARTGRID grid_type;
    typedef MAP      mapping_type;
    typedef typename mapping_type::result_type coord_type;
    typedef grid_types<grid_type>    gt;
    typedef point_traits<coord_type> pt;

    typedef typename gt::Vertex Vertex;
    typedef typename gt::Cell   Cell;
    typedef typename gt::index_type index_type;
  private:
    ref_ptr<grid_type const> g;
    mapping_type       f;     // defined on unit cube (at least).
    index_type         low_;
    index_type         high_; // [low_, high_] will be first mapped to unit cube and
                              // then mapped by f.
    coord_type         delta;
  public:
    mapped_geometry() {}
    mapped_geometry(grid_type const& gg, mapping_type ff = mapping_type()) 
      : g(gg), f(ff), low_(gg.low_vertex_index()), high_(gg.high_vertex_index()) { init();}
    mapped_geometry(grid_type const& gg, index_type lw, index_type hgh, 
		    mapping_type ff = mapping_type()) 
      : g(gg), f(ff), low_(lw), high_(hgh) { init();}


    void init() {
      delta = coord_type(1.0);
      delta = quotient(delta, high_ - low_);
    }
    void rebind(grid_type const& gg) {
      g = ref_ptr<grid_type const>(gg);
      low_  = g->low_vertex_index();
      high_ = g->high_vertex_index();
      init();
    }
    void rebind(grid_type const& gg, mapping_type const& ff) {
      f = ff;
      rebind(g);
    }

    ref_ptr<grid_type const> TheGrid() const { return g;}
    unsigned space_dimension() const { return pt::Dim(delta);}

    coord_type unit_coord(index_type idx) const { 
      coord_type r(idx - low_);
      return quotient(r, high_);
    }
    coord_type coord (Vertex const& v) const { cb(); return f(unit_coord(v.index()));}
    /*! \brief Result is the mapping of the center of c with respect to unit coordinates \f$ \in [0,1]^d \f$
        
        This is the center of inertia only if the mapping \c f is affine. 
    */
    coord_type center(Cell   const& c) const { cb(); return f(unit_coord(c.index()) + 0.5*delta);} 

    bool bound() const { return g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);}

  }; // mapped_geometry<CARTGRID,MAP>

} // namespace cartesiannd


#endif
