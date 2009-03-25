#ifndef GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_H
#define GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/flag.h"
#include "Geometry/algebraic-primitives.h"
#include "Container/partial-mapping.h"

namespace GrAL {

namespace measurement {

/*! \brief Class for calculating condition numbers of 
    jacobians of cell corners

    The corner condition number as quality measure for cells has been described in
    Patrick M. Knupp, <em>Matrix Norms &amp; The Condition Number: A General Framework
    to Improve Mesh Quality Via Node-Movement </em>,
    Proceedings of 8th International Meshing RoundTable, 1999.

    \todo works for 3D only
    \todo assumes each cell corner has 3 edges incident to the cell
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
  typedef typename ap::matrix_type          matrix_type;

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
  // mapping  Cell -> (ArchVertex -> star) 
  partial_mapping<archetype_type const*, vertex_star_map> vertex_stars;

  geom_type const* ideal_geom;
  matrix_type      inv_ideal_corner;

public: 
  /*! \brief Initialize
      \post
         \c condition() can be called.
   */
  corner_jacobian(grid_type const& gg, geom_type const& ggeom)
    : g(&gg), geom(&ggeom), ideal_geom(0), inv_ideal_corner(matrix_type::UnitMatrix())
  { }

  //! \brief Get condition number at a cell corner.
  real condition(VertexOnCellIterator const& corner);

  /*! \brief Teach what is an "ideal" corner

      By default, the corner of a unit cube is considered "ideal".
      However, for a tetrahedron, the corner of a equilateral tetrahedron
      would be ideal.
   */
  void set_ideal_corner(VertexOnCellIterator const& ideal_corner_, 
			geom_type            const& ideal_geom_);


  grid_type const& TheGrid() const { return *g;}
  geom_type const& TheGeom() const { return *geom;}
private:
  void get_edges(VertexOnCellIterator const& corner_, 
		 geom_type            const& geom_,
		 matrix_type               & edges);

  void set_vertex_stars(archetype_type const& A);
  // real worst_corner_condition(Cell const& c);
};

} //  namespace measurement

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Measurement/corner-jac-condition-number.C"
#endif

#endif
