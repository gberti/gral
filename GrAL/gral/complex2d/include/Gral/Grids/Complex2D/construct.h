#ifndef NMWR_GB_COMPLEX2D_CONSTRUCT_H
#define NMWR_GB_COMPLEX2D_CONSTRUCT_H


// $LICENSE


#include "Gral/Base/construct-grid.h"

#include "Gral/Grids/Complex2D/complex2d.h"

//----------------------------------------------------------------
// copy-construct a Complex2D <dest> from another 2D grid <source_grid>
// supporting 
//  - grid_types<>, 
//  - CellIterator, VertexIterator, VertexOnCellIterator,
//  - vertex_handle
// This template is defined in generic/complex2d-construct.C
//----------------------------------------------------------------

template<class Geom1, class G2, class Geom2>
extern void 
Construct_complex2d(Complex2D& CC, 
                    Geom1         & destGeom,
                    G2       const& srcG,
                    Geom2    const& srcGeom);

template<class Geom1, class G2, class Geom2, class VertexMap>
extern void
Construct_complex2d(Complex2D     & CC, 
                    Geom1         & destGeom,
                    G2       const& srcG,
                    Geom2    const& geom,
                    VertexMap     &  VertexCorr);
// VertexCorr  maps src::vertex_handles to  complex2d::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class Geom1, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
Construct_complex2d(Complex2D     & CC, 
                    Geom1         & destGeom,
                    G2       const& srcG,
                    Geom2    const& srcGeom,
                    VertexMap     & VertexCorr,  // maps src::handles to
                    CellMap       & CellCorr);   // complex2d::handles


// construct without geometry

template<class G2, class VertexMap, class CellMap>
extern void 
Construct_complex2d0(Complex2D     & CC, 
                     G2       const& srcG,
                     VertexMap     &  VertexCorr,  // maps src::handles to
                     CellMap       &  CellCorr);   // complex2d::handles


//----------------------------------------------------------------
//  Partial specializations of the templates in 
//  Grids/Algorithms/construct-grid.h
//----------------------------------------------------------------

template<class Geom1, class G2, class Geom2>
extern  void 
ConstructGrid(Complex2D  & destG,
              Geom1      & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);

template<class Geom1, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(Complex2D     & destG, 
               Geom1         & destGeom,
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 
// VertexCorr  maps G2::vertex_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class Geom1, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(Complex2D     & destG, 
                Geom1         & destGeom,
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 
// CellCorr  maps G2::cell_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Complex2D     & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 

// for inclusion model

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/complex2d-construct.C"
#endif

#endif
