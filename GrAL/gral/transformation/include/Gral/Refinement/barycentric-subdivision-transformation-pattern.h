#ifndef GRAL_GB_TRANSFORMATION_BARYCENTRIC_SUBDIVISION_PATTERN_H
#define GRAL_GB_TRANSFORMATION_BARYCENTRIC_SUBDIVISION_PATTERN_H

// $LICENSE_NEC_2005

#include "Gral/Transformation/pattern-data.h"
#include "Gral/Refinement/barycentric-subdivision.h"
#include "Gral/Base/vertex-set.h"

#include "Container/partial-mapping.h"

namespace GrAL {

  template<class GRID, class PATTERN_GRID, class PATTERN_GEOM>
  struct transformation_pattern_config {
    typedef GRID         host_grid_type;
    typedef PATTERN_GRID pattern_grid_type;
    typedef PATTERN_GEOM pattern_geom_type;
  };
  

  template<class CFG>
  class barycentric_subdivision_transformation_pattern {
    typedef barycentric_subdivision_transformation_pattern<CFG> self;
  public:
    typedef typename CFG::host_grid_type host_grid_type;
    typedef grid_types<host_grid_type> hgt;

    enum {dim = hgt::dimension_tag::dim };
    typedef barycentric_subdivision<dim> subdivision_type;

    typedef typename CFG::pattern_grid_type  pattern_grid_type;
    typedef typename CFG::pattern_geom_type  pattern_geom_type;

    typedef transformation_pattern_data_cfg<pattern_grid_type, pattern_geom_type>      pcfg;
    typedef typename subdivision_type::template pattern_data_type<pcfg>                         pattern_data_type;
    typedef mapped_transformation_pattern_data_cfg<host_grid_type, pattern_data_type>  mcfg;
    typedef mapped_transformation_pattern_data<mcfg>                                   mapped_pattern_data_type;
    // mapping pattern orig patch to host patch
    typedef typename mapped_pattern_data_type::vertex_morphism_type vertex_morphism_type;
    
    typedef typename mapped_pattern_data_type::PatternVertex    PatternVertex;
    typedef typename mapped_pattern_data_type::PatternCell      PatternCell;
    typedef typename mapped_pattern_data_type::local_coord_type local_coord_type;

    typedef typename hgt::Cell   HostCell;
    typedef typename hgt::Vertex HostVertex;    

    struct context_type {

      typedef typename hgt::archetype_type host_archetype_type;

      std::vector<ref_ptr<pattern_data_type const> >                                 patterns;
      mutable partial_mapping<host_archetype_type const*, ref_ptr<pattern_data_type const> > trivial_patterns;
      ref_ptr<host_grid_type const>                                                  host_grid;
      // TODO: list containing all archetypes      

      context_type() {}
      context_type(host_grid_type const& hg) : host_grid(hg) {}

      void add_pattern(ref_ptr<pattern_data_type const> p) { patterns.push_back(p); }

      ref_ptr<pattern_data_type const> get_pattern    (HostCell hc) const { return * patterns.begin();}
      ref_ptr<pattern_data_type const> trivial_pattern(HostCell hc) const 
      { 
	host_archetype_type const* A = &(hc.TheGrid().ArchetypeOf(hc));
	if(!trivial_patterns.defined(A)) 
	  trivial_patterns[A] = make_trivial_pattern(hc);
	return trivial_patterns(A);
      }
      ref_ptr<pattern_data_type> make_trivial_pattern(HostCell hc) const 
      {
	return copy_to_ref_ptr(pattern_data_type::trivial_pattern_data(hc));
      }

      typedef typename pattern_data_type::grid_type  pattern_grid_type;
      typedef grid_types<pattern_grid_type>          pgt;

      typedef typename pgt::archetype_type           archetype_type;
      typedef typename pgt::archetype_geom_type      archetype_geom_type;
      typedef typename pgt::Cell                     Cell;
      // These are not correct any more!
      typedef typename pgt::archetype_iterator       archetype_iterator;
      typedef typename pgt::archetype_handle         archetype_handle;
 
      archetype_type      const& ArchetypeOf  (Cell const& c) const { return c.TheGrid().ArchetypeOf(c);}
      archetype_geom_type const& ArchetypeGeom(Cell const& c) const { return c.TheGrid().ArchetypeGeom(c);}

      // FIXME: we must collect the archetypes of all patterns related to host grid.!
      archetype_iterator BeginArchetype()  const { return patterns[0]->orig_patch.BeginArchetype();}
      archetype_iterator EndArchetype()    const { return patterns[0]->orig_patch.EndArchetype();  }
      unsigned           NumOfArchetypes() const { return patterns[0]->orig_patch.NumOfArchetypes();}

    }; // context_type

    typedef typename context_type::archetype_type           archetype_type;
    typedef typename context_type::archetype_geom_type      archetype_geom_type;
    typedef typename context_type::archetype_iterator       archetype_iterator;
    typedef typename context_type::archetype_handle         archetype_handle;


    typedef partial_mapping<host_grid_type const*, ref_ptr<context_type> > context_table_type;

    struct trivial_pattern_tag {};
  private:
    mapped_pattern_data_type   data;
    static context_table_type* context_table;

   static void  init_context_table() { 
      static int count = 0;
      if(count == 0) {
	++count;
	context_table = new context_table_type();
      }
   }

  public:
    barycentric_subdivision_transformation_pattern() {}
    barycentric_subdivision_transformation_pattern(HostCell hc) : data(full_pattern(hc), hc) {}
    barycentric_subdivision_transformation_pattern(HostCell hc, trivial_pattern_tag) 
      : data(trivial_pattern(hc), hc) {}
    barycentric_subdivision_transformation_pattern(HostCell hc, vertex_morphism_type const& p2h) 
      : data(full_pattern(hc), hc, p2h) {}
    
    ref_ptr<pattern_data_type const> full_pattern   (HostCell const& hc) const { return ctxt(hc.TheGrid())->get_pattern(hc);}
    ref_ptr<pattern_data_type const> trivial_pattern(HostCell const& hc) const { return ctxt(hc.TheGrid())->trivial_pattern(hc);}
    
    
    // trivial (identity) refinement pattern
    static self default_pattern(HostCell hc) { return self(hc, trivial_pattern_tag()); }
    static void init(host_grid_type const& hg);
    static void init(ref_ptr<host_grid_type const> hg) { init(*hg);}

    static ref_ptr<context_type const>  ctxt(host_grid_type const& g)                 { return (*context_table)(&g);}
    static ref_ptr<context_type const>  ctxt(ref_ptr<host_grid_type const> g)         { return (*context_table)(&(*g));}
    static ref_ptr<context_type      >  ctxt_r(host_grid_type const& g)               { return (*context_table)(&g);}
    static ref_ptr<context_type      >  ctxt_r(ref_ptr<host_grid_type const> g)       { return (*context_table)(&(*g));}
    
    // this is specific to barycentric_subdivision
    typedef general_vertex_set<host_grid_type> new_vertex_global_identifier;
    new_vertex_global_identifier global_id(PatternVertex v) const 
    {
      typedef typename pattern_data_type::vertex_range_type  corner_index_set;
      corner_index_set corners = data.Pattern()->corners(v);
      new_vertex_global_identifier res; //(corners.size());
      typedef grid_types<corner_index_set> rgt;
      for(typename rgt::VertexIterator v(corners.FirstVertex()); !v.IsDone(); ++v)
	res.push_back(data.orig2host_v(*v).handle());
      res.init();
      return res;
    }

    

    unsigned NumOfNewCells()  const { return data.NumOfNewCells();}
    unsigned NumOfHostCells() const { return data.NumOfHostCells();}
    unsigned NumOfCells()     const { return data.NumOfCells();}

    typedef typename pattern_data_type::CellIterator PatternCellIterator;
    PatternCellIterator FirstCell() const { return data.FirstCell();}
    PatternCellIterator EndCell()   const { return data.EndCell();}


    typedef typename mapped_pattern_data_type::PatternNewVertexIterator       PatternNewVertexIterator;
    typedef typename mapped_pattern_data_type::PatternRemovedVertexIterator   PatternRemovedVertexIterator;


    unsigned NumOfNewVertices()     const { return data.NumOfNewVertices();}
    unsigned NumOfHostVertices()    const { return data.NumOfHostVertices(); }
    unsigned NumOfRemovedVertices() const { return data.NumOfRemovedVertices();}
    unsigned NumOfVertices()        const { return data.NumOfVertices();}

    PatternNewVertexIterator     FirstNewVertex()     const { return data.FirstNewVertex();}
    PatternNewVertexIterator     EndNewVertex()       const { return data.EndNewVertex();}
    PatternRemovedVertexIterator FirstRemovedVertex() const { return data.FirstRemovedVertex();}
    PatternRemovedVertexIterator EndRemovedVertex()   const { return data.EndRemovedVertex();}


    HostCell   host_cell  (PatternVertex v) const { return data.host_cell(v); }
    local_coord_type local_coord(PatternVertex v) const { return data.local_coord(v);}
    bool       old        (PatternVertex v) const { return data.old(v);}
    HostVertex host_vertex(PatternVertex v) const { return data.host_vertex(v);}
  };

  template<class CFG>
  void barycentric_subdivision_transformation_pattern<CFG>::init
  (typename barycentric_subdivision_transformation_pattern<CFG>::host_grid_type const& hg)
  {
    init_context_table();
    (*context_table)[&hg] = copy_to_ref_ptr(context_type(hg));
    ref_ptr<pattern_data_type> full_pattern = new_ref_ptr(new pattern_data_type());
    subdivision_type::init_pattern(*full_pattern);
    ctxt_r(hg)->add_pattern(full_pattern);
  }

  template<class CFG>
  typename barycentric_subdivision_transformation_pattern<CFG>::context_table_type *
  barycentric_subdivision_transformation_pattern<CFG>::context_table;


} // namespace GrAL


#endif
