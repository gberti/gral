#ifndef GRAL_GB_HIERARCHICAL_GEOMETRY_H
#define GRAL_GB_HIERARCHICAL_GEOMETRY_H

// $LICENSE_NEC

#include "Utility/ref-ptr.h"
#include "Gral/Hierarchical/hierarchical-grid-table.h"
#include "Gral/Hierarchical/hierarchical-grid.h"

namespace hierarchical {

  /*! \brief A geometry type  for hierarchical grids

      \author Guntram Berti

      \templateparams
      - \c HGRID : an hierarchical grid
      - \c FLATGEOM : a geometry on HGRID::flat_grid_type


  */
  template<class HGRID, class FLATGEOM>
  class hier_geometry /* : public HGRID::observer_type<hier_grid_table<HGRID, GE> > */ {
  public:
    typedef HGRID          hier_grid_type;
    typedef hier_grid_type grid_type;
    typedef hier_grid_type hgt;
    typedef typename hgt::level_handle   level_handle;

    typedef FLATGEOM                                flat_geometry_type;
    typedef typename flat_geometry_type::coord_type coord_type;

    typedef hier_grid_table<hier_grid_type, flat_geometry_type>  table_type;
    typedef typename table_type::reference         reference;
    typedef typename table_type::const_reference   const_reference;

    template<class ELEMBASE>
    struct vertex_template { typedef typename hier_grid_type::vertex_template<ELEMBASE>::type type;};
  private:
    table_type geoms;
 
  public:
    hier_geometry();
    hier_geometry(hier_grid_type const& gg);
    hier_geometry(ref_ptr<hier_grid_type const> gg);

    void set_grid  (hier_grid_type const& gg);
    void set_grid  (ref_ptr<hier_grid_type const> gg);
    void clear();

    //! get in sync with grid: add/remove missing/superfluous level
    void update();

    reference       operator[](level_handle lev)       { return geoms[lev];}
    const_reference operator()(level_handle lev) const { return geoms(lev);}

    // template<class ELEMBASE>
    //    coord_type  coord(typename vertex_template<ELEMBASE>::type const& v) const 
      // coord_type  coord(typename hier_grid_type::vertex_template<ELEMBASE>::type const& v) const 
      // coord_type  coord(h_vertex_t<ELEMBASE> const& v) const 
    template<class V>
    coord_type coord(V const& v) const
    { return geoms(v.level()).coord(v.Flat());}


    //! get the coarsest (root) level
    level_handle  coarsest_level() const { return geoms.coarsest_level();}
    //! get the finest level
    level_handle  finest_level()   const { return geoms.finest_level();}

    ref_ptr<const hier_grid_type> TheGrid() const { return geoms.TheGrid();}
  private:
    //    void init(hier_grid_type const& gg);
  public:
    // could deduce coordinates by restriction from children
    level_handle add_coarser_level();
    // could calculate coordinates from pattern
    level_handle add_finer_level();
    void         remove_coarsest_level();
    void         remove_finest_level();

  }; //  class hier_geometry<HGRID, GE>


} // namespace hierarchical


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "hierarchical-geometry.C"
#endif

#endif
