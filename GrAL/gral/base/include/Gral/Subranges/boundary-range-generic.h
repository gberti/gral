#ifndef GRAL_BASE_GB_GRID_BOUNDARY_GENERIC_H
#define GRAL_BASE_GB_GRID_BOUNDARY_GENERIC_H


// $LICENSE



#include "Gral/Iterators/boundary-iterator.h"
#include "Gral/Base/grid-functors.h"
#include "Gral/Subranges/find-boundary-components.h"

#include <list>

namespace GrAL {



//----------------------------------------------------------------
/*! \defgroup boundaryrange Generic Grid Boundary Range
    \ingroup boundary
    \ingroup subranges   
 
   \brief Classes implementing generically a grid boundary range for 2D grids.

   Initalization time is \f$ O(|F|) \f$, \f$ F \f$ is the facet set of the grid.
   Storage is \f$ O(|Cb|) \f$ , where \f$ Cb \f$ is the number of boundary components.

   This works only for 2D, as it uses the 2D-component iterators. 
   For higher dimensions, use GrAL::boundary_range_nd.

   \see Module \ref boundarycompiter
   \see Module \ref boundaryalgorithms

   \todo Rename to boundary_range_2d
   \todo Change boundary_range_nd to select boundary_range_2d in the 2D case
*/
//----------------------------------------------------------------


template<class GRID>
class boundary_range_iterator;

template<class GRID>
class boundary_range_vertex_iterator;

template<class GRID>
class boundary_range_edge_iterator;

template<class GRID>
class boundary_component;

/*! \ingroup boundaryrange
    \brief Class representing the boundary of a grid.

    \see \ref boundaryrange module 
 */
template<class GRID>
class boundary_range_generic {
  typedef boundary_range_generic<GRID> self;

  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
public:

  typedef typename gt::Facet  Facet;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Vertex Vertex;
  typedef boundary_component<GRID> component_type;
  
  friend class boundary_range_iterator<GRID>;
  typedef boundary_range_vertex_iterator<GRID> VertexIterator;
  typedef boundary_range_edge_iterator<GRID>   EdgeIterator;
  typedef boundary_range_edge_iterator<GRID>   FacetIterator;

  typedef Facet                                   germ_type;
  typedef std::vector<germ_type>                  germ_list_type;
  typedef typename germ_list_type::const_iterator germ_iterator;
private:
  germ_list_type    component_germs;
  grid_type const*  g;
  bool              initialized;

  mutable int num_of_vertices;
  mutable int num_of_edges;
public:
  boundary_range_generic()
    : g(0)
  { default_init();} 
  boundary_range_generic(grid_type const& gg) 
    : g(&gg)
  { 
    default_init();
    init();
  } 

  VertexIterator FirstVertex() const;
  FacetIterator  FirstEdge()    const;
  FacetIterator  FirstFacet()   const;

  int NumOfVertices() const { 
    if(num_of_vertices < 0) num_of_vertices = compute_num_of_vertices();
    return num_of_vertices;
  }
  int NumOfEdges() const {
    if(num_of_edges < 0) num_of_edges = compute_num_of_edges();
    return num_of_edges;
  }
  int NumOfFacets() const { return NumOfEdges();}

  int NumOfComponents() const { return component_germs.size();}

  component_type Component(int i) const;

  grid_type const& TheGrid() const { 
    REQUIRE((g != 0), "No grid!\n",1);
    return *g;
  }

private:
  void default_init() {
    initialized     = false;
    num_of_vertices = -1;
    num_of_edges    = -1;
  }
  int compute_num_of_vertices() const;
  int compute_num_of_edges()    const;

  void init() const { 
    self * my = (self *) this;
    find_boundary_component_germs(TheGrid(), my->component_germs);
    my->initialized = true;
  }

};



/*! \internal 
    \ingroup boundaryrange
    \brief Iterator over vertices and edges of \e all boundary components of a grid.
     
 */
template<class GRID>
class boundary_range_iterator
{
  typedef GRID                grid_type;
  typedef grid_types<GRID>    gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;

  typedef boundary_range_generic<GRID>          bd_range_type;
  typedef typename bd_range_type::germ_iterator germ_iterator;

  typedef iscellinside_pred<grid_type>  inside;
  typedef BoundaryComponentIterator2D<grid_type, inside> BoundaryComponentIterator;


private:
  boundary_range_generic<GRID> const* r;
  germ_iterator                germ;
  BoundaryComponentIterator    bc;

  typedef  boundary_range_iterator<GRID> self;
public:
  boundary_range_iterator(boundary_range_generic<GRID> const& BR)
    : r(&BR), 
      germ(BR.component_germs.begin())
  {
    if(BR.NumOfComponents() != 0)
      bc = BoundaryComponentIterator(*germ, inside(BR.TheGrid()));
  }
  
  self& operator++()
    {
      ++bc;
      if(bc.IsDone()) {
	++germ;
	if( germ != r->component_germs.end())
	  bc = BoundaryComponentIterator(*germ,inside(TheGrid()));
      }
      return *this;
    }
  bool  IsDone() const { return ( germ == r->component_germs.end());}
  
  grid_type const&  TheGrid() const { return r->TheGrid(); }
  Vertex curr_vertex() const { return bc.curr_vertex();}
  Edge   curr_edge()   const { return bc.curr_edge();}
  Facet  curr_facet()  const { return bc.curr_facet();}
};


/*! \internal
    \ingroup boundaryrange
    \brief Iterator over the vertices of \e all boundary components of a grid.
     
 */
template<class GRID>
class boundary_range_vertex_iterator {
public:
  typedef GRID                grid_type;
  typedef grid_types<GRID>    gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;

  typedef grid_sequence_iterator_category_d<0> category;
private:
  boundary_range_iterator<GRID> base;

  typedef boundary_range_vertex_iterator<GRID> self;
public:
  boundary_range_vertex_iterator() {}
  boundary_range_vertex_iterator(boundary_range_iterator<GRID> const& b) 
    : base(b) {}

  Vertex operator*() const { return base.curr_vertex();}
  bool   IsDone()    const { return base.IsDone();}
  self&  operator++() { ++base; return (*this);}

  grid_type const&  TheGrid() const { return base.TheGrid();}

};

/*! \internal 
    \ingroup boundaryrange
    \brief Iterator over the edges of \e all boundary components of a grid.
     
 */
template<class GRID>
class boundary_range_edge_iterator {
public:
  typedef GRID                grid_type;
  typedef grid_types<GRID>    gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;

  typedef grid_sequence_iterator_category_d<1> category;
private:
  boundary_range_iterator<GRID> base;

  typedef boundary_range_edge_iterator<GRID> self;
public:
  boundary_range_edge_iterator() {}
  boundary_range_edge_iterator(boundary_range_iterator<GRID> const& b) 
    : base(b) {}

  Edge operator*() const { return base.curr_edge();}
  bool   IsDone()    const { return base.IsDone();}
  self&  operator++() { ++base; return (*this);}

  grid_type const&  TheGrid() const { return base.TheGrid();}

};





/*! \internal 
    \ingroup boundaryrange
    \brief Iterator over vertices and edges of \e one boundary component of a grid.
     
 */
template<class GRID>
class boundary_component_multielem_iterator
{
  typedef GRID                grid_type;
  typedef grid_types<GRID>    gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;

  typedef boundary_range_generic<GRID>          bd_range_type;
  typedef typename bd_range_type::germ_type     germ_type;

  typedef iscellinside_pred<grid_type>          inside;
  typedef BoundaryComponentIterator2D<grid_type, inside> BoundaryComponentIterator;


private:
  boundary_range_generic<GRID> const* r;
  germ_type                    germ;
  BoundaryComponentIterator    bc;

  typedef  boundary_component_multielem_iterator<GRID> self;
public:
  boundary_component_multielem_iterator(boundary_range_generic<GRID> const& BR,
					germ_type grm)
    : r(&BR), 
      germ(grm),
      bc(grm, inside(BR.TheGrid()))
  {}
  
  self& operator++()
  {
    ++bc;
    return *this;
  }
  bool  IsDone() const { return  bc.IsDone();}
  
  grid_type const&  TheGrid() const { return r->TheGrid(); }
  Vertex curr_vertex() const { return bc.curr_vertex();}
  Edge   curr_edge()   const { return bc.curr_edge();}
  Facet  curr_facet()  const { return bc.curr_facet();}
  
  Vertex curr(vertex_type_tag) const { return curr_vertex();}
  Edge   curr(edge_type_tag)   const { return curr_edge();}
};


/*! \internal
    \ingroup boundaryrange
    \brief Iterator over the vertices of \e one boundary component of a grid.
     
 */
  template<class GRID, int D>
  class boundary_component_element_iterator {
public:
    typedef GRID                grid_type;
    typedef grid_types<GRID>    gt;
    typedef typename gt::Vertex Vertex;
    typedef typename gt::Edge   Edge;
    typedef typename gt::Facet  Facet;
    typedef typename element_d<gt,D>::type element_type;

    typedef grid_sequence_iterator_category_d<D> category;
  private:
    typedef boundary_component_multielem_iterator<GRID> base_type;
    base_type base;
    
    typedef boundary_component_element_iterator<GRID,D> self;
  public:
    boundary_component_element_iterator() {}
    boundary_component_element_iterator(base_type const& b) 
      : base(b) {}
    
    element_type operator*() const { return base.curr(typename int2element_tag<2,D,2-D>::type());}
    bool   IsDone()    const { return base.IsDone();}
    self&  operator++() { ++base; return (*this);}
    
    grid_type const&  TheGrid() const { return base.TheGrid();}
  };


template<class GRID>
class boundary_component 
{
public:
  typedef boundary_range_generic<GRID>            boundary_range_type;
  typedef typename boundary_range_type::germ_type germ_type;

  typedef grid_range_category_d<2>   category;
private:
  GrAL::ref_ptr<boundary_range_type const> range;
  germ_type                                germ;
  mutable int                              num_of_vertices;
  mutable int                              num_of_edges;

public:
  boundary_component(boundary_range_type const& r, germ_type grm)
    : range(r), germ(grm), num_of_vertices(-1), num_of_edges(-1) {}

  typedef boundary_component_multielem_iterator<GRID> multi_iterator;
  typedef boundary_component_element_iterator<GRID,0> VertexIterator;
  typedef boundary_component_element_iterator<GRID,1> EdgeIterator;
  typedef EdgeIterator                                FacetIterator;

  VertexIterator FirstVertex()   const { return VertexIterator(multi_iterator(*range,germ));}
  EdgeIterator   FirstEdge()     const { return EdgeIterator  (multi_iterator(*range,germ));}
  FacetIterator  FirstFacet()    const { return FirstEdge();}

  int            NumOfVertices() const {
    if(num_of_vertices < 0) num_of_vertices = iterator_range_size(FirstVertex());
    return num_of_vertices;
  }
  int            NumOfEdges() const {
    if(num_of_edges < 0) num_of_edges = iterator_range_size(FirstEdge());
    return num_of_edges;
  }
  int            NumOfFacets() const { return NumOfEdges();}
};

//---------------- inline functions ---------------------

template<class GRID>
inline
boundary_range_vertex_iterator<GRID>
boundary_range_generic<GRID>::FirstVertex() const 
{ 
  if ( ! initialized) 
    init();
  return VertexIterator(*this);
}

template<class GRID>
inline
boundary_range_edge_iterator<GRID>
boundary_range_generic<GRID>::FirstFacet() const
    { 
      if ( ! initialized) 
	init();
      return FacetIterator(*this);
    }

template<class GRID>
inline
boundary_range_edge_iterator<GRID>
boundary_range_generic<GRID>::FirstEdge() const
{ return FirstFacet();}


template<class GRID>
inline
int
boundary_range_generic<GRID>::compute_num_of_vertices() const
{ 
  int res = 0;
  for(VertexIterator v = FirstVertex(); !v.IsDone(); ++v)
    ++res;
  return res;
}

template<class GRID>
inline
int
boundary_range_generic<GRID>::compute_num_of_edges() const
{ 
  int res = 0;
  for(EdgeIterator e = FirstEdge(); !e.IsDone(); ++e)
    ++res;
  return res;
}


template<class GRID>
inline
boundary_component<GRID> 
boundary_range_generic<GRID>::Component(int i) const
{ 
  REQUIRE(0 <= i && i < NumOfComponents(), "i=" << i,1);
  return component_type(*this, component_germs[i]);
}


template<class GRID>
struct grid_types_boundary_range_generic  
  : public grid_types_detail::grid_types_root
{
  typedef GRID                             grid_type;
  typedef boundary_range_generic<GRID>     range_type;
  typedef grid_types<grid_type>            bgt;
  typedef typename bgt::dimension_tag      dimension_tag;

  typedef typename range_type::Facet          Facet;
  typedef typename range_type::FacetIterator  FacetIterator;
  typedef typename range_type::Edge           Edge;
  typedef typename range_type::EdgeIterator   EdgeIterator;
  typedef typename range_type::Vertex         Vertex;
  typedef typename range_type::VertexIterator VertexIterator;

  // typedef typename GT::VertexOnFacetIterator  VertexOnFacetIterator;
};


template<class GRID>
struct grid_types<boundary_range_generic<GRID> >
  : public grid_types_base<grid_types_boundary_range_generic<GRID> >
{ };



template<class GRID>
struct grid_types_boundary_component  
  : public grid_types_detail::grid_types_root
{
  typedef GRID                             grid_type;
  typedef boundary_component<GRID>         range_type;
  typedef grid_types<grid_type>            bgt;
  typedef typename bgt::dimension_tag      dimension_tag;


  typedef typename bgt::Vertex                Vertex;
  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename bgt::Edge                  Edge;
  typedef typename range_type::EdgeIterator   EdgeIterator;
  typedef typename bgt::Facet                 Facet;
  typedef typename range_type::FacetIterator  FacetIterator;
};


template<class GRID>
struct grid_types<boundary_component<GRID> >
  : public grid_types_base<grid_types_boundary_component<GRID> >
{ };



} // namespace GrAL 

#endif
