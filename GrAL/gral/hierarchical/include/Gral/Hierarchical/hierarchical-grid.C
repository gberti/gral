#ifndef GRAL_GB_HIERARCHICAL_GRID_C
#define GRAL_GB_HIERARCHICAL_GRID_C

// $LICENSE_NEC
#include "Gral/Hierarchical/hierarchical-grid.h"


namespace hierarchical {



  template<class Grid, class GT>
  hgrid_cartesian<Grid,GT>::hgrid_cartesian(typename hgrid_cartesian<Grid,GT>::flat_grid_type    const& root, 
					    typename hgrid_cartesian<Grid,GT>::pattern_grid_type const& pat)
  {
    init(root,pat);
  }

  template<class Grid, class GT>
  hgrid_cartesian<Grid,GT>::hgrid_cartesian(ref_ptr<typename hgrid_cartesian<Grid,GT>::flat_grid_type const> root, 
					    typename hgrid_cartesian<Grid,GT>::pattern_grid_type const& pat)
  {
    init(*root,pat);
  }


  template<class Grid, class GT>
  void
  hgrid_cartesian<Grid,GT>::init(typename hgrid_cartesian<Grid,GT>::flat_grid_type    const& root, 
				 typename hgrid_cartesian<Grid,GT>::pattern_grid_type const& pat)
  {
    grids.push_back(root);
    the_pattern = pat;
    for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
      dynamic_cast<observer_type*>(*ob)->notifier_assigned(this);
    
  }


 template<class Grid, class GT>
 hgrid_cartesian<Grid,GT> & hgrid_cartesian<Grid,GT>::operator=(hgrid_cartesian<Grid,GT> const& rhs)
 {
   if(this != &rhs) {
     before_assignment();
     grids = rhs.grids;
     after_assignment();
   }
   return *this;
 }


 template<class Grid, class GT>
 typename hgrid_cartesian<Grid,GT>::level_handle 
 hgrid_cartesian<Grid,GT>::add_finer_level() 
 {
   REQUIRE_ALWAYS( (! empty()), "",1);
   grids.push_back(flat_grid_type(index_type(1) + product(grids.back().cell_size(), the_pattern.cell_size())));
   for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
     dynamic_cast<observer_type*>(*ob)->hgrid_level_added(this,finest_level());
   return finest_level();
 }


 template<class Grid, class GT>
 typename hgrid_cartesian<Grid,GT>::level_handle 
 hgrid_cartesian<Grid,GT>::add_coarser_level() 
 { 
   REQUIRE_ALWAYS( (! empty()), "",1);
   REQUIRE_ALWAYS( coarsenable(), "", 1);
   grids.push_front(flat_grid_type(index_type(1) + quotient(grids.front().cell_size(), the_pattern.cell_size())));
   for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
     dynamic_cast<observer_type*>(*ob)->hgrid_level_added(this,coarsest_level());
   return coarsest_level();
 }

 template<class Grid, class GT>
 void
 hgrid_cartesian<Grid,GT>::remove_finest_level() 
 { 
   ce();
   level_handle oldlev = finest_level();
   grids.pop_back(); 
   for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
     dynamic_cast<observer_type*>(*ob)->hgrid_level_removed(this,oldlev);
 }


 template<class Grid, class GT>
 void
 hgrid_cartesian<Grid,GT>::remove_coarsest_level() 
 { 
   ce(); 
   level_handle oldlev = coarsest_level();
   grids.pop_front();
   for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
     dynamic_cast<observer_type*>(*ob)->hgrid_level_removed(this,oldlev);
   
 }

 template<class Grid, class GT>
 template<class HVERTEX>
 typename HVERTEX::vertex_handle
 hgrid_cartesian<Grid,GT>::coarsest_parent(HVERTEX const& v) const 
 {
   index_type pat0  = the_pattern.cell_size(); //vertex_size();
   index_type pat   = pat0;
   index_type vidx = v.Flat().index();
   int level_diff = 0;
   while(v.level() - level_diff > coarsest_level() && does_divide(pat, vidx)) {
     ++level_diff;
     pat = product(pat, pat0);
   }
   pat = quotient(pat, pat0); // pat = pat0^level_diff
   
   level_handle  newlev = v.level() - level_diff;
   return  typename HVERTEX::vertex_handle(FlatGrid(newlev)->get_vertex_handle(quotient(vidx,pat)), newlev); 
 }

  template<class Grid, class GT>
  temporary<typename hgrid_cartesian<Grid,GT>::cart_subrange_type>
  hgrid_cartesian<Grid,GT>::descendants(typename hgrid_cartesian<Grid,GT>::hier_cell_type const& p, 
					typename hgrid_cartesian<Grid,GT>::level_handle lev) const 
  {
    unsigned level_diff = lev - p.level();
    REQUIRE(level_diff >= 0, "leveldiff=" << level_diff,1);
    cell_index_type sz = power(the_pattern.cell_size(), level_diff);
    cell_index_type offset = product(p.Flat().index(), sz);
    // assume vertex based arguments to subrange
    return temporary<cart_subrange_type>(cart_subrange_type(*FlatGrid(lev), offset, offset+sz+cell_index_type(1))); 
  }

} // namespace hierarchical

#endif
