#ifndef NMWR_GB_GRID_SMOOTH_PARTITIONING_C
#define NMWR_GB_GRID_SMOOTH_PARTITIONING_C

#include "../smooth-partitioning.h" 
#include "../used-parts.h" 
#include "../balance.h" 
#include "../cut-size.h" 

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//   author: Holger Kache
//----------------------------------------------------------------

template<class GRID>
smooth_partitioning<GRID>::
smooth_partitioning(partitioning<GRID> & PP)
  : numC(PP.TheGrid()),
    P(&PP)
{

  CellIterator c;
 
  //-------- Zellen nummerieren---------------
  int c_cnt = 0;
  for(c = PP.TheGrid().FirstCell(); ! c.IsDone(); ++c, ++c_cnt) {
    numC[*c] = c_cnt;
    NumOfNeighbours(PP,(*c));
  };
};

template<class GRID>
smooth_partitioning<GRID>::
~smooth_partitioning() {};

template<class GRID>
void smooth_partitioning<GRID>::
write_partitioning(partitioning<GRID>& P) {
  
   typedef typename partitioning<GRID>::cell_range_rf cell_range;
   typedef typename cell_range::CellIterator RgeCellIterator;

   CellOnCellIterator cc;
   RgeCellIterator rc;

   int part = 0;
   int cnt = 0;
   int cut_size_hold = cut_size(P) + 1;
   

   while (cut_size(P) < cut_size_hold) {
     
     cut_size_hold = cut_size(P);
     cnt++;

     // Untersuche alle Partitionen,
    
     for (part = 0; part < used_parts(P); part++) {
       // deren Zellen	
       for(rc = P.Range(part).FirstCell(); !rc.IsDone(); ++rc) {
	// am Rand der Partition liegen
	 if (cells[*rc] > 0)
	 
	  // und Nachbarn haben
	  for (cc = (*rc).FirstCell();!cc.IsDone(); ++cc) {	     
	    // die kritisch sind
	    // isolierte Zellen
	      if (cells[*cc] == (*cc).NumOfNeighbours())
		P.set_partition((*cc),part);
	      // einzelne Nasen
	      //**************************//
	      if (cells[*cc] > 1)
		if ((cells[*cc] + 1) == (*cc).NumOfNeighbours())
		  P.set_partition((*cc),part);
	  }
	 
	 P.update();
	for (cc = (*rc).FirstCell();!cc.IsDone(); ++cc) 	     
	  NumOfNeighbours(P,(*cc));
	NumOfNeighbours(P,(*rc));
       }
     }
   }
};


template <class GRID>
void smooth_partitioning<GRID>::
statistics(const partitioning<GRID>& P) {
  

   //__cout__ 
   cout << "NumOfUsedParts() = " << used_parts(P) << endl;
   cout << "cut_size() = " << cut_size(P) << endl;
   for (int part = 0; part <  used_parts(P);part++) 
     cout << "balance(" << part << ") = " << balance(P,part) << "%\n";
};

template <class GRID>
void smooth_partitioning<GRID>::
NumOfNeighbours(partitioning<GRID> const& P,
		const Cell & C) {
  
  CellOnCellIterator cc;
  cells[C]=0;
  for (cc = C.FirstCell(); ! cc.IsDone(); ++cc) 
    if(P.partition(C) != P.partition(*cc))
      cells[C]++;
      
};

#endif
