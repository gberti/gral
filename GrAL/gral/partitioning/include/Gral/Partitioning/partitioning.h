#ifndef NMWR_GB_GRID_PARTITION_H
#define NMWR_GB_GRID_PARTITION_H


// $LICENSE

#include <vector>   // STL

// Gral Base
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Iterators/boundary-iterator.h"

// Utility
#include "Utility/pre-post-conditions.h"

//----------------------------------------------------------------
/*! \defgroup partitioningds Data structures for grid partitionings
 */


/*! \brief predicate object for membership in partition p
    \ingroup partitioningds
 */
template<class Ptng>
class in_partition_pred {
public:
  typedef typename Ptng::grid_type      grid_type;
  typedef grid_types<grid_type>         gt;
  typedef typename gt::Cell             Cell;
  typedef typename gt::Facet            Facet;
private:
  int p;
  const Ptng* the_partitioning;
public:
  in_partition_pred() {}
  in_partition_pred(int pp, const Ptng& tp) 
    : p(pp), the_partitioning(&tp) {}
  bool operator()(const Cell& c) const 
    { return (p == the_partitioning->partition(c));}

  bool IsOnBoundary(const Facet& f) const {
    return ( the_partitioning->TheGrid().IsOnBoundary(f) 
	     || (*this)(f.C1()) != (*this)(f.C2()));
  }

  // STL unary function
  typedef Cell argument_type;
  typedef bool result_type;
};


/*! \brief creator function for in_partition_pred 

    \ingroup partitioningds
 */
template<class Ptng>
inline in_partition_pred<Ptng>
InPartition(int p, const Ptng& Pt)
    { return in_partition_pred<Ptng>(p,Pt);}


//----------------------------------------------------------------
//            [1] template<class Grid> class partitioning 
//----------------------------------------------------------------

/*! \brief Data structure for representing cell based partitioning

    \ingroup partitioningds
 */

template<class Grid> 
class partitioning {
public:
  typedef partitioning<Grid>           self;

  typedef Grid                         grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename gt::Cell            Cell;
  typedef typename gt::Facet           Facet;
  typedef typename gt::CellIterator    CellIterator;
  typedef typename gt::FacetOnCellIterator  FacetOnCellIterator;

  typedef enumerated_cell_range<Grid>         cell_range;
  typedef typename cell_range::range_type_ref cell_range_rf;

  typedef in_partition_pred<self>       partition_member_test;
  typedef BoundaryComponentVertexIterator2D<grid_type,partition_member_test>
                                         PartBdVertexIterator;
  typedef BoundaryComponentEdgeIterator2D<grid_type,partition_member_test>
                                         PartBdFacetIterator;
  typedef typename cell_range_rf::CellIterator RangeCellIterator;

private:
  //----------- DATA ---------------
  //  const Grid*                        the_grid; // referenced
  Grid*                        the_grid; // referenced
  std::vector<cell_range>      ranges;
  grid_function<Cell,int>      the_partitions;

public:
  //---------------------- construction -----------------------------

  //  partitioning(const grid_type& g) : the_grid(&g), the_partitions(g) {
  partitioning(grid_type& g) : the_grid(&g), the_partitions(g) {
    for(CellIterator c = TheGrid().FirstCell(); ! c.IsDone(); ++c) {
     the_partitions[*c] = -1;
    }
  }

  int add_partition() {
    ranges.push_back(cell_range(TheGrid())); 
    return (ranges.size() -1);
  }

  void add_cell(int i, const Cell& c) {
    REQUIRE( ((0 <= i ) && i < (int)ranges.size()), "i out of range: " << i << '\n',1);
    ranges[i].append(c);
    the_partitions[c] = i;
  }

  //void read_grid(istream& in);
  void read_partition (std::istream& in);
  void write_partition(std::ostream& out);

  void create_vertex_partitions();

  // handle with care -- leaves class in an inconsistent state!!
  void set_partition(const Cell& c, int p) { the_partitions[c] = p;}

  // re-establish consistent state after several set_partition() calls
  void calculate_ranges();

  // eine Kr"ucke ... falls das Gitter sich "andert (vergr"ossert)
  void update() {
    if ((unsigned)TheGrid().NumOfCells() != the_partitions.size())
      the_partitions.resize(TheGrid().NumOfCells());
  }

  //---------------------- data access  -----------------------------

  const grid_type& TheGrid() const { return *the_grid;}
  grid_type&       TheGrid()       { return *the_grid;}

  cell_range_rf Range(int i) const { 
    REQUIRE( ((0 <= i ) && i < (int)ranges.size()), "i out of range: " << i << '\n',1);
   return(cell_range_rf(ranges[i].range()));
  }

  int partition(const Cell& C) const { return the_partitions(C);}
  int other_partition(const Facet& F, int p) const {
    int q = the_partitions(F.C1());
    return (q != p ? q :  the_partitions(F.C2()));
  }
  unsigned NumOfPartitions() const { return ranges.size();}


  PartBdVertexIterator FirstBdVertex(int p) const {
    return PartBdVertexIterator(find_bd_facet(p), InPartition(p,*this));
  }
  PartBdFacetIterator FirstBdFacet(int p) const {
    return PartBdFacetIterator(find_bd_facet(p), InPartition(p,*this));
  }

private:
  Facet find_bd_facet(int p) const {
    for(RangeCellIterator c = Range(p).FirstCell(); ! c.IsDone(); ++c)
      for(FacetOnCellIterator fc = (*c).FirstFacet(); ! fc.IsDone(); ++fc)
	if(    TheGrid().IsOnBoundary(*fc) 
	    || (partition((*fc).C1()) != partition((*fc).C2())))
	  return (*fc);
    ENSURE(false,"no boundary facet of partition " << p << " found!\n",1);
    return Facet(); // keep compilers quiet
  }
};


//----------------------------------------------------------------
//         
//----------------------------------------------------------------

/*! \brief  Filter for selecting correct cell of a facet

    \ingroup partitioningds
 */

template<class GridPredicate>
class inner_facet_selector {
public:
  typedef typename GridPredicate::grid_type      grid_type;
  typedef grid_types<grid_type>         gt;
  typedef typename gt::Cell             Cell;
  typedef typename gt::Facet            Facet;
private:
  GridPredicate pred;
public:
  inner_facet_selector(const GridPredicate& gp) : pred(gp) {}
  
  // precondition: F is not on boundary
  Cell operator()(const Facet) const 
    { return ( pred(F.C1()) ? F.C1() : F.C2()); }
};

/*! \brief  Creator function for inner_facet_selector

    \ingroup partitioningds
 */
template<class GridPredicate>
inline inner_facet_selector<GridPredicate>
InnerFacet(const GridPredicate& gp) 
{ return inner_facet_selector<GridPredicate>(gp); }



/*! \brief  Mapping Vertices -> Set of partitions 

    \ingroup partitioningds
    
    This data structure gives the set of partitions
    a vertex belongs to (in a cell-based partitioning,
    e.g. partitioning<Grid>).
 */
template<class Partition>
class PartitionsByVertex {
public:
  typedef typename Partition::grid_type grid_type;
  typedef grid_types<grid_type>         gt;
  typedef typename  gt::Vertex          Vertex;

  typedef Partition                     partitioning_type;
private:
  Partition const* the_partitioning;
  grid_function<Vertex, std::vector<int> > vertex_partitions;

public:
  //----------- construction --------------------

  PartitionsByVertex(const Partition& Prtng) 
    : the_partitioning(&Prtng), vertex_partitions(Prtng.TheGrid()) 
    { calculate_vertex_partitions();}

  void calculate_vertex_partitions();


  partitioning_type const& ThePartitioning() const { return *the_partitioning;} 
 
  //------------- iteration ---------------------

  typedef typename std::vector<int>::const_iterator PartitionOfVertexIterator;
 
  PartitionOfVertexIterator begin(Vertex const& v) const 
    { return vertex_partitions(v).begin();}
  PartitionOfVertexIterator end(Vertex const& v) const 
    { return vertex_partitions(v).end();}
};


/*! \brief  Create a partitioning<Grid> from the inverse image of a cell mapping
    \ingroup partitioningds

   Constructs a k-way partitioning from a mapping 
   \f$ f : Cells(P.TheGrid()) \mapsto \{t_1, ... t_k\} \f$ 
     where \f$t_i\f$ are of an arbitrary type T.
*/
template<class Partitioning, class CellMapping>
extern void 
CreatePartitioning(Partitioning& P,      // out
		   const CellMapping& f); // in



/*! \brief Returns the number of partition orphanes

    \ingroup partitioningpostproc

   Returns the number of orphanes, that is, cells that are not
   connected to their partitions via at least one neighbour.
   Routine such as CreateCoarsePartitionGrid depend on a partition
   without orphanes. Unluckily, some partitioning tools seem to create
   partitions with orphanes.

 \note: A partition consisting of only one cell will be NOT be counted
  as orphane.
 \see remove_orphanes
*/

template<class Grid>
extern
int
check_partition_for_orphanes(partitioning<Grid> const& P);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/create-partitioning.C"
#include "Gral/Partitioning/internal/partitioning.C"
#include "Gral/Partitioning/internal/check-partitioning.C"
#endif

#endif
