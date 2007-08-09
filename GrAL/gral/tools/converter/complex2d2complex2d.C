#include <Gral/IO/complex2d-format.h>
#include <Gral/Grids/Complex2D/all.h>

#include <Gral/Geometries/simple-geometry.h>
#include <Gral/Geometries/transformed-geometry.h>

#include <IO/control-device.h>

#include <Container/tuple-point-traits.h>
#include <Geometry/affine-mapping.h>
#include <Geometry/matrix.h>

#include <string>

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  typedef tuple<double,3>                               coord_type;
  typedef point_traits<coord_type>                      pt;
  typedef algebraic_primitives<coord_type>              ap;
  typedef GrAL::matrix<3,3,0>                           matrix_type;
  typedef GrAL::affine_mapping<matrix_type, coord_type> mapping_type;


  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string h; 
  h += "complex2d2complex2d: Transform a mesh in complex2d format.\n";

  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  RegisterAt(Ctrl, "-i", grid_in);
  RegisterAt(Ctrl, "-in", grid_in);
  h += "   -g|-i|-in <grid>  (input grid, complex2d format)\n";
  // Nice to have:
  // RegisterAt(Ctrl, opt("-g", "-i", "-in"), grid_in);

  string grid_out;
  RegisterAt(Ctrl, "-o",   grid_out);
  RegisterAt(Ctrl, "-out", grid_out);
  h += "   -o|-out <grid>  (output grid, complex2d format)\n";

  unsigned sdim_in  = 2;
  RegisterAt(Ctrl, "-gd", sdim_in);
  h += "   -gd <spacedim=2> (spatial dim of input grid)\n";

  unsigned sdim_out = 2;
  RegisterAt(Ctrl, "-od", sdim_out);
  h += "   -od <spacedim=2> (spatial dim of output grid)\n";

  unsigned offset_in = 0;
  RegisterAt(Ctrl, "-goff", offset_in);
  h += "   -goff <offset=0> (vertex numbering offset of input grid)\n";

  unsigned offset_out = 0;
  RegisterAt(Ctrl, "-ooff", offset_out);
  h += "   -ooff <offset=0> (vertex numbering offset of output grid)\n";

  // transformation info
  mapping_type M = mapping_type::identity();
  RegisterAt(Ctrl, "-map", M);
  h += "    -map  <a11 a12 a13 t1 a21 a22 a23 t2 a31 a32 a33 t3> \n";
  h += "          (mapping coefficients, the aij forming the linear part and the ti the translation part)\n";



  Mutator helpmsg = GetMessageOnReadMutator(cerr,h);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);

  Ctrl.update();

  if(grid_in == "") 
    return 2;

  IstreamComplex2DFmt In(grid_in, offset_in);
  In.set_spacedim(sdim_in);
  
  typedef Complex2D                                     grid_type;
  typedef simple_geometry<grid_type, coord_type>        geom_type;
  
  grid_type  G;
  geom_type  GeomG;
  ConstructGrid(G,GeomG, In,In);
  
  transformed_geom<geom_type, mapping_type> TGeomG(GeomG, M);
  
  if(grid_out == "")
    grid_out = "-";
  
  OstreamComplex2DFmt Out(grid_out, offset_out);
  
  ConstructGrid(Out, G, TGeomG);

}
