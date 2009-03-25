/*! \file

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Subranges/facet-grid.h"
#include "Gral/Subranges/enumerated-element-range.h"

#include "Gral/Grids/Cartesian3D/all.h"

int main() {
  

  typedef GrAL::cartesian3d::CartesianGrid3D grid_type;
  typedef GrAL::grid_types<grid_type>        gt;
  grid_type R(2,2,2);

  typedef GrAL::enumerated_element_range<gt::Facet> facet_range;
  facet_range fr(R); 
  fr.append(R.FirstFacet(), R.EndFacet());

  
  typedef GrAL::facet_grid<facet_range> f_grid_type;
  f_grid_type fg(fr);
  typedef GrAL::grid_types<f_grid_type> fgt;
  
  GrAL::iterator<fgt::VertexIterator, f_grid_type>::type it;
  it = fg.FirstVertex();

  fgt::VertexIterator vb = GrAL::begin<fgt::VertexIterator>(fg);
  fgt::VertexIterator ve = GrAL::end_x  (fg);
  fgt::size_type      nv = GrAL::size_d<0>(fg);

  fgt::CellIterator cb = GrAL::begin_x(fg);
  fgt::CellIterator ce = GrAL::end_x  (fg);
  fgt::size_type    nc = GrAL::size_d<2>(fg);


  
}
