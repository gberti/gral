#include "Gral/IO/complex2d-format.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2complex2d.in","main");

  string h; 
  h += "complex2d2complex2d -g <grid>   [-gd <spacedim=2> -goff <offset=0>]\n";
  h += "                    -o <output> [-od <spacedim=2> -ooff <offset=0>]\n";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);

  string grid_out;
  RegisterAt(Ctrl, "-o", grid_out);
  unsigned sdim_in  = 2;
  RegisterAt(Ctrl, "-gd", sdim_in);
  unsigned sdim_out = 2;
  RegisterAt(Ctrl, "-od", sdim_out);
  unsigned offset_in = 0;
  RegisterAt(Ctrl, "-goff", offset_in);
  unsigned offset_out = 0;
  RegisterAt(Ctrl, "-ooff", offset_out);

  Mutator* helpmsg = new MessageOnReadMutator(cerr,h);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);

  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset_in);
  In.set_spacedim(sdim_in);
  OstreamComplex2DFmt Out(grid_out, offset_out);
  //  Out.set_spacedim(sdim_out);

  ConstructGrid(Out, In, In);
}
