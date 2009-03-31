#ifndef NMWR_GB_PARTITIONING_METIS_WRAP_H
#define NMWR_GB_PARTITIONING_METIS_WRAP_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

namespace GrAL {

/*! \brief Wrapper for Metis graph partitioning routines
    \ingroup metispartitioninginternal

    \author Holger Kache, Guntram Berti
*/


extern
void metis_partGraphRecursive(int n, 
                              int* xadj,    int* adjncy,
                              int* vwgt,    int* adjwgt, 
			      int  wgtflag, int numflag,
                              int  nparts,
			      int* opt,     int& edgecut, int* part);


/*! \brief Wrapper for Metis graph partitioning routines
    \ingroup metispartitioninginternal

    \author Holger Kache, Guntram Berti
*/

extern
void metis_partGraphKway(int n, 
                         int* xadj,   int* adjncy, 
                         int* vwgt,   int* adjwgt, 
                         int wgtflag, int numflag, 
                         int nparts, 
			 int* opt, int& edgecut, int* part);


} // namespace GrAL 

#endif
