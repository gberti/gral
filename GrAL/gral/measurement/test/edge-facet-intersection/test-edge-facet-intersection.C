
// $LICENSE_NEC

/* \file
   
    Test the detection of edge-facet-intersections,
   indicating invalid elements (e.g. for hexahedral cells).

 */

#include "Gral/Measurement/mappings.h"
#include "Gral/Measurement/check-edge-facet-intersections.h"

#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian3D/integer-lattice-geometry.h"
#include "Gral/Grids/Cartesian3D/mapped-geometry.h"

#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Complex2D/all.h"

#include "Gral/Base/predicates.h" // incident()
#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Geometries/intersections.h"


#include "Geometry/point.h"
#include "IO/control-device.h"
#include "Container/tuple.h"


// make point being treated as 3D point.
template<>
struct dimension_dependent_primitives<point, variable_dimension_tag>
  : public dimension_dependent_primitives_3d<point> 
{};




int main(int argc, char* argv[]) {
  using std::cout;
  using cartesian3d::CartesianGrid3D;
  using cartesian3d::mapped_geometry;
  namespace ms = measurement;

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"map.in","main");
  double wf = 0.0;// warping factor.
  RegisterAt(Ctrl, "-wf", wf);
  unsigned nx = 2, ny =2, nz = 2;
  RegisterAt(Ctrl, "-nx", nx);
  RegisterAt(Ctrl, "-ny", ny);
  RegisterAt(Ctrl, "-nz", nz);

  Ctrl.update();

  typedef CartesianGrid3D                           reg_grid_type;
  typedef point                                     coord_type;
  typedef ms::warping3d<coord_type>                 mapping_type;
  typedef mapped_geometry<mapping_type> reg_geom_type;
  typedef simple_geometry<Complex3D,coord_type>     geom_type;
  typedef grid_types<CartesianGrid3D>               reggt;
  typedef Complex3D                                 grid_type;
  typedef grid_types<grid_type>                     gt;

  //  typedef grid_types<gt::archetype_type> agt; // model of 3D cell surface
  // typedef flag2d<gt::archetype_type>     flag_type; // flag on cell surface
    
  CartesianGrid3D R(nx,ny,nz);
  reg_geom_type GeomR(R, mapping_type(wf));
    
  Complex3D G;
  vertex_morphism<CartesianGrid3D, Complex3D> v_corr(R,G);
  cell_morphism  <CartesianGrid3D, Complex3D> c_corr(R,G);
  ConstructGrid0(G,R, v_corr, c_corr);

  geom_type GeomG(G);
  // CopyGeom(GeomG, GeomR, v_corr, c_corr);
  for(reggt::VertexIterator v(R); ! v.IsDone(); ++v)
    GeomG.coord(v_corr(*v)) = GeomR.coord(*v);

  ms::check_edge_facet_intersections<grid_type,geom_type> check_efi(G,GeomG);
  check_efi.check_all_cells();

  for(gt::FacetIterator f(G); ! f.IsDone(); ++f)
    if(check_efi.IntersectedFacets()(*f) > 0)
      cout << "Facet " << f.handle() 
	   << ": " << check_efi.IntersectedFacets()(*f) << " intersections\n"; 
}





