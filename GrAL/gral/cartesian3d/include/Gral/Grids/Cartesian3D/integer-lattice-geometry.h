#ifndef GRAL_GB_GRIDS_CARTESIAN3D_LATTICE_GEOMETRY_H
#define GRAL_GB_GRIDS_CARTESIAN3D_LATTICE_GEOMETRY_H

// $LICENSE


namespace cartesian3d {

/*! \brief A geometry for CartesianGrid3D 
    living on the integer lattice \f$ \Z^3 \f$

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
  lattice_geometry(grid_type const& gg)
    : g(&gg)
    {}

  coord_type coord(gt::Vertex const& v) const { return v.index();}
  
  real volume(gt::Cell const&) const { return 1;}
  real volume(gt::Edge const&) const { return 1;}
  real volume(gt::Face const&) const { return 1;}
};

}; // namespace cartesian3d
#endif
