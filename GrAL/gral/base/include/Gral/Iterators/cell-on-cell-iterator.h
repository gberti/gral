#ifndef GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_H
#define GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/facet-on-cell-function.h"
#include <map>

/*! \brief A generic CellOnCellIterator 

   This class is a model of $GrAL CellOnCellIterator.
   
   \templateparams
    - \c G: Model of $GrAL CellRange, has types \c Cell and \c FacetOnCellIterator
    - \c NBTABLE: maps GT::FacetOnCellIterator to GT::cell_handle
    - \c GT: grid_types<> for \c G

    \ingroup iterators

   The neighbor table \c nbs (of type NBTABLE) has to be passed to an object \c cc of 
    cell_on_cell_iterator<>. There are several possibilities to do so:
    - via the constructor. This can be done if the context of constructing \cc,
      its type (and the corresponding \c nbs object) are known.  
      This can be only a top-level context, where the concrete types are known.
    - implicitely via a cell  \c c of type \c GT::Cell.
      This means that the original grid and all of its types have to be wrapped into
      a grid-with-nb-table.
    - via a mapping from the original grid to the neighbor table object \c nbs, which
      must be accessible to \c cc. The only way to do so in a transparent way is via a static
      member of cell_on_cell_iterator<>.
      Then we can do the following:
      \code
        grid_t    g;
        nbtable_t nbs;
       // calculate nbs
       // ...

       // extend grid types of grid_t
       typedef grid_types<grid_t> gt;
       struct xgt: public gt { typedef cell_on_cell_iterator<grid_t,nbtable_t> CellOnCellIterator;};
       
       // call generic algorithm
       algorithm_needing_cell_on_cell_adjacencies(g,xgt());
      \endcode

    \see Test in \ref test-cell-on-cell-iterator.C
*/

template<class G, 
	 class NBTABLE = facet_on_cell_function<G,
						typename grid_types<G>::cell_handle>, 
	 class GT      = grid_types<G> >
class cell_on_cell_iterator {
  typedef cell_on_cell_iterator<G,NBTABLE,GT> self;
public:
  typedef G                        grid_type;
  typedef typename GT::Cell        Cell;
  typedef typename GT::Facet       Facet;
  typedef typename GT::cell_handle cell_handle;
  typedef typename GT::FacetOnCellIterator FacetOnCellIterator;
  typedef Cell     value_type;
private:
  NBTABLE   const* nbs;
  Cell                 c;
  FacetOnCellIterator fc;

  //  make it possible to pass nbs not via constructor.
  //  We want to use cell_on_cell_iterator<> like
  //  XGT::Cell c; XGT::CellOnCellIterator(c);
  //  where XGT is GT + cell_on_cell_iterator.
  //  Otherwise, we would have to wrap G+NBTABLE and each type in GT.
  typedef std::map<grid_type const*, NBTABLE const*> context_table;
  static  context_table  ctxt;
public:
  cell_on_cell_iterator() : nbs(0) {}
  cell_on_cell_iterator(Cell const& c_)
    : c(c_), fc(c)
  {
    REQUIRE(ctxt.find(& (c.TheGrid())) != ctxt.end(), 
	    "No neighbor table registered for grid " << & (c.TheGrid()),1);
    nbs = ctxt[& (c.TheGrid())];
    advance_till_valid();
  }
  cell_on_cell_iterator(Cell const& c_, NBTABLE const& nbs_) 
    : nbs(&nbs_), c(c_),  fc(c) 
  { advance_till_valid();}
  
  self& operator++() { ++fc; advance_till_valid(); return (*this);}
  Cell  operator* () const { return Cell(TheGrid(), handle());}
  cell_handle      handle()  const { return const_cast<NBTABLE &>(*nbs)[fc];}
  grid_type const& TheGrid() const { return c.TheGrid();}
  bool             IsDone()  const { return fc.IsDone();}
  Cell      const& TheCell() const { return c;}
  Facet            TheFacet() const { return *fc;}

  static void map_nb_table(grid_type const& g_, NBTABLE const& nbs_) { ctxt[&g_] = &nbs_;}
  static void init        (grid_type const& g_);
  static void remove      (grid_type const& g_);
private:
  bool valid(cell_handle c) const { return TheGrid().valid(c);}
  void advance_till_valid() 
  { 
    while(! fc.IsDone() && ! valid(handle()))
      ++fc;
  } 
};

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Iterators/cell-on-cell-iterator.C"
#endif

#endif
