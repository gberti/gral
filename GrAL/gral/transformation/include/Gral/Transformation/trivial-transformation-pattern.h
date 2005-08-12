#ifndef GRAL_GB_TRIVIAL_TRANSFORMATION_PATTERN_H
#define GRAL_GB_TRIVIAL_TRANSFORMATION_PATTERN_H

// $LICENSE_NEC_2005

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Iterators/single-element-iterator.h"

namespace GrAL {

  /*! \brief Trivial (identical) transformation pattern.

  \ingroup transformation_pattern
  
  This pattern simply provides an identical copy of the underlying cell.
  It can be useful if a pattern is formally required, or for testing.
  */
  template<class GRID>
  class trivial_transformation_pattern {
    typedef  trivial_transformation_pattern<GRID> self;
  public:
    typedef GRID pattern_grid_type;
    typedef grid_types<pattern_grid_type>          pgt;
    typedef typename pgt::archetype_type           archetype_type;
    typedef typename pgt::archetype_iterator       archetype_iterator;
    typedef typename pgt::archetype_handle         archetype_handle;
    typedef typename pgt::archetype_geom_type      archetype_geom_type;
    typedef archetype_geom_type                    pattern_geom_type;
    typedef typename pattern_geom_type::coord_type local_coord_type;
    
    // typedef typename pgt::Vertex      vertex_type;
    // typedef typename pgt::Cell        cell_type;
    typedef typename pgt::Vertex      PatternVertex;
    typedef typename pgt::Cell        PatternCell;
    typedef typename pgt::cell_handle pattern_cell_handle;


    typedef GRID host_grid_type;
    typedef grid_types<GRID> hgt;
    typedef typename hgt::Vertex HostVertex;
    typedef typename hgt::Cell   HostCell;

    typedef int  new_vertex_global_identifier;
    typedef single_element_iterator<PatternVertex>     PatternNewVertexIterator;
    typedef single_element_iterator<HostVertex>        PatternRemovedVertexIterator;
    // typedef typename hgt::VertexOnCellIterator         old_vertex_iterator;
    typedef typename pgt::VertexOnCellIterator         PatternVertexOnCellIterator;

    // typedef old_vertex_iterator                      VertexIterator;
    typedef single_element_iterator<PatternCell>     PatternCellIterator;
    
    struct context_type {
    public:
      typedef trivial_transformation_pattern<GRID> pattern_type;
      typedef pattern_type pgt;
      typedef typename pgt::host_grid_type           host_grid_type;
      typedef typename pgt::archetype_type           archetype_type;
      typedef typename pgt::archetype_iterator       archetype_iterator;
      typedef typename pgt::archetype_handle         archetype_handle;
      typedef typename pgt::archetype_geom_type      archetype_geom_type;
    private:
      ref_ptr<host_grid_type const> g;
    public:
      context_type(host_grid_type const& gg) : g(gg) {}
      context_type(ref_ptr<host_grid_type const> gg) : g(gg) {}

      archetype_type   const& ArchetypeOf (PatternCell const& cc) const { return g->ArchetypeOf (cc);}
      archetype_type   const& ArchetypeOf (pattern_cell_handle cc) const { return g->ArchetypeOf (cc);}
      archetype_handle        archetype_of(pattern_cell_handle cc) const { return g->archetype_of(cc);}
      archetype_handle        archetype_of(PatternCell const& cc) const { return g->archetype_of(cc);}
      
      archetype_iterator BeginArchetype()  const { return g->BeginArchetype();}
      archetype_iterator EndArchetype()    const { return g->EndArchetype();  }
      unsigned           NumOfArchetypes() const { return g->NumOfArchetypes();}
      archetype_handle   handle(archetype_iterator a) const { return g->handle(a);}
    
      archetype_geom_type const& ArchetypeGeom(archetype_handle   a)   const { return g->ArchetypeGeom(a);}
      archetype_geom_type const& ArchetypeGeom(archetype_iterator a)   const { return g->ArchetypeGeom(a);}
      archetype_geom_type const& ArchetypeGeom(PatternCell        const& cc)  const { return g->ArchetypeGeom(cc);}
      archetype_geom_type const& ArchetypeGeom(pattern_cell_handle const& cc)  const { return g->ArchetypeGeom(cc);}
    };


  private:
    HostCell c;
  public:
    // a possible extension could be a permuation vector of the vertices of c.
    // Together with a different archetype, this would permit to change just the
    // archetype types of a grid.
    trivial_transformation_pattern(HostCell const& cc) : c(cc) {}

    trivial_transformation_pattern() {}

    static self default_pattern(HostCell const& cc) { return  self(cc);}

    static void init(host_grid_type const&) {}
    static void init(ref_ptr<host_grid_type const>) {}

    static ref_ptr<context_type const>  ctxt(host_grid_type const& g)         { return new_ref_ptr(new context_type(g)); }
    static ref_ptr<context_type const>  ctxt(ref_ptr<host_grid_type const> g) { return new_ref_ptr(new context_type(g)); }


    unsigned NumOfNewCells () const { return 1;}
    unsigned NumOfHostCells() const { return 1;}
    unsigned NumOfCells()     const { return NumOfNewCells();}

    unsigned NumOfNewVertices()     const { return 0;}
    unsigned NumOfHostVertices()    const { return c.NumOfVertices();}
    unsigned NumOfRemovedVertices() const { return 0;}
    unsigned NumOfVertices()        const { return NumOfHostVertices();}

    ref_ptr<host_grid_type    const> HostGrid()    const { return ref_ptr<host_grid_type    const>(c.TheGrid());}
    ref_ptr<pattern_grid_type const> PatternGrid() const { return ref_ptr<pattern_grid_type const>(c.TheGrid());}

    new_vertex_global_identifier global_id(PatternVertex) const { return 0;}
    HostCell    host_cell  (PatternVertex)   const { return c;}
    local_coord_type  local_coord(PatternVertex)   const { return local_coord_type(0.0);}
    bool        old        (PatternVertex)   const { return true;}
    HostVertex  host_vertex(PatternVertex v) const { return v;}

    PatternCellIterator FirstCell() const { return PatternCellIterator(c);}
    PatternCellIterator EndCell()   const { return PatternCellIterator(); }


    PatternNewVertexIterator     FirstNewVertex()     const { return  PatternNewVertexIterator();}
    PatternNewVertexIterator     EndNewVertex()       const { return  PatternNewVertexIterator();}
    PatternRemovedVertexIterator FirstRemovedVertex() const { return  PatternRemovedVertexIterator();}
    PatternRemovedVertexIterator EndRemovedVertex()   const { return  PatternRemovedVertexIterator();}
  };

} // namespace GrAL

#endif
