/*! \file 

*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex2D/all.h"

#include <map>
#include <algorithm> 

#include "Gral/Base/vtuple2d.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"


namespace GrAL {

//---------------  construction & destruction  ----------------------

Complex2D::Complex2D() : num_of_edges_cache(-1) {}


// in the special case of vectors of integers, this could be done
// more efficiently by just copying the vectors.
Complex2D::Complex2D(const Complex2D& rhs)
{ 
  num_of_edges_cache = rhs.num_of_edges_cache;
  stored_geometry_complex2D dummy(*this);
  ConstructGrid(*this,dummy,
                rhs,stored_geometry_complex2D(rhs)); 
}

Complex2D& Complex2D::operator=(const Complex2D& rhs)
{ 
  if(this != &rhs) {
    clear();
    stored_geometry_complex2D dummy(*this);
    ConstructGrid(*this,dummy, rhs,stored_geometry_complex2D(rhs)); 
    num_of_edges_cache = rhs.num_of_edges_cache;
  }
  return (*this);
}

Complex2D::~Complex2D() { clear();}

void Complex2D::clear()
{ 
  num_of_edges_cache = -1;
  _cells.clear(); 
  _vertices.clear();
  _boundary.clear(); 
  archetypes.clear();
  arch_for_n_vertices.clear();
}

//----------------- Adjacency calculations --------------------------

//--------------------- cells on vertices ---------------------------

// calculates for each vertex the (unordered) list of adjacent cells.
// Precondition: 
//  the vertex information in cells is set, i.e. VertexOnCellIterator works.  
void Complex2D::calculate_vertex_cells() 
{
  for(CellIterator c=FirstCell(); ! c.IsDone(); ++c)
    for(VertexOnCell2D_Iterator v(*c); ! v.IsDone(); ++v) {
        _vertices[handle(*v)]._cells.push_back(handle(c));
    }
}


//----------------------- cells on cells ----------------------------


//-------------------------------------------------------------------
//  calculate cell neighbor information.
//  PRE:
//   + local cell vertex lists are correct.
//   + local cell neighbor lists contain space for the 
//     correct number of neighbors; the content is not used, but should
//     be  set to boundary.
//   + this->_boundary should be empty (results will be appended)
//  POST:
//  + for each cell the ordered list of neighbor cells is filled,
//    using the vertex-information of the cells.
//  + local cell neighbor list entries for facets on the boundary 
//    remain unchanged. (this is correct if they have been set to boundary before.)
//  + the unordered list of boundary facets (this->_boundary) is filled.
//
//  ALGORITHM:
//   c1 is a neighbour to c2
//   <=> c1 and c2 have a common facet == vertex-pair == vtuple
//   so we loop trough all facets and see if we already have 
//   visited its vtuple (and the corr. cell) from the other side.
//  This algorithm generalizes to 3D (only function get_vertices has to
//  be rewritten)
//-------------------------------------------------------------------

void Complex2D::calculate_neighbour_cells()
{
  typedef vtuple_2d<Complex2D> vtuple;
  typedef ::std::map<vtuple, FacetOnCellIterator, ::std::less<vtuple> > FacetTable;
  FacetTable Facets;
  CalculateNeighbourCells(*this,*this,Facets);

  typedef FacetTable::iterator MapIt;
  MapIt bfacet; 
  for(bfacet = Facets.begin(); bfacet != Facets.end(); ++bfacet){
    _boundary.push_back((*bfacet).second);
  }
}

void Complex2D::calculate_num_of_edges() const 
{
  num_of_edges_cache= 0;
  for(EdgeIterator e(FirstEdge()); ! e.IsDone(); ++e)
    ++num_of_edges_cache;
}

void Complex2D::calculate_archetypes() 
{
  archetypes.clear();
  arch_for_n_vertices.clear();
  for(CellIterator c(*this); ! c.IsDone(); ++c)
    add_archetype_of(*c);
}

void Complex2D::add_archetype_of(Complex2D::Cell const& c) 
{
  add_archetype(archetype_type(c.NumOfVertices()), c.NumOfVertices());
}


Complex2D::archetype_handle
Complex2D::add_archetype(Complex2D::archetype_type const& A, int nv)  
{
  REQUIRE(nv >= 0, " nv=" << nv ,1);

  // if A is not already present, add it.
  if( ((int)arch_for_n_vertices.size() < nv + 1 || arch_for_n_vertices[nv] == -1)) {

    archetypes.push_back(A);
    archetype_handle a = archetypes.size()-1;
    
    // enlarge mapping (#cell vertices) -> archetype
    int old_sz = arch_for_n_vertices.size();
    if(old_sz < nv +1) {
      arch_for_n_vertices.resize(nv+1, -1);
      //for(unsigned i = old_sz; i < arch_for_n_vertices.size(); ++i)
      //	arch_for_n_vertices[i] = -1;
    }
    arch_for_n_vertices[nv] = a;
  }

  return  arch_for_n_vertices[nv];
}

} // namespace GrAL 
