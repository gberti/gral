#ifndef GRAL_GB_CARTESIANND_CARTESIANND_H
#define GRAL_GB_CARTESIANND_CARTESIANND_H

// $LICENSE_NEC

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Container/index-map-nd.h"
#include "Container/range.h"
#include "Container/combinatorial.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/partial-grid-function-hash.h"
#include "Gral/Base/archetype-0D.h"

#include "Gral/Grids/CartesianND/direction-maps.h"


#include <vector>
#include <iostream>

#include <cmath>



namespace cartesiannd {
  
  //-------------- forward declarations ---------------

  template<class CARTGRID, unsigned K>
  class element_handle_t;
  template<class CARTGRID, unsigned K>
  class sequence_iterator_t;
  template<class CARTGRID, unsigned K, unsigned M>
  class downward_inc_iterator_t;
  template<unsigned DIM>
  class subrange;
  template<unsigned DIM>
  class grid;
  template<class CARTGRID>
  class archetype_t;
  

  template<class GRID,   unsigned D, unsigned CD>  struct element_type 
  { typedef sequence_iterator_t<GRID, D> type; };
  template<unsigned DIM, unsigned D, unsigned CD>  struct element_type<subrange<DIM>, D, CD> 
  { typedef sequence_iterator_t<grid<DIM>, D> type; };

  template<class CARTGRID, unsigned K> struct element_handle_type
  { typedef element_handle_t<CARTGRID, K>  type; };
  template<unsigned DIM, unsigned K> struct element_handle_type<subrange<DIM>, K>
  { typedef element_handle_t<grid<DIM>, K> type;};

  template<class CARTGRID, unsigned K, unsigned M>
  struct incidence_iterator_type 
  { typedef downward_inc_iterator_t<CARTGRID,K,M> type;};


  template<class CARTGRID, unsigned DIM>
  struct archetype_of_grid_aux {
    typedef archetype_t<CARTGRID> type;
  };
  /*
  template<class CARTGRID, 2>
  struct archetype_of_grid_aux {
    typedef polygon1d::polygon type;
  };
  */

  template<class CARTGRID>
  struct archetype_of_grid_aux<CARTGRID,1> {
    typedef archetype_0D::archetype_t type;
  };

  template<class CARTGRID>
  struct archetype_of_grid_aux<CARTGRID,0> {
    // not quite correct ... could define a type empty_grid with dimension -1.
    typedef grid<0> type;
  };

  template<class CARTGRID>
  struct archetype_of_grid : public archetype_of_grid_aux<CARTGRID, CARTGRID::dimension> {};



  template<class CARTGRID, unsigned DIM>
  struct grid_types_base
  {
    enum { dimension = DIM};
    typedef grid_dim_tag<dimension> dimension_tag;

    //  typedef element_handle_t   <CARTGRID, 0>                vertex_handle;
    typedef typename element_handle_type<CARTGRID, 0>::type vertex_handle;
    typedef sequence_iterator_t<CARTGRID, 0>                VertexIterator;
    typedef typename element_type<CARTGRID, 0, DIM>::type   Vertex;

    //    typedef element_handle_t   <CARTGRID, 1>                edge_handle;
    typedef typename element_handle_type<CARTGRID, 1>::type edge_handle;
    typedef sequence_iterator_t<CARTGRID, 1>                EdgeIterator;
    typedef typename element_type<CARTGRID, 1, DIM-1>::type Edge;

    // typedef element_handle_t   <CARTGRID, 2>                face_handle;
    typedef typename element_handle_type<CARTGRID, 2>::type face_handle;
    typedef sequence_iterator_t<CARTGRID, 2>                FaceIterator;
    typedef typename element_type<CARTGRID, 2, DIM-2>::type Face;

    // typedef element_handle_t   <CARTGRID, DIM-1>            facet_handle;
    typedef typename element_handle_type<CARTGRID, DIM-1>::type facet_handle;
    typedef sequence_iterator_t<CARTGRID, DIM-1>            FacetIterator;
    typedef typename element_type<CARTGRID, DIM-1, 1>::type Facet;

    // typedef element_handle_t   <CARTGRID, DIM>              cell_handle;
    typedef typename element_handle_type<CARTGRID, DIM>::type cell_handle;
    typedef sequence_iterator_t<CARTGRID, DIM>              CellIterator;
    typedef typename element_type<CARTGRID, DIM, 0>::type   Cell;

    typedef typename incidence_iterator_type<CARTGRID, DIM, 0>    ::type VertexOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 1>    ::type EdgeOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 2>    ::type FaceOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM-1>::type FacetOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM>  ::type CellOnCellIterator;

    typedef typename incidence_iterator_type<CARTGRID, 1, 0>      ::type VertexOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, 2>      ::type FaceOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, DIM-1>  ::type FacetOnEdgeIterator;
    typedef typename incidence_iterator_type<CARTGRID, 1, DIM>    ::type CellOnEdgeIterator;

    typedef typename incidence_iterator_type<CARTGRID, 2, 0>      ::type VertexOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, 1>      ::type EdgeOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, DIM-1>  ::type FacetOnFaceIterator;
    typedef typename incidence_iterator_type<CARTGRID, 2, DIM>    ::type CellOnFaceIterator;

    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 0>  ::type VertexOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 1>  ::type EdgeOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, 2>  ::type FaceOnFacetIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM-1, DIM>::type CellOnFacetIterator;

    typedef typename incidence_iterator_type<CARTGRID, DIM, 0>    ::type VertexOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 1>    ::type EdgeOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, 2>    ::type FaceOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM-1>::type FacetOnCellIterator;
    typedef typename incidence_iterator_type<CARTGRID, DIM, DIM>  ::type CellOnCellIterator;


    // We could distinguish vertex-  and cell_index_type.
    // This would allow for clearer construction semantics
    typedef tuple<int, DIM> vertex_index_type;
    typedef tuple<int, DIM> cell_index_type;
    typedef tuple<int, DIM> index_type;

    typedef typename archetype_of_grid_aux<CARTGRID,DIM>::type archetype_type;
    typedef unsigned                                   archetype_handle;
    typedef archetype_type const*                      archetype_iterator;
  }; // struct grid_types_base<unsigned DIM>
 


  //--------------------- element handles -------------------

  template<class CARTGRID, unsigned K>
  class element_handle_t {
    typedef element_handle_t<CARTGRID,K> self;
    int h_;
  public:
    element_handle_t() {}
    explicit element_handle_t(int hh) : h_(hh) {}
    int h() const { return h_;}
    operator int()       { return h_;}
    self& operator++() {++h_; return *this;}
    //operator int () const { return h;}
  };

  template<class CARTGRID, unsigned K>
  inline bool operator==(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() == rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator< (element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() <  rhs.h();}
#ifndef GRAL_INCLUDES_RELOPS
  template<class CARTGRID, unsigned K>
  inline bool operator!=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() != rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator> (element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() >  rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator<=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() <= rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator>=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() >= rhs.h();}
#endif

  template<class CARTGRID, unsigned K>
  inline std::ostream& operator<<(std::ostream& out, element_handle_t<CARTGRID,K> lhs)
  { return (out << lhs.h());}
  template<class CARTGRID, unsigned K>
  inline std::istream& operator>>(std::istream& in,  element_handle_t<CARTGRID,K> lhs)
  { return (in >> lhs.h());}

} // namespace cartesiannd

// global namespace 
template<class R, class CARTGRID, unsigned K>
inline R convert(cartesiannd::element_handle_t<CARTGRID,K> const& h) { return h.h();}



namespace STDEXT {
  template<class T> class hash;

  template<class CARTGRID, unsigned K>
  class hash<cartesiannd::element_handle_t<CARTGRID,K> > {
  public:
    typedef cartesiannd::element_handle_t<CARTGRID,K>  key_type;
    typedef cartesiannd::element_handle_t<CARTGRID,K>  argument_type;
    typedef size_t                        result_type;
    result_type operator()(key_type h) const { return h.h();}
  }; 
} // namespace STDEXT 


namespace cartesiannd {


  //---------------------------- grid types ----------------------------


  template<class CARTGRID, unsigned DIM>
  class grid_base : public grid_types_base<CARTGRID, DIM> {
    typedef grid_types_base<CARTGRID, DIM> base;
  public:
    typedef typename base::vertex_index_type vertex_index_type;
    typedef typename base::cell_index_type   cell_index_type;
    typedef typename base::index_type        index_type;
    typedef typename base::VertexIterator    VertexIterator;
    typedef typename base::EdgeIterator      EdgeIterator;
    typedef typename base::FaceIterator      FaceIterator;
    typedef typename base::FacetIterator     FacetIterator;
    typedef typename base::CellIterator      CellIterator;
    enum { dimension = DIM };
    // protected:
  public:
    map_table<DIM>    maps   [DIM+1]; // [0, ..., DIM]
    offset_table<DIM> offsets[DIM+1];

    void init(vertex_index_type low, vertex_index_type beyond);
  public:
    vertex_index_type low_vertex_index()    const { return maps[0][0].min_tuple();}
    vertex_index_type high_vertex_index()   const { return maps[0][0].max_tuple();}
    vertex_index_type beyond_vertex_index() const { return maps[0][0].beyond_tuple();}

    cell_index_type low_cell_index()    const { return maps[DIM][0].min_tuple();}
    cell_index_type high_cell_index()   const { return maps[DIM][0].max_tuple();}
    cell_index_type beyond_cell_index() const { return maps[DIM][0].beyond_tuple();}

    cell_index_type cell_size() const { return beyond_cell_index() - low_cell_index();}

    unsigned NumOfDirections(unsigned k) const { 
      cvdim(k);
      return maps[k].num_of_directions();
    }

    unsigned NumOfVertices() const { return NumOfElements(0);}
    unsigned NumOfEdges()    const { return NumOfElements(1);}
    unsigned NumOfFaces()    const { return NumOfElements(2);}
    unsigned NumOfFacets()   const { return NumOfElements(DIM-1);}
    unsigned NumOfCells()    const { return NumOfElements(DIM);}

    template<unsigned K>
    unsigned NumOfElements()           const { cvdim(K); return offsets[K][maps[K].num_of_maps()];}
    unsigned NumOfElements(unsigned k) const { cvdim(k); return offsets[k][maps[k].num_of_maps()];}
    

    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> FirstElement() const;
    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> EndElement()   const;

    VertexIterator FirstVertex()  const { return FirstElement<0>();}
    EdgeIterator   FirstEdge()    const { return FirstElement<1>();}
    FaceIterator   FirstFace()    const { return FirstElement<2>();}
    FacetIterator  FirstFacet()   const { return FirstElement<DIM-1>();}
    CellIterator   FirstCell()    const { return FirstElement<DIM>();}

    VertexIterator EndVertex()    const { return EndElement<0>();}
    EdgeIterator   EndEdge()      const { return EndElement<1>();}
    FaceIterator   EndFace()      const { return EndElement<2>();}
    FacetIterator  EndFacet()     const { return EndElement<DIM-1>();}
    CellIterator   EndCell()      const { return EndElement<DIM>();}

    CARTGRID      & to_derived()       { return static_cast<CARTGRID      &>(*this);}
    CARTGRID const& to_derived() const { return static_cast<CARTGRID const&>(*this);}


    template<unsigned K>
    element_handle_t<CARTGRID, K> index2handle(index_type idx, unsigned dir) const 
    { return element_handle_t<CARTGRID, K>(maps[K][dir](idx) + offsets[K][dir]); }

    typename base::vertex_handle get_vertex_handle(index_type idx) const { return index2handle<0>(idx,0);}

    template<unsigned K>
    index_type handle2index(element_handle_t<CARTGRID, K> h, unsigned dir) const
    { return  (maps[K][dir](h.h() - offsets[K][dir])); }

    template<unsigned K>
    bool valid_handle(element_handle_t<CARTGRID, K> h)  const { return 0 <= h.h() && h.h() < (int) offsets[K].beyond();}

    bool valid_dim(unsigned k) const { return k <= DIM;}
    void cvdim    (unsigned k) const { REQUIRE(valid_dim(k), "k=" << k << " DIM=" << DIM, 1);}

    void print(std::ostream& out) const;

    // Archetype stuff

    typedef typename base::archetype_type     archetype_type;
    typedef typename base::archetype_handle   archetype_handle;
    typedef typename base::archetype_iterator archetype_iterator;

    static archetype_type *       archetype_array;
    static int *                  archetype_initialized;

    static archetype_iterator    BeginArchetype()  { init_archetypes(); return archetype_array;}
    static archetype_iterator    EndArchetype()    { return BeginArchetype() + 1;}
    static unsigned              NumOfArchetypes() { return 1;}
    static archetype_type const& TheArchetype()  { return * BeginArchetype();} // archetype_array[0];}
    static archetype_handle      the_archetype() { return 0;}
    static archetype_type const& ArchetypeOf(typename base::Cell const&) { return TheArchetype();}
    static archetype_type const& ArchetypeOf(typename base::cell_handle) { return TheArchetype();}
    static archetype_type const& Archetype  (archetype_handle) { return TheArchetype();}
    static archetype_handle      handle(archetype_iterator ait) 
    { REQUIRE(ait == BeginArchetype(), "", 1); return (ait - BeginArchetype());} 
    static archetype_handle        archetype_of(typename base::cell_handle) { return 0;}
    static archetype_handle        archetype_of(typename base::Cell const&) { return 0;}

    static void init_archetypes() { 
      if(! archetype_initialized ) {
	archetype_initialized = new int(1);
	archetype_array = new archetype_type[1];
	// base_dim_1::init_archetypes();
      }
    }

  }; // class grid_base<unsigned DIM>


  template<class CARTGRID, unsigned DIM>
  typename grid_base<CARTGRID, DIM>::archetype_type *
  grid_base<CARTGRID, DIM>::archetype_array = 0;
  template<class CARTGRID, unsigned DIM>
  int * grid_base<CARTGRID, DIM>::archetype_initialized = 0;


  //  template<class CARTGRID, unsigned DIM>
  template<unsigned DIM>
  class subrange : public grid_base<subrange<DIM>, DIM> {
    typedef grid_base<subrange<DIM>, DIM> base;
    typedef subrange<DIM>                 self;
  public:
    typedef grid<DIM> grid_type;
    typedef grid<DIM> base_grid_type;
    typedef grid<DIM-1> facet_grid_type;

    typedef typename base::vertex_index_type vertex_index_type;
  private:
    ref_ptr<grid_type const> g;
  public:
    subrange() {}
    subrange(grid_type const&         gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond); }
    subrange(ref_ptr<grid_type const> gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond); }

    ref_ptr<grid_type const> BaseGrid() const { return g;}

    template<unsigned K>
    element_handle_t<base_grid_type, K>  base_handle(element_handle_t<self, K> h, unsigned dir) const 
    { return BaseGrid()->template index2handle<K>(handle2index(h,dir), dir);}


  }; // class subrange<unsigned DIM>


  /*! \brief An N-dimensional Cartesian grid

     \todo Upward Incidence iterators
     \todo switch operators
  */
  template<unsigned DIM>
  class grid : public grid_base<grid<DIM>, DIM> {
    typedef grid_base<grid<DIM>, DIM> base;
    typedef grid<DIM>                 self;

  public:
    typedef grid<DIM>                        base_grid_type;
    typedef grid<DIM-1>                      facet_grid_type;
    typedef typename base::vertex_index_type vertex_index_type;
    enum { dimension = DIM};

    //! Empty grid
    grid() {}
    //! Grid with vertex indices \f$ x_i \in [0, b_i[ \f$ with \f$ b = \f$  \c beyond
    grid(vertex_index_type beyond) 
    { init(vertex_index_type(0), beyond);}
    //! Grid with vertex indices \f$ x_i \in [l_, b_i[ \f$ with \f$ l = \f$ \c  low and  \f$ b = \f$  \c beyond    
    grid(vertex_index_type low, vertex_index_type high)
    { init(low, high+index_type(1));}

    ref_ptr<self const> BaseGrid() const { return ref_ptr<self const>(*this);}

    template<unsigned K>
    element_handle_t<self, K> base_handle(element_handle_t<self, K> h, unsigned dir) const { return h;}

  }; // class grid<unsigned DIM>




  //-----------------------------  archetype ---------------------------------

  template<class CARTGRID>
  struct  archetype_base_t {
    typedef grid_types<CARTGRID> gt;

    enum   { dimension = CARTGRID::dimension -1};
    typedef typename gt::Facet  Cell;
    typedef typename gt::Vertex Vertex;
    typedef typename gt::facet_handle  cell_handle;
    typedef typename gt::vertex_handle vertex_handle;

    typedef typename gt::VertexOnCellIterator VertexIterator; 
    typedef typename gt::FacetOnCellIterator  CellIterator; 
    typedef typename gt::VertexOnFacetIterator VertexOnCellIterator;

    typedef typename gt::Edge Edge;
    typedef typename gt::edge_handle edge_handle;
  };

  template<class CARTGRID>
  class archetype_t : public archetype_base_t<CARTGRID> {
    typedef grid_types<CARTGRID> gt;
    typedef archetype_base_t<CARTGRID> base;
    CARTGRID g; // single D-dimensional cell
  public:
    enum { dimension = CARTGRID::dimension -1};
    typedef typename base::Cell  Cell;
    typedef typename base::Vertex Vertex;
    typedef typename base::cell_handle   cell_handle;
    typedef typename base::vertex_handle vertex_handle;

    typedef typename base::VertexIterator       VertexIterator; 
    typedef typename base::CellIterator         CellIterator; 
    typedef typename base::VertexOnCellIterator VertexOnCellIterator;

    typedef typename CARTGRID::facet_grid_type facet_grid_type;
    typedef grid_types<facet_grid_type>        fgt;
    typedef typename gt::vertex_index_type     vertex_index_type;
    typedef typename gt::index_type            index_type;
    // define archetype_type, _iterator etc.
    // typedef typename archetype_of_grid<CARTGRID, CARTGRID::dimension-1>::archetype archetype_type;

    // create grid with 1 cell
    //    archetype_t() : g(vertex_index_type(0), vertex_index_type(1)) {}
    archetype_t() 
    {
      delta_map<CARTGRID::dimension>::init();
      g.init(vertex_index_type(0), vertex_index_type(2));
      cg();
      REQUIRE_ALWAYS(g.NumOfCells() == 1, "g.NumOfCells()=" << g.NumOfCells(), 1);
      /*
      std::cerr << "Init archetype " << CARTGRID::dimension << ": " 
		<< g.NumOfVertices() << " vertices, " << g.NumOfCells() << " cells" << std::endl;
      g.print(std::cerr);
      */
    }
    void print(std::ostream& out) const { g.print(out);}

    void cg() const {
      REQUIRE(g.NumOfCells() == 1, "g.NumOfCells()=" << g.NumOfCells(), 1);
    }
    // for having grid functions on archetype ...
    operator CARTGRID const&() const { cg(); return g;}
    unsigned NumOfVertices() const { cg(); return g.NumOfVertices();}
    unsigned NumOfCells()    const { cg(); return g.NumOfFacets();}

    VertexIterator FirstVertex() const { cg(); return VertexIterator(* g.FirstCell());}
    CellIterator   FirstCell()   const { cg(); return CellIterator  (* g.FirstCell());}

    // hack: assume 2D grid -> 1D arch
    static void switch_vertex(Vertex      & v, Cell const& e) 
    {
      index_type idx_v = v.index();
      index_type idx_e = e.index();
      if(idx_v != idx_e) 
	v = Vertex(v.TheGrid(), idx_e);
      else {
	int d = delta_map<dimension+1>::dirs[1][e.direction()][0];
	idx_v[d] += 1;
	v = Vertex(v.TheGrid(), idx_v);
      }
    }

    static void switch_cell  (Vertex  const& f, Cell      & c) {
      index_type idx_f = f.index();
      index_type idx_c = c.index();
      int m = c.direction(); // in {0,1}
      int d_alt = delta_map<dimension+1>::dirs[1][m  ][0];
      int d_new = delta_map<dimension+1>::dirs[1][1-m][0];
      m = (1-m); 
      index_type diff_1 = idx_f - idx_c;
      index_type diff_2 = idx_c; // - 0 = index of g.TheCell();
      index_type idx_c_new = idx_c;
      idx_c_new[d_alt] += diff_1[d_alt];
      idx_c_new[d_new] -= diff_2[d_new];
      c = Cell(c.TheGrid(), idx_c_new, m);
    }


    typedef typename fgt::archetype_iterator archetype_iterator;
    typedef typename fgt::archetype_handle   archetype_handle;
    typedef typename fgt::archetype_type     archetype_type;

    static archetype_type const&  ArchetypeOf (Cell const&) { return facet_grid_type::TheArchetype();}
    static archetype_type const&  ArchetypeOf (cell_handle) { return facet_grid_type::TheArchetype();}
    static archetype_handle       archetype_of(Cell const&) { return facet_grid_type::the_archetype();}
    static archetype_handle       archetype_of(cell_handle) { return facet_grid_type::the_archetype();}
 
    static archetype_type const&  Archetype(archetype_handle a) { return facet_grid_type::Archetype(a);}

    static archetype_iterator BeginArchetype() { return facet_grid_type::BeginArchetype();}
    static archetype_iterator EndArchetype()   { return facet_grid_type::EndArchetype();}
    static archetype_handle   handle(archetype_iterator it) { return facet_grid_type::handle(it);}
    static unsigned NumOfArchetypes() { return facet_grid_type::NumOfArchetypes();}
    
  };


  //----------------------------- sequence iterators ---------------------------------

  template<class CARTGRID, unsigned K>
  class sequence_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dimension> {
    typedef CARTGRID                                       grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dimension> base;
    typedef sequence_iterator_t<CARTGRID, K>               self;
  public:
    enum { griddim = CARTGRID::dimension};
    typedef typename grid_type::base_grid_type  base_grid_type;
    typedef element_handle_t<base_grid_type,K>  element_handle;
    typedef element_handle_t<CARTGRID, K>       local_element_handle;

    typedef typename base::index_type        index_type;

    //    typedef self                             value_type;
    typedef typename element_type<grid_type, K, griddim - K>::type value_type;
  private:
    ref_ptr<grid_type const> g;
    local_element_handle     h;
    unsigned                 m; // index into the choice of basis vectors, \in [0, \choice{D, K}[

  public:
    sequence_iterator_t() {}
    explicit sequence_iterator_t(grid_type         const& gg) : g(gg), h(0) { init_m();} 
    explicit sequence_iterator_t(ref_ptr<grid_type const> gg) : g(gg), h(0) { init_m();} 
    sequence_iterator_t(grid_type         const& gg, local_element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(ref_ptr<grid_type const> gg, local_element_handle hh) : g(gg), h(hh) { init_m();}
    sequence_iterator_t(grid_type         const& gg, local_element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}
    sequence_iterator_t(ref_ptr<grid_type const> gg, local_element_handle hh, unsigned mm) : g(gg), h(hh), m(mm) {}
    sequence_iterator_t(grid_type         const& gg, index_type idx) : g(gg)  { init(idx,0); }
    sequence_iterator_t(ref_ptr<grid_type const> gg, index_type idx) : g(gg)  { init(idx,0); } 
    sequence_iterator_t(grid_type         const& gg, index_type idx, unsigned mm) : g(gg)  { init(idx,mm); }
    sequence_iterator_t(ref_ptr<grid_type const> gg, index_type idx, unsigned mm) : g(gg)  { init(idx,mm); } 

    void init(index_type idx) {
      REQUIRE(g->NumOfDirections(K) == 1, "", 1);
      init(idx,0);
    }
    void init(index_type idx, unsigned dir) {
      m = dir;
      h = g-> template index2handle<K>(idx,dir);
    }


    template<class CARTSUBRANGE>
    sequence_iterator_t(sequence_iterator_t<CARTSUBRANGE, K> const& rhs) 
      : g(rhs.TheGrid()->BaseGrid()), 
	h(g->template index2handle<K>(rhs.index(),rhs.direction())),  m(rhs.direction())
    {}

    // initialization from subrange 
    // this will work only if element_handle and local_element_handle are the same.
    template<class CARTSUBRANGE>
    sequence_iterator_t(ref_ptr<CARTSUBRANGE const> rge, element_handle hh) : g(rge->BaseGrid()), h(hh) { init_m();}


    self&       operator++()       { cv(); ++h; if(! valid_map()) ++m; return *this; }
    bool        IsDone()     const { cb(); return m >= g->maps[K].num_of_maps();}

    // this will use the implicit copy-constructor if value_type == self,
    // and the templated copy constructor otherwise.
    value_type  operator*()  const { cv(); return value_type(*this); } 
    // value_type  operator*()  const { cv(); return value_type(g->BaseGrid(), handle(), m);}

    local_element_handle local_handle() const { cb(); return h;}
    //! \c handle() return  unique element handle with respect to base grid
    element_handle     handle()    const { cb(); return TheGrid()->base_handle(h,m);}
    unsigned           direction() const { cb(); return m;}

    //! for \f$ K \neq 0, D \f$, \c index() is not unique!
    index_type index()  const { cb(); return g->handle2index(h,m);} //  g->maps[K][m](h - g->offsets[K][m]);}

    typedef typename base::archetype_type archetype_type;
    typedef typename base::Vertex         Vertex;
    typedef typename base::vertex_handle  vertex_handle;
    // this is useful only for cells, i.e. K=DIM
    Vertex        V(typename archetype_type::Vertex const& av) const { return Vertex(TheGrid(), index() + av.index());}
    vertex_handle v(typename archetype_type::Vertex const& av) const { return TheGrid()->get_vertex_handle(index()+av.index());}

    ref_ptr<grid_type const> TheGrid() const { return g;}

    template<unsigned M>
    typename incidence_iterator_type<CARTGRID,K,M>::type FirstElement() const
    {  return typename incidence_iterator_type<CARTGRID,K,M>::type(*this);}

    typename incidence_iterator_type<CARTGRID,K,0>::type FirstVertex() const { return FirstElement<0>();}
    typename incidence_iterator_type<CARTGRID,K,1>::type FirstEdge  () const { return FirstElement<1>();}
    typename incidence_iterator_type<CARTGRID,K,2>::type FirstFace  () const { return FirstElement<2>();}
    typename incidence_iterator_type<CARTGRID,K,griddim-1>::type FirstFacet() const 
    { return FirstElement<griddim-1>();}
    typename incidence_iterator_type<CARTGRID,K,griddim>::type   FirstCell () const 
    { return FirstElement<griddim>();}



    // Warning: this works only if M < K! Else there might be incident elements outside the grid!
    template<unsigned M>
    unsigned NumOfElements() const { return delta_map<griddim>::incs[K][M][direction()].size();}

    unsigned NumOfVertices() const { return NumOfElements<0>();}    
    unsigned NumOfEdges   () const { return NumOfElements<1>();}    
    unsigned NumOfFaces   () const { return NumOfElements<2>();}    
    unsigned NumOfFacets  () const { return NumOfElements<griddim-1>();}    
    unsigned NumOfCells   () const { return NumOfElements<griddim>  ();}    



    bool bound() const { return g != 0;}
    bool valid() const { return bound() && ! IsDone();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}

    // find appropriate m by looking up the correct interval of h in offsets[K];
    void init_m() { m = 0; while(!valid_map() && m < g->NumOfDirections(K)) ++m;}
    // is h a handle for the current map m?
    bool valid_map() const { return (unsigned)h.h() < g->offsets[K][m+1];}

  }; // class sequence_iterator_t<class CARTGRID, unsigned K>  

 template<class CARTGRID, unsigned K>
 inline bool operator==(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return !(lhs==rhs);}
#endif


  //------------------------ incidence iterators -------------------------

  template<class CARTGRID, unsigned K, unsigned M>
  class downward_inc_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dimension> {
  private:
    typedef CARTGRID                                       grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dimension> base;
    typedef downward_inc_iterator_t<CARTGRID, K, M>        self;
  public:
    enum { dimension = base::dimension};
    typedef typename base::index_type                      index_type;
    typedef typename grid_type::base_grid_type             base_grid_type;
    typedef element_handle_t<base_grid_type,M>             element_handle;
    typedef typename element_type<base_grid_type, K, dimension-K>::type anchor_type;
    typedef typename element_type<base_grid_type, M, dimension-M>::type element_type;
    typedef element_type                                   value_type;
  private:
    anchor_type a;

    unsigned                    loc;

  public:
    downward_inc_iterator_t() {}
    explicit downward_inc_iterator_t(anchor_type const&         aa, unsigned ll = 0) : a(aa), loc(ll) {}

    self& operator++()  { cv(); ++loc; return *this;}
    element_type operator*() const { return element_type(TheGrid(), handle());}
    
    element_handle handle()   const { cv(); return TheGrid()->template index2handle<M>(index(), direction());}
    index_type index()        const { cv(); return a.index() + table()[loc].offset;}
    unsigned   direction()    const { cv(); return table()[loc].dir;}
    unsigned   local_handle() const { cb(); return loc;}

    bool IsDone() const { cb(); return loc >= table().size(); }
    
    ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    ref_ptr<grid_type   const> TheGrid()   const { return a.TheGrid();}

    bool bound() const { return a.valid();}
    bool valid() const { return bound() && ! IsDone();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  private:
    typedef typename delta_map<dimension>::inc_descriptor incidences;
    std::vector<incidences> table() const { return delta_map<dimension>::incs[K][M][a.direction()];}
  };


 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator==(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator!=(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return !(lhs==rhs);}
#endif



  //-----------------------  inline functions  ----------------------------
 

  template<class CARTGRID, unsigned DIM>
  template<unsigned K>
  sequence_iterator_t<CARTGRID, K>
  grid_base<CARTGRID,DIM>::FirstElement() const { return sequence_iterator_t<CARTGRID, K>(to_derived());}

  template<class CARTGRID, unsigned DIM>
  template<unsigned K>
  sequence_iterator_t<CARTGRID, K>
  grid_base<CARTGRID,DIM>::EndElement() const { 
    return sequence_iterator_t<CARTGRID, K>(to_derived(), 
					    element_handle_t<CARTGRID,K>(offsets[K][maps[K].num_of_maps()]), 
					    maps[K].num_of_maps());
  }


  template<class CARTGRID, unsigned DIM>
  void grid_base<CARTGRID,DIM>::init(typename grid_base<CARTGRID,DIM>::vertex_index_type low,
				     typename grid_base<CARTGRID,DIM>::vertex_index_type beyond) 
  {
    // std::cout << "grid_base<CARTGRID,DIM>::init(" << "[" << low << "], [" << beyond << "[)\n";
    if(! delta_map<DIM>::initialized()) {
      // std::cout << "Initializing grid_base<" << DIM << ">" << std::endl;
      delta_map<DIM>::init();
    }
    // if(archetype_array == 0)
    if(archetype_initialized == 0) {
      archetype_initialized = new int(1);
      archetype_array = new archetype_type[1];
    }

    for(unsigned k = 0; k <= dimension; ++k) {
      maps   [k].init(k, low, beyond);
      offsets[k].init(k);
      offsets[k][0] = 0;
      for(unsigned m = 1; m <= maps[k].num_of_maps(); ++m)
	offsets[k][m] = offsets[k][m-1] + maps[k][m-1].flat_size();
    }
  }
  template<class CARTGRID, unsigned DIM>
  void grid_base<CARTGRID,DIM>::print(std::ostream& out) const
  {
    //  out << "directions:\n";
    //  delta_map<DIM>::print(out);
    for(unsigned k = 0; k <= DIM; ++k) {
      out << "maps   [" << k << "]: ";
      maps[k].print(out); 
      out << "\n";
      out << "offsets[" << k << "]: "; 
      offsets[k].print(out); 
      out << "\n";
    }
  } 


}; // namespace cartesiannd


template<unsigned DIM>
struct grid_types<cartesiannd::grid<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::grid<DIM>, DIM> >
{
  typedef cartesiannd::subrange<DIM>      cartesian_subrange_type;
};

template<unsigned DIM>
struct grid_types<cartesiannd::archetype_t<cartesiannd::grid<DIM> > >
  : public grid_types_base<cartesiannd::archetype_base_t<cartesiannd::grid<DIM> > >
{
  typedef cartesiannd::archetype_t<cartesiannd::grid<DIM> > at;
  typedef typename at::archetype_iterator archetype_iterator;
  typedef typename at::archetype_handle   archetype_handle;
  typedef typename at::archetype_type     archetype_type;

  typedef grid_dim_tag<DIM-1> dimension_tag;
};

template<unsigned DIM>
struct grid_types<cartesiannd::subrange<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::subrange<DIM>, DIM> >
{
};


// vertex
template<unsigned DIM>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM> ,0> >
  : public element_traits_vertex_base<cartesiannd::grid<DIM> > 
{
  typedef element_traits_vertex_base<cartesiannd::grid<DIM> >       base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef typename base::hasher_type_elem_base       hasher_type;

};

// edge
template<unsigned DIM>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, 1> >
  : public element_traits_edge_base<cartesiannd::grid<DIM> > 
{
  typedef element_traits_edge_base<cartesiannd::grid<DIM> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef typename base::hasher_type_elem_base       hasher_type;
};



// cell
template<unsigned DIM>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, DIM> >
  : public element_traits_cell_base<cartesiannd::grid<DIM> > 
{
  typedef element_traits_cell_base<cartesiannd::grid<DIM> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef typename base::hasher_type_elem_base       hasher_type;
};

// facet
template<unsigned DIM>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>, DIM-1> >
  : public element_traits_facet_base<cartesiannd::grid<DIM> > 
{
  typedef element_traits_cell_base<cartesiannd::grid<DIM> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef typename base::hasher_type_elem_base       hasher_type;
};



// avoid conflict vertex<->cell in 0D
template<>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<0> ,0> >
  : public element_traits_vertex_base<cartesiannd::grid<0> > 
{
  typedef element_traits_vertex_base<cartesiannd::grid<0> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};


// avoid conflict edge<->cell in 1D
template<>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<1> ,1> >
  : public element_traits_edge_base<cartesiannd::grid<1> > 
{
  typedef element_traits_edge_base<cartesiannd::grid<1> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};

// avoid conflict vertex <->facet in 1D
template<>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<1>, 0> >
  : public element_traits_vertex_base<cartesiannd::grid<1> > 
{
  typedef element_traits_vertex_base<cartesiannd::grid<1> >  base;
  typedef consecutive_integer_tag<0>                       consecutive_tag;
  typedef base::hasher_type_elem_base                      hasher_type;
};

// avoid conflict edge <->facet in 2D
template<>
struct element_traits<cartesiannd::sequence_iterator_t<cartesiannd::grid<2>, 1> >
  : public element_traits_edge_base<cartesiannd::grid<2> > 
{
  typedef element_traits_edge_base<cartesiannd::grid<2> >  base;
  typedef consecutive_integer_tag<0>                       consecutive_tag;
  typedef base::hasher_type_elem_base                      hasher_type;
};







#endif
