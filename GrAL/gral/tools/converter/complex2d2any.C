

// Input
#include "Gral/IO/complex2d-format-input.h"

// Output
#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/Adapters/VTK/all.h"
#include "Gral/IO/write-stl.h"
#include "Gral/IO/write-cgns.h"

// Intermediate representation
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Container/tuple-point-traits.h"
#include "IO/control-device.h"

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  cerr << "------ Starting complex2d2any -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"complex2d2gmv.in","main");
  string h = "complex2d2gmv - convert mesh in complex2d format to other formats\n";
  h += "Usage: complex2d2gmv <OPTIONS>\n";
  string p = "   ";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  RegisterAt(Ctrl, "-i", grid_in);
  RegisterAt(Ctrl, "-in", grid_in);
  h += p + "-i|-in|-g <grid> (grid in complex2d format)\n";

  string gmv_grid_out;
  RegisterAt(Ctrl, "-gmv", gmv_grid_out);
  h += p + "-gmv <GMV file> (GMV output file)\n";

  string vtk_ugrid_out;
  RegisterAt(Ctrl, "-vtk_ug", vtk_ugrid_out);
  h += p + "-vtk|-vtk_ug\t <VTK file name>\t (write to VTK unstructured grid format file)\n";

  string vtk_poly_out;
  RegisterAt(Ctrl, "-vtk_poly", vtk_poly_out);
  h += p + "-vtk_poly\t <VTK file name>\t (write to VTK polydata format file)\n";

  string stl_ascii_out;
  RegisterAt(Ctrl, "-stl_ascii", stl_ascii_out);
  h += p + "-stl_ascii\t <STL file name>\t (write to STL ASCII format file)\n";

  string stl_geom_name;
  RegisterAt(Ctrl, "-geom_name", stl_geom_name);
  h += p + "-geom_name\t <string>\t (geometry name for STL output)\n";

  string cgns_grid_out;
  RegisterAt(Ctrl, "-cgns", cgns_grid_out);
  h += p + "-cgns\t <CGNS file name>\t (write CGNS surface mesh)\n";


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

  if(vtk_ugrid_out != "") {
    vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
    ConstructGrid(vtkgrid, vtkgrid, G,GeomG);
    vtkgrid.write(vtk_ugrid_out);
    cerr << "Written to VTK unstructured grid file \"" << vtk_ugrid_out << "\"" << endl;
  }

  if(vtk_poly_out != "") {
    vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
    ConstructGrid(vtkgrid, vtkgrid, G,GeomG);
    vtkgrid.write_polydata(vtk_poly_out);
    cerr << "Written to VTK polydata file \"" << vtk_poly_out << "\"" << endl;
  }

  if(stl_ascii_out != "") {
    ofstream stl_out(stl_ascii_out.c_str());
    int num_non_triangles = 0;
    write_stl_ascii(stl_out, G, GeomG, stl_geom_name, num_non_triangles);
    if(num_non_triangles > 0)
      cerr << "WARNING: " << num_non_triangles << " non-triangular cells detected in STL output!" << endl;
    cerr << "Written to file \"" << stl_ascii_out << "\"" << endl;
  }


}
