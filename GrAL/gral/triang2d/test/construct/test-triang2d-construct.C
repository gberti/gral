/*! \file test-triang2d-construct.C
  
    Testing the construction of Triang2D,
    and the CalculateNeighborCells
    algorithm.

 */

#include <iostream>
#include <fstream>
#include <string>

#include "IO/control-device.h"

#include "Container/tuple.h"

#include "Gral/Grids/Triang2D/all.h"
#include "Gral/Grids/Triang2D/test-triang2d.h"

#include "Gral/Test/all.h"

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/Algorithms/cell-neighbor-search.h"

#include "Container/bijective-mapping.h"
#include "Container/dummy-mapping.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

template<class T>
class CellFacetMapTriang2D {
private:
  typedef GrAL::grid_types<GrAL::Triang2D> gt; 

  //---------- DATA ---------
  GrAL::grid_function<gt::Cell, GrAL::tuple<T,3> > table;
public:
  typedef T                       value_type;
  typedef T                       result_type;
  typedef gt::FacetOnCellIterator argument_type;

  CellFacetMapTriang2D(GrAL::Triang2D const& tri) : table(tri) {}

  T&       operator[](gt::FacetOnCellIterator const& fc) 
  { return table[fc.TheCell()][gt::local(fc)];}
  T const& operator()(gt::FacetOnCellIterator const& fc) const 
  { return table(fc.TheCell())[gt::local(fc)];}
};


int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

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

  typedef grid_types<IstreamComplex2DFmt> srcgt;
  typedef grid_types<Triang2D>            dstgt;
  bijective_mapping<srcgt::Vertex, dstgt::Vertex> vcorr;
  dummy_mapping<int,int>     ccorr;
  ConstructGridVC(T,GeomT, Gsrc, Gsrc, vcorr,ccorr);

  

  test_vertex_iterator(T, cout);
  test_edge_iterator  (T, cout);
  test_cell_iterator  (T, cout);
  test_vertex_on_cell_iterator(T, cout);
  test_facet_on_cell_iterator (T, cout);
  test_archetypes(T, cout);

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

  {   
    gt::VertexIterator vb = GrAL::begin_x(T);
    gt::VertexIterator ve = GrAL::end_x  (T);
    gt::CellIterator   cb = GrAL::begin_x(T);
    gt::CellIterator   ce = GrAL::end_x  (T);
    gt::VertexOnCellIterator vcb = GrAL::begin_x(*cb);
    gt::VertexOnCellIterator vce = GrAL::end_x  (*cb);
    gt::EdgeOnCellIterator   ecb = GrAL::begin_x(*cb);
    gt::EdgeOnCellIterator   ece = GrAL::end_x  (*cb);
    int nv = GrAL::size_d<0>(T);
    int ne = GrAL::size_d<1>(T);
    int nc = GrAL::size_d<2>(T);
    int nvc = GrAL::size_d<0>(*cb);
    int nec = GrAL::size_d<1>(*ce);
  }
  {
    gt::VertexIterator vb = GrAL::begin_d<0>(T);
    gt::VertexIterator ve = GrAL::end_d  <0>(T);
    gt::EdgeIterator   eb = GrAL::begin_d<1>(T);
    gt::EdgeIterator   ee = GrAL::end_d  <1>(T);
    gt::CellIterator   cb = GrAL::begin_d<2>(T);
    gt::CellIterator   ce = GrAL::end_d  <2>(T);

    gt::VertexOnCellIterator vcb = GrAL::begin_d<0>(*cb);
    gt::VertexOnCellIterator vce = GrAL::end_d  <0>(*cb);
    gt::EdgeOnCellIterator   ecb = GrAL::begin_d<1>(*cb);
    gt::EdgeOnCellIterator   ece = GrAL::end_d  <1>(*cb);
  }

  {

    test_sequence_iterator<gt::Vertex>(T, cout);
    test_sequence_iterator<gt::Edge>  (T, cout);
    test_sequence_iterator<gt::Cell>  (T, cout);

    test_incidence_iterator<gt::Vertex, gt::Cell>(T, cout);
    test_incidence_iterator<gt::Edge,   gt::Cell>(T, cout);
    test_incidence_iterator<gt::Vertex, gt::Edge>(T, cout);

    typedef GrAL::element_handle_d<gt,0>::type  vertex_handle;
    typedef GrAL::element_d<gt,0>::type Vertex;
    typedef GrAL::sequence_iterator_d<gt,0>::type VertexIterator;
    typedef GrAL::incidence_iterator_d<gt,0,2>::type VertexOnCellIterator;

    BOOST_STATIC_ASSERT( (1 == boost::is_same<gt::vertex_handle,  GrAL::element_handle_d   <gt,0>::type>::value));
    BOOST_STATIC_ASSERT( (1 == boost::is_same<gt::Vertex,         GrAL::element_d          <gt,0>::type>::value));
    BOOST_STATIC_ASSERT( (1 == boost::is_same<gt::VertexIterator, GrAL::sequence_iterator_d<gt,0>::type>::value));
    BOOST_STATIC_ASSERT( (1 == boost::is_same<gt::VertexOnCellIterator, 
			                      GrAL::incidence_iterator_d<gt,0,2>::type>
			  ::value));

  }

  return 0;  

}
