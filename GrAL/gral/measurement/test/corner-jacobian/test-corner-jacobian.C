


// GRAL
#include "Gral/Measurement/corner-jac-condition-number.h"
#include "Gral/Measurement/mappings.h"


#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian3D/mapped-geometry.h"

#include "Gral/Grids/Complex2D/all.h"


#include "Geometry/point.h"

#include "IO/control-device.h"


// use 3D functionality for class 'point' 
template<>
struct dimension_dependent_primitives<point, variable_dimension_tag>
  : public dimension_dependent_primitives_3d<point> 
{};

// move to Container/tuple.h
/*
template<class T, unsigned N>
struct point_traits<tuple<T,N> >
  : public point_traits_base<tuple<T,N> >
{
  typedef tuple<T,N>                    Ptype;
  typedef typename Ptype::value_type    component_type;  
  typedef typename dim_tag<N>::dimension_tag dimension_tag; 

  static  int LowerIndex(Ptype const&) { return 0;}
  static  int UpperIndex(Ptype const&) { return N-1;}
  static  int Dim(Ptype const&) { return N;}

  static  Ptype Origin(unsigned)   { return Ptype(0);}
  static  Ptype Origin()           { return Ptype(0);}
  static  void  ConstructWithDim(unsigned, Ptype&) { }
};
*/



int main(int argc, char* argv[]) {
  using std::cout;
  using cartesian3d::CartesianGrid3D;
  using cartesian3d::mapped_geometry_cartesian3d;

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"map.in","main");
  double wf = 0.0;// warping factor.
  // wf = 0.5 corresponds to warping top of cube by 90 degrees
  // and results in invalid cell!
  RegisterAt(Ctrl, "-wf", wf);
  unsigned nx = 2, ny =2, nz = 2;
  RegisterAt(Ctrl, "-nx", nx);
  RegisterAt(Ctrl, "-ny", ny);
  RegisterAt(Ctrl, "-nz", nz);

  Ctrl.update();

  typedef CartesianGrid3D                           grid_type;
  typedef warping3d<point>                          mapping_type;
  typedef mapped_geometry_cartesian3d<mapping_type> geom_type;
  typedef grid_types<CartesianGrid3D>               gt;
    
  CartesianGrid3D G(nx,ny,nz);
  geom_type Geom(G, mapping_type(wf));


  // now loop over cells of R, check quality of (R,GeomR)
  corner_jacobian<grid_type,geom_type> jac(G,Geom);
 
  cout << "Measuring corner jacobian condition numbers\n";
  for(gt::CellIterator c(G); ! c.IsDone(); ++c) {
    cout << "Cell " << c.handle() << '\n';
    for(gt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc) {
      cout << "  vertex " << vc.local_handle()
	   << "\t : quality = " << jac.condition(vc) << "\t"
	   << "( @ " << Geom.coord(*vc) << ")\n";
	   
    }
  }
}

