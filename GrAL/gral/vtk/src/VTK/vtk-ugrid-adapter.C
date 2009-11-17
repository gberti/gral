
/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Adapters/VTK/vtk-ugrid-adapter.h"
#include "Utility/pre-post-conditions.h"
#include "Gral/IO/stream-grid-adapter.h"

#include "vtkUnstructuredGridWriter.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"
#include "vtkCellTypes.h"
#include "vtkAppendFilter.h"

namespace GrAL { 

namespace vtk_ugrid {

  UGridVTKAdapter_base::UGridVTKAdapter_base()
    : adaptee_(0), owned_(true) 
  {
    clear();
  }

   UGridVTKAdapter_base::UGridVTKAdapter_base(Adaptee *a) 
    : adaptee_(a), owned_(false) 
  {
    REQUIRE(a != NULL, "Initalization with NULL pointer", 1);
    if (adaptee_) {
      adaptee_->Register(NULL); // VTK objects are reference counted
    }
  }

  UGridVTKAdapter_base::~UGridVTKAdapter_base() { 
    release();
  }

  void UGridVTKAdapter_base::SetAdaptee(Adaptee *a) {
    REQUIRE(a != NULL, "Initalization with NULL pointer", 1);
    //make_empty();
    release();
    adaptee_ = a;
    owned_   = false;
    adaptee_->Register(NULL);
  }

  bool UGridVTKAdapter_base::IsOwner() const 
  { 
    if (adaptee_) {
      return (adaptee_->GetReferenceCount() <= 1);
    }
    return false;
  }

  void UGridVTKAdapter_base::make_empty()  
  {
    if (adaptee_) {
      if(owned_) {
	adaptee_->Delete();
	adaptee_ = 0;
      }
      else
	allocate(0,0);
    }
  }

  void UGridVTKAdapter_base::release() 
  {
    if(adaptee_) {
      adaptee_->Delete();
      adaptee_ = 0;
    }
  }
  
  void UGridVTKAdapter_base::clear() 
  {
    make_empty();
    if(adaptee_ == 0) {
      adaptee_ = vtkUnstructuredGrid::New();
      owned_   = true;
    }
  }

  void UGridVTKAdapter_base::allocate(int npoints, int ncells) 
  {
    adaptee_->Allocate(ncells);
    vtkPoints *points = vtkPoints::New();
    points->Allocate(npoints);
    adaptee_->SetPoints(points);
    points->Delete();
  }

  
  bool UGridVTKAdapter_base::read(std::string const& filename)
  {
    // FIXME: Warn if dimensions do not match.
    clear();
    // try VTK  unstructured grid format
    bool done = false;
    if(! done) {
      vtkUnstructuredGridReader *reader = vtkUnstructuredGridReader::New();
      reader->SetFileName(filename.c_str());
      if(reader->IsFileUnstructuredGrid()) {
	reader->Update();
	SetAdaptee(reader->GetOutput());
	done = true;
      }
      reader->Delete();
    }
    if(! done) {
      vtkPolyDataReader *reader = vtkPolyDataReader::New();
      reader->SetFileName(filename.c_str());
      if(reader->IsFilePolyData()) {
	reader->Update();
	CopyFrom(reader->GetOutput());
	done = true;
      }
      reader->Delete();
    }
    return  done;
  }


  bool UGridVTKAdapter_base::write(std::string const& filename) const
  {
    vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
    writer->SetFileName(filename.c_str());
    writer->SetInput(GetAdaptee());
    writer->Write();
    writer->Delete();
    return true;
  }

  bool UGridVTKAdapter_base::write_polydata(std::string const& filename) const
  {
    vtkPolyData * poly = vtkPolyData::New();
    CopyTo(poly);
    vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
    writer->SetFileName(filename.c_str());
    writer->SetInput(poly);
    writer->Write();
    writer->Delete();
    poly->Delete();
    return true;
  }


  void UGridVTKAdapter_base::CopyTo(vtkPolyData * poly) const 
  {
    poly->SetPoints(GetAdaptee()->GetPoints());
    poly->Allocate(GetAdaptee()->GetNumberOfCells());
    for(int c = 0; c < NumOfCells(); ++c) {
      vtkIdList * pts = vtkIdList::New();
      GetAdaptee()->GetCellPoints(vtkIdType(c), pts);
      poly->InsertNextCell(GetAdaptee()->GetCellType(c), pts);
      pts->Delete();
    }
  }

  void UGridVTKAdapter_base::CopyFrom(vtkPolyData * poly) 
  {
    clear();

    vtkAppendFilter * filter = vtkAppendFilter::New();
    filter->SetInput(poly);
    filter->Update();
    adaptee_->ShallowCopy(filter->GetOutput());
  }



  std::string  UGridVTKArchetypes_Base::VTK_type(int t) 
  {
    switch(t) {
    case VTK_VERTEX:          return "VTK_VERTEX";        break;
    case VTK_POLY_VERTEX:     return "VTK_POLY_VERTEX";   break;
    case VTK_LINE:            return "VTK_LINE";          break;
    case VTK_POLY_LINE:       return "VTK_POLY_LINE";     break;
    case VTK_TRIANGLE:        return "VTK_TRIANGLE";      break;
    case VTK_TRIANGLE_STRIP:  return "VTK_TRIANGLE_STRIP";break;
    case VTK_QUAD:            return "VTK_QUAD";          break;
    case VTK_PIXEL:           return "VTK_PIXEL";         break;
    case VTK_VOXEL:           return "VTK_VOXEL";         break;
    case VTK_TETRA:           return "VTK_TETRA";         break;
    case VTK_HEXAHEDRON:      return "VTK_HEXAHEDRON";    break;
    case VTK_WEDGE:           return "VTK_WEDGE";         break;
    case VTK_PYRAMID:         return "VTK_PYRAMID";       break;
    default:
      return "unknown";
    }
  }

template <>
UGridVTKArchetypes<2>::UGridVTKArchetypes()
{
  int i;
  typedef grid_types<archetype_type>::size_type arch_size_type;
  // construct TRIANGLE1 element archetype
  arch_size_type conn_triangle1[3][2] = {{0,1}, {1,2}, {2,0}};
  archetype_type triangle1(conn_triangle1, 3);
  
  archetypes.push_back(triangle1);
  i = archetypes.size() - 1; 
  names[i]     = "VTK_TRIANGLE";
  vtk_types[i] = VTK_TRIANGLE;

  // construct QUAD1 element archetype 
  // this is the VTK_PIXEL connectivity!!
  // int conn_quad1[4][2] = { {0,1}, {1,3}, {3,2}, {2,0}};
  arch_size_type conn_quad1[4][2] = { {0,1}, {1,2}, {2,3}, {3,0}};
  archetype_type quad1(conn_quad1, 4);
  archetypes.push_back(quad1);
  i = archetypes.size() - 1 ;
  names[i]     = "VTK_QUAD";
  vtk_types[i] = VTK_QUAD;
}


template <>
UGridVTKArchetypes<3>::UGridVTKArchetypes()
{
  {
    // construct VTK_TETRA element archetype
    int conn_tetra[4*4] = {
      3,  0, 1, 2,
      3,  0, 3, 1,
      3,  0, 2, 3,
      3,  1, 3, 2
    };
    stream_grid_mask<int *>   g(4,4,conn_tetra);
    archetype_type            arch;
    ConstructGrid0(arch, g);
    archetypes.push_back(arch);
    int i = archetypes.size() - 1; 
    names[i]     = "VTK_TETRA";
    vtk_types[i] = VTK_TETRA;
  }

  {
    // construct VTK_HEXAHEDRON
    // 0: (0,0,0), 1: (1,0,0), 2: (1,1,0), 3: (0,1,0)
    // 4: (0,0,1), 5: (1,0,1), 6: (1,1,1), 7: (0,1,1)
    int conn[5*6] = {
      4,  0, 3, 2, 1,
      4,  0, 1, 5, 4,
      4,  0, 4, 7, 3,
      4,  1, 2, 6, 5,
      4,  2, 3, 7, 6,
      4,  4, 5, 6, 7
    };
    GrAL::stream_grid_mask<int *> g(8,6,conn);
    archetypes.push_back(archetype_type());
    ConstructGrid0(archetypes.back(), g);
    int i = archetypes.size() - 1; 
    names[i]     = "VTK_HEXAHEDRON";
    vtk_types[i] = VTK_HEXAHEDRON;
  }

  {
    // construct VTK_WEDGE
    // 0: (0,0,0), 1: (1,0,0), 2: (0,1,0),
    // 3: (0,0,1), 4: (1,0,1), 5: (0,1,1),
    int conn[5*3 + 4*2] = {
      3,   0, 2, 1,
      3,   3, 4, 5,
      4,   0, 1, 4, 3,
      4,   1, 2, 5, 4,
      4,   0, 3, 5, 2
    };
    GrAL::stream_grid_mask<int *> g(6,5,conn);
    archetypes.push_back(archetype_type());
    ConstructGrid0(archetypes.back(), g);
    int i = archetypes.size() - 1; 
    names[i]     = "VTK_WEDGE";
    vtk_types[i] = VTK_WEDGE;
  }

  {
    // construct VTK_PYRAMID
    // 0: (0,0,0), 1: (1,0,0), 2: (1,1,0), 3: (0,1,0),
    // 4: (0.5, 0.5, 1)
    int conn[5*1 + 4*4] = {
      4, 0, 3, 2, 1,
      3, 4, 0, 1,
      3, 4, 1, 2,
      3, 4, 2, 3,
      3, 4, 3, 0
    };
    GrAL::stream_grid_mask<int *> g(5,5,conn);
    archetypes.push_back(archetype_type());
    ConstructGrid0(archetypes.back(), g);
    int i = archetypes.size() - 1; 
    names[i]     = "VTK_PYRAMID";
    vtk_types[i] = VTK_PYRAMID;
  }
}



template <>
UGridVTKArchetypes<2>::archetype_handle 
UGridVTKArchetypes<2>::archetype_of(UGridVTKArchetypes<2>::Cell const& c) const {
  switch (c.NumOfVertices()) {
    case 3: 
      return 0; 
    case 4:
      return 1; 
    default:
      GRAL_ERROR << "No archetype for VTK cell with " 
                 << c.NumOfVertices() << " vertices";
      GRAL_ABORT;
   }
}


template <>
UGridVTKArchetypes<3>::archetype_handle 
UGridVTKArchetypes<3>::archetype_of(UGridVTKArchetypes<3>::Cell const& c) const {
  // FIXME: switching by number of cells is not sufficient in general
  // (though it suffices for VTK if we don't use meshes of mixed dimension).
  // we must store the correct archetype for each cell.
  switch (c.NumOfVertices()) {
  case 4: 
    return 0; 
  case 8:
    return 1;
  case 6:
    return 2;
  case 5:
    return 3;
  default:
      GRAL_ERROR << "No 3D archetype for VTK cell with " 
                 << c.NumOfVertices() << " vertices";
      GRAL_ABORT;
   }
}


} // namespace vtk_ugrid

} // namespace GrAL 


