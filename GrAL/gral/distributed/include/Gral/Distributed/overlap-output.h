#ifndef NMWR_GB_OVERLAP_OUTPUT_H
#define NMWR_GB_OVERLAP_OUTPUT_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/overlap-range-output.h"

namespace GrAL {

//----------------------------------------------------------------
/*! \brief Output adapter for overlap ranges 
    \ingroup overlapds
    
    This class parallels overlap<>.
*/
//----------------------------------------------------------------


template<class CoarseGrid, class FineGrid, 
         template<class U> class OVLP_RANGES = overlap_range_output>
class overlap_output {
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
  // typedef typename gt::Facet         Facet;
  typedef typename gt::Cell          Cell;
  typedef tp<Vertex>                 tpV;
  // typedef tp<Facet>                  tpF;
  typedef tp<Cell>                   tpC;

  // instead of missing template typedef
  template<class U>
  struct  overlap_ranges_t { typedef OVLP_RANGES<U> type;};

  typedef typename overlap_ranges_t<Vertex>::type v_range_type;
  //typedef typename overlap_ranges_t<Facet >::type f_range_type;                          
  typedef typename overlap_ranges_t<Cell  >::type c_range_type;                        
  typedef enumerated_subrange_ref<grid_type>      range_type_ref;

  typedef  partial_grid_function<CoarseCell, v_range_type>  v_neighbour_range_map;
  //  typedef  partial_grid_function<CoarseCell, f_range_type>  f_neighbour_range_map;
  typedef  partial_grid_function<CoarseCell, c_range_type>  c_neighbour_range_map;

  // these iterate only over non-zero ranges

  typedef typename c_neighbour_range_map::ElementIterator CellNbIterator;
  typedef typename v_neighbour_range_map::ElementIterator VertexNbIterator;

  typedef enumerated_cell_range<coarse_grid_type>  coarse_cell_range;
  //! range(NbIterator) = range(CellNbIt) \f$ \cup \f$ range(VertexNbIt)
  typedef typename coarse_cell_range::CellIterator        NbIterator;

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

  // contains all coarse cells with non-empty partial ranges.
  coarse_cell_range   neighbours;

  std::string basenm;

  //---------- END DATA --------

public:
  //----------------- construction -------------------------

  overlap_output() : the_grid(0) {}  

  void  init(coarse_grid_type const& cg,
             fine_grid_type   const& fg,
             std::string      const& basenm_);

  void  calc_dependent_information() {  set_neighbours(); }

  void init_bilateral_vertex_range(CoarseCell const& Nb) {
    neighbour_ranges_v[Nb].init(basenm + "." + as_string(number(Nb)) + ".vertices");
  }
  void init_bilateral_cell_range(CoarseCell const& Nb) {
    neighbour_ranges_c[Nb].init(basenm + "." + as_string(number(Nb)) + ".cells");
  }

private:
  void set_coarse_grid(coarse_grid_type const& cg) {
    neighbour_ranges_v.set_grid(cg);
    neighbour_ranges_c.set_grid(cg);
  }

  void set_fine_grid(grid_type const& fg) {
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

  // FIXME: build real mapping CoarseCells->int!
  int number(CoarseCell const& C) const { return C.handle();}

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
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/overlap-output.C"
#endif


#endif
