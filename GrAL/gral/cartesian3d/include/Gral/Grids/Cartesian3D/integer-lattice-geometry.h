#ifndef GRAL_GB_GRIDS_CARTESIAN3D_LATTICE_GEOMETRY_H
#define GRAL_GB_GRIDS_CARTESIAN3D_LATTICE_GEOMETRY_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

namespace cartesian3d {

/*! \brief A geometry for CartesianGrid3D 
    living on the integer lattice \f$ \Z^3 \f$
   
    \ingroup cartesian3dmodule

    Model of $GrAL VertexGridGeometry (immutable).
 */

class lattice_geometry {

public:
  typedef CartesianGrid3D             grid_type;
  typedef grid_types<CartesianGrid3D> gt;
  typedef gt::Vertex                  Vertex; 

  //FIXME: we cannot represent e.g. cell centers with this type!
  typedef grid_type::index_type   coord_type;
  typedef int                     real;

private:
  grid_type const* g;

public:
  //! Construct over grid \c gg
  lattice_geometry(grid_type const& gg)
    : g(&gg)
    {}

  //! Coordinate of vertex
  coord_type coord(gt::Vertex const& v) const { return v.index();}

  //@{
  /*! \name Volume calculation
     \brief Dimension-dependent volumes of the elements
   */
  real volume(gt::Cell const&) const { return 1;}
  real volume(gt::Edge const&) const { return 1;}
  real volume(gt::Face const&) const { return 1;}
  //@}
};

} // namespace cartesian3d

} // namespace GrAL 

#endif
