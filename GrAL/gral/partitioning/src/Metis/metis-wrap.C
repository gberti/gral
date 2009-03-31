

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
 

/*! \file
  
    \author Holger Kache, Guntram Berti
 */

extern "C" {
#include <metis.h>
}

namespace GrAL {

void metis_partGraphRecursive(int n, 
                              int* xadj,    int* adjncy,
                              int* vwgt,    int* adjwgt, 
			      int  wgtflag, int numflag,
                              int  nparts,
			      int* opt,     int& edgecut, int* part)  {

METIS_PartGraphRecursive(&n,
                         xadj,adjncy,
                         vwgt, adjwgt,
                         &wgtflag,
                         &numflag,
                         &nparts,
			 opt,&edgecut,part);
 };

void metis_partGraphKway(int n, 
                         int* xadj,   int* adjncy, 
                         int* vwgt,   int* adjwgt, 
                         int wgtflag, int numflag, 
                         int nparts, 
			 int* opt, int& edgecut, int* part) {

METIS_PartGraphKway(&n,
                    xadj,adjncy,
                    vwgt,adjwgt,
                    &wgtflag,
                    &numflag,
                    &nparts,
		    opt, &edgecut, part);

};


} // namespace GrAL 



