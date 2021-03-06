#ifndef NMWR_GB_WRITE_COMPLEX2D_C
#define NMWR_GB_WRITE_COMPLEX2D_C




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/IO/write-complex2d.h"
#include "Container/dummy-mapping.h"

#include <iomanip>
#include <limits>

namespace GrAL {

//-------------------------------------------------------------------------
//
// write_complex2d(GRID, GEOM, ostream&) - write a grid in complex2d format.
//
//
//-------------------------------------------------------------------------


template<class GRID>
void write_complex2d(GRID const& G,   ::std::ostream& out, int offset)
{ 
  typedef grid_types<GRID>                  gt;
  dummy_mapping<typename gt::vertex_handle, int> G2Out_v;
  dummy_mapping<typename gt::cell_handle,   int> G2Out_c;
  write_complex2d(G,out,offset, G2Out_v, G2Out_c);
}

template<class GRID, class VCORR, class CCORR>
void write_complex2d(GRID const& G,   ::std::ostream& out, 
                     VCORR & G2Out_v, CCORR & G2Out_c)
{ write_complex2d(G,out,0,G2Out_v, G2Out_c); }

template<class GRID, class VCORR, class CCORR>
void write_complex2d(GRID const& G,   ::std::ostream& out, int offset, 
                     VCORR & G2Out_v, CCORR & G2Out_c)
{
  typedef grid_types<GRID>                  gt;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::VertexIterator       VertexIterator;
  typedef typename gt::CellIterator         CellIterator;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  out << G.NumOfVertices() << " " << G.NumOfCells() << "\n\n";

  // if GRID is a subrange type, we cannot initialized grid-functions
  // directly with G, but must get hold of the underlying grid.
  // This solution is not optimal. There could be a c-t branch
  // with an additional grid-type parameter for grid-functions.
  typedef element_traits<Vertex> et;
  typedef typename et::grid_type base_grid_type; // possibly != GRID
  Vertex dummy(*(G.FirstVertex()));
  base_grid_type const& baseG(dummy.TheGrid());

  grid_function<Vertex,int> VNum(baseG);
  int vnum = offset;
  for(VertexIterator v= G.FirstVertex(); ! v.IsDone(); ++v){
    //    out << Geo.coord(*v) << "\n"; 
    VNum[*v] = vnum;
    G2Out_v[v.handle()] = vnum;
    vnum++;
  }
  out << "\n";

  int cnum = offset;
  for(CellIterator c = GrAL::begin_x(G); !c.IsDone(); ++c){
    out	<< (*c).NumOfVertices() << "   ";
    G2Out_c[c.handle()] = cnum; ++cnum;
    for(VertexOnCellIterator vc = GrAL::begin_x(*c); ! vc.IsDone(); ++vc)
      out << VNum[*vc] << " ";
    out << "\n";
  }
}





template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo,  ::std::ostream& out)
{ write_complex2d(G,Geo,out,1); }

template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo,  ::std::ostream& out, int offset)
{
  typedef grid_types<GRID>                  gt;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::VertexIterator       VertexIterator;
  typedef typename gt::CellIterator         CellIterator;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  out << G.NumOfVertices() << " " << G.NumOfCells() << "\n\n";

  // if GRID is a subrange type, we cannot initialized grid-functions
  // directly with G, but must get hold of the underlying grid.
  // This solution is not optimal. There could be a c-t branch
  // with an additional grid-type parameter for grid-functions.
  typedef element_traits<Vertex> et;
  typedef typename et::grid_type base_grid_type; // possibly != GRID
  Vertex dummy(*(G.FirstVertex()));
  ref_ptr<base_grid_type const> baseG(dummy.TheGrid());

  grid_function<Vertex,int> VNum(* baseG);
  int vnum = offset;
  for(VertexIterator v= G.FirstVertex(); ! v.IsDone(); ++v){
    out << std::setprecision(std::numeric_limits<double>::digits10) << Geo.coord(*v) << "\n"; 
    VNum[*v] = vnum;
    vnum++;
  }
  out << "\n";


  for(CellIterator c = G.FirstCell(); !c.IsDone(); ++c){
    out	<< GrAL::size<Vertex>(*c) << "  ";
    for(VertexOnCellIterator vc = GrAL::begin<Vertex>(*c); ! vc.IsDone(); ++vc)
      out << VNum[*vc] << " ";
    out << "\n";
  }
}

} // namespace GrAL 

#endif
