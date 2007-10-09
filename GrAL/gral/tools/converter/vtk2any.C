#include "Gral/Adapters/VTK/all.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/complex2d-format-output.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cerr << "------ Starting vtk2gmv -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");
  string h = "vtk2any - convert VTK 2D mesh to diverse formats.\n";
  h += "Usage: vtk2any -g|-i|-in <vtk> [-p (if POLYDATA)] [-gmv <file>] [-o|-out|-complex2d <file>]\n";

  string grid_in;
  RegisterAt(Ctrl, "-g",  grid_in);
  RegisterAt(Ctrl, "-i",  grid_in);
  RegisterAt(Ctrl, "-in", grid_in);

  bool is_poly = false;
  Ctrl.add("-p", GetTrueOnReadMutator(is_poly));

  string gmv_grid_out;
  RegisterAt(Ctrl, "-gmv", gmv_grid_out);
  string complex2d_grid_out;
  RegisterAt(Ctrl, "-o",         complex2d_grid_out);
  RegisterAt(Ctrl, "-out",       complex2d_grid_out);
  RegisterAt(Ctrl, "-complex2d", complex2d_grid_out);

  //string vtk_ugrid_out;

  AddHelp(Ctrl, h);


  Ctrl.update();

  cerr << "Reading grid from file " << grid_in << " ... " << flush;
  vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
  if(! is_poly) {
    vtkUnstructuredGridReader *reader = vtkUnstructuredGridReader::New();
    reader->SetFileName(grid_in.c_str());
    reader->Update();
    vtkgrid.SetAdaptee(reader->GetOutput());
    reader->Delete();
  }
  else {
    vtkPolyDataReader *reader = vtkPolyDataReader::New();
    reader->SetFileName(grid_in.c_str());
    reader->Update();
    vtkPolyData * surface = reader->GetOutput();
    vtkUnstructuredGrid * ug = vtkUnstructuredGrid::New();
    ug->SetPoints(surface->GetPoints());
    ug->Allocate(surface->GetNumberOfCells());
    //ug->SetCells(surface->GetCells());
    for(int c = 0; c < surface->GetNumberOfCells(); ++c) {
      //      vtkIdType np;
      vtkIdList * pts = vtkIdList::New();
      surface->GetCellPoints(vtkIdType(c),pts); //np,pts);
      ug->InsertNextCell(surface->GetCellType(c),
			 pts);
    }
    vtkgrid.SetAdaptee(ug);
  }
  
  if(gmv_grid_out != "") {
    OstreamGMV2DFmt     Out(gmv_grid_out);
    ConstructGrid(Out, vtkgrid, vtkgrid);
    cerr << "Written to file " << gmv_grid_out << endl;
  }
  if(complex2d_grid_out != "") {
    OstreamComplex2DFmt Out(complex2d_grid_out);
    ConstructGrid(Out, vtkgrid, vtkgrid);
    cerr << "Written to file " << complex2d_grid_out << endl;    
  }

}
