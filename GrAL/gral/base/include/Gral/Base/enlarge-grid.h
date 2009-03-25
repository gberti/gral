#ifndef GRAL_BASE_GB_ENLRARGE_GRID_H
#define GRAL_BASE_GB_ENLRARGE_GRID_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



/*! \file
 */

namespace GrAL {

/*! \defgroup enlargeoperations Generic grid enlargement operations

    \brief  Attach (glue) a copy of \c srcG to \c G, identifying a given set of vertices.
    \ingroup mutatingoperations
    \see module mutatingoperations

   This file does \e not contain any actual function; rather, the following  functions
   have to be implemented separately for each grid type \c G1:

    - <tt> void EnlargeGrid()  </tt> 
    - <tt> void EnlargeGridV()  </tt> 
    - <tt> void EnlargeGridVC() </tt>

     \code
      template<class G1, class G2, class Geom2, class VertexIdent>
      void
      EnlargeGrid(G1               & destG,     // in/out
	          G2          const& srcG,      // in
	          Geom2       const& srcGeom,   // in
	          VertexIdent const& Iv);       // in

       template<class G1, class G2, class Geom2, class VertexIdent,
                class VertexMap, class CellMap>
       void
       EnlargeGridVC(G1               & destG,       // in/out
        	     G2          const& srcG,        // in
	             Geom2       const& srcGeom,     // in
	             VertexIdent const& Iv,          // in  src -> dest
	             VertexMap        & VertexCorr,  // out src -> dest
	             CellMap          & CellCorr);   // out src -> dest
      \endcode 

   <b> Template parameters: </b>
    - G1: The functions are overloaded for concrete types of G1.
    - G2, Geom2: same as for ConstructGrid family.
    - Iv gives a rule which vertices are to be identified: <BR>
      Iv is a injective mapping  \f$ V(srcG) \mapsto V(G) \f$

   \b Preconditions:
 
   The following preconditions must hold:
     - \f$ G_{src} \neq \emptyset \f$ (contains at least one cell)
     - \f$ \dom(Iv) \subset V(\Bd(G_{src})) \f$
     - \f$ \ran(Iv) \subset V(\Bd(G)) \f$

    or more specifically:

    - \f$ \exists F_1, ... ,F_k \in F(\Bd(G)) : 
           \ran(Iv) = \bigcup_{i=1}^k V(F_i) \f$
    - \f$ \exists F_1, ... ,F_k \in F(\Bd(G_src)) :
           \dom(Iv) = \bigcup_{i=1}^n V(F_i) \f$
     
    \f$ Iv \f$ may be empty, in which case the result will not be connected.

 \b Postconditions:

   The following postconditions will hold
   (G' is the value of G after execution):
    - \f$ G' = G  \cup G_{src} \f$
    - \f$ |C(G')| = |C(G)| + |C(G_{src})| \f$
    - \f$ |V(G')| = |V(G)| + |V(G_src)| - |dom(Iv)| \f$

  \todo
   <tt> EnlargeGrid() </tt>, <tt> EnlargeGridV() </tt> can be defined
   \e fully generically in terms of <tt> EnlargeGridVC() </tt>

  \see  construct-grid.h
*/


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

} // namespace GrAL 

#endif
