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
*/
//----------------------------------------------------------------


template<class GRID>
class boundary_range_iterator;

template<class GRID>
class boundary_range_vertex_iterator;

template<class GRID>
class boundary_range_edge_iterator;


/*! \ingroup boundaryrange
    \brief Class representing the boundary of a grid.

    \see \ref boundaryrange module 
 */
template<class GRID>
class boundary_range_generic {
  typedef boundary_range_generic<GRID> self;

  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
  typedef typename gt::Facet  Facet;
  
  friend class boundary_range_iterator<GRID>;
  typedef boundary_range_vertex_iterator<GRID> VertexIterator;
  typedef boundary_range_edge_iterator<GRID>   EdgeIterator;
  typedef boundary_range_edge_iterator<GRID>   FacetIterator;

  typedef typename  ::std::list<Facet>::const_iterator germ_iterator;
private:
   ::std::list<Facet> component_germs;
  grid_type const* g;
  bool initialized;
public:
  boundary_range_generic()
    : g(0), initialized(false) {} 
  boundary_range_generic(grid_type const& gg) 
    : g(&gg), initialized(false) {} 

  VertexIterator FirstVertex() const;
  FacetIterator  FirstEdge()    const;
  FacetIterator  FirstFacet()   const;

  grid_type const& TheGrid() const { 
    REQUIRE((g != 0), "No grid!\n",1);
    return *g;
  }

private:
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
      germ(BR.component_germs.begin()),
      bc(*germ,inside(BR.TheGrid()))
    {}
  
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
  typedef GRID                grid_type;
  typedef grid_types<GRID>    gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;

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
  typedef GRID                grid_type;
  typedef grid_types<GRID>    gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;

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


} // namespace GrAL 

#endif
