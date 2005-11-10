/*! \file
 
*/


#include "Gral/Iterators/cell-on-cell-iterator.h"
#include "Gral/Base/facet-on-cell-function.h"
#include "Container/my-hash-map.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Algorithms/cell-neighbor-search.h"
#include <iostream>


typedef GrAL::grid_types<GrAL::Complex2D> cgt;

class hasher_facet_on_cell {
public:
  typedef cgt::FacetOnCellIterator key_type;

  unsigned operator()(cgt::FacetOnCellIterator const& fc) const 
  { return 4*fc.TheCell().handle() + fc.local_handle();}
};



int main() {
  using namespace GrAL;
  using namespace std;
  using namespace cartesian2d;
  
  CartesianGrid2D R(3,3);
  Complex2D       G; 
  ConstructGrid0(G,R);
 

  // Possibility #1: Explicit calculation of neighbor table
  typedef STDHASH::hash_map<cgt::FacetOnCellIterator, cgt::cell_handle,
                            hasher_facet_on_cell>   nb_table_type;
  nb_table_type nbs;
  CalculateNeighborCells(nbs,G);
  typedef cell_on_cell_iterator<Complex2D, nb_table_type> c_o_c_iter_1;
  c_o_c_iter_1::map_nb_table(G,nbs);

  // Possibility #2: Let cell_on_cell_iterator do the work
  typedef cell_on_cell_iterator<Complex2D> c_o_c_iter_2;
  c_o_c_iter_2::init(G);

  // compare result with native CellOnCellIterator

  //  test_cell_on_cell_iterator(R, cit);

  for(cgt::CellIterator c(G); ! c.IsDone(); ++c) {
    cout << "Cell " << (*c).handle() << " has " << (*c).NumOfCells() <<  " neighbors " << endl;
    cout << "Contents of nbs: " << flush;
    for(cgt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc) {
      cout << " ";
      if(nbs.find(fc) == nbs.end())
	cout << " (B) ";
      else
	cout << nbs[fc] << " ";
    }
    cout << endl;
    cout << "With generic iterator #1: " << flush;
    for( c_o_c_iter_1 cc(*c); ! cc.IsDone(); ++cc) {
      cgt::Cell nb = *cc;
      cout << "[" << nb.handle() << "] " << flush;
    }
    cout << endl;
    cout << "With generic iterator #2: " << flush;
    for( c_o_c_iter_2 cc(*c); ! cc.IsDone(); ++cc) {
      cgt::Cell nb = *cc;
      cout << "[" << nb.handle() << "] " << flush;
    }
    cout << endl;
    cout << "With generic iterator #2 (all facets): " << flush; 
    for(c_o_c_iter_2 cc((*c).FirstFacet()); ! cc.IsDone(); cc.next_facet())
      cout  << "[" << cc.handle() << "]" << (cc.valid_neighbor() ? "i" : "b" ) << " " << flush;
    cout << endl;

    cout << "With native  iterator:    " << flush;
    for(cgt::CellOnCellIterator cc(*c); ! cc.IsDone(); ++cc) {
      cgt::Cell nb = *cc;
      cout << "[" << nb.handle() << "] " << flush;
    }
    cout << endl;

  }

}
