#ifndef GRAL_GB_BASE_FACET_GRID_H
#define GRAL_GB_BASE_FACET_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Subranges/enumerated-subrange.h"

// for vertex_set_of_facets_iterator<>
#include "Gral/Base/element-sequence-references.h"

namespace GrAL {

/*! \internal
    \brief helper class for facet_grid<FACETRANGE>

 */
template<class FACETRANGE>
struct grid_types_facet_grid {
  typedef grid_types<FACETRANGE> fgt;
  //typedef typename fgt::grid_type base_grid_type; 
  typedef typename FACETRANGE::grid_type base_grid_type;
  typedef grid_types<base_grid_type> bgt;

  typedef typename bgt::Facet         Cell;
  typedef typename bgt::Vertex        Vertex;
  typedef typename bgt::facet_handle  cell_handle;
  typedef typename bgt::vertex_handle vertex_handle;
  typedef typename bgt::VertexOnFacetIterator VertexOnCellIterator;

  // typedef typename fgt::FacetIterator  CellIterator;
  typedef typename FACETRANGE::FacetIterator  CellIterator;
};


/*! \brief View a facet subrange as a d-1 dimensional grid

    \ingroup subranges   
    Model of $GrAL Cell-VertexInputGridRange
    \see \ref subranges module
 */
template<class FACETRANGE>
class facet_grid
  : public grid_types_facet_grid<FACETRANGE>  {
private:
  typedef grid_types_facet_grid<FACETRANGE>       gt;
public:
  typedef typename gt::base_grid_type                base_grid_type;
  typedef enumerated_vertex_range<base_grid_type>    vertex_range_type;
  typedef FACETRANGE                                 facet_range_type;
  typedef typename vertex_range_type::VertexIterator VertexIterator;
  typedef typename gt               ::CellIterator   CellIterator;
private:
 FACETRANGE const* facets;
 vertex_range_type vertices;

public:
  facet_grid() : facets(0) {}

  facet_grid(FACETRANGE const& f) : facets(&f), vertices(facets->TheGrid()) 
    {
      vertex_set_of_facets_iterator<FACETRANGE> v(*facets);
      while(! v.IsDone()) {
	vertices.push_back(*v);
	++v;
      }
    }

 

  unsigned NumOfCells()    const { return facets->NumOfFacets();}
  unsigned NumOfVertices() const { return vertices.NumOfVertices();}

  CellIterator   FirstCell()   const { return facets->FirstFacet();}
  VertexIterator FirstVertex() const { return vertices.FirstVertex();}
};

template<class FACETRANGE>
struct grid_types<facet_grid<FACETRANGE> > 
  : public grid_types_facet_grid<FACETRANGE>
{
  typedef typename facet_grid<FACETRANGE>::VertexIterator VertexIterator; 
};


} // namespace GrAL 

#endif
