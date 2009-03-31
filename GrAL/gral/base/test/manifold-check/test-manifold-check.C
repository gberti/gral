
/*! \file
   Test for check_manifold_property
*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Algorithms/manifold-check.h"
#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Triang2D/all.h"
#include "Gral/IO/complex2d-format-input.h"

#include <string>
#include <iostream>

template<class GRID>
void check(std::string const& gfile, std::ostream& out)
{
  using namespace GrAL;
  typedef grid_types<GRID> gt;
  GRID G;
  GrAL::IstreamComplex2DFmt In(gfile);
  ConstructGrid0(G, In);

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
  out << "Grid \"" << gfile << "\" is" << (is_mf ? "" : " not" ) << " a manifold grid\n";
  if(! is_mf) {
    out << "|V|=" << G.NumOfVertices() << " |E|=" << G.NumOfEdges() << " |C|=" << G.NumOfCells() << "\n";
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
}

int main() {

  using namespace GrAL;
  using namespace std;

  std::string gfile[] = { string("manifold.complex2d"), 
			  string("isolated-vertex.complex2d"),
			  string("singular-edge.complex2d"),
			  string("singular-bd-vertex.complex2d"),
			  string("singular-interior-vertex.complex2d"),
			  // interior singular vtx hidden by singular edge
			  string("all-singularities.complex2d")}; 
  typedef Triang2D grid_type;
  //typedef Complex2D grid_type;

  for(int i = 0; i < sizeof(gfile)/sizeof(string); ++i)
    check<grid_type>(gfile[i], cout);

}
