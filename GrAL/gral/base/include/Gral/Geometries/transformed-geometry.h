#ifndef GRAL_BASE_GB_TRANSFORMED_GEOMETRY_H
#define GRAL_BASE_GB_TRANSFORMED_GEOMETRY_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Base/common-grid-basics.h"
#include "Geometry/algebraic-primitives.h"

//----------------------------------------------------------------
// 
// Grid geometry transformed by a mapping.
// Only function is currently transformation of vertices --
// every other geometric primitives depends on the nature of
// the transformation, i.e linear or non-linear.
//
//----------------------------------------------------------------

template<class Geom, class Trafo>
class transformed_geom {
public:
  typedef typename Geom::grid_type grid_type;
  typedef grid_types<grid_type>       gt;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::Edge           Edge;
  typedef typename Trafo::result_type coord_type;
  typedef algebraic_primitives<coord_type> ap;
private:
  Geom  const*  geom;
  Trafo const*  f;
public:
  transformed_geom(Geom const& geo, Trafo const& trf) : geom(&geo), f(&trf) {}

  coord_type coord(const Vertex& V) const { return (*f)(geom->coord(V));}
  double     length(const Edge& E)  const { return ap::distance(coord(E.V1()),coord(E.V2()));}

  grid_type const& TheGrid() const { return geom->TheGrid();}

};

template<class Geom, class Trafo>
transformed_geom<Geom, Trafo>
TransformGeom(Geom const& geom, Trafo const& f)
{ return transformed_geom<Geom, Trafo>(geom,f);}

#endif
