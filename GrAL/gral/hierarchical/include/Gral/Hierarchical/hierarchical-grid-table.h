#ifndef GRAL_GB_HIERARCHICAL_GRID_TABLE_H
#define GRAL_GB_HIERARCHICAL_GRID_TABLE_H

// $LICENSE_NEC

#include "Utility/ref-ptr.h"

namespace hierarchical {

  /*! \brief A container for storing grid-entities bound to the levels of an hierarchical grid
      \author Guntram Berti

      This class template can be used to lift all types of grid entities 
      (subranges, grid functions, geometries, morphisms etc) to hierarchical grids, 
      by holding one instance of a grid entity (of type \c GE) bound to each level of 
      the hierarchical grid.   

      \templateparams
      - \c HGRID : an hierarchical grid
      - \c GE : a grid entity which can be bound to \c HGRID::flat_grid_type
        for example a grid function.


      Usage example:
      \code
      typedef ....                                flat_grid_type;
      typedef grid_types<flat_grid_type>          flatgt;
      typedef hgrid<flat_grid_type>               hier_grid_type;
      typedef grid_types<hier_grid_type>          hiergt;
      typedef enumerated_subrange<flat_grid_type> subrange_type;

      hier_grid_type hg;
      // ... fill hg with some levels  ...
      
      // create hierarchy of subranges 
      hier_grid_table<hier_grid_type, subrange_type> marked(hg);
      
      for(hiergt::level_type lev = hg.coarsest_level(); lev <= hg.finest_level(); ++lev) {
         for(hiergt::LevelCellIterator c(hg,lev); ! c.IsDone(); ++c)  {
	    flatgt::Cell fc = *c.Flat();
	    if(condition(fc)) 
              marked[lev].push_back(fc);
         }
      }  
      \endcode

  */
  template<class HGRID, class GE>
  class hier_grid_table /* : public HGRID::observer_type<hier_grid_table<HGRID, GE> > */ {
  public:
    typedef HGRID          hier_grid_type;
    typedef hier_grid_type hgt;
    typedef typename hgt::level_handle   level_handle;

    typedef GE                value_type;
    typedef GE                grid_entity_type;
    typedef value_type      & reference;
    typedef value_type const& const_reference;
  private:
    // this type essentially must be a bivector<>
    typedef typename hier_grid_type::table_type_template<GE>::type  table_type;
    table_type                entities;
    ref_ptr<const hier_grid_type> g;
  public:
    hier_grid_table();
    hier_grid_table(hier_grid_type const& gg);
    hier_grid_table(ref_ptr<hier_grid_type const> gg);
    // pass additional initializer as constructor arg. to grid entity levels
    //template<class T>
    //hier_grid_table(hier_grid_type const& gg, T const& initializer);

    void set_grid  (hier_grid_type const&         gg);
    void set_grid  (ref_ptr<hier_grid_type const> gg);
    void clear();

    //! get in sync with grid: add/remove missing/superfluous level
    void update();

    reference       operator[](level_handle lev)       { return entities[lev];}
    const_reference operator()(level_handle lev) const { return entities(lev);}

    //! get the coarsest (root) level
    level_handle  coarsest_level() const { return level_handle(entities.begin_index());}
    //! get the finest level
    level_handle  finest_level()   const { return level_handle(entities.back_index());}

    ref_ptr<const hier_grid_type> TheGrid() const { return g;}
  private:
    void init(hier_grid_type const& gg);
  public:
    level_handle add_coarser_level();
    level_handle add_finer_level();
    void         remove_coarsest_level();
    void         remove_finest_level();

  }; //  class hier_grid_table<HGRID, GE>


} // namespace hierarchical


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "hierarchical-grid-table.C"
#endif

#endif
