#ifndef GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_H
#define GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_H

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Hierarchical/hierarchical-grid-table.h"

namespace GrAL {

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
  template<class HGRID, class GF> 
  class hier_grid_function_base : public HGRID::observer_type { // E::hier_grid_type::observer_type {
    typedef hier_grid_function_base<HGRID, GF>              self;
  public:
    typedef HGRID                                     hier_grid_type;
    typedef typename hier_grid_type::observer_type    base;
    typedef typename base::notifier_base              notifier_base;
    typedef typename base::notifier_type              notifier_type;

  public:
    typedef GF                                            flat_gf_type; 
    typedef typename flat_gf_type::value_type             value_type;
    typedef typename flat_gf_type::reference              reference;
    typedef typename flat_gf_type::const_reference        const_reference;
    typedef typename hier_grid_type::level_handle         level_handle;
    
    hier_grid_table<hier_grid_type, flat_gf_type> gfs;
    // this value may be used to fill automatically added layers
    bool       default_val_set;
    value_type default_val;
  public:
    hier_grid_function_base() : default_val_set(false) {}
    hier_grid_function_base(hier_grid_type const& gg) : gfs(gg), default_val_set(false) { connect(&gg);}
    hier_grid_function_base(hier_grid_type const& gg, value_type t) 
      : gfs(gg), default_val_set(true), default_val(t)
    { 
      set_value(t);
      connect(&gg);
    }
    hier_grid_function_base(ref_ptr<hier_grid_type const> gg) : gfs(gg), default_val_set(false) { connect(& (*gg));}
    hier_grid_function_base(ref_ptr<hier_grid_type const> gg, value_type t) 
      : gfs(gg), default_val_set(true), default_val(t)
    { 
      set_value(t);
      connect(&(*gg));
    }

    void init(hier_grid_type const& gg, value_type const& t); 
    void set_grid(hier_grid_type const& gg);
    void set_value(value_type t);  
    void clear();

    //! get in sync with grid: add/remove missing/superflous levels
    void update();

    ref_ptr<const hier_grid_type> TheGrid () const { return gfs.TheGrid();}
    ref_ptr<const hier_grid_type> TheHGrid() const { return gfs.TheGrid();}


    template<class ELEM>
    reference       operator[](ELEM const& e)       { return gfs[e.level()][e.Flat()];}
    template<class ELEM>
    const_reference operator()(ELEM const& e) const { return gfs(e.level())(e.Flat());}

    flat_gf_type      & operator[](level_handle lev)       { cv(lev); return gfs[lev];}
    flat_gf_type const& operator()(level_handle lev) const { cv(lev); return gfs(lev);}

    bool empty() const { return gfs.empty();}

    // this could be private if automatic level management via observer/notifier would be implemented
    level_handle add_finer_level();
    level_handle add_finer_level(value_type const& t);
    level_handle add_coarser_level();
    level_handle add_coarser_level(value_type const& t);
    void         remove_finest_level();
    void         remove_coarsest_level();

    bool valid() const { 
      return  
	(empty() && TheGrid()->empty() )
	||    
	(   (gfs.coarsest_level() == TheGrid()->coarsest_level()) 
	 && (gfs.finest_level()   == TheGrid()->finest_level()));
    }
    bool valid(level_handle lev) const { return (lev >= gfs.coarsest_level()) && (lev <= gfs.finest_level());}
    void cv(level_handle lev) const { REQUIRE(valid(lev), "lev=" << lev, 1); }

    virtual void notifier_assigned  (notifier_base const* n);
    virtual void hgrid_level_added  (notifier_type const* n, level_handle added);
    virtual void hgrid_level_removed(notifier_type const* n, level_handle removed);
  private:
  }; // class hier_grid_function_base

} //  namespace hierarchical
} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "hierarchical-grid-function-base.C"
#endif


#endif
