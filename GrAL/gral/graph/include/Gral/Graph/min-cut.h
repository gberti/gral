#ifndef GRAL_GB_GRAPH_MIN_CUT_H
#define GRAL_GB_GRAPH_MIN_CUT_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Graph/BGL/cell-neighbor-graph.h"
#include "Gral/Graph/BGL/max-flow.h"
#include "Gral/Iterators/cell-on-cell-iterator.h"
#include "Gral/Base/extend-grid-types.h"

#include "Gral/Views/restricted-grid-view.h"
#include "Gral/Views/restricted-component-view.h"

#include "Container/bijective-mapping.h"
#include "Container/function-adapter.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>

namespace GrAL { namespace graph {
 
  template<class GRAPH, class CAP>
  void print_flow_graph(GRAPH const& G, CAP const& capacity, std::ostream& out)
  {
    typedef boost::graph_traits<GRAPH> gt;
    typename gt::vertex_iterator v,vend;
    for(boost::tie(v,vend) = boost::vertices(G); v != vend; ++v) {
      typename gt::out_edge_iterator e,eend;
      for(boost::tie(e,eend) = boost::out_edges(*v,G); e != eend; ++e) { 
	out << boost::source(*e,G) << " -> " << boost::target(*e,G) << " c=" << capacity[*e] << "\n";
      }
    }
  }

  /*! \brief Compute min-cut / max-flow based binary partitioning of grid
   
     The cut size is returned.       
   */
  template<class GRID,  class CELLRANGE, class WEIGHTGF, class PARTGF>
  typename WEIGHTGF::value_type
  min_cut_binary_partition(GRID        const& G,
			   CELLRANGE   const& seedsA,
			   CELLRANGE   const& seedsB,
			   WEIGHTGF    const& cutweights,
			   PARTGF           & partition,
			   int                partA = 0,
			   int                partB = 1)
  {
    // transform Grid into Graph
    typedef GrAL::cell_on_cell_iterator<GRID> CellOnCellIterator;
    CellOnCellIterator::init(G);
    typedef GrAL::grid_types<GRID> origgt;
    typedef GrAL::xgt<origgt, override_CellOnCellIterator<CellOnCellIterator> > grid_t;
    typedef GrAL::graph::graph_view<GRID, GrAL::graph::cell_neighbor_graph_tag, GrAL::graph::directed_graph_tag, grid_t>
      graph_type;
    typedef boost::graph_traits<graph_type> graph_t;

    graph_type graphG(G);
    //TODO:  need  grid function -> property map *view*
    typename graph_t::template edge_property_map<int>::type capacity(graphG);
    {
      typename graph_t::vertex_iterator v,vend;
      for(boost::tie(v,vend) = vertices(graphG); v != vend; ++v) {
	typename graph_t::out_edge_iterator e,eend;
	for(boost::tie(e,eend) = out_edges(*v,graphG); e != eend; ++e) {
	  capacity[*e] = cutweights(graphG.facet(*e));
	  // std::cout <<  boost::source(*e,graphG) << " -> " << boost::target(*e,graphG) << ": "
	  //	    << capacity[*e] << "=" <<  cutweights(graphG.facet(*e)) << "\n";
	}
      }
    }

    //--- build Maxflow graph
    typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> adjt;
    typedef boost::property<boost::vertex_index_t, int>                            vertex_property_type;
    typedef boost::property<boost::edge_reverse_t, typename adjt::edge_descriptor> ep_rev;
    typedef boost::property<boost::edge_capacity_t,int, ep_rev>                    ep_cap_rev;
    typedef boost::property<boost::edge_residual_capacity_t, int, ep_cap_rev>      edge_property_type;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, vertex_property_type, edge_property_type>    
      flow_graph_type;
    typedef boost::graph_traits<flow_graph_type> fgt;

    flow_graph_type FG;
    typename graph_t::template vertex_property_map<typename fgt::vertex_descriptor>::type orig2copy(graphG);

    typename fgt::vertex_descriptor flow_source, flow_sink;
    boost::tie(flow_source, flow_sink) = copy_directed_to_max_flow_graph(graphG,capacity, seedsA, seedsB, FG, orig2copy);

    //--- compute flow & partition
    typedef typename boost::property_map<flow_graph_type, boost::edge_capacity_t>         ::type flow_capacity_map;
    typedef typename boost::property_map<flow_graph_type, boost::edge_residual_capacity_t>::type flow_res_cap_map;
    typedef typename boost::property_map<flow_graph_type, boost::edge_reverse_t >         ::type flow_edge_rev_map;
    typedef typename boost::property_map<flow_graph_type, boost::vertex_index_t >         ::type flow_vertex_index_map;
 
    flow_capacity_map      flow_capacity     = boost::get(boost::edge_capacity, FG);
    flow_res_cap_map       flow_res_capacity = boost::get(boost::edge_residual_capacity, FG);
    flow_edge_rev_map      flow_reverse      = boost::get(boost::edge_reverse,  FG);
    flow_vertex_index_map  flow_vertex_index = boost::get(boost::vertex_index,  FG);

    // print_flow_graph(FG, flow_capacity, std::cout);


    boost::detail::push_relabel<flow_graph_type, flow_capacity_map,flow_res_cap_map,flow_edge_rev_map,flow_vertex_index_map, int>
      max_flow_algo(FG,flow_capacity, flow_res_capacity, flow_reverse, flow_source, flow_sink, flow_vertex_index);
    typename WEIGHTGF::value_type flow = max_flow_algo.maximum_preflow();
    // std::cout << "Total flow: " << flow << std::endl;
    max_flow_algo.convert_preflow_to_flow();

    // max_flow_algo.print_flow_values(std::cout);

    {
      typename fgt::vertex_iterator v, vend;
      bool vertices_added = false;
      do { 
	vertices_added = false;
	for(boost::tie(v,vend) = boost::vertices(FG); v != vend; ++v) {
	  if(max_flow_algo.color[*v] > 0) {
	    typename fgt::out_edge_iterator e,eend;
	    for(boost::tie(e,eend) = boost::out_edges(*v,FG); e != eend; ++e)
	      // flow_capacity[*e] > 0 && flow_capacity[*e]  - flow_res_capacity[*e] > 0)
	      if(max_flow_algo.color[boost::target(*e,FG)] == 0 &&  !max_flow_algo.is_saturated(*e)) {
		max_flow_algo.color[boost::target(*e,FG)] = max_flow_algo.color[*v];
		vertices_added = true;
	      }
	  }
	}
      } while(vertices_added);
    }
    //--- copy max_flow_algo.color to partition
    // FIXME: Use correspondence between graphG and FG
    bijective_mapping<int,int> color_map;
    color_map[max_flow_algo.color[flow_source]] = partA;
    color_map[max_flow_algo.color[flow_sink  ]] = partB;
    std::vector<typename graph_t::vertex_descriptor> copy2orig(boost::num_vertices(FG));
    {
      typename graph_t::vertex_iterator v, vend;
      for(boost::tie(v,vend) = vertices(graphG); v != vend; ++v)
	copy2orig[flow_vertex_index[orig2copy[*v]]] = *v;
    }
    {
      typename fgt::vertex_iterator v, vend;
      for(boost::tie(v,vend) = boost::vertices(FG); v != vend; ++v) 
	if(*v != flow_source && *v != flow_sink)
	  // partition[typename grid_t::Cell(G, flow_vertex_index(*v))] = color_map(max_flow_algo.color[*v]);
	  partition[copy2orig[flow_vertex_index(*v)].Base()] = color_map(max_flow_algo.color[*v]);
    }

    return flow;
  }






  /*! \brief Compute min-cut / max-flow based k-way partitioning of grid
   
     The accumulated cut size is returned.       
   */

  template<class GRID,  class CELLRANGE_SET, class WEIGHTGF, class PARTGF>
  typename WEIGHTGF::value_type
  min_cut_kway_partition(GRID          const& G,
			 CELLRANGE_SET const& seed_sets,
			 WEIGHTGF      const& cutweights,
			 PARTGF             & partition)
  {
    typedef GRID                                           grid_t;
    typedef grid_types<grid_t>                             gt;
    // typedef enumerated_cell_range<grid_t>                  cell_range_t;
    typedef std::vector<typename gt::Cell>                 cell_range_t;
    typedef typename CELLRANGE_SET::value_type             input_cell_range_t;
    typedef grid_types<typename CELLRANGE_SET::value_type> rgt;
    int current_seed_set = 0;

    // set partition to 0 for cell reachable from seeds, = -1 for others.

    for(typename gt::CellIterator c(G); !c.IsDone(); ++c)
      partition[*c] = -1;

    typedef constant<typename gt::Cell, bool> inside_t;
    typedef restricted_grid_component_view::grid_view<grid_t, inside_t> comp_view_t;
    typedef grid_types<comp_view_t>           compgt;

    //    cell_range_t all_seeds(G);
    cell_range_t all_seeds;
    for(int s = 0; s < (int)seed_sets.size(); ++s)
      for(typename input_cell_range_t::const_iterator c(seed_sets[s].begin()); c != seed_sets[s].end(); ++c)
	all_seeds.push_back(*c);

    comp_view_t comp(G, inside_t(true), all_seeds.begin(), all_seeds.end());
    for(typename compgt::CellIterator c(comp); !c.IsDone(); ++c)
      partition[*c] = current_seed_set;


    // iteratively call binary partitioning

    int cut_sum = 0;
    int num_seed_sets = seed_sets.size();
    while(current_seed_set < num_seed_sets-1) {
      namespace rgv =                                       restricted_grid_view;
      typedef   map_is_equal<PARTGF, typename gt::Cell>     restriction_pred_t;
      typedef   rgv::grid_view<rgv::cfg<gt,restriction_pred_t> >   grid_view_t;
      typedef   grid_types<grid_view_t>                     restgt;
      typedef   std::vector<typename restgt::Cell>          rest_cell_range_t;
      grid_view_t  RestG (G, restriction_pred_t(partition,current_seed_set));
      rest_cell_range_t seedsA; //(G); // = seed_sets[current_seed_set];
      rest_cell_range_t seedsB; //(G); // = seed_sets[current_seed_set+1, ..., num_seed_sets-1]
      for(typename input_cell_range_t::const_iterator c = seed_sets[current_seed_set].begin(); 
	  c != seed_sets[current_seed_set].end(); ++c)
	seedsA.push_back(typename restgt::Cell(RestG,*c));
      for(int s = current_seed_set+1; s < num_seed_sets; ++s)
	for(typename input_cell_range_t::const_iterator c(seed_sets[s].begin()); c != seed_sets[s].end(); ++c)
	   seedsB.push_back(typename restgt::Cell(RestG,*c));

      cut_sum += min_cut_binary_partition(RestG, 
					  seedsA, seedsB, 
					  cutweights,
					  partition, current_seed_set, current_seed_set+1);
   
      ++current_seed_set;
    }

    return cut_sum;
  }


}}


#endif
