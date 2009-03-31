#ifndef NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_H
#define NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_H

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/complex2d-format-output.h"


/*! \defgroup complex2dformat Complex2D format I/O Adapter 

  \brief A grid interface for a file in complex2d-format.
  \ingroup gridio
  \see  Module gridio

  \par Example
  \code
  ifstream gridfile("my.complexd");
  IstreamComplex2DFmt Gfile(gridfile,1);  // 1 == offset, default: 0
  ConstructGrid(G,Geom,Gfile,Gfile);
  \endcode 

 \warning
 The constructor \e modifies the stream, therefore \e never
 do the following:
 \code
 ConstructGrid(G, IstreamComplex2DFmt(in),
                  IstreamComplex2DFmt(in));
 
 \endcode

 <b> The complex2d format: </b>
 \code
 nv // number of vertices
 nc // number of cells

 x_1  y_1  // coords of vertex 1 
 ....
 x_nv y_nv // coords of vertex nv

 nvc1 //  number of vertices of cell 1
    vc1_1  ....  vc1_nvc1   // indices of vertices of cell 1 ( starting with 1)
 ....
 nvc_nc //  number of vertices of cell nc
    vc1_1  ....  vc1_nvc_nc // indices of vertices of cell nc ( starting with 1)
 \endcode
*/
//----------------------------------------------------------------


#endif
