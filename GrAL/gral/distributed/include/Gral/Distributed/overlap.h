#ifndef NMWR_GB_OVERLAP_H
#define NMWR_GB_OVERLAP_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Distributed/range-layers.h"

//----------------------------------------------------------------
// class overlap<CoarseG,FineG>;
//
// classes allowing economic storage of layered overlap ranges
// allows efficient combination of adjacent ranges
// like exported = exposed + shared
//      imported = shared  + copied
//
// CONTENTS:
// ---------
//   [1] template<class CoarseGrid, class FineGrid> class overlap;
//
//   [2a] template<class SrcRange, class DestRange, class Filter>
//        void copy_overlap_ranges(const SrcRange& prv,   // in
//                                 const SrcRange& exp,   // in
//                                 const SrcRange& shd,   // in
//                                 const SrcRange& cop,   // in
//                                 DestRange& dest,       // out
//                                 const Filter& f);      // in
//   [2b] template<class SrcRange, class DestRange, class Filter>
//        void copy_overlap_ranges(const SrcRange& exp,   // in
//                                 const SrcRange& shd,   // in
//                                 const SrcRange& cop,   // in
//                                 DestRange& dest,       // out
//                                 const Filter& f);      // in
//   [3] template<CG1, FG1,  Ovlp2, 
//                CoarseCCorr, FineVCorr, FineCCorr>
//       void CopyOverlap(overlap<CG1,FG1>       &  dest,            // out
//              	  Ovlp2             const&  src,             // in
//		          CG1               const&  cg_dest,         // in
//		          CoarseCCorr       const&  crs_src2dest_c,  // in 
//   		          FG1               const&  fg_dest,         // in
//		          FineVCorr         const&  src2dest_v,      // in 
//                        FineCCorr         const&  src2dest_c);     // in
//
//   [4] template<class Overlap>
//       void write_ovlp(Overlap const& ovlp, ostream & out);
//   
//
// DESCRIPTION:
// ------------
//  [1] bundles range_layers for different types of E and gives
//      access to complete as well as per-neighbor ranges
//
//  [2a-b] a simple helper function to set layered ranges from
//      different source ranges.
//
//  [3] generic copy operations for overlap structures.
//      dest = src modulo the corr. mappings 
//  [4] print an overlap structure to a stream
//
//----------------------------------------------------------------


//----------------------------------------------------------------
//               [1] overlap<CoarseGrid,FineGrid>
//----------------------------------------------------------------


template<class CoarseGrid, class FineGrid>
class overlap {
public:
  typedef CoarseGrid                   coarse_grid_type;
  typedef grid_types<coarse_grid_type> cgt;
  typedef typename cgt::Cell           CoarseCell;

  typedef FineGrid                   grid_type;
  typedef grid_types<grid_type>      gt;
  //  typedef typename gt::vertex_handle vertex_handle;
  //  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Vertex        Vertex;
  typedef typename gt::Facet         Facet;
  typedef typename gt::Cell          Cell;
  typedef tp<Vertex>                 tpV;
  typedef tp<Facet>                  tpF;
  typedef tp<Cell>                   tpC;

  typedef range_layers<Vertex>                         v_range_type;
  typedef range_layers<Facet>                          f_range_type;
  typedef range_layers<Cell>                           c_range_type;
  typedef enumerated_subrange_ref<grid_type>           range_type_ref;

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

  v_range_type & vertices() { return total_ranges_v;}
  c_range_type & cells()    { return total_ranges_c;}
  v_range_type & elements(tpV) { return total_ranges_v;}
  c_range_type & elements(tpC) { return total_ranges_c;}

  v_range_type & vertices(const CoarseCell& Nb) { return neighbour_ranges_v[Nb];}
  c_range_type & cells   (const CoarseCell& Nb) { return neighbour_ranges_c[Nb];}
  v_range_type & elements(tpV, const CoarseCell& Nb) { return neighbour_ranges_v[Nb];}
  c_range_type & elements(tpC, const CoarseCell& Nb) { return neighbour_ranges_c[Nb];}

  v_range_type const& vertices() const { return total_ranges_v;}
  c_range_type const& cells() const    { return total_ranges_c;}
  v_range_type const& elements(tpV) const { return total_ranges_v;}
  c_range_type const& elements(tpC) const { return total_ranges_c;}

  v_range_type const& vertices(const CoarseCell& Nb) const { return neighbour_ranges_v(Nb);}
  c_range_type const& cells   (const CoarseCell& Nb) const { return neighbour_ranges_c(Nb);}
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



//----------------------------------------------------------------
//          [2]  copy_overlap_ranges(...)
//----------------------------------------------------------------



template<class SrcRange, class DestRange, class Filter>
void copy_overlap_ranges(const SrcRange& priv, const SrcRange& exp,
			 const SrcRange& shd,  const SrcRange& cop,
			 DestRange& dest,const Filter& f);

template<class SrcRange, class DestRange, class Filter>
void copy_overlap_ranges(                      const SrcRange& exp,
			 const SrcRange& shd,  const SrcRange& cop,
			 DestRange& dest,const Filter& f);

//template<class Ovlp1, class Ovlp2, class FineVCorr, class FineCCorr>
//template<class CG1, class CG2, class FG1, class FG2, class CoarseCCorr, class FineVCorr, class FineCCorr>
template<class CG1, class FG1, class Ovlp2, class CoarseCCorr, class FineVCorr, class FineCCorr>
void CopyOverlap(overlap<CG1,FG1>       &  dest,            // out
		 Ovlp2             const&  src,             // in
		 CG1               const&  cg_dest,         // in
		 CoarseCCorr       const&  crs_src2dest_c,  // in 
		 FG1               const&  fg_dest,         // in
		 FineVCorr         const&  src2dest_v,      // in 
		 FineCCorr         const&  src2dest_c);     // in




template<class Overlap>
void write_ovlp(Overlap const& ovlp, ostream & out);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Distributed/generic/overlap.C"
#endif


#endif
