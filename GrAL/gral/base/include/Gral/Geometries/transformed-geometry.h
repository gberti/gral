#ifndef GRAL_BASE_GB_TRANSFORMED_GEOMETRY_H
#define GRAL_BASE_GB_TRANSFORMED_GEOMETRY_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Geometry/algebraic-primitives.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

/*! \brief Grid geometry transformed by a mapping.

   \ingroup gridgeometries

   The only function is currently transformation of vertices --
   every other geometric primitive depends on the nature of
   the transformation, i.e linear or non-linear.
 
   \templateparams
   - Geom: Model of $GrAL VertexGridGeometry
   - Trafo: Model of Unary Function <BR>
    (<tt>Geom::coord_type -> Trafo::result_type</tt>)

   \todo 
   Extend approach to yield transformed (i.e., curved) segments,
   polygons etc.
   \todo Test

  \see Module gridgeometries
*/  

template<class Geom, class Trafo>
class transformed_geom {
public:
  typedef typename Geom::grid_type grid_type;
  typedef grid_types<grid_type>       gt;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::Edge           Edge;
  typedef typename Trafo::result_type coord_type;
  typedef point_traits<coord_type>         pt;
  typedef algebraic_primitives<coord_type> ap;
private:
  Geom  const*  geom;
  Trafo const*  f;
public:
  transformed_geom(Geom const& geo, Trafo const& trf) : geom(&geo), f(&trf) {}

  coord_type coord(const Vertex& V) const { return (*f)(geom->coord(V));}

  // FIXME: not correct for nonlinear mapping
  double     length(const Edge& E)  const { return ap::distance(coord(E.V1()),coord(E.V2()));}

  grid_type const& TheGrid() const { return geom->TheGrid();}

  bool empty() const { cb(); return TheGrid().NumOfVertices() == 0;}
  // does not work for empty grid
  int space_dimension() const { cb(); return pt::Dim(coord(* (TheGrid().FirstVertex())));}

  bool bound() const { return geom->bound();}
  void rebind(grid_type const& g) { geom->rebind(g);}
  void cb() const { REQUIRE(bound(), "", 1);}
};

/*! \brief Creator function for transformed_geom<Geom,Trafo>
    \ingroup gridgeometries    
    \relates transformed_geom<Geom,Trafo>
 */
template<class Geom, class Trafo>
transformed_geom<Geom, Trafo>
inline
TransformGeom(Geom const& geom, Trafo const& f)
{ return transformed_geom<Geom, Trafo>(geom,f);}

} // namespace GrAL 

#endif
