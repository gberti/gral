#ifndef NMWR_GB_ENUMERATED_SUBRANGE_C
#define NMWR_GB_ENUMERATED_SUBRANGE_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Subranges/enumerated-subrange.h"

namespace GrAL {



  template<class GRID, class GT>
  void enumerated_subrange<GRID, GT>::init() const
{
  if(! initialized) {
    initialized = true;
    if(! cells.empty()) {
      const_cast<self *>(this)->vertices.clear();
      cell_range_ref<GRID,cell_sequence> cell_wrapper(0, cells.end()-cells.begin(), cells, TheGrid());
      ConstructVertexRange(cell_wrapper,
			   const_cast<self *>(this)->vertices);
    }
  }
}


  template<class GRID, class GT>
template<class CELLIT>
  void enumerated_subrange<GRID,GT>::construct(CELLIT c) 
{
  REQUIRE_ALWAYS(cells.empty(), "enumerated_subrange<GRID>::construct: cells must be empty!",1);
  while(! c.IsDone()) {
    append_cell(c.handle());
    ++c;
  }
  init();
  init_counts();
}

template<class CELLRANGE, class VERTEXRANGE>  
void ConstructVertexRange(CELLRANGE const& CR, 
			  VERTEXRANGE    & VR)
{
  typedef typename CELLRANGE::grid_type     grid_type;
  typedef grid_types<grid_type>             gt;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  partial_grid_function<Vertex,bool> visited(CR.TheGrid(),false);
  for(typename CELLRANGE::CellIterator c=CR.FirstCell(); ! c.IsDone(); ++c) {
    for(VertexOnCellIterator vc = GrAL::begin_x(*c); ! vc.IsDone(); ++vc) {
      if(!visited(*vc)) {
	visited[*vc] = true;
	VR.push_back(vc.handle());
      }
    }
  
  }
}

template<class Range, class CellIt>
void ConstructSubrangeFromCells
(Range& R,   // out: the subrange to be constructed
 //  const Grid&         G,          // in : base  grid
 CellIt              Cit)        // in : range(Cit) is the cell subrange 
{
  typedef typename CellIt::grid_type        grid_type;
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
    for(VertexOnCellIterator vc = GrAL::begin_x(C); ! vc.IsDone(); ++vc) {
      if(!visited(*vc)) {
	visited[*vc] = true;
	R.append_vertex(vc.handle());
      }
    }
    ++Cit;
  }

  
}

}  // namespace GrAL 

#endif
