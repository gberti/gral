#ifndef NMWR_GB_WRITE_COMPLEX2D_C
#define NMWR_GB_WRITE_COMPLEX2D_C



// $LICENSE

#include "Gral/IO/write-complex2d.h"

//-------------------------------------------------------------------------
//
// write_complex2d(GRID, GEOM, ostream&) - write a grid in complex2d format.
//
//
//-------------------------------------------------------------------------


template<class GRID>
void write_complex2d(GRID const& G,  ostream& out, int offset = 1)
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
    vnum++;
  }
  out << "\n";

  grid_function<Cell,int> CNum(baseG);
  int cnum = offset;
  CellIterator c;
  for(c = G.FirstCell(); !c.IsDone(); ++c){
   CNum[*c] = cnum;
   cnum++;
  }


  for(c = G.FirstCell(); !c.IsDone(); ++c){
    out	<< (*c).NumOfVertices() << "   ";
    for(VertexOnCellIterator vc = (*c).FirstVertex(); ! vc.IsDone(); ++vc)
      out << VNum[*vc] << " ";
    out << "\n";
  }
}





template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo, ostream& out)
{ write_complex2d(G,Geo,out,1); }

template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo, ostream& out, int offset)
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
    out << Geo.coord(*v) << "\n"; 
    VNum[*v] = vnum;
    vnum++;
  }
  out << "\n";

  grid_function<Cell,int> CNum(baseG);
  int cnum = offset;
  CellIterator c;
  for(c = G.FirstCell(); !c.IsDone(); ++c){
   CNum[*c] = cnum;
   cnum++;
  }


  for(c = G.FirstCell(); !c.IsDone(); ++c){
    out	<< (*c).NumOfVertices() << "   ";
    for(VertexOnCellIterator vc = (*c).FirstVertex(); ! vc.IsDone(); ++vc)
      out << VNum[*vc] << " ";
    out << "\n";
  }
}



#endif
