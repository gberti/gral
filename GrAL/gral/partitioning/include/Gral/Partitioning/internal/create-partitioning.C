#ifndef NMWR_GB_CREATE_PARTITIONING_C
#define NMWR_GB_CREATE_PARTITIONING_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/partitioning.h"
#include "Container/partial-mapping.h"

template<class Partitioning, class CellMapping>
void CreatePartitioning(Partitioning& P,      // out
			const CellMapping& f) // in
{
  typedef typename Partitioning::grid_type grid_type;
  typedef grid_types<grid_type>            gt;
  typedef typename gt::CellIterator        CellIterator;

  typedef typename CellMapping::result_type v_type;
  partial_mapping<v_type,int> part_num;
  for(CellIterator c = P.TheGrid().FirstCell(); ! c.IsDone(); ++c) {
    v_type fc = f(*c);
    if (part_num.undefined(fc)) {
      part_num[fc] = P.add_partition();
    }
    P.add_cell(part_num(fc),*c);
  }

}


#endif
