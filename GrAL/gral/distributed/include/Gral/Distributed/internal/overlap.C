#ifndef NMWR_GB_OVERLAP_C
#define NMWR_GB_OVERLAP_C

// $LICENSE

#include "Gral/Distributed/overlap.h"

template<class CoarseGrid, class FineGrid,
         template<class U> class OVLP_RANGES>
void overlap<CoarseGrid, FineGrid, OVLP_RANGES>::set_neighbours()
{
  neighbours.set_grid(TheCoarseGrid());
  for(VertexNbIterator VNb = FirstVertexNeighbour(); ! VNb.IsDone(); ++VNb)
    neighbours.push_back(*VNb);

  // add neighbours from cell ranges not already contained in neighbours
  for(CellNbIterator CNb = FirstCellNeighbour(); ! CNb.IsDone(); ++CNb) {
    bool found = false;
    for (NbIterator N = neighbours.FirstCell() ; ! N.IsDone(); ++N)
      if(*N == *CNb) {
	found = true;
	break;
      }
    if ( ! found) 
      neighbours.push_back(*CNb);
  }
}


template<class Overlap>
void write_ovlp(Overlap const& ovlp, std::ostream & out)
{
  out << "Total ranges\n";
  out << "vertices:\n";
  write_ovrge(ovlp.vertices(),out);
  //  out << "facets:\n";
  // write_ovrge(ovlp.facets(),  out);
  out << "cells:\n";
  write_ovrge(ovlp.cells(),   out);
  out << '\n';

  out << "Partial ranges:\n";

  out << "vertices:\n";
  typedef typename Overlap::VertexNbIterator VertexNbIterator;
  for(VertexNbIterator VNb = ovlp.FirstVertexNeighbour(); ! VNb.IsDone(); ++VNb) {
    out << "Neighbour " << ovlp.TheCoarseGrid().handle(*VNb) << '\n'; 
    write_ovrge(ovlp.vertices(*VNb), out);
  }
  
  /*
  out << "facets:\n";
  typedef typename Overlap::FacetNbIterator FacetNbIterator;
  for(FacetNbIterator FNb = ovlp.FirstFacetNeighbour(); ! FNb.IsDone(); ++FNb) {
    out << "Neighbour " << ovlp.TheCoarseGrid().handle(*FNb) << '\n'; 
    write_ovrge(ovlp.facets  (*FNb), out);
  }
  */

  out << "cells:\n";
  typedef typename Overlap::CellNbIterator CellNbIterator;
  for(CellNbIterator CNb = ovlp.FirstCellNeighbour(); ! CNb.IsDone(); ++CNb) {
    out << "Neighbour " << ovlp.TheCoarseGrid().handle(*CNb) << '\n'; 
    write_ovrge(ovlp.cells   (*CNb), out);
    out << '\n';
  }

}


#endif
