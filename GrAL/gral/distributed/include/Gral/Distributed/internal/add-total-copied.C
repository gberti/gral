#ifndef NMWR_GB_ADD_TOTAL_COPIED_C
#define NMWR_GB_ADD_TOTAL_COPIED_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Distributed/internal/add-total-copied.h"


#include <iterator.h>  // STL

#include "Container/some-algorithms.h"

#include "Gral/Base/grid-functors.h"
#include "Gral/Distributed/mark-ranges.h"

//----------------------------------------------------------------
//
// Construct the local total overlap ranges from shared_{v,f} and pattern
// as subranges of a global grid.
// Changed are ovrlp.{cells,vertices}.{copied,exposed}
// ovrlp is the overlap structure corresponding to the partition
// p of Prtng for which inside(p) == true.
// output:
//  Cp^P = Cp^P \cup Hull_pat(sh_v, !inside) (outer hull)
//  Ex^P = Ex^P \cup Hull_pat(sh_v,  inside) (inner hull)
//
//----------------------------------------------------------------
#include "compiler-config.h"

template <class Predicate>
class my_unary_negate : 
    public unary_function<typename Predicate::argument_type, bool> {
protected:
    Predicate pred;
public: 
    my_unary_negate() {}
    explicit my_unary_negate(const Predicate& x) : pred(x) {}
    bool operator()(const typename Predicate::argument_type& x) const { return !pred(x); }
};

template <class Predicate>
inline my_unary_negate<Predicate> my_not1(const Predicate& pred) {
  return my_unary_negate<Predicate>(pred);
}

template<class Overlap, class Partition, class InsidePred, class Pat,
         class VertexRange, class FacetRange>
void AddTotalRanges(Overlap          &  ovrlp,   // out
		    Partition   const&  Prtng,   // in
		    InsidePred  const&  inside,  //
		    Pat         const&  pattern,
		    VertexRange const&  shared_v,
		    FacetRange  const&  shared_f)
{

  //  typename Part2Cell::value_type  P(Prtng.PartCell(p));
  
  //----  (I) cell ranges --------

  mark_layers(shared_f, ovrlp.cells().copied() , pattern, my_not1(inside));
  mark_layers(shared_f, ovrlp.cells().exposed(), pattern,      inside );

  //---- (II) vertex ranges ------
  
  // generate exposed and copied vertices as adjacency hull of
  // exposed and copied cells , minus shared vertices:
  // Ex_V = V(Ex_C) - Sh_V
  // Cp_V = V(Cp_C) - Sh_V  

  typedef typename VertexRange::Element  Vertex;
  partial_grid_function<Vertex,bool> not_shared(Prtng.TheGrid(), true);
  range_assign(not_shared,shared_v.FirstElement(), false);
  // ==  is_shared[ shared_v[pt] ] = true;
  

  copy_it_range_if_filter(VerticesOfCells(ovrlp.cells().copied()).FirstVertex(), 
			  back_inserter(ovrlp.vertices().copied().TheContainer()), 
			  not_shared, 
			  vertex2handle(Prtng.TheGrid()));
  copy_it_range_if_filter(VerticesOfCells(ovrlp.cells().exposed()).FirstVertex(),
			  back_inserter(ovrlp.vertices().exposed().TheContainer()), 
			  not_shared, 
			  vertex2handle(Prtng.TheGrid()));
  
}

#endif
