
// $LICENSE

/*! \file
 */

#include "Gral/Grids/Cartesian3D/all.h"


#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>


#include "Gral/Test/test-iterators.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

#include "Utility/pre-post-conditions.h"

int main() {
  using namespace GrAL;
  using namespace std; 
  using namespace GrAL::cartesian3d;

  ofstream* out = new ofstream("test-cartesian-iterators.out");

  typedef grid_types<CartesianGrid3D> gt;
  // testing default constructor
  CartesianGrid3D R0;
  CartesianGrid3D R1; R1 = R0;
  vector<CartesianGrid3D> RR (10);
  vector<CartesianGrid3D> RR2(10);
  RR2 = RR;

  // Testing archetype
  OstreamComplex2DFmt OG(*out);
  ConstructGrid0(OG, * (CartesianGrid3D::BeginArchetype()));
  *out << endl;

  int NX = 4, NY = 4, NZ = 4;
  for(int nx = 0; nx < NX; ++nx) {
    for(int ny = 0; ny < NY; ++ny) {
      for(int nz = 0; nz < NZ; ++nz) {
	CartesianGrid3D R(nx,ny,nz);
	*out << "CartesianGrid3D(" << nx << ',' << ny << ',' << nz << ")\n"
	     << R.NumOfVertices() << " vertices"
	     << " (" << R.NumOfXVertices() << "," << R.NumOfYVertices() << "," << R.NumOfZVertices() << ")\n"
	     << R.NumOfEdges() << " edges (" 
	     << "x: " << R.NumOfXDirEdges()  << ' '
	     << "y: " << R.NumOfYDirEdges()  << ' '
	     << "z: " << R.NumOfZDirEdges()  <<")\n"
	     << R.NumOfFacets() << " facets ("
	     << "xy: " << R.NumOfXYDirFacets() << ' '
	     << "xz: " << R.NumOfXZDirFacets() << ' '
	     << "yz: " << R.NumOfYZDirFacets() << ")\n"
	     << R.NumOfCells()    << " cells"
	     << " (" << R.NumOfXCells() << "," << R.NumOfYCells() << "," << R.NumOfZCells() << ")\n";

	REQUIRE_ALWAYS( (R.NumOfCells() == (R.NumOfXCells()*R.NumOfYCells()*R.NumOfZCells())), "", 1);
        REQUIRE_ALWAYS( (R.NumOfVertices() == (R.NumOfXVertices()*R.NumOfYVertices()*R.NumOfZVertices())), "", 1);
	if(! R.empty()) {
	  REQUIRE_ALWAYS( (+ R.NumOfVertices() 
			   - R.NumOfEdges() 
			   + R.NumOfFacets() 
			   -  R.NumOfCells() == 1),
			  "Euler formula does not hold!\n",1);
	}
	test_vertex_iterator(R,*out);
	test_edge_iterator(R,*out);
	test_facet_iterator(R,*out);
	test_cell_iterator(R,*out);
	test_vertex_on_cell_iterator(R,*out);
	test_edge_on_cell_iterator(R,*out);
	test_facet_on_cell_iterator(R,*out);

	test_vertex_on_facet_iterator(R,*out);
	test_edge_on_facet_iterator(R,*out);

	test_vertex_on_edge_iterator(R,*out);

	test_sequence_iterator<gt::Vertex>(R, *out);
	test_sequence_iterator<gt::Edge>  (R, *out);
	test_sequence_iterator<gt::Facet> (R, *out);
	test_sequence_iterator<gt::Cell>  (R, *out);
	
	
	test_incidence_iterator<gt::Vertex, gt::Edge> (R, *out); 
	
	test_incidence_iterator<gt::Edge,   gt::Facet>(R, *out);
	test_incidence_iterator<gt::Vertex, gt::Facet>(R, *out);
	
	test_incidence_iterator<gt::Vertex, gt::Cell>(R, *out);
	test_incidence_iterator<gt::Edge,   gt::Cell>(R, *out);
	test_incidence_iterator<gt::Facet,  gt::Cell>(R, *out);

	*out << endl;
      }
    }
  }

  CartesianGrid3D R(2,2,2); // 1 cell
  gt::Cell c(R.FirstCell());
  out->width(2);
  *out << "Grid 2x2x2\n"
      << "Vertices:" << endl;
  for(gt::VertexOnCellIterator v(c); !v.IsDone(); ++v)
    *out << setw(2) << v.handle() << " @ " << v.index() << endl;
  *out << "Edges:" << endl;
  for(gt::EdgeOnCellIterator e(c); ! e.IsDone(); ++e) {
    *out << setw(2) << e.handle() << " dir=" << e.dir() << " index=" << e.index()
	 << " Vertices: ";
    for(gt::VertexOnEdgeIterator ve(*e); ! ve.IsDone(); ++ve)
      *out << setw(2) << ve.local_handle() << ": " << setw(2)  <<  ve.handle() << "@" << ve.index() << "  ";
    *out << endl;
  }
  *out << "Facets:" << endl;
  for(gt::FacetOnCellIterator f(c); ! f.IsDone(); ++f) {
    *out << f.handle() << " dir=" << f.dir() << " index=" << f.index() << endl;
    *out << " Facet vertices: ";
    for(gt::VertexOnFacetIterator vf(*f); ! vf.IsDone(); ++vf) {
      *out << vf.handle() << "@(" << vf.index() << ") "; 
    }
    *out << endl;
    *out << " Facet edges: ";
    for(gt::EdgeOnFacetIterator ef(*f); ! ef.IsDone(); ++ef) {
      *out << ef.handle() << ": dir=" << ef.dir() << " index=" << ef.index() << "  " ;
    }
    *out << endl;
  }

  delete out;
  return 0;
}
