#ifndef GRAL_GB_BASE_BOUNDARY_GRID_H
#define GRAL_GB_BASE_BOUNDARY_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"

// to be specialized for DIM
template<class E, class ETAG, class DIM>
class boundary_grid_aux {};

template<class E, class ETAG, class DIM>
class boundary_grid_types_aux {};



template<class E, class ETAG, class DIM>
class boundary_grid_types_base 
{
protected:
  typedef element_traits<E>       et;
  typedef typename et::grid_type  base_grid_t;
  typedef grid_types<base_grid_t> bgt;
public:
  typedef typename bgt::Vertex        Vertex;
  typedef typename bgt::vertex_handle vertex_handle;

};


/*! Specialization for 2D grid, boundary of cells -> 1D grid
 */
template<class E>
class boundary_grid_types_aux<E, cell_type_tag, grid_dim_tag<2> > 
  : public boundary_grid_types_base<E, cell_type_tag, grid_dim_tag<2> >
{
  typedef grid_dim_tag<1> dimension_tag;

  typedef typename bgt::VertexOnCellIterator VertexIterator;
  typedef typename bgt::Facet                Cell;
  typedef typename bgt::facet_handle         cell_handle;
  typedef typename bgt::FacetOnCellIterator  CellIterator;

  typedef typename bgt::Vertex               Facet;
  typedef typename bgt::vertex_handle        facet_handle;
  typedef typename bgt::VertexOnEdgeIterator FacetOnCellIterator;
};


/*! Specialization for 2D grid, boundary of cells -> 1D grid
 */
template<class E>
class boundary_grid_aux<E, cell_type_tag, grid_dim_tag<2> > 
  : public  boundary_grid_types_aux<E, cell_type_tag, grid_dim_tag<2> >
{
  typedef E BaseCell;
  BaseCell  bc;
public:
  boundary_grid_aux() {}
  boundary_grid_aux(BaseCell bbc) : bc(bbc) {}

  // for usage as in VertexIterator v(g); where g is of typeof(*this) 
  operator BaseCell const&() const { return bc;}

  VertexIterator FirstVertex() const { return VertexIterator(bc);}
  unsigned       NumOfVertices() const { return bc.NumOfVertices();}
  CellIterator   FirstCell()   const { return CellIterator(bc);}
  unsigned       NumOfCells()  const { return bc.NumOfFacets();}

  void switch_cell(Facet const& f, Cell & c) const
    { bc.TheGrid().switch_edge(f,c,bc); }
  Cell switched_cell(Facet const& f, Cell const& c) const 
    { Cell sc(c); switch_cell(f,sc); return sc;}

  // a boundary has no boundary
  bool IsOnBoundary(FacetOnCellIterator const&) const { return false;} 
  bool IsOnBoundary(Facet               const&) const { return false;}
};




/*! Specialization for 3D grid, boundary of cell -> 2D grid
 */
template<class E>
class boundary_grid_types_aux<E, cell_type_tag, grid_dim_tag<3> > 
  : public boundary_grid_types_base<E, cell_type_tag, grid_dim_tag<3> >
{
  typedef grid_dim_tag<2> dimension_tag;

  typedef typename bgt::VertexOnCellIterator VertexIterator;
  typedef typename bgt::Facet                Cell;
  typedef typename bgt::facet_handle         cell_handle;
  typedef typename bgt::FacetOnCellIterator  CellIterator;

  typedef typename bgt::Edge                 Facet;
  typedef typename bgt::edge_handle          facet_handle;
  typedef typename bgt::EdgeOnFacetIterator  FacetOnCellIterator;
};


/*! Specialization for 3D grid, boundary of cell -> 2D grid
 */
template<class E>
class boundary_grid_aux<E, cell_type_tag, grid_dim_tag<3> > 
  : public  boundary_grid_types_aux<E, cell_type_tag, grid_dim_tag<3> >
{
  typedef E BaseCell;
  BaseCell  bc;
public:
  boundary_grid_aux() {}
  boundary_grid_aux(BaseCell bbc) : bc(bbc) {}

  // for usage as in VertexIterator v(g); where g is of typeof(*this) 
  operator BaseCell const&() const  { return bc;}

  VertexIterator FirstVertex() const { return VertexIterator(bc);}
  CellIterator   FirstCell()   const { return CellIterator(bc);}
  unsigned       NumOfVertices() const { return bc.NumOfVertices();}
  unsigned       NumOfCells()  const { return bc.NumOfFacets();}

  void switch_cell  (Facet const& f, Cell const& c) const
    { bc.TheGrid().switch_facet(f,c,bc);}
  Cell switched_cell(Facet const& f, Cell const& c) const 
    { Cell sc(c); switch_cell(f,sc); return sc;}

  // a boundary has no boundary
  bool IsOnBoundary(FacetOnCellIterator const&) const { return false;}
  bool IsOnBoundary(Facet               const&) const { return false;}
};





/*! Specialization for 3D grid, boundary of facet -> 1D grid
 */
template<class E>
class boundary_grid_types_aux<E, facet_type_tag, grid_dim_tag<3> > 
  : public boundary_grid_types_base<E, facet_type_tag, grid_dim_tag<3> >
{
public:
  typedef grid_dim_tag<1> dimension_tag;

  typedef typename bgt::VertexOnFacetIterator VertexIterator;
  typedef typename bgt::Edge                  Cell;
  typedef typename bgt::edge_handle           cell_handle;
  typedef typename bgt::EdgeOnFacetIterator   CellIterator;

  typedef typename bgt::Vertex                Facet;
  typedef typename bgt::vertex_handle         facet_handle;
  typedef typename bgt::VertexOnEdgeIterator  FacetOnCellIterator;
};


/*! Specialization for 3D grid, boundary of facet -> 1D grid
 */
template<class E>
class boundary_grid_aux<E, facet_type_tag, grid_dim_tag<3> > 
  : public boundary_grid_types_aux<E, facet_type_tag, grid_dim_tag<3> > 
{
  typedef E BaseFacet;
  BaseFacet  bf;
public:
  boundary_grid_aux() {}
  boundary_grid_aux(BaseFacet bbf) : bf(bbf) {}

  // for usage as in VertexIterator v(g); where g is of typeof(*this) 
  operator BaseFacet const&() const  { return bf;}

  VertexIterator FirstVertex() const { return VertexIterator(bf);}
  CellIterator   FirstCell()   const { return CellIterator(bf);}
  unsigned       NumOfVertices() const { return bf.NumOfVertices();}
  unsigned       NumOfCells()  const { return bf.NumOfEdges();}

  void switch_cell(Facet const& f, Cell & c) const 
    { bf.TheGrid().switch_edge(f,c);}
  Cell switched_cell(Facet const& f, Cell const& c) const 
    { Cell sc(c); switch_cell(f,sc); return sc;}

  // a boundary has no boundary
  bool IsOnBoundary(FacetOnCellIterator const&) const { return false;}
  bool IsOnBoundary(Facet               const&) const { return false;}
};

/*! Specialization for ND grid,  boundary of edge -> 0D grid (2-element vertex set)
 */
template<class E,class DIM>
class boundary_grid_types_aux<E, edge_type_tag, DIM> 
  : public boundary_grid_types_base<E, edge_type_tag, DIM>
{
  typedef grid_dim_tag<0> dimension_tag;

  typedef typename boundary_grid_types_base<E, edge_type_tag, DIM>::bgt bgt;
  typedef typename bgt::VertexOnEdgeIterator VertexIterator;
  typedef typename bgt::Vertex               Cell;
  typedef typename bgt::vertex_handle        cell_handle;
  typedef typename bgt::VertexOnEdgeIterator CellIterator;

  // no facets: would be of dimension -1.
};

/*! Specialization for ND grid,  boundary of edge -> 0D grid (2-element vertex set)
 */
template<class E,class DIM>
class boundary_grid_aux<E, edge_type_tag, DIM> 
  : public boundary_grid_types_aux<E, edge_type_tag, DIM> 
{
  typedef E BaseEdge;
  BaseEdge  be;
public:
  boundary_grid_aux() {}
  boundary_grid_aux(BaseEdge bbe) : be(bbe) {}

  // for usage as in VertexIterator v(g); where g is of typeof(*this) 
  operator BaseEdge const&() const { return be;}

  VertexIterator FirstVertex()   const { return VertexIterator(be);}
  CellIterator   FirstCell()     const { return CellIterator(be);}
  unsigned       NumOfVertices() const { return 2; } // be.NumOfVertices();}
  unsigned       NumOfCells()    const { return 2; } // be.NumOfVertices();}

};




/*! Boundary grid of a grid element.
   \ingroup boundary

   The grid's and element's dimensions are deduced at compile-time,
   in order to use the correct specialization of boundary_grid_aux.
 */
template<class E>
class boundary_grid 
  : public boundary_grid_aux<E, typename element_traits<E>::element_type_tag,
                                typename element_traits<E>::grid_dimension_tag>
{
  typedef boundary_grid_aux<E,typename element_traits<E>::element_type_tag,
                              typename element_traits<E>::grid_dimension_tag>
  base;
public:
  boundary_grid() {}
  boundary_grid(E const& e) : base(e) {}
};



template<class E>
class grid_types<boundary_grid<E> >
  : public boundary_grid_types_aux
           <E, typename element_traits<E>::element_type_tag,
               typename element_traits<E>::grid_dimension_tag>
{};


#endif
