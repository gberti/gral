/*! \file
 */

#include "Gral/Graph/BGL/cell-neighbor-graph.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#include <boost/ref.hpp>

#include <iostream>

namespace GrAL { namespace graph {
  typedef cartesian2d::CartesianGrid2D  grid_type;
  typedef grid_types<grid_type>         gt;
  template class graph_view<grid_type, cell_neighbor_graph_tag, directed_graph_tag, gt>;
  typedef graph_view<grid_type, cell_neighbor_graph_tag, directed_graph_tag, gt> graph_view_t;
}}

namespace boost {
  typedef GrAL::cartesian2d::CartesianGrid2D  grid_type;
  typedef GrAL::grid_types<grid_type>         gt;
  typedef GrAL::graph::graph_view<grid_type, 
				  GrAL::graph::cell_neighbor_graph_tag, 
				  GrAL::graph::directed_graph_tag,gt>  graph_type;
  
  template class  boost::IncidenceGraphConcept <graph_type>;  
  template class  boost::VertexListGraphConcept<graph_type>; 
}


int main() {
  using namespace std;

  typedef GrAL::cartesian2d::CartesianGrid2D  grid_type;
  typedef GrAL::grid_types<grid_type> gt;
  typedef GrAL::graph::graph_view<grid_type, GrAL::graph::cell_neighbor_graph_tag, GrAL::graph::directed_graph_tag, gt> graph_type;
  typedef boost::graph_traits<graph_type> graph_t;

  grid_type  R(8,8);
  graph_type G(R);
  {
    cout << "#vertices: " << boost::num_vertices(G)
	 << "  "
	 << "#edges: "    << boost::num_edges(G) << endl;
    graph_t::vertex_iterator v, vend;
    for(boost::tie(v,vend) = boost::vertices(G); v != vend; ++v) {
      cout << "v" << (*v)  << " edges: ";
      graph_t::out_edge_iterator e,eend;
      for(boost::tie(e,eend) = boost::out_edges(*v,G); e != eend; ++e)
	cout << boost::source(*e,G) << " -> " << boost::target(*e,G) << ", ";
      cout << endl;
    }
  }

  graph_t::vertex_descriptor startv = * boost::vertices(G).first;
  graph_t::edge_descriptor e = * boost::out_edges(startv, G).first;
  graph_t::vertex_descriptor vs = boost::source(e,G);
  graph_t::vertex_descriptor vt = boost::target(e,G);
  int nv = boost::num_vertices(G);
  graph_t::out_edge_iterator_range r; // (e,e);


  typedef boost::default_color_type color_type;
  typedef boost::color_traits<color_type> Color;
  graph_t::vertex_property_map<color_type>::type colors(G);
  boost::put(colors,startv, Color::white());
  color_type c = boost::get(colors, startv);


  graph_t::edge_property_map<int>::type weight(G,0);
  boost::put(weight, e,1);
  int w = boost::get(weight, e);

  boost::queue<graph_t::vertex_descriptor>  buffer;
  boost::bfs_visitor<boost::null_visitor> vis;

  graph_t::vertex_property_map<int>::type d(G,0);

  boost::breadth_first_search(G, startv, buffer,
			      boost::make_bfs_visitor(boost::record_distances(boost::ref(d), boost::on_tree_edge())),
			      colors);

  graph_t::vertex_iterator v, vend;
  for (boost::tie(v, vend) = boost::vertices(G); v != vend; ++v)
    cout << (*v) << " " << boost::get(d,*v) << "\n";

 
  graph_t::vertex_index_map_type vertex_indices = graph_t::vertex_index_map(G);
  graph_t::reverse_edge_map_type reverse_edges  = graph_t::reverse_edge_map(G);
  
  graph_t::edge_property_map<int>::type capacity(G,0);
  graph_t::edge_property_map<int>::type residual_cap(G,0);
  for(boost::tie(v,vend) = boost::vertices(G); v != vend; ++v) {
    graph_t::out_edge_iterator e,eend;
    for(boost::tie(e,eend) = boost::out_edges(*v,G); e != eend; ++e) {
      graph_t::edge_descriptor e1 = *e;
      capacity[e1]            = 1;
      residual_cap[e1] = 0;// capacity[e1];
      //capacity[G.reverse(e1)] = 0;
    }
  }

  graph_t::vertex_descriptor source_v(R.cell(0,0));
  graph_t::vertex_descriptor sink_v  (R.cell(R.urx()-1,R.ury()-1));

  int flow =  boost::push_relabel_max_flow(G, source_v, sink_v, 
					   boost::ref(capacity), boost::ref(residual_cap),
					   reverse_edges, vertex_indices);

  cout << "c  The total flow:\n" << "s " << flow <<  endl;
  cout << "source: " << source_v  << "  sink: " << sink_v << "\n";
  for(boost::tie(v,vend) = boost::vertices(G); v != vend; ++v) {
    graph_t::out_edge_iterator e,eend;
    for(boost::tie(e,eend) = boost::out_edges(*v,G); e != eend; ++e) {
      graph_t::edge_descriptor e1 = *e;
      if(capacity[e1] > 0) 
	cout << "f " << (*v) << " " << boost::target(e1,G) 
	     << " " << (capacity(e1) - residual_cap(e1)) << "\n";
    }
  }

}
