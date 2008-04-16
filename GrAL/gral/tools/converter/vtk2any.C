
#include "Gral/Adapters/VTK/all.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/IO/write-cgns.h"

#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cerr << "------ Starting vtk2any -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");
  string h = "vtk2any - convert VTK 2D mesh to diverse formats.\n";
  h += "Usage: vtk2any <options>\n";
  string id = "    ";

  string grid_in;
  RegisterAt(Ctrl, "-g",  grid_in);
  RegisterAt(Ctrl, "-i",  grid_in);
  RegisterAt(Ctrl, "-in", grid_in);
  h +=  id + "-g|-i|-in <vtk> <VTK surface mesh> (Input mesh in VTK unstructured grid or polydata (use -p) format)\n";

  bool is_poly = false;
  Ctrl.add("-p", GetTrueOnReadMutator(is_poly));
  h += id + "-p (input is VTK polydata)\n";

  string gmv_grid_out;
  RegisterAt(Ctrl, "-gmv", gmv_grid_out);
  h += id + "-gmv <GMV file name>  (write GMV surface file)\n";

  string complex2d_grid_out;
  RegisterAt(Ctrl, "-o",         complex2d_grid_out);
  RegisterAt(Ctrl, "-out",       complex2d_grid_out);
  RegisterAt(Ctrl, "-complex2d", complex2d_grid_out);
  h += id + "-o|-out|-complex2d <Complex2D file name> (write GrAL's Complex2D format)\n";

  string cgns_grid_out;
  RegisterAt(Ctrl, "-cgns", cgns_grid_out);
  h += id + "-cgns <CGNS file name> (write CGNS surface mesh)\n";

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
  cerr << " done." << std::endl;
  
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
  if(cgns_grid_out != "") {
    write_triangular_grid_to_cgns(cgns_grid_out, vtkgrid, vtkgrid);
    cerr << "Written to file " << cgns_grid_out << endl;
  }

}
