#ifndef GRAL_GB_TRANSFORMATION_TRANSFORMED_GRID_VIEW_H
#define GRAL_GB_TRANSFORMATION_TRANSFORMED_GRID_VIEW_H



/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"

#include "Utility/ref-ptr.h"
#include "Container/bijective-mapping.h"
#include "Container/range.h"
//#include "Container/constant-mapping.h"


#include <vector>

namespace GrAL { namespace transformed_grid_view {

  /*! \brief Simple configuration for grid_view

  */
  template<class GRID, class GEOM, class PATTERN, class GT = grid_types<GRID> >
  struct cfg {
    typedef GRID grid_type;
    typedef GEOM geom_type;
    typedef GT gt;
    typedef partial_grid_function<typename gt::Vertex, bool> removed_vertex_table_type;
    typedef partial_grid_function<typename gt::Cell,   bool> removed_cell_table_type;

    typedef PATTERN pattern_type;
  };

  /*! \brief Simple configuration for grid_view for pure refinement patterns

 */
  /*  
  template<class GRID, class GEOM, class PATTERN, class GT = grid_types<GRID> >
  struct cfg_pure_refinement {
    typedef GRID grid_type;
    typedef GEOM geom_type;
    typedef GT gt;
    typedef constant<typename gt::Vertex, bool, false>       removed_vertex_table_type;
    typedef partial_grid_function<typename gt::Cell,   bool> removed_cell_table_type;

    typedef PATTERN pattern_type;
  };
  */




  template<class CFG> class vertex_t;
  template<class CFG> class vertex_handle_t;
  template<class CFG> class vertex_iterator_t;
  template<class CFG> class cell_t;
  template<class CFG> class cell_handle_t;
  template<class CFG> class cell_iterator_t;
  template<class CFG> class vertex_on_cell_iterator_t;

  template<class CFG> class grid_view;

  template<class CFG>
  struct grid_types_base_tgv
  {
    typedef grid_view<CFG>               grid_type;

    typedef vertex_handle_t<CFG>         vertex_handle;
    typedef vertex_t<CFG>                Vertex;
    typedef vertex_iterator_t<CFG>       VertexIterator;

    typedef cell_handle_t<CFG>           cell_handle;
    //typedef cell_t<CFG>                  Cell;
    typedef cell_iterator_t<CFG>           Cell;
    typedef cell_iterator_t<CFG>           CellIterator;

    typedef vertex_on_cell_iterator_t<CFG> VertexOnCellIterator;

    typedef int                          new_vertex_handle;

    typedef typename CFG::grid_type      base_grid_type;
    typedef grid_types<base_grid_type>   bgt;
    typedef typename bgt::vertex_handle  base_vertex_handle;

    typedef typename CFG::pattern_type   pattern_type;
    //   typedef grid_types<pattern_type>     pgt;
    typedef pattern_type                 pgt;
    
    typedef typename bgt::dimension_tag dimension_tag;

    typedef typename pattern_type::archetype_type           archetype_type;
    typedef typename pattern_type::archetype_iterator       archetype_iterator;
    typedef typename pattern_type::archetype_handle         archetype_handle;
    typedef typename pattern_type::archetype_geom_type      archetype_geom_type;
  };





  template<class CFG>
  class grid_view {
    typedef grid_types_base_tgv<CFG>       gt;
  public:
    typedef typename CFG::grid_type    base_grid_type;
    typedef typename CFG::geom_type    base_geom_type;
    typedef typename CFG::pattern_type pattern_type;
    typedef typename base_geom_type::coord_type  coord_type;
    typedef typename pattern_type  ::local_coord_type  local_coord_type;
    typedef typename pattern_type:: new_vertex_global_identifier  vertex_identifier_type;

    typedef typename CFG::removed_cell_table_type   removed_cell_table_type;
    typedef typename CFG::removed_vertex_table_type removed_vertex_table_type;

    typedef typename gt::bgt bgt;
    typedef typename bgt::Cell baseCell;
    typedef typename gt::Cell           Cell;
    typedef typename gt::cell_handle    cell_handle;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::vertex_handle  vertex_handle;
    typedef typename gt::VertexIterator VertexIterator;
    typedef typename gt::CellIterator   CellIterator;
    typedef typename gt::new_vertex_handle new_vertex_handle;
  private:
    ref_ptr<base_grid_type const>        g;
    ref_ptr<base_geom_type const>        geom;

    removed_cell_table_type   m_removed_c;
    // may be trivial impl returning false if pattern is pure refinement
    removed_vertex_table_type m_removed_v; 
    
    typedef bijective_mapping<vertex_identifier_type, int> new_vertex_table_type;
    typedef std::vector<coord_type>                        new_vertex_coord_table_type;
    new_vertex_table_type          m_new_vertices;
    new_vertex_coord_table_type    m_new_coords;


    typedef partial_grid_function<baseCell, pattern_type> pattern_table_type;
    typedef typename pattern_table_type::ElementIterator  TransformedCellIterator;
    pattern_table_type m_pattern;

    int m_num_removed_vertices;
    int m_num_removed_cells;
    int m_num_new_cells;
  public:
    grid_view(        base_grid_type const& gg,         base_geom_type const& ggeom) : g(gg), geom(ggeom) { init();}
    grid_view(ref_ptr<base_grid_type const> gg, ref_ptr<base_geom_type const> ggeom) : g(gg), geom(ggeom) { init();}

    void init();

    unsigned NumOfVertices() const { return BaseGrid()->NumOfVertices() + m_new_vertices.size() - m_num_removed_vertices;}
    unsigned NumOfCells()    const { return BaseGrid()->NumOfCells()    + m_num_new_cells       - m_num_removed_cells;}

    unsigned NumOfNewVertices() const { return m_new_vertices.size();}

    VertexIterator FirstVertex() const;
    VertexIterator EndVertex()   const;
    CellIterator   FirstCell() const;
    CellIterator   EndCell()   const;

    void transform_cell(baseCell c) {
      REQUIRE_ALWAYS(! m_removed_c(*c), "Cell " << c.handle() << " already transformed!", 1);
      m_removed_c[*c] = true;
      ++m_num_removed_cells;
      m_pattern  [*c] = pattern_type(*c);
    }

    void transform();

    bool transformed(baseCell const& c) const { return m_pattern.defined(c);}

    ref_ptr<pattern_type const>  pattern(baseCell const& c) const { 
      if(m_pattern.defined(c))
	return  ref_ptr<pattern_type const>(m_pattern(c));
      else
	return new_ref_ptr(new pattern_type(pattern_type::default_pattern(*c)));
    }
    new_vertex_handle new_handle(vertex_identifier_type v) const { 
      return new_vertex_handle(m_new_vertices(v));
    }

    coord_type coord(Vertex const& v) const { 
      return (v.old() 
	      ? BaseGeom()->coord(v.Base())
	      : m_new_coords[v.handle().new_handle()]);
    }

    TransformedCellIterator FirstTransformedCell() const { return m_pattern.FirstElement();}
    TransformedCellIterator EndTransformedCell()   const { return m_pattern.EndElement();}

    ref_ptr<base_grid_type const> BaseGrid() const { return g;}
    ref_ptr<base_geom_type const> BaseGeom() const { return geom;}

   /*@{*/
    /*! \name Archetype handling
      
      \see $GrAL ArchetypedGrid concept
    */
    typedef typename gt::archetype_type           archetype_type;
    typedef typename gt::archetype_iterator       archetype_iterator;
    typedef typename gt::archetype_handle         archetype_handle;
    typedef typename gt::archetype_geom_type      archetype_geom_type;

    typedef typename pattern_type::context_type context_type;
    ref_ptr<context_type const> ctxt() const { return pattern_type::ctxt(BaseGrid());}

    archetype_type   const& ArchetypeOf (Cell const& c) const { return ctxt()->ArchetypeOf (c.PatternCell());}
    //    archetype_type   const& ArchetypeOf (cell_handle c) const { return ctxt()->ArchetypeOf (c.PatternCell());}
    // archetype_handle        archetype_of(cell_handle c) const { return ctxt()->archetype_of(c.PatternCell());}
    archetype_handle        archetype_of(Cell const& c) const { return ctxt()->archetype_of(c.PatternCell());}

    archetype_iterator BeginArchetype()  const { return ctxt()->BeginArchetype();}
    archetype_iterator EndArchetype()    const { return ctxt()->EndArchetype();  }
    unsigned           NumOfArchetypes() const { return ctxt()->NumOfArchetypes();}
    archetype_handle   handle(archetype_iterator a) const { return ctxt()->handle(a);}
    
    archetype_geom_type const& ArchetypeGeom(archetype_handle   a)  const { return ctxt()->ArchetypeGeom(a);}
    archetype_geom_type const& ArchetypeGeom(archetype_iterator a)  const { return ctxt()->ArchetypeGeom(a);}
    archetype_geom_type const& ArchetypeGeom(Cell        const& c)  const { return ctxt()->ArchetypeGeom(c.PatternCell());}
    // archetype_geom_type const& ArchetypeGeom(cell_handle const& c)  const { return ctxt()->ArchetypeGeom(c.PatternCell());}
    /*@}*/

  }; // class grid_view




  template<class CFG>
  class vertex_handle_t {
    typedef vertex_handle_t<CFG> self;

    typedef grid_types_base_tgv<CFG> gt;
    typedef typename gt::base_vertex_handle base_vertex_handle;
    typedef typename gt::new_vertex_handle  new_vertex_handle;

    base_vertex_handle ov;
    new_vertex_handle  nv;
  public:
    vertex_handle_t() : ov(-1), nv(-1) {}
    vertex_handle_t(base_vertex_handle ovv, new_vertex_handle nvv) : ov(ovv), nv(nvv) {}

    base_vertex_handle old_handle() const { return ov;}
    new_vertex_handle  new_handle() const { return nv;}

    bool old() const { return nv < 0;}
    bool operator==(self const& rhs) const { return ov == rhs.ov && nv == rhs.nv;}
    bool operator< (self const& rhs) const { return ov <  rhs.ov || (ov == rhs.ov &&  nv < rhs.nv); }
  };
 

  template<class CFG>
  class vertex_t {
    typedef vertex_t<CFG> self;

    typedef grid_types_base_tgv<CFG> gt;
    typedef typename gt::grid_type     grid_type;
    typedef typename gt::vertex_handle vertex_handle;
    typedef typename gt::bgt           bgt;
    typedef typename bgt::Vertex       baseVertex;
  private:
    ref_ptr<grid_type const> g;
    vertex_handle            v;
  public:
    vertex_t() {}
    vertex_t(grid_type const& gg,         vertex_handle vv) : g(gg), v(vv) {}
    vertex_t(ref_ptr<grid_type const> gg, vertex_handle vv) : g(gg), v(vv) {}

    bool old() const { return v.old();}
    vertex_handle handle() const { return v;}
    ref_ptr<grid_type const> TheGrid() const { return g;}
    
    baseVertex Base() const { REQUIRE(old(), "", 1); return baseVertex(* TheGrid()->BaseGrid(), v.old_handle());}
    
    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator< (self const& rhs) const { return v <  rhs.v;}

  }; // class vertex



  template<class CFG> 
  class vertex_iterator_t {
    typedef vertex_iterator_t<CFG> self;

    typedef grid_types_base_tgv<CFG>     gt;
  public:
    typedef typename gt::grid_type       grid_type;
    typedef typename gt::vertex_handle   vertex_handle;
    typedef typename gt::new_vertex_handle  new_vertex_handle;
    typedef typename gt::Vertex          Vertex;
    typedef typename gt::bgt             bgt;
    typedef typename bgt::VertexIterator baseVertexIterator;
  private:
    ref_ptr<grid_type const> g;
    baseVertexIterator       ov;
    new_vertex_handle        nv;
  public:
    vertex_iterator_t(grid_type const& gg) : g(gg),  ov(* gg.BaseGrid()), nv(-1) 
    { advance_till_valid(); }
    vertex_iterator_t(grid_type const& gg, baseVertexIterator ovv, new_vertex_handle nvv) : g(gg), ov(ovv), nv(nvv) 
    { advance_till_valid();}

    self& operator++()   { 
      if(nv >= 0) 
	++nv; 
      else {
	++ov;
	advance_till_valid();
      } 
      return *this;
    }
    Vertex operator*() const { return Vertex(g,handle()); }
    bool   IsDone() const { return (ov.IsDone() && nv >= new_vertex_handle(TheGrid()->NumOfNewVertices())); }
    vertex_handle handle() const { return (nv >= 0 ? vertex_handle(-1, nv) : vertex_handle(ov.handle(), nv));}
    ref_ptr<grid_type const> TheGrid() const { return g;}
  private:
    void advance_till_valid() {
      if(nv < 0) 
	while(!ov.IsDone() && TheGrid()->m_removed_v(*ov))
	  ++ov;
      if(ov.IsDone())
	// switch to new vertices
	nv = 0;
    }
  }; // class vertex_iterator




  template<class CFG>
  class cell_handle_t {
    typedef cell_handle_t<CFG> self;

    typedef grid_types_base_tgv<CFG> gt;
    typedef typename gt::bgt           bgt;
    typedef typename bgt::cell_handle          base_cell_handle;

    typedef typename gt::pattern_type  pattern_type;
    typedef grid_types<typename pattern_type::pattern_grid_type> pgt;
    typedef typename pgt::cell_handle  pattern_cell_handle;

    base_cell_handle    c;
    pattern_cell_handle pc;
  public:
    cell_handle_t() : c(-1), pc(-1) {}
    cell_handle_t(base_cell_handle cc, pattern_cell_handle pcc) : c(cc), pc(pcc) {}
    
    bool changed() const { return pc >= 0;}
    bool operator==(self const& rhs) const { return c == rhs.c && pc == rhs.pc;}
    bool operator< (self const& rhs) const { return c <  rhs.c || (c == rhs.c &&  pc < rhs.pc); }
  };


  template<class CFG>
  class cell_iterator_t {
    typedef cell_iterator_t<CFG>         self;
    typedef grid_types_base_tgv<CFG>     gt;
  public:
    typedef typename gt::grid_type     grid_type;
    typedef typename gt::bgt           bgt;
    typedef typename bgt::CellIterator baseCellIterator;
    typedef typename gt::pattern_type  pattern_type;
    typedef typename pattern_type::PatternCell          patternCell;        
    typedef typename pattern_type::PatternCellIterator  patternCellIterator;
    typedef grid_types<typename pattern_type::pattern_grid_type>   pgt;
    typedef typename pgt::cell_handle                   pattern_cell_handle;

    typedef typename gt::cell_handle   cell_handle;
    typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
  private:
    ref_ptr<grid_type const> g;
    baseCellIterator         c;
    patternCellIterator      pc;

  public:
    cell_iterator_t() {}
    cell_iterator_t(grid_type const& gg) : g(gg) 
    { 
      c = TheGrid()->BaseGrid()->FirstCell();
      init_pc();
    }
    cell_iterator_t(grid_type const& gg, baseCellIterator cc) : g(gg), c(cc) { init_pc();}
    cell_iterator_t(grid_type const& gg, baseCellIterator cc, patternCellIterator pcc) 
      : g(gg), c(cc), pc(pcc) {}

    self& operator++() { advance(); return *this; }
    self const& operator*() const { return *this;}
    bool IsDone() { return c.IsDone();}
    //cell_handle handle() const { return cell_handle(c.handle(), (!pc.IsDone() ? pc.handle() : pattern_cell_handle(-1))); }
    cell_handle handle() const { return cell_handle(c.handle(), pc.handle()); }

    unsigned NumOfVertices() const { return (*pc).NumOfVertices();}
    VertexOnCellIterator FirstVertex() const;
    VertexOnCellIterator EndVertex()   const;

    ref_ptr<grid_type    const> TheGrid()    const { return g;}
    ref_ptr<pattern_type const> ThePattern() const { return  ref_ptr<pattern_type const>(TheGrid()->pattern(*c)); }
    patternCell PatternCell() const { return *pc;}

    bool operator==(self const& rhs) const { return c == rhs.c && pc == rhs.pc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    bool operator< (self const& rhs) const { return c <  rhs.c || (c == rhs.c && pc < rhs.pc);}

  private:
    void init_pc();
    void advance();
  };



  template<class CFG>
  class vertex_on_cell_iterator_t {
    typedef vertex_on_cell_iterator_t<CFG> self;
    typedef grid_types_base_tgv<CFG>       gt;
  public:
    typedef typename gt::grid_type         grid_type;
    typedef typename gt::Cell              Cell;
    typedef typename gt::Vertex            Vertex;
    typedef typename gt::vertex_handle     vertex_handle;
    typedef typename gt::new_vertex_handle new_vertex_handle;
    typedef typename gt::bgt               bgt;
    typedef typename bgt::vertex_handle    old_vertex_handle;
    typedef typename gt::pattern_type      pattern_type;
    typedef grid_types<typename pattern_type::pattern_grid_type>   pgt;
    typedef typename pgt::VertexOnCellIterator  patternVertexOnCellIterator;
  private:
    Cell                         c;
    patternVertexOnCellIterator vc;
  public:
    vertex_on_cell_iterator_t(Cell const& cc) : c(cc), vc(c.PatternCell().FirstVertex()) {}
    vertex_on_cell_iterator_t(Cell const& cc, patternVertexOnCellIterator vcc) 
      : c(cc), vc(vcc) {}
    
    self& operator++() { ++vc; return *this;}
    Vertex operator*() const { return Vertex(TheGrid(), handle());}
    bool IsDone() const { return vc.IsDone();}
    vertex_handle handle() const { 
      return (old() 
	      ? vertex_handle(old_handle(), new_vertex_handle(-1))
	      : vertex_handle(old_vertex_handle(-1), new_handle()));
    }

    ref_ptr<grid_type const> TheGrid() const { return c.TheGrid();}

    bool old() const { return c.ThePattern()->old(*vc);}

    bool operator==(self const& rhs) const { return vc == rhs.vc;}
    bool operator!=(self const& rhs) const { return vc != rhs.vc;}
    bool operator< (self const& rhs) const { return vc <  rhs.vc;}

  private:
    new_vertex_handle new_handle() const { return  TheGrid()->new_handle(c.ThePattern()->global_id(*vc));}
    old_vertex_handle old_handle() const { return  c.ThePattern()->host_vertex(*vc).handle();}

  };



  template<class CFG>
  inline
  typename cell_iterator_t<CFG>::VertexOnCellIterator cell_iterator_t<CFG>::FirstVertex() const 
  { return VertexOnCellIterator(*this); }


  template<class CFG>
  inline
  typename cell_iterator_t<CFG>::VertexOnCellIterator cell_iterator_t<CFG>::EndVertex() const 
  { return VertexOnCellIterator(*this, PatternCell().EndVertex()); }


  template<class CFG>
  inline void cell_iterator_t<CFG>::init_pc() 
    {
      if(!c.IsDone()) {
	pc = ThePattern()->FirstCell();
      }
    }

  template<class CFG>
  inline void cell_iterator_t<CFG>::advance()
  { 
    // if(!pc.IsDone())
    ++pc; 
    if(pc.IsDone()) {
      ++c;
      init_pc();
    }
  }



  template<class CFG>
  typename grid_view<CFG>::VertexIterator grid_view<CFG>::FirstVertex() const 
  { return VertexIterator(*this); }

  template<class CFG>
  typename grid_view<CFG>::VertexIterator grid_view<CFG>::EndVertex() const 
  { return VertexIterator(*this, BaseGrid()->EndVertex(), NumOfNewVertices()); }

  template<class CFG>
  typename grid_view<CFG>::CellIterator grid_view<CFG>::FirstCell() const 
  { return CellIterator(*this); }

  template<class CFG>
  typename grid_view<CFG>::CellIterator grid_view<CFG>::EndCell() const 
  { return CellIterator(*this, BaseGrid()->EndCell()); }



  template<class CFG>
  void grid_view<CFG>::init() {
    m_num_removed_vertices = 0;
    m_num_removed_cells    = 0;
    m_num_new_cells        = 0;

    m_removed_c.init(g, false);
    m_removed_v.init(g, false);
    m_pattern.set_grid(g);

    pattern_type::init(BaseGrid());
  }


  template<class CFG>
  void grid_view<CFG>::transform() {
    // process all refined/transformed patches
    for(TransformedCellIterator c = FirstTransformedCell(); c != EndTransformedCell(); ++c) {
      pattern_type const& p = m_pattern(*c);
      m_num_new_cells += p.NumOfCells();

      // remove p's removed vertices
      for(typename pattern_type::PatternRemovedVertexIterator rv = p.FirstRemovedVertex(); 
	  rv != p.EndRemovedVertex(); ++rv) {
	m_removed_v[*rv] = true;
	++m_num_removed_vertices;
      }

      // add p's new vertices
      for(typename pattern_type::PatternNewVertexIterator v = p.FirstNewVertex(); v != p.EndNewVertex(); ++v) {
	if(m_new_vertices.undefined(p.global_id(*v))) {
	  // could this be done elsewhere, eg geom_view<CFG> ?
	  // we could just store (host_cell(v), local_coord(v))
	  coord_type new_coord = BaseGeom()->coord(p.host_cell(*v),
						   p.local_coord(*v));
	  m_new_coords.push_back(new_coord);
	  m_new_vertices[p.global_id(*v)] = m_new_coords.size() -1;
	}
      }

      // add p's new cells 
      // default: cell iterator take care of it ....
    }
      
  }

} // namespace transformed_grid_view 
  
  template<class CFG>
  class grid_types<transformed_grid_view::grid_view<CFG> > 
    : public grid_types_base<transformed_grid_view::grid_types_base_tgv<CFG> > 
  { };


  template<class CFG>
  class element_traits<transformed_grid_view::vertex_t<CFG> >
    : public element_traits_vertex_base<transformed_grid_view::grid_view<CFG> >
  {
  public:
    typedef element_traits_vertex_base<transformed_grid_view::grid_view<CFG> >  base;
    typedef typename base::element_type   element_type;
    typedef typename base::handle_type    handle_type;
    typedef grid_types<transformed_grid_view::grid_view<CFG> > gt; // gt of view
    typedef typename gt::bgt                         bgt; // gt of GRID
    typedef element_traits<typename bgt::Vertex>     bet;
    typedef typename bet::hasher_type                base_hasher_type;

    struct hasher_type : public base::hasher_type_elem_base {
      unsigned operator()(element_type const& v) const {
	base_hasher_type h;
	return(v.old() ? h(v.Base()) : unsigned(v.handle().new_handle() + v.TheGrid()->BaseGrid()->NumOfVertices()));
      }
    };
  };

  template<class CFG>
  class element_traits<transformed_grid_view::cell_iterator_t<CFG> >
    : public element_traits_cell_base<transformed_grid_view::grid_view<CFG> >
  {};


  template<class CFG, class T>
  class grid_function<transformed_grid_view::vertex_t<CFG>, T>
    : public grid_function_hash<transformed_grid_view::vertex_t<CFG>, T>
  {
    typedef  grid_function_hash<transformed_grid_view::vertex_t<CFG>, T> base;
  public:
    typedef typename base::grid_type grid_type;
    
    grid_function() {}
    grid_function(grid_type const& g)         : base(g) {}
    grid_function(ref_ptr<grid_type const> g) : base(g) {}
    grid_function(grid_type const& g,
		  T         const& t0) : base(g,t0) {}
    grid_function(ref_ptr<grid_type const> g,
		  T         const& t0) : base(g,t0) {}
  };

  template<class CFG, class T>
  class grid_function<transformed_grid_view::cell_iterator_t<CFG>, T>
    : public grid_function_hash<transformed_grid_view::cell_iterator_t<CFG>, T>
  {
    typedef  grid_function_hash<transformed_grid_view::cell_iterator_t<CFG>, T> base;
  public:
    typedef typename base::grid_type grid_type;
    
    grid_function() {}
    grid_function(grid_type const& g) : base(g) {}
    grid_function(ref_ptr<grid_type const> g) : base(g) {}
    grid_function(grid_type const& g,
		  T         const& t0) : base(g,t0) {}
    grid_function(ref_ptr<grid_type const> g,
		  T         const& t0) : base(g,t0) {}
  };



} // namespace GrAL


namespace STDEXT {
  template<class T> struct hash;
  
  template<class CFG>
  struct hash<GrAL::transformed_grid_view::vertex_t<CFG> > {
    typedef GrAL::transformed_grid_view::vertex_t<CFG> key_type;
    typedef GrAL::transformed_grid_view::vertex_t<CFG> argument_type;
    typedef size_t                                     result_type;
    typedef GrAL::element_traits<key_type> et;
    typedef typename et::hasher_type hasher_type;
    result_type operator()(key_type v) const { hasher_type h; return h(v);}
  };

}
#endif
