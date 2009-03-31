

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex3D/complex3d.h"

namespace GrAL {

Complex3D::Complex3D()
  : num_of_vertices(0)
{ offset.push_back(0); }

Complex3D::~Complex3D() { clear();}

void Complex3D::clear()
{
  cells         .clear();
  offset        .clear();
  offset.push_back(0);
  cell_archetype.clear();
  archetypes    .clear();
  num_of_vertices = 0;

  ENSURE(NumOfVertices() == 0, " (is " << NumOfVertices() << ")\n",1);
  ENSURE(NumOfCells   () == 0, " (is " << NumOfCells   () << ")\n",1);
}


void Complex3D::calculate_dependent() {}

} // namespace GrAL 
