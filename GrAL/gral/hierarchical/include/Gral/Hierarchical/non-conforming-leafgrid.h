#ifndef GRAL_GB_NON_CONFORMING_LEAF_GRID_H
#define GRAL_GB_NON_CONFORMING_LEAF_GRID_H

// $LICENSE_NEC

// TODO: the templates used from these files could be forwarded in the template parameters
// e.g. OCTREE::vertex_type<ELEMBASE>::type;
#include "Gral/Hierarchical/octree.h"            // octree_leaf_cell_iterator_t<> 
#include "Gral/Hierarchical/hierarchical-grid.h" // h_vertex_t<> etc.

#include "Gral/Iterators/vertex-iterator-of-cell-set.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Base/partial-grid-function-hash.h"


namespace octree {

  template<class LEAFGRID>     class nc_leafgrid_element_base_t;
  template<class OCTREE>       class non_conforming_leafgrid;
  template<class ELEMENTBASE>  class nc_leafgrid_vertex_t;

  /*! \brief A non-conforming grid formed by the leafs of an octree
 
      \autor Guntram Berti
      \see   conforming_leafgrid
   */
  template<class OCTREE>
  class non_conforming_leafgrid {
    typedef non_conforming_leafgrid<OCTREE> self;
  public:
    typedef self                                           grid_type;
    typedef OCTREE                                         octree_type;
    typedef typename octree_type::hier_grid_type           hier_grid_type;

    typedef hier_grid_type                                 hgt;
    typedef typename hgt::level_handle                     level_handle;

    typedef typename hgt::flat_grid_type                   flat_grid_type;
    typedef typename hier_grid_type::flatgt                flatgt;
    typedef typename flatgt::vertex_handle                 flat_vertex_handle;
    typedef typename flatgt::cell_handle                   flat_cell_handle;


    typedef nc_leafgrid_element_base_t<self>               element_base_type;

    // cannot use h_vertex_t: must use special functionality to fuse
    // vertices of different level which map to the same leaf vertex.
    //    typedef hierarchical::h_vertex_t<element_base_type>      Vertex;
    typedef nc_leafgrid_vertex_t    <element_base_type>      Vertex;
    typedef hierarchical::h_cell_t  <element_base_type>      Cell;
  
    typedef hierarchical::h_element_handle_t<self, flat_cell_handle>   cell_handle;
    typedef hierarchical::h_element_handle_t<self, flat_vertex_handle> vertex_handle;
  
    //typedef typename hgt::vertex_handle vertex_handle;
    //typedef typename hgt::cell_handle   cell_handle;
    
    typedef hierarchical::h_vertex_on_cell_iterator_t<element_base_type>  
                                                           VertexOnCellIterator;
     
    // from octree
    typedef leaf_cell_iterator_t<element_base_type>          CellIterator;

    typedef vertex_iterator_of_cell_set<CellIterator, self>  VertexIterator;
    // dummy, the h_cell_t wants it.
    // But a leaf grid conceptually is a flat grid.
    struct CellChildIterator {};

  private:
    ref_ptr<const octree_type> the_oct;
  public:
    non_conforming_leafgrid() {}
    non_conforming_leafgrid(        octree_type const& o) : the_oct(&o) {}
    non_conforming_leafgrid(ref_ptr<octree_type const> o) : the_oct(o)  {}

    
    CellIterator   FirstCell()     const;
    size_t         NumOfCells()    const { return TheOctree()->NumOfLeafCells();}
    VertexIterator FirstVertex()   const;  
    size_t         NumOfVertices() const;

    ref_ptr<const octree_type>    TheOctree() const { return the_oct;}
    ref_ptr<const hier_grid_type> TheHierGrid() const { return TheOctree()->TheHierGrid();}

    // reuse archetypes from flat grid.
    // NOTE: this assumes that flat_grid has static archetypes!
    typedef typename flatgt::archetype_type     archetype_type;
    typedef typename flatgt::archetype_handle   archetype_handle;
    typedef typename flatgt::archetype_iterator archetype_iterator;
    typedef grid_types<archetype_type>          archgt;               

    static archetype_iterator BeginArchetype()  { return flat_grid_type::BeginArchetype();}
    static archetype_iterator EndArchetype()    { return flat_grid_type::EndArchetype();}
    static unsigned NumOfArchetypes()           { return flat_grid_type::NumOfArchetypes();}
    static archetype_type const& ArchetypeOf(Cell const& c)  { return flat_grid_type::ArchetypeOf(c);}
    static archetype_type const& ArchetypeOf(cell_handle c)  { return flat_grid_type::ArchetypeOf(c);}
    static archetype_handle      archetype_of(Cell const& c) { return flat_grid_type::archetype_of(c);}
    static archetype_handle      archetype_of(cell_handle c) { return flat_grid_type::archetype_of(c);}

    static archetype_type const& Archetype(archetype_handle const& a) { return flat_grid_type::Archetype(a);}

  };

  template<class OCTREE>
  struct nc_leafgrid_grid_types_base {
    typedef non_conforming_leafgrid<OCTREE>    grid_type;
    typedef typename grid_type::Cell           Cell;
    typedef typename grid_type::Vertex         Vertex;
    typedef typename grid_type::CellIterator   CellIterator;
    typedef typename grid_type::VertexIterator VertexIterator;
    typedef typename grid_type::cell_handle    cell_handle;
    typedef typename grid_type::vertex_handle  vertex_handle;
    typedef typename grid_type::VertexOnCellIterator VertexOnCellIterator;

    typedef typename grid_type::flatgt         flatgt;
    typedef typename flatgt::dimension_tag     dimension_tag;

    typedef typename grid_type::archetype_type     archetype_type;
    typedef typename grid_type::archetype_handle   archetype_handle;
    typedef typename grid_type::archetype_iterator archetype_iterator;

  };

  template<class LEAFGRID>
  class nc_leafgrid_element_base_t
  /* : public typename OCTREE::element_base_type */
  {
    // typedef typename OCTREE::element_base_type base;
    typedef nc_leafgrid_element_base_t<LEAFGRID> self;
  public:
    typedef LEAFGRID                             grid_type;
    typedef grid_type                            leaf_grid_type;
    // shouldn't this be grid_types<LEAFGRID> ??
    typedef grid_type                            gt;
    typedef typename grid_type::octree_type      octree_type;
    typedef typename octree_type::hier_grid_type hier_grid_type;

    // types related to 'current' grid type
    typedef typename gt::Cell           Cell;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::cell_handle    cell_handle;
    typedef typename gt::vertex_handle  vertex_handle;
    
    // types related to hierarchical grid type
    typedef hier_grid_type               hgt;
    typedef typename hgt::level_handle   level_handle;
    typedef typename hgt::flatgt         flatgt;
    typedef typename hgt::flat_grid_type flat_grid_type;

    // types related to octree type
    typedef octree_type                          ogt;
    typedef typename ogt::OctCell                OctCell;
  protected:
    ref_ptr<const grid_type> the_grid;
    level_handle             lev;

    void set(level_handle newlev) { lev = newlev;}
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
    ref_ptr<const octree_type>    TheOctree()   const { return TheGrid().TheOctree();}
    //    ref_ptr<const grid_type>      TheGrid()     const { return the_grid;}
    const grid_type&   TheGrid() const { return *the_grid;}
   ref_ptr<const leaf_grid_type> TheLeafGrid() const { return the_grid;}

    ref_ptr<const flat_grid_type> Grid(tp<flat_grid_type>) const { return TheFlatGrid();}
    ref_ptr<const hier_grid_type> Grid(tp<hier_grid_type>) const { return TheHierGrid();} 
    ref_ptr<const octree_type>    Grid(tp<octree_type>)    const { return TheOctree();} 
    ref_ptr<const grid_type>      Grid(tp<grid_type>)      const { return the_grid;}
  }; // class nc_leafgrid_element_base_t<>



  template<class ELEMENTBASE>
  class nc_leafgrid_vertex_t : public hierarchical::h_vertex_t<ELEMENTBASE>
  {
  private:
    typedef nc_leafgrid_vertex_t    <ELEMENTBASE> self;
    typedef hierarchical::h_vertex_t<ELEMENTBASE> base;
  public:
    nc_leafgrid_vertex_t() {} 
    nc_leafgrid_vertex_t(grid_type const& gg, vertex_handle hh) : base(gg, hh)
    { normalize(); }
    nc_leafgrid_vertex_t(grid_type const& gg, flat_vertex_type   f, level_handle lev) : base(gg,f,lev)
    { normalize(); }
    nc_leafgrid_vertex_t(grid_type const& gg, flat_vertex_handle f, level_handle lev) : base(gg,f,lev)
    { normalize(); }
    nc_leafgrid_vertex_t(ref_ptr<grid_type const> gg, vertex_handle hh) : base(gg, hh)
    { normalize(); }
    nc_leafgrid_vertex_t(ref_ptr<grid_type const> gg, flat_vertex_type   f, level_handle lev) : base(gg,f,lev)
    { normalize(); }
    nc_leafgrid_vertex_t(ref_ptr<grid_type const> gg, flat_vertex_handle f, level_handle lev) : base(gg,f,lev)
    { normalize();}
    

    friend bool operator==(self const& lhs, self const& rhs) 
    { lhs.cv(); rhs.cv(); return lhs.level() == rhs.level() && lhs.flat_handle() == rhs.flat_handle();}
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
  protected:
    void set(flat_vertex_handle newfh, level_handle newlev) { base::set(newfh, newlev);}
  private:
    void normalize() { 
      vertex_handle cv = TheHierGrid()->coarsest_parent(*this);
      if (cv.level() < level())
	set(cv.flat_handle(), cv.level());
    }

  };



  //---------- inline functions -----------------------

  template<class OCTREE>
  typename non_conforming_leafgrid<OCTREE>::CellIterator
  non_conforming_leafgrid<OCTREE>::FirstCell() const 
  { return CellIterator(*this);}

  template<class OCTREE>
  typename non_conforming_leafgrid<OCTREE>::VertexIterator
  non_conforming_leafgrid<OCTREE>::FirstVertex() const 
  { return VertexIterator(FirstCell());}
  


} // namespace octree 


/* NOTE: cannot use 
template<class OCTREE>
struct element_traits<hierarchical::h_vertex_t<typename octree::non_conforming_leafgrid<OCTREE>::element_base_type> >
 here
*/

template<class OCTREE>
struct element_traits<octree::nc_leafgrid_vertex_t
                         <octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >
                      >
  : public element_traits_vertex_base< octree::non_conforming_leafgrid<OCTREE> >
{
  typedef element_traits_vertex_base<octree::non_conforming_leafgrid<OCTREE> > base;
  typedef typename octree::non_conforming_leafgrid<OCTREE>::Vertex Vertex;
  typedef typename Vertex::flatgt flatgt;
  typedef typename flatgt::Vertex flat_vertex_type;
  typedef typename element_traits<flat_vertex_type>::hasher_type flat_hasher_type;
 
  struct hasher_type : public base::hasher_type_elem_base {
    // this is a bad hasher
    // could multiply with (level() -coarsest_level())*FinestGrid().NumOfVertices()
    // to guarantee unique numbers
    size_t operator()(element_type const& V) const { flat_hasher_type h; return h(V);}
  };
};

template<class OCTREE>
struct element_traits<hierarchical::h_cell_t
                         <octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >
                      >
  : public element_traits_cell_base< octree::non_conforming_leafgrid<OCTREE> >
{
  typedef element_traits_cell_base<octree::non_conforming_leafgrid<OCTREE> > base;
  typedef typename octree::non_conforming_leafgrid<OCTREE>::Cell Cell;
  typedef typename Cell::flatgt flatgt;
  typedef typename flatgt::Cell flat_cell_type;
  typedef typename element_traits<flat_cell_type>::hasher_type flat_hasher_type;
 
  struct hasher_type : public base::hasher_type_elem_base {
    // this is a bad hasher
    // could multiply with (level() -coarsest_level())*FinestGrid().NumOfVertices()
    // to guarantee unique numbers
    size_t operator()(element_type const& V) const { flat_hasher_type h; return h(V);}
  };
};


template<class OCTREE>
struct grid_types<octree::non_conforming_leafgrid<OCTREE> > 
  : public  octree::nc_leafgrid_grid_types_base<OCTREE> 
{

};


// partial specializations of grid functions

template<class OCTREE, class T>
class grid_function<hierarchical::h_cell_t<
                      octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >,
		    T>
: public grid_function_hash<hierarchical::h_cell_t<
                              octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >,
                            T>
{
  typedef grid_function_hash<hierarchical::h_cell_t<
                               octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >,
                             T> base;
 public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};



template<class OCTREE, class T>
class grid_function<octree::nc_leafgrid_vertex_t<
                       octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >
		    ,T>
: public grid_function_hash<octree::nc_leafgrid_vertex_t<
                                octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >
                            ,T>
{
  typedef grid_function_hash<octree::nc_leafgrid_vertex_t<
                                octree::nc_leafgrid_element_base_t<octree::non_conforming_leafgrid<OCTREE> > >
                             ,T> base;
 public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Hierarchical/non-conforming-leafgrid.C"
#endif


#endif
