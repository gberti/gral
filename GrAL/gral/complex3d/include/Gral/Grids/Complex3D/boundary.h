#ifndef GRAL_GB_COMPLEX3D_BOUNDARY_H
#define GRAL_GB_COMPLEX3D_BOUNDARY_H

// $LICENSE_NEC_2003

#include "Gral/Base/boundary.h"


#include "Gral/Base/vertex-set.h"
#include "Gral/Base/facet-on-cell-function.h"

#include "Gral/Iterators/cell-on-cell-iterator.h"
#include "Gral/Algorithms/cell-neighbor-search.h"

#include "Container/my-hash-map.h"

#include "Gral/Grids/Complex3D/complex3d.h"
#include "Gral/Grids/Complex3D/grid-functions.h"


namespace complex3d_detail {

  class BdFacetIterator;

  struct basic_grid_types {
    typedef grid_types<Complex3D> gt;
    typedef facet_on_cell_function<gt::grid_type, gt::cell_handle> nb_table_type;

    typedef STDHASH::hash_map<vertex_set<gt::Facet>, 
			      gt::FacetOnCellIterator,
			      STDEXT::hash<vertex_set<gt::Facet> > > facet_map_type;

    typedef BdFacetIterator                         FacetIterator;
    typedef BdFacetIterator                         FaceIterator;
    // typedef vertex_iterator_of_cell_set<basic_grid_types> VertexIterator;
    // typedef edge_iterator_of_cell_set<basic_grid_types>   EdgeIterator;

    typedef gt::Vertex Vertex;
    typedef gt::Edge   Edge;
    typedef gt::Face   Face;
    typedef gt::Facet  Facet;

    typedef gt::vertex_handle vertex_handle;
    typedef gt::edge_handle   edge_handle;
    typedef gt::face_handle   face_handle;
    typedef gt::facet_handle  facet_handle;

    typedef gt::VertexOnFacetIterator VertexOnFacetIterator;
    typedef gt::VertexOnFaceIterator  VertexOnFaceIterator;
    typedef gt::EdgeOnFacetIterator   EdgeOnFacetIterator;
    typedef gt::EdgeOnFaceIterator    EdgeOnFaceIterator;
    typedef gt::VertexOnEdgeIterator  VertexOnEdgeIterator;
  }; // struct basic_grid_types


} // namespace complex3d_detail 

/*! \defgroup complex3dboundary Boundary Ranges for Complex3D
    \ingroup complex3dmodule 
*/

/*! \brief Specialization of the BoundaryRange primary template
    \ingroup complex3dboundary

    Model of $GrAL FacetGridRange.

    \see Test in \ref test-boundary.C
 */
template<>
class BoundaryRange<Complex3D> : public complex3d_detail::basic_grid_types
{
public:
  typedef Complex3D grid_type;
private:
  friend class complex3d_detail::BdFacetIterator;

  ref_ptr<grid_type const> g;
  facet_map_type facet_map;

public:
  //!
  BoundaryRange() {}
  //!
  BoundaryRange(grid_type const& gg) : g(gg) { init();}

  //! Anchor grid
  ref_ptr<grid_type const> TheGrid() const { return g;}

  //!
  void init() 
  {
   nb_table_type nbs(*TheGrid(), -1);
   CalculateNeighborCells(nbs,*TheGrid(),facet_map, gt());
  }

  //!
  FacetIterator FirstFacet() const;
  //!
  FacetIterator EndFacet()   const;
  //!
  unsigned NumOfFacets()     const { return facet_map.size();}
}; // class BoundaryRange<Complex3D>

/*! \brief Specialization of the grid_types primary template
  
   \ingroup complex3dboundary
 */
template<>
struct grid_types<BoundaryRange<Complex3D> >
  : public complex3d_detail::basic_grid_types 
{

};

namespace complex3d_detail {


  class BdFacetIterator : public basic_grid_types {
    typedef BdFacetIterator self;

    typedef BoundaryRange<Complex3D> range_type;
    typedef facet_map_type::const_iterator range_iter;
    ref_ptr<range_type const>    rge;
    range_iter                   it;
  public:
    BdFacetIterator() {}
    BdFacetIterator(range_type const& r);
    BdFacetIterator(range_type const& r, range_iter i) : rge(r), it(i) {}

    self& operator++() { ++it; return *this;}
    Facet operator*() const { return val();}
    Facet val()   const { return *((*it).second);}
    inline bool IsDone() const;
    facet_handle handle() const { return val().handle();}

    gt::VertexOnFacetIterator FirstVertex() const { return val().FirstVertex();}
    gt::VertexOnFacetIterator EndVertex()   const { return val().EndVertex();}
    unsigned NumOfVertices() const { return val().NumOfVertices();}
  }; // class BdFacetIterator


  inline BdFacetIterator::BdFacetIterator(BdFacetIterator::range_type const& r) 
    : rge(r), it(rge->facet_map.begin()) {}
 
  inline bool BdFacetIterator::IsDone()  const { return it == rge->facet_map.end();}

} //  namespace complex3d_detail


inline
BoundaryRange<Complex3D>::FacetIterator BoundaryRange<Complex3D>::FirstFacet() const 
{ return FacetIterator(*this);}

inline
BoundaryRange<Complex3D>::FacetIterator BoundaryRange<Complex3D>::EndFacet()   const 
{ return FacetIterator(*this, facet_map.end());}

#endif
