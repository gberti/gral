
// $LICENSE_NEC

#include "Gral/Grids/Complex3D/complex3d.h"

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