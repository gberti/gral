#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/geomview-format-output.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2off.in","main");

  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  string grid_out;
  RegisterAt(Ctrl, "-o", grid_out);
  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);
  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);
  OstreamOFF2DFmt     Out(grid_out);

  ConstructGrid(Out, In, In);
}
