#ifndef NMWR_GB_OVERLAP_C
#define NMWR_GB_OVERLAP_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Container/algorithms.h"
#include "Grids/Distributed/overlap.h"

template<class CoarseGrid, class FineGrid>
void overlap<CoarseGrid, FineGrid>::set_neighbours()
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


//template<class Ovlp1, class Ovlp2, class FineVCorr, class FineCCorr>
template<class CG1, class FG1, class Ovlp2,
         class CoarseCCorr, class FineVCorr, class FineCCorr>
void CopyOverlap(overlap<CG1,FG1>       &  dest,            // out
		 Ovlp2             const&  src,             // in
		 CG1               const&  cg_dest,         // in
		 CoarseCCorr       const&  crs_src2dest_c,  // in 
		 FG1               const&  fg_dest,         // in
		 FineVCorr         const&  src2dest_v,      // in 
		 FineCCorr         const&  src2dest_c)      // in
{
  typedef overlap<CG1,FG1> Ovlp1;

  dest.set_coarse_grid(cg_dest);
  dest.set_fine_grid(fg_dest);
  
  copy_overlap_ranges(src.vertices(),dest.vertices(),src2dest_v);
  copy_overlap_ranges(src.cells(),   dest.cells(),   src2dest_c);
  
  // NOTE: dest.neighbour-iteration does not yet work!
  typedef typename Ovlp1::CoarseCell        destCoarseCell;
  typedef typename Ovlp2::CellNbIterator    srcCellNbIterator;
  typedef typename Ovlp2::VertexNbIterator  srcVertexNbIterator;

  for(srcCellNbIterator sCNb = src.FirstCellNeighbour(); ! sCNb.IsDone(); ++sCNb) {
    destCoarseCell dNb = cg_dest.cell(crs_src2dest_c(src.TheCoarseGrid().handle(*sCNb)));
    copy_overlap_ranges(src.cells   (*sCNb),dest.cells   (dNb),src2dest_c);
    
  }
  for(srcVertexNbIterator sVNb = src.FirstVertexNeighbour(); ! sVNb.IsDone(); ++sVNb) {
    destCoarseCell dNb = cg_dest.cell(crs_src2dest_c(src.TheCoarseGrid().handle(*sVNb)));
    copy_overlap_ranges(src.vertices(*sVNb),dest.vertices(dNb),src2dest_v);
  }

  dest.set_neighbours();
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
