#ifndef NMWR_GB_ENUMERATED_SUBRANGE_C
#define NMWR_GB_ENUMERATED_SUBRANGE_C

// $LICENSE


#include "Gral/Subranges/enumerated-subrange.h"


//----------------------------------------------------------------
//  ConstructSubrangeFromCells
//
//  This template function constructs an enumerated subrange 
//  over a Grid G from  a set of cells described by the range of
//  CellIt Cit.
//----------------------------------------------------------------


  
template<class Range, class CellIt>
void ConstructSubrangeFromCells
(Range& R,   // out: the subrange to be constructed
 //  const Grid&         G,          // in : base  grid
 CellIt              Cit)        // in : range(Cit) is the cell subrange 
{
  typedef typename Range::grid_type         grid_type;
  typedef grid_types<grid_type>             gt;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  //FIXME:  not 100% ok, R should contain valid ref. to Grid ... 
  if(Cit.IsDone())
    return;
  grid_type const& G(Cit.TheGrid());

  R = Range(G); //enumerated_subrange<Grid>(G); // empty subrange

  ENSURE( (R.FirstCell().IsDone()), "range not empty on construction!\n",1);

  partial_grid_function<Vertex,bool> visited(G,false);
  while(! Cit.IsDone()) {
    Cell C(*Cit);
    R.append_cell(C.handle());
    for(VertexOnCellIterator vc = C.FirstVertex(); ! vc.IsDone(); ++vc) {
      if(!visited(*vc)) {
	visited[*vc] = true;
	R.append_vertex(vc.handle());
      }
    }
    ++Cit;
  }

  
}

#endif
