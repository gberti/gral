#ifndef GRAL_BASE_GB_GRIDFUNCTIONS_H
#define GRAL_BASE_GB_GRIDFUNCTIONS_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

/*! \brief empty template for total grid functions.
   \ingroup gridfunctions

   This template has to be specialized for
   concrete types of grid elements E. 
   For example, E = Complex2D::Vertex , E = RegGrid2D::Edge, ...
   T is a user-selected type

   A <tt>grid_function<E,T> </tt> represents a mapping \f$ E \mapsto T \f$


 \b Functionality:
 - grid_function(size_type n);   allocate memory
 - T operator()(const E&) const;  get value
 - T& operator[](const E&);       set value
 
 \b Example:
 \code
 grid_function<Vertex,int> vnum(G.NumOfVertices());
 for(vit = G.FirstVertex(), i = 0; ! vit.IsDone(); ++i, ++vit)
    vnum[*vit] = i;
  ....
  A(vnum(V),vnum(W)) = a(Phi(V),Phi(W)); 
 \endcode

 \see $GrAL TotalGridFunction concept.
 \see \ref gridfunctions module
*/

template<class E, class T>
class grid_function {};


/*! \brief empty template for partial grid functions.
   \ingroup gridfunctions

   This template has to be specialized for
   concrete types of grid elements E. 
   For example, E = Complex2D::Vertex , E = RegGrid2D::Edge, ...
   T is a user-selected type

   A <tt>partial_grid_function<E,T> </tt> 
   represents a mapping \f$ E \mapsto T \f$,
   which relies on default values.


 \b Functionality (excerpt):
 - partial_grid_function(size_type n);   allocate memory
 - T  const& operator()(const E&) const;  get value
 - T       & operator[](const E&);       set value
 - bool      defined(E const&);   test explicit definition

 \b Example:
 \code
 partial_grid_function<Vertex,bool>  marked(G.NumOfVertices(), false);
 for(v(G); ! v.IsDone(); ++v)
   if(condition(*v))
      marked[*v] = true;
  ....
 \endcode

 \see $GrAL PartialGridFunction concept.
*/

template<class E, class T>
class partial_grid_function;

} // namespace GrAL 

#endif
