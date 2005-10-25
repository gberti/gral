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


namespace GrAL {

namespace cartesiannd {
  
  //-------------- forward declarations ---------------

  template<class CARTGRID, unsigned K>
  class element_handle_t;
  template<class CARTGRID, unsigned K>
  class sequence_iterator_t;
  template<class CARTGRID, unsigned K, unsigned M>
  class downward_inc_iterator_t;
  template<class CARTGRID, unsigned K, unsigned M>
  class upward_inc_iterator_t;
  template<unsigned DIM>
  class subrange;
  template<unsigned DIM>
  class grid;
  template<class CARTGRID>
  class archetype_t;




 template<class CARTGRID, unsigned K>
 inline bool operator==(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator<(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);

#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator>(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator<=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
 template<class CARTGRID, unsigned K>
 inline bool operator>=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs);
#endif



 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator==(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs);
  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator!=(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs);
  // #endif



 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator==(upward_inc_iterator_t<CARTGRID,K, M> lhs, 
			upward_inc_iterator_t<CARTGRID,K, M> rhs);
  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator!=(upward_inc_iterator_t<CARTGRID,K, M> lhs, 
			upward_inc_iterator_t<CARTGRID,K, M> rhs);
  //#endif

  

  template<class GRID,   unsigned D, unsigned CD>  struct element_type 
  { typedef sequence_iterator_t<GRID, D> type; };
  template<unsigned DIM, unsigned D, unsigned CD>  struct element_type<subrange<DIM>, D, CD> 
  { typedef sequence_iterator_t<grid<DIM>, D> type; };

  template<class CARTGRID, unsigned K> struct element_handle_type
  { typedef element_handle_t<CARTGRID, K>  type; };
  template<unsigned DIM, unsigned K> struct element_handle_type<subrange<DIM>, K>
  { typedef element_handle_t<grid<DIM>, K> type;};

  template<unsigned K, unsigned M> struct less_then { enum { result = ( K <= M ? 1 : 0) }; };

  template<class CARTGRID, unsigned K, unsigned M, unsigned LESS>
  struct incidence_iterator_type_aux {};

  // K <= M => <M>On<K> is upward
  template<class CARTGRID, unsigned K, unsigned M>
  struct incidence_iterator_type_aux<CARTGRID,K,M,1> 
  { typedef upward_inc_iterator_t<CARTGRID,K,M> type; };
  
  
  // K > M => <M>On<K> is downward
  template<class CARTGRID, unsigned K, unsigned M>
  struct incidence_iterator_type_aux<CARTGRID,K,M,0>
  { typedef downward_inc_iterator_t<CARTGRID,K,M> type; };

  template<class CARTGRID, unsigned K, unsigned M>
  struct incidence_iterator_type : public incidence_iterator_type_aux<CARTGRID,K,M, less_then<K,M>::result> {};

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
  struct archetype_of_grid : public archetype_of_grid_aux<CARTGRID, CARTGRID::dim> {};



  template<class CARTGRID, unsigned DIM>
  struct grid_types_base
  {
    enum { dim = DIM};
    typedef grid_dim_tag<dim> dimension_tag;

    typedef typename element_handle_type<CARTGRID, 0>       ::type vertex_handle;
    typedef          sequence_iterator_t<CARTGRID, 0>              VertexIterator;
    typedef typename element_type       <CARTGRID, 0, DIM>  ::type Vertex;

    typedef typename element_handle_type<CARTGRID, 1>       ::type edge_handle;
    typedef          sequence_iterator_t<CARTGRID, 1>              EdgeIterator;
    typedef typename element_type       <CARTGRID, 1, DIM-1>::type Edge;

    typedef typename element_handle_type<CARTGRID, 2>       ::type face_handle;
    typedef          sequence_iterator_t<CARTGRID, 2>              FaceIterator;
    typedef typename element_type       <CARTGRID, 2, DIM-2>::type Face;

    typedef typename element_handle_type<CARTGRID, DIM-1>   ::type facet_handle;
    typedef          sequence_iterator_t<CARTGRID, DIM-1>          FacetIterator;
    typedef typename element_type       <CARTGRID, DIM-1, 1>::type Facet;

    typedef typename element_handle_type<CARTGRID, DIM>     ::type cell_handle;
    typedef          sequence_iterator_t<CARTGRID, DIM>            CellIterator;
    typedef typename element_type       <CARTGRID, DIM, 0>  ::type Cell;

    typedef typename incidence_iterator_type<CARTGRID, 0, 0>      ::type VertexOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, 1>      ::type EdgeOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, 2>      ::type FaceOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, DIM-1>  ::type FacetOnVertexIterator;
    typedef typename incidence_iterator_type<CARTGRID, 0, DIM>    ::type CellOnVertexIterator;

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
    typedef grid_types<archetype_type>                 archgt;
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
  //#ifndef GRAL_INCLUDES_RELOPS
  template<class CARTGRID, unsigned K>
  inline bool operator!=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() != rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator> (element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() >  rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator<=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() <= rhs.h();}
  template<class CARTGRID, unsigned K>
  inline bool operator>=(element_handle_t<CARTGRID,K> lhs, element_handle_t<CARTGRID,K> rhs) { return lhs.h() >= rhs.h();}
  //#endif

  template<class CARTGRID, unsigned K>
  inline  ::std::ostream& operator<<( ::std::ostream& out, element_handle_t<CARTGRID,K> lhs)
  { return (out << lhs.h());}
  template<class CARTGRID, unsigned K>
  inline  ::std::istream& operator>>( ::std::istream& in,  element_handle_t<CARTGRID,K> lhs)
  { return (in >> lhs.h());}

} // namespace cartesiannd

// global namespace 
template<class R, class CARTGRID, unsigned K>
inline R convert(cartesiannd::element_handle_t<CARTGRID,K> const& h) { return h.h();}

} // namespace GrAL

namespace STDEXT {
  template<class T> class hash;

  template<class CARTGRID, unsigned K>
  class hash<GrAL::cartesiannd::element_handle_t<CARTGRID,K> > {
  public:
    typedef GrAL::cartesiannd::element_handle_t<CARTGRID,K>  key_type;
    typedef GrAL::cartesiannd::element_handle_t<CARTGRID,K>  argument_type;
    typedef size_t                        result_type;
    result_type operator()(key_type h) const { return h.h();}
  }; 
} // namespace STDEXT 


namespace GrAL {
namespace cartesiannd {


  //---------------------------- grid types ----------------------------

  /*! \brief Base class for n-dimensional Cartesian grids
      \ingroup cartesianndmodule
   */
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
    enum { dim = DIM };
    // protected:
  public:
    map_table<DIM>    maps   [DIM+1]; // [0, ..., DIM]
    offset_table<DIM> offsets[DIM+1];

    void init(vertex_index_type low, vertex_index_type beyond);
  public:
    //! Combinatorial dimension
    unsigned dimension() const { return dim;}

    //@{
    /*! \name Size information and bounds
     */
    //! \brief Lowest vertex index, valid vertex indices are \Ge \c low_vertex_index() 
    vertex_index_type low_vertex_index()    const { return maps[0][0].min_tuple();}
    //! \brief Highest vertex index, valid vertex indices are \Le \c high_vertex_index() 
    vertex_index_type high_vertex_index()   const { return maps[0][0].max_tuple();}
    //! \brief Past-the-end vertex index, valid vertex indices are \Lt \c high_vertex_index() 
    vertex_index_type beyond_vertex_index() const { return maps[0][0].beyond_tuple();}
    //! \brief Number of vertices in each direction
    vertex_index_type vertex_size()         const { return beyond_vertex_index() - low_vertex_index();}


    //! \brief Lowest cell index, valid cell indices are \Ge \c low_cell_index() 
    cell_index_type low_cell_index()    const { return maps[DIM][0].min_tuple();}
    //! \brief Highest cell index, valid cell indices are \Le \c high_cell_index() 
    cell_index_type high_cell_index()   const { return maps[DIM][0].max_tuple();}
    //! \brief Past-the-end cell index, valid cell indices are \Lt \c high_cell_index() 
    cell_index_type beyond_cell_index() const { return maps[DIM][0].beyond_tuple();}

    //! \brief Number of cells in each direction
    cell_index_type cell_size() const { return beyond_cell_index() - low_cell_index();}
    //@}

    /*! \brief Number of possible element direction in dimension \c k
  
        For dimensions <tt> k == 0 </tt> and <tt> k == DIM </tt>, there is only one direction.
	In 3D, there are 3 directions (x,y, and z) for edges (<tt> k == 1 </tt>) ,
	and 3 directions (xy, xz, yz) for facets (<tt> k == 2</tt>).

        In general, there are \f$ k \choose  \mbox{DIM} \f$  directions.
     */
    unsigned NumOfDirections(unsigned k) const { 
      cvdim(k);
      return maps[k].num_of_directions();
    }


    //@{
    /*! \name Sequence iteration (Dimension-independent interface)
     */
    //! \brief Number of elements of dimension \c K (compile-time dimension)
    template<unsigned K>
    unsigned NumOfElements()           const { cvdim(K); return offsets[K][maps[K].num_of_maps()];}
    //! \brief Number of elements of dimension \c k (run-time dimension)
    unsigned NumOfElements(unsigned k) const { cvdim(k); return offsets[k][maps[k].num_of_maps()];}
    
    //! \brief First element of dimension \c K
    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> FirstElement() const;
    //! \brief Past-the-end element of dimension \c K
    template<unsigned K>
    sequence_iterator_t<CARTGRID, K> EndElement()   const;
    //@}

    //@{
    /*! \name Sequence iteration
        \brief STL-style (begin/end) and GrAL-style (IsDone) iteration  
    */
    unsigned NumOfVertices() const { return NumOfElements(0);}
    unsigned NumOfEdges()    const { return NumOfElements(1);}
    unsigned NumOfFaces()    const { return NumOfElements(2);}
    unsigned NumOfFacets()   const { return NumOfElements(DIM-1);}
    unsigned NumOfCells()    const { return NumOfElements(DIM);}

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
    //@}

    CARTGRID      & to_derived()       { return static_cast<CARTGRID      &>(*this);}
    CARTGRID const& to_derived() const { return static_cast<CARTGRID const&>(*this);}

    //@{
    //! \name Indices, directions and handles

    //! \brief Convert (index, direction) to handle
    template<unsigned K>
    element_handle_t<CARTGRID, K> index2handle(index_type idx, unsigned dir) const 
    { return element_handle_t<CARTGRID, K>(maps[K][dir](idx) + offsets[K][dir]); }

    //! \brief Convert vertex index to handle
    typename base::vertex_handle get_vertex_handle(index_type idx) const { return index2handle<0>(idx,0);}
    //! \brief Convert cell index to handle
    typename base::cell_handle   get_cell_handle  (index_type idx) const { return index2handle<DIM>(idx,0);}
 
   //! \brief Convert (handle,direction) to index
    template<unsigned K>
    index_type handle2index(element_handle_t<CARTGRID, K> h, unsigned dir) const
    { return  (maps[K][dir](h.h() - offsets[K][dir])); }

    //! \brief Check handle for validity
    template<unsigned K>
    bool valid_handle(element_handle_t<CARTGRID, K> h)  const { return 0 <= h.h() && h.h() < (int) offsets[K].beyond();}
    //! \brief Check (direction, index) for validity
    template<unsigned K>
    bool valid_index(int dir, index_type idx) const { return maps[K][dir].valid(idx);}
    //! \brief Check vertex index for validity
    bool valid_vertex_index(vertex_index_type idx) const { return valid_index<0  >(0,idx);}
    //! \brief Check cell index for validity
    bool valid_cell_index  (cell_index_type   idx) const { return valid_index<dim>(0,idx);}
    //@}

    bool valid_dim(unsigned k) const { return k <= DIM;}
    void cvdim    (unsigned k) const { REQUIRE(valid_dim(k), "k=" << k << " DIM=" << DIM, 1);}

    void print(std::ostream& out) const;


    //@{
    /*! \name Archetype handling
        \brief See $GrAL ArchetypedGrid
     */
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
    //@}

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




  /*! \brief Subrange of a Cartesian grid

      \ingroup cartesianndmodule
      \see Test in  \ref test-cartesiannd-construct.C
   */
  template<unsigned DIM>
  class subrange : public grid_base<subrange<DIM>, DIM> {
    typedef grid_base<subrange<DIM>, DIM> base;
    typedef subrange<DIM>                 self;

  public:
    typedef grid<DIM> grid_type;
    typedef grid<DIM> base_grid_type;
    typedef grid<DIM-1> facet_grid_type;

    typedef typename base::index_type        index_type;
    typedef typename base::vertex_index_type vertex_index_type;

    typedef grid_types_base<grid_type, DIM> bgt;
    typedef typename bgt::Vertex  BaseVertex;
    typedef typename bgt::Cell    BaseCell;

  private:
    ref_ptr<grid_type const> g;
  public:
    //@{
    //! \name Constructors
    //! \brief Empty subrange
    subrange() {}
    //! \brief Subrange with vertex bounds \c [low,beyond[
    subrange(grid_type const&         gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond); }

    //! \brief Subrange with vertex bounds \c [low,beyond[
    subrange(ref_ptr<grid_type const> gg, vertex_index_type low, vertex_index_type beyond) : g(gg) 
    { init(low,beyond); }

    //! \brief Subrange with cell bounds \c [clow,chigh]
    subrange(grid_type         const& gg, BaseCell const& clow, BaseCell const& chigh)  : g(gg)
    { init(clow.index(), chigh.index() + index_type(2));}
    //! \brief Subrange with cell bounds \c [clow,chigh]
    subrange(ref_ptr<grid_type const> gg, BaseCell const& clow, BaseCell const& chigh)  : g(gg)
    { init(clow.index(), chigh.index() + index_type(2));}

    //! \brief Subrange with vertex bounds \c [vlow,vhigh]
    subrange(grid_type         const& gg, BaseVertex const& vlow, BaseVertex const& vhigh)  : g(gg)
    { init(vlow.index(), vhigh.index() + index_type(1));}
    //! \brief Subrange with vertex bounds \c [vlow,vhigh]
    subrange(ref_ptr<grid_type const> gg, BaseVertex const& vlow, BaseVertex const& vhigh)  : g(gg)
    { init(vlow.index(), vhigh.index() + index_type(1));}
    //@}

    //! \brief Underlying grid
    ref_ptr<grid_type const> BaseGrid() const { return g;}

    //! \brief Conversion of subrange element handle to handle of underlying grid
    template<unsigned K>
    element_handle_t<base_grid_type, K>  base_handle(element_handle_t<self, K> h, unsigned dir) const 
    { return BaseGrid()->template index2handle<K>(handle2index(h,dir), dir);}


  }; // class subrange<unsigned DIM>


  /*! \brief An N-dimensional Cartesian grid

      \ingroup cartesianndmodule
     
     \see Test in  \ref test-cartesiannd-construct.C
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
    enum { dim = DIM};
    //! \name Constructors
    //@{
    //! Empty grid
    grid() {}
    //! Grid with vertex indices \f$ x_i \in [0, b_i[ \f$ with \f$ b = \f$  \c beyond
    grid(vertex_index_type beyond) 
    { init(vertex_index_type(0), beyond);}
    //! Grid with vertex indices \f$ x_i \in [l_, b_i[ \f$ with \f$ l = \f$ \c  low and  \f$ b = \f$  \c beyond    
    grid(vertex_index_type low, vertex_index_type beyond)
    { init(low, beyond);}

    //! Grid with vertex indices \f$ (x_1,x_2) \in [0, b_1[\times[0,b_2[ \f$  (only 2D)
    grid(int b1, int b2);

    //! Grid with vertex indices \f$ (x_1,x_2,x_3) \in [0, b_1[\times[0,b_2[ \times[0,b_3[ \f$  (only 3D)
    grid(int b1, int b2, int b3);
    //@}

    ref_ptr<self const> BaseGrid() const { return ref_ptr<self const>(*this);}

    template<unsigned K>
    element_handle_t<self, K> base_handle(element_handle_t<self, K> h, unsigned dir) const { return h;}

  }; // class grid<unsigned DIM>




  //-----------------------------  archetype ---------------------------------

  template<class CARTGRID>
  struct  archetype_base_t {
    typedef grid_types<CARTGRID> gt;

    enum   { dim = CARTGRID::dim -1};
    typedef grid_dim_tag<dim> dimension_tag;

    unsigned dimension() const { return dim;}
    typedef typename gt::Facet  Cell;
    typedef typename gt::Vertex Vertex;
    typedef typename gt::facet_handle  cell_handle;
    typedef typename gt::vertex_handle vertex_handle;

    typedef typename gt::VertexOnCellIterator  VertexIterator; 
    typedef typename gt::FacetOnCellIterator   CellIterator; 
    typedef typename gt::VertexOnFacetIterator VertexOnCellIterator;

    typedef typename gt::Edge Edge;
    typedef typename gt::edge_handle edge_handle;
  };

  /*! \brief Archetype type for n-dimensional Cartesian grids
      \ingroup cartesianndmodule
      
      \todo The class does not work with copy algorithms using archetype isomorphisms,
      because only typedefs are used.
   */
  template<class CARTGRID>
  class archetype_t : public archetype_base_t<CARTGRID> {
    typedef grid_types<CARTGRID> gt;
    typedef archetype_base_t<CARTGRID> base;
    CARTGRID g; // single D-dimensional cell
  public:
    enum { dim = CARTGRID::dim -1};
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
    // typedef typename archetype_of_grid<CARTGRID, CARTGRID::dim-1>::archetype archetype_type;

    // create grid with 1 cell
    //    archetype_t() : g(vertex_index_type(0), vertex_index_type(1)) {}
    archetype_t() 
    {
      delta_map<CARTGRID::dim>::init();
      g.init(vertex_index_type(0), vertex_index_type(2));
      cg();
      REQUIRE_ALWAYS(g.NumOfCells() == 1, "g.NumOfCells()=" << g.NumOfCells(), 1);
      /*
       ::std::cerr << "Init archetype " << CARTGRID::dim << ": " 
		<< g.NumOfVertices() << " vertices, " << g.NumOfCells() << " cells" <<  ::std::endl;
      g.print( ::std::cerr);
      */
    }
    void print( ::std::ostream& out) const { g.print(out);}

    void cg() const {
      REQUIRE(g.NumOfCells() == 1, "g.NumOfCells()=" << g.NumOfCells(), 1);
    }
    // for having grid functions on archetype ...
    operator CARTGRID const&() const { cg(); return g;}

    CARTGRID const& BaseGrid() const { cg(); return g;}
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
	int d = delta_map<dim+1>::sd->dirs[1][e.direction()][0];
	idx_v[d] += 1;
	v = Vertex(v.TheGrid(), idx_v);
      }
    }

    static void switch_cell  (Vertex  const& f, Cell      & c) {
      index_type idx_f = f.index();
      index_type idx_c = c.index();
      int m = c.direction(); // in {0,1}
      int d_alt = delta_map<dim+1>::sd->dirs[1][m  ][0];
      int d_new = delta_map<dim+1>::sd->dirs[1][1-m][0];
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
  class sequence_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dim> {
    typedef grid_types_base<CARTGRID, CARTGRID::dim> base;
    typedef sequence_iterator_t<CARTGRID, K>         self;
  public:
    typedef CARTGRID                                 grid_type;
    enum { dim = K, codim = CARTGRID::dim -K, griddim = CARTGRID::dim};
    unsigned dimension()   const { return dim;}
    unsigned codimension() const { return codim;}

    typedef typename grid_type::base_grid_type  base_grid_type;
    typedef element_handle_t<base_grid_type,K>  element_handle;
    typedef element_handle_t<CARTGRID, K>       local_element_handle;

    typedef typename base::index_type        index_type;
    typedef tuple<int, K>                    local_index_type;

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
      REQUIRE(g.NumOfDirections(K) == 1, "", 1);
      init(idx,0);
    }
    void init(index_type idx, unsigned dir) {
      m = dir;
      h = g-> template index2handle<K>(idx,dir);
    }


    template<class CARTSUBRANGE>
    sequence_iterator_t(sequence_iterator_t<CARTSUBRANGE, K> const& rhs) 
      : g(rhs.TheGrid().BaseGrid()), 
	h(g->template index2handle<K>(rhs.index(),rhs.direction())),  m(rhs.direction())
    {}

    // initialization from subrange 
    // this will work only if element_handle and local_element_handle are the same.
    template<class CARTSUBRANGE>
    sequence_iterator_t(ref_ptr<CARTSUBRANGE const> rge, element_handle hh) : g(rge->BaseGrid()), h(hh) { init_m();}

    template<class CARTSUBRANGE>
    sequence_iterator_t(        CARTSUBRANGE const& rge, element_handle hh) : g(rge. BaseGrid()), h(hh) { init_m();}


    self&       operator++()       { cv(); ++h; advance_m(); return *this; }
    bool        IsDone()     const { cb(); return m >= g->maps[K].num_of_maps();}

    // this will use the implicit copy-constructor if value_type == self,
    // and the templated copy constructor otherwise.
    value_type  operator*()  const { cv(); return value_type(*this); } 
    // value_type  operator*()  const { cv(); return value_type(g->BaseGrid(), handle(), m);}

    local_element_handle local_handle() const { cb(); return h;}
    //! \c handle() return  unique element handle with respect to base grid
    element_handle     handle()    const { cb(); return TheGrid().base_handle(h,m);}
    unsigned           direction() const { cb(); return m;}

    //! for \f$ K \neq 0, D \f$, \c index() is not unique!
    index_type index()  const { cb(); return g->handle2index(h,m);} //  g->maps[K][m](h - g->offsets[K][m]);}
    index_type low_vertex_index() const { return index();}
    index_type high_vertex_index() const { 
      index_type res(index()); 
      res += delta_map<griddim>::template num2index<index_type>(K,m);
      /*
      for(int d = 0; d < dirs.size(); ++d)
	res[dirs[d]] += 1;
      */
      return res;
    }

    typedef typename base::archetype_type archetype_type;
    typedef typename base::Vertex         Vertex;
    typedef typename base::vertex_handle  vertex_handle;

    // this is useful only for cells, i.e. K=DIM
    Vertex        V(typename archetype_type::Vertex const& av) const { return Vertex(TheGrid(), index() + av.index());}
    vertex_handle v(typename archetype_type::Vertex const& av) const { return TheGrid().get_vertex_handle(index()+av.index());}

    Vertex        V(typename archetype_type::vertex_handle av) const 
    { return V(typename archetype_type::Vertex(TheArchetype(), av));}
    vertex_handle v(typename archetype_type::vertex_handle av) const 
    { return V(av).handle();}
    Vertex        V(int av) const { return V(typename archetype_type::vertex_handle(av));}
    vertex_handle v(int av) const { return v(typename archetype_type::vertex_handle(av));}


    // relative is the relative coordinates (offset) in the grid coordinates
    Vertex        V(index_type relative) const { return Vertex(TheGrid(), index() + relative);}
    vertex_handle v(index_type relative) const { return TheGrid().get_vertex_handle(index() + relative);}

    // local is offset on local coordinates of the element, i.e. has as many dimensions as  this->dimension()
    Vertex        Vl(local_index_type local) const { return V(full_index(local));}
    vertex_handle vl(local_index_type local) const { return v(full_index(local));}

    index_type full_index(local_index_type local) const {
      //if(dimension() == griddim)
      //	return local;
      //     else {
	index_type full(0);
	typename delta_map<griddim>::vector_system dir = delta_map<griddim>::num2vec(K,direction());
	for(unsigned i = 0; i < local.size(); ++i)
	  full[dir[i]] = local[i];
	return full;
	// }
    }

    ref_ptr<grid_type const> TheAnchor() const { return g;}
    // ref_ptr<grid_type const> TheGrid() const { return g;}
    grid_type      const& TheGrid()      const { return *g;}
    archetype_type const& TheArchetype() const { return TheGrid().ArchetypeOf(*this);}

    template<unsigned M>
    typename incidence_iterator_type<CARTGRID,K,M>::type FirstElement() const
    {  return typename incidence_iterator_type<CARTGRID,K,M>::type(*this);}
    template<unsigned M>
    typename incidence_iterator_type<CARTGRID,K,M>::type EndElement() const
    {  return typename incidence_iterator_type<CARTGRID,K,M>::type(*this,NumOfElements<M>());}


    typename incidence_iterator_type<CARTGRID,K,0>::type FirstVertex() const { return FirstElement<0>();}
    typename incidence_iterator_type<CARTGRID,K,1>::type FirstEdge  () const { return FirstElement<1>();}
    typename incidence_iterator_type<CARTGRID,K,2>::type FirstFace  () const { return FirstElement<2>();}
    typename incidence_iterator_type<CARTGRID,K,griddim-1>::type FirstFacet() const 
    { return FirstElement<griddim-1>();}
    typename incidence_iterator_type<CARTGRID,K,griddim>::type   FirstCell () const 
    { return FirstElement<griddim>();}
 
    typename incidence_iterator_type<CARTGRID,K,0>::type EndVertex() const { return EndElement<0>();}
    typename incidence_iterator_type<CARTGRID,K,1>::type EndEdge  () const { return EndElement<1>();}
    typename incidence_iterator_type<CARTGRID,K,2>::type EndFace  () const { return EndElement<2>();}
    typename incidence_iterator_type<CARTGRID,K,griddim-1>::type EndFacet() const 
    { return EndElement<griddim-1>();}
    typename incidence_iterator_type<CARTGRID,K,griddim>::type   EndCell () const 
    { return EndElement<griddim>();}



    template<unsigned M>
    unsigned NumOfElements() const;

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
    void init_m() { m = 0; advance_m(); }
    void advance_m() { while(m < g->NumOfDirections(K) && !valid_map()) ++m;}
    // is h a handle for the current map m?
    bool valid_map() const { return (unsigned)h.h() < g->offsets[K][m+1];}

  }; // class sequence_iterator_t<class CARTGRID, unsigned K>  

 template<class CARTGRID, unsigned K>
 inline bool operator==(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

 template<class CARTGRID, unsigned K>
 inline bool operator<(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return lhs.local_handle() < rhs.local_handle();} 

  //#ifndef GRAL_INCLUDES_RELOPS
  
 template<class CARTGRID, unsigned K>
 inline bool operator!=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return !(lhs==rhs);}
  
 template<class CARTGRID, unsigned K>
 inline bool operator>(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return  (rhs<lhs);}

 template<class CARTGRID, unsigned K>
 inline bool operator<=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return  !(rhs<lhs);}

 template<class CARTGRID, unsigned K>
 inline bool operator>=(sequence_iterator_t<CARTGRID,K> lhs, sequence_iterator_t<CARTGRID,K> rhs)
 { return  !(lhs<rhs);}
  // #endif


  //------------------------ incidence iterators -------------------------

  // <M>On<K>Iterator

  template<class CARTGRID, unsigned K, unsigned M>
  class downward_inc_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dim> {
  private:
    typedef CARTGRID                                 grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dim> base;
    typedef downward_inc_iterator_t<CARTGRID, K, M>  self;
  public:
    enum { dim = base::dim}; 
    typedef typename base::index_type                      index_type;
    typedef typename grid_type::base_grid_type             base_grid_type;
    typedef element_handle_t<base_grid_type,M>             element_handle;
    typedef typename element_type<base_grid_type, K, dim-K>::type anchor_type;
    typedef typename element_type<base_grid_type, M, dim-M>::type element_type;
    typedef element_type                                   value_type;
  private:
    anchor_type a;

    unsigned                    loc;

  public:
    downward_inc_iterator_t() {}
    explicit downward_inc_iterator_t(anchor_type const& aa, unsigned ll = 0) : a(aa), loc(ll) 
    { } //  ::std::cout << "downward_inc_iterator_t<" << K << "," << M << ">" <<  ::std::endl; }

    self& operator++()  { cv(); ++loc; return *this;}
    element_type operator*() const { return element_type(TheGrid(), handle());}
    
    element_handle handle()   const { cv(); return TheGrid().template index2handle<M>(index(), direction());}
    index_type index()        const { cv(); return a.index() + table()[loc].offset;}
    unsigned   direction()    const { cv(); return table()[loc].dir;}
    unsigned   local_handle() const { cb(); return loc;}

    bool IsDone() const { cb(); return loc >= table().size(); }
    
    ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    //  ref_ptr<grid_type   const> TheGrid()   const { return a.TheGrid();}
    grid_type const& TheGrid() const { return a.TheGrid();}

    bool bound() const { return a.valid();}
    bool valid() const { return bound() && ! IsDone();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  private:
    typedef typename delta_map<dim>::inc_descriptor incidences;
     ::std::vector<incidences> table() const { return delta_map<dim>::sd->incs[K][M][a.direction()];}
  };


 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator==(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator!=(downward_inc_iterator_t<CARTGRID,K, M> lhs, 
			downward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return !(lhs==rhs);}
  //#endif




  // <M>On<K>Iterator

  template<class CARTGRID, unsigned K, unsigned M>
  class upward_inc_iterator_t : public grid_types_base<CARTGRID, CARTGRID::dim> {
  private:
    typedef CARTGRID                                 grid_type;
    typedef grid_types_base<CARTGRID, CARTGRID::dim> base;
    typedef upward_inc_iterator_t<CARTGRID, K, M>    self;
  public:
    enum { dim = base::dim};
    typedef typename base::index_type                      index_type;
    typedef typename grid_type::base_grid_type             base_grid_type;
    typedef element_handle_t<base_grid_type,M>             element_handle;
    typedef typename element_type<base_grid_type, K, dim-K>::type anchor_type;
    typedef typename element_type<base_grid_type, M, dim-M>::type element_type;
    typedef element_type                                   value_type;
  private:
    anchor_type a;
    unsigned    loc;

  public:
    upward_inc_iterator_t() {}
    explicit upward_inc_iterator_t(anchor_type const&         aa, unsigned ll = 0) : a(aa), loc(ll) 
    {
      //  ::std::cout << "upward_inc_iterator_t<" << K << "," << M << ">" <<  ::std::endl; 
      advance_till_valid();
    }

    self& operator++()  { cv(); ++loc; advance_till_valid(); return *this;}
    element_type operator*() const { return element_type(TheGrid(), handle());}
    
    element_handle handle()   const { cv(); return TheGrid().template index2handle<M>(index(), direction());}
    index_type index()        const { cv(); return a.index() + table()[loc].offset;}
    unsigned   direction()    const { cv(); return table()[loc].dir;}
    unsigned   local_handle() const { cb(); return loc;}

    bool IsDone() const { cb(); return loc >= table().size(); }
    
    ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    //ref_ptr<grid_type   const> TheGrid()   const { return a.TheGrid();}
    grid_type const& TheGrid() const { return a.TheGrid();}

    bool bound() const { return a.valid();}
    bool valid() const { return bound() && ! IsDone();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
  private:
    void advance_till_valid() {
      while(! IsDone() && ! TheGrid().template valid_index<M>(table()[loc].dir, a.index()+table()[loc].offset))
	++loc;
    }
    typedef typename delta_map<dim>::inc_descriptor incidences;
     ::std::vector<incidences> const& table() const { return delta_map<dim>::sd->incs[K][M][a.direction()];}
  };


 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator==(upward_inc_iterator_t<CARTGRID,K, M> lhs, 
			upward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return lhs.local_handle() == rhs.local_handle();} 

  //#ifndef GRAL_INCLUDES_RELOPS
 template<class CARTGRID, unsigned K, unsigned M>
 inline bool operator!=(upward_inc_iterator_t<CARTGRID,K, M> lhs, 
			upward_inc_iterator_t<CARTGRID,K, M> rhs)
 { return !(lhs==rhs);}
  //#endif

} // namespace cartesiannd




template<unsigned DIM>
struct grid_types<cartesiannd::grid<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::grid<DIM>, DIM> >
{
  typedef cartesiannd::grid<DIM>          grid_type;
  typedef cartesiannd::subrange<DIM>      cartesian_subrange_type;
};

template<unsigned DIM>
struct grid_types<cartesiannd::archetype_t<cartesiannd::grid<DIM> > >
  : public grid_types_base<cartesiannd::archetype_base_t<cartesiannd::grid<DIM> > >
{
  typedef cartesiannd::archetype_t<cartesiannd::grid<DIM> > grid_type;
  typedef cartesiannd::archetype_t<cartesiannd::grid<DIM> > at;
  typedef typename at::archetype_iterator archetype_iterator;
  typedef typename at::archetype_handle   archetype_handle;
  typedef typename at::archetype_type     archetype_type;

  typedef grid_dim_tag<DIM-1> dimension_tag;
};

template<unsigned DIM>
struct grid_types<cartesiannd::archetype_t<cartesiannd::subrange<DIM> > >
  : public grid_types_base<cartesiannd::archetype_base_t<cartesiannd::subrange<DIM> > >
{
  typedef cartesiannd::archetype_t<cartesiannd::subrange<DIM> > grid_type;
  typedef cartesiannd::archetype_t<cartesiannd::subrange<DIM> > at;
  typedef typename at::archetype_iterator                       archetype_iterator;
  typedef typename at::archetype_handle                         archetype_handle;
  typedef typename at::archetype_type                           archetype_type;

  typedef grid_dim_tag<DIM-1> dimension_tag;
};


template<unsigned DIM>
struct grid_types<cartesiannd::subrange<DIM> > 
  : public grid_types_base<cartesiannd::grid_types_base<cartesiannd::subrange<DIM>, DIM> >
{
  typedef cartesiannd::subrange<DIM> grid_type;

  // subrange of subrange does currently not work.
  //  typedef cartesiannd::subrange<DIM> cartesian_subrange_type;
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
  typedef element_traits_facet_base<cartesiannd::grid<DIM> >  base;
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




//-----------------------  inline functions  ----------------------------

namespace cartesiannd {

  template<class CARTGRID, unsigned K>
  template<unsigned M>
  inline
  unsigned sequence_iterator_t<CARTGRID,K>::NumOfElements() const {
    if(M < K)
      return delta_map<griddim>::sd->incs[K][M][direction()].size();
    else {
      typename incidence_iterator_type<CARTGRID,K,M>::type it(FirstElement<M>());
      int cnt = 0;
      while(!it.IsDone()) {
	++it;
	++cnt;
      }
      return cnt; 
    }
  }

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
    //std::cout << "grid_base<CARTGRID,DIM>::init(" << "[" << low << "], [" << beyond << "[)\n";
    if(! delta_map<DIM>::initialized()) {
      //std::cout << "Initializing grid_base<" << DIM << ">" <<  std::endl;
      delta_map<DIM>::init();
    }
    // if(archetype_array == 0)
    if(archetype_initialized == 0) {
      archetype_initialized = new int(1);
      archetype_array = new archetype_type[1];
    }

    for(unsigned k = 0; k <= dim; ++k) {
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

  template<>
  inline 
  grid<2>::grid(int b1, int b2)         { init(vertex_index_type(0), vertex_index_type(b1,b2)); }

  template<>
  inline 
  grid<3>::grid(int b1, int b2, int b3) { init(vertex_index_type(0), vertex_index_type(b1,b2,b3)); }

} // namespace cartesiannd

} // namespace GrAL 

/*
template<class CARTGRID, unsigned K>
inline bool operator!=(GrAL::cartesiannd::sequence_iterator_t<CARTGRID,K>  lhs, 
		       GrAL::cartesiannd::sequence_iterator_t<CARTGRID,K>  rhs)
{ return !(lhs==rhs);}
*/

#endif
