#ifndef GRAL_BASE_SUBRANGES_BOUNDARY_RANGE_ND_H
#define GRAL_BASE_SUBRANGES_BOUNDARY_RANGE_ND_H


/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Algorithms/cell-neighbor-search.h"
#include "Gral/Base/facet-on-cell-function.h"
#include "Gral/Base/vertex-set.h"
#include "Gral/Base/element-sequence-references.h" // vertex-iterator-of-facet-set.h

#include "Container/my-hash-map.h"
#include "Utility/ref-ptr.h"

namespace GrAL {

  /*!  \brief Generic boundary range for arbitrary grids

       \ingroup boundary
       \ingroup subranges

       The class gives access to the boundary facets and vertices
       of an arbitrary grid.
       \templateparams
        - \c GRID :
          - Model of $GrAL CellRange
          - types \c Facet,  \c FacetOnCellIterator
          - types \c Vertex, \c VertexOnFacetIterator

       \see Tested in \ref test-boundary-range-nd.C

       \todo Specialization for grids already supporting boundary ranges.
       \todo Support for edge iteration
   */
template<class GRID, class GT = grid_types<GRID> >
class boundary_range_nd {
  typedef boundary_range_nd<GRID, GT> self;
public:
  typedef typename GT::dimension_tag  dimension_tag;
  // FIXME: is this correct? No if this were a grid, not a grid_range !
  enum { dim = dimension_tag::dim };
  typedef grid_range_category_d<dim>  category;

  typedef GRID                        grid_type;
  typedef GT                          gt;
  typedef typename gt::Cell           Cell;
  typedef typename gt::Facet          Facet;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::cell_handle    cell_handle;
  typedef typename gt::FacetOnCellIterator FacetOnCellIterator;

  typedef enumerated_facet_range<grid_type>  facet_range_type;
  //typedef grid_types<facet_range_type>    fgt;
  typedef typename facet_range_type::FacetIterator        FacetIterator;
  typedef vertex_set_of_facets_iterator<facet_range_type> VertexIterator;


  unsigned dimension() const { return dim;}
private:

  ref_ptr<grid_type const>   g;
  facet_range_type           boundary_facets;
public:
  boundary_range_nd(grid_type const& gg) : g(gg) 
  { init();}

  FacetIterator FirstFacet()  const { return boundary_facets.FirstFacet();}
  FacetIterator EndFacet()    const { return boundary_facets.EndFacet();}
  int           NumOfFacets() const { return boundary_facets.NumOfFacets();}
  operator facet_range_type const& () const { return boundary_facets;}
  VertexIterator FirstVertex() const { return VertexIterator(boundary_facets);}

  void init();
};


template<class GRID, class GT>
void boundary_range_nd<GRID,GT>::init() 
{
  boundary_facets.set_grid(*g);
  typedef facet_on_cell_function<grid_type, cell_handle>       nbf_type;
  typedef STDHASH::hash_map<vertex_set<Facet>, 
                            FacetOnCellIterator,
                            STDEXT::hash<vertex_set<Facet> > > facet_map_type;

  nbf_type       nbs(*g,cell_handle(-1));
  facet_map_type facet_map;
  CalculateNeighborCells(nbs, *g, facet_map);
  for(typename facet_map_type::iterator f=facet_map.begin(); f !=facet_map.end(); ++f)
    boundary_facets.push_back( *(f->second));
}

template<class GRID, class GT>
struct grid_types_boundary_range_nd 
{
  typedef GRID                        grid_type;
  typedef boundary_range_nd<GRID>     range_type;
  typedef typename GT::dimension_tag  dimension_tag;

  typedef typename range_type::Facet          Facet;
  typedef typename range_type::FacetIterator  FacetIterator;
  typedef typename range_type::Vertex         Vertex;
  typedef typename range_type::VertexIterator VertexIterator;
  // perhaps we should not define incidence iterators in sub*ranges* ... ?
  // at least no upward incidence iterators 
  typedef typename GT::VertexOnFacetIterator  VertexOnFacetIterator;
};


template<class GRID, class GT>
struct grid_types<boundary_range_nd<GRID,GT> >
  : public grid_types_base<grid_types_boundary_range_nd<GRID,GT> >
{ };

} // namespace GrAL

#endif
