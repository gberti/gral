
// $LICENSE 

/*! \file
  
    \author Holger Kache, Guntram Berti
 */

extern "C" {
#include <metis.h>
}


void metis_partGraphRecursive(int n, 
                              int* xadj,    int* adjncy,
                              int* vwgt,    int* adjwgt, 
			      int  wgtflag, int numflag,
                              int  nparts,
			      int* opt,     int edgecut, int* part)  {

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
			 int* opt, int edgecut, int* part) {

METIS_PartGraphKway(&n,
                    xadj,adjncy,
                    vwgt,adjwgt,
                    &wgtflag,
                    &numflag,
                    &nparts,
		    opt, &edgecut, part);

};






