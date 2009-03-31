
/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/IO/gmv-format-output3d.h"

#include "Gral/IO/stream-grid-adapter.h"
#include "Gral/Grids/Complex2D/construct.h"

#include <fstream>

namespace GrAL { 
  
  OstreamGMV3DFmt::OstreamGMV3DFmt() { data.initialize(); }

  OstreamGMV3DFmt::OstreamGMV3DFmt(std::ostream& ot)
    : base(ot) {  data.initialize(); }

OstreamGMV3DFmt::OstreamGMV3DFmt(std::string const& nm)
  : base(nm) { data.initialize(); }

OstreamGMV3DFmt::~OstreamGMV3DFmt() {}

void OstreamGMV3DFmt::copy(OstreamGMV3DFmt const& rhs)
{ base::copy(rhs);}



OstreamGMV3DFmt::StaticData OstreamGMV3DFmt::data;


OstreamGMV3DFmt::StaticData::StaticData()
{ initialize(); }

  void OstreamGMV3DFmt::StaticData::initialize()
{
  if(archetypes == 0 || names == 0) {
    archetypes = new archetype_table;
    names      = new archetype_name_map;

    // construct tet
    int conn_tet[4*4] = {
      3,  0, 1, 2,
      3,  0, 3, 1,
      3,  0, 2, 3,
      3,  1, 3, 2
    };
    stream_grid_mask<int *> tet(4,4,conn_tet); 
    archetypes->push_back(archetype_type());
    ConstructGrid0(archetypes->back(), tet);
    (*names)[archetypes->size() -1] = "tet";
    
    // construct hex
    
    // 0: (0,0,0), 1: (1,0,0), 2: (1,1,0), 3: (0,1,0)
    // 4: (0,0,1), 5: (1,0,1), 6: (1,1,1), 7: (0,1,1)
    
    int conn_hex[5*6] = {
      4,  0, 3, 2, 1,
      4,  0, 1, 5, 4,
      4,  0, 4, 7, 3,
      4,  1, 2, 6, 5,
      4,  2, 3, 7, 6,
      4,  4, 5, 6, 7
    };
    stream_grid_mask<int *> hex(8,6,conn_hex); 
    archetypes->push_back(archetype_type());
    ConstructGrid0(archetypes->back(), hex);
    (*names)[archetypes->size()-1] = "phex8";

    // construct prism
    int conn_prism[3*5+2*4] = {
      3, 0, 1, 2,
      3, 3, 5, 4,
      4, 0, 3, 4, 1,
      4, 1, 4, 5, 2,
      4, 0, 2, 5, 3
    };
    stream_grid_mask<int *> prism(6,5,conn_prism); 
    archetypes->push_back(archetype_type());
    ConstructGrid0(archetypes->back(), prism);
    (*names)[archetypes->size()-1] = "prism";
    
    
    // construct pyramid
    int conn_pyramid[1*5+4*4] = {
      4,  4, 3, 2, 1,
      3,  0, 1, 2,
      3,  0, 2, 3,
      3,  0, 3, 4,
      3,  0, 4, 1
    };
    stream_grid_mask<int *> pyramid(5,5,conn_pyramid); 
    archetypes->push_back(archetype_type());
    ConstructGrid0(archetypes->back(), pyramid);
    (*names)[archetypes->size() -1] = "pyramid";

    //  cerr << Printer(*names) << '\n';
  }
}

OstreamGMV3DFmt::StaticData::~StaticData()
{
  delete archetypes;
  delete names;
}

} // namespace GrAL 
