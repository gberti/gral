/*! \file test-triang2d-construct.C
  
    Testing the construction of Triang2D,
    and the CalculateNeighborCells
    algorithm.

 */

#include <fstream>
#include <string>

#include "IO/control-device.h"

#include "Container/tuple.h"

#include "Gral/Grids/Triang2D/all.h"
#include "Gral/Grids/Triang2D/test-triang2d.h"

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/Algorithms/cell-neighbor-search.h"

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
  using std::ostream;

  typedef grid_types<Triang2D> gt; 

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"test.in","main");

  std::string gridfile_nm;
  RegisterAt(Ctrl, "-f", gridfile_nm);

  TestTriang2D Test;
  Test.register_at(Ctrl);

  Ctrl.update();

  ostream& testout(cout);  

  IstreamComplex2DFmt Gsrc(gridfile_nm);
  Triang2D T;
  stored_geometry_triang2d GeomT(T);

  bijective_mapping<int,int> vcorr;
  dummy_mapping<int,int>     ccorr;
  ConstructGridVC(T,GeomT, Gsrc, Gsrc, vcorr,ccorr);

  OstreamComplex2DFmt Gout(gridfile_nm +".out");
  ConstructGrid(Gout, T, GeomT);
  
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
