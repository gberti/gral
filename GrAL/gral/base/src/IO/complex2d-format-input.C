#include "Gral/IO/complex2d-format-input.h"

namespace GrAL {

void 
IstreamComplex2DFmt::read_coords() const
{
  base::init();
  typedef point_traits<coord_type> pt;
  coords =  ::std::vector<coord_type>(NumOfVertices());
  for(unsigned v = 0; v < NumOfVertices(); ++v) {
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
