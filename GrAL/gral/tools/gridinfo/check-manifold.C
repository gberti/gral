
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

#include "Container/partial-mapping.h"
#include "Container/range.h"

#include "IO/control-device.h"

#include <string>
#include <vector>
#include <iostream>

template<class GRID, class GF_F, class GF_V>
void  manifold_checking_report(std::ostream& out, std::string const& grid_name, bool is_mf,
			       GRID const& G,
			       GF_F const& singular_facets, 
			       GF_V const& isolated_vertices,
			       GF_V const& singular_interior_vertices,
			       GF_V const& singular_boundary_vertices)
{
  using namespace GrAL;
  typedef grid_types<GF_V> vgt;
  typedef grid_types<GF_F> fgt;

  out << "Grid \"" << grid_name << "\" is" << (is_mf ? " " : " NOT " ) << "a manifold grid.\n";
  out << "|V|=" << G.NumOfVertices() << " |E|=" << G.NumOfEdges() << " |C|=" << G.NumOfCells() 
      << std::endl;

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
  }
}



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
  h += " Return is 0 if mesh is manifold or could be fixed, 1 if it is not, 2 if file not found.\n";
  string grid_in;
  RegisterAt(Ctrl, "-in", grid_in);
  h += (p + "-in <file> (grid in Complex2D format)\n");
  string grid_out;
  RegisterAt(Ctrl, "-out", grid_out);
  h += (p + "-out <file> (fixed mesh in Complex2D format, currently only isolated vertices and duplicated cells are fixed)\n");

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
  manifold_checking_report(out, grid_in, is_mf,
			   G,
			   singular_facets, 
			   isolated_vertices,
			   singular_interior_vertices,
			   singular_boundary_vertices);


    

  // search for duplicate cells
  typedef typename gt::Cell            Cell;
  typedef typename gt::cell_handle     cell_handle;
  typedef vertex_set<Cell>             vset;
  typedef std::vector<cell_handle>     cell_set;
  partial_mapping<vset, cell_handle>   cell_of_vs;
  partial_mapping<vset, cell_set>      duplicate_cells_of_vs;
  if(! is_mf) {
    for(typename gt::CellIterator c = GrAL::begin_x(G); c != GrAL::end_x(G); ++c) {
      vset vs(*c);
      if(cell_of_vs.defined(vs)) {
	// c is a duplicate cell of vs
	if(!duplicate_cells_of_vs.defined(vs))
	  //c is the first duplicate of vs: Save first occurence, too.
	  duplicate_cells_of_vs[vs].push_back(cell_of_vs[vs]);
	duplicate_cells_of_vs[vs].push_back(c.handle());
      }
      else {
	// c unique so far
	cell_of_vs[vs] = c.handle();
      }
    }
    // report 
    if(!duplicate_cells_of_vs.empty()) {
      out << duplicate_cells_of_vs.size() << " sets of duplicate cells:\n";
      for(typename  partial_mapping<vset, cell_set>::const_iterator dup 
	    = duplicate_cells_of_vs.begin(); dup != duplicate_cells_of_vs.end(); ++dup) {
	out << "Vertex set " << range_c(dup->first) << ": cells " << range_c(dup->second) << "\n";
      }
    }
  }
  out << "\n";
  


  if(grid_out != "") {
    if(is_mf) {
      out << "Writing unchanged grid to \"" << grid_out << "\" ... " << flush;
      OstreamComplex2DFmt Out(grid_out);
      ConstructGrid(Out, G, GeomG);
      out << " done." << endl;
    }
    else { // try to fix
      if(singular_facets           .size() > 0 ||
	 singular_interior_vertices.size() > 0 ||
	 singular_boundary_vertices.size() > 0)
	out << "WARNING: Can only fix isolated vertices and duplicate cells!" << endl;
      enumerated_subrange<grid_type> fixed_grid(G);
      for(typename gt::CellIterator c = GrAL::begin_x(G); !c.IsDone(); ++c) {
	vset vs(*c);
	if(!duplicate_cells_of_vs.defined(vs))
	  fixed_grid.push_back(*c);
      }
      // check if fixed grid is really mf.
      out << "Checking fixed grid:\n";
      grid_type FixedG;
      geom_type FixedGeom(FixedG);
      ConstructGrid(FixedG, FixedGeom, fixed_grid, GeomG);
      enumerated_element_range<typename gt::Vertex> singular_interior_vertices_fixed(FixedG);
      enumerated_element_range<typename gt::Vertex> singular_boundary_vertices_fixed(FixedG);
      enumerated_element_range<typename gt::Vertex> isolated_vertices_fixed(FixedG);
      enumerated_element_range<typename gt::Facet>  singular_facets_fixed  (FixedG);
      
      bool is_fixed_mf = check_manifold_property(FixedG, 
						 singular_facets_fixed, 
						 isolated_vertices_fixed,
						 singular_interior_vertices_fixed,
						 singular_boundary_vertices_fixed);
      manifold_checking_report(out, grid_in + " with fixes", is_fixed_mf,
			       FixedG,
			       singular_facets_fixed, 
			       isolated_vertices_fixed,
			       singular_interior_vertices_fixed,
			       singular_boundary_vertices_fixed);


      out << "Writing fixed grid to \"" << grid_out << "\" ... " << flush;
      OstreamComplex2DFmt Out(grid_out);
      ConstructGrid(Out, FixedG, FixedGeom);
      out << " done." << endl;
      res = is_fixed_mf;
    }
    
  }

  return res;
}

int main(int argc, char * argv[]) {

  using namespace GrAL;
  using namespace std;

  typedef GrAL::Complex2D grid_type;

  int res = check<grid_type>(argc, argv);
  return (res ? 0 : 1);
}
