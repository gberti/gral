#ifndef GRAL_GB_TRANSFORMATION_PATTERN_DATA_H
#define GRAL_GB_TRANSFORMATION_PATTERN_DATA_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Base/grid-morphism.h"
#include "Gral/Base/element-grid.h"
#include "Geometry/point-traits.h"

namespace GrAL {
 
  template<class GRID, class GEOM>
  struct transformation_pattern_data_cfg {
    typedef GRID grid_type;
    typedef GEOM geom_type;
  };

  // per pattern data
  template<class CFG>
  class transformation_pattern_data {
    typedef transformation_pattern_data<CFG> self;
  public:
    typedef typename CFG::grid_type               grid_type;
    typedef typename CFG::geom_type               geom_type;
    typedef typename geom_type::coord_type        coord_type;
    typedef enumerated_vertex_range<grid_type>    vertex_range_type;
    typedef vertex_morphism<grid_type, grid_type> vertex_morphism_type;

    typedef grid_types<grid_type> gt;
    typedef typename gt::Vertex   Vertex;
    typedef typename gt::Cell     Cell;
    typedef typename gt::CellIterator CellIterator;
    typedef grid_types<vertex_range_type> rgt;
    typedef typename rgt::VertexIterator  NewVertexIterator;
    typedef typename rgt::VertexIterator  RemovedVertexIterator;


    // the original patch - in most (refinement) cases, a single cell.
    grid_type                   orig_patch;
    geom_type                   orig_geom;
    grid_function<Vertex, bool> removed_vertex;   // true if the vertex is not present in transformed patch
    vertex_range_type           removed_vertices; // list of all vertices of orig_patch not present in transformed_patch

    // the transformed patch
    grid_type                   transformed_patch;
    geom_type                   transformed_geom;
    grid_function<Vertex, bool> new_vertex;   // true if the vertex has been added in transformed_ patch
    vertex_range_type           new_vertices; // list of all new vertices in transformed_patch
    
    // mapping of new vertices of transformed_patch  
    // to containing original cell + local coordinates;
    struct local_coord_type {
      Cell       cell; 
      coord_type coord;
      local_coord_type() {}
      local_coord_type(Cell c, coord_type x) : cell(c), coord(x) {}
    };
    grid_function<Vertex, local_coord_type> local; 

    coord_type coord    (Vertex v) const { return local(v).coord;}
    Cell       orig_cell(Vertex v) const { return local(v).cell;}

    vertex_morphism_type transformed2orig; // mapping of copied vertices
    
    template<class CELL>
    static self trivial_pattern_data(CELL const& c);

    void init_ranges();
  };


  template<class CFG>
  void transformation_pattern_data<CFG>::init_ranges() 
  {
    new_vertices.set_grid(transformed_patch);
    removed_vertices.set_grid(orig_patch);
    new_vertex.init(transformed_patch, false);
    removed_vertex.init(orig_patch, false);
  }





  template<class CFG>
  template<class CELL>
  transformation_pattern_data<CFG> transformation_pattern_data<CFG>::trivial_pattern_data(CELL const& c)
  {
    self res;
    // typedef element_grid<CELL>  se_grid_type;
    typedef element_traits<CELL>               et;
    typedef grid_types<typename et::grid_type> cgt;
    typedef enumerated_subrange<typename cgt::grid_type> se_grid_type;
    se_grid_type c_grid(c.TheGrid());
    c_grid.push_back(c);
    
    typedef vertex_morphism<typename cgt::grid_type, grid_type> se_vertex_morphism;
    se_vertex_morphism Phi_orig (c.TheGrid(), res.orig_patch);
    se_vertex_morphism Phi_trans(c.TheGrid(), res.transformed_patch);

    ConstructGrid0(res.orig_patch,        c_grid, Phi_orig);
    ConstructGrid0(res.transformed_patch, c_grid, Phi_trans);

    res.init_ranges();    

    res.orig_geom       .set_grid(res.orig_patch);
    res.transformed_geom.set_grid(res.transformed_patch);
    res.transformed2orig.set_grids(res.transformed_patch, res.orig_patch);
    res.local.set_grid(res.transformed_patch);

    for(typename cgt::VertexOnCellIterator vc(c); !vc.IsDone(); ++vc) {
      res.orig_geom       .coord(Phi_orig(*vc))  = convert_point<coord_type>(c.TheGrid().ArchetypeGeom(c).coord(vc.local_handle()));
      res.transformed_geom.coord(Phi_trans(*vc)) = convert_point<coord_type>(c.TheGrid().ArchetypeGeom(c).coord(vc.local_handle()));
      res.transformed2orig[Phi_trans(*vc)] = Phi_orig(*vc);
      res.local[Phi_trans(*vc)] = local_coord_type(* res.orig_patch.FirstCell(), res.orig_geom.coord(Phi_orig(*vc)));
    }
    
    return res;  
  }
  

  template<class HOST_GRID, class PATTERN_DATA_TYPE>
  struct mapped_transformation_pattern_data_cfg {
    typedef HOST_GRID         grid_type;
    // PATTERN_DATA_TYPE will be transformation_pattern_data<cfg> + pattern-specific stuff.
    typedef PATTERN_DATA_TYPE pattern_data_type;
  };

  // per host patch data
  template<class CFG>
  class mapped_transformation_pattern_data {
  public:
    typedef typename CFG::pattern_data_type pattern_data_type;
    
    typedef typename pattern_data_type::grid_type pattern_grid_type;
    typedef typename CFG::grid_type               host_grid_type;
    typedef grid_types<pattern_grid_type>         pgt;
    typedef grid_types<host_grid_type>            hgt;
    typedef typename hgt::Vertex                  HostVertex;
    typedef typename hgt::Cell                    HostCell;

    typedef typename pattern_data_type::coord_type local_coord_type;
    typedef typename pattern_data_type::Vertex                PatternVertex;
    typedef typename pattern_data_type::Cell                  PatternCell;
    typedef typename pattern_data_type::CellIterator          PatternCellIterator;
    typedef typename pattern_data_type::NewVertexIterator     PatternNewVertexIterator;
    typedef typename pattern_data_type::RemovedVertexIterator PatternRemovedVertexIterator;

    typedef cell_morphism  <pattern_grid_type, host_grid_type> cell_morphism_type;
    typedef vertex_morphism<pattern_grid_type, host_grid_type> vertex_morphism_type;

  private:
   
    ref_ptr<pattern_data_type const> pattern;

    // Mappings from pattern to host
    //                                   t2o      o2h
    // mapping old to host: transformed  --> orig --> host
    //                                  o2h  
    // mapping removed to host: removed --> host
     
    cell_morphism_type   orig2host_c;
    vertex_morphism_type orig2host_v;
  public:
    mapped_transformation_pattern_data() {}
    // use implicit vertex mapping given by VertexOnCellIterator on c
    mapped_transformation_pattern_data(ref_ptr<pattern_data_type const> p, HostCell hc)
      : pattern(p),
	orig2host_c(p->orig_patch, hc.TheGrid()),
	orig2host_v(p->orig_patch, hc.TheGrid())
    {
      REQUIRE(OrigPatch()->NumOfCells() == 1, "", 1);
      //      orig2host_c[* (OrigPatch()->FirstCell())] = hc;
      orig2host_c[OrigPatch()->FirstCell().handle()] = hc.handle();
      REQUIRE(orig2host_c(* (OrigPatch()->FirstCell())) == hc, "", 1);
      // compute default mapping of host patch
      // FIXME:  Probably we should compute an isomorphism here ... archetypes may differ!
      PatternCell pc = *OrigPatch()->FirstCell();
      typename pgt::VertexOnCellIterator pvc(pc);
      typename hgt::VertexOnCellIterator hvc(hc); //orig2host_c(pc));
      for( ; !hvc.IsDone(); ++hvc, ++pvc)
	orig2host_v[*pvc] = *hvc;
    }
    
    mapped_transformation_pattern_data(ref_ptr<pattern_data_type const> p, HostCell hc, vertex_morphism_type p2h)
      : pattern(p), 
	orig2host_c(p->orig_patch, hc.TheGrid()), 
	orig2host_v(p2h)
    {
      REQUIRE(OrigPatch()->NumOfCells() == 1, "", 1);
      orig2host_c[* (OrigPatch()->FirstCell())] = hc;
    }
    
    // most general case: pattern operates on a cell range on the host grid.
    // Isomorphism of pattern orig patch to host grid patch is given by cell order in host_patch
    // and by p2h. We should check that this is indeed an isomorphism.
    /*
    template<class CELLRANGE>
    mapped_transformation_pattern_data(ref_ptr<pattern_data_type const> p, 
				       CELLRANGE const& host_patch, 
				       vertex_morphism_type p2h);
    */

    ref_ptr<pattern_data_type const> Pattern() const { return pattern;}

    ref_ptr<pattern_grid_type const> TransformedPatch() const { return const_ref_to_ref_ptr(pattern->transformed_patch);}
    ref_ptr<pattern_grid_type const> OrigPatch()        const { return const_ref_to_ref_ptr(pattern->orig_patch);}
 
    unsigned NumOfNewCells()  const { return TransformedPatch()->NumOfCells();}
    unsigned NumOfHostCells() const { return OrigPatch()->NumOfCells();}
    unsigned NumOfCells()     const { return NumOfNewCells();}
  
    PatternCellIterator FirstCell() const { return TransformedPatch()->FirstCell(); }
    PatternCellIterator EndCell()   const { return TransformedPatch()->EndCell();}

    PatternNewVertexIterator     FirstNewVertex()     const { return pattern->new_vertices.FirstVertex();}
    PatternNewVertexIterator     EndNewVertex()       const { return pattern->new_vertices.EndVertex();}
    PatternRemovedVertexIterator FirstRemovedVertex() const { return pattern->removed_vertices.FirstVertex();}
    PatternRemovedVertexIterator EndRemovedVertex()   const { return pattern->removed_vertices.EndVertex();}
  
    unsigned NumOfNewVertices()     const { return pattern->new_vertices.NumOfVertices();}
    unsigned NumOfHostVertices()    const { return OrigPatch()->NumOfVertices();}
    unsigned NumOfRemovedVertices() const { return pattern->removed_vertices.NumOfVertices();}
    unsigned NumOfVertices()        const { return TransformedPatch()->NumOfVertices();}


    HostCell   host_cell  (PatternVertex v) const
    { return orig2host_c(pattern->local(v).cell);}
    local_coord_type local_coord(PatternVertex v) const 
    { return pattern->local(v).coord; }
    bool old(PatternVertex v) const { return ! pattern->new_vertex(v);}
    HostVertex host_vertex(PatternVertex v) const 
    { REQUIRE(old(v), "", 1); return orig2host_v(pattern->transformed2orig(v));}

    
  }; // mapped_transformation_pattern_data

} // namespace GrAL

#endif
