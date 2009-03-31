/*! \file
 */

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Graph/min-cut.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format.h"
#include "Gral/IO/gmv-format-output2d.h"

#include <iostream>

template<class Graph, class Cap, class Res, class Color, class T>
void print_flow(Graph const& G, Cap const& cap, Res const& res, 
		Color const& col,
		T flow, int source_v, int sink_v, std::ostream& out)
{
  typedef boost::graph_traits<Graph> graph_t;
  out << "c  The total flow:\n" << "s " << flow <<  std::endl;
  out << "source: " << source_v  << "  sink: " << sink_v << "\n";
  typename graph_t::vertex_iterator v,vend;
  for(boost::tie(v,vend) = boost::vertices(G); v != vend; ++v) {
    typename graph_t::out_edge_iterator e,eend;
    for(boost::tie(e,eend) = boost::out_edges(*v,G); e != eend; ++e) {
      //      typename graph_t::edge_descriptor e1 = *e;
      typename Cap::key_type e1 = *e;
      if(cap[e1] > 0) 
	out << "f " << (*v) << " " << boost::target(e1,G) 
	     << " " << (cap[e1] - res[e1]) << "\n";
    }
  }
  for(boost::tie(v,vend) = boost::vertices(G); v != vend; ++v) {
    out << *v << " " << col[*v] << "\n";
  }
}



int main() {
  using namespace std;

  {
    typedef GrAL::cartesian2d::CartesianGrid2D  grid_type;
    typedef GrAL::grid_types<grid_type> gt;
    typedef GrAL::graph::graph_view<grid_type, 
      GrAL::graph::cell_neighbor_graph_tag, 
      GrAL::graph::directed_graph_tag, gt>  graph_type;
    typedef boost::graph_traits<graph_type> graph_t;
  

    grid_type  R(5,3); // 4x2 cells
    // grid_type  R(7,4); // 4x3 cells
    //grid_type R(7,2); 
    GrAL::grid_function<gt::Facet, int> cutweights(R, 1);
    GrAL::grid_function<gt::Cell, int>  partition(R,0);

    int x_mid = R.urx()/2;
    for(int y = 0; y < R.ury(); ++y) {
      cutweights[gt::Facet(gt::Vertex(R, x_mid,y), gt::Vertex(R, x_mid,y+1))] = 1; 
    }

    /*
    for(gt::FacetIterator f(R); !f.IsDone(); ++f) {
      cout << f.handle() << ": c=" << cutweights(*f) << "\n";
    }
    */

    std::vector<gt::Cell> source;
    std::vector<gt::Cell> sink;  
    for(int y = 0; y < R.ury(); ++y) {
      source.push_back(R.cell(0        ,y));
      sink  .push_back(R.cell(R.urx()-1,y));
    }

    GrAL::graph::min_cut_binary_partition(R,  source, sink, cutweights, partition);
  
    if(R.NumOfCells() <= 10)
      for(gt::CellIterator c(R); !c.IsDone(); ++c)
	cout << "cell " << (*c).index() << " part " << partition(*c) << "\n";
  }

  {
    typedef GrAL::Complex2D  grid_type;
    typedef GrAL::grid_types<grid_type> gt;
    typedef GrAL::graph::graph_view<grid_type, 
      GrAL::graph::cell_neighbor_graph_tag, 
      GrAL::graph::directed_graph_tag, gt>  graph_type;
    typedef boost::graph_traits<graph_type> graph_t;
    typedef GrAL::stored_geometry_complex2D geom_type;
    typedef geom_type::coord_type           coord_type;
    typedef GrAL::point_traits<coord_type>  pt;
    
    std::string gridinfile  = "grid.complex2d";
    std::string gridoutfile = "grid.gmv";

    grid_type G;
    geom_type GeomG(G);
    GrAL::IstreamComplex2DFmt In(gridinfile);
    GrAL::ConstructGrid(G,GeomG, In,In);

    GrAL::grid_function<gt::Facet, int> cutweights(G,0);
    GrAL::grid_function<gt::Cell, int>  partition (G,0);

    double min_length = GeomG.length(*G.FirstEdge());
    for(gt::EdgeIterator e(G); !e.IsDone(); ++e)
      min_length = min(min_length, GeomG.length(*e));

    for(gt::EdgeIterator e(G); !e.IsDone(); ++e)
      cutweights[*e] = round(10000*GeomG.length(*e)/min_length);

    std::vector<gt::Cell> source;
    std::vector<gt::Cell> sink;
    double eps = 1.0e-6;
    for(gt::CellIterator c(G); !c.IsDone(); ++c) {
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	if     (pt::x(GeomG.coord(*vc)) <= 0.0 + eps) {
	  source.push_back(*c);
	  break;
	}
	else if(pt::x(GeomG.coord(*vc)) >= 1.0 - eps) {
	  sink.push_back(*c);
	  break;
	}
    }
    cout << "Source: " << source.size() << " cells, sink: " << sink.size() << " cells." << endl;
    //source.push_back(gt::Cell(G, 0));
    //sink  .push_back(gt::Cell(G, G.NumOfCells()-1));

    int min_cut_size = GrAL::graph::min_cut_binary_partition(G,  source, sink, cutweights, partition);
 
    cout << "Cut size: " << (min_length/10000) * min_cut_size << endl;

    if(G.NumOfCells() <= 10) 
      for(gt::CellIterator c(G); !c.IsDone(); ++c)
	cout << "cell " << (*c).handle() << " part " << partition(*c) << "\n";

    
    
    GrAL::OstreamGMV2DFmt Out(gridoutfile);
    GrAL::ConstructGrid_GF(Out,G,GeomG,
			   (Out.BeginGFs(), Out.pair("part", partition)));
  }  
}
