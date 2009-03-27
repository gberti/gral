

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Grids/Triang3D/triang3d.h"
#include "Gral/IO/stream-grid-adapter.h"

#include "Gral/Grids/Complex2D/construct.h"

#include <vector>
#include <algorithm>

namespace GrAL {

Triang3D::Triang3D(Triang3D const& rhs) {
  cells     = rhs.cells;
  ncells    = rhs.ncells;
  nvertices = rhs.nvertices;
  do_copy(); 
  owned = true; 
}

Triang3D& Triang3D::operator=(Triang3D const& rhs) {
  if( this != &rhs) {
    clear();
    cells     = rhs.cells;
    ncells    = rhs.ncells;
    nvertices = rhs.nvertices;
    do_copy();  
    owned = true; 
  }
  return *this;
}

Triang3D::~Triang3D() { clear();}

void Triang3D::clear() 
{
  if(owned) {
    delete [] cells; 
  }
  cells = 0;
  owned = false;
  ncells = nvertices = 0;
}

  Triang3D::size_type Triang3D::calc_num_of_vertices() 
{
  std::vector<bool>  found_vertices((dim+1)*ncells, false); // max. number of vertices
  for(size_type c = 0; c < ncells; ++c)
    for(int vc = 0; vc < dim+1; ++vc)
      found_vertices[cells[(dim+1)*c+vc]] = true;
  bool found = true;
  size_type v = -1;
  while(found) {
    found &= found_vertices[++v];
  }
  nvertices = v;  

  // verify that vertices are consecutive 
  // (<=> no vertex v >= nvertices exists)
  // Better: renumber vertices if not consecutive.
  for(; v < (dim+1)*ncells; ++v)
    if (found_vertices[v])
      //      IOMgr::Error()  << "Vertices not consecutive: v = " << v << endl;
      std::cerr << "Vertices not consecutive: v = " << v << std::endl;
  return nvertices;
}

void Triang3D::do_copy() 
{
    size_type * my_cells = new size_type[(dim+1)*ncells];
    std::copy(cells, cells+(dim+1)*ncells, my_cells);
    cells = my_cells;
    owned = true;
}

void Triang3D::DoCopy() { if (!owned) do_copy();}

void Triang3D::Steal(Triang3D::size_type* c, Triang3D::size_type nc, Triang3D::size_type nv)
{
  clear();
  cells = c;
  ncells = nc;
  nvertices = nv;
  owned = true;
}

Triang3D::SD::SD() { initialize(); }

void Triang3D::SD::initialize()
{
  static int cnt = 0;
  if(cnt == 0) {
    ++cnt;
    int conn[4*4] = {
      3,    0, 1, 2,
      3,    1, 0, 3,
      3,    0, 2, 3,
      3,    3, 2, 1
    };
    stream_grid_mask<int *> arch(4,4,conn);
    ConstructGrid0(the_archetype[0], arch);
  }
}


Triang3D::SD Triang3D::sd;

} // namespace GrAL 
