
/*! \file
   Test for check_manifold_property
*/

#include "Gral/Algorithms/manifold-check.h"
#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format-input.h"

#include "IO/control-device.h"

#include <string>
#include <iostream>

template<class GRID>
void check(std::string const& gfile, std::ostream& out)
{
  using namespace GrAL;
  out << "Checking grid \"" << gfile << "\" ... " << std::endl;

  typedef grid_types<GRID> gt;
  GRID G;
  GrAL::IstreamComplex2DFmt In(gfile);
  ConstructGrid0(G, In);

  out << "|V|=" << G.NumOfVertices() << " |E|=" << G.NumOfEdges() << " |C|=" << G.NumOfCells() 
      << std::endl;

  enumerated_element_range<typename gt::Vertex> singular_interior_vertices(G);
  enumerated_element_range<typename gt::Vertex> singular_boundary_vertices(G);
  enumerated_element_range<typename gt::Facet>  singular_facets  (G);
  typedef grid_types<enumerated_element_range<typename gt::Vertex> > vgt;
  typedef grid_types<enumerated_element_range<typename gt::Facet > > fgt;



  bool is_mf = check_manifold_property(G, singular_facets, 
				       singular_interior_vertices,
				       singular_boundary_vertices);

  out << "Grid \"" << gfile << "\" is" << (is_mf ? "" : " not" ) << " a manifold grid\n";
  if(! is_mf) {
    if(singular_facets.size() > 0) {
      out << singular_facets.size() << " singular edges: ";
      for(typename fgt::EdgeIterator e = GrAL::begin_x(singular_facets); !e.IsDone(); ++e)
	out << "[" << (*e).v1() << "," << (*e).v2() << "] ";
      out << "\n";
    }
    if(singular_interior_vertices.size() > 0) {
      out << singular_interior_vertices.size() << " singular interior vertices: ";
      for(typename vgt::VertexIterator v = GrAL::begin_x(singular_interior_vertices); !v.IsDone(); ++v)
	out << v.handle() << ",";
      out << "\n";
    }
    if(singular_boundary_vertices.size() > 0) {
      out << singular_boundary_vertices.size() << " singular boundary vertices: ";
      for(typename vgt::VertexIterator v = GrAL::begin_x(singular_boundary_vertices); !v.IsDone(); ++v)
	out << v.handle() << ",";
      out << "\n";
    }

    out << "\n";
  }
}

int main(int argc, char * argv[]) {

  using namespace GrAL;
  using namespace std;

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string p = "     ";
  string command = argv[0];
  string h = command + ": Check if input grid is a manifold with boundary\n";
  h += "Usage: " + command + " <options>\n";
  string grid_in;
  RegisterAt(Ctrl, "-in", grid_in);
  h += (p + "-in <file> (grid in Complex2D format)\n");

  AddHelp(Ctrl, h);
  Ctrl.update();

  if(grid_in == "") {
    cerr << h << endl;
    return 2;
  }


  typedef Complex2D grid_type;
  
  check<grid_type>(grid_in,cout);

}
