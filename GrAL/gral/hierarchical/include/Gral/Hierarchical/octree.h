#ifndef GRAL_GB_HIERARCHICAL_OCTREE_H
#define GRAL_GB_HIERARCHICAL_OCTREE_H

// $LICENSE_NEC


#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Hierarchical/hierarchical-grid-table.h"
#include "Gral/Hierarchical/hierarchical-partial-grid-function.h"

#include "Utility/ref-ptr.h"

#include "Container/sequence-algorithms.h"

#include <algorithm>  // find
#include <functional> // mem_fun_ref


/*! \brief Implementation of a generalized octree
    \author Guntram Berti 

    A generalized octree has as root subdivision which is not necessarily a single
    cube, but can be a Cartesian grid \f${\cal C}_0\f$. 
    In the special case that \f${\cal C}_0\f$ has only one cell, this reduces
    to a conventional octree.

    \note It is also possible to add coarser layers (refining the initial root layer) afterwards

    \test test-octree.C
*/

namespace octree {
  
 
  template<class PREDCLASS>
  struct isActive_pred {
    PREDCLASS const* pred;
    isActive_pred(PREDCLASS const& pr) : pred(&pr) {}
    template<class T>
    bool operator()(T const& t) const { return pred->isActive(t);}
  };

  template<class PREDCLASS>
  inline
  isActive_pred<PREDCLASS> isActivePred(PREDCLASS const& pr) { return isActive_pred<PREDCLASS>(pr);}


  template<class GRID, class GT = grid_types<GRID> >
  class octree_element_base_t;

  template<class ELEMBASE>
  class leaf_cell_iterator_t;

/*! \brief Generalized octree

    \author Guntram Berti

    A generalized octree need not have a single root cell, but may
    have a Cartesian <em> root grid </em> instead. 
    This root can be accessed by using the \c coarsest_level() member.
 */

  namespace hier = hierarchical;

template<class Grid, class GT = grid_types<Grid> >
class Octree {
  typedef Octree self;
public:
  typedef Grid                                     flat_grid_type;
  typedef GT                                       flatgt;
  typedef typename flatgt::index_type              index_type;
  typedef hierarchical::hgrid_cartesian<flat_grid_type>  hier_grid_type;

  typedef hier_grid_type                           hgt;
  typedef typename hgt::pattern_grid_type          pattern_grid_type;
  typedef typename hgt::level_handle               level_handle;
  typedef typename hgt::hier_cell_type                   hier_cell_type;
  typedef typename hgt::CellChildIterator          HierCellChildIterator;
  typedef typename hgt::cart_subrange_type         flat_subrange_type;
  typedef typename flatgt::CellIterator            flat_cell_iterator;
  typedef typename flatgt::Cell                    flat_cell_type;

  typedef octree_element_base_t<flat_grid_type, flatgt> element_base_type;
  typedef leaf_cell_iterator_t<element_base_type>       LeafCellIterator;

  //FIXME: these should be separate types instead of typedefs
  typedef hier_cell_type              oct_cell_type;
  typedef HierCellChildIterator OctCellChildIterator;

  typedef oct_cell_type               Cell;
  typedef OctCellChildIterator  CellChildIterator;

  typedef size_t size_type;

  //  typedef marked_subrange<cartesian_grid_type>     grid_range_type;
  //typedef hier_grid_table<active_grid_range_type>  subrange_table_type;

  // FIXME: partial_gf is no good impl. of a cell range. ElementIterator
  // is not guaranteed to traverse only Cells with value 'true'. (see deactivate(oct_cell_type)).
  typedef hier::hier_partial_grid_function<hier_cell_type, bool>  subrange_table_type;
  typedef typename subrange_table_type::flat_gf_type              grid_range_type;

  typedef typename grid_range_type::CellIterator ActiveLevelCellIterator;
private:  
  hier_grid_type      levels;
  subrange_table_type active_range;

  

  // Forbidden for the moment
  //  Octree(Octree const&);
  //  Octree& operator=(Octree const&);
public:
  /*! \name Constructors
   */
  //@{ 
  //! Construct empty octree
  Octree();
  //! Construct with initial root layer \c C0
  Octree(flat_grid_type const& C0,
	 pattern_grid_type   const& refpat);
  Octree(ref_ptr<flat_grid_type const> C0,
	 pattern_grid_type   const& refpat);
  Octree(hier_grid_type const& H);

  // Octree(int nx_, int ny_, int nz_);

  /*! \brief Add the initial level.
      All  cells are active by default (ensuring that everything is covered by active cells)
   */
  void init(flat_grid_type const& C0,
	    pattern_grid_type   const& refpat);
  //@}

  ref_ptr<const self>               TheOctree() const { return ref_ptr<const self>(this);}
  ref_ptr<const hier_grid_type>     TheHierGrid() const { return ref_ptr<const hier_grid_type>(&levels);}
  ref_ptr<const pattern_grid_type>  ThePatternGrid()  const { return TheHierGrid()->ThePatternGrid();}
  ref_ptr<const flat_grid_type>     LevelGrid(level_handle lev) const 
  { return ref_ptr<const flat_grid_type>(levels(lev));}
  ref_ptr<const grid_range_type>    ActiveRange(level_handle lev) const
  { return ref_ptr<const grid_range_type>(active_range(lev));}


  /*! \name Modifying operations
   */
  //@{
  /*! \brief join the cells (i.e. subtree) beneath \c newLeaf, such that \c newLeaf becomes a leaf.
      
      \pre  <tt> isLeaf(c) == false </tt> \f$ \wedge \f$ </tt> coarsenable() </tt>
      \post <tt> isLeaf(c) == true  </tt> 
  */
  void join_cells(oct_cell_type const& newLeaf);

  /*! \brief split the leaf cell \c oldLeaf, and add new layer if necessary.

    Inverse to join_cells().
   
     \pre <tt> is_leaf(c) == true </tt>
     \invariant <tt> split_cell(c); join_cells(c) </tt> does not change the state of the octree.
     
   */ 

  void split_cell(oct_cell_type const& oldLeaf); 

  /*! \brief add a new coarser level
      \pre the current coarsest level can be coarsened with the refinement pattern
   */
  void add_coarser_level(); 
  /*! \brief add a new finer level
   */
  void add_finer_level(); 

  //@}


  //! check whether the octree can be coarsened with the current refinement/coarsening pattern
  bool coarsenable() const { return TheHierGrid()->coarsenable();}

  /*! \name Operations on oct_cell_types
   */
  //@{
  /*! \brief true if \c is part of the octree
   */
  bool isActive(hier_cell_type const& c) const { return active_range(c.level())(c);}
  /*! A cell is a leaf if there are no sons, that is, 
    all child cells are inactive.
 
    \note If only some of the children are inactive, the cell is not a leaf.
    */
  bool isLeaf(hier_cell_type const& c)   const 
    { 
      return isActive(c) 
	&& (c.level() == finest_level() 
	    ||  ! sequence::exists(c.FirstChild(), c.EndChild(), isActivePred(*this)));
    }
  /*! \brief True if \c is active but not a leaf.
  */
  bool isBranch(hier_cell_type const& c) const
  {
    return isActive(c) && ! isLeaf(c);
  }

  //! return the level of \c c
  level_handle  level(hier_cell_type c)   const { return c.level();}

  //! return the youngest ancestor of \c subLeaf which is in the octree (i.e. is a leaf).
  oct_cell_type leaf_ancestor(hier_cell_type subLeaf) const;
  //@}

  /*! \name Level navigation
   */
  //@{
  //! get the coarsest (root) level
  level_handle  coarsest_level()   const { return TheHierGrid()->coarsest_level();}
  //! get the finest level
  level_handle  finest_level() const { return TheHierGrid()->finest_level();}
  /*! \brief get handle of next finer level
      \pre <tt> lev != finest_level() </tt>
   */
  level_handle  next_finer_level  (level_handle lev) const { return TheHierGrid()->next_finer_level(lev);}
  /*! \brief  get handle of next coarser level
      \pre <tt> lev != coarsest_level() </tt>
  */
  level_handle  next_coarser_level(level_handle lev) const { return TheHierGrid()->next_coarser_level(lev);}
  //! get the number of levels
  size_type     num_of_levels() const { return levels.num_of_levels();}
  //@}

  /*! \brief Check whether to octree is empty.
     To fill an empty octree, one has to use the \c init() member, and
     subsequent calls to \c split_cell() / \c join_cells().
   */
  bool          empty()         const { return num_of_levels()==0;}

  //! Number of active cells
  size_type NumOfCells()     const;
  //! Numbdr of leaf cells
  size_type NumOfLeafCells() const;


  /*! \brief true if level \c lev does not contain active cells.
      This means that also all finer levels are empty.
   */
  bool empty(level_handle lev) const { return active_range(lev).empty();}


  bool valid(level_handle lev) const { return (levels.coarsest_level() <= lev && lev <= levels.finest_level());}
  void cv(level_handle lev) const 
  { REQUIRE(valid(lev), "lev=" << lev, 1); }
  //private:
public:
  
  /*! \brief mark the level as active
   */
  void activate (level_handle lev) { 
    for(typename flatgt::CellIterator c = LevelGrid(lev)->FirstCell(); ! c.IsDone(); ++c)
      active_range[lev][*c] = true;
  }
  /*! \brief mark the level as non-active
   */
  void deactivate (level_handle lev) { 
    active_range.set_default(false);
    for(typename flatgt::CellIterator c = LevelGrid(lev)->FirstCell(); ! c.IsDone(); ++c)
      active_range[lev].undefine(*c);
  }


  /*! \brief mark the cell as active
   */
  void activate (oct_cell_type c) { active_range[c.level()][c] = true;}

  /*! \brief unmark the cell as active
      \todo This works only if the default value is false!
   */
  void deactivate (oct_cell_type c) { active_range[c.level()].undefine(c); }
  //! mark the cell as active and leaf
  void make_leaf  (oct_cell_type c) { active_range[c.level()][c] = true;}
  //! mark the cell as an internal octcell (active, but no leaf)
  void make_branch(oct_cell_type c) { active_range[c.level()][c] = true;}

  void join_cells_rec(oct_cell_type const& newLeaf);

}; // class Octree


  // element_base_t: plugin to construct element types (and sequence iterators)
  // referring to octree
  template<class GRID, class GT /* = grid_types<GRID> */ >
  class octree_element_base_t {
    typedef octree_element_base_t<GRID,GT> self;
  public:
    typedef Octree<GRID, GT>                     octree_type;

    typedef octree_type                          grid_type;
    typedef grid_type                            gt;

    typedef octree_type                          ogt;
    typedef typename ogt::hier_grid_type         hier_grid_type;
    typedef hier_grid_type                       hgt;

    typedef typename hgt::flat_grid_type         flat_grid_type;
    typedef typename hgt::flatgt                 flatgt;
    typedef typename hgt::level_handle           level_handle;
    // typedef typename hgt::flat_cell_type               flat_cell_type;

    // typedef typename octree_type::OctVertex      Vertex;
    typedef typename ogt::oct_cell_type        oct_cell_type;

    typedef typename gt::Cell            Cell;

  protected:
    ref_ptr<const octree_type> oct;
    level_handle               lev;
  public:
    octree_element_base_t() {}
    explicit
    octree_element_base_t(octree_type const* o,         level_handle l = o->coarsest_level()) 
      : oct(o), lev(l) {}
    explicit
    octree_element_base_t(ref_ptr<const octree_type> o, level_handle l = o->coarsest_level()) 
      : oct(o), lev(l) {}

    bool bound() const { return oct != 0;}
    void cb()    const { REQUIRE(bound(), "", 1); }
    level_handle level() const { return lev;}

    //    ref_ptr<const grid_type>      TheGrid()     const { return oct;}
    grid_type const& TheGrid() const { return *oct;}
    ref_ptr<const octree_type>    TheOctree()   const { return oct;}
    ref_ptr<const hier_grid_type> TheHierGrid() const { return oct->TheHierGrid();}
    ref_ptr<const flat_grid_type> TheFlatGrid() const { return oct->LevelGrid(lev);}

    ref_ptr<const flat_grid_type> Grid(tp<flat_grid_type>) const { return TheFlatGrid();}
    ref_ptr<const hier_grid_type> Grid(tp<hier_grid_type>) const { return TheHierGrid();} 
    ref_ptr<const octree_type>    Grid(tp<octree_type>)    const { return TheOctree();} 
    // ref_ptr<const grid_type>      Grid(tp<grid_type>)      const { return TheGrid();}

  }; // class element_base_t<GRID,GT> 



  template<class ELEMBASE>
  class leaf_cell_iterator_t : public ELEMBASE {
    typedef leaf_cell_iterator_t<ELEMBASE> self;
  public:
    typedef ELEMBASE            base;
    typedef typename base::grid_type grid_type;
    typedef typename base::octree_type octree_type;
    typedef typename base::gt        gt;
    typedef typename gt  ::Cell      Cell;

    typedef typename base::ogt      ogt;   
    typedef typename ogt::oct_cell_type   oct_cell_type;
    typedef typename ogt::ActiveLevelCellIterator ActiveLevelCellIterator;

    typedef typename base::hgt       hgt;
    typedef typename hgt::flat_cell_type   flat_cell_type;

  private:
    ActiveLevelCellIterator c;
  public:
    leaf_cell_iterator_t() {}
    leaf_cell_iterator_t(grid_type const& o)
      : base(&o, o.TheOctree()->coarsest_level()),
      c(o.TheOctree()->ActiveRange(o.TheOctree()->coarsest_level())->FirstCell())
    { make_valid();}
    leaf_cell_iterator_t(ref_ptr<const grid_type> o)
      : base(o, o->TheOctree()->coarsest_level()),
      c(oct->ActiveRange(o->TheOctree()->coarsest_level())->FirstCell())
    { make_valid();}

    bool IsDone()    const { cb(); return level() > TheOctree()->finest_level();}
    Cell operator*() const { cv(); return Cell(TheGrid(), *c, level());}    
    self& operator++() {
      cv();  ++c; make_valid(); return *this;
    }

    flat_cell_type Flat() const { return *c;}
    bool valid() const { return ! IsDone() && on_leaf();}
    void cv()    const { REQUIRE( (bound() && valid()), "", 1);}
  private:
    void make_valid() {
      while(! IsDone() && ! on_leaf()) {
	make_valid_c();
	if( c.IsDone()) {
	  ++lev;
	  if(lev <= TheOctree()->finest_level())
	    c=TheOctree()->ActiveRange(lev)->FirstCell(); 
	}
      }
    }
    void make_valid_c() { while(!c.IsDone() && ! on_leaf()) ++c;}
    bool on_leaf() const { return ! c.IsDone() && TheOctree()->isLeaf(oct_cell_type(* TheHierGrid(), *c, level()));}
  }; // class leaf_cell_iterator_t<BASE>


   

} // namespace octree


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Hierarchical/octree.C"
#endif

#endif
