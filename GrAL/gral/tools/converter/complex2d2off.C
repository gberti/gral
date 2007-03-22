#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/geomview-format-output.h"

#include "IO/control-device.h"

#include <string>

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace GrAL;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2off.in","main");

  string helptxt = 
    "complex2doff -g <grid> -o <output> -d <spacedim=2> -off <offset=0>\n";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  string grid_out;
  RegisterAt(Ctrl, "-o", grid_out);
  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  AddHelp(Ctrl, helptxt);


  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);
  OstreamOFF2DFmt     Out(grid_out);

  ConstructGrid(Out, In, In);
}
