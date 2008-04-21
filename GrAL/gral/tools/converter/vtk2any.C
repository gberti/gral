
#include "Gral/Adapters/VTK/all.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/IO/write-cgns.h"
#include "Gral/IO/write-stl.h"

#include "IO/control-device.h"

#include <iostream>
#include <fstream>

static const char* version = "$Id$";

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;


  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");
  string h = "vtk2any - convert VTK 2D mesh to diverse formats.\n";
  h += version + string("\n");
  h += "Usage: vtk2any <options>\n";
  string id = "    ";

  string grid_in;
  RegisterAt(Ctrl, "-g",  grid_in);
  RegisterAt(Ctrl, "-i",  grid_in);
  RegisterAt(Ctrl, "-in", grid_in);
  h +=  id + "-g|-i|-in\t <VTK surface mesh>\t (Input mesh in VTK unstructured grid or polydata format)\n";


  string gmv_grid_out;
  RegisterAt(Ctrl, "-gmv", gmv_grid_out);
  h += id + "-gmv\t <GMV file name>\t (write GMV surface file)\n";

  string complex2d_grid_out;
  RegisterAt(Ctrl, "-o",         complex2d_grid_out);
  RegisterAt(Ctrl, "-out",       complex2d_grid_out);
  RegisterAt(Ctrl, "-complex2d", complex2d_grid_out);
  h += id + "-o|-out|-complex2d\t <Complex2D file name>\t (write GrAL's Complex2D format)\n";

  string cgns_grid_out;
  RegisterAt(Ctrl, "-cgns", cgns_grid_out);
  h += id + "-cgns\t <CGNS file name>\t (write CGNS surface mesh)\n";


  string vtk_ugrid_out;
  RegisterAt(Ctrl, "-vtk_ug", vtk_ugrid_out);
  h += id + "-vtk_ug\t <VTK file name>\t (write to VTK unstructured grid format file)\n";

  string vtk_poly_out;
  RegisterAt(Ctrl, "-vtk_poly", vtk_poly_out);
  h += id + "-vtk_poly\t <VTK file name>\t (write to VTK polydata format file)\n";

  string stl_ascii_out;
  RegisterAt(Ctrl, "-stl_ascii", stl_ascii_out);
  h += id + "-stl_ascii\t <STL file name>\t (write to STL ASCII format file)\n";

  string stl_geom_name;
  RegisterAt(Ctrl, "-geom_name", stl_geom_name);
  h += id + "-geom_name\t <string>\t (geometry name for STL output)\n";

  AddHelp(Ctrl, h);

  Ctrl.update();

  if(grid_in == "") {
    cerr << h << endl;
    return 2;
  }

  cerr << "------ Starting vtk2any -------\n" << endl;

  cerr << "Reading grid from file " << grid_in << " ... " << flush;
  vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
  vtkgrid.read(grid_in);
  cerr << " done." << std::endl;
  
  if(gmv_grid_out != "") {
    OstreamGMV2DFmt     Out(gmv_grid_out);
    ConstructGrid(Out, vtkgrid, vtkgrid);
    cerr << "Written to file \"" << gmv_grid_out << "\"" << endl;
  }
  if(complex2d_grid_out != "") {
    OstreamComplex2DFmt Out(complex2d_grid_out);
    ConstructGrid(Out, vtkgrid, vtkgrid);
    cerr << "Written to file \"" << complex2d_grid_out << "\"" << endl;    
  }
  if(cgns_grid_out != "") {
    write_triangular_grid_to_cgns(cgns_grid_out, vtkgrid, vtkgrid);
    cerr << "Written to file \"" << cgns_grid_out << "\"" << endl;
  }
  if(vtk_ugrid_out != "") {
    vtkgrid.write(vtk_ugrid_out);
    cerr << "Written to file \"" << vtk_ugrid_out << "\"" << endl;
  }
  if(vtk_poly_out != "") {
    vtkgrid.write_polydata(vtk_poly_out);
    cerr << "Written to file \"" << vtk_poly_out << "\"" << endl;
  }
  if(stl_ascii_out != "") {
    ofstream stl_out(stl_ascii_out.c_str());
    int num_non_triangles = 0;
    write_stl_ascii(stl_out, vtkgrid, vtkgrid, stl_geom_name, num_non_triangles);
    if(num_non_triangles > 0)
      cerr << "WARNING: " << num_non_triangles << " non-triangular cells detected in STL output!" << endl;
    cerr << "Written to file \"" << stl_ascii_out << "\"" << endl;
  }

}
