#ifndef NMWR_GB_COARSE_FROM_PARTITIONING_C
#define NMWR_GB_COARSE_FROM_PARTITIONING_C


// $LICENSE

#include "Gral/Partitioning/coarse-grid-from-partitioning.h"
#include "Gral/Partitioning/collect-element-partitions.h"

#include "Gral/IO/stream-grid-adapter.h"

#include "Gral/Base/construct-grid.h"


//----------------------------------------------------------------
//  
//  Construct a coarse grid from a partitioning of a fine grid.
//  The cells of the coarse grid correspond to the partitions of
//  the fine grid. This work only for 2D grids (see below).
//
//  Algorithm:
//  ----------
//  (1) for all vertices on partition boundaries: collect all
//      adjacent partitions.
//  (2) for all partitions p: scan all vertices v on the  boundary
//      of p, and see if it is a vertex of the coarse grid.
//      - in 2D, v is a vertex of p <=> v has >= 3 adjacent partitions
//        (including the outside of the underlying fine grid)
//      - in 3D, we will have to adapt this test:
//         First identify the facets of the partition, then the edges
//         and finally the vertices.
//
//----------------------------------------------------------------


template<class T>
class vector_map : public std::vector<T>
{
  typedef std::vector<T> base;
public:
  vector_map() {}
  vector_map(const std::vector<T>& v) : base(v) {}
  vector_map(size_t sz)          : base(sz) {}
  vector_map(size_t sz, const T& t) : base(sz,t) {}

  typedef int argument_type;
  typedef T   result_type;
  const T& operator()(int i) const { return *(begin()+i);}
};


template<class CoarseGrid, class Partition, 
         class CoarseToFineVertex, class CoarseCellToPart>
void ConstructCoarsePartitionGrid(CoarseGrid& G,      // out
				  const Partition& P, // in
				  CoarseToFineVertex& coarse2fine_v,   // out
				  CoarseCellToPart  & coarsecell2part) // out
{
  typedef typename Partition::PartBdVertexIterator PartBdVertexIterator;
  typedef typename Partition::PartBdFacetIterator  PartBdFacetIterator;
  typedef typename Partition::grid_type   grid_type;
  typedef grid_types<grid_type>           gt;
  typedef typename gt::Vertex             Vertex;
  //  typedef typename gt::BoundaryFacetIterator  BdFacetIterator;

  partial_grid_function<Vertex, std::vector<int> > partitions_of_vertex(P.TheGrid());

  // fine vertices that correspond to vertices of coarse grid
  std::vector<Vertex>                     coarse_vertices;
  // the inverse function
  partial_grid_function<Vertex,int>  coarse_vertex_num(P.TheGrid());

  // numbers of coarse vertices of partitions 
  // {0 ... npart-1} --> P{ 0 ... nvert-1}
  std::vector< std::vector<int> > local_vertices(P.NumOfPartitions());

  //-----  collect adjacent partitions for all vertices on part. bd. ----

  collect_vertex_partitions(P,partitions_of_vertex);
  
  //-------- collect local vertices of all partitions ------------------

  // a vertex of the partition grid is caracterized by the fact that
  // it is adjacent to at least d+1 partitions, where d is the (intrinsic)
  // dimension of the grid. Here also the complement of the grid counts 
  // as partition. (not true in 3D!)
  // Note: the collection of the vertices relies on an ordered transversal
  // through the boundary of a partition. This is not possible in 3D, where
  // more information has to be gathered!

  for(int pt = 0; pt < (int)P.NumOfPartitions(); ++pt) {
    for(PartBdVertexIterator pbv = P.FirstBdVertex(pt); ! pbv.IsDone(); ++pbv)
      if( partitions_of_vertex(*pbv).size() > 2) {
	if (! coarse_vertex_num.defined(*pbv)) {
	   coarse_vertices.push_back(*pbv);
	   coarse_vertex_num[*pbv] = coarse_vertices.size() -1;
	}
	local_vertices[pt].push_back(coarse_vertex_num[*pbv]);
      }
  }

  //---- flatten local vertex list ----------

  typedef std::vector< std::vector<int> >::const_iterator vvi_iter;
  typedef std::vector<int>::const_iterator vi_iter;
  std::vector<int> vertex_list;
  vertex_list.reserve(P.NumOfPartitions() * 5); // heuristical
  for(vvi_iter v = local_vertices.begin(); v != local_vertices.end(); ++v) {
    vertex_list.push_back((*v).size());
    for (vi_iter lv = (*v).begin(); lv != (*v).end(); ++lv)
      vertex_list.push_back(*lv);
  }


  //  copy(vertex_list.begin(),vertex_list.end(), ostream_iterator<int>(cerr," "));
 
  //--- construct coarse grid from vertex_list
  typedef grid_types<CoarseGrid>  cgt;
  typedef typename cgt::vertex_handle coarse_vertex_handle;
  typedef typename cgt::cell_handle   coarse_cell_handle;

  vector_map<coarse_vertex_handle>  CoarseVertexCorr(coarse_vertices.size());
  vector_map<coarse_cell_handle>    CoarseCellCorr(P.NumOfPartitions());
  ConstructGrid0(G,                                        // out
		 StreamGridMask((int)coarse_vertices.size(),    // in
				(int)P.NumOfPartitions(),
				vertex_list.begin()),
		 CoarseVertexCorr,
		 CoarseCellCorr);

  for(unsigned v0 = 0; v0 < coarse_vertices.size(); ++v0)
    coarse2fine_v[CoarseVertexCorr[v0]] = coarse_vertices[v0].handle();  
  
  for(unsigned c = 0; c < P.NumOfPartitions(); ++c)
    coarsecell2part[CoarseCellCorr[c]] = c;

}

#endif
