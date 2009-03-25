#ifndef NMWR_GB_GRID_TO_MPI_GRAPH_H
#define NMWR_GB_GRID_TO_MPI_GRAPH_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

//----------------------------------------------------------------
//
// CONTENTS:
// ---------
// [1]  template<class Grid> 
//      void mpi_graph_format(Grid,int* index, int* index_end, 
//              		   int* edges, int* edges_end);
// [1b] template<class Grid>
//      void mpi_graph_size_of_index_array(const Grid& G);
// [1c] template<class Grid>
//      void mpi_graph_size_of_edge_array(const Grid& G);
// DESCRIPTION
// -----------
// [1] write the cell-ceighbor-graph of a grid G into the format
//     understood be MPI_Graph_create(...).
//
//----------------------------------------------------------------

template<class Grid>
int mpi_graph_size_of_index_array(const Grid& G);

template<class Grid>
int mpi_graph_size_of_edges_array(const Grid& G);

template<class Grid>
void mpi_graph_format(const Grid& G, 
		      int* index, int* index_end, 
		      int* edges, int* edges_end);


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/grid-to-mpi-graph.C"
#endif


#endif 
