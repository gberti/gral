#ifndef GRAL_GB_GRAPH_BGL_MAX_FLOW_H
#define GRAL_GB_GRAPH_BGL_MAX_FLOW_H

// $LICENSE_NEC_2005

#include "Container/tuple.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>

namespace GrAL { namespace graph {

 
  /*! \brief Convert directed graph to graph suited for max flow computation

      boost max flow algorithms require a directed graph, augmented with formal reverse edges
      having capacity 0. That is, each edge in the original directed graph is copied to its
      formal reverse, having zero capacity.

   */
  template<class DirectedGraph, class Capacity, class VertexList, class MaxflowGraph, class Orig2CopyMap>
  boost::tuple<typename boost::graph_traits<MaxflowGraph>::vertex_descriptor,
	       typename boost::graph_traits<MaxflowGraph>::vertex_descriptor>
  copy_directed_to_max_flow_graph(DirectedGraph const& origG,
				  Capacity      const& orig_capacity,
				  VertexList    const& source_vertices,
				  VertexList    const& sink_vertices,
				  MaxflowGraph       & copyG,
				  Orig2CopyMap       & orig2copy)
  {
    // TODO: static assert that MaxflowGraph supports parallel edges

    typedef boost::graph_traits<DirectedGraph> ogt;
    typedef boost::graph_traits<MaxflowGraph>  cgt;

    //    std::vector<typename cgt::vertex_descriptor> orig2copy(boost::num_vertices(origG));
    //    typename ogt::template vertex_property_map<typename cgt::vertex_descriptor>::type orig2copy(origG);
    typename ogt::vertex_index_map_type orig_vertex_index(ogt::vertex_index_map(origG));

    // create vertices in copyG 
    typename ogt::vertex_iterator ov, ov_end;
    for(boost::tie(ov, ov_end) = vertices(origG); ov != ov_end; ++ov) {
      orig2copy[*ov] = add_vertex(copyG);
    }
    typename cgt::vertex_descriptor c_source = add_vertex(copyG);
    typename cgt::vertex_descriptor c_sink   = add_vertex(copyG);

    // create edges in copyG
    typename boost::property_map<MaxflowGraph, boost::edge_capacity_t>::type 
      copy_capacity = get(boost::edge_capacity, copyG);

   for(boost::tie(ov, ov_end) = vertices(origG); ov != ov_end; ++ov) {
      typename ogt::out_edge_iterator oe, oe_end;
      for(boost::tie(oe, oe_end) = out_edges(*ov,origG); oe != oe_end; ++oe) {
	bool inserted;
	typename cgt::edge_descriptor ce, ce_rev;
	boost::tie(ce, inserted) =
	  add_edge(orig2copy[source(*oe, origG)],
		   orig2copy[target(*oe, origG)], copyG);
	put(copy_capacity, ce, get(orig_capacity,*oe));
	// this would be used  for an undirected input graph.
	/* 
 	boost::tie(ce_rev, inserted) =
	  boost::add_edge(orig2copy[boost::target(*oe, origG)],
			  orig2copy[boost::source(*oe, origG)],
			  copyG);
	put(copy_capacity, ce_rev, get(orig_capacity,*oe));
	*/
      }
    }

    // create source / sink clusters
    typedef typename VertexList::const_iterator list_iter;
    GrAL::tuple<VertexList const*, 2> ss_list(&source_vertices, &sink_vertices);
    GrAL::tuple<typename cgt::vertex_descriptor, 2> ss_vertex(c_source, c_sink);
    for(int i = 0; i <= 1; ++i) {
      for(list_iter osv_it = ss_list[i]->begin(); osv_it != ss_list[i]->end(); ++osv_it) {
	typename ogt::vertex_descriptor v(*osv_it);
	int sum_cap = 0;
	typename ogt::out_edge_iterator oe, oe_end;      
	for(boost::tie(oe, oe_end) = out_edges(v,origG); oe != oe_end; ++oe) {
	  sum_cap += get(orig_capacity,*oe);
	}
	typename cgt::edge_descriptor ce     = add_edge(ss_vertex[i], orig2copy[v], copyG).first;
	typename cgt::edge_descriptor ce_rev = add_edge(orig2copy[v], ss_vertex[i], copyG).first;
	put(copy_capacity, ce,     sum_cap+1); // capacity of edges from/to source/sink should be
	put(copy_capacity, ce_rev, sum_cap+1); // at least a large as the total capacity of adjacent vertices.
      }
    }

    
    // add formal reverse edges
    typename boost::property_map<MaxflowGraph, boost::edge_reverse_t >::type 
      copy_reverse  = get(boost::edge_reverse,  copyG);
    typedef typename cgt::vertex_descriptor copy_vertex_descriptor;
    std::vector<std::pair<copy_vertex_descriptor,copy_vertex_descriptor> > formal_edges;
    typename cgt::vertex_iterator cv, cv_end;
    for(boost::tie(cv, cv_end) = vertices(copyG); cv != cv_end; ++cv) {
      typename cgt::out_edge_iterator ce, ce_end;
      for(boost::tie(ce, ce_end) = out_edges(*cv,copyG); ce != ce_end; ++ce) {
	formal_edges.push_back(std::make_pair(target(*ce,copyG), source(*ce,copyG)));
      }
    }
    for(int e = 0; e < (int)formal_edges.size();++e) {
      typename cgt::edge_descriptor 
	ce = edge    (formal_edges[e].first, formal_edges[e].second,copyG).first;
      typename cgt::edge_descriptor 
	ce_rev =  add_edge(formal_edges[e].second,formal_edges[e].first, copyG).first;
      put(copy_reverse,  ce_rev, ce);
      put(copy_reverse,  ce,     ce_rev);
      put(copy_capacity, ce_rev, 0);
    }
    return boost::make_tuple(c_source, c_sink);
  }

}} // namespace GrAL { namespace graph {

#endif
