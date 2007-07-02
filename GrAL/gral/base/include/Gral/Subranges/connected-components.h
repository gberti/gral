#ifndef GRAL_GB_CONNECTED_COMPONENTS_H
#define GRAL_GB_CONNECTED_COMPONENTS_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"
#include "Container/function-adapter.h"

#include <queue>

namespace GrAL {

/*!  \brief Tools for handling connected components of grids

    Two cells are considered connected, if they share a facet.

    The classes of interest for a user are
    - component<GT>: View to a single connected component of a grid,
      containing a given cell
    - component_list<GT>: List of all connected components of a grid,
    giving STL-style iteration capability over them.

    \ingroup subranges
    \ingroup combinatoric_grid_algorithms
    \ingroup gridviews
    \see \ref combinatoric_grid_algorithms
*/
namespace connected_components {


  template<class GT>
  class component_iterator;

  template<class GT>
  class component;

  template<class GT>
  class cell_iterator;

  template<class GT>
  class component_list;



  /*! \brief View to a connected component of a grid.

      component<GT> is a model of $GrAL CellRange.
      Starting from a germ cell \f$ c \f$ ,
      it gives access to the corresponding connected component of the grid,
      that is, all cells reachable from \f$ c \f$  via facet-cell incidences.


      \templateparams
        - \c GT :
           - \c typedef  Cell, cell_handle
           - \c typedef  CellOnCellIterator

      \see \ref gridviews module
      \see Test in \ref test-connected-components.C
      \todo Forward other element types  defined in GT::grid_type,
       define  iterator types  for them.
   */ 
  template<class GT>
  class component {
    typedef component<GT>            self;
  public:
    enum { dim = GT::dimension_tag::dim };
    typedef grid_range_category_d<dim>      category;

    typedef typename GT::Cell               Cell;
    typedef typename GT::cell_handle        cell_handle;
    typedef cell_iterator<GT>               CellIterator;
    typedef typename GT::CellOnCellIterator CellOnCellIterator;
    typedef typename GT::grid_type          base_grid_type;
    typedef typename GT::size_type          size_type;

    // typedef closure_iterators::vertex_iterator<self, GT> VertexIterator;
  private:
    Cell                         c; // germ
    mutable size_type            num_of_cells;
    ref_ptr<component_list<GT> const> all_components;
  public:
    component() : num_of_cells(0) {}
    component(Cell c_, component_list<GT> const& all_c) 
      : c(c_), num_of_cells(0),   all_components(all_c) {}
    component(Cell c_, int noc, component_list<GT> const& all_c) 
      : c(c_), num_of_cells(noc), all_components(all_c) {}
    
    void init() const; 

    CellIterator FirstCell() const;
    size_type NumOfCells() const { init(); return num_of_cells;}
    //! true iff cc is in this component
    bool operator()(Cell const& cc) const;

    base_grid_type const& BaseGrid() const { return c.TheGrid();}
    // ref_ptr<component_list<GT> const> Components() const { return all_components;}
    Cell const& Germ() const { return c;}

  };




  /*! \brief CellIterator for component<GT>
      
      This type should be accessed via component<GT>
   */
  template<class GT /* , class PRED = true_pred */>
  class cell_iterator {
    typedef cell_iterator<GT> self;
    typedef component<GT>     component_type;
  public:
    typedef typename component_type::Cell           Cell;
    typedef typename component_type::cell_handle    cell_handle;
    typedef typename component_type::base_grid_type base_grid_type;
    typedef typename component_type::CellOnCellIterator CellOnCellIterator;
    typedef component_type                          grid_type;
    typedef partial_grid_function<Cell,bool>        visited_table_type;
  private:
    //component_type const* comp;
    ref_ptr<component_type const> comp; // may be temporary object!
    std::queue<cell_handle> Q;
    visited_table_type      visited;
  public:
    cell_iterator() : comp(0) {}
    cell_iterator(component_type const&  comp_) 
      : comp(new_ref_ptr(new component_type(comp_))), 
	visited(comp_.BaseGrid(), false) 
    { init();}
    cell_iterator(ref_ptr<component_type const>  comp_) 
      : comp(comp_), 
	visited(comp_->BaseGrid(), false) 
    { init();}
    
    void init() { 
      Q.push (comp->Germ().handle());
      visited[comp->Germ()] = true;
    }


    self& operator++() { 
      c_(); 
      Cell c(this->operator*());
      Q.pop();
      for(CellOnCellIterator cc(c); ! cc.IsDone(); ++cc) 
	if((*comp)(*cc) && ! visited(*cc)) {
	  visited[*cc] = true;
	  Q.push(cc.handle());
	}
      return *this;
    }
    bool IsDone()    const { cb_(); return Q.empty();}
    Cell operator*() const { c_ (); return Cell(BaseGrid(), handle());}
    cell_handle handle() const { c_(); return Q.front();}

    grid_type      const& TheGrid()  const { cb_(); return *comp;}
    base_grid_type const& BaseGrid() const { return TheGrid().BaseGrid();}

    bool bound() const { return comp != 0;}
    bool valid() const { return bound() && !IsDone();}
  private:
    // check object state
    void cb_() const {  REQUIRE(bound(), "no component object!", 1);}
    void c_ () const {  REQUIRE(valid(), "invalid iterator!", 1);}
  };


  template<class GT> 
  inline
  cell_iterator<GT> component<GT>::FirstCell() const { return cell_iterator<GT>(new_ref_ptr(new self(*this)));}




  /*! \brief A list of the connected components of a grid

     Usage example:
     \code
     grid_type g;
     // ....
     typedef connected_components::component_list<grid_type>  comp_list_t;
     comp_list_t comps(g);
     cout << "Grid has " << comps.NumOfComponents() << " components\n";
     for(comp_list_t::const_iterator cmp=comps.begin(); cmp != comps.end(); ++cmp)
       cout << "Component #" << cmp.handle() << " has " << (*cmp).NumOfCells() << " cells\n";
     \endcode
  */
  template<class GT>
  class component_list {
  public:
    typedef typename GT::grid_type  grid_type;
    typedef typename GT::size_type  size_type;
    typedef grid_type               range_type;
    // even better: cell_range_type
    // typedef typename GT::range_type range_type; // may be the same as grid_type, but also a subrange type
    typedef typename GT::Cell                Cell;
    typedef typename GT::CellIterator        CellIterator;
    typedef typename GT::cell_handle         cell_handle;
    typedef typename GT::CellOnCellIterator  CellOnCellIterator;

  private:
    grid_type    const*              g;
    mutable grid_function<Cell, size_type> comps; // map cells to component number
    mutable std::vector<Cell>         germs; // map component number to a cell
    mutable std::vector<size_type>    num_of_cells; // number of cell of comp. #i
    mutable size_type                 num_of_components;
    mutable bool                      initialized;
  public:
    // use lazy initialization
    component_list(grid_type const& g_) 
      : g(&g_), num_of_components(0), initialized(false) {}

    template<class PRED>
    component_list(grid_type const& g_, PRED inside) 
      : g(&g_), num_of_components(0), initialized(false) 
    { init(inside); }


    template<class PRED>
    void init(PRED inside) const;

    void init() const { // does not change the *logical* state
      init(constant<Cell, bool>(true));
    }
    

    struct component_handle {
      size_type c;
      operator int() const { return c;}
      explicit component_handle(size_type c_) : c(c_) {}
    };
    
    typedef component_iterator<GT>  const_iterator;
    typedef component<GT>           component_type;
    typedef component<GT>           component_type_cref; // lightweight class, can always return it by value
    
    bool bound() const { return (g != 0);}
    
    grid_type const& TheGrid() const { c_(); return *g;} 
    size_type NumOfComponents() const { c_(); init(); return num_of_components;}

    const_iterator FirstComponent() const;
    //! past-the-end iterator
    const_iterator EndComponent() const;

    //! STL-compliant alias for FirstComponent()
    const_iterator begin() const;
    //! STL-compliant alias for EndComponent()
    const_iterator end()   const;
    
    component_handle ComponentOf(Cell const& c) const { c_(); return component_handle(comps(c));}
    component_type_cref   Component(component_handle c) const 
    { REQUIRE(valid(c), "",1); return component_type_cref(germs[c],num_of_cells[c], *this);}

    bool valid(component_handle c) const { return (0 <= c) && (c < static_cast<int>(NumOfComponents()));} 
  private:
    void c_() const { REQUIRE(bound(), "no grid!",1);}
  }; // class component_list



  /*! \brief Iterator over the connected components of a grid.

       Access via component_list::const_iterator;
  */
  template<class GT>
  class component_iterator {
    typedef component_iterator<GT> self;
    typedef component_list<GT>     component_list_type; 
    typedef typename component_list_type::component_handle component_handle;
  public:
    typedef typename component_list_type::component_type      value_type;
    typedef typename component_list_type::component_type_cref value_type_cref;
    typedef typename GT::size_type                            size_type;
  private:
    size_type c;
    component_list_type const* comps;
  public:
    component_iterator() : c(0), comps(0) {}
    component_iterator(component_list_type const& comps_,
		       int c_ = 0) : c(c_), comps(&comps_) {}

    self & operator++() { c_(); ++c; return *this;}
    bool IsDone() const { cb_(); return (c >= static_cast<int>(comps->NumOfComponents()));}
    value_type_cref operator*() const { c_(); return comps->Component(handle());}
    component_handle handle  () const { c_(); return component_handle(c);}

    bool bound() const { return (comps != 0);}
    bool valid() const { return bound() && (! IsDone()) && (c >= 0);}

    bool operator==(self const& rhs) const { 
      cb_();
      REQUIRE((comps == rhs.comps), "component_iterator<GT>::==: Component lists differ",1); 
      return (c == rhs.c);
    }
    bool operator!=(self const& rhs) const { return !(*this == rhs);} 

  private:
    void c_ () const { REQUIRE(valid(), "invalid component_iterator! c = " << c,1);}
    void cb_() const { REQUIRE(bound(), "no connected_components object!",1); }
  };

  template<class GT>
  inline
  component_iterator<GT> component_list<GT>::FirstComponent() const { return begin();}

  template<class GT>
  inline
  component_iterator<GT> component_list<GT>::EndComponent()   const { return end();}
 
  template<class GT>
  inline
  component_iterator<GT> component_list<GT>::begin() const { return const_iterator(*this,0);}

  template<class GT>
  inline
  component_iterator<GT> component_list<GT>::end  ()   const { return const_iterator(*this,num_of_components);}

  template<class GT>
  inline
  bool component<GT>::operator()(typename component<GT>::Cell const& cc) const 
  { return all_components->ComponentOf(c) == all_components->ComponentOf(cc);}


} // namespace connected_components

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Subranges/connected-components.C"
#endif


#endif
