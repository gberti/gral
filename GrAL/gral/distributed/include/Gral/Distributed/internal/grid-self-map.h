#ifndef NMWR_GB_GRID_SELF_MAP_H
#define NMWR_GB_GRID_SELF_MAP_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Base/common-grid-basics.h"
#include "Container/bijective-mapping.h"

//----------------------------------------------------------------
//
// template<class Grid> class GridSelfMap;
// class for mapping the grid elements of a grid G onto themselves.
// This defaults to the identity mapping.
//
//----------------------------------------------------------------

template<class Grid>
class GridSelfMap {
public:
  typedef Grid             grid_type;
  typedef grid_types<Grid> gt;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::Vertex        Vertex;
  typedef typename gt::facet_handle  facet_handle;
  typedef typename gt::Facet         Facet;
  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Cell          Cell;
private:
  const grid_type* the_grid;

  typedef bijective_mapping<vertex_handle, vertex_handle> vmap_type;
  typedef bijective_mapping<facet_handle, facet_handle>   fmap_type;
  typedef bijective_mapping<cell_handle, cell_handle>     cmap_type;
  bijective_mapping<vertex_handle, vertex_handle> vertex_map;
  bijective_mapping<facet_handle,  facet_handle>  facet_map;
  bijective_mapping<cell_handle,   cell_handle>   cell_map;

public:
  GridSelfMap(const grid_type& g) : the_grid(&g) {}

  grid_type const& TheGrid() const { return (*the_grid);}

  vmap_type &  TheVertexMap() { return vertex_map;}
  fmap_type &  TheFacetMap()  { return facet_map;}
  cmap_type &  TheCellMap()   { return cell_map;}

  Vertex operator() (const Vertex& V) const 
    { 
      return ( vertex_map.defined(TheGrid().handle(V)) ? 
	       TheGrid().vertex(vertex_map(TheGrid().handle(V)))
	       : V);
	}
  Facet operator() (const Facet& F) const 
    { 
      return ( facet_map.defined(TheGrid().handle(F)) ? 
	       TheGrid().facet(facet_map(TheGrid().handle(F)))
	       : F);
	}
  Cell operator() (const Cell& C) const 
    { 
      return ( cell_map.defined(TheGrid().handle(C)) ? 
	       TheGrid().cell(cell_map(TheGrid().handle(C)))
	       : C);
	}
  
};

#endif
