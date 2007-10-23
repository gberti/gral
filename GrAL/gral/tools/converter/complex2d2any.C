

// Input
#include "Gral/IO/complex2d-format-input.h"

// Output
#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/Adapters/VTK/all.h"

// Intermediate representation
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"


#include "vtkUnstructuredGridWriter.h"

#include "Container/tuple-point-traits.h"
#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cerr << "------ Starting complex2d2vtk -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2gmv.in","main");
  string h = "complex2d2gmv - converted mesh in complex2d format to gmv format\n";
  h += "Usage: complex2d2gmv -g|-i|-in <complex2d> -o|-out <gmv>  -d <space dim = 2> -off <offset = 0>\n";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  RegisterAt(Ctrl, "-i", grid_in);
  RegisterAt(Ctrl, "-in", grid_in);

  string gmv_grid_out;
  RegisterAt(Ctrl, "-gmv", gmv_grid_out);
  string vtk_grid_out;
  RegisterAt(Ctrl, "-vtk", vtk_grid_out);

  AddHelp(Ctrl, h);


  Ctrl.update();

  typedef tuple<double,3>                  coord_type;

  IstreamComplex2DFmt In(grid_in);
  cerr << "Reading grid from file " << grid_in << " ... " << flush;
  Complex2D G;
  simple_geometry<Complex2D,coord_type> GeomG(G);
  ConstructGrid(G, GeomG, In, In);

  if(gmv_grid_out != "") {
    OstreamGMV2DFmt     Out(gmv_grid_out);
    ConstructGrid(Out, G, GeomG);
    cerr << "Written to gmv file " << gmv_grid_out << endl;
  }

  if(vtk_grid_out != "") {
    vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
    ConstructGrid(vtkgrid, vtkgrid, G, GeomG);
    //vtkgrid.write(vtk_grid_out);
    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName(vtk_grid_out.c_str());
    writer->SetInput(vtkgrid.GetAdaptee());
    writer->Write();
    writer->Delete();
  }

}
