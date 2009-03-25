#ifndef GRAL_BASE_GB_WRAPPED_GEOMETRY_H
#define GRAL_BASE_GB_WRAPPED_GEOMETRY_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

namespace GrAL {

/*! \brief Simple wrapper for a grid geometry and grid view / subrange

   \ingroup gridgeometries

   The purpose of this class is simply to supply a geometry with the right
   \c TheGrid() function for a grid view to a base grid, given a geometry referencing the base grid

   <pre>
           Geom                    ---->   Grid

            ^                                ^
            |                                |
            
    wrapped_geometry<Geom, Grid>   ----> grid_view<Grid>
   </pre>
          

   \templateparams
   - Geom: Model of $GrAL VertexGridGeometry
   - Grid: Model of $GrAL GridRange

   \todo Test

  \see Module gridgeometries
*/  

template<class Geom, class Grid>
class wrapped_geometry {
public:
  typedef Grid                        grid_type;
  typedef Geom                        base_geom_type;
  typedef grid_types<grid_type>       gt;
  typedef typename Geom::grid_type    base_grid_type;
  typedef grid_types<base_grid_type>  bgt;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::Edge           Edge;
  typedef typename gt::Cell           Cell;
  typedef typename bgt::Vertex        baseVertex;
  typedef typename Geom::coord_type   coord_type;
private:
  ref_ptr<const grid_type>       g;
  ref_ptr<const base_geom_type>  geom;
public:
  wrapped_geometry(Grid const& gg, Geom const& geo) : g(gg), geom(geo) {}

  coord_type coord (Vertex const& v)  const { return geom->coord(baseVertex(v));}
  double     length(Edge   const& e)  const { return geom->length(baseEdge(e));}
  coord_type center(Cell   const& c)  const { return geom->center(baseCell(c));}
  double     volume(Edge   const& e)  const { return geom->volume(baseEdge(e));}
  double     volume(Cell   const& c)  const { return geom->volume(baseCell(c));}

  grid_type const& TheGrid() const { return *g;}

  bool empty() const { cb(); return TheGrid().NumOfVertices() == 0;}
  // does not work for empty grid
  int space_dimension() const { cb(); return geom->space_dimension();}

  bool bound() const { return geom->bound();}
  void rebind(grid_type const& gg) { gg = g;}
  void cb() const { REQUIRE(bound(), "", 1);}
};

/*! \brief Creator function for wrapped_geom<Geom,Grid>
    \ingroup gridgeometries    
    \relates wrapped_geometry<Geom,Grid>
 */
template<class Geom, class Grid>
wrapped_geometry<Geom, Grid>
inline
WrapGeom(Grid const& g,Geom const& geom)
{ return wrapped_geometry<Geom, Grid>(g,geom);}

} // namespace GrAL 

#endif
