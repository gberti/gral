

#include <fstream.h>
#include "IO/control-device.h"

#include "Container/tuple.h"
#include "Grids/Triang2D/triang2d.h"
#include "Grids/Triang2D/construct.h"
#include "Grids/Triang2D/grid-functions.h"
#include "Grids/Triang2D/test-triang2d.h"

#include "Grids/Reg2D/cartesian-grid2d.h"
#include "Grids/Adapter/neumann-triang2d.h"
#include "Grids/Algorithms/cell-neighbor-search.h"

#include "Container/bijective-mapping.h"
#include "Container/dummy-mapping.h"


template<class T>
class CellFacetMapTriang2D {
private:
  typedef grid_types<Triang2D> gt; 

  //---------- DATA ---------
  grid_function<gt::Cell, tuple<T,3> > table;
public:
  typedef T                       value_type;
  typedef T                       result_type;
  typedef gt::FacetOnCellIterator argument_type;

  CellFacetMapTriang2D(Triang2D const& tri) : table(tri) {}

  T&       operator[](gt::FacetOnCellIterator const& fc) 
  { return table[fc.TheCell()][gt::local(fc)];}
  T const& operator()(gt::FacetOnCellIterator const& fc) const 
  { return table(fc.TheCell())[gt::local(fc)];}
};

int main(int argc, char* argv[]) {

  typedef grid_types<Triang2D> gt; 

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"test.in","main");
  
  int nx = 4;
  int ny = 5;
  RegisterAt(Ctrl,"-nx",nx);
  RegisterAt(Ctrl,"-ny",ny);

  TestTriang2D Test;
  Test.register_at(Ctrl);

  Ctrl.update();

  ostream& testout(cout);  

  RegGrid2D  source(nx,ny);
  neumann_triang2d<RegGrid2D> Nm(source);
  Triang2D T;

  bijective_mapping<int,int> vcorr;
  dummy_mapping<int,int>     ccorr;
  ConstructGrid0(T,Nm,vcorr,ccorr);
  
  testout << "Testing T\n";
  Test.test_iterators(T,testout);
  testout << "---------------------------------------------------\n"
	  << "---------------------------------------------------\n\n";

  CellFacetMapTriang2D<gt::cell_handle> nbf(T);
  CalculateNeighborCells(nbf,T);
 
  testout << "\n\n";
  testout << "Cell Neighbors\n";
  for(gt::CellIterator c(T); ! c.IsDone(); ++c) {
    for(gt::FacetOnCellIterator fc(c); ! fc.IsDone(); ++fc)
      testout << nbf[fc] << ' ';
    testout << '\n';
  }
  return 0;
}
