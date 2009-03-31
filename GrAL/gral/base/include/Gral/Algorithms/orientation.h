#ifndef GRAL_GB_BASE_ALGORITHMS_ORIENTATION_H
#define GRAL_GB_BASE_ALGORITHMS_ORIENTATION_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include <Gral/Iterators/cell-on-cell-iterator.h>

#include <queue>
#include <algorithm>

namespace GrAL {


/*! \defgroup orientation_algorithms Grid orientations 

    \ingroup combinatoric_grid_algorithms

    \brief Algorithms related to orientation of grids.
*/



/*! \brief Determine relative orientation of cells

    \ingroup orientation_algorithms 

    \templateparams
     - \c G
        - Model of $GrAL CellRange
        - type \c FacetOnCellIterator
        - The function
          <tt> int relative_orientation(FacetOnCellIterator f1, FacetOnCellIterator f2) </tt> 
          has been overloaded for <tt> G::FacetOnCellIterator </tt>
          and returns  the <em> relative orientation </em> of the facet <tt> *f1 == *f2 </tt> 
	  induced by \c f1 and \c f2. For instance, in a 2D grid, this means that if the edge
          \c *f1 is represented with opposite vertex ordering by \c f1 and \c f2, 
          the result should be -1, and +1 otherwise. 
     - \c GF
        - Model of cell based $GrAL MutableGridFunction 

    \param g input grid
    \param orientation output grid function indicating orientation changes
    \return true if the grid \g is orientable, and false otherwise.

    \pre \c g is a  Glossary manifold-with-boundary grid
    \pre \c orientation is \e bound to \c g
    \post On return, <tt> orientation(c) </tt>
    is positive if the orientation does not need to be changed, and negative otherwise. The absolute value of <tt> orientation(c) </tt> indicates the connected component of \c c.

    \note The orientation flags in \c orientation are computed on a purely combinatorial basis.
     If an orientation with a geometric meaning is wanted (e.g. producing outward pointing normals
     in the case of a manifold grid embedded in an <tt> dim(g)+1 </tt> dimensional space, 
     one has to flip, 
     orientations of either all cells with positive or all cells with negative flags, for each component
     independently.

    \see Tested in \ref test-orientation.C
     
*/


  template<class G, class GT>
  bool get_orientation(G const& g, GT & orientation) {
    // FIXME: should reuse G's CellOnCellIterator if possible
    typedef cell_on_cell_iterator<G> coc_type;
    coc_type::init(g);

    typedef grid_types<G> gt;
    typedef typename gt::Cell  Cell;
    typedef typename gt::Facet Facet;
    typedef typename gt::FacetOnCellIterator FacetOnCellIterator;
  
    grid_function<typename gt::Cell, bool> visited    (g,false);
    std::queue<Cell> cells;
    int component = 0;
    for(typename gt::CellIterator germ(g); !germ.IsDone(); ++germ) {
      if(! visited(*germ)) {
	++component;
	visited[*germ] = true;
	orientation[*germ] = component;
	cells.push(*germ);
	while(!cells.empty()) {
	  Cell c = cells.front(); cells.pop();
	  for( coc_type cc(c); !cc.IsDone(); ++cc) {
	    FacetOnCellIterator f1 = cc.TheFacetOnCellIterator();
	    FacetOnCellIterator f2 = std::find(GrAL::begin<FacetOnCellIterator>(*cc),
					       GrAL::end  <FacetOnCellIterator>(*cc), *f1);
	    // FacetOnCellIterator f2 = std::find((*cc).FirstFacet(), (*cc).EndFacet(), *f1);
	    if(visited(*cc)) {
	      if(orientation(*cc) != - relative_orientation(f1,f2) * orientation(c))
		// not orientable
		return false;
	    }
	    else {
	      cells.push(*cc);
	      visited[*cc] = true;
	      orientation[*cc] = - relative_orientation(f1,f2) * orientation(c);
	    }
	  }
	}
      }
    }
    return true;
  }

/*! \brief Change orientation of cells such that it is consistent

    \ingroup orientation_algorithms 

    \templateparams
     - \c G
        - Model of $GrAL CellRange
        - type \c FacetOnCellIterator
        - function \c swap_orientation(Cell)
	- The function
          <tt> int relative_orientation(FacetOnCellIterator f1, FacetOnCellIterator f2) </tt> 
          has been overloaded for <tt> G::FacetOnCellIterator </tt>
          and returns  the <em> relative orientation </em> of the facet <tt> *f1 == *f2 </tt> 
	  induced by \c f1 and \c f2. For instance, in a 2D grid, this means that if the edge
          \c *f1 is represented with opposite vertex ordering by \c f1 and \c f2, 
          the result should be -1, and +1 otherwise. 
    - \c C
       - Model of $GrAL GridCell
    \param g input grid, orientation of some cells will be changed on output
    \param \c guide_cell Cell with reference orientation
    \return true if the grid \g is orientable, and false otherwise.

    \pre \c g is a  Glossary manifold-with-boundary grid
    \pre \c c \f$ \in \f$ \c  g
    \post On return, \c g is oriented consistent with \c guide_cell, 
     such that  a call to \c get_orientation(g, orient) will yield <tt> orientation(c) == 1</tt>
     for each cell \c c of \c g.
 
     \see Tested in \ref test-orientation.C
     \todo Extend to list of guide cells for disconnected grids.
     
  */


  template<class G, class C>
  bool fix_orientation(G & g, C guide_cell) {
    typedef grid_types<G> gt;
    grid_function<typename gt::Cell, int> orientation(g,0);
    bool orientable = get_orientation(g, orientation);
    int default_orientation = orientation(guide_cell);
    for(typename gt::CellIterator c(g); !c.IsDone(); ++c)
      if(orientation(*c)*default_orientation < 0) 
	g.swap_orientation(*c);
    return orientable;
  }

  /*! \brief Convenience wrapper for orientation

    \ingroup orientation_algorithms 
   
     Equivalent to <tt> fix_orientation(g, * g.FirstCell()) </tt>

  */
  template<class G>
  bool fix_orientation(G & g) {
    return fix_orientation(g, * g.FirstCell());
  }


  /* \brief A potential implementation of relative_orientation

  template<class FoC>
  int relative_orientation(FoC f1, FoC f2) {
    std::vector<vertex_handle> v[2];
    F & f[2] = { f1, f2 };
    for(int i = 0; i < 2; ++i)
      for(gt::VertexOnFacetIterator vf(f[i]); !vf.IsDone(); ++vf)
	v[i].push_back(vf.handle());
    return (even_permutation(v[0],v[1]) ? 1 : -1);
  }
  */

} // namespace GrAL

#endif
