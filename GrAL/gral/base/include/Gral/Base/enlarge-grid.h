#ifndef GRAL_BASE_GB_ENLRARGE_GRID_H
#define GRAL_BASE_GB_ENLRARGE_GRID_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
//   The functions EnlargeGrid[...] attach a copy of srcG to G.
//
//   CONTENTS:
//   ---------
//    [1] void EnlargeGridV() 
//    [2] void EnlargeGridVC()
//
//   PARAMETERS:
//   -----------
//   * G1: This template is specialized for concrete types of G1.
//   * G2, Geom2: same as for ConstructGrid family.
//   * Iv gives a rule which vertices are to be identified: 
//     Iv is a injective mapping  V_h(G_src) -|-> V_h(G)
//
//   PRECONDITIONS:
//   --------------
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
// POSTCONDITIONS:
// ---------------
//
//   The following postconditions will hold:
//      G' is the value of G after execution, and 
//      G' = G  \cup G_src
//      |C(G')| = |C(G)| + |C(G_src)|, 
//      |V(G')| = |V(G)| + |V(G_src)| - |dom(Iv)|
//
//
//----------------------------------------------------------------

/*
template<class G1, class G2, class Geom2, class VertexIdent>
extern
void
EnlargeGrid(G1               & destG,     // in/out
	    G2          const& srcG,      // in
	    Geom2       const& srcGeom,   // in
	    VertexIdent const& Iv);       // in

template<class G1, class G2, class Geom2, class VertexIdent,
         class VertexMap, class CellMap>
extern
void
EnlargeGridVC(G1               & destG,       // in/out
	      G2          const& srcG,        // in
	      Geom2       const& srcGeom,     // in
	      VertexIdent const& Iv,          // in  src -> dest
	      VertexMap        & VertexCorr,  // out src -> dest
	      CellMap          & CellCorr);   // out src -> dest

*/
#endif
