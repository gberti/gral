#ifndef GRAL_GB_FACET_ON_CELL_FUNCTION_H
#define GRAL_GB_FACET_ON_CELL_FUNCTION_H

// $LICENSE

#include "Gral/Base/grid-functions.h"

namespace GrAL {

/*! \brief Associate data which each cell-facet incidence

    \ingroup gridfunctions
    
    This class can be used e.g. for 
    \link cell_neighbor_search cell-cell neighborhood calculations. \endlink 


    \todo The implementation contains some overhead in terms of storage:
    The grid function on archetype cells (i.e. cell facets) does not
    need a grid pointer. It could be optimized for grids with archetypes which have
    contiguous cell handles, and in particular for grids with a single archetype.
    \todo Similar classes could be designed for cell-vertex and cell-edge
    incidences.
 */
template<class GRID, class T>
class facet_on_cell_function 
{
  typedef GRID                  grid_type;
  typedef grid_types<grid_type> gt;
  typedef typename gt::Cell                Cell;
  typedef typename gt::archgt::Cell        ArchCell;
  typedef typename gt::FacetOnCellIterator FacetOnCellIterator;

 grid_function<Cell, grid_function<ArchCell,T> > gf;

public:
  facet_on_cell_function() {}
  facet_on_cell_function(grid_type const& g) :  gf(g) 
    {
      for(typename gt::CellIterator c(gf.TheGrid()); ! c.IsDone(); ++c)
	gf[*c].set_grid((*c).TheArchetype());
    }
  facet_on_cell_function(grid_type const& g, T const& t) :  gf(g) 
    {
      for(typename gt::CellIterator c(gf.TheGrid()); ! c.IsDone(); ++c)
	gf[*c] = grid_function<ArchCell,T>(TheGrid().ArchetypeOf(*c),t);
    }

  T const& operator()(FacetOnCellIterator const& fc) const
  { return gf(fc.TheCell())(fc.ArchetypeCell());}

  T      & operator[](FacetOnCellIterator const& fc) 
  { return gf[fc.TheCell()][fc.ArchetypeCell()];}

  grid_type const& TheGrid() const { return gf.TheGrid();}
};


} // namespace GrAL 

#endif
