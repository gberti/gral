

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
*/

#include "Gral/Grids/Triang2D/triang2d.h"

#include <vector>
#include <algorithm>

namespace GrAL {

Triang2D::Triang2D(Triang2D const& rhs) {
  cells     = rhs.cells;
  ncells    = rhs.ncells;
  nvertices = rhs.nvertices;
  nedges    = rhs.nedges;
  nedges_valid = rhs.nedges_valid;

  do_copy(); 
  owned = true; 
}

Triang2D& Triang2D::operator=(Triang2D const& rhs) {
  if( this != &rhs) {
    clear();
    cells     = rhs.cells;
    ncells    = rhs.ncells;
    nvertices = rhs.nvertices;
    nedges    = rhs.nedges;
    nedges_valid = rhs.nedges_valid;

    do_copy();  
    owned = true; 
  }
  return *this;
}

Triang2D::~Triang2D() { clear();}

void Triang2D::clear() 
{
  if(owned) {
    delete [] cells; // !!
  }
  cells = 0;
  owned = false;
  ncells = nvertices = nedges = 0;
  nedges_valid = false;
}

Triang2D::size_type Triang2D::calc_num_of_edges() const {
  nedges = 0;
  for(EdgeIterator e = FirstEdge(); !e.IsDone(); ++e)
    ++nedges;
  return nedges;
}

Triang2D::size_type Triang2D::calc_num_of_vertices() 
{
  std::vector<bool>  found_vertices(3*ncells, false); // max. number of cells
  for(size_type c = 0; c < ncells; ++c)
    for(size_type vc = 0; vc < 3; ++vc)
      found_vertices[cells[3*c+vc]] = true;
  bool found = true;
  size_type v = -1;
  while(found) {
    found &= found_vertices[++v];
  }
  nvertices = v;  

  // verify that vertices are consecutive - VertexIterator relies on this!
  for(; v < 3*ncells; ++v)
    if (found_vertices[v])
      std::cerr << "Vertices not consecutive: v = " << v << std::endl;
  return nvertices;
}

void Triang2D::do_copy() 
{
    size_type * my_cells = new size_type[3*ncells];
    std::copy(cells, cells+3*ncells, my_cells);
    cells = my_cells;
    owned = true;
}

void Triang2D::DoCopy() { if (!owned) do_copy();}

void Triang2D::Copy(size_type const* c, size_type nc, size_type nv, int offset)
{
  size_type * cc = new size_type[nc*3];
  for(size_type cell = 0; cell < 3*nc; ++cell)  
    cc[cell] = c[cell] - offset;
  Steal(cc, nc, nv);
  owned = true;
}

void Triang2D::Steal(size_type* c, size_type nc, size_type nv)
{
  clear();
  cells = c;
  ncells = nc;
  nvertices = nv;
  owned = true;
}

Triang2D::SD::SD() 
{
  typedef archetype_geom_type::coord_type local_coord_type;
  the_archetype[0] = archetype_type(3);
  the_archetype_geom[0] = archetype_geom_type(the_archetype[0]);
  the_archetype_geom[0].coord(0) = local_coord_type(0.0, 0.0);
  the_archetype_geom[0].coord(1) = local_coord_type(1.0, 0.0);
  the_archetype_geom[0].coord(2) = local_coord_type(0.0, 1.0);
}

Triang2D::SD Triang2D::sd;

} // namespace GrAL 
