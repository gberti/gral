#ifndef GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_H
#define GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_H

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Hierarchical/hierarchical-grid-table.h"


namespace hierarchical {

  /*! \brief A hierarchical grid function for arbitrary (total or partial) flat grid function types
      \author Guntram Berti

      \templateparams
      - \c E is an element type of an hierarchical grid
      - \c T is a plain value type
      - \c GF is a grid function template, e.g. \c grid_function<> or \c partial_grid_function<>,
           see gridfunctions module.

     \note The template template parameter syntax of C++ is a bit rigid.
      Adding additional (defaulted) template parameters to \c GF would break this code.
      We could also use a non-template wrappers using template members to access the
      flat grid functions.
  */
  template<class E, class T, template<class EE, class TT> class GF>
  class hier_grid_function_base {
    typedef hier_grid_function_base<E,T, GF>              self;
  public:
    typedef E                                             element_type;
    typedef T                                             value_type;
    typedef typename E::hier_grid_type                    hier_grid_type;
    typedef typename E::flat_element_type                 flat_element_type;
    typedef typename hier_grid_type::level_handle         level_handle;
    typedef GF<flat_element_type,T>                       flat_gf_type; 
    //    typedef typename flat_gf_type::value_type             value_type;
    typedef typename flat_gf_type::reference              reference;
    typedef typename flat_gf_type::const_reference        const_reference;
    
    hier_grid_table<hier_grid_type, flat_gf_type> gfs;
    // this value may be used to fill automatically added layers
    // value_type default_val;
  public:
    hier_grid_function_base() {}
    hier_grid_function_base(hier_grid_type const& gg) : gfs(gg) {}
    hier_grid_function_base(hier_grid_type const& gg, value_type t) : gfs(gg) { set_value(t);}

    void init(hier_grid_type const& gg, value_type const& t); 
    void set_grid(hier_grid_type const& gg);
    void set_value(value_type t);  
    void clear();

    //! get in sync with grid: add/remove missing/superflous levels
    void update();

    const_ptr<hier_grid_type> TheGrid () const { return gfs.TheGrid();}
    const_ptr<hier_grid_type> TheHGrid() const { return gfs.TheGrid();}

    reference       operator[](element_type const& e)       { return gfs[e.level()][e.Flat()];}
    const_reference operator()(element_type const& e) const { return gfs(e.level())(e.Flat());}

    flat_gf_type      & operator[](level_handle lev)       { cv(lev); return gfs[lev];}
    flat_gf_type const& operator()(level_handle lev) const { cv(lev); return gfs(lev);}

    // this could be private if automatic level management via observer/notifier would be implemented
    level_handle add_finer_level();
    level_handle add_finer_level(value_type const& t);
    level_handle add_coarser_level();
    level_handle add_coarser_level(value_type const& t);
    void         remove_finest_level();
    void         remove_coarsest_level();

    bool valid() const { return (gfs.coarsest_level() == TheGrid()->coarsest_level()) 
			   &&   (gfs.finest_level() == TheGrid()->finest_level());}
    bool valid(level_handle lev) const { return (lev >= gfs.coarsest_level()) && (lev <= gfs.finest_level());}
    void cv(level_handle lev) const { REQUIRE(valid(lev), "lev=" << lev, 1); }
  private:
  }; // class hier_grid_function_base

} //  namespace hierarchical

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "hierarchical-grid-function-base.C"
#endif


#endif
