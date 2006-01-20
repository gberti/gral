#ifndef NMWR_GB_OVERLAP_H
#define NMWR_GB_OVERLAP_H


// $LICENSE

#include "Gral/Distributed/overlap-ranges.h"
#include "Gral/Subranges/enumerated-subrange-ref.h"

#include <iostream>

namespace GrAL {

//----------------------------------------------------------------
/*! \defgroup overlapds  Grid overlap ranges
    \ingroup distributedgrids
  \brief Overlap ranges for grids and corresponding functions
  \ingroup distributedgrids

 classes allowing economic storage of layered overlap ranges
 allows efficient combination of adjacent ranges
 like exported = exposed + shared
      imported = shared  + copied

*/
//----------------------------------------------------------------


//----------------------------------------------------------------
/*! \brief Overlap ranges with economic storage
    \ingroup overlapds
    
 An object of class overlap is tied to a local grid (\c FineGrid) 
 and a quotient grid (\c CoarseGrid).
 It gives access to total as well as bilateral overlap ranges
 of type overlap_ranges<E>, where \c E is an element type of \c FineGrid.

 Therefore, one can access (adjacent) overlap ranges,
 like exported = exposed \f$ \cup \f$ shared
  or  imported = shared  \f$ \cup \f$ copied.

 \see overlap_ranges, dyn_overlap 
 \todo Add facet ranges.
*/
//----------------------------------------------------------------


template<class CoarseGrid, class FineGrid, 
         template<class U> class OVLP_RANGES = overlap_ranges>
class overlap {
public:
  typedef CoarseGrid                   coarse_grid_type;
  typedef grid_types<coarse_grid_type> cgt;
  typedef typename cgt::Cell           CoarseCell;

  typedef FineGrid                   grid_type;
  typedef FineGrid                   fine_grid_type;
  typedef grid_types<grid_type>      gt;
  //  typedef typename gt::vertex_handle vertex_handle;
  //  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Vertex        Vertex;
  typedef typename gt::Facet         Facet;
  typedef typename gt::Cell          Cell;
  typedef tp<Vertex>                 tpV;
  typedef tp<Facet>                  tpF;
  typedef tp<Cell>                   tpC;

  // instead of missing template typedef
  template<class U>
  struct  overlap_ranges_t { typedef OVLP_RANGES<U> type;};

  typedef typename overlap_ranges_t<Vertex>::type v_range_type;
  typedef typename overlap_ranges_t<Facet >::type f_range_type;                          
  typedef typename overlap_ranges_t<Cell  >::type c_range_type;                        
  typedef enumerated_subrange_ref<grid_type>      range_type_ref;

  //  typedef typename v_range_type::range_ref  vertex_range_type_ref;
  //  typedef typename c_range_type::range_ref  cell_range_type_ref;

  typedef  partial_grid_function<CoarseCell, v_range_type>  v_neighbour_range_map;
  typedef  partial_grid_function<CoarseCell, f_range_type>  f_neighbour_range_map;
  typedef  partial_grid_function<CoarseCell, c_range_type>  c_neighbour_range_map;

  // these iterate only over non-zero ranges
  typedef enumerated_cell_range<coarse_grid_type>  coarse_cell_range;


  typedef typename c_neighbour_range_map::ElementIterator CellNbIterator;
  typedef typename v_neighbour_range_map::ElementIterator VertexNbIterator;
  // range(NbIterator) = range(CellNbIt) \cup range(VertexNbIt)
  typedef typename coarse_cell_range::CellIterator        NbIterator;

  //typedef ExtendedCellOnCellIterator<coarse_grid_type>  CellNbIterator;
  //typedef ExtendedCellOnCellIterator<coarse_grid_type>  VertexNbIterator;

  // must know my coarse cell for this!
  //typedef ExtendedCellOnCellIterator<coarse_grid_type>  NbIterator;
private:
  //----------- DATA ----------

  //----- referenced -------

  const grid_type * the_grid; 

  //-------- owned ---------

  // partial (neighbour) ranges
  // contain reference to coarse grid.
  v_neighbour_range_map neighbour_ranges_v;
  c_neighbour_range_map neighbour_ranges_c;

  // total ranges
  v_range_type    total_ranges_v;
  c_range_type    total_ranges_c;

  // contains all coarse cell with no-empty partial ranges.
  coarse_cell_range   neighbours;

  //---------- END DATA --------

public:
  //----------------- construction -------------------------

  overlap() : the_grid(0) {}  

  void  init(coarse_grid_type const& cg,
             fine_grid_type   const& fg)
  {
    set_coarse_grid(cg);
    set_fine_grid  (fg);
  }

  void init_bilateral_vertex_range(CoarseCell const& Nb) {}
  void init_bilateral_cell_range  (CoarseCell const& Nb) {}

  void calc_dependent_information() { set_neighbours();}

private:

  void set_coarse_grid(const coarse_grid_type& cg) {
    neighbour_ranges_v.set_grid(cg);
    neighbour_ranges_c.set_grid(cg);
  }

  void set_fine_grid(const grid_type& fg) {
    the_grid = &fg;
    total_ranges_v.set_grid(fg);
    total_ranges_c.set_grid(fg);
    neighbour_ranges_v.set_default(v_range_type(fg));
    neighbour_ranges_c.set_default(c_range_type(fg));
  }

  void set_neighbours();

public:

  //----------------- component access ---------------------

  coarse_grid_type const& TheCoarseGrid() const { return neighbour_ranges_v.TheGrid();}
  grid_type        const& TheGrid()       const { return *the_grid;}

  //------------------- iteration --------------------------

 
  CellNbIterator   FirstCellNeighbour()   const { return neighbour_ranges_c.FirstElement();}
  CellNbIterator   FirstNeighbour(tpC)    const { return neighbour_ranges_c.FirstElement();}
  VertexNbIterator FirstVertexNeighbour() const { return neighbour_ranges_v.FirstElement();}
  VertexNbIterator FirstNeighbour(tpV)    const { return neighbour_ranges_v.FirstElement();}

  NbIterator      FirstNeighbour()        const { return neighbours.FirstCell();}  

  //----------------- range access -------------------------

  v_range_type & vertices()    { return total_ranges_v;}
  c_range_type & cells   ()    { return total_ranges_c;}
  v_range_type & elements(tpV) { return total_ranges_v;}
  c_range_type & elements(tpC) { return total_ranges_c;}

  v_range_type & vertices(     const CoarseCell& Nb) { return neighbour_ranges_v[Nb];}
  c_range_type & cells   (     const CoarseCell& Nb) { return neighbour_ranges_c[Nb];}
  v_range_type & elements(tpV, const CoarseCell& Nb) { return neighbour_ranges_v[Nb];}
  c_range_type & elements(tpC, const CoarseCell& Nb) { return neighbour_ranges_c[Nb];}

  v_range_type const& vertices()    const { return total_ranges_v;}
  c_range_type const& cells()       const { return total_ranges_c;}
  v_range_type const& elements(tpV) const { return total_ranges_v;}
  c_range_type const& elements(tpC) const { return total_ranges_c;}

  v_range_type const& vertices(     const CoarseCell& Nb) const { return neighbour_ranges_v(Nb);}
  c_range_type const& cells   (     const CoarseCell& Nb) const { return neighbour_ranges_c(Nb);}
  v_range_type const& elements(tpV, const CoarseCell& Nb) const { return neighbour_ranges_v(Nb);}
  c_range_type const& elements(tpC, const CoarseCell& Nb) const { return neighbour_ranges_c(Nb);}

  // local = owned cells , owned+shared vertices.
  range_type_ref  local_range()  const { return range_type_ref(vertices().local(),cells().owned());}

  // these are not "correct" ranges in the sense that the vertex set of cells is
  // a strict superset of the vertices!
  //  range_type_ref  owned()  const { return range_type_ref(vertices().owned(),cells.owned());}
  //  range_type_ref  privee() const { return range_type_ref(vertices().privee(),cells.privee());}
  //  range_type_ref  exported() const { return range_type_ref(vertices().exported(),cells.exported());}
};


/*! Pretty-print an overlap
   \ingroup overlapds
 */
template<class Overlap>
void write_ovlp(Overlap const& ovlp, std::ostream & out);

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/overlap.C"
#endif


#endif
