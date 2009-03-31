#ifndef GRAL_GB_ALGORITHM_CONSTRUCT_ISOMORPHISM_C
#define GRAL_GB_ALGORITHM_CONSTRUCT_ISOMORPHISM_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Algorithms/construct-isomorphism.h"
#include "Gral/Subranges/boundary-grid.h"

#include "Utility/pre-post-conditions.h"

#include <queue>

namespace GrAL {

template<unsigned N>
struct grid_isomorphism_construction;

template<class GRID1, class GRID2, 
         class GT1,   class GT2,
         class PHI>
bool construct_isomorphism(GRID1 const& G1,
			   GRID2 const& G2,
			   GT1   gt1,                // only type used
			   GT2   gt2,                //    
			   PHI        & phi); 


template<class GRID1, class GRID2, 
         class PHI>
bool construct_isomorphism(GRID1 const& G1,
			   GRID2 const& G2,
			   PHI        & phi)   // OUT
{ 
  return construct_isomorphism(G1,G2,
			       grid_types<GRID1>(), 
			       grid_types<GRID2>(),
			       phi);
}

template<class CELL1, class CELL2, class GRID1, class GRID2, class PHI>
bool extend_isomorphism(CELL1 const& c1,
			CELL2 const& c2,
			GRID1 const& G1,
			GRID2 const& G2,
			PHI        & phi);


// base case: 0-dimensional grid
template<>
struct grid_isomorphism_construction<0>
{ 
  template<class GRID1, class GRID2, 
           class GT1,   class GT2,
           class PHI>
  static
  bool construct(GRID1 const& G1,
		 GRID2 const& G2,
		 GT1,                // only type used
		 GT2,                //    
		 PHI        & phi)   // OUT
    {
      // G1 and G2 should consist of exactly 2 vertices each
      REQUIRE( (G1.NumOfVertices() == 2 && G2.NumOfVertices() == 2),
	       "0D bd grid must consist of exactly 2 vertices!\n",1);
      REQUIRE( phi.empty(), "construct called with non-empty phi!\n",1);

      typename GT1::VertexIterator v1(G1);
      typename GT2::VertexIterator v2(G2);
      phi[*  v1] = *  v2; 
      phi[*++v1] = *++v2;
      return true;
    }
 
  template<class CELL1, class CELL2, 
           class GRID1, class GRID2, 
           class GT1,   class GT2,
           class PHI>
  static
  bool extend(CELL1 const& c1,
	      CELL2 const& c2,
	      GRID1 const& G1,
	      GRID2 const& G2,
	      GT1,                // only type used
	      GT2,                //    
	      PHI        & phi)   // OUT
    {
      // G1 and G2 should consist of exactly 2 vertices each
      REQUIRE( (G1.NumOfVertices() == 2 && G2.NumOfVertices() == 2),
		"0D bd grid must consist of exactly 2 vertices!\n",1);
      REQUIRE(phi(c1) == c2, "wrong input mapping!\n",1);

      typename GT1::VertexIterator v1(G1);
      if(*v1 == c1) ++v1; // get second vertex of G1
      typename GT2::VertexIterator v2(G2);
      if(*v2 == c2) ++v2;

      phi[*v1] = *v2; 
      return true;
    }
};


template<unsigned N>
struct grid_isomorphism_construction {

  template<class GRID1, class GRID2,    
           class GT1,   class GT2,
           class PHI>
  static
  bool construct(GRID1 const& G1,
		 GRID2 const& G2,
		 GT1,
		 GT2,
		 PHI        & phi)
    {
      REQUIRE( phi.empty(), "construct called with non-empty phi!\n",1);

      if(  G1.NumOfCells() != G2.NumOfCells() 
	 ||G1.NumOfVertices() != G2.NumOfVertices())
	return false;

      typename GT1::CellIterator c1(G1.FirstCell());
      typename GT2::CellIterator c2(G2.FirstCell());
      bool success = false;
      while(! success && !c1.IsDone()) {
	typedef boundary_grid<typename GT1::Cell> bd1;
	typedef boundary_grid<typename GT2::Cell> bd2;
	bd1 bdc1(*c1); bd2 bdc2(*c2);
	success =
	  construct_isomorphism(bdc1,bdc2,
				grid_types<bd1>(), grid_types<bd2>(),
				phi);

	if(  success ) { // c1, c2 are isomorphic 
          phi[*c1] = *c2;
	  // try to extend to whole grid
	  success =
	    extend_isomorphism(*c1,*c2,G1,G2,GT1(),GT2(),phi);
	}
	if( ! success ) { // c1, c2  not isomorphic
	  // or isorphism cannot be extended to G1, G2
	  // => get next candidate cell pair
	  phi.clear();
	  ++c2;
	  if(c2.IsDone()) {
	    ++c1;
	    c2 = G2.FirstCell();
	  }
	}
	
      }
      return success; // (phi.empty() ? false : true);
    }

  template<class CELL1, class CELL2, 
    class GRID1, class GRID2, 
    class GT1,   class GT2,
    class PHI>
  static
  bool extend(CELL1 const& c1,
	      CELL2 const& c2,
	      GRID1 const& G1,
	      GRID2 const& G2,
	      GT1,                // only type used
	      GT2,                //    
	      // PHI   const& phi_c, // IN
	      PHI        & phi)   // OUT
    {
      typedef typename GT1::Facet Facet1;
      typedef typename GT2::Facet Facet2;
      typedef CELL1               Cell1;
      typedef CELL2               Cell2;
      typedef typename GT1::FacetOnCellIterator FacetOnCellIterator1;

      std::queue<CELL1> cells;
      cells.push(c1);

      while(! cells.empty()) {
	Cell1 d1 = cells.front(); cells.pop();
	Cell2 d2 = phi(d1);
	for(FacetOnCellIterator1 f1(d1); ! f1.IsDone(); ++f1) {
	  Facet2 f2 = phi(*f1);
	  // IsOnBoundary can only happen in outermost grid,
	  // as a boundary grid has no boundary.
	  // one on boundary, the other internal: failure.
	  // both facets internal => proceed
	  // both on boundary: skip
	  bool onbd1 = G1.IsOnBoundary(f1), onbd2 = G2.IsOnBoundary(f2);
	  if( !onbd1 && onbd2 || onbd1 && !onbd2) {
	    //phi.clear(); 
	    return false;
	  } 
	  else if (!onbd1 && !onbd2) { // f1, f2 are internal and not visited
	    // visited_f1[*f1] = visited_f2[f2] = true;
	    Cell1 n1 = G1.switched_cell(*f1, d1);
	    Cell2 n2 = G2.switched_cell( f2, d2);
	    if ( ! phi.defined(n1)) {
	      if( phi.inverse().defined(n2)) {
		// phi.clear(); 
		return false;
	      }
	      else { // n1, n2 not yet visited
		// => extend to n1, n2
		// (1) extend recursively to boundary of n1
		cells.push(n1);
		typedef boundary_grid<Cell1> bd1;
		typedef boundary_grid<Cell2> bd2;
		bd1 bdn1(n1); bd2 bdn2(n2);
		bool success =
		extend_isomorphism(*f1,f2,
				   bdn1, bdn2, 
				   grid_types<bd1>(),grid_types<bd2>(),
				   phi);
		// (2) extend to cell n1, n2 if success
		if( ! success) {
		  return false;
		}
		else { 
		  phi[n1] = n2;
		}
	      } // else [n1, n2 not yet visited]
	    } // if ( ! phi.defined(n1))
	  } // else [f1, f2 are internal and not visited]
	} // for (FacetOnCellIterator1 f1(d1) ...
      } // while(! cells.empty()) 
      return true;
    }

};

  
template<class CELL1, class CELL2, 
         class GRID1, class GRID2, 
         class GT1,   class GT2,
         class PHI>
bool extend_isomorphism(CELL1 const& c1,
			CELL2 const& c2,
			GRID1 const& G1,
			GRID2 const& G2,
			GT1   gt1,                // only type used
			GT2   gt2,                //    
			PHI        & phi)   // OUT
    { 
      typedef typename GT1::dimension_tag d_tag;
      return grid_isomorphism_construction<d_tag::dim>::
	extend(c1,c2,G1,G2,GT1(), GT2(),phi);
    }


template<class GRID1, class GRID2, 
         class GT1,   class GT2,
         class PHI>
bool construct_isomorphism(GRID1 const& G1,
			   GRID2 const& G2,
			   GT1   gt1,                // only type used
			   GT2   gt2,                //    
			   PHI        & phi)   // OUT
    { 
      typedef typename GT1::dimension_tag d_tag;
      return grid_isomorphism_construction<d_tag::dim>::
	construct(G1,G2,GT1(), GT2(),phi);
    }


} // namespace GrAL 

#endif
