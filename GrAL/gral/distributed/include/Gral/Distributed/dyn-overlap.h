#ifndef NMWR_GB_DYN_OVERLAP_H
#define NMWR_GB_DYN_OVERLAP_H


// $LICENSE


#include "Gral/Subranges/enumerated-subrange.h"

//----------------------------------------------------------------
/*! \defgroup dynoverlap Dynamic (extensible) overlap ranges
    \ingroup overlapds
   \brief Classes bundling overlap ranges allowing efficient 
   \e extension of ranges
 
   The classes in this module  are typically  used to assemble the 
   overlap ranges before copying them to a form supporting 
   combination of adjacent ranges

   The class dyn_overlap is the dynamic version of overlap, and
   dyn_overlap_ranges is the dynamic version of overlap_ranges..
*/
//----------------------------------------------------------------


//----------------------------------------------------------------
/*!  \brief Provides storage  for ranges of elements of type E 
      and gives and read/write access to them.
     \ingroup dynoverlap

      This class is the dynamic version of overlap_ranges.

      Supported are only atomic (non-composite) ranges:
      <pre>
       private   exposed    shared    copied
      +--------+----------+---------+---------+
      </pre>
 */
//----------------------------------------------------------------

template<class E>
class dyn_overlap_ranges {
public:
  typedef dyn_overlap_ranges<E> self;
  typedef element_traits<E>          et;
  typedef typename et::grid_type     grid_type;
  typedef grid_types<grid_type>      gt;
  typedef typename et::handle_type   element_handle;
  typedef E                          Element;

  //  typedef vector<element_handle>               element_seq;
  typedef enumerated_element_range<E>          element_seq;
  typedef enumerated_element_range<E>          range_type;
  typedef typename element_seq::iterator       e_iterator;
  typedef typename element_seq::const_iterator const_e_iterator;
  typedef element_handle                       value_type;

  //  typedef element_range_ref<E,element_seq>     range_cref;

private:
  //----- DATA -----

  const grid_type*  the_grid;
  element_seq       private_range;
  element_seq       exposed_range;
  element_seq       shared_range;
  element_seq       copied_range;
 
  //  private   exposed    shared    copied
  // ---------+----------+---------+---------+
  // |-     owned       -|-    imported     -|
  //          |-      exported    -|

public:
  //------------------- construction ------------------------

  dyn_overlap_ranges() : the_grid(0) {} // init();}
  dyn_overlap_ranges(const grid_type& g) : the_grid(&g) { init();}

  void set_grid(const grid_type& g) {  the_grid = &g; init();}

  void init() {   
    private_range.set_grid(*the_grid);
    exposed_range.set_grid(*the_grid);
    shared_range .set_grid(*the_grid);
    copied_range .set_grid(*the_grid);
 }



  //------------------ component access  ----------------------

  const grid_type& TheGrid() const { 
    REQUIRE((the_grid != 0), "no grid!\n",1);
    return *the_grid;
  }

  unsigned size() const { return (private_range.size() + exposed_range.size()+
				  shared_range.size() + copied_range.size());}
  //------------------ element ranges  ----------------------

  element_seq const&  privee() const { return private_range;}
  element_seq      &  privee()       { return private_range;}

  element_seq const&  exposed() const { return exposed_range;}
  element_seq      &  exposed()       { return exposed_range;}

  element_seq const&  shared() const  { return shared_range;}
  element_seq      &  shared()        { return shared_range;}

  element_seq const&  copied() const { return copied_range;}
  element_seq      &  copied()       { return copied_range;}

};



//----------------------------------------------------------------
/*! \brief Dynamic (extensible) overlap data structure.
    \ingroup dynoverlap
   
    This class is the dynamic version of overlap,
    better suited for incremental creation of overlap ranges.

    \see module overlapds
 */
//----------------------------------------------------------------


template<class CoarseGrid, class FineGrid>
class dyn_overlap {
public:
  typedef CoarseGrid                   coarse_grid_type;
  typedef grid_types<coarse_grid_type> cgt;
  typedef typename cgt::Cell           CoarseCell;

  typedef FineGrid                   grid_type;
  typedef grid_types<grid_type>      gt;
  typedef typename gt::vertex_handle vertex_handle;
  typedef typename gt::facet_handle  facet_handle;
  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::Vertex        Vertex;
  typedef typename gt::Facet         Facet;
  typedef typename gt::Cell          Cell;
  typedef tp<Vertex>                 tpV;
  typedef tp<Facet>                  tpF;
  typedef tp<Cell>                   tpC;

  typedef dyn_overlap_ranges<Vertex>                         v_range_type;
  typedef dyn_overlap_ranges<Facet>                          f_range_type;
  typedef dyn_overlap_ranges<Cell>                           c_range_type;

  typedef typename dyn_overlap_ranges<Vertex>::range_type   vertex_range_type;
  typedef typename dyn_overlap_ranges<Facet>::range_type    facet_range_type;
  typedef typename dyn_overlap_ranges<Cell>::range_type     cell_range_type;

  typedef  partial_grid_function<CoarseCell, v_range_type>  v_neighbour_range_map;
  typedef  partial_grid_function<CoarseCell, f_range_type>  f_neighbour_range_map;
  typedef  partial_grid_function<CoarseCell, c_range_type>  c_neighbour_range_map;

  // these iterate only over non-zero ranges
  typedef typename v_neighbour_range_map::ElementIterator VertexNbIterator;
  typedef typename f_neighbour_range_map::ElementIterator FacetNbIterator;
  typedef typename c_neighbour_range_map::ElementIterator CellNbIterator;

public:
  typedef dyn_overlap<CoarseGrid, FineGrid> self;

private:
  //----------- DATA ----------

  //----- referenced -------

  const grid_type * the_grid; 

  //-------- owned ---------

  // partial (neighbour) ranges
  v_neighbour_range_map neighbour_ranges_v;
  f_neighbour_range_map neighbour_ranges_f;
  c_neighbour_range_map neighbour_ranges_c;

  // total ranges
  v_range_type    total_ranges_v;
  f_range_type    total_ranges_f;
  c_range_type    total_ranges_c;
 
  //---------- END DATA --------

public:
  //----------------- construction -------------------------

  dyn_overlap(coarse_grid_type  const& cg, grid_type const& fg)
    : the_grid(&fg)
    {
      set_coarse_grid(cg);
      set_fine_grid  (fg);
    }

  dyn_overlap() : the_grid(0) {}  

  void set_coarse_grid(const coarse_grid_type& cg) {
    neighbour_ranges_v.set_grid(cg);
    neighbour_ranges_f.set_grid(cg);
    neighbour_ranges_c.set_grid(cg);
  }

  void set_fine_grid(const grid_type& fg) {
    the_grid = &fg;
    total_ranges_v.set_grid(fg);
    total_ranges_f.set_grid(fg);
    total_ranges_c.set_grid(fg);
    neighbour_ranges_v.set_default(v_range_type(fg));
    neighbour_ranges_f.set_default(f_range_type(fg));
    neighbour_ranges_c.set_default(c_range_type(fg));
  }

  //----------------- component access ---------------------

  coarse_grid_type const& TheCoarseGrid() const { return neighbour_ranges_v.TheGrid();}
  grid_type        const& TheGrid()       const { return *the_grid;}

  //------------------- iteration --------------------------

 
  VertexNbIterator FirstVertexNeighbour() const { return neighbour_ranges_v.FirstElement();}
  VertexNbIterator FirstNeighbour(tpV)    const { return neighbour_ranges_v.FirstElement();}
  FacetNbIterator  FirstFacetNeighbour()  const { return neighbour_ranges_f.FirstElement();}
  FacetNbIterator  FirstNeighbour(tpF)    const { return neighbour_ranges_f.FirstElement();}
  CellNbIterator   FirstCellNeighbour()   const { return neighbour_ranges_c.FirstElement();}
  CellNbIterator   FirstNeighbour(tpC)    const { return neighbour_ranges_c.FirstElement();}
  

  //----------------- range access -------------------------

  // reference
  v_range_type & vertices() { return total_ranges_v;}
  f_range_type & facets()   { return total_ranges_f;}
  c_range_type & cells()    { return total_ranges_c;}
  v_range_type & elements(tpV) { return total_ranges_v;}
  f_range_type & elements(tpF) { return total_ranges_f;}
  c_range_type & elements(tpC) { return total_ranges_c;}

  v_range_type & vertices(const CoarseCell& Nb) { return neighbour_ranges_v[Nb];}
  f_range_type & facets  (const CoarseCell& Nb) { return neighbour_ranges_f[Nb];}
  c_range_type & cells   (const CoarseCell& Nb) { return neighbour_ranges_c[Nb];}
  v_range_type & elements(tpV, const CoarseCell& Nb) { return neighbour_ranges_v[Nb];}
  f_range_type & elements(tpF, const CoarseCell& Nb) { return neighbour_ranges_f[Nb];}
  c_range_type & elements(tpC, const CoarseCell& Nb) { return neighbour_ranges_c[Nb];}

  // const reference
  v_range_type const& vertices() const { return total_ranges_v;}
  f_range_type const& facets  () const { return total_ranges_f;}
  c_range_type const& cells   () const { return total_ranges_c;}
  v_range_type const& elements(tpV) const { return total_ranges_v;}
  f_range_type const& elements(tpF) const { return total_ranges_f;}
  c_range_type const& elements(tpC) const { return total_ranges_c;}

  v_range_type const& vertices(const CoarseCell& Nb) const { return neighbour_ranges_v(Nb);}
  f_range_type const& facets  (const CoarseCell& Nb) const { return neighbour_ranges_f(Nb);}
  c_range_type const& cells   (const CoarseCell& Nb) const { return neighbour_ranges_c(Nb);}
  v_range_type const& elements(tpV, const CoarseCell& Nb) const { return neighbour_ranges_v(Nb);}
  f_range_type const& elements(tpF, const CoarseCell& Nb) const { return neighbour_ranges_f(Nb);}
  c_range_type const& elements(tpC, const CoarseCell& Nb) const { return neighbour_ranges_c(Nb);}

};



#endif
