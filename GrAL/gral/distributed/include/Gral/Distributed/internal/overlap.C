#ifndef NMWR_GB_OVERLAP_C
#define NMWR_GB_OVERLAP_C


// $LICENSE

#include "Container/some-algorithms.h"
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


template<class SrcRange, class DestRange, class Filter>
void copy_overlap_ranges(const SrcRange& src, DestRange& dest, const Filter& f)
{
  dest.TheContainer().reserve(src.size());
  copy_overlap_ranges(src.privee(), src.exposed(), src.shared(), src.copied(),
		      dest, f);
}

template<class SrcRange, class DestRange, class Filter>
void copy_overlap_ranges(const SrcRange& priv, const SrcRange& exp,
			 const SrcRange& shd,  const SrcRange& cop,
			 DestRange& dest,const Filter& f)
{
  copy_filter(priv.begin(),priv.end(), back_inserter(dest), f);
  dest.set_exposed_begin();
  copy_filter(exp.begin(),exp.end(), back_inserter(dest), f);
  dest.set_shared_begin();
  copy_filter(shd.begin(),shd.end(), back_inserter(dest), f);
  dest.set_copied_begin();
  copy_filter(cop.begin(),cop.end(), back_inserter(dest), f);
  // dest.set_copied_end();
}

template<class SrcRange, class DestRange, class Filter>
void copy_overlap_ranges(                      const SrcRange& exp,
			 const SrcRange& shd,  const SrcRange& cop,
			 DestRange& dest,const Filter& f)
{ 
  SrcRange priv(exp.TheGrid());
  ENSURE( priv.empty() , " priv is not empty!",1);
  copy_overlap_ranges(priv, exp,shd,cop,dest,f);
}









template<class Overlap>
void write_ovlp(Overlap const& ovlp, ostream & out)
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
