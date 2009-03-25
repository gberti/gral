/*! \file 
    Test and demonstrate output to VTK format (2D and 3D)
 */

/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, Roman Putanowicz, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


// GrAL
#include "Gral/Adapters/VTK/all.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Triang3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/complex2d-format-output.h"

#include "Gral/Base/grid-morphism.h"
#include "Gral/Geometries/simple-geometry.h"

// VTK
#include "vtkUnstructuredGridWriter.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkFloatArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCellTypes.h"

#include <functional>

int main() {
  using namespace std;
  using namespace GrAL;
  vtkUnstructuredGrid * mesh;
  {
    std::string grid_data =
      "6 3\n"
      "1.0 0.0\n"
      "1.0 1.0\n"
      "0.0 1.0\n"
      "0.0 0.0\n"
      "2.0 0.0\n"
      "2.0 2.0\n"
      "\n"
      "3  0 1 2\n"
      "3  0 2 3\n"
      "4  0 1 5 4\n";
    std::istringstream gridstream(grid_data); 
    IstreamComplex2DFmt Gsrc(gridstream); 
    Complex2D T;
    stored_geometry_complex2D GeomT(T); 
    ConstructGrid(T, GeomT, Gsrc, Gsrc);
    vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
    typedef grid_types<vtk_ugrid::UGridVTKAdapter<2> > vtkgt;

    ConstructGrid(vtkgrid, vtkgrid, T, GeomT);
    vtkgrid.GetAdaptee()->DebugOn();

    cerr << "\n>>>>>> Printing mesh:\n"; 
    vtkgrid.GetAdaptee()->PrintSelf(std::cerr,0);
    mesh = vtkgrid.GetAdaptee();
  
  }

  {
    vtkUnstructuredGrid * input_mesh = vtkUnstructuredGrid::New();
    input_mesh->DebugOn();
    vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
    vtkgrid.SetAdaptee(input_mesh);
    input_mesh->Delete();
    cerr << "\n>>>>>> Printing mesh:\n"; 
    vtkgrid.GetAdaptee()->PrintSelf(std::cerr,0);
  }
  //cerr << "\n>>>>>> Printing mesh:\n"; 
  // mesh->PrintSelf(std::cerr,0);
  
}
