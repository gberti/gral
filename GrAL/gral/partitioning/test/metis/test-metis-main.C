

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

// GrAL Partitioning
#include "Gral/Partitioning/metis-partitioning.h"
#include "Gral/Partitioning/partitioning.h"

// GrAL Complex2D
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

// Gral Base
#include "Gral/IO/complex2d-format-input.h"

// Utility
#include "Utility/as-string.h"

// STD
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

int main() {
  using namespace GrAL;

  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  

  std::ifstream gridfile("grid.complex2d"); 
  IstreamComplex2DFmt IG(gridfile,1);
  grid_type G;
  ConstructGrid0(G, IG);
  gridfile.close();

  MetisPartitioning MP;
  int np = 2;
  //  grid_function<gt::Cell, int> parts(G);
  partitioning<grid_type> parts(G);

  MP.calculate_cell_partitioning(G,parts,np);

  std::ofstream partfile( ("grid.complex2d." + as_string(np) + ".out").c_str());
  std::copy(parts.begin(), parts.end(), std::ostream_iterator<int>(partfile, "\n"));
  partfile.close();
  //std::copy(parts.begin(), parts.end(), std::ostream_iterator<int>(std::cout, "\n"));
} 
