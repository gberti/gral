#ifndef GRAL_GB_COMPLEXND_COMPLEXND_H
#define GRAL_GB_COMPLEXND_COMPLEXND_H

// $LICENSE_NEC

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include <vector>


namespace complexnd {

  // value indicating runtime variable dimension
  enum { ANY = -1};

  template<class GRID, int D, int CD = GRID::dim - D>
  class element_handle_t;

  template<class GRID, int D, int CD = GRID::dim - D>
  class element_t;

  template<class GRID, int D, int CD = GRID::dim - D>
  class element_iterator_t;

  template<class GRID, int D, int K, int CD = GRID::dim - D, int CK = GRID::dim - K>
  class incidence_iterator_t;

  template<int D>
  class ComplexND;

  template<int D>
  struct codim     { enum { codim0 = D,   codim1 = D-1}; };
  template<>
  struct codim<ANY> { enum {codim0 = ANY, codim1 = ANY}; };


  template<int D>
  struct grid_types_complexnd  {
    typedef ComplexND<D>                        grid_type;
    typedef element_handle_t<grid_type, ANY, ANY> any_element_handle;
    typedef element_handle_t<grid_type, 0, D>   vertex_handle;
    typedef element_handle_t<grid_type, 1, D-1> edge_handle;
    typedef element_handle_t<grid_type, 2, D-2> face_handle;

    typedef element_handle_t<grid_type, D-1, 1> facet_handle;
    typedef element_handle_t<grid_type, D,   0> cell_handle;

    typedef element_t<grid_type, ANY, ANY> AnyElement;
    typedef element_t<grid_type, 0, D>   Vertex;
    typedef element_t<grid_type, 1, D-1> Edge;
    typedef element_t<grid_type, 2, D-2> Face;

    typedef element_t<grid_type, D-1, 1> Facet;
    typedef element_t<grid_type, D,   0> Cell;

    typedef element_iterator_t<grid_type, ANY, ANY> AnyElementIterator;
    typedef element_iterator_t<grid_type, 0, D>   VertexIterator;
    typedef element_iterator_t<grid_type, 1, D-1> EdgeIterator;
    typedef element_iterator_t<grid_type, 2, D-2> FaceIterator;

    typedef element_iterator_t<grid_type, D-1, 1> FacetIterator;
    typedef element_iterator_t<grid_type, D,   0> CellIterator;
  }; 

  template<>
  struct grid_types_complexnd<ANY>  {
    typedef ComplexND<ANY>                        grid_type;
 
    typedef element_handle_t<grid_type, ANY, ANY> any_element_handle;

    typedef element_handle_t<grid_type, 0, ANY>   vertex_handle;
    typedef element_handle_t<grid_type, 1, ANY>   edge_handle;
    typedef element_handle_t<grid_type, 2, ANY>   face_handle;

    typedef element_handle_t<grid_type, ANY, 1>   facet_handle;
    typedef element_handle_t<grid_type, ANY, 0>   cell_handle;


    typedef element_t<grid_type, ANY, ANY> AnyElement;
    typedef element_t<grid_type, 0, ANY>   Vertex;
    typedef element_t<grid_type, 1, ANY>   Edge;
    typedef element_t<grid_type, 2, ANY>   Face;
 
    typedef element_t<grid_type, ANY, 1>   Facet;
    typedef element_t<grid_type, ANY, 0>   Cell;

    typedef element_iterator_t<grid_type, ANY, ANY> AnyElementIterator;
    typedef element_iterator_t<grid_type, 0, ANY> VertexIterator;
    typedef element_iterator_t<grid_type, 1, ANY> EdgeIterator;
    typedef element_iterator_t<grid_type, 2, ANY> FaceIterator;

    typedef element_iterator_t<grid_type, ANY, 1> FacetIterator;
    typedef element_iterator_t<grid_type, ANY, 0> CellIterator;
  }; 



  //--------------------- element handles ----------------------

  template<class GRID, int D, int CD = GRID::dim - D>
  struct element_handle_base {
    typedef GRID grid_type;
    typedef element_handle_base<GRID, D, CD> self;
    int h_;
  public:
    element_handle_base() {}
    element_handle_base(int hh) : h_(hh) {}
    
    int  h() const { return h_;}
    int& h()       { return h_;}

    self& operator++() { ++h_; return *this;}
  };

  template<class GRID, int D, int CD>
  struct element_handle_t : public element_handle_base<GRID, D, CD> 
  {
  private:
    typedef element_handle_t   <GRID, D, CD>  self;
    typedef element_handle_base<GRID, D, CD>  base;
  public:
    element_handle_t() {}
    explicit element_handle_t(int hh) : base(hh) {}
    explicit element_handle_t(element_handle_t<GRID,ANY,ANY> rhs) : base(rhs.h()) {}
  };

  // specialization for ANY
  template<class GRID>
  struct element_handle_t<GRID,ANY,ANY> : public element_handle_base<GRID,ANY,ANY> {
    typedef element_handle_t<GRID,ANY,ANY>      self;
    typedef element_handle_base<GRID,ANY,ANY>   base;
  public:
    element_handle_t() {}
    explicit element_handle_t(int hh) : base(hh) {}

    template<int D, int CD>
    explicit element_handle_t(element_handle_t<GRID,D,CD> const& rhs) : base(rhs.h()) {}

    template<int D, int CD>
    self & operator=(element_handle_t<GRID,D,CD> const& rhs) { base::operator=(rhs.h());}
  };

  template<class GRID, int D, int CD>
  inline std::ostream& operator<<(std::ostream& out, element_handle_t<GRID,D,CD>  e) { return (out << e.h());}
  template<class GRID, int D, int CD>
  inline std::istream& operator>>(std::istream& in,  element_handle_t<GRID,D,CD>& e) { return (in  >> e.h());}


  // comparison
  template<class GRID, int D, int CD>
  inline bool operator==(element_handle_t<GRID,D,CD>    lhs, element_handle_t<GRID,D,CD>   rhs) { return lhs.h() == rhs.h();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_handle_t<GRID,D,CD>    lhs, element_handle_t<GRID,D,CD>   rhs) { return lhs.h() != rhs.h();}

  // can compare any dimension to runtime dimension
  // TODO: REQUIRE that dimensions match
  template<class GRID, int D, int CD>
  inline bool operator==(element_handle_t<GRID,ANY,ANY>  lhs, element_handle_t<GRID,D,CD>   rhs) { return lhs.h() == rhs.h();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_handle_t<GRID,ANY,ANY>  lhs, element_handle_t<GRID,D,CD>   rhs) { return lhs.h() != rhs.h();}
  template<class GRID, int D, int CD>
  inline bool operator==(element_handle_t<GRID,D,CD>    lhs, element_handle_t<GRID,ANY,ANY> rhs) { return lhs.h() == rhs.h();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_handle_t<GRID,D,CD>    lhs, element_handle_t<GRID,ANY,ANY> rhs) { return lhs.h() != rhs.h();}

  // overload to disambiguate
  // TODO: REQUIRE that dimensions match
  template<class GRID>
  inline bool operator==(element_handle_t<GRID,ANY,ANY>  lhs, element_handle_t<GRID,ANY,ANY> rhs) { return lhs.h() == rhs.h();}
  template<class GRID>
  inline bool operator!=(element_handle_t<GRID,ANY,ANY>  lhs, element_handle_t<GRID,ANY,ANY> rhs) { return lhs.h() != rhs.h();}

  //--------------------------------------------------------------------------------------


  //--------------- mixin for handling element dimension ------------------------

  // primary template: dimension and codimension part of the type
  template<class ELEMENT, class GRID, int D, int CD>
  struct dimension_mixin {
    int dimension()   const { return D;}
    int codimension() const { return CD;}
  };
 
  // partial specialization: only dimension given (Vertex, Edge, Face)
  template<class ELEMENT, class GRID, int D>
  struct dimension_mixin<ELEMENT, GRID, D, ANY> {
  private:
    typedef GRID grid_t;
    grid_t const& the_grid() const { return static_cast<ELEMENT const*>(this)->TheGrid();}
  public:
    int dimension()   const { return D;}
    int codimension() const { return the_grid().dimension() - dimension();}
  };

  // partial specialization: only codimension given (Facet, Cell)
  template<class ELEMENT, class GRID, int CD>
  struct dimension_mixin<ELEMENT, GRID, ANY, CD> {
  private:
    typedef GRID grid_t;
    grid_t const& the_grid() const { return static_cast<ELEMENT const*>(this)->TheGrid();}
  public:
    int dimension()   const { return the_grid().dimension() - codimension();}
    int codimension() const { return CD;}
  };

  // partial specialization: neither dim nor codim given
  template<class ELEMENT, class GRID>
  struct dimension_mixin<ELEMENT, GRID, ANY, ANY> {
  private:
    typedef GRID grid_t;
    grid_t const& the_grid() const { return static_cast<ELEMENT const*>(this)->TheGrid();}
    int d;
  public:
    dimension_mixin()       : d(-1) {}
    dimension_mixin(int dd) : d(dd) {}

    int dimension()   const { return d;}
    int codimension() const { return the_grid().dimension() - dimension();}
  };



  //--------------- mixin for handling grid dimension ------------------------

  // in general, dimension of the grid is encoded in the type ...
  template<int D>
  struct dimension_mixin_grid {
    enum { dim = D };
    int dimension() const { return D;} 
  };

  // ... but for ANY, dimension is determined at runtime (object construction time)
  template<>
  struct dimension_mixin_grid<ANY> {
  private:
    int d;
  public:
    dimension_mixin_grid()       : d(-1) {}
    dimension_mixin_grid(int dd) : d(dd) {}
    int dimension() const { return d;}
  };
  

  //---------------------- the grid type ----------------------------

  /*! \brief General d-dimensional grid with full incidence information
      
      \author Guntram Berti

       ComplexND<N> is a N-dimensional grid.
       ComplexND<ANY> is a grid of construction-time determined dimension.

   */
  template<int D>
  class ComplexND : public grid_types_complexnd<D>, public dimension_mixin_grid<D> {
    typedef ComplexND               self;
    typedef dimension_mixin_grid<D> mixin;
    typedef grid_types_complexnd<D> gt;

    // data structure to hold all possible incidences
    typedef std::vector<int>                      incidence_sequence;       // [0, |I_{k,j}(e)| )
    typedef std::vector<incidence_sequence>       element_incidences;       // index j \in [0,d]
    typedef std::vector<element_incidences>       element_incidences_table; // index e \in [0, n(k)-1]
    typedef std::vector<element_incidences_table> incidence_table;          // index k \in [0,d]
    
    // incidences[k][e][j][i] gives the i-th incident element of dim j to the e-th element of dim k
    // if k==j, we have entries only for k==0 and k==dimension()
    incidence_table incidences;  

  public:
    // for D!=ANY => init incidences
    ComplexND() : incidences(dimension()+1) {}
    // allowed only for D==ANY, else compile-time error
    // made compilable here to allow for explicit instantiation for testing
    ComplexND(int dim) : incidences(dim+1) { REQUIRE_ALWAYS(dim==dimension(), "",1);}

    
    element_incidences const& Incidences(typename gt::AnyElement e) const { return incidences[e.dimension()][e.handle().h()];}
    template<int K, int CK>
    element_incidences const& Incidences(element_t<self,K,CK>    e) const { return incidences[e.dimension()][e.handle().h()];}


    template<int K, int CK> // = dim-K>
    element_iterator_t<self,K,CK> FirstElement() const;
    template<int K, int CK> // = dim-K>
    element_iterator_t<self,K,CK> EndElement() const;

    typedef typename gt::AnyElementIterator AnyElementIterator;
    AnyElementIterator FirstElement(unsigned dd) const;
    typename gt::AnyElementIterator EndElement  (unsigned dd) const;
   

    unsigned NumOfElements(unsigned k) const { return incidences[k].size();}

    unsigned NumOfVertices() const { return NumOfElements(0);}
    unsigned NumOfEdges()    const { return NumOfElements(1);}
    unsigned NumOfFaces()    const { return NumOfElements(2);}
    unsigned NumOfFacets()   const { return NumOfElements(dimension()-1);}
    unsigned NumOfCells()    const { return NumOfElements(dimension());}
  }; // class ComplexND<D>




  //------------------------------- elements and sequence iterators ---------------------------

  // base for elements and sequence iterators 
  template<class GRID, int D, int CD = GRID::dim - D>
  class element_base : public grid_types_complexnd<D>, public dimension_mixin<element_base<GRID,D,CD>, GRID, D, CD> 
  {
    typedef element_base<GRID,D,CD> self;
    typedef dimension_mixin<element_base<GRID,D,CD>, GRID, D, CD> mixin;
  public:
    typedef GRID                        grid_type;
    typedef element_handle_t<GRID,D,CD> element_handle_type;
  private:
    ref_ptr<grid_type const> g;
    element_handle_type      h;
  public:
    element_base() : g(0) {}
    // use these 2 only with D or CD != ANY
    element_base(grid_type const&         gg, element_handle_type hh = 0) : g(&gg), h(hh) {}
    element_base(ref_ptr<grid_type const> gg, element_handle_type hh = 0) : g(gg),  h(hh) {}
    // use only for D==CD==ANY
    element_base(grid_type const&         gg, unsigned dim, element_handle_type hh = 0) : mixin(dim), g(&gg), h(hh) {}
    element_base(ref_ptr<grid_type const> gg, unsigned dim, element_handle_type hh = 0) : mixin(dim), g(gg),  h(hh) {}

    grid_type const& TheGrid()   const { cb(); return *g;}
    grid_type const& TheAnchor() const { cb(); return *g;}

    element_handle_type handle() const { return h;}

    bool bound() const { return g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);}
    bool valid() const { return bound() && 0 <= h && h < g->NumOfElements(dimension());}
    void cv()    const { REQUIRE(valid(), "", 1);}
  protected:
    void inc() { ++h;}
  }; // class element_base<GRID,D,CD>


  // primary template. At least one of D and CD are fixed (!= ANY)
  template<class GRID, int D, int CD> // = GRID::dim - D>
  class element_t : public element_base<GRID,D,CD> {
    typedef element_t   <GRID,D,CD> self;
    typedef element_base<GRID,D,CD> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::element_handle_type element_handle_type;
  public:
    element_t() {}
    element_t(grid_type const&         gg, element_handle_type hh = 0) : base(gg,hh) {}
    element_t(ref_ptr<grid_type const> gg, element_handle_type hh = 0) : base(gg,hh) {}
    element_t       (element_t<GRID,ANY,ANY> const& rhs);
    self & operator=(element_t<GRID,ANY,ANY> const& rhs);
  }; // class element_t<GRID,D,CD>


  // specialization: neither dimension nor codimension are known
  template<class GRID>
  class element_t<GRID,ANY,ANY> : public element_base<GRID,ANY,ANY> {
    typedef element_t   <GRID,ANY,ANY> self;
    typedef element_base<GRID,ANY,ANY> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::element_handle_type element_handle_type;
  public:
    element_t() {}
    element_t(grid_type const&         gg, unsigned dim, element_handle_type hh = 0) : base(gg,dim,hh) {}
    element_t(ref_ptr<grid_type const> gg, unsigned dim, element_handle_type hh = 0) : base(gg,dim,hh) {}
    
    template<int D, int CD>
    element_t(element_t<GRID,D,CD> const& rhs) : base(rhs.TheGrid(), rhs.dimension(), rhs.handle()) {}
    template<int D, int CD>
    self& operator=(element_t<GRID,D,CD> const& rhs) {
      base::operator=(base(rhs.TheGrid(), rhs.dimension(), rhs.handle()));
      return *this;
    }
  }; // class element_t<GRID,ANY,ANY>



  //------------ constructors of element_t<> ----------------------
  
  template<class GRID, int D, int CD>
  inline
  element_t<GRID,D,CD>::element_t(element_t<GRID,ANY,ANY> const& rhs) 
    : base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h()))
  { REQUIRE(rhs.dimension() == D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); }


  template<class GRID, int D, int CD>
  inline
  element_t<GRID,D,CD>&  element_t<GRID,D,CD>::operator=(element_t<GRID,ANY,ANY> const& rhs) 
  {
    REQUIRE(rhs.dimension() == D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); 
    base::operator=(base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h())));
    return *this;
  }

  
  /*
  template<class GRID>
  template<int D, int CD>
  inline
  element_t<GRID,ANY,ANY>::element_t(element_t<GRID,D,CD> const& rhs) : base(rhs.TheGrid(), rhs.dimension(), rhs.handle()) {}

  template<class GRID>
  template<int D, int CD>
  inline
  element_t<GRID,ANY,ANY>& element_t<GRID,ANY,ANY>::operator=(element_t<GRID,D,CD> const& rhs) 
  {
    base::operator=(base(rhs.TheGrid(), rhs.dimension(), rhs.handle()));
    return *this;
  }
  */

  //------------------ comparison operators ------------------------

  // comparison
  template<class GRID, int D, int CD>
  inline bool operator==(element_t<GRID,D,CD> const&    lhs, element_t<GRID,D,CD> const&   rhs) { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_t<GRID,D,CD> const&    lhs, element_t<GRID,D,CD> const&   rhs) { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}

  // can compare any dimension to runtime dimension
  // TODO: REQUIRE that dimensions match
  template<class GRID, int D, int CD>
  inline bool operator==(element_t<GRID,ANY,ANY> const&  lhs, element_t<GRID,D,CD> const&   rhs) { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_t<GRID,ANY,ANY> const&  lhs, element_t<GRID,D,CD> const&   rhs) { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator==(element_t<GRID,D,CD> const&    lhs, element_t<GRID,ANY,ANY> const& rhs) { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_t<GRID,D,CD> const&    lhs, element_t<GRID,ANY,ANY> const& rhs) { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}

  // overload to disambiguate
  // TODO: REQUIRE that dimensions match
  template<class GRID>
  inline bool operator==(element_t<GRID,ANY,ANY> const&  lhs, element_t<GRID,ANY,ANY> const& rhs) { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID>
  inline bool operator!=(element_t<GRID,ANY,ANY> const&  lhs, element_t<GRID,ANY,ANY> const& rhs) { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}



  //------------------ sequence iterators ---------------------

  template<class GRID, int D, int CD> // = GRID::dim - D>
  class element_iterator_t : public element_base<GRID,D,CD> {
    typedef element_iterator_t<GRID,D,CD> self;
    typedef element_base      <GRID,D,CD> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::element_handle_type element_handle_type;
  public:
    typedef element_t<GRID,D,CD> element_type;
    typedef element_type         value_type;

    element_iterator_t() {}
    element_iterator_t(grid_type const&         gg, element_handle_type hh = 0) : base(gg,hh) {}
    element_iterator_t(ref_ptr<grid_type const> gg, element_handle_type hh = 0) : base(gg,hh) {}
    element_iterator_t(element_iterator_t<GRID,ANY,ANY> const& rhs);
    self & operator=  (element_iterator_t<GRID,ANY,ANY> const& rhs);

    bool IsDone() const { cb(); return valid();}
    self& operator++()  { cv(); inc(); return *this;}
    value_type operator*() const { cv(); return value_type(TheGrid(), handle());}
  }; // class element_iterator_t<GRID,D,CD>


  // specialization: neither dimension nor codimension are known
  template<class GRID>
  class element_iterator_t<GRID,ANY,ANY> : public element_base<GRID,ANY,ANY> {
    typedef element_iterator_t<GRID,ANY,ANY> self;
    typedef element_base      <GRID,ANY,ANY> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::element_handle_type element_handle_type;
  public:
    typedef element_t<GRID,ANY,ANY> element_type;
    typedef element_type            value_type;

    element_iterator_t() {}
    element_iterator_t(grid_type const&         gg, unsigned dim, element_handle_type hh = element_handle_type(0)) : base(gg,dim,hh) {}
    element_iterator_t(ref_ptr<grid_type const> gg, unsigned dim, element_handle_type hh = element_handle_type(0)) : base(gg,dim,hh) {}
    
    template<int D, int CD>
    element_iterator_t(element_iterator_t<GRID,D,CD> const& rhs)  : base(rhs.TheGrid(), rhs.dimension(), rhs.handle()) {}
    template<int D, int CD>
    self& operator=(element_iterator_t<GRID,D,CD> const& rhs) {
      base::operator=(base(rhs.TheGrid(), rhs.dimension(), rhs.handle()));
      return *this;
    }

    bool IsDone() const { cb(); return valid();}
    self& operator++()  { cv(); inc(); return *this;}
    value_type operator*() const { cv(); return value_type(TheGrid(), dimension(), handle());}
 }; // class element_iterator_t<GRID,ANY,ANY>

  //---------------- constructors for element_iterator_t -------------------------

  template<class GRID, int D, int CD>
  inline
  element_iterator_t<GRID,D,CD>::element_iterator_t(element_iterator_t<GRID,ANY,ANY> const& rhs) 
    : base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h()))
  { REQUIRE(rhs.dimension() == D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); }


  template<class GRID, int D, int CD>
  inline
  element_iterator_t<GRID,D,CD>&  element_iterator_t<GRID,D,CD>::operator=(element_iterator_t<GRID,ANY,ANY> const& rhs) 
  {
    REQUIRE(rhs.dimension() == D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); 
    base::operator=(base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h())));
    return *this;
  }

  /*
  template<class GRID>
  template<int D, int CD>
  inline
  element_iterator_t<GRID,ANY,ANY>::element_iterator_t(element_iterator_t<GRID,D,CD> const& rhs) 
    : base(rhs.TheGrid(), rhs.dimension(), rhs.handle()) {}

  template<class GRID>
  template<int D, int CD>
  inline
  element_iterator_t<GRID,ANY,ANY>& element_iterator_t<GRID,ANY,ANY>::operator=(element_iterator_t<GRID,D,CD> const& rhs) 
  {
    base::operator=(base(rhs.TheGrid(), rhs.dimension(), rhs.handle()));
    return *this;
  }
  */

  //------------------ comparison operators ------------------------

  // comparison
  template<class GRID, int D, int CD>
  inline bool operator==(element_iterator_t<GRID,D,CD>    const&  lhs, element_iterator_t<GRID,D,CD> const&   rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_iterator_t<GRID,D,CD>    const&  lhs, element_iterator_t<GRID,D,CD> const&   rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}

  // can compare any dimension to runtime dimension
  // TODO: REQUIRE that dimensions match
  template<class GRID, int D, int CD>
  inline bool operator==(element_iterator_t<GRID,ANY,ANY> const&  lhs, element_iterator_t<GRID,D,CD> const&   rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_iterator_t<GRID,ANY,ANY> const&  lhs, element_iterator_t<GRID,D,CD> const&   rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator==(element_iterator_t<GRID,D,CD> const&    lhs, element_iterator_t<GRID,ANY,ANY> const& rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID, int D, int CD>
  inline bool operator!=(element_iterator_t<GRID,D,CD> const&    lhs, element_iterator_t<GRID,ANY,ANY> const& rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}

  // overload to disambiguate
  // TODO: REQUIRE that dimensions match
  template<class GRID>
  inline bool operator==(element_iterator_t<GRID,ANY,ANY> const&  lhs, element_iterator_t<GRID,ANY,ANY> const& rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}
  template<class GRID>
  inline bool operator!=(element_iterator_t<GRID,ANY,ANY> const&  lhs, element_iterator_t<GRID,ANY,ANY> const& rhs) 
  { lhs.cb(); rhs.cb(); return lhs.handle() != rhs.handle();}



  //------------------ incidence iterators ---------------------

  // Element<K>OnElement<D>Iterator 
  // CD = codim(D, GRID), CK = codim(K, GRID) 
  template<class GRID, int D, int K, int CD, int CK>
  class incidence_iterator_t : public grid_types_complexnd<D> {
    typedef incidence_iterator_t<GRID, D, K, CD, CK> self;
  public:
    typedef GRID                          grid_type;
    typedef element_t<GRID, K, CK>        value_type;
    typedef element_handle_t<GRID, K, CK> value_handle_type;
    typedef element_t<GRID, D, CD>        anchor_type;
    typedef element_handle_t<GRID, D, CD> anchor_handle_type;
    typedef typename grid_type::incidence_sequence incidence_sequence;
  private:
    ref_ptr<grid_type const> g;
    int                      lh;
    anchor_handle_type       a;
    
  public:
    incidence_iterator_t() : g(0) {}
    incidence_iterator_t(anchor_type const& A) : g(A.TheGrid()), a(A.handle()), lh(0) {}

    self& operator++() { cv();  ++lh; return *this;}
    bool IsDone() const { cb(); return lh >= inc().size();}
    value_type        operator*() const { cv(); return value_type(TheGrid(), handle());}
    value_handle_type handle()    const { cv(); return value_handle_type(inc(lv));}
    unsigned value_dimension() const { return K;}

    anchor_handle_type anchor_handle() const { cb(); return a;}
    anchor_type        TheAnchor()     const { cb(); return anchor_type(TheGrid(), anchor_handle());}

    bool bound() const { return g != 0;}
    bool valid() const { return bound() && !IsDone();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "", 1);}

  private:
    incidence_sequence const& inc() const 
	{ cb(); return TheGrid().Incidence(dimension(), TheAnchor().handle())[value_dimension()];}
    };


  template<class GRID, int D, int K, int CD, int CK>
  inline bool operator==(incidence_iterator_t<GRID,D,K,CD,CK> const& lhs, incidence_iterator_t<GRID,D,K,CD,CK> const& rhs)
  { lhs.cb(); rhs.cb(); return lhs.handle() == rhs.handle();}

  template<class GRID, int D, int K, int CD, int CK>
  inline bool operator!=(incidence_iterator_t<GRID,D,K,CD,CK> const& lhs, incidence_iterator_t<GRID,D,K,CD,CK> const& rhs)
  { return !(lhs==rhs);}
 


  //------------------------------ inline members of ComplexND<D> ---------------------------------

  // no initialization possible for run-time determined dimension
  ComplexND<ANY>::ComplexND() {}
  ComplexND<ANY>::ComplexND(int dim) : mixin(dim),  incidences(dim+1) {}

  template<int D>
  template<int K, int CK>
  inline element_iterator_t<ComplexND<D>,K,CK>
  ComplexND<D>::FirstElement() const { return element_iterator_t<self,K,CK>(*this);}

  template<int D>
  template<int K, int CK>
  inline element_iterator_t<ComplexND<D>,K,CK> 
  ComplexND<D>::EndElement()   const { 
    element_handle_t<self,K,CK>  h = NumOfElements(K);
    return element_iterator_t<self,K,CK>(*this,h);
  }

  template<int D>
  inline 
  typename ComplexND<D>::AnyElementIterator
  ComplexND<D>::FirstElement(unsigned d) const { return AnyElementIterator(*this, d);}

  template<int D>
  inline 
  typename ComplexND<D>::AnyElementIterator
  ComplexND<D>::EndElement(unsigned d) const { return AnyElementIterator(*this, d, typename gt::any_element_handle(NumOfElements(d)));}
  
} // namespace complexnd

#endif
