#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/gmv-format-output2d.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cout << "------ Starting complex2d2gmv -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2gmv.in","main");
  string h = "complex2d2gmv - converted mesh in complex2d format to gmv format\n";
  h += "Usage: complex2d2gmv -g <complex2d> -o <gmv>  -d <space dim = 2> -off <offset = 0>\n";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  string grid_out;
  RegisterAt(Ctrl, "-o", grid_out);
  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  Mutator* helpmsg = new MessageOnReadMutator(cerr,h);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);


  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);

  OstreamGMV2DFmt     Out(grid_out);

  cout << "Reading grid from file " << grid_in << " ... " << flush;
  ConstructGrid(Out, In, In);
  cout << "Written to gmv file " << grid_out << endl;
}
