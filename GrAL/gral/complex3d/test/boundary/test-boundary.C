/*! \file
    \author Guntram Berti <berti@ccrl-nece.de>
    $Id$
*/


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Grids/Complex3D/boundary.h"

#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;  

  using GrAL::cartesian3d::CartesianGrid3D;
  typedef grid_types<Complex3D> gt;

  {
    CartesianGrid3D R(2,2,2);
    Complex3D       G;
    ConstructGrid0(G,R);
    BoundaryRange<Complex3D> Bd(G);
    typedef grid_types<BoundaryRange<Complex3D> > bgt;
    cout << "Boundary 2x2x2: " << Bd.NumOfFacets() << " Facets " << endl;
    for(bgt::FacetIterator bf(Bd); ! bf.IsDone(); ++bf) {
      cout << "Facet " << bf.handle() << ": ";
      for(bgt::VertexOnFacetIterator vbf(*bf); ! vbf.IsDone(); ++vbf)
	cout << vbf.handle() << " ";
      cout << endl;
    }    
  }
  {
    CartesianGrid3D R(3,3,3);
    Complex3D       G;
    ConstructGrid0(G,R);
    BoundaryRange<Complex3D> Bd(G);
    cout << "Boundary 3x3x3: " << Bd.NumOfFacets() << " Facets " << endl;
  }

}
