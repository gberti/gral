#ifndef NMWR_GB_PARTITIONING_METIS_WRAP_H
#define NMWR_GB_PARTITIONING_METIS_WRAP_H

// $LICENSE

/*! \brief Wrapper for Metis graph partitioning routines
    \ingroup metispartitioning_internal

    \author Holger Kache, Guntram Berti
*/

extern
void metis_partGraphRecursive(int n, 
                              int* xadj,    int* adjncy,
                              int* vwgt,    int* adjwgt, 
			      int  wgtflag, int numflag,
                              int  nparts,
			      int* opt,     int edgecut, int* part);

extern
void metis_partGraphKway(int n, 
                         int* xadj,   int* adjncy, 
                         int* vwgt,   int* adjwgt, 
                         int wgtflag, int numflag, 
                         int nparts, 
			 int* opt, int edgecut, int* part);

#endif
