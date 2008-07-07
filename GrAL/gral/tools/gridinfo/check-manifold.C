
/*! \file
   Test for check_manifold_property
*/

#include "Gral/Algorithms/manifold-check.h"

#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Subranges/enumerated-subrange.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/complex2d-format-output.h"


#include "IO/control-device.h"

#include <string>
#include <iostream>

template<class GRID>
int check(int argc, char* argv[]) // std::string const& gfile, std::ostream& out)
{
  using namespace GrAL;
  using namespace std;

  int res = 0;

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string p = "     ";
  string command = argv[0];
  string h = command + ": Check if input grid is a manifold with boundary\n";
  h += "Usage: " + command + " <options>\n";
  string grid_in;
  RegisterAt(Ctrl, "-in", grid_in);
  h += (p + "-in <file> (grid in Complex2D format)\n");
  string grid_out;
  RegisterAt(Ctrl, "-out", grid_out);
  h += (p + "-out <file> (fixed mesh in Complex2D format, currently only isolated vertices are fixed)\n");

  AddHelp(Ctrl, h);
  Ctrl.update();

  if(grid_in == "") {
    cerr << h << endl;
    return 2;
  }

  std::ostream& out = cerr;

  out << "Checking grid \"" << grid_in << "\" ... " << std::endl;

  typedef GRID grid_type;
  typedef grid_types<grid_type> gt;
  typedef GrAL::IstreamComplex2DFmt::coord_type coord_type;
  typedef simple_geometry<grid_type, coord_type> geom_type;
  grid_type G;
  geom_type GeomG(G);
  GrAL::IstreamComplex2DFmt In(grid_in);
  ConstructGrid(G, GeomG, In, In);

  out << "|V|=" << G.NumOfVertices() << " |E|=" << G.NumOfEdges() << " |C|=" << G.NumOfCells() 
      << std::endl;

  enumerated_element_range<typename gt::Vertex> singular_interior_vertices(G);
  enumerated_element_range<typename gt::Vertex> singular_boundary_vertices(G);
  enumerated_element_range<typename gt::Vertex> isolated_vertices(G);
  enumerated_element_range<typename gt::Facet>  singular_facets  (G);
  typedef grid_types<enumerated_element_range<typename gt::Vertex> > vgt;
  typedef grid_types<enumerated_element_range<typename gt::Facet > > fgt;



  bool is_mf = check_manifold_property(G, singular_facets, 
				       isolated_vertices,
				       singular_interior_vertices,
				       singular_boundary_vertices);

  out << "Grid \"" << grid_in << "\" is" << (is_mf ? "" : " not" ) << " a manifold grid\n";
  if(! is_mf) {
    if(singular_facets.size() > 0) {
      out << singular_facets.size() << " singular edges: ";
      for(typename fgt::EdgeIterator e = GrAL::begin_x(singular_facets); !e.IsDone(); ++e)
	out << "[" << (*e).v1() << "," << (*e).v2() << "] ";
      out << "\n";
    }
    if(isolated_vertices.size() > 0) {
      out << isolated_vertices.size() << " isolated vertices: ";
      for(typename vgt::VertexIterator v = GrAL::begin_x(isolated_vertices); !v.IsDone(); ++v)
	out << v.handle() << ",";
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


  if(grid_out != "") {
    if(is_mf) {
      out << "Writing unchanged grid to \"" << grid_out << "\" ... " << flush;
      OstreamComplex2DFmt Out(grid_out);
      ConstructGrid(Out, G, GeomG);
      out << " done." << endl;
    }
    else {
      if(singular_facets           .size() > 0 ||
	 singular_interior_vertices.size() > 0 ||
	 singular_boundary_vertices.size() > 0)
	out << "WARNING: Can only fix isolated vertices!" << endl;
      enumerated_subrange<grid_type> fixed_grid(G);
      for(typename gt::CellIterator c = GrAL::begin_x(G); !c.IsDone(); ++c)
	fixed_grid.push_back(*c);
      out << "Writing fixed grid to \"" << grid_out << "\" ... " << flush;
      OstreamComplex2DFmt Out(grid_out);
      ConstructGrid(Out, fixed_grid, GeomG);
      out << " done." << endl;
    }
    
  }

  return res;
}

int main(int argc, char * argv[]) {

  using namespace GrAL;
  using namespace std;

  typedef GrAL::Complex2D grid_type;

  int res = check<grid_type>(argc, argv);
  return res;
}
