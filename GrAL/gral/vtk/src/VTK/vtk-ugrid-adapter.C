
#include "Gral/Adapters/VTK/vtk-ugrid-adapter.h"
#include "Utility/pre-post-conditions.h"
#include "Gral/IO/stream-grid-adapter.h"

namespace GrAL { 

namespace vtk_ugrid {

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
  // construct TRIANGLE1 element archetype
  int conn_triangle1[3][2] = {{0,1}, {1,2}, {2,0}};
  archetype_type triangle1(conn_triangle1, 3);
  
  archetypes.push_back(triangle1);
  i = archetypes.size() - 1; 
  names[i]     = "VTK_TRIANGLE";
  vtk_types[i] = VTK_TRIANGLE;

  // construct QUAD1 element archetype 
  // this is the VTK_PIXEL connectivity!!
  // int conn_quad1[4][2] = { {0,1}, {1,3}, {3,2}, {2,0}};
  int conn_quad1[4][2] = { {0,1}, {1,2}, {2,3}, {3,0}};
  archetype_type  quad1(conn_quad1, 4);
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


