
/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Adapters/VTK/all.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/IO/write-cgns.h"
#include "Gral/IO/write-stl.h"

#include "IO/control-device.h"

#include <iostream>
#include <fstream>

/*! \file

   \todo There seems to be a bug in vtk_ugrid which means only one field of each domain type 
   (cell, vertex) is actually created.
   
*/

static const char* version = "$Id$";


struct field_input {
  std::string file;
  std::string name;
  int         dim_loc; // 0 = vertices, 2 = cells
  int         dim; // 1 = scalar field

  field_input() : dim(1) {}
};


inline std::istream& operator>>(std::istream& in, field_input& f)
{ return (in >> f.file >> f.name >> f.dim_loc >> f.dim); }

inline std::ostream& operator<<(std::ostream& out, field_input const& f)
{ return (out << f.file << f.name << f.dim_loc << f.dim); }



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

  vector<field_input> fields;
  Ctrl.add("-field", GrAL::GetPushbackMutator(fields));
  h += id + "-field\t <string filename> <string name> <int dim_of_loc> <int dim> \n";
  h += id + "   (Field in file filename, with name, dim_of_loc =0|2 (vertex|cell), dim = 1 for scalar fields)\n";

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

  // conditionally read field
  for(int i = 0; i < fields.size(); ++i) {
    std::vector<double> field_data;
    if(fields[i].file != "") {
      cerr << "Reading field  from file \"" << fields[i].file << "\" ... " << endl;
      ifstream file(fields[i].file.c_str());
      if(! file.is_open()) {
	cerr << "Could not open field file \"" << fields[i].file << "\", quitting." << endl;
	return 2;
      }
      double x;
      while(file >> x)
	field_data.push_back(x);
      file.close();
    }
    if(fields[i].file != "") {
      int req_size = (fields[i].dim_loc == 0 ? vtkgrid.NumOfVertices() : vtkgrid.NumOfCells() );
      if(field_data.size() != fields[i].dim * req_size) {
	cerr << "Field has wrong size " << field_data.size() << ", should be " << fields[i].dim * req_size << endl;
	return 2;
      }
      if(fields[i].name == "")
	fields[i].name = fields[i].file;
      vtkDoubleArray *field = vtkDoubleArray::New();
      field->Allocate(req_size);
      field->SetName(fields[i].name.c_str());
      if(fields[i].dim == 1) {
	for(int k = 0; k < req_size; ++k) 
	  field->InsertValue(k, field_data[k]);
	if(fields[i].dim_loc == 0) // vertex field
	  vtkgrid.TheAdaptee()->GetPointData()->SetScalars(field);
	else // Cells
	  vtkgrid.TheAdaptee()->GetCellData() ->SetScalars(field);
      }
      else {
	field->SetNumberOfComponents(fields[i].dim);
	field->SetNumberOfTuples(req_size);
	double* val = new double[fields[i].dim];
	for(int k = 0; k < req_size; ++k) {
	  for(int j = 0; j < fields[i].dim; ++j)
	    val[j] = field_data[fields[i].dim*k +j];
	  field->SetTuple(k,val);
	}
	delete val;
	if(fields[i].dim_loc == 0) // vertex field
	  vtkgrid.TheAdaptee()->GetPointData()->SetVectors(field);
	else // Cells
	  vtkgrid.TheAdaptee()->GetCellData() ->SetVectors(field);
      }
      field->Delete();
      cout << "done." << endl;
    }
  }



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
