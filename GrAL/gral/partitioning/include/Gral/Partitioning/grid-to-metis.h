#ifndef NMWR_GB_GRID_TO_METIS_H
#define NMWR_GB_GRID_TO_METIS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iostream>

/*! \file
 */ 

namespace GrAL {

/*! \brief Write a grid in Metis graph format, 
    where cells correspond to graph nodes and  facets to graph edges
    \ingroup metispartitioning

    \templateparams
     - GRID:
      -  type grid_types<GRID>  gt;
      -  type gt::Cell;
      -  type gt::Facet;             
      -  type gt::CellIterator;      
      -  type gt::CellOnCellIterator;
      -  type grid_function<Cell,int>;

 */
template<class GRID>
extern void write_metis_cell_graph(GRID const& G, 
                                   std::ostream& out);

/*! \brief Write a grid in Metis graph format, 
    where cells correspond to graph nodes and  facets to graph edges
    \ingroup metispartitioning

   This function also writes weights on edges.
   
   \templateparams
   In addition to \ref write_metis_cell_graph:
    - FACETFCT:
      - value_type operator()(gt::Facet)
      - value_type convertible to int
      - weights(F) is an integer > 0 \f$ \forall F \in G \f$
*/

template<class GRID, class FACETFCT>
extern void write_metis_cell_graph_edge_weights(GRID const& G, 
                                           FACETFCT const& weights,
					   std::ostream& out);


/*! \brief Write a grid in Metis mesh format.
    \ingroup metispartitioning 

   G must consist entirely of triangles OR entirely of quads.
   Else use write_metis_graph.

   \templateparams
    - type grid_types<GRID> gt;
    - type gt::Cell
    - type gt::CellIterator
    - type gt::Facet
    - type gt::CellOnCellIterator;
 */
template<class GRID>
extern int write_metis_mesh(GRID const& G, std::ostream& out);


/*! \defgroup metispartitioninginternal Internal functions for Metis Partitioning Module
    \ingroup   metispartitioning
 */


/*! \brief Calculate the size for Metis vector xadj
    \ingroup metispartitioninginternal
 */
template<class GRID>
extern int metis_cell_graph_size_of_xadj(GRID const& G);

/*! \brief Calculate the size for Metis vector adjncy 
    \ingroup metispartitioninginternal

    \see metis_cell_graph_format
 */
template<class GRID>
extern int metis_cell_graph_size_of_adjncy(GRID const& G);



/*! \brief Copy the grid into Metis mesh vectors xadj and adjncy
    \ingroup metispartitioninginternal

    Cells correspond to graph vertices, and facets to graph edges.
    \templateparams
     - type grid_types<GRID> gt;
     - type gt::Cell
     - type gt::CellIterator
     - type gt::Facet
     - type gt::CellOnCellIterator;
 */
template<class GRID>
extern void metis_cell_graph_format(GRID const& G, 
                                    int * xadj,   int * vwgt,
                                    int * adjncy, int * adjwgt);

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/grid-to-metis.C"
#endif

#endif
