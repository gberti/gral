#ifndef NMWR_GB_ENLARGE_GRID_COMPLEX2D_H
#define NMWR_GB_ENLARGE_GRID_COMPLEX2D_H


// $LICENSE

/*! \file 
 */

#include "Gral/Base/enlarge-grid.h"
#include "Gral/Grids/Complex2D/complex2d.h"

/*! \defgroup complex2denlarge Generic grid enlargement (glueing) for Complex2D
 
    \ingroup complex2dmodule
    Overloads of the templates in  <tt> Gral/Base/</tt> \ref <tt> enlarge-grid.h </tt>
    \see $GrAL ConstructGrid
    \see Gral base module \ref enlargeoperations
    \see module \ref complex2dcopy

   The functions <tt> EnlargeGrid(G, G_src, Geom_src, Iv, ...) </tt> attach a copy of \c G_src
   to \c G, which is the target grid of type Complex2D.
   \c G_src and \c Geom_src are the source grid and geometry, of arbitrary type,
   and \c Iv gives a rule which vertices are to be identified: 
   \c Iv is a injective mapping  \f$ V(G_{\mbox{src}}) \mapsto V(G) \f$ (defined on vertex handles).

   Here and in the following, 
   \f$\Bd(G)\f$ is the boundary range of \f$G\f$, 
   \f$V(G)\f$, \f$F(G)\f$,\f$C(G)\f$  are the sets of vertices, facets, and cell of \f$G\f$.

   \pre
     - \f$ G_{\mbox{src}} \neq \emptyset \f$ (contains at least one cell)
     - \f$ \dom(Iv) \subset V(\Bd(G_{\mbox{src}})) \f$. More precisely: <br>
       \f$ \exists F_1, ... ,F_k \in F(\Bd(G_{\mbox{src}})) :
           \dom(Iv) = \bigcup_{i=1}^n V(F_i) \f$
     - \f$ \ran(Iv) \subset V_h(\Bd(G)) \f$. More precisely: <br>
        \f$ \exists F_1, ... ,F_k \in F(\Bd(G)) : 
         \ran(Iv) = \bigcup_{i=1}^k V(F_i) \f$
     - \c Iv may be empty, in which case the result will not be connected.

   \post
      let \f$ G' \f$ be the value of \f$ G \f$  after execution. Then
      - \f$ G' = G  \cup G_src \f$
      - \f$ |C(G')| = |C(G)| + |C(G_{\mbox{src}})| \f$, 
      - \f$ |V(G')| = |V(G)| + |V(G_{\mbox{src}})| - |\dom(Iv)| \f$

   \todo Test EnlargeGrid variants
*/


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/complex2d-enlarge.C"
#else

/*! \brief Glue the grid \c G_src to  \c G.

    \ingroup complex2denlarge
    \see Module \ref complex2denlarge
 */
template<class Conn, class Geom, class VertexIdent>
void EnlargeGrid(Complex2D& G,                 // in/out
		 const Conn& G_src,            // in
		 const Geom& Geo_src,          // in
		 const VertexIdent& Iv);       // in

/*! \brief Glue the grid \c G_src to  \c G
    
     This version maps cells (\c CellCorr) and vertices (\c VertexCorr) in \c G_src 
     to their copies in \c G \f$ \cup \f$ \c G_src.

    \ingroup complex2denlarge
    \see Module \ref complex2denlarge
 */
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

/*! \brief Glue the grid \c G_src to  \c G
    
     This version maps cells (\c CellCorr) and vertices (\c VertexCorr) in \c G_src 
     to their copies in \c G \f$ \cup \f$ \c G_src.
    \ingroup complex2denlarge
    \see Module \ref complex2denlarge
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


#endif


#endif
