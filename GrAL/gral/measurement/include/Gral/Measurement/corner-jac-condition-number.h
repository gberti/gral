#ifndef GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_H
#define GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_H

// $LICENSE_NEC

#include "Gral/Base/flag.h"
#include "Geometry/algebraic-primitives.h"
#include "Container/partial-mapping.h"


/*! \brief Class for calculating condition numbers of 
    jacobians of cell corners

    \todo works for 3D only
    \todo assumes each cell corner has 3 edges incident to the cell
    \todo has to introduce weighting matrices for tetrahedrons,
    where the ideal cell corner is that of a regular tet and not of a cube. 
 */
template<class GRID, class GEOM>
class corner_jacobian {
public:
  typedef GRID grid_type;
  typedef GEOM geom_type;
  typedef grid_types<grid_type> gt;

  typedef typename geom_type::coord_type    coord_type;
  typedef point_traits<coord_type>          pt;
  typedef algebraic_primitives<coord_type>  ap;
  typedef typename ap::real                 real;

  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  typedef typename gt::archetype_type archetype_type;
  typedef grid_types<archetype_type>  agt;
  typedef typename agt::Vertex        ArchVertex;

private:
  grid_type const* g;
  geom_type const* geom;

  typedef grid_function<ArchVertex, flag<archetype_type> >  vertex_star_map;
  // Cell -> (ArchVertex -> star) 
  partial_mapping<archetype_type const*, vertex_star_map> vertex_stars;


public: 
  corner_jacobian(grid_type const& gg, geom_type const& ggeom)
    : g(&gg), geom(&ggeom) {}

  real condition(VertexOnCellIterator const& corner);
  void set_vertex_stars(archetype_type const& A);
  // real worst_corner_condition(Cell const& c);

  grid_type const& TheGrid() const { return *g;}
  geom_type const& TheGeom() const { return *geom;}

};



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Measurement/corner-jac-condition-number.C"
#endif

#endif
