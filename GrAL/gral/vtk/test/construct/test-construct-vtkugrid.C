/*! \file 
    Test and demonstrate output to VTK format (2D and 3D)
 */

/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, Roman Putanowicz

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
 
    typedef grid_types<vtk_ugrid::UGridVTKAdapter<2> > vtkgt;
    {
      vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
      ConstructGrid(vtkgrid, vtkgrid, T, GeomT);
      //vtkgrid.TheAdaptee()->GetCellTypesArray()->PrintSelf(std::cerr,0);
      vtkCellTypes * ct = vtkCellTypes::New();
      vtkgrid.TheAdaptee()->GetCellTypes(ct);
      for(int t = 0; t <  ct->GetNumberOfTypes(); ++t) {
	cerr << "Cell type " << t << " = " << vtkgt::VTK_type(ct->GetCellType(t)) << "\n";
      }
      ct->Delete();
      
      
      vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
      writer->SetFileName("tri+quad.vtk");
      writer->SetInput(vtkgrid.TheAdaptee());
      writer->Write();
      writer->Delete();
      
      vtkgrid.write         ("tri+quad-builtin.vtk");
      vtkgrid.write_polydata("tri+quad-poly.vtk");

      // try reading
      {
	vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
	vtkgrid.read ("tri+quad-builtin.vtk");
	vtkgrid.write("tri+quad-builtin2.vtk");
      }
      {
	vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
	vtkgrid.read ("tri+quad-poly.vtk");
	vtkgrid.write("tri+quad-poly2.vtk");
      }
    }
    {
      vtkUnstructuredGrid * vtk_ptr = vtkUnstructuredGrid::New();
      vtk_ugrid::UGridVTKAdapter<2> vtkgrid(vtk_ptr);
      ConstructGrid(vtkgrid, vtkgrid, T, GeomT);
      vtkCellTypes * ct = vtkCellTypes::New();
      vtk_ptr->GetCellTypes(ct);
      for(int t = 0; t <  ct->GetNumberOfTypes(); ++t) {
	cerr << "Cell type " << t << " = " << vtkgt::VTK_type(ct->GetCellType(t)) << "\n";
      }
      ct->Delete();
      
      
      vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
      writer->SetFileName("tri+quad2.vtk");
      writer->SetInput(vtk_ptr);
      writer->Write();
      writer->Delete();
      vtk_ptr->Delete();
    }
    {
      vtkUnstructuredGrid * vtk_ptr = vtkUnstructuredGrid::New();
      vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
      vtkgrid.SetAdaptee(vtk_ptr);
      ConstructGrid(vtkgrid, vtkgrid, T, GeomT);
      vtkCellTypes * ct = vtkCellTypes::New();
      vtk_ptr->GetCellTypes(ct);
      for(int t = 0; t <  ct->GetNumberOfTypes(); ++t) {
	cerr << "Cell type " << t << " = " << vtkgt::VTK_type(ct->GetCellType(t)) << "\n";
      }
      ct->Delete();
      
      
      vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
      writer->SetFileName("tri+quad3.vtk");
      writer->SetInput(vtk_ptr);
      writer->Write();
      writer->Delete();
      vtk_ptr->Delete();
    }


  }

  {
    // read back grid
    vtkUnstructuredGridReader *reader = vtkUnstructuredGridReader::New();
    reader->SetFileName("tri+quad.vtk");
    reader->Update();
    //vtkUnstructuredGrid * ug = reader->GetOutput();
    vtk_ugrid::UGridVTKAdapter<2> vtkgrid;
    vtkgrid.SetAdaptee(reader->GetOutput());
    reader->Delete();
    Complex2D G;
    //    stored_geometry_complex2D Geom;
    simple_geometry<Complex2D, vtk_ugrid::UGridVTKAdapter<2>::coord_type> Geom;
    ConstructGrid(G, Geom, vtkgrid, vtkgrid);
    OstreamComplex2DFmt Out("tri+quad.complex2d");
    //ConstructGrid(Out, vtkgrid, vtkgrid);
    ConstructGrid(Out, G, Geom);
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
      vtkgrid.TheAdaptee()->GetPointData()->SetScalars(scalars);
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
      vtkgrid.TheAdaptee()->GetCellData()->SetScalars(scalars);
      scalars->Delete();
    }


    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName("tet3d.vtk");
    writer->SetInput(vtkgrid.TheAdaptee());
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
    writer->SetInput(vtkgrid.TheAdaptee());
    writer->Write();
    writer->Delete();
  }
  {
    typedef cartesian3d::CartesianGrid3D   grid_type;
    typedef grid_types<grid_type>          gt;
    grid_type T(2,2,2);
    cartesian3d::lattice_geometry GeomT(T);

    typedef vtk_ugrid::UGridVTKAdapter<3> vtk_grid_type;
    vtk_grid_type                             vtkgrid;
    vertex_morphism<grid_type, vtk_grid_type> cart2vtk_v(T, vtkgrid);
    cell_morphism  <grid_type, vtk_grid_type> cart2vtk_c(T, vtkgrid);
    ConstructGridVC(vtkgrid, vtkgrid, T, GeomT, cart2vtk_v, cart2vtk_c);
    
    vtkFloatArray *vectors = vtkFloatArray::New();
    vectors->SetNumberOfComponents(3);
    vectors->SetNumberOfTuples(vtkgrid.NumOfVertices());
    for(gt::VertexIterator v(T); !v.IsDone(); ++v) {
      gt::index_type idx = v.index();
      float val[3] = { idx[0], idx[1], idx[2] };
      vectors->SetTuple(cart2vtk_v(v.handle()), val);
    }
    
    vtkgrid.TheAdaptee()->GetPointData()->SetVectors(vectors);
    vectors->Delete();

    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName("cart3d+data.vtk");
    writer->SetInput(vtkgrid.TheAdaptee());
    writer->Write();
    writer->Delete();
  }

}
