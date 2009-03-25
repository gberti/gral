#ifndef GRAL_VTK_UGRID_ADAPTER_GRID_FUNCTIONS_H
#define GRAL_VTK_UGRID_ADAPTER_GRID_FUNCTIONS_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Adapters/VTK/vtk-ugrid-adapter.h"

// generic grid functions
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Base/partial-grid-function-hash.h"

#include "Utility/ref-ptr.h"

namespace GrAL {

  template<int D, class T> 
  class grid_function<vtk_ugrid::VertexVTK<vtk_ugrid::UGridVTKAdapter<D> >, T>
    : public grid_function_vector<vtk_ugrid::VertexVTK<vtk_ugrid::UGridVTKAdapter<D> >, T> 
  {
    typedef grid_function_vector<vtk_ugrid::VertexVTK<vtk_ugrid::UGridVTKAdapter<D> >, T> base;
  public:
    typedef typename base::grid_type grid_type;
    
    grid_function() {}
    grid_function(grid_type const& gg) : base(gg) {}
    grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
    grid_function(ref_ptr<grid_type const> gg) : base(gg) {}
    grid_function(ref_ptr<grid_type const> gg, T const& t) : base(gg,t) {}
  };
  

  template<int D, class T> 
  class grid_function<vtk_ugrid::CellVTK<vtk_ugrid::UGridVTKAdapter<D> >, T>
    : public grid_function_vector<vtk_ugrid::CellVTK<vtk_ugrid::UGridVTKAdapter<D> >, T> 
  {
    typedef grid_function_vector<vtk_ugrid::CellVTK<vtk_ugrid::UGridVTKAdapter<D> >, T> base;
  public:
    typedef typename base::grid_type grid_type;
    
    grid_function() {}
    grid_function(grid_type const& gg) : base(gg) {}
    grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
    grid_function(ref_ptr<grid_type const> gg) : base(gg) {}
    grid_function(ref_ptr<grid_type const> gg, T const& t) : base(gg,t) {}
  };
  

  



} // namespace GrAL

#endif
