#ifndef GRAL_GB_BASE_WRAP_ELEMENTS_H
#define GRAL_GB_BASE_WRAP_ELEMENTS_H

// $LICENSE_NEC

#include "Utility/ref-ptr.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/element-traits.h"
#include "Gral/Base/grid-function-hash.h"

namespace GrAL {


  template<class NEW_GRID, class OLD_EH>
  class wrapped_element_handle {
    typedef wrapped_element_handle<NEW_GRID, OLD_EH> self;
  public:    
    typedef NEW_GRID grid_type;
    typedef OLD_EH   base_element_handle;
    typedef typename grid_type::size_type size_type;
    

  public:
    base_element_handle h;

    wrapped_element_handle() {}
    explicit wrapped_element_handle(size_type hh) : h(hh)  {}
    wrapped_element_handle(base_element_handle hh) : h(hh) {}
    
    operator size_type()           const { return h; } // base_element_handle::operator size_type();}
    operator base_element_handle() const { return h; } // base_element_handle::operator size_type();}

    self& operator=(size_type hh) { h = hh; return *this;}
    self& operator++() { ++h; return *this;}
    self& operator+=(size_type d) { h+=d; return *this;}
    self& operator-=(size_type d) { h-=d; return *this;}
    
    bool operator==(self rhs) const { return h == rhs.h;}
    bool operator!=(self rhs) const { return h != rhs.h;}
    bool operator< (self rhs) const { return h <  rhs.h;}
  };

  template<class NEW_GRID, class OLD_EH>
  inline std::ostream& operator<<(std::ostream& out, wrapped_element_handle<NEW_GRID, OLD_EH>  h) { return out << h.h;}

  template<class NEW_GRID, class OLD_EH>
  inline std::istream& operator>>(std::ostream& in , wrapped_element_handle<NEW_GRID, OLD_EH>& h) { return in  >> h.h;}

} // namespace GrAL 
namespace STDEXT  {
  template<class T> struct hash;

  template<class NEW_GRID, class OLD_EH>
  struct  hash<GrAL::wrapped_element_handle<NEW_GRID, OLD_EH> > {
    typedef GrAL::wrapped_element_handle<NEW_GRID, OLD_EH> key_type;
    typedef GrAL::wrapped_element_handle<NEW_GRID, OLD_EH> argument_type;
    typedef size_t                                         result_type;
    size_t operator()(argument_type const& h) const { 
      hash<typename argument_type::base_element_handle> bh;
      return bh(h.h);
    }
  };

} // namespace STDEXT

namespace GrAL {

  /*! \defgroup element_type_wrapping Wrapping of grid entities

      \ingroup traits    

      The purpose of the components in this group is the easy reuse
      of element and iterator types of the base grid in a grid view.

      Of most interest to a user are the classes 
      \c wrap_all_element_types 
      and
      \c wrap_all_downward_incidence_iterator_types.
      Depending on the availability of a type in the base grid,
      its wrapped counterpart is defined for the grid view.
   */

  /*! \brief A type wrapper for a grid element

     \ingroup element_type_wrapping
   
     
  */
  /*
template<class NEW_GRID, class OLD_E, class NEW_EH>
class wrapped_element {
  typedef wrapped_element<NEW_GRID,OLD_E,NEW_EH> self;
public:
  typedef NEW_GRID grid_type;
  typedef OLD_E    base_element_type;
  typedef NEW_EH   element_handle;
  typedef element_traits<base_element_type> bet;
  typedef typename bet::element_type_tag element_type_tag;
private:
  ref_ptr<grid_type const> g;
  base_element_type        e;

public:
  wrapped_element() {}
  wrapped_element(grid_type const& gg, element_handle ee) : g(gg), e(gg.BaseGrid(), ee) {}
  wrapped_element(grid_type const& gg, base_element_type const& ee) : g(gg), e(ee) {}
  wrapped_element(ref_ptr<grid_type const> gg, element_handle ee) : g(gg), e(gg->BaseGrid(), ee) {}
  wrapped_element(ref_ptr<grid_type const> gg, base_element_type const& ee) : g(gg), e(ee) {}

  operator base_element_type() const { return e;}
  base_element_type     Base() const { return e;}
  element_handle        handle() const { return element_handle(e.handle());}

  // ref_ptr<grid_type const> TheGrid()   const { return g;}
  // ref_ptr<grid_type const> TheAnchor() const { return g;}
  grid_type const& TheGrid()   const { return *g;}
  grid_type const& TheAnchor() const { return *g;}

  bool operator==(self const& rhs) const { return e == rhs.e;}
  bool operator!=(self const& rhs) const { return e != rhs.e;}
  bool operator< (self const& rhs) const { return e <  rhs.e;}
};
  */



  
template<class NEW_GRID, class OLD_E, class NEW_EH>
class wrapped_element : public OLD_E {
  typedef wrapped_element<NEW_GRID, OLD_E, NEW_EH> self;
  typedef OLD_E                                    base;
public:
  typedef NEW_GRID grid_type;
  typedef OLD_E    base_element_type;
  typedef NEW_EH   element_handle;
  typedef element_traits<base_element_type> bet;
  typedef typename bet::element_type_tag element_type_tag;
private:
  ref_ptr<grid_type const> g;
  //  base_element_type        e;
  
public:
  wrapped_element() {}
  wrapped_element(grid_type const& gg, element_handle ee)                   : base(gg.BaseGrid(), ee),  g(gg) {}
  wrapped_element(grid_type const& gg, base_element_type const& ee)         : base(ee),                 g(gg) {}
  wrapped_element(ref_ptr<grid_type const> gg, element_handle ee)           : base(gg->BaseGrid(), ee), g(gg) {}
  wrapped_element(ref_ptr<grid_type const> gg, base_element_type const& ee) : base(ee),                 g(gg) {}

  operator base_element_type()   const { cb(); return Base();}
  base_element_type     Base()   const { cb(); return static_cast<base>(*this);}
  element_handle        handle() const { cb(); return element_handle(base::handle());}

  
  // ref_ptr<grid_type const> TheGrid()   const { return g;}
  // ref_ptr<grid_type const> TheAnchor() const { return g;}
  grid_type const& TheGrid()   const { cb(); return *g;}
  grid_type const& TheAnchor() const { cb(); return *g;}

  bool operator==(self const& rhs) const { cb(); return Base() == rhs.Base();}
  bool operator!=(self const& rhs) const { cb(); return Base() != rhs.Base();}
  bool operator< (self const& rhs) const { cb(); return Base() <  rhs.Base();}

  bool bound() const { return g != 0; }
  void cb()    const { REQUIRE(bound(), "", 1);}
};

  
  template <class GRID, class E, class EH>
  struct element_traits<wrapped_element<GRID,E,EH> >
    : public element_traits_base<wrapped_element<GRID,E,EH> > 
  {
    typedef element_traits_base<wrapped_element<GRID,E,EH> >  base;
    typedef element_traits<E> bet;
    typedef typename bet::hasher_type base_hasher_type;
    typedef typename base::ElementIterator ElementIterator;

    static ElementIterator FirstElement(typename base::grid_type const& g) { return GrAL::begin<ElementIterator>(g);}
    static ElementIterator EndElement  (typename base::grid_type const& g) { return GrAL::end  <ElementIterator>(g);}

    struct hasher_type : public base::hasher_type_elem_base
    {
      typedef  typename base::hasher_type_elem_base hb;
      typename hb::result_type operator()(wrapped_element<GRID,E,EH> e) const {
	base_hasher_type bh;
	return bh(e.Base());
      }
    };
  };
  

  
  template <class GRID, class E, class EH, class T>
  class grid_function<wrapped_element<GRID,E,EH>, T>
    : public grid_function_hash<wrapped_element<GRID,E,EH>, T>
  {
    typedef grid_function_hash<wrapped_element<GRID,E,EH>, T> base;
    typedef GRID                                              grid_type;
  public:
    grid_function() {}
    grid_function(grid_type const& g)                     : base(g)   {}
    grid_function(grid_type const& g, T const& t)         : base(g,t) {}
    grid_function(ref_ptr<grid_type const> g)             : base(g)   {}
    grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
  };
  

  /*! \brief A type wrapper for a grid incidence iterator

     \ingroup element_type_wrapping
  */
  template<class GRID, class ANCHOR, class E, class EH, class IT, class GT>
  class wrapped_incidence_iterator {
    typedef wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT> self;
    typedef GT           gt;
  public:
    //  typedef typename IT::category category;
    typedef grid_incidence_iterator_category category;
    typedef GRID   grid_type;
    typedef E      element_type;
    typedef EH     element_handle;
    typedef IT     base_iterator;
 
    typedef ANCHOR       anchor_type;
    typedef element_type value_type;

    typedef typename gt::size_type size_type;
  private:
    typedef typename element_type::element_type_tag  value_element_type_tag;
    typedef typename gt::archgt archgt;
    typedef typename GrAL::element_handle<archgt,value_element_type_tag>::type arch_element_handle;

  private:
    anchor_type    a;
    base_iterator it;
  public:
    wrapped_incidence_iterator() {}
    wrapped_incidence_iterator(anchor_type aa) : a(aa), it(a) {}
    wrapped_incidence_iterator(anchor_type aa, base_iterator ii) : a(aa), it(ii) {}
    wrapped_incidence_iterator(ref_ptr<anchor_type const> aa) : a(aa), it(a) {}
    wrapped_incidence_iterator(ref_ptr<anchor_type const> aa, base_iterator ii) : a(aa), it(ii) {}

    self& operator++() { cv(); ++it; return *this;}
    element_type operator*() const { cv(); return element_type(TheGrid(), handle());}
    bool IsDone() const { cb(); return it.IsDone();}

    // this works only if element_handle can be constructed from base_iterator::element_handle
    element_handle handle() const { cv(); return it.handle(); }
    // this may be wrong: handle1 -> size_type -> handle2 always works.
    //element_handle handle() const { return element_handle(it.handle());}

    arch_element_handle local_handle() const { cv(); return it.local_handle();}

    static self      begin(anchor_type const& aa)         { return self(aa, GrAL::begin<base_iterator>(aa.Base()));}
    static self      end  (anchor_type const& aa)         { return self(aa, GrAL::end  <base_iterator>(aa.Base()));}
    static size_type size (anchor_type const& aa)         { return          GrAL::size <base_iterator>(aa.Base());} 
    static self      begin(ref_ptr<anchor_type const> aa) { return self(aa, GrAL::begin<base_iterator>(aa->Base()));}
    static self      end  (ref_ptr<anchor_type const> aa) { return self(aa, GrAL::end  <base_iterator>(aa->Base()));}
    static size_type size (ref_ptr<anchor_type const> aa) { return          GrAL::size <base_iterator>(aa->Base());}


    bool bound() const { return it.bound();}
    bool valid() const { return it.valid();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}

    // ref_ptr<grid_type   const> TheGrid()   const { return ref_ptr<grid_type   const>(a.TheGrid());}
    // ref_ptr<anchor_type const> TheAnchor() const { return ref_ptr<anchor_type const>(a);}
    grid_type   const& TheGrid()   const { return ref(a.TheGrid());}
    anchor_type const& TheAnchor() const { return a;}


    bool operator==(self const& rhs) const { cb(); return it == rhs.it;}
    bool operator!=(self const& rhs) const { cb(); return it != rhs.it;}
    bool operator< (self const& rhs) const { cb(); return it <  rhs.it;}
  };



  /*! \brief A type wrapper for a grid sequence iterator

     \ingroup element_type_wrapping
  */
  template<class GRID, class E, class EH, class IT>
  class wrapped_sequence_iterator {
    typedef wrapped_sequence_iterator<GRID,E,EH,IT> self;
  public:
    //  typedef typename IT::category category;
    typedef grid_sequence_iterator_category category;
    typedef GRID   grid_type;
    typedef E      element_type;
    typedef EH     element_handle;
    typedef GRID   anchor_type;
    typedef IT     base_iterator;
    typedef grid_types<GRID> gt;
    typedef typename gt::size_type size_type;

    typedef element_type  value_type;
    typedef typename base_iterator::element_type base_element_type;
    typedef element_traits<base_element_type>  bet;
  private:
    ref_ptr<grid_type const> g;
    base_iterator            it;
  public:
    wrapped_sequence_iterator() {}
    wrapped_sequence_iterator(grid_type const& gg)                           : g(gg), it(*g) {}
    wrapped_sequence_iterator(grid_type const& gg, base_iterator ii)         : g(gg), it(ii) {}
    wrapped_sequence_iterator(ref_ptr<grid_type const> gg)                   : g(gg), it(*g) {}
    wrapped_sequence_iterator(ref_ptr<grid_type const> gg, base_iterator ii) : g(gg), it(ii) {}

    self& operator++()      { cv(); ++it; return *this;}
    bool  IsDone()    const { cb(); return it.IsDone();}
    element_handle handle   () const { cv(); return element_handle(it.handle());}
    element_type   operator*() const { cv(); return element_type(TheGrid(), handle());}

    static self      begin(anchor_type const& aa)         { return self(aa, GrAL::begin<base_iterator>(aa.BaseGrid()));}
    static self      end  (anchor_type const& aa)         { return self(aa, GrAL::end  <base_iterator>(aa.BaseGrid()));}
    static size_type size (anchor_type const& aa)         { return          GrAL::size <base_iterator>(aa.BaseGrid());} 
    static self      begin(ref_ptr<anchor_type const> aa) { return self(aa, GrAL::begin<base_iterator>(aa->BaseGrid()));}
    static self      end  (ref_ptr<anchor_type const> aa) { return self(aa, GrAL::end  <base_iterator>(aa->BaseGrid()));}
    static size_type size (ref_ptr<anchor_type const> aa) { return          GrAL::size <base_iterator>(aa->BaseGrid());}


    //  ref_ptr<grid_type const> TheGrid()   const { return g;}
    //  ref_ptr<grid_type const> TheAnchor() const { return g;}
    grid_type   const& TheGrid()   const { cb(); return *g;}
    anchor_type const& TheAnchor() const { cb(); return *g;}

    bool bound() const { return it.bound();}
    bool valid() const { return it.valid();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}


    bool operator==(self const& rhs) const { cb(); return it == rhs.it;}
    bool operator!=(self const& rhs) const { cb(); return it != rhs.it;}
    bool operator< (self const& rhs) const { cb(); return it <  rhs.it;}
  };



#define DEFINE_WRAP_BASE_ELEM(T, TH)					\
  template<int NEW_HAS, int OLD_HAS, class NEW_GT, class OLD_GT>	   \
  struct wrap_base_elem_aux_##T : public virtual NEW_GT {  };					\
  \
  template<class NEW_GT, class OLD_GT>					\
  struct wrap_base_elem_aux_##T<0,1,NEW_GT, OLD_GT> \
    : public virtual NEW_GT \
  { \
    /* typedef typename get_from_base_gt_##TH<NEW_GT, OLD_GT>::TH TH;	*/ \
    typedef int TH; \
   typedef wrapped_element<typename NEW_GT::grid_type, typename OLD_GT::T, TH>  T; }; \
  \
  template<class NEW_GT, class OLD_GT> \
  struct wrap_base_elem_##T : public wrap_base_elem_aux_##T   <has_##T<NEW_GT>::value, \
                                                               has_##T<OLD_GT>::value, \
                                                               NEW_GT,OLD_GT> \
  {};


  //  GRAL_DEFINE_ENTITY_FOR_ALL_GRID_ELEMENT_TYPES(DEFINE_WRAP_BASE_ELEM);
  
  DEFINE_WRAP_BASE_ELEM(Vertex, vertex_handle);
  DEFINE_WRAP_BASE_ELEM(Edge,   edge_handle);
  DEFINE_WRAP_BASE_ELEM(Face,   face_handle);
  DEFINE_WRAP_BASE_ELEM(Facet,  facet_handle);
  DEFINE_WRAP_BASE_ELEM(Cell,   cell_handle);
  

#undef  DEFINE_WRAP_BASE_ELEM

  /*! \brief Convenience class to wrap all undefined element types

     \ingroup element_type_wrapping
  */

  template<class NEW_GT, class OLD_GT>
  struct wrap_all_element_types_aux  : 
    public virtual wrap_base_elem_Vertex<NEW_GT, OLD_GT>,
    public virtual wrap_base_elem_Edge  <NEW_GT, OLD_GT>,
    public virtual wrap_base_elem_Face  <NEW_GT, OLD_GT>,
    public virtual wrap_base_elem_Facet <NEW_GT, OLD_GT>,
    public virtual wrap_base_elem_Cell  <NEW_GT, OLD_GT>
  {};


  template<class NEW_GT, class OLD_GT>
  struct wrap_all_element_types :
    public wrap_all_element_types_aux<dim_dep_defs<NEW_GT>, OLD_GT>
  {};
    


#define DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(A, E, EH, IT)	\
  \
  template<int NEW_HAS, int OLD_HAS, class NEW_GT, class OLD_GT>	   \
  struct wrap_base_incidence_iterator_aux_##IT : public virtual NEW_GT {};			\
  \
  template<class NEW_GT, class OLD_GT>					\
  struct wrap_base_incidence_iterator_aux_##IT<0,1,NEW_GT, OLD_GT>  \
    : public virtual NEW_GT \
  { \
    typedef wrapped_incidence_iterator<typename NEW_GT::grid_type, \
				       typename NEW_GT::A,	   \
				       typename NEW_GT::E,	   \
				       typename NEW_GT::EH,	   \
				       typename OLD_GT::IT, NEW_GT>  IT; };	\
  \
  template<class NEW_GT, class OLD_GT> \
  struct wrap_base_incidence_iterator_##IT : public wrap_base_incidence_iterator_aux_##IT   \
  <has_##IT<NEW_GT>::value,						\
   has_##IT<OLD_GT>::value,						\
   NEW_GT,OLD_GT> \
  {};




  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Vertex, Vertex, vertex_handle, VertexOnVertexIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Vertex, Edge,   edge_handle,   EdgeOnVertexIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Vertex, Face,   face_handle,   FaceOnVertexIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Vertex, Facet,  facet_handle,  FacetOnVertexIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Vertex, Cell,   cell_handle,   CellOnVertexIterator);

  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Edge, Vertex, vertex_handle,   VertexOnEdgeIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Edge, Face,   face_handle,     FaceOnEdgeIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Edge, Facet,  facet_handle,    FacetOnEdgeIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Edge, Cell,   cell_handle,     CellOnEdgeIterator);

  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Face, Vertex, vertex_handle,   VertexOnFaceIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Face, Edge,   edge_handle,     EdgeOnFaceIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Face, Facet,  facet_handle,    FacetOnFaceIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Face, Cell,   cell_handle,     CellOnFaceIterator);

  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Facet, Vertex, vertex_handle,  VertexOnFacetIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Facet, Edge,   edge_handle,    EdgeOnFacetIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Facet, Face,   face_handle,    FaceOnFacetIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Facet, Cell,   cell_handle,    CellOnFacetIterator);


  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Cell, Vertex, vertex_handle, VertexOnCellIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Cell, Edge,   edge_handle,   EdgeOnCellIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Cell, Face,   face_handle,   FaceOnCellIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Cell, Facet,  facet_handle,  FacetOnCellIterator);
  DEFINE_WRAP_BASE_INCIDENCE_ITERATOR(Cell, Cell,   cell_handle,   CellOnCellIterator);



#undef DEFINE_WRAP_BASE_INCIDENCE_ITERATOR


  /*! \brief Convenience class to lift all incidence iterator types of base grid 

     \ingroup element_type_wrapping


     In contrast to sequence iterators and upward incidence iterators 
     (like CellOnVertex), which leave the closure of a grid subrange,
     \e local grid entities like element or downward incidence iterators (VertexOnCell)
     can often be reused in a grid view (see for instance restricted_grid_view).
     However, directly using a typedef for the types of the underlying base grid poses two problems:
     - In general, we must use the typedef only if the concrete type 
       is defined  in the base grid
     - The typedef has the wrong signature, e.g. \c TheGrid() return the base grid, not the view.
     The solution is to use conditionally wrapped types of the base grid.

     Using this class, all downward incidence iterator types defined in \c OLD_GT
     and not in \c NEW_GT are wrapped and typedef'd, 
     as well as all needed element  and element handle types.

     A typical use is as follows:
     \code
     template<class Grid> class grid_view;

     template<class Grid>
     struct grid_types_grid_view_1 {

       typedef XXX Vertex;
       typedef YYY Cell;
       // some other defs.
     };

     // get the missing local types from grid_types<Grid>
     // uses Vertex and Cell from grid_types_grid_view_1,
     // and defines Edge, ... , VertexOnCellIterator, ...
     // as wrapper of the corresponding types in grid_types<Grid> if they exist.
     struct grid_types_grid_view_2 : 
      public wrap_all_downward_incidence_iterator_types<grid_types_grid_view_1<Grid>, grid_types<Grid> >
    {};

    // finally, as always we specialize grid_types for grid_view:
    template<class Grid>
    struct grid_types<grid_view<Grid> > : 
     public grid_types_base<grid_types_grid_view_2<Grid> > 
   {};

    \endcode
  */


  template<class NEW_GT, class OLD_GT, int D>
  struct wrap_all_downward_incidence_iterator_types_dim : 
    // public NEW_GT,
    public virtual wrap_base_incidence_iterator_VertexOnEdgeIterator<NEW_GT, OLD_GT>,

    public virtual wrap_base_incidence_iterator_VertexOnFaceIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_EdgeOnFaceIterator  <NEW_GT, OLD_GT>,

    public virtual wrap_base_incidence_iterator_VertexOnFacetIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_EdgeOnFacetIterator  <NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_FaceOnFacetIterator  <NEW_GT, OLD_GT>,

    public virtual wrap_base_incidence_iterator_VertexOnCellIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_EdgeOnCellIterator  <NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_FaceOnCellIterator  <NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_FacetOnCellIterator <NEW_GT, OLD_GT>
  { };

  template<class NEW_GT, class OLD_GT>
  struct wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,0> : 
    public NEW_GT {};

  template<class NEW_GT, class OLD_GT>
  struct wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,1> : 
    public wrap_base_incidence_iterator_VertexOnEdgeIterator<NEW_GT, OLD_GT>
  {
    typedef wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,1> base;

    typedef typename base::VertexOnEdgeIterator FacetOnCellIterator;
    typedef typename base::VertexOnEdgeIterator FacetOnEdgeIterator;
    typedef typename base::VertexOnEdgeIterator VertexOnCellIterator;
  };

  template<class NEW_GT, class OLD_GT>
  struct wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,2> : 
    public virtual wrap_base_incidence_iterator_VertexOnEdgeIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_VertexOnFaceIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_EdgeOnCellIterator  <NEW_GT, OLD_GT>
  {    
    typedef wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,2> base;

    typedef typename base::VertexOnEdgeIterator VertexOnFacetIterator;

    typedef typename base::EdgeOnCellIterator  EdgeOnFaceIterator;
    typedef typename base::EdgeOnCellIterator  FacetOnCellIterator;
    typedef typename base::EdgeOnCellIterator  FacetOnFaceIterator;

    typedef typename base::VertexOnFaceIterator VertexOnCellIterator;
  };


  template<class NEW_GT, class OLD_GT>
  struct wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,3> : 
    public virtual wrap_base_incidence_iterator_VertexOnEdgeIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_VertexOnFaceIterator<NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_VertexOnCellIterator<NEW_GT, OLD_GT>,

    public virtual wrap_base_incidence_iterator_EdgeOnFaceIterator  <NEW_GT, OLD_GT>,
    public virtual wrap_base_incidence_iterator_EdgeOnCellIterator  <NEW_GT, OLD_GT>,

    public virtual wrap_base_incidence_iterator_FaceOnCellIterator  <NEW_GT, OLD_GT>    
  {
    typedef wrap_all_downward_incidence_iterator_types_dim<NEW_GT, OLD_GT,3> base;

    typedef typename base::VertexOnFaceIterator  VertexOnFacetIterator;
    typedef typename base::EdgeOnFaceIterator    EdgeOnFacetIterator;
    typedef typename base::FaceOnCellIterator    FacetOnCellIterator;
  };


  template<class NEW_GT, class OLD_GT>
  struct wrap_all_downward_incidence_iterator_types :
    public wrap_all_downward_incidence_iterator_types_dim<wrap_all_element_types<NEW_GT,OLD_GT>, 
							  OLD_GT, 
							  NEW_GT::dimension_tag::dim> 
  {};



  // overloads of begin/end/size

  template<class GRID, class ANCHOR, class E, class EH, class IT, class GT>
  wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>
  gral_begin(ANCHOR const& a, wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>)
  { return wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>::begin(a);}

  template<class GRID, class ANCHOR, class E, class EH, class IT, class GT>
  wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>
  gral_end  (ANCHOR const& a, wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>)
  { return wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>::end(a);}

  template<class GRID, class ANCHOR, class E, class EH, class IT, class GT>
  typename wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>::size_type
  gral_size (ANCHOR const& a, wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>)
  { return wrapped_incidence_iterator<GRID,ANCHOR,E,EH,IT, GT>::size(a);}



  template<class GRID, class E, class EH, class IT>
  wrapped_sequence_iterator<GRID,E,EH,IT>
  gral_begin(GRID const& a, wrapped_sequence_iterator<GRID,E,EH,IT>)
  { return wrapped_sequence_iterator<GRID,E,EH,IT>::begin(a); } 

  template<class GRID, class E, class EH, class IT>
  wrapped_sequence_iterator<GRID,E,EH,IT>
  gral_end  (GRID const& a, wrapped_sequence_iterator<GRID,E,EH,IT>)
  { return wrapped_sequence_iterator<GRID,E,EH,IT>::end(a);}


  template<class GRID, class E, class EH, class IT>
  typename wrapped_sequence_iterator<GRID,E,EH,IT>::size_type
  gral_size (GRID const& a, wrapped_sequence_iterator<GRID,E,EH,IT>)
  { return wrapped_sequence_iterator<GRID,E,EH,IT>::size(a);}




} // namespace GrAL



#endif
