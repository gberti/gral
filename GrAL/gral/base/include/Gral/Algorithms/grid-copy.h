#ifndef NMWR_GB_GRID_COPY_ALG_H
#define NMWR_GB_GRID_COPY_ALG_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"

// OBSOLETE. DO NOT USE.
// this requires that G1 has been constructed from G2
// and hence corresponding cells come at the same position
// in the cell sequences
// The usefulness of this routine is questionable.
template<class G1, class G2, class CCorr12>
inline
void ConstructCellCorrespondence(G1     const& g1,    // in
				 G2     const& g2,    // in
				 CCorr12     & ccorr) // out
{
  typedef typename grid_types<G1>::CellIterator CellIt1;
  typedef typename grid_types<G2>::CellIterator CellIt2;

  CellIt1 C1 = g1.FirstCell();
  CellIt2 C2 = g2.FirstCell();
 
  for(; ! C1.IsDone(); ++C1, ++C2)
    ccorr[C1.handle()] = C2.handle();
}


// dest = src modulo src2dest
template<class GFsrc, class GFdest, class ElemCorr>
void copy_grid_function(GFsrc    const& src,       // in 
			GFdest        & dest,      // out
			ElemCorr const& src2dest)  // in
{
  //  typedef typename GFsrc::grid_type  Gsrc;
  // typedef typename GFdest::grid_type Gdest;
  //typedef typename grid_types<Gsrc>  gtsrc;
  //typedef typename grid_types<Gdest> gtdest;

  typedef typename GFsrc::element_type     Esrc;
  typedef typename GFdest::element_type    Edest;
  typedef element_traits<Esrc>             etsrc;
  typedef element_traits<Edest>            etdest;
  typedef typename etsrc::ElementIterator  EItsrc;
  typedef typename etdest::ElementIterator EItdest;

  EItsrc  e_src  = etsrc::FirstElement(src.TheGrid());
  //  EItdest e_dest = etdest::FirstElement(dest.TheGrid());

  while( ! e_src.IsDone()) {
    dest[src2dest(src.TheGrid().handle(*e_src))] = src(*e_src); 
    // this works only if gr::op[](element_handle) is defined!
    ++e_src; 
  }
}

#endif
