/*! \file
 */


#include "Gral/Views/cut-grid-view.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Test/all.h"

#include "Gral/IO/complex2d-format-output.h"

#include "Container/functions.h"
#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"


#include <iostream>

namespace GrAL { namespace cut_grid_view {
  typedef cartesian2d::RegGrid2D  grid_type;
  typedef grid_types<grid_type>   gt;
  typedef cfg<gt>                 cfg_type;
  template class grid_view<cfg_type>;

}}


int main()
{
  typedef  GrAL::cartesian2d::RegGrid2D             grid_type;
  typedef  GrAL::grid_types<grid_type>              gt;
  typedef  GrAL::cut_grid_view::cfg<gt>             cfg_type;
  typedef  GrAL::cut_grid_view::grid_view<cfg_type> view_type;
  typedef  GrAL::grid_types<view_type>              vgt;

  typedef GrAL::tuple<double,2>                               coord_type;
  typedef stdext::identity<coord_type>                        mapping_type;
  typedef GrAL::cartesian2d::mapped_geometry<mapping_type>    geom_type;
  typedef GrAL::cut_grid_view::geom_view<cfg_type, geom_type> geom_view_type;

  {
    grid_type R(3,3);
    GrAL::grid_function<gt::Cell,int> comps(R,0);
    for(gt::CellIterator c(R); !c.IsDone(); ++c)
      //   if(c.index()[0] == 0)
      comps[*c] = c.handle();
    
    view_type cut_view(R, comps);
    
    GrAL::test_sequence_iterator <vgt::Vertex>           (cut_view, std::cout);
    GrAL::test_sequence_iterator <vgt::Cell>             (cut_view, std::cout);
    GrAL::test_incidence_iterator<vgt::Vertex, vgt::Cell>(cut_view, std::cout);
    
    geom_type      GeomR(R);
    geom_view_type cut_geom(cut_view, GeomR);
    GrAL::OstreamComplex2DFmt Out("total-cut.complex2d");
    
    ConstructGrid(Out, Out, cut_view, cut_geom);
  }


  {
    grid_type R(3,3);
    GrAL::grid_function<gt::Cell,int> comps(R,0);
    comps[gt::Cell(R,0,0)] = 1;
    comps[gt::Cell(R,1,0)] = 2;
    
    std::vector<int> non_cut_comps(1,0);
    view_type cut_view(R, comps, non_cut_comps.begin(), non_cut_comps.end());
    
    GrAL::test_sequence_iterator <vgt::Vertex>           (cut_view, std::cout);
    GrAL::test_sequence_iterator <vgt::Cell>             (cut_view, std::cout);
    GrAL::test_incidence_iterator<vgt::Vertex, vgt::Cell>(cut_view, std::cout);
    
    geom_type      GeomR(R);
    geom_view_type cut_geom(cut_view, GeomR);
    GrAL::OstreamComplex2DFmt Out("partial-cut.complex2d");
    
    ConstructGrid(Out, Out, cut_view, cut_geom);
  }



}
