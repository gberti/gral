
// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"

typedef CartesianGrid3D::index_type it;

it CartesianGrid3D::corner_offset[8] = 
 { it(0,0,0), it(0,0,1), it(0,1,0), it(0,1,1),
   it(1,0,0), it(1,0,1), it(1,1,0), it(1,1,1) };
