#ifndef GRAL_GB_GRAPHS_BASE_H
#define GRAL_GB_GRAPHS_BASE_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"

namespace GrAL { namespace graph {
  /*! \defgroup gral_grid_graphs  Viewing grids as graphs 
  
  Grids can be viewed as graphs in a number of different ways.
  For instance, we can see all grid elements as graph vertices,
  and define edges by the incidence relationship.

  A very common and important possibility are \ref cell_neighbor_graph .
  Here cells are the graph vertices, and facets joining two cells are the graph edges.
 
  */

  struct cell_neighbor_graph_tag {};

  struct directed_graph_tag   {};
  struct undirected_graph_tag {};
  struct reverse_map_tag {};
  struct vertex_index_map_tag {};

  template<class GRID, class GRID_AS_GRAPH_TAG, class DIRECTED_TAG, class GT = grid_types<GRID> > 
  struct graph_view {};

  template<class E, class T, class GRAPH, class TAG>
  class vertex_property_map {};

  template<class E, class T, class GRAPH, class TAG>
  class edge_property_map {};


}} // namespace GrAL {  namespace graph

#endif
