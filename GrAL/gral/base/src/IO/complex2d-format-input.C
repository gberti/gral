
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/IO/complex2d-format-input.h"
#include "Container/string-utils.h"

#include <vector>
#include <sstream>
#include <iterator>

namespace GrAL {

void 
IstreamComplex2DFmt::read_coords() const
{
  base::init();
  typedef point_traits<coord_type> pt;
  coords = std::vector<coord_type>(NumOfVertices());

  // read first line, check number of floating point numbers
  std::string coord_line = get_non_empty_line(In());
  std::istringstream is(coord_line);
  std::vector<double> coords_1;
  coords_1.insert(coords_1.begin(),
		  std::istream_iterator<double>(is),
		  std::istream_iterator<double>());
  if(coords_1.size() != spacedim) {
    spacedim = coords_1.size();
  }
  unsigned v = 0;
  pt::ConstructWithDim(spacedim,coords[v]);
  for(int i = pt::LowerIndex(coords[v]); i <= pt::UpperIndex(coords[v]); ++i) {
    coords[v][i] = coords_1[i-pt::LowerIndex(coords[v])];
  }
  
  for(v = 1; v < NumOfVertices(); ++v) {

    pt::ConstructWithDim(spacedim,coords[v]);
    for(int i = pt::LowerIndex(coords[v]); i <= pt::UpperIndex(coords[v]); ++i) {
      In() >> coords[v][i];
    }
  }
  coords_read = true;
}

bool
IstreamComplex2DFmt::initialized() const
{ return (base::initialized() && coords_read); }

void 
IstreamComplex2DFmt::init() const
{
  base::init();
  if(! coords_read) {
    read_coords();
  }
}

} // namespace GrAL 
