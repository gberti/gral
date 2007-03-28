/*! \file 
    Test and demonstrate output to VTK format (2D and 3D)
 */

// GrAL
#include "Gral/Adapters/VTK/all.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Triang3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/IO/complex2d-format-input.h"


// VTK
#include "vtkUnstructuredGridWriter.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"


#include <functional>

int main() {
  using namespace GrAL;

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
    
    ConstructGrid(vtkgrid, vtkgrid, T, GeomT);
    
    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName("tri+quad.vtk");
    writer->SetInput(vtkgrid.GetAdaptee());
    writer->Write();
    writer->Delete();
  }

  {
    typedef Triang3D              grid_type;
    typedef grid_types<grid_type> gt;
    const gt::size_type nv = 4;
    const gt::size_type nc = 1;
    gt::size_type cells[4*nc] = { 0, 1, 2, 3};
    double coords[3*nv] = 
      { 0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0 
      };
    Triang3D                     T(cells, nc, nv);
    stored_geometry_triang3d GeomT(T, coords);

    vtk_ugrid::UGridVTKAdapter<3> vtkgrid;
    ConstructGrid(vtkgrid, vtkgrid, T, GeomT);

    typedef grid_types<vtk_ugrid::UGridVTKAdapter<3> > vtkgt;
    {
      vtkFloatArray *scalars = vtkFloatArray::New();
      scalars->Allocate(vtkgrid.NumOfVertices());
      scalars->SetName("vertex-handles");
      for(vtkgt::VertexIterator v(vtkgrid); !v.IsDone(); ++v) {
	float value = v.handle();
	scalars->InsertValue(v.handle(), value);
      }
      vtkgrid.GetAdaptee()->GetPointData()->SetScalars(scalars);
      scalars->Delete();
    }
    {
      vtkFloatArray *scalars = vtkFloatArray::New();
      scalars->Allocate(vtkgrid.NumOfCells());
      scalars->SetName("cell-handles");
      for(vtkgt::CellIterator c(vtkgrid); !c.IsDone(); ++c) {
	float value = c.handle();
	scalars->InsertValue(c.handle(), value);
      }
      vtkgrid.GetAdaptee()->GetCellData()->SetScalars(scalars);
      scalars->Delete();
    }


    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName("tet3d.vtk");
    writer->SetInput(vtkgrid.GetAdaptee());
    writer->Write();
    writer->Delete();
  }

  {
    typedef cartesian3d::CartesianGrid3D   grid_type;
    typedef grid_types<grid_type>          gt;
    grid_type T(2,2,2);
    cartesian3d::lattice_geometry GeomT(T);

    vtk_ugrid::UGridVTKAdapter<3> vtkgrid;
    ConstructGrid(vtkgrid, vtkgrid, T, GeomT);

    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName("cart3d.vtk");
    writer->SetInput(vtkgrid.GetAdaptee());
    writer->Write();
    writer->Delete();
  }

}
