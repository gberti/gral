#ifndef GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_H
#define GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/facet-on-cell-function.h"
#include "Utility/ref-ptr.h"

#include <map>

namespace GrAL {

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
  typedef Cell     anchor_type;
  typedef typename get_incidence_iterator_category<typename category<value_type> ::type,
						   typename category<anchor_type>::type>::type
		   category;
private:
  ref_ptr<NBTABLE   const> nbs;
  Cell                 c;
  FacetOnCellIterator fc;

  //  make it possible to pass nbs not via constructor.
  //  We want to use cell_on_cell_iterator<> like
  //  XGT::Cell c; XGT::CellOnCellIterator(c);
  //  where XGT is GT + cell_on_cell_iterator.
  //  Otherwise, we would have to wrap G+NBTABLE and each type in GT.
  typedef std::map<grid_type const*, ref_ptr<NBTABLE const> > context_table;
  static  context_table  * ctxt;
public:
  cell_on_cell_iterator() : nbs(0) {}
  cell_on_cell_iterator(Cell const& c_)
    : c(c_), fc(c)
  {
    init_table();
    advance_till_valid();
  }
  cell_on_cell_iterator(Cell const& c_, NBTABLE const& nbs_) 
    : nbs(&nbs_), c(c_),  fc(c) 
  { advance_till_valid();}

  /*! \brief Position iterator at first facet
      
      There is not necessarily a valid neighbor at this position.
      This constructor is useful when iteration over all facets with
      access to valid neighbors is required.
      \example
      \code
      for(CellOnCellIterator cc(c.FirstFacet()); !cc.IsDone(); cc.next_facet()) {
         if(cc.valid_neighbor())
           // handle inner facet
           Cell n = *cc;
         else 
          // handle boundary facet
      }
      \endcode
   */
  explicit cell_on_cell_iterator(FacetOnCellIterator const& fc_) : c(fc_.TheAnchor()), fc(fc_) 
  {
    init_table();
    // not necessary valid_neigbhor().
  }
  void init_table() {
    REQUIRE(ctxt->find(& (c.TheGrid())) != ctxt->end(), 
	    "No neighbor table registered for grid " << & (c.TheGrid()),1);
    nbs = (*ctxt)[& (c.TheGrid())];
  }

  //! move to next valid position
  self& operator++() { ++fc; advance_till_valid(); return (*this);}
  Cell  operator* () const { cv(); return Cell(TheGrid(), handle());}
  cell_handle      handle()  const { return const_cast<NBTABLE &>(*nbs)[fc];}
  bool             IsDone()  const { return fc.IsDone();}

  grid_type    const& TheGrid()   const { return c.TheGrid();}
  Cell         const& TheCell()   const { return c;}
  Cell         const& TheAnchor() const { return c;}
  Facet               TheFacet()  const { return *fc;}
  FacetOnCellIterator TheFacetOnCellIterator() const { return fc;}

  static void init();

  static void init        (grid_type const& g_);
  static void remove      (grid_type const& g_);

  static void map_nb_table(grid_type const& g_, NBTABLE const& nbs_); 

  bool valid_neighbor() const { return  valid(handle());}
  bool inner_facet()    const { return  valid(handle());}
  bool boundary_facet() const { return !valid(handle());}

  void next_facet()           { ++fc;}
  void next_neighbor()        { next_facet(); advance_till_valid();}

  static self begin(Cell c) { return self(c);}
  static self end  (Cell c) { return self(GrAL::end<FacetOnCellIterator>(c));}

  bool operator==(self rhs) const { return c == rhs.c && fc == rhs.fc;}
  bool operator!=(self rhs) const { return !(*this == rhs);}

private:
  void cv() const { REQUIRE(valid_neighbor(), "",1);}
  bool valid(cell_handle ch) const { return TheGrid().valid_cell(ch);}

  void advance_till_valid() 
  { 
    while(! fc.IsDone() && ! valid(handle()))
      ++fc;
  } 
};


  template<class G, class NBTABLE, class GT>
  inline
  cell_on_cell_iterator<G,NBTABLE, GT> gral_begin(typename GT::Cell const& c, cell_on_cell_iterator<G,NBTABLE, GT>)
  { return cell_on_cell_iterator<G,NBTABLE, GT>::begin(c);}
 
  template<class G, class NBTABLE, class GT>
  inline
  cell_on_cell_iterator<G,NBTABLE, GT> gral_end  (typename GT::Cell const& c, cell_on_cell_iterator<G,NBTABLE, GT>)
  { return cell_on_cell_iterator<G,NBTABLE, GT>::end(c);}



} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Iterators/cell-on-cell-iterator.C"
#endif

#endif
