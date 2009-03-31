#ifndef NMWR_GB_GRID_PARTITION_C
#define NMWR_GB_GRID_PARTITION_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Partitioning/partitioning.h"

// STD
#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h> // for isdigit()

#include "Container/partial-mapping.h"
#include "Gral/Partitioning/collect-element-partitions.h"

namespace GrAL {

template<class Grid> 
void partitioning<Grid>::write_partition(std::ostream& out)
{
 CellIterator C = TheGrid().FirstCell();
 for(; ! C.IsDone(); ++C)
   out << the_partitions[*C] << '\n';
}

template<class Grid> 
void partitioning<Grid>::read_partition(std::istream& in)
{ 
 int p;
 partial_mapping<int,int> part_nums(-1);
 CellIterator C = TheGrid().FirstCell();
 
 
 char c;
 in >> c;
 in.putback(c);
 if(isdigit(c)) {
   while(in && ! C.IsDone()) {
     in >> p;
     int np = ranges.size()-1; //NumOfPartitions();
     for( int pp = 0; pp <= p - np; ++pp)
       add_partition();
     // if( part_nums(p) == -1) {
     //  part_nums[p] = add_partition();
     // }
     add_cell(p,*C);
     ++C;
   }
   if(!in && ! C.IsDone()) {
     std::cerr << "partitioning<Grid>::read_partition(): input ended prematurely!\n"
	       << "creating new partition for the remaining cells.\n";
     p = add_partition();
     while(! C.IsDone()) {
       add_cell(p,*C);
       ++C;
     }
   }
 }
 else {
   std::string format;
   in >> format;
   if(format == "handle-partition-pair")
     read_handle_partition_pair(*this,in);
   else {
     REQUIRE(false, 
	     "partition file format " << format << " not recognized!\n"
	     << "valid formats: " 
	     <<  "handle-partition-pair\n",
	     1);
   }
 }
}


template<class Grid> 
void partitioning<Grid>::calculate_ranges() 
{
  int max_partition = -1;
  for(CellIterator C = TheGrid().FirstCell(); !C.IsDone(); ++C) {
    REQUIRE(the_partitions(*C) >= 0, "partition of cell = " << the_partitions(*C),1);
    max_partition = std::max(max_partition, the_partitions(*C));
  }
  ranges.clear();
  if(max_partition >= 0)
    ranges.resize(max_partition+1);

  for(int i = 0; i < (int)ranges.size(); ++i)
    ranges[i] = cell_range(TheGrid()); // empty
  for(CellIterator C = TheGrid().FirstCell(); !C.IsDone(); ++C)
    ranges[the_partitions(*C)].append(*C);
} 

template<class Grid>
void read_handle_partition_pair(partitioning<Grid> & P, std::istream& in)
{
  typedef grid_types<Grid> gt;
  typedef typename gt::Cell         Cell;
  typedef typename gt::cell_handle  cell_handle;

  int p;
  cell_handle c;
  partial_mapping<int,int> part_nums(-1);
  partial_grid_function<Cell,bool> marked(P.TheGrid(),false);

  int cnt = 0;
  while(in && cnt < (int) P.TheGrid().NumOfCells()) {
    in >> c >> p;
    in >> std::ws;
    ++cnt;
    if( part_nums(p) == -1) {
      part_nums[p] = P.add_partition();
    }
    Cell C = P.TheGrid().cell(c);
    if( ! marked(C)) {
      marked[C] = true;
      P.add_cell(part_nums(p),C);
      std::cerr << "added C = " << c <<  "; p = " << part_nums(p) << std::endl;
    }
    else {
      ENSURE(false, 
	     "partition p of cell c multiply defined: "
	     << "c = " << c << "; p = " << p << "!\n",1);
    }
  }
  
}



template<class Partition>
void PartitionsByVertex<Partition>::calculate_vertex_partitions()
{
  collect_vertex_partitions(ThePartitioning(),
			    vertex_partitions,
			    (bool)false);             // do not count ouside as partition
}



} // namespace GrAL {

#endif
