#ifndef GRAL_GB_NON_CONFORMING_LEAF_GRID_H
#define GRAL_GB_NON_CONFORMING_LEAF_GRID_H

// $LICENSE_NEC

// TODO: the templates used from these files could be forwarded in the template parameters
// e.g. OCTREE::vertex_type<ELEMBASE>::type;
#include "Gral/Hierarchical/octree.h"            // octree_leaf_cell_iterator_t<> 
#include "Gral/Hierarchical/hierarchical-grid.h" // h_vertex_t<> etc.

namespace octree {

  template<class LEAFGRID>  class nc_leafgrid_element_base_t;


  /*! \brief A non-conforming grid formed by the leafs of an octree
 
      \autor Guntram Berti
      \see   conforming_leafgrid
   */
  template<class OCTREE>
  class non_conforming_leafgrid {
    typedef non_conforming_leafgrid<OCTREE> self;
  public:
    typedef OCTREE                                         octree_type;
    typedef typename octree_type::hier_grid_type           hier_grid_type;

    typedef hier_grid_type                                 hgt;
    typedef typename hgt::level_handle                     level_handle;

    typedef typename hier_grid_type::flatgt                flatgt;
    typedef typename flatgt::vertex_handle                 flat_vertex_handle;
    typedef typename flatgt::cell_handle                   flat_cell_handle;


    typedef nc_leafgrid_element_base_t<self>               element_base_type;

    typedef hierarchical::h_vertex_t<element_base_type>                Vertex;
    typedef hierarchical::h_cell_t  <element_base_type>                Cell;
    // cannot use hier_grid::valid() with these ... -> make valid() a template!
    //    typedef hierarchical::h_element_handle_t<self, flat_cell_handle>   cell_handle;
    //    typedef hierarchical::h_element_handle_t<self, flat_vertex_handle> vertex_handle;
    typedef typename hgt::vertex_handle vertex_handle;
    typedef typename hgt::cell_handle   cell_handle;
    //    typedef typename Vertex::vertex_handle                 vertex_handle;
    
    typedef hierarchical::h_vertex_on_cell_iterator_t<element_base_type>  
                                                           VertexOnCellIterator;
     
    // from octree
    typedef leaf_cell_iterator_t<element_base_type>        CellIterator;

    // dummy, the h_cell_t wants it.
    // But a leaf grid conceptually is a flat grid.
    struct CellChildIterator {};

  private:
    ref_ptr<const octree_type> the_oct;
  public:
    non_conforming_leafgrid() {}
    non_conforming_leafgrid(ref_ptr<const octree_type> o) : the_oct(o) {}

    
    CellIterator FirstCell();
    size_t       NumOfCells() { return TheOctree()->NumOfLeafCells();}
    VertexIterator FirstVertex();  
  // size_t       NumOfVertices() { ??? } 

    ref_ptr<const octree_type>    TheOctree() const { return the_oct;}

    // TODO:  reuse archetype stuff from underlying flat grid
    // FIXME: there are no archetypes for 2D grids!
  };
  
  template<class LEAFGRID>
  class nc_leafgrid_element_base_t
  /* : public typename OCTREE::element_base_type */
  {
    // typedef typename OCTREE::element_base_type base;
    typedef nc_leafgrid_element_base_t<LEAFGRID> self;
  public:
    typedef LEAFGRID                             grid_type;
    typedef grid_type                            gt;
    typedef typename grid_type::octree_type      octree_type;
    typedef octree_type                          ogt;
    typedef typename octree_type::hier_grid_type hier_grid_type;
    typedef hier_grid_type                       hgt;

    // types related to 'current' grid type
    
    typedef typename gt::Cell           Cell;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::cell_handle    cell_handle;
    typedef typename gt::vertex_handle  vertex_handle;
    
    /*
    typedef hierarchical::h_cell_t  <self>  Cell;
    typedef hierarchical::h_vertex_t<self>  Vertex;
    typedef hierarchical::h_element_handle_t<LEAFGRID, flat_cell_handle> cell_handle;
    typedef hierarchical::h_element_handle_t<LEAFGRID, flat_cell_handle> cell_handle;
    */
    //typedef typename Cell::cell_handle      cell_handle;
    //typedef typename Vertex::vertex_handle  vertex_handle;
  
    // types related to hierarchical grid type
    typedef typename hgt::level_handle   level_handle;
    typedef typename hgt::flatgt         flatgt;
    typedef typename hgt::flat_grid_type flat_grid_type;


    // types related to octree type
    // typedef typename ogt::xxx  yyy;
  private:
    ref_ptr<const grid_type> the_grid;
    level_handle             lev;
  public:
    nc_leafgrid_element_base_t() {}
    nc_leafgrid_element_base_t(grid_type const* g, level_handle l = g->TheOctree()->finest_level())
      : the_grid(g), lev(l) {}
    nc_leafgrid_element_base_t(ref_ptr<const grid_type> g, level_handle l = g->TheOctree()->finest_level())
      : the_grid(g), lev(l) {}

    bool bound() const { return the_grid != 0;}
    void cb()    const { REQUIRE(bound(), "", 1); }
    level_handle level() const { return lev;}

    ref_ptr<const flat_grid_type> TheFlatGrid() const { return TheHierGrid()->FlatGrid(level());}
    ref_ptr<const hier_grid_type> TheHierGrid() const { return TheOctree()->TheHierGrid();}
    ref_ptr<const octree_type>    TheOctree()   const { return TheGrid()->TheOctree();}
    ref_ptr<const grid_type>      TheGrid()     const { return the_grid;}
    ref_ptr<const grid_type>      TheLeafGrid() const { return the_grid;}

    ref_ptr<const flat_grid_type> Grid(tp<flat_grid_type>) const { return TheFlatGrid();}
    ref_ptr<const hier_grid_type> Grid(tp<hier_grid_type>) const { return TheHierGrid();} 
    ref_ptr<const octree_type>    Grid(tp<octree_type>)    const { return TheOctree();} 
    ref_ptr<const grid_type>      Grid(tp<grid_type>)      const { return TheGrid();}
  }; // class nc_leafgrid_element_base_t<>


  //---------- inline functions -----------------------

  template<class OCTREE>
  typename non_conforming_leafgrid<OCTREE>::CellIterator
  non_conforming_leafgrid<OCTREE>::FirstCell() const 
  { return CellIterator(*this);}
  

} // namespace octree 

template<class OCTREE>
struct grid_types<octree::non_conforming_leafgrid<OCTREE> > 
  : public grid_types_base<octree::non_conforming_leafgrid<OCTREE> > 
{
  
};

#endif
