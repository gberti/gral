#ifndef NMWR_GB_ENLARGE_GRID_COMPLEX2D_H
#define NMWR_GB_ENLARGE_GRID_COMPLEX2D_H


// $LICENSE

#include "Gral/Base/enlarge-grid.h"
#include "Gral/Grids/Complex2D/complex2d.h"

//----------------------------------------------------------------
//
//   The function EnlargeGrid attaches a copy of G_src
//   to G, where Iv gives a rule which vertices are 
//   to be identified: 
//   Iv is a injective mapping  V_h(G_src) -|-> V_h(G)
//
//   The following preconditions must hold:
//     G_src \neq \emptyset (contains at least one cell)
//     dom(Iv) \subset V_h(Bd(G_src))
//     ran(Iv) \subset V_h(Bd(G))
//    or more specifically:
//     \exists F_1, ... ,F_k \in F(Bd(G)) : 
//         ran(Iv) = \union_{i=1}^k V(F_i)
//     \exists F_1, ... ,F_k \in F(Bd(G_src)) :
//         dom(Iv) = \union_{i=1}^n V(F_i)
//     Iv may be empty, in which case the result will not be connected.
//
//   The following postconditions will hold:
//      G' is the value of G after execution, and 
//      G' = G  \cup G_src
//      |C(G')| = |C(G)| + |C(G_src)|, 
//      |V(G')| = |V(G)| + |V(G_src)| - |dom(Iv)|
//----------------------------------------------------------------


template<class Conn, class Geom, class VertexIdent>
void EnlargeGrid(Complex2D& G,                 // in/out
		 const Conn& G_src,            // in
		 const Geom& Geo_src,          // in
		 const VertexIdent& Iv);       // in


template<class Conn, class Geom, class VertexIdent, class VertexMap, class CellMap>
void EnlargeGrid(Complex2D& G,                 // in/out
		 const Conn& G_src,            // in
		 const Geom& Geo_src,          // in
		 const VertexIdent& Iv,        // in src -> dest
		 VertexMap& VertexCorr,        // out
		 CellMap  & CellCorr);         // out

/*
template<class G2, class Geom2, class VertexIdent>
void EnlargeGrid(Complex2D        & G,        // in/out
		 G2          const& srcG,     // in
		 Geom2       const& srcGeom,  // in
		 VertexIdent const& Iv);      // in
*/

template<class G2, class Geom2, class VertexIdent,
         class VertexMap, class CellMap>
extern
void
EnlargeGridVC(Complex2D        & destG,       // in/out
	      G2          const& srcG,        // in
	      Geom2       const& srcGeom,     // in
	      VertexIdent const& Iv,          // in  src -> dest
	      VertexMap        & VertexCorr,  // out src -> dest
	      CellMap          & CellCorr);   // out src -> dest


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/complex2d-enlarge.C"
#endif


#endif
