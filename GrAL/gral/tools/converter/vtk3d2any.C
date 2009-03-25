
/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Adapters/VTK/all.h"
#include "vtkUnstructuredGridReader.h"

#include "Gral/IO/gmv-format-output3d.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cerr << "------ Starting vtk2gmv -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");
  string h = "vtk3d2any - convert VTK 3D mesh to diverse formats.\n";
  h += "Usage: vtk3d2any -g|-i|-in <vtk>  [-gmv <file>] \n";

  string grid_in;
  RegisterAt(Ctrl, "-g",  grid_in);
  RegisterAt(Ctrl, "-i",  grid_in);
  RegisterAt(Ctrl, "-in", grid_in);

  bool is_poly = false;
  Ctrl.add("-p", GetTrueOnReadMutator(is_poly));

  string gmv_grid_out;
  RegisterAt(Ctrl, "-gmv", gmv_grid_out);

  AddHelp(Ctrl, h);


  Ctrl.update();

  cerr << "Reading grid from file " << grid_in << " ... " << flush;
  vtk_ugrid::UGridVTKAdapter<3> vtkgrid;
  vtkUnstructuredGridReader *reader = vtkUnstructuredGridReader::New();
  reader->SetFileName(grid_in.c_str());
  reader->Update();
  vtkgrid.SetAdaptee(reader->GetOutput());
  reader->Delete();

  if(gmv_grid_out != "") {
    OstreamGMV3DFmt     Out(gmv_grid_out);
    ConstructGrid(Out, vtkgrid, vtkgrid);
    cerr << "Written to file " << gmv_grid_out << endl;
  }

}
