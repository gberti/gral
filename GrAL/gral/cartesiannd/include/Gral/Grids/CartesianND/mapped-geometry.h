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
    enum { dimension = CARTGRID::dimension };
    typedef tuple<double, dimension> type;
  };


  template<class CARTGRID, class MAP> //  = identity<cartesiannd::default_coord<CARTGRID>::type > >
  class mapped_geometry {
  public:
    typedef CARTGRID grid_type;
    typedef MAP      mapping_type;
    typedef typename mapping_type::result_type coord_type;
    typedef grid_types<grid_type> gt;
    typedef typename gt::Vertex Vertex;
    typedef typename gt::index_type index_type;
  private:
    ref_ptr<grid_type const> g;
    mapping_type       f;     // defined on unit cube (at least).
    index_type         low_;
    index_type         high_; // [low_, high_] will be first mapped to unit cube and
                              // then mapped by f.
  public:
    mapped_geometry() {}
    mapped_geometry(grid_type const& gg, mapping_type ff = mapping_type()) 
      : g(gg), f(ff), low_(gg.vertex_low()), high_(gg.vertex_high()) {}
    mapped_geometry(grid_type const& gg, index_type lw, index_type hgh, 
		    mapping_type ff = mapping_type()) 
      : g(gg), f(ff), low_(lw), high_(hgh) {}

    coord_type unit_coord(Vertex const& v) const { 
      coord_type r(v.index() - low_);
      return quotient(r, high_);
    }
    coord_type coord(Vertex const& v) const { cb();  return f(unit_coord(v));}

    bool bound() const { return g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);}

  }; // mapped_geometry<CARTGRID,MAP>

} // namespace cartesiannd


#endif
