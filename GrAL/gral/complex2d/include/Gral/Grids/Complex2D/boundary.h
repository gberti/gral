#ifndef NMWR_GB_COMPLEX2D_BOUNDARY_H
#define NMWR_GB_COMPLEX2D_BOUNDARY_H

// $LICENSE


// Gral Base
#include "Gral/Base/boundary.h"
#include "Gral/Iterators/boundary-iterator.h"
#include "Gral/Base/grid-functors.h"

// Gral Complex2D
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"

/*! \defgroup complex2dboundary Boundary range for Complex2D
    \ingroup complex2dmodule

    \todo Test
*/

class boundary_range_iterator_complex2d {

  typedef Complex2D              grid_type;
  typedef grid_types<grid_type>  gt;
  typedef  gt::Vertex    Vertex;
  typedef  gt::Edge      Edge;
  typedef  gt::Facet     Facet;
  typedef  gt::Cell      Cell;

  typedef iscellinside_pred<grid_type>                      inside_p;
  typedef BoundaryComponentIterator2D<Complex2D, inside_p > component_iter;
  // this loops over all boundary facets in no special order.
  typedef BoundaryFacet2D_Iterator                          boundary_facet_iter;

  typedef boundary_range_iterator_complex2d self;

private:
  //-------- DATA ----------------------------
  grid_type const*                  g;
  boundary_facet_iter               bf;
  component_iter                    component;
  partial_grid_function<Facet,bool> visited;
  //------- END DATA -------------------------
public:
  boundary_range_iterator_complex2d() 
    : g(0) {}
  boundary_range_iterator_complex2d(const Complex2D& gg) 
    : g(&gg), bf(gg.FirstBoundaryFacet()), visited(gg,false)
    {
      if(!bf.IsDone()) {
	component = component_iter(*bf, IsCellInside(gg));
	visited[component.curr_facet()] = true;
      }
    }

  self& operator++() {
    if(!component.IsDone()) {
      ++component;
      visited[component.curr_facet()] = true;
    }
    if(component.IsDone()) {
      while( !bf.IsDone() && visited[*bf])
	++bf;
      if ( !bf.IsDone())
	component = component_iter(*bf,IsCellInside(*g));
    }
    return *this;
  }
  bool IsDone() const { return bf.IsDone();}

  Vertex curr_vertex() const { return component.curr_vertex();}
  Edge   curr_edge()   const { return component.curr_edge();}
  Facet  curr_facet()  const { return component.curr_facet();}

  grid_type const& TheGrid() const { 
    REQUIRE((g != 0), "no grid!", 1);
    return (*g);
  }
};


class boundary_vertex_iterator_complex2d {
private:
  //------- DATA ----------------------
  boundary_range_iterator_complex2d base;

  typedef boundary_vertex_iterator_complex2d self;
public:
  typedef grid_types<Complex2D>    gt;
  typedef gt::Vertex               Vertex;
  typedef gt::vertex_handle        vertex_handle;
  typedef Vertex                   value_type;
  typedef Complex2D                grid_type;


public:

  boundary_vertex_iterator_complex2d() {}
  boundary_vertex_iterator_complex2d(const Complex2D& g) : base(g) {}

  Vertex operator*() const { return base.curr_vertex();}
  bool   IsDone()    const { return base.IsDone();}
  self&  operator++() { ++base; return (*this);}
  vertex_handle handle() const { return base.curr_vertex().handle();}
  grid_type const&  TheGrid() const { return base.TheGrid();}
};


class boundary_edge_iterator_complex2d {
private:
  //------- DATA ----------------------
  boundary_range_iterator_complex2d base;

  typedef boundary_edge_iterator_complex2d self;
public:
  typedef grid_types<Complex2D>  gt;
  typedef gt::Edge               Edge;
  typedef gt::edge_handle        edge_handle;
  typedef Edge                   value_type;
  typedef Complex2D              grid_type;
public:
  boundary_edge_iterator_complex2d() {}
  boundary_edge_iterator_complex2d(const Complex2D& g) : base(g) {}

  Edge operator*() const { return base.curr_edge();}
  bool   IsDone()  const { return base.IsDone();}
  self&  operator++() { ++base; return (*this);}
  edge_handle handle() const { return base.curr_edge().handle();}
};

/*! \brief Specialization of the BoundaryRange template for Complex2D.

   Model of $GrAL VertexRange, $GrAL EdgeRange, $GrAL GridRange.

   \ingroup complex2dboundary
*/
template<>
class BoundaryRange<Complex2D> {
private:
  //--------- DATA -------------
  const Complex2D*   g;
public:
  typedef boundary_vertex_iterator_complex2d VertexIterator;
  typedef boundary_edge_iterator_complex2d   EdgeIterator;
  typedef boundary_edge_iterator_complex2d   FacetIterator;
  typedef Complex2D                          grid_type;


  BoundaryRange() : g(0) {}
  BoundaryRange(const Complex2D& gg) : g(&gg) {}

  VertexIterator  FirstVertex() const { return VertexIterator(*g);}
  EdgeIterator    FirstEdge()   const { return EdgeIterator(*g);}
  FacetIterator   FirstFacet()  const { return FacetIterator(*g);}

  grid_type const& TheGrid() const { 
    REQUIRE((g != 0), "no grid!", 1);
    return (*g);
  }

};

/*! \brief Specialization of \c grid_types for <tt> BoundaryRange <Complex2D> </tt>

    \ingroup complex2dboundary
 */
template<>
struct grid_types<BoundaryRange<Complex2D> > 
{
  typedef grid_types<Complex2D>     gt;
  typedef BoundaryRange<Complex2D>  BrC;

  typedef  gt::Vertex Vertex;
  typedef  gt::Edge   Edge;
  typedef  gt::Facet  Facet;
  typedef  gt::Cell   Cell;

  typedef  BrC::VertexIterator VertexIterator;
  typedef  BrC::EdgeIterator   EdgeIterator;
  typedef  BrC::FacetIterator  FacetIterator;
  //  typedef  BrC::CellIterator   CellIterator;

  typedef  gt::VertexOnCellIterator VertexOnCellIterator;
  typedef  gt::EdgeOnCellIterator   EdgeOnCellIterator;
  typedef  gt::FacetOnCellIterator  FacetOnCellIterator;
  typedef  gt::CellOnCellIterator   CellOnCellIterator;
};


#endif
