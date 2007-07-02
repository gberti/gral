#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/gmv-format-output2d.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cerr << "------ Starting complex2d2gmv -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2gmv.in","main");
  string h = "complex2d2gmv - converted mesh in complex2d format to gmv format\n";
  h += "Usage: complex2d2gmv -g|-i|-in <complex2d> -o|-out <gmv>  -d <space dim = 2> -off <offset = 0>\n";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  RegisterAt(Ctrl, "-i", grid_in);
  RegisterAt(Ctrl, "-in", grid_in);

  string grid_out;
  RegisterAt(Ctrl, "-o",   grid_out);
  RegisterAt(Ctrl, "-out", grid_out);

  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  AddHelp(Ctrl, h);


  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);

  OstreamGMV2DFmt     Out(grid_out);

  cerr << "Reading grid from file " << grid_in << " ... " << flush;
  ConstructGrid(Out, In, In);
  cerr << "Written to gmv file " << grid_out << endl;
}
