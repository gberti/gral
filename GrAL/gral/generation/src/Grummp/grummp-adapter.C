#include "Gral/Generation/Grummp/grummp-adapter.h"

namespace GrAL {
namespace grummp_adapter {
  
  Mesh2DAdapter::SD::SD() {
  the_archetype[0] = archetype_type(3);
  } 
  Mesh2DAdapter::SD Mesh2DAdapter::sd;

} // namespace grummp_adapter
} // namespace GrAL
