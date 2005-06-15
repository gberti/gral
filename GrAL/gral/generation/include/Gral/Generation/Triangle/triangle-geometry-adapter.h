#ifndef GRAL_MG_TRIANGLE_GEOMETRY_ADAPTER_H
#define GRAL_MG_TRIANGLE_GEOMETRY_ADAPTER_H

#include "triangle-grid-adapter.h"
#include "Geometry/coords.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>
#include <cmath>

namespace GrAL {

/*! \brief Geometry adapter for Shewchuk's Triangle triangulatio data structure
 * to the geometry micro kernel

   It is a model of $GrAL VertexGridGeometry.
*/
class TriangleGeometryAdapter {
public:
  typedef TriangleGridAdapter       grid_type; 
  typedef grid_types<grid_type>     gt;
  typedef gt::Vertex                Vertex;
  typedef coordN<2> coord_type;
  typedef point_traits<coord_type> pt;
  typedef algebraic_primitives<coord_type> ap;

private:
  grid_type const* _g;
  const double * _coordlist;
  void init_coords() {
    _coordlist = _g->getCoordsList();
  }

public:
  //! Empty geometry
  TriangleGeometryAdapter() : _g(0), _coordlist(0) {}
  TriangleGeometryAdapter(grid_type const& g) : _g(&g) { 
      init_coords();
  }

  //! Dimension of embedding space
  unsigned space_dimension() const { return 2;}
public:
  //! Anchor grid
  grid_type const& TheGrid() const { return *_g;}

  //! Representation of real numbers
  typedef  pt::component_type       scalar_type;

  //! coordinate of Vertex (read access)
  coord_type  coord(Vertex const& v) const { 
    const double *cs = _coordlist + 2*v.handle();
    return coord_type(*cs, *(cs+1));} 

  //! Length of (straight) edge
  //inline scalar_type length(Edge const& e) const;

  //! \f$ d-1 \f$ dimensional volume of Facet (i.e. edge length)
  //scalar_type volume(Facet const& f)  const { return length(f);}

  //! Center of intertia of \c c
  //coord_type center(Cell const& c) const { return (coord(c.V(0)) + coord(c.V(1)) + coord(c.V(2)))/3.0;}

  //! Barycenter/Centroid (average of vertices) of cell \c c
  //coord_type barycenter(Cell const& c) const { return center(c);}

  //! solid angle of the wedge of vertex \c vc, in radians (2D) 
  //inline scalar_type solid_angle(VertexOnCellIterator const& vc) const;

  /*! ratio of solid angle of wedge \c vc to complete solid angle

      The ratios of the wedges of an internal regular vertex sum up to 1.
   */
  //scalar_type solid_angle_ratio(VertexOnCellIterator const& vc) const { return solid_angle(vc)/(2*M_PI);}
};

inline std::ostream&
operator<<(std::ostream& out, TriangleGeometryAdapter::coord_type const& p)
{ 
  typedef TriangleGeometryAdapter::pt pt;
  return (out << pt::x(p) << ' ' << pt::y(p));}

} // namespace GrAL 

#endif /* ifndef GRAL_MG_TRIANGLE_GEOMETRY_ADAPTER_H */
