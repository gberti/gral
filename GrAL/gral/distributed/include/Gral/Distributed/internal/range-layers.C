#ifndef NMWR_GB_RANGE_LAYERS_C
#define NMWR_GB_RANGE_LAYERS_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Distributed/range-layers.h"

template<class EltRange>
void write_rge(EltRange const& rge, ostream & out)
{
  typedef typename EltRange::const_iterator cit;
  for(cit e = rge.begin();  e != rge.end(); ++e)
    out << *e << ' ';
}

template<class OVRange>
void write_ovrge(OVRange const& rnge, ostream& out)
{
  out << "private:\n";
      write_rge(rnge.privee(),out);
      out << '\n' << "exposed:\n";
      write_rge(rnge.exposed(), out);
      out << '\n'<< "shared:\n";
      write_rge(rnge.shared(),  out);
      out << '\n' << "copied\n";
      write_rge(rnge.copied(), out);
}

#endif
