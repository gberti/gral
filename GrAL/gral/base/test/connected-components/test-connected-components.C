// $LICENSE_NEC

#include "Gral/Subranges/connected-components.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Subranges/enumerated-subrange.h"

#include <iostream>

/*! \file 

   Tests for connected_components
*/

int main() {
  using namespace GrAL;
  using namespace std;
  using namespace cartesian2d;

  //  CartesianGrid2D R0x0(0,0); // empty: core dump.
  CartesianGrid2D R0x0(1,1); // 0 cells
  CartesianGrid2D R1x1(2,2); // 1 cell
  CartesianGrid2D R2x2(3,3); // 4 cells

  namespace cc = connected_components;
  {
    typedef cc::component_list<grid_types<CartesianGrid2D> > comp_list_t;
    typedef comp_list_t::const_iterator comp_iterator;
    typedef comp_list_t::component_type component_type;
    typedef component_type::CellIterator compCellIterator;
    
    comp_list_t C0x0(R0x0);
    comp_list_t C1x1(R1x1);
    comp_list_t C2x2(R2x2);
    
    cout << "C0x0 : " << C0x0.NumOfComponents() << " components " << endl;
    cout << "C1x1 : " << C1x1.NumOfComponents() << " components " << endl;
    cout << "C2x2 : " << C2x2.NumOfComponents() << " components " << endl;
    
    int i = 0;
    cout << "Components of C0x0:" << endl;
    for(comp_iterator c=C0x0.begin(); c != C0x0.end(); ++c, ++i) {
      cout << " comp. " << i << ": " << (*c).NumOfCells() << " cells " << endl;
      for(compCellIterator ci(*c); ! ci.IsDone(); ++ci)
	cout << (*ci).ll() << "  ";
      cout << endl;
    }
    i = 0;
    cout << "Components of C1x1:" << endl;
    for(comp_iterator c=C1x1.begin(); c != C1x1.end(); ++c, ++i){
      cout << " comp. " << i << ": " << (*c).NumOfCells() << " cells " << endl;
      for(compCellIterator ci(*c); ! ci.IsDone(); ++ci)
	cout << (*ci).ll() << "  ";
      cout << endl;
    }
    i = 0;
    cout << "Components of C2x2:" << endl;
    for(comp_iterator c=C2x2.begin(); c != C2x2.end(); ++c, ++i) {
      cout << " comp. " << i << ": " << (*c).NumOfCells() << " cells " << endl;
      for(compCellIterator ci(*c); ! ci.IsDone(); ++ci)
	cout << (*ci).ll() << "  ";
      cout << endl;
    }
  }

  // test grids with several components
  {
    typedef grid_types<CartesianGrid2D>     rgt;
    typedef enumerated_subrange<CartesianGrid2D> range_type;
    range_type  SR(R2x2);
    for(rgt::CellIterator c(R2x2); ! c.IsDone(); ++c)
      if( ((*c).ll().x() + (*c).ll().y()) % 2 == 0)
	SR.append_cell(c.handle());
    // should be done automatically!
    SR.init();

    // using SR directly would not have the desired effect, as the CellOnCelliterator
    // used for finding connected components would reach beyond the subrange cells!
    Complex2D G;
    ConstructGrid0(G,SR);
 
   
    typedef cc::component_list<grid_types<Complex2D> > comp_list_t;
    typedef comp_list_t::const_iterator  comp_iterator;
    typedef comp_list_t::component_type  component_type;
    typedef component_type::CellIterator compCellIterator;
    
    comp_list_t C_SR(G);
    cout << "Checkerboard grid: " << C_SR.NumOfComponents() << " components" << endl; 
    int i = 0;
    cout << "Components of C_SR:" << endl;
    for(comp_iterator c=C_SR.begin(); c != C_SR.end(); ++c, ++i) {
      cout << " comp. " << i << ": " << (*c).NumOfCells() << " cells " << endl;
      for(compCellIterator ci(*c); ! ci.IsDone(); ++ci)
	cout << (*ci).handle() << "  ";
      cout << endl;
    }
  }
}
