#ifndef NMWR_GB_COMPLEX2D_CONSTRUCT_C
#define NMWR_GB_COMPLEX2D_CONSTRUCT_C


// $LICENSE


#include <map> // STL

#include "Gral/Grids/Complex2D/internal/copy.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Base/grid-morphism.h"

#include "Container/partial-mapping.h"
#include "Container/dummy-mapping.h"

//----------------------------------------------------------------
//
//  Copy-construct a Complex2D from a right-hand side (srcG)
//  that allows
//  - iteration over vertices ( VertexIterator | FirstVertex() )
//  - iteration over cells    ( CellIterator   | FirstCell() )
//  - iteration over cell-vertices (VertexOnCellIterator | Cell::FirstVertex() )
//  and a geometry (geom) that gives coordinates for each vertex.
//  The additional connectivity information is then calculated
//  (that is, neighbors of cells and adjacent cells of vertices)
//
//  This algorithm is essentially the same for reading a file from
//  disk (storing vertex coordinates and indices of vertices for each cell)
//  or, more general, for constructing the grid from a serialized representation
//  (e.g. in message passing).
//  Perhaps the best way to exploit this would be to write a grid mask
//  for these serial versions, using one single template method for
//  the copy-construction. This could then also be used for Enlarge(...)
//  and future similar functions.
//----------------------------------------------------------------


//----------------------------------------------------------------
// Definitions of the partial specializations of template version
// from Grid/Algorithms
//----------------------------------------------------------------

template<class Geom1, class G2, class Geom2>
void 
ConstructGrid(Complex2D  & destG,
              Geom1      & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom)
{ Construct_complex2d(destG,destGeom, srcG,srcGeom); }

template<class Geom1, class G2, class Geom2, class VertexMap>
void 
ConstructGridV(Complex2D     & destG, 
               Geom1         & destGeom,
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1)
{ Construct_complex2d(destG,destGeom, srcG,srcGeom,VCorrG2_G1);}

template<class Geom1, class G2, class Geom2, class VertexMap, class CellMap>
void 
ConstructGridVC(Complex2D     & destG, 
                Geom1         & destGeom,
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1) 
{ Construct_complex2d(destG,destGeom, srcG,srcGeom,VCorrG2_G1, CCorrG2_G1);}

template<class G2, class VertexMap, class CellMap>
void 
ConstructGrid0(Complex2D     & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1)
{ Construct_complex2d0(destG,srcG,VCorrG2_G1,CCorrG2_G1);}


//----------------------------------------------------------------
//  Definitions of the Complex2D versions.
//----------------------------------------------------------------

template<class Geom1, class G2, class Geom2>
void Construct_complex2d(Complex2D& CC, 
                         Geom1         & destGeom,
                         G2       const& srcG,
                         Geom2    const& srcGeom)
{ 
  typedef grid_types<G2>                        source_gt;
  typedef typename source_gt::vertex_handle     src_vertex_handle;
  typedef grid_types<Complex2D>::vertex_handle  vertex_handle;

  // this should be replaced by an array if possible.
  // map also lacks an operator() const  |  operator[] const 
  //vertex_morphism<G2,Complex2D> VertexCorr(srcG,CC);
  partial_mapping<src_vertex_handle,vertex_handle> VertexCorr;
  Construct_complex2d(CC,destGeom,srcG,srcGeom,VertexCorr);
}

template<class Geom1, class G2, class Geom2, class VertexMap>
void Construct_complex2d(Complex2D     & CC, 
                         Geom1         & destGeom,
                         G2       const& srcG,
                         Geom2    const& srcGeom,
                         VertexMap     & VertexCorr) // maps src::handles to Complex2D::handles
{

  typedef grid_types<G2>                      source_gt;
  typedef typename source_gt::cell_handle     src_cell_handle;
  typedef grid_types<Complex2D>::cell_handle  cell_handle;

  // here we are not interested in a cell correspondance
  dummy_mapping<src_cell_handle,cell_handle> CellCorr;

  Construct_complex2d(CC,destGeom,srcG,srcGeom,VertexCorr,CellCorr);
}


template<class Geom1, class G2, class Geom2, class VertexMap, class CellMap>
void Construct_complex2d(Complex2D     & CC, 
                         Geom1         & destGeom,
                         G2       const& srcG,
                         Geom2    const& srcGeom,
                         VertexMap     & VertexCorr, // maps src::handles to
                         CellMap       & CellCorr)   // complex2d::handles
{
  // construct combinatorial grid
 Construct_complex2d0(CC,srcG,VertexCorr,CellCorr);

 // copy geometry

 destGeom.set_grid(CC);
 typedef typename grid_types<G2>::VertexIterator src_vertex_it;
 for(src_vertex_it src_v = srcG.FirstVertex(); ! src_v.IsDone(); ++src_v) {
   //   assign_point(cc.coord(CC.vertex(VertexCorr(src_v.handle()))) , geom.coord(*src_v));
   assign_point(destGeom.coord(CC.vertex(VertexCorr(src_v.handle()))) , srcGeom.coord(*src_v));
   
 }

}

template<class G2, class VertexMap, class CellMap>
void Construct_complex2d0(Complex2D     & CC, 
                          G2       const& srcG,
                          VertexMap     &  VertexCorr, // maps src::handles to
                          CellMap       &  CellCorr)   // complex2d::handles

{
  typedef grid_types<G2> source_gt;

  //  typedef typename source_gt::vertex_handle   src_v_index;
  typedef typename source_gt::cell_handle          src_cell_handle;
  typedef typename source_gt::CellIterator         src_cell_it;
  typedef typename source_gt::VertexIterator       src_vertex_it;
  typedef typename source_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef typename Complex2D::Cell      Cell;
  typedef typename Complex2D::cell_handle   cell_handle;
  typedef typename Complex2D::vertex_handle vertex_handle;

  friend_for_input cc(CC); // template routines cannot be friends,
                           // so we use a dirty trick

  cc.clear();

  //---  construct vertices ---------------------------
  
  // cc._vertices().reserve(Connect.NumOfVertices());
  for(src_vertex_it src_v = srcG.FirstVertex(); ! src_v.IsDone(); ++src_v) {
    VertexCorr[src_v.handle()] = cc._new_vertex(); // geom.coord(*src_v));
  }
 

  //---- construct cells ------------------------------

  copy_cells(CC,srcG,VertexCorr,CellCorr);

  //---- construct additional adjaceny information ----------------------

  cc.calculate_vertex_cells();    // unordered list of adjacent cells per vertex
  cc.calculate_neighbour_cells(); // correct neighbour is calculated here
  // setup archetype info. NOTE: if the src G2 has archetype info, this should be used.
  if(CC.NumOfArchetypes() == 0)
    cc.calculate_archetypes();     
}

#endif
