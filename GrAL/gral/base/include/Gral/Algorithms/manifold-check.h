#ifndef GRAL_GB_BASE_ALGORITHMS_MANFIFOLD_CHECK_H
#define GRAL_GB_BASE_ALGORITHMS_MANFIFOLD_CHECK_H


/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/predicates.h"

#include <vector>

namespace GrAL {


  /*! \defgroup check_grid_property_algorithms Grid property checks
    
  \ingroup combinatoric_grid_algorithms
  
  \brief Algorithms checking properties of grids
  
  Often, properties of grids cannot be checked statically just by using
  a particular data structure. For instance, the manifold (or mf-with-boundary)
  property of a grid can often not be guaranteed by constraining the data structure.
  If such a property is required for an algorithm to work properly,
  it must be checked for dynamically.
  */
  

  // FIXME: move to cell-neighbor-search or others
  template<class E1_IT, class E2OnE1_IT, class RANGE, class GF_E2>
  inline void compute_incidence(RANGE const& R, GF_E2 & e1_of_e2)
  {
    for(E1_IT e1 = GrAL::begin_x(R); !e1.IsDone(); ++e1)
      for(E2OnE1_IT e2e1 = GrAL::begin_x(*e1); !e2e1.IsDone(); ++e2e1)
	e1_of_e2[*e2e1].push_back(*e1);
  }

  template<class V, class C>
  inline
  typename grid_types<typename element_traits<V>::grid_type>::Facet 
  incident_facet(V v,C c) 
  {
    typedef grid_types<typename element_traits<V>::grid_type> gt;
    for(typename gt::FacetOnCellIterator f = GrAL::begin_x(c); !f.IsDone(); ++f)
      if(incident(v,*f))
	return *f;
    return typename gt::Facet();
  }


  /*! \brief Check whether G is a manifold-with-boundary mesh (2D only)
  
    \ingroup  check_grid_property_algorithms

    \templateparams
     - \c GRID
         - Model of $GrAL VertexRange
         - Model of $GrAL CellRange
         - type \c Facet
	 - incidence iterators 
            - \c VertexOnFacetIterator
            - \c VertexOnCellIterator
            - \c FacetOnCellIterator
     - \c GF_F Model of $GrAL FacetRange
     - \c GF_V Model of $GrAL VertexRange (can be input vertex range if results are ignored)

     \par Definitions: \n
     A facet is \e boundary, \e interior or \e singular depending on 
     whether the number of incident cells is 1, 2 or greater then 2. 
     A vertex is \e boundary if it is incident to a boundary facet, else it is \e interior.
     A vertex is \e singular if its \e star is not homeomorphic to a d-ball (d=dim(G)).
     See also [1].

    \post singular_facets contains all singular facets
    \post isolated_vertices contains all isolated vertices (without an incident cell)
    \post singular_interior_vertices contains singular interior vertices 
     that are not incident to singular facets
    \post singular_boundary_vertices contains all singular boundary vertices
     that are not incident to singular facets
   
    \return true iff G is a manifold-with-boundary    

    \note This works for 2-dimensional meshes only.

    \see Tested in \ref test-manifold-check.C 

    \par References \n
    - [1] <a href="http://mesh.brown.edu/taubin/pdfs/Gueziec-etal-tvcg01.pdf">
        Cutting and Stitching: Converting Sets of Polygons to Manifold Surfaces</a>, 
        by A. Gueziec, G. Taubin, F. Lazarus, and W. Horn, 
        IEEE Transactions on Visualization and Computer Graphics, Vol. 7, No. 2, 
        April-June 2001, pp. 136-151.
 
  */
  template<class RANGE, class GF_F, class GF_V>
  bool check_manifold_property(RANGE const&  R,
			       GF_F & singular_facets, 
			       GF_V & isolated_vertices,
			       GF_V & singular_interior_vertices,
			       GF_V & singular_boundary_vertices)
  {
    bool res = true;

    typedef grid_types<RANGE> gt;
    typedef typename gt::Vertex Vertex;
    typedef typename gt::Facet  Facet;
    typedef typename gt::Cell   Cell;
    //typedef typename gt::FacetIterator  FacetIterator;
    typedef typename gt::VertexIterator VertexIterator;
    typedef typename gt::CellIterator   CellIterator;
    typedef typename gt::VertexOnFacetIterator VertexOnFacetIterator;
    typedef typename gt::VertexOnCellIterator  VertexOnCellIterator;
    typedef typename gt::FacetOnCellIterator   FacetOnCellIterator;

    //--- find singular facets ---

    typedef std::vector<Cell> cell_range;
    //FIXME: will this always work if e.g. FacetIterator duplicates facets?
    //Should use a mapping  vertex set of facet -> {incident cells}
    grid_function<Facet, cell_range> cells_of_facet(R);
    compute_incidence<CellIterator, FacetOnCellIterator>(R, cells_of_facet);

    // here we could check wether there are facets with zero incident cells.

    GF_F boundary_facets(R);
    // avoid FacetIterator, it might not work properly for non-manifold grids
    for(CellIterator c = GrAL::begin_x(R); !c.IsDone(); ++c) {
      for(FacetOnCellIterator fc = GrAL::begin_x(*c); !fc.IsDone(); ++fc) {
	ENSURE(! cells_of_facet(*fc).empty(), "fc = << " << fc.handle(), 1);
	if(c == cells_of_facet(*fc)[0]) {
	  if(cells_of_facet(*fc).size() > 2) 
	    singular_facets.push_back(*fc);
	  else if (cells_of_facet(*fc).size() == 1)
	    boundary_facets.push_back(*fc);
	}
      }
    }

    //--- find singular vertices ---

    partial_grid_function<Vertex, bool> vertex_of_singular_facet(R,false);
    partial_grid_function<Vertex, bool> boundary_vertex         (R,false);
    for(typename GF_F::ElementIterator sf = GrAL::begin_x(singular_facets); !sf.IsDone(); ++sf)
      for(VertexOnFacetIterator vsf(*sf); !vsf.IsDone(); ++vsf)
	vertex_of_singular_facet[*vsf] = true;
    for(typename GF_F::ElementIterator bf = GrAL::begin_x(boundary_facets); !bf.IsDone(); ++bf)
      for(VertexOnFacetIterator vsf(*bf); !vsf.IsDone(); ++vsf)
	boundary_vertex[*vsf] = true;

    grid_function<Vertex,cell_range> cells_of_vertex(R);
    compute_incidence<CellIterator, VertexOnCellIterator>(R, cells_of_vertex);

    // A vertex is a (2-)manifold vertex if its star is homeomorphic to a (2-)ball
    // (a small neighorhood of v in the star is sufficient.) 
    // To check this, we count number of cells in the first connected component of the vertex star. 
    // If this covers the whole star, 
    // it must be (in 2D) already homeomorphic to a disk,
    // because singular edges are excluded:
    // We can construct a homeomorphism
    // by mapping each corner of an incident cell to a cake-segment of the disk,
    // and this is continuous across edges (because of connectedness) 
    // and one-to-one (because of non-singularity).
    // In higher dimensions, we would have to check for homeomorphism explicitely.
    for(VertexIterator v = GrAL::begin_x(R); !v.IsDone(); ++v) {
      if(cells_of_vertex(*v).empty()) {
	isolated_vertices.push_back(*v);
	continue;
      }
      if(! vertex_of_singular_facet(*v)) {
        Cell  start_c = cells_of_vertex(*v)[0];
	Facet start_f = incident_facet(*v,start_c);
	Facet f = start_f;
	Cell  c = start_c;
	unsigned cnt = 0;
	if(! boundary_vertex(*v)) {
	  do {
	    REQUIRE(cells_of_facet(f).size() == 2, " size is " << cells_of_facet(f).size(), 1);
	    c = (c == cells_of_facet(f)[0] ? cells_of_facet(f)[1] : cells_of_facet(f)[0]);
	    // c = R.TheGrid().switched_cell(f,c);
	    f = R.TheGrid().switched_facet(*v,f,c);
	    ++cnt;
	  } while(f != start_f);
	  REQUIRE(c == start_c, "", 1);
	if(cnt < cells_of_vertex(*v).size()) 
	  singular_interior_vertices.push_back(*v);
	}
	else { // boundary vertex, must go in both directions to get all cells.
	  // forward sweep
	  while(cells_of_facet(f).size() == 2) {
	    ++cnt;
	    c = (c == cells_of_facet(f)[0] ? cells_of_facet(f)[1] : cells_of_facet(f)[0]);
	    f = R.TheGrid().switched_facet(*v,f,c);
	  }
	  c = start_c;
	  ++cnt; // counting start_c;
	  f = R.TheGrid().switched_facet(*v,start_f,start_c);
	  // backward sweep
	  while(cells_of_facet(f).size() == 2) {
	    ++cnt;
	    c = (c == cells_of_facet(f)[0] ? cells_of_facet(f)[1] : cells_of_facet(f)[0]);
	    f = R.TheGrid().switched_facet(*v,f,c);
	  }
	  if(cnt < cells_of_vertex(*v).size()) 
	    singular_boundary_vertices.push_back(*v);
	}
      }
    }
    res = (   0 == singular_facets.size()
	   && 0 == isolated_vertices.size()
	   && 0 == singular_interior_vertices.size() 
	   && 0 == singular_boundary_vertices.size()); 	
    return res;
  }

} // namespace GrAL 

#endif
