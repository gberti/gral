
#include "Gral/Grids/Triang2D/triang2d.h"

#include <vector>


Triang2D::Triang2D(Triang2D const& rhs) {
  cells     = rhs.cells;
  ncells    = rhs.ncells;
  nvertices = rhs.nvertices;
  do_copy(); 
  owned = true; 
}

Triang2D& Triang2D::operator=(Triang2D const& rhs) {
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

Triang2D::~Triang2D() { clear();}

void Triang2D::clear() 
{
  if(owned) {
    delete [] cells; // !!
  }
  cells = 0;
  owned = false;
  ncells = nvertices = 0;
}

int Triang2D::calc_num_of_vertices() 
{
  // should test if vertices are numbered consecutively from 0..nv !
  // VertexIterator relies on this!
  vector<bool>  found_vertices(3*ncells, false); // max. number of cells
  for(int c = 0; c < ncells; ++c)
    for(int vc = 0; vc < 3; ++vc)
      found_vertices[cells[3*c+vc]] = true;
  bool found = true;
  int v = -1;
  while(found) {
    found &= found_vertices[++v];
  }
  nvertices = v;  

  // verify that vertices are consecutive
  for(; v < 3*ncells; ++v)
    if (found_vertices[v])
      //      IOMgr::Error()  << "Vertices not consecutive: v = " << v << endl;
      std::cerr << "Vertices not consecutive: v = " << v << std::endl;
  return nvertices;
}

void Triang2D::do_copy() 
{
    int * my_cells = new int[3*ncells];
    copy(cells, cells+3*ncells, my_cells);
    cells = my_cells;
    owned = true;
}

void Triang2D::DoCopy() { if (!owned) do_copy();}

void Triang2D::Steal(int* c, int nc, int nv)
{
  clear();
  cells = c;
  ncells = nc;
  nvertices = nv;
  owned = true;
}


