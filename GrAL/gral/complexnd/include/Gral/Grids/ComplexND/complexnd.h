#ifndef GRAL_GB_COMPLEXND_COMPLEXND_H
#define GRAL_GB_COMPLEXND_COMPLEXND_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/predicates.h"  // incident()

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include <vector>

/*! \file

*/

namespace GrAL {

/*! \brief Namespace for classes related to ComplexND, 
     a n-dimensional unstructured grid.

    Every class and function related to ComplexND is located in this namespace.

    \ingroup complexndstuff    
*/
namespace complexnd {

  //----------------------- forward declarations -------------------------

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


  //----------------------- grid_types_ComplexND ---------------------------


  // TODO: spec. for D=1 (no Face type) and D=0 (no Edge, Face, Facet types, no archetype type)
  // But we need them for defining e.g. FirstEdge() in ComplexND<> -> see solution in element_t<>

  template<int D>
  struct grid_types_ComplexND_base  {
    typedef ComplexND<D>                        grid_type;
    typedef grid_dim_tag<D>                     dimension_tag;

    typedef element_handle_t<grid_type, ANY, ANY> any_element_handle;
    typedef element_handle_t<grid_type, 0, D>   vertex_handle;
    typedef element_handle_t<grid_type, 1, D-1> edge_handle;
    //typedef element_handle_t<grid_type, 2, D-2> face_handle;

    typedef element_handle_t<grid_type, D-1, 1> facet_handle;
    typedef element_handle_t<grid_type, D,   0> cell_handle;

    typedef element_t<grid_type, ANY, ANY> AnyElement;
    typedef element_t<grid_type, 0, D>   Vertex;
    typedef element_t<grid_type, 1, D-1> Edge;
    // typedef element_t<grid_type, 2, D-2> Face;

    typedef element_t<grid_type, D-1, 1> Facet;
    typedef element_t<grid_type, D,   0> Cell;

    typedef element_iterator_t<grid_type, ANY, ANY> AnyElementIterator;

    typedef element_iterator_t<grid_type, 0, D>   VertexIterator;
    typedef element_iterator_t<grid_type, 1, D-1> EdgeIterator;
    // typedef element_iterator_t<grid_type, 2, D-2> FaceIterator;
    typedef element_iterator_t<grid_type, D-1, 1> FacetIterator;
    typedef element_iterator_t<grid_type, D,   0> CellIterator;


    typedef incidence_iterator_t<grid_type, ANY,ANY,ANY, ANY>   AnyIncidenceIterator; 

    typedef incidence_iterator_t<grid_type, 0, 0,   D, D>     VertexOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, 1,   D, D-1>   EdgeOnVertexIterator;
    // typedef incidence_iterator_t<grid_type, 0, 2,   D, D-2>   FaceOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, D-1, D, 1>     FacetOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, D,   D, 0>     CellOnVertexIterator;

    typedef incidence_iterator_t<grid_type, 1, 0,   D-1,D>     VertexOnEdgeIterator;
    // typedef incidence_iterator_t<grid_type, 1, 1, D-1,D-1>   EdgeOnEdgeIterator;
    // typedef incidence_iterator_t<grid_type, 1, 2,   D-1,D-2>   FaceOnEdgeIterator;
    typedef incidence_iterator_t<grid_type, 1, D-1, D-1,1>     FacetOnEdgeIterator;
    typedef incidence_iterator_t<grid_type, 1, D,   D-1,0>     CellOnEdgeIterator;
    /*
    typedef incidence_iterator_t<grid_type, 2, 0, D-2, D>      VertexOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, 1, D-2, D-1>    EdgeOnFaceIterator;
    //typedef incidence_iterator_t<grid_type, 2, 2, D-2, D-2>   FaceOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, D-1, D-2, 1>    FacetOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, D,   D-2, 0>    CellOnFaceIterator;
    */
    typedef incidence_iterator_t<grid_type, D-1, 0, 1,  D>     VertexOnFacetIterator;
    typedef incidence_iterator_t<grid_type, D-1, 1, 1,  D-1>   EdgeOnFacetIterator;
    // typedef incidence_iterator_t<grid_type, D-1, 2, 1,  D-2>   FaceOnFacetIterator;
    // typedef incidence_iterator_t<grid_type, D-1, D-1, 1,1> FacetOnFacetIterator;
    typedef incidence_iterator_t<grid_type, D-1, D, 1,  0>     CellOnFacetIterator;

    typedef incidence_iterator_t<grid_type, D, 0,   0, D>     VertexOnCellIterator;
    typedef incidence_iterator_t<grid_type, D, 1,   0, D-1>   EdgeOnCellIterator;
    // typedef incidence_iterator_t<grid_type, D, 2,   0, D-2>   FaceOnCellIterator;
    typedef incidence_iterator_t<grid_type, D, D-1, 0, 1>     FacetOnCellIterator;
    typedef incidence_iterator_t<grid_type, D, D,   0, 0>     CellOnCellIterator;

    // Face types
    typedef element_handle_t  <grid_type, 2, D-2> face_handle;
    typedef element_t         <grid_type, 2, D-2> Face;
    typedef element_iterator_t<grid_type, 2, D-2> FaceIterator;

    typedef incidence_iterator_t<grid_type, 0, 2,   D, D-2>    FaceOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 1, 2,   D-1,D-2>   FaceOnEdgeIterator;

    typedef incidence_iterator_t<grid_type, 2, 0, D-2, D>      VertexOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, 1, D-2, D-1>    EdgeOnFaceIterator;
    //typedef incidence_iterator_t<grid_type, 2, 2, D-2, D-2>   FaceOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, D-1, D-2, 1>    FacetOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, D,   D-2, 0>    CellOnFaceIterator;

    typedef incidence_iterator_t<grid_type, D-1, 2, 1,  D-2>   FaceOnFacetIterator;
    typedef incidence_iterator_t<grid_type, D, 2,   0, D-2>    FaceOnCellIterator;


    typedef unsigned                   archetype_handle;
    // not ok for dim = 0!
    typedef ComplexND<D-1>             archetype_type;
    typedef grid_types<archetype_type> archgt;
    typedef typename std::vector<archetype_type>::const_iterator  archetype_iterator;
  }; 

  // primary template
  template<int D>
  struct grid_types_ComplexND : public grid_types_ComplexND_base<D> 
  {
    /*
    typedef typename grid_types_ComplexND_base<D>::grid_type grid_type;

    // add face related types
    typedef element_handle_t  <grid_type, 2, D-2> face_handle;
    typedef element_t         <grid_type, 2, D-2> Face;
    typedef element_iterator_t<grid_type, 2, D-2> FaceIterator;

    typedef incidence_iterator_t<grid_type, 0, 2,   D, D-2>    FaceOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 1, 2,   D-1,D-2>   FaceOnEdgeIterator;

    typedef incidence_iterator_t<grid_type, 2, 0, D-2, D>      VertexOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, 1, D-2, D-1>    EdgeOnFaceIterator;
    //typedef incidence_iterator_t<grid_type, 2, 2, D-2, D-2>   FaceOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, D-1, D-2, 1>    FacetOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, D,   D-2, 0>    CellOnFaceIterator;

    typedef incidence_iterator_t<grid_type, D-1, 2, 1,  D-2>   FaceOnFacetIterator;
    typedef incidence_iterator_t<grid_type, D, 2,   0, D-2>    FaceOnCellIterator;
    */
  };

  // D = 1: No face type
  template<>
  struct grid_types_ComplexND<1> : public grid_types_ComplexND_base<1> {};

  // D = 0: no edge, face, facet
  template<>
  struct grid_types_ComplexND<0> : public grid_types_ComplexND_base<0> {
    /*
    typedef ComplexND<0>                          grid_type;
    typedef grid_dim_tag<0>                       dimension_tag;
    typedef element_handle_t<grid_type, ANY, ANY> any_element_handle;
    typedef element_handle_t<grid_type, 0, 0>     vertex_handle;
    typedef element_handle_t<grid_type, 0, 0>     cell_handle;

    typedef element_t<grid_type, ANY, ANY> AnyElement;
    typedef element_t<grid_type, 0, 0>     Vertex;
    typedef element_t<grid_type, 0, 0>     Cell;

    typedef element_iterator_t<grid_type, ANY, ANY> AnyElementIterator;
    typedef element_iterator_t<grid_type, 0, 0>     VertexIterator;
    typedef element_iterator_t<grid_type, 0, 0>     CellIterator;

    // no incidence iterators
    */

    // is this OK?
    typedef ComplexND<0>               archetype_type;
    typedef grid_types<archetype_type> archgt;
    typedef std::vector<archetype_type>::const_iterator  archetype_iterator;
  }; // grid_types_ComplexND<0>




  // specialization for ANY: runtime grid dimension
  template<>
  struct grid_types_ComplexND<ANY>  {
    typedef ComplexND<ANY>                        grid_type;
    typedef grid_dim_tag<ANY>                     dimension_tag; 
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

    typedef incidence_iterator_t<grid_type, ANY,ANY,ANY, ANY>   AnyIncidenceIterator; 
    typedef incidence_iterator_t<grid_type, 0, 0,   ANY, ANY>   VertexOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, 1,   ANY, ANY>   EdgeOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, 2,   ANY, ANY>   FaceOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, ANY, ANY, 1>     FacetOnVertexIterator;
    typedef incidence_iterator_t<grid_type, 0, ANY, ANY, 0>     CellOnVertexIterator;

    typedef incidence_iterator_t<grid_type, 1, 0,   ANY,ANY>   VertexOnEdgeIterator;
    // typedef incidence_iterator_t<grid_type, 1, 1, ANY,ANY>   EdgeOnEdgeIterator;
    typedef incidence_iterator_t<grid_type, 1, 2,   ANY,ANY>   FaceOnEdgeIterator;
    typedef incidence_iterator_t<grid_type, 1, ANY, ANY,1>     FacetOnEdgeIterator;
    typedef incidence_iterator_t<grid_type, 1, ANY, ANY,0>     CellOnEdgeIterator;

    typedef incidence_iterator_t<grid_type, 2, 0,   ANY,ANY>   VertexOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, 1,   ANY,ANY>   EdgeOnFaceIterator;
    //typedef incidence_iterator_t<grid_type, 2, 2, ANY, ANY>   FaceOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, ANY, ANY, 1>    FacetOnFaceIterator;
    typedef incidence_iterator_t<grid_type, 2, ANY, ANY, 0>    CellOnFaceIterator;

    typedef incidence_iterator_t<grid_type, ANY, 0,   1,  ANY>   VertexOnFacetIterator;
    typedef incidence_iterator_t<grid_type, ANY, 1,   1,  ANY>   EdgeOnFacetIterator;
    typedef incidence_iterator_t<grid_type, ANY, 2,   1,  ANY>   FaceOnFacetIterator;
    // typedef incidence_iterator_t<grid_type, ANY, ANY, 1,1> FacetOnFacetIterator;
    typedef incidence_iterator_t<grid_type, ANY, ANY, 1,  0>     CellOnFacetIterator;

    typedef incidence_iterator_t<grid_type, ANY, 0,   0, ANY>   VertexOnCellIterator;
    typedef incidence_iterator_t<grid_type, ANY, 1,   0, ANY>   EdgeOnCellIterator;
    typedef incidence_iterator_t<grid_type, ANY, 2,   0, ANY>   FaceOnCellIterator;
    typedef incidence_iterator_t<grid_type, ANY, ANY, 0, 1>     FacetOnCellIterator;
    typedef incidence_iterator_t<grid_type, ANY, ANY, 0, 0>     CellOnCellIterator;

    typedef unsigned                   archetype_handle;
    typedef ComplexND<ANY>             archetype_type;
    typedef grid_types<archetype_type> archgt;
    typedef std::vector<archetype_type>::const_iterator  archetype_iterator;
  };  // grid_types_ComplexND<ANY>




  //--------------------- element handles ----------------------

  template<class GRID, int D, int CD>
  struct element_handle_base {
    typedef GRID grid_type;
    typedef element_handle_base<GRID, D, CD> self;
    int     h_;
  public:
    element_handle_base() {}
    element_handle_base(unsigned hh) : h_(hh) {}
    
    int  h() const { return h_;}
    int& h()       { return h_;}
    
    operator int() const { return h_;} 
    self& operator++() { ++h_; return *this;}

    static int dimension()   { return  D;}
    static int codimension() { return CD;}
  };

  // primary template
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
    self & operator=(element_handle_t<GRID,D,CD> const& rhs) { base::operator=(rhs.h()); return *this;}
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





  //--------------- mixin for handling grid dimension ------------------------

  // in general, dimension of the grid is encoded in the type ...
  template<int D>
  struct dimension_mixin_grid {
    enum { dim = D };
    //! \brief Grid dimension
    unsigned dimension() const { return D;} 
  };

  // ... but for ANY, dimension is determined at runtime (object construction time)
  template<>
  struct dimension_mixin_grid<ANY> {
  private:
    unsigned d;
  public:
    enum { dim = ANY };

    dimension_mixin_grid()            : d(0) {}
    dimension_mixin_grid(unsigned dd) : d(dd) {}
    unsigned dimension() const { return d;}
  };

  template<int D, int K>
  struct d2c { enum { value = D-K};};
  template<int D, int CK>
  struct c2d { enum { value = D-CK};};
  template<int K>
  struct d2c<ANY,K> { enum { value = ANY};};
  template<int CK>
  struct c2d<ANY,CK> { enum { value = ANY};};

  /*! \brief Helper class to initialize ComplexND<0>
    \ingroup complexndstuff
  */  
  class pointcloud {
    unsigned nnodes;
  public:
    /*! \brief Construction with \c n  the number of vertices
      \pre 
         \f$ n \ge 0 \f$
     */
    explicit pointcloud(unsigned n = 0) : nnodes(n) {}

    /*! \invariant <tt> n == pointcloud(n).NumOfVertices() </tt>
     */
    unsigned NumOfVertices() const { return nnodes;}
  };

  /*! \brief Helper class to initialize ComplexND<1>
     
      \ingroup complexndstuff
  */  
  class polygon {
    unsigned nnodes;
  public:
    //! \brief Construction with n vertices
    explicit polygon(unsigned n = 0) : nnodes(n) {}

    //!
    unsigned NumOfVertices() const { return nnodes;}
    //!
    unsigned NumOfEdges()    const { return nnodes;}
  };





  //-----------------------------------------------------------------
  //---------------------- the grid type ----------------------------
  //-----------------------------------------------------------------


  /*! \brief General d-dimensional grid with full incidence information
      
      \author Guntram Berti

       <tt> ComplexND<N> </tt>   is a N-dimensional grid (dimension fixed at compile time). <br>
       <tt> ComplexND<ANY> </tt> is a grid with runtime determined dimension.
   
      \ingroup complexndstuff
      \todo Calculate incidence information
      \todo Specialization of switch operators for ANY
   */
  template<int D>
  class ComplexND : public grid_types_ComplexND<D>, public dimension_mixin_grid<D> {
    typedef ComplexND               self;
    typedef dimension_mixin_grid<D> mixin;
    typedef grid_types_ComplexND<D> gt;
  public:
    typedef typename gt::AnyElementIterator AnyElementIterator;
    typedef typename gt::VertexIterator     VertexIterator;
    typedef typename gt::EdgeIterator       EdgeIterator;
    typedef typename gt::FaceIterator       FaceIterator;
    typedef typename gt::FacetIterator      FacetIterator;
    typedef typename gt::CellIterator       CellIterator;

    typedef typename gt::Vertex             Vertex;
    typedef typename gt::Edge               Edge;
    typedef typename gt::Face               Face;
    typedef typename gt::Facet              Facet;
    typedef typename gt::Cell               Cell;

    typedef typename gt::vertex_handle      vertex_handle;
    typedef typename gt::edge_handle        edge_handle;
    typedef typename gt::face_handle        face_handle;
    typedef typename gt::facet_handle       facet_handle;
    typedef typename gt::cell_handle        cell_handle;

    typedef typename gt::FacetOnCellIterator FacetOnCellIterator;

    typedef typename gt::archetype_handle   archetype_handle;
    typedef typename gt::archetype_type     archetype_type;
    typedef typename gt::archetype_iterator archetype_iterator;

    // data structure to hold all possible incidences
    // TODO: reordered indexing incidences[d1][d2][e1][e2] would be more efficient
    // typedef std::vector<unsigned>                 incidence_sequence;       // [0, |I_{k,j}(e)| )
    typedef std::vector<int>                      incidence_sequence;       // [0, |I_{k,j}(e)| )
    typedef std::vector<incidence_sequence>       element_incidences;       // index j \in [0,d]
    typedef std::vector<element_incidences>       element_incidences_table; // index e \in [0, n(k)-1]
    typedef std::vector<element_incidences_table> incidence_table;          // index k \in [0,d]
   
    // incidences[k][e][j][i] gives the i-th incident element of dim j to the e-th element of dim k
    // if k==j, we have entries only for k==0 and k==dimension()
    using mixin::dimension;
  public:
    incidence_table incidences;  

  private:
    std::vector<unsigned>       cell_archetype;
    std::vector<archetype_type> archetypes;

    /*
    template<class G_SRC, class PHI> 
    friend void ConstructGrid0(ComplexND<D>& g, G_SRC const& g_src, PHI& phi);
    */
  public:
    /*! @name Constructors
        \brief for <tt> D != ANY</tt>  
        \internal init incidences
    */
    //@{ 
    ComplexND() : incidences(dimension()+1) {}
  
    /*! \brief Only for <tt> D==ANY </tt>, else compile-time error
       \internal
        made compilable here to allow for explicit instantiation for testing
    */
    explicit ComplexND(unsigned dim) : incidences(dim+1) { REQUIRE_ALWAYS(dim==dimension(), "",1);}
      
    //! only for <tt>D==0</tt>
    explicit ComplexND(pointcloud p);

    //! only for  <tt>D==1</tt>
    explicit ComplexND(polygon pn);
    //! Only for  <tt>D==1</tt>
    ComplexND(int a[][2], int N);
    //@}

    element_incidences const& Incidences(typename gt::AnyElement e) const { return incidences[e.dimension()][e.handle().h()];}
    template<int K, int CK>
    element_incidences const& Incidences(element_t<self,K,CK>    e) const { return incidences[e.dimension()][e.handle().h()];}
    element_incidences const& Incidences(unsigned d, unsigned h) const { return incidences[d][h];}

    /*! \name Dimension-independent sequence iteration (compiletime defined dimension) 
     \brief \c K is the dimension, \c CK is the codimension of the element. 
      \pre 
          if <tt>D!=ANY</tt> we must have  <tt> K+CK==D </tt>
    */
    /*@{*/ 
    template<int K, int CK> // = dim-K>
    element_iterator_t<self,K,CK> FirstElement() const; 
    template<int K, int CK> // = dim-K>
    element_iterator_t<self,K,CK> EndElement() const;
    /*@}*/

    /*! \name Dimension-independent sequence iteration (runtime defined dimension)
        \brief \c d is the dimension of the element.
    */
    //@{ 
    AnyElementIterator FirstElement(unsigned dd) const;
    AnyElementIterator EndElement  (unsigned dd) const;
    unsigned NumOfElements(unsigned k) const { return incidences[k].size();}
    //@}

    /*! \name Dimension-dependent sequence iteration
        \brief Classical sequence iterator interface
    */
    //@{ 
    VertexIterator FirstVertex() const;
    EdgeIterator   FirstEdge()   const;
    FaceIterator   FirstFace()   const;
    FacetIterator  FirstFacet()  const;
    CellIterator   FirstCell()   const;


    unsigned NumOfVertices() const { return NumOfElements(0);}
    unsigned NumOfEdges()    const { return NumOfElements(1);}
    unsigned NumOfFaces()    const { return NumOfElements(2);}
    unsigned NumOfFacets()   const { return NumOfElements(dimension()-1);}
    unsigned NumOfCells()    const { return NumOfElements(dimension());}
    //@}

    Vertex switched_vertex(Vertex const& v, Edge const& e) const {
      return Vertex(*this, vertex_handle(v.handle() == Incidences(e)[0][0] 
					 ? Incidences(e)[0][1] 
					 : Incidences(e)[0][0])); 
    }

    template<int K>
    element_t<ComplexND<D>,K,D-K> switched_element(element_t<ComplexND<D>,K-1,D-(K-1)> const& e_k_1,
						   element_t<ComplexND<D>,K  ,D- K   > const& e_k,
						   element_t<ComplexND<D>,K+1,D-(K+1)> const& e_k1) const;
    /*
    template<int K>
    element_t<self,K,dim-K> switched_element(element_t<self,K-1,dim-K+1> const& e_k_1,
					     element_t<self,K  ,dim-K  > const& e_k,
					     element_t<self,K+1,dim-K-1> const& e_k1) const;
    */
    Edge  switched_edge (Vertex                  const& v, Edge  const& e, Face                    const& f) const; 
    Face  switched_face (Edge                    const& e, Face  const& f, element_t<self,3,dim-3> const& c) const;
    Facet switched_facet(element_t<self,dim-2,2> const& r, Facet const& f, Cell                    const& c) const;


    Cell switched_cell(Facet const& f, Cell const& c) const {
      return Cell(*this, 
		  cell_handle((c.handle() == Incidences(f)[dimension()][0]
			       ? Incidences(f)[dimension()][1] 
			       : Incidences(f)[dimension()][0])));
    }


    // must make them public because friend definition does not work
    // private:

    // modifying functions
    void calculate_incidences() {}

    cell_handle      add_cell(archetype_handle a) 
    { ca(a); cell_archetype.push_back(a); return cell_handle(cell_archetype.size() -1);}
    archetype_handle add_archetype() { return add_archetype(archetype_type());}
    archetype_handle add_archetype(archetype_type const& A)
    { archetypes.push_back(A); return (archetypes.size() -1); }

    template<class VIt>
    cell_handle     add_cell(archetype_handle a, VIt begin, VIt end) 
    { 
      ca(a);
      cell_handle  new_cell(cell_archetype.size());
      cell_archetype.push_back(a);
      incidences[dimension()].push_back(element_incidences(dimension()));
      unsigned nv = std::distance(begin,end);
      incidences[dimension()][new_cell][0].resize(nv);
      for(unsigned v = 0; v < nv; ++v, ++begin)
	incidences[dimension()][new_cell][0][v] = *begin;
      return new_cell;
    }
     
    void set_num_of_vertices(unsigned nv) 
    { REQUIRE_ALWAYS(NumOfVertices() == 0, "", 1);  incidences[0].resize(nv);}

  public:
    /*! @name Archetype handling
       \brief Access to archetypes
    */
    //@{ 
    archetype_type      & Archetype(archetype_handle a)       { ca(a); return archetypes[a]; }
    archetype_type const& Archetype(archetype_handle a) const { ca(a); return archetypes[a]; }

    bool valid_archetype(archetype_handle a) const  { return ((0 <= a) && (a < archetypes.size())); }

    inline
    archetype_type const& ArchetypeOf(Cell const&  c) const;
    archetype_type const& ArchetypeOf(cell_handle  c) const { return archetypes[cell_archetype[c]];}
    archetype_handle      archetype_of(cell_handle c) const { return cell_archetype[c];}
    
    archetype_iterator BeginArchetype() const { return archetypes.begin();}
    archetype_iterator   EndArchetype() const { return archetypes.end  ();}
    //@} 
  private:
    void ca(archetype_handle a) const { REQUIRE(valid_archetype(a), "a = " << a,1); }

  public:
    cell_handle outer_cell_handle()   const { return cell_handle(-1);}
    cell_handle invalid_cell_handle() const { return cell_handle(-1);}
    bool IsOnBoundary(FacetOnCellIterator const& fc) const { return IsOnBoundary(*fc);}
    bool IsOnBoundary(Facet               const& f)  const { 
      return Incidences(f)[dimension()][0] == outer_cell_handle() 
	||   Incidences(f)[dimension()][1] == outer_cell_handle();
    }

  }; // class ComplexND<D>







  //-------------------------------------------------------------------------------------------
  //------------------------------- elements and sequence iterators ---------------------------
  //-------------------------------------------------------------------------------------------


  //--------------- mixin for handling element dimension ------------------------

  // primary template: dimension and codimension part of the type
  template<class ELEMENT, class GRID, int D, int CD>
  struct dimension_mixin {
    unsigned dimension()   const { return D;}
    unsigned codimension() const { return CD;}
  };
 
  // partial specialization: only dimension given (Vertex, Edge, Face)
  template<class ELEMENT, class GRID, int D>
  struct dimension_mixin<ELEMENT, GRID, D, ANY> {
  private:
    typedef GRID grid_t;
    grid_t const& the_grid() const { return static_cast<ELEMENT const*>(this)->TheGrid();}
  public:
    unsigned dimension()   const { return D;}
    unsigned codimension() const { return the_grid().dimension() - dimension();}
  };

  // partial specialization: only codimension given (Facet, Cell)
  template<class ELEMENT, class GRID, int CD>
  struct dimension_mixin<ELEMENT, GRID, ANY, CD> {
  private:
    typedef GRID grid_t;
    grid_t const& the_grid() const { return static_cast<ELEMENT const*>(this)->TheGrid();}
  public:
    unsigned dimension()   const { return the_grid().dimension() - codimension();}
    unsigned codimension() const { return CD;}
  };

  // partial specialization: neither dim nor codim given
  template<class ELEMENT, class GRID>
  struct dimension_mixin<ELEMENT, GRID, ANY, ANY> {
  private:
    typedef GRID grid_t;
    grid_t const& the_grid() const { return static_cast<ELEMENT const*>(this)->TheGrid();}
    unsigned d;
  public:
    dimension_mixin()            : d( 0) {}
    dimension_mixin(unsigned dd) : d(dd) {}

    unsigned dimension()   const { return d;}
    unsigned codimension() const { return the_grid().dimension() - dimension();}
  };




  //--------------------  base for elements and sequence iterators -----------------------


  template<class GRID, int D, int CD = GRID::dim - D>
  class element_base_t : public grid_types_ComplexND<D>, public dimension_mixin<element_base_t<GRID,D,CD>, GRID, D, CD> 
  {
    typedef element_base_t<GRID,D,CD> self;
    typedef dimension_mixin<element_base_t<GRID,D,CD>, GRID, D, CD> mixin;
  public:
    typedef GRID                        grid_type;
    typedef element_handle_t<GRID,D,CD> element_handle_type;
    using mixin::dimension;
  private:
    ref_ptr<grid_type const> g;
    element_handle_type      h;
  public:
    element_base_t() : g(0) {}
    // use these 2 only with D or CD != ANY
    element_base_t(grid_type const&         gg, element_handle_type hh = 0) : g(gg), h(hh) {}
    element_base_t(ref_ptr<grid_type const> gg, element_handle_type hh = 0) : g(gg),  h(hh) {}
    // use only for D==CD==ANY
    element_base_t(grid_type const&         gg, unsigned dim, element_handle_type hh = 0) : mixin(dim), g(gg), h(hh) { checkdim();}
    element_base_t(ref_ptr<grid_type const> gg, unsigned dim, element_handle_type hh = 0) : mixin(dim), g(gg),  h(hh) { checkdim();}

    void checkdim() const    
    { 
      REQUIRE(dimension() <= TheGrid().dimension(), 
	      "dim=" << dimension() << "griddim="  << TheGrid().dimension(), 1); 
    }

    grid_type const& TheGrid()   const { cb(); return *g;}
    grid_type const& TheAnchor() const { cb(); return *g;}

    element_handle_type handle() const { return h;}



    template<int K, int CK> 
    unsigned NumOfElements()           const { return NumOfElements(K);}
    unsigned NumOfElements(unsigned K) const { cv(); return g->Incidences(dimension(), handle())[K].size();}

    unsigned NumOfVertices() const { return NumOfElements(0);}
    unsigned NumOfEdges()    const { return NumOfElements(1);}
    unsigned NumOfFaces()    const { return NumOfElements(2);}
    unsigned NumOfFacets()   const { return NumOfElements(dimension()-1);}
    unsigned NumOfCells()    const { return NumOfElements(dimension());}

    bool bound() const { return g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);}
    bool valid() const { return bound() && 0 <= h && h < (int)g->NumOfElements(dimension());}
    void cv()    const { REQUIRE(valid(), "", 1);}
  protected:
    void incr() { ++h;}
  }; // class element_base_t<GRID,D,CD>

  // checking templates fo ensure dimensions are consistent
  template<int D1, int D2> struct check_same_number {};
  template<int D>          struct check_same_number<D,D>   { typedef int ok;};

  // general case with fixed dimensions: OK <=> D_GRID = D_ELEM + CD_ELEM
  template<int D_GRID, int D_ELEM, int CD_ELEM> struct check_consistence
  { typedef typename check_same_number<D_GRID, D_ELEM+CD_ELEM>::ok ok;};
  // case with runtime dimensions: OK if at least one of D_ELEM, CD_ELEM = ANY
  template<int D_ELEM, int CD_ELEM> struct check_consistence<ANY, D_ELEM, CD_ELEM> {};
  template<int D_ELEM             > struct check_consistence<ANY, D_ELEM, ANY    > { typedef int ok;};
  template<            int CD_ELEM> struct check_consistence<ANY, ANY   , CD_ELEM> { typedef int ok;};
  template<                       > struct check_consistence<ANY, ANY   , ANY    > { typedef int ok;};

  // primary template. At least one of D and CD are fixed (!= ANY)
  template<class GRID, int D, int CD> // = GRID::dim - D>
  class element_t : public element_base_t<GRID,D,CD> {
    // enum { d1 = GRID::dim, d2 = D+CD};
    typedef typename check_consistence<GRID::dim,D,CD>::ok ok_type;
    typedef element_t     <GRID,D,CD> self;
    typedef element_base_t<GRID,D,CD> base;
    typedef typename base::element_handle_type element_handle_type;
  public:
    typedef typename base::grid_type grid_type;
  public:
    element_t() {}
    element_t(grid_type const&         gg, element_handle_type hh = 0) : base(gg,hh) {}
    element_t(ref_ptr<grid_type const> gg, element_handle_type hh = 0) : base(gg,hh) {}
    element_t       (element_t<GRID,ANY,ANY> const& rhs);
    self & operator=(element_t<GRID,ANY,ANY> const& rhs);

    template<int K, int CK>
    incidence_iterator_t<grid_type, D, K,   CD, CK>  FirstElement() const;
    incidence_iterator_t<grid_type, D, ANY, CD, ANY> FirstElement(unsigned k) const;

    incidence_iterator_t<grid_type, D, 0, CD, d2c<D,0>::value>  FirstVertex() const; 
    incidence_iterator_t<grid_type, D, 1, CD, d2c<D,1>::value>  FirstEdge() const; 
    incidence_iterator_t<grid_type, D, 2, CD, d2c<D,2>::value>  FirstFace() const; 

    incidence_iterator_t<grid_type, D, c2d<D,1>::value, CD, 1>  FirstFacet() const; 
    incidence_iterator_t<grid_type, D, c2d<D,0>::value ,CD, 0>  FirstCell() const; 

  }; // class element_t<GRID,D,CD>


  // specialization: neither dimension nor codimension are known
  template<class GRID>
  class element_t<GRID,ANY,ANY> : public element_base_t<GRID,ANY,ANY> {
    typedef element_t     <GRID,ANY,ANY> self;
    typedef element_base_t<GRID,ANY,ANY> base;
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

    template<int K, int CK>
    incidence_iterator_t<grid_type, ANY, K,   ANY, CK>  FirstElement() const;
    incidence_iterator_t<grid_type, ANY, ANY, ANY, ANY> FirstElement(unsigned k) const;


    incidence_iterator_t<grid_type, ANY, 0,   ANY, ANY>  FirstVertex() const; 
    incidence_iterator_t<grid_type, ANY, 1,   ANY, ANY>  FirstEdge() const; 
    incidence_iterator_t<grid_type, ANY, 2,   ANY, ANY>  FirstFace() const; 

    incidence_iterator_t<grid_type, ANY, ANY, ANY, 1>  FirstFacet() const; 
    incidence_iterator_t<grid_type, ANY, ANY, ANY, 0>  FirstCell() const; 

  }; // class element_t<GRID,ANY,ANY>



  //----------------- constructors of element_t<> ------------------------------
  
  template<class GRID, int D, int CD>
  inline
  element_t<GRID,D,CD>::element_t(element_t<GRID,ANY,ANY> const& rhs) 
    : base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h()))
  { REQUIRE(rhs.dimension() == (unsigned)D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); }


  template<class GRID, int D, int CD>
  inline
  element_t<GRID,D,CD>&  element_t<GRID,D,CD>::operator=(element_t<GRID,ANY,ANY> const& rhs) 
  {
    REQUIRE(rhs.dimension() == (unsigned)D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); 
    base::operator=(base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h())));
    return *this;
  }

  //------------------ comparison operators of element_t<> ------------------------

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
  class element_iterator_t : public element_base_t<GRID,D,CD> {
    typedef element_iterator_t<GRID,D,CD> self;
    typedef element_base_t    <GRID,D,CD> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::element_handle_type element_handle_type;
  public:
    typedef element_t<GRID,D,CD> element_type;
    typedef element_type         value_type;
    using base::cb;
    using base::cv;
    using base::valid;
    using base::incr;
    using base::TheGrid;
    using base::handle;

    element_iterator_t() {}
    element_iterator_t(grid_type const&         gg, element_handle_type hh =  element_handle_type(0)) : base(gg,hh) {}
    element_iterator_t(ref_ptr<grid_type const> gg, element_handle_type hh =  element_handle_type(0)) : base(gg,hh) {}
    element_iterator_t(element_iterator_t<GRID,ANY,ANY> const& rhs);
    self & operator=  (element_iterator_t<GRID,ANY,ANY> const& rhs);

    bool IsDone() const { cb(); return !valid();}
    self& operator++()  { cv(); incr(); return *this;}
    value_type operator*() const { cv(); return value_type(TheGrid(), handle());}
  }; // class element_iterator_t<GRID,D,CD>


  // specialization: neither dimension nor codimension are known
  template<class GRID>
  class element_iterator_t<GRID,ANY,ANY> : public element_base_t<GRID,ANY,ANY> {
    typedef element_iterator_t<GRID,ANY,ANY> self;
    typedef element_base_t    <GRID,ANY,ANY> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::element_handle_type element_handle_type;
  public:
    typedef element_t<GRID,ANY,ANY> element_type;
    typedef element_type            value_type;
    using base::dimension;
    using base::cb;
    using base::cv;
    using base::valid;
    using base::handle;
    using base::TheGrid;

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
    self& operator++()  { cv(); base::incr(); return *this;}
    value_type operator*() const { cv(); return value_type(TheGrid(), dimension(), handle());}
 }; // class element_iterator_t<GRID,ANY,ANY>

  //---------------- constructors for element_iterator_t -------------------------

  template<class GRID, int D, int CD>
  inline
  element_iterator_t<GRID,D,CD>::element_iterator_t(element_iterator_t<GRID,ANY,ANY> const& rhs) 
    : base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h()))
  { REQUIRE(rhs.dimension() == (unsigned)D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); }


  template<class GRID, int D, int CD>
  inline
  element_iterator_t<GRID,D,CD>&  element_iterator_t<GRID,D,CD>::operator=(element_iterator_t<GRID,ANY,ANY> const& rhs) 
  {
    REQUIRE(rhs.dimension() == (unsigned)D, "D= " << D << " rhs.dimension=() " << rhs.dimension(), 1); 
    base::operator=(base(rhs.TheGrid(), typename base::element_handle_type(rhs.handle().h())));
    return *this;
  }


  //------------------ comparison operators for element_iterator_t<>  ------------------------

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






  //---------------------------------------------------------------------------------------
  //--------------------------------- incidence iterators ---------------------------------
  //---------------------------------------------------------------------------------------


  // Element<K>OnElement<D>Iterator 
  // CD = codim(D, GRID), CK = codim(K, GRID) 
  template<class GRID, int D, int K, int CD, int CK>
  class incidence_iterator_base_t : 
    public grid_types_ComplexND<D>, 
    public dimension_mixin<incidence_iterator_base_t<GRID, D, K, CD, CK>, GRID, K, CK> 
  {
    typedef incidence_iterator_base_t<GRID, D, K, CD, CK> self;
  public:
    typedef GRID                          grid_type;
    typedef element_t<GRID, K, CK>        value_type;
    typedef element_handle_t<GRID, K, CK> value_handle_type;
    typedef element_t<GRID, D, CD>        anchor_type;
    typedef element_handle_t<GRID, D, CD> anchor_handle_type;
    typedef typename grid_type::incidence_sequence incidence_sequence;
    typedef dimension_mixin<incidence_iterator_base_t<GRID, D, K, CD, CK>, GRID, K, CK>  mixin;
    using mixin::dimension;
  private:
    anchor_type  a;
  protected:
    unsigned     lh;
    
  public:
    incidence_iterator_base_t()  {}
    // for K or CK != ANY
    incidence_iterator_base_t(anchor_type const& A) : a(A), lh(0) {}
    // for K and CK == ANY
    incidence_iterator_base_t(anchor_type const& A, unsigned kk) : mixin(kk), a(A), lh(0) 
    { 
      REQUIRE(dimension() <= TheGrid().dimension(), 
	      "dim=" << dimension() << "griddim="  << TheGrid().dimension(), 1); 
    }
  
    //self& operator++() { cv();  ++lh; return *this;}
    bool IsDone() const { cb(); return lh >= incidences().size();}
    //    value_type        operator*() const { cv(); return value_type(TheGrid(), handle());}
    value_handle_type handle()    const { cv(); return value_handle_type(incidences()[lh]);}

    // unsigned dimension()       const { return D;}
    // unsigned value_dimension() const { return K;}

    //  anchor_handle_type anchor_handle() const { cb(); return a.handle();}
    anchor_type const&   TheAnchor()     const { cb(); return a;}
    grid_type   const&   TheGrid()       const { cb(); return a.TheGrid();}
 
    bool bound() const { return a.bound();}
    bool valid() const { return bound() &&  0 <= lh && lh < incidences().size();}
    void cb()    const { REQUIRE(bound(), "", 1);}
    void cv()    const { REQUIRE(valid(), "lh=" << lh, 1);}

    friend bool operator==(self const& lhs, self const& rhs) {  lhs.cb(); rhs.cb();  return lhs.lh == rhs.lh;}
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs==rhs);}
  protected:
    // for cell-on-cell: should check if incidences[lh] is != outer_cell_handle()
    void incr()  { cv(); ++lh;}
    //private:
  protected:
    incidence_sequence const& incidences() const 
	{ cb(); return TheGrid().Incidences(TheAnchor())[dimension()];}
  }; // class incidence_iterator_base_t<GRID, D, K, CD, CK>


  // primary template
  template<class GRID, int D, int K, int CD, int CK>
  class incidence_iterator_t : public  incidence_iterator_base_t<GRID, D, K, CD, CK>
  {
    typedef  incidence_iterator_base_t<GRID, D, K, CD, CK> base;
    typedef  incidence_iterator_t     <GRID, D, K, CD, CK> self;
  public:
    using base::handle;
    using base::TheGrid;
    using base::cv;

    incidence_iterator_t() {}
    incidence_iterator_t(typename base::anchor_type const& a) : base(a) {}
    
    self& operator++() { base::incr(); return *this;}
    typename base::value_type  operator*() const { cv(); return typename base::value_type(TheGrid(), handle());}
    

  };


  // partial specialization for CellOnCellIterator
  template<int D> //, int D, int K, int CD, int CK>
  class incidence_iterator_t<ComplexND<D>, D, D, 0,0> 
    : public  incidence_iterator_base_t<ComplexND<D>, D, D, 0,0>
  {
    typedef  incidence_iterator_base_t<ComplexND<D>, D, D, 0, 0> base;
    typedef  incidence_iterator_t     <ComplexND<D>, D, D, 0, 0> self;
  public:
    using base::handle;
    using base::TheGrid;
    using base::cv;

    incidence_iterator_t() {}
    incidence_iterator_t(typename base::anchor_type const& a) : base(a) 
    {
      while(base::lh < base::incidences().size() && handle() == TheGrid().outer_cell_handle()) 
	base::incr();
    }
    
    self& operator++() { 
      do {
	base::incr();
      } while( base::lh < base::incidences().size() && handle() == TheGrid().outer_cell_handle());
      return *this;
    }

    typename base::value_type  operator*() const { cv(); return typename base::value_type(TheGrid(), handle());}
    

  };



  // partial specialization for ANY
  template<class GRID, int D, int CD>
  class incidence_iterator_t<GRID, D, ANY, CD, ANY> : public  incidence_iterator_base_t<GRID, D, ANY, CD, ANY>
  {
    typedef  incidence_iterator_base_t<GRID, D, ANY, CD, ANY> base;
    typedef  incidence_iterator_t     <GRID, D, ANY, CD, ANY> self;

    int k;
  public:
    using base::dimension;
    using base::handle;
    using base::TheGrid;
    using base::cv;

    incidence_iterator_t() {}
    incidence_iterator_t(typename base::anchor_type const& a, int kk) : base(a,kk) {}
    // TODO: Add templated constructor for K, CK
    
    self& operator++() { base::incr(); return *this;}
    typename base::value_type  operator*() const { cv(); return typename base::value_type(TheGrid(), dimension(), handle());}
  };









  //---------------------------------------------------------------------------------------------
  //---------------------------------      inline members        --------------------------------
  //---------------------------------------------------------------------------------------------




  //------------------------------ inline members of element_base_t<> ---------------------------


  template<class GRID, int D, int CD>
  template<int K, int CK>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, K, CD, CK> element_t<GRID,D,CD>::FirstElement() const
  { return incidence_iterator_t<GRID, D, K, CD, CK>(*this);}

  template<class GRID, int D, int CD>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, ANY, CD, ANY> element_t<GRID,D,CD>::FirstElement(unsigned k) const
  { return incidence_iterator_t<GRID, D, ANY, CD, ANY>(*this, k);}


  template<class GRID, int D, int CD>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, 0, CD, d2c<D,0>::value>
  element_t<GRID,D,CD>::FirstVertex() const
  { return incidence_iterator_t<GRID, D, 0, CD, d2c<D,0>::value>(*this);}

  template<class GRID, int D, int CD>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, 1, CD, d2c<D,1>::value>
  element_t<GRID,D,CD>::FirstEdge() const
   { return incidence_iterator_t<GRID, D, 1, CD, d2c<D,1>::value>(*this);}

  template<class GRID, int D, int CD>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, 2, CD, d2c<D,2>::value>
  element_t<GRID,D,CD>::FirstFace() const
  { return incidence_iterator_t<GRID, D, 2, CD, d2c<D,2>::value>(*this);}

  template<class GRID, int D, int CD>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, c2d<D,1>::value, CD, 1>
  element_t<GRID,D,CD>::FirstFacet() const
  { return incidence_iterator_t<GRID, D, c2d<D,1>::value, CD, 1>(*this);}

  template<class GRID, int D, int CD>
  incidence_iterator_t<typename element_t<GRID,D,CD>::grid_type, D, c2d<D,0>::value, CD, 0>
  element_t<GRID,D,CD>::FirstCell() const
  { return incidence_iterator_t<GRID, D, c2d<D,0>::value, CD, 0>(*this);}



  //-------------------------- partial specializations for ANY ----------------

  template<class GRID>
  template<int K, int CK>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, K, ANY, CK> element_t<GRID,ANY,ANY>::FirstElement() const
  { return incidence_iterator_t<GRID, ANY, K, ANY, CK>(*this);}

  template<class GRID>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, ANY, ANY, ANY> element_t<GRID,ANY,ANY>::FirstElement(unsigned k) const
  { return incidence_iterator_t<GRID, ANY, ANY, ANY, ANY>(*this, k);}

  template<class GRID>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, 0, ANY, ANY >
  element_t<GRID,ANY,ANY>::FirstVertex() const
  { return incidence_iterator_t<GRID, ANY, 0, ANY, d2c<ANY,0>::value >(*this);}

  template<class GRID>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, 1, ANY, ANY>
  element_t<GRID,ANY,ANY>::FirstEdge() const
  { return incidence_iterator_t<GRID, ANY, 1, ANY, d2c<ANY,1>::value >(*this);}

  template<class GRID>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, 2, ANY, ANY>
  element_t<GRID,ANY,ANY>::FirstFace() const
  { return incidence_iterator_t<GRID, ANY, 2, ANY, d2c<ANY,2>::value >(*this);}


  template<class GRID>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, ANY, ANY, 1>
  element_t<GRID,ANY,ANY>::FirstFacet() const
  { return incidence_iterator_t<GRID, ANY, c2d<ANY,1>::value, ANY, 1>(*this);}

  template<class GRID>
  incidence_iterator_t<typename element_t<GRID,ANY,ANY>::grid_type, ANY, ANY, ANY, 0>
  element_t<GRID,ANY,ANY>::FirstCell() const
  { return incidence_iterator_t<GRID, ANY, c2d<ANY,0>::value, ANY, 0>(*this);}





  //------------------------------ inline members of ComplexND<D> ---------------------------------
 
  //---- specializations for ANY --------------
  // no initialization possible for run-time determined dimension
  template<> inline ComplexND<ANY>::ComplexND()  {}
  template<> inline ComplexND<ANY>::ComplexND(unsigned dim) : mixin(dim),  incidences(dim+1) {}

  template<> inline ComplexND<ANY>::archetype_handle 
  ComplexND<ANY>::add_archetype(ComplexND<ANY>::archetype_type const& A)
  { archetypes.push_back(A);  return archetypes.size() -1;}
  template<> inline ComplexND<ANY>::archetype_handle ComplexND<ANY>::add_archetype() { return add_archetype(archetype_type(dimension()-1));}

  //---- specializations for D=0 --------------
  template<> inline ComplexND<0>::ComplexND(pointcloud p) : incidences(dimension() +1)
  { incidences[0].resize(p.NumOfVertices());}

  //---- specializations for D=1 --------------
  // cyclic polygon - could go into source file
  template<> inline ComplexND<1>::ComplexND(polygon p) : incidences(dimension() +1) 
  {
    incidences[0].resize(p.NumOfVertices());
    incidences[1].resize(p.NumOfEdges());
    //unsigned v = 0, e = 0;
    unsigned n = p.NumOfVertices() -1 ;
    for( unsigned v = 0, e = 0; v < p.NumOfVertices(); ++v, ++e) {
      incidences[0][v].resize(dimension()+1);
      incidences[1][e].resize(dimension()+1);

      unsigned succ_v = ( v < n   ? v+1 : 0);
      unsigned prec_v = ( v > 0   ? v-1 : n);
      unsigned succ_e = succ_v; unsigned prec_e = prec_v;
      // v_{n-1} e_{e-1}   v0    e0   v1   e1 ...
      //    *--------------*----------*-------  
      incidences[0][v][1].push_back(e);
      incidences[0][v][1].push_back(prec_e);
      incidences[0][v][0].push_back(prec_v);
      incidences[0][v][0].push_back(succ_v);
      incidences[1][e][0].push_back(v);
      incidences[1][e][0].push_back(succ_v);
      incidences[1][e][1].push_back(prec_e);
      incidences[1][e][1].push_back(succ_e);
    }
    
    archetype_handle a = add_archetype(archetype_type(pointcloud(2)));
    cell_archetype.resize(NumOfCells());
    for(unsigned c = 0; c < NumOfCells(); ++c)
      cell_archetype[c] = a;

    calculate_incidences();
  }


  template<> inline ComplexND<1>::ComplexND( int edges[][2], int NE) : incidences(dimension() +1) 
  {
    // NE = number of edges/vertices of polygon.
    // calculate Number of vertices from edges[]
    int max_v = -1;
    for(int e = 0; e < NE; ++e) {
      for(int ve = 0; ve < 2; ++ve)
	max_v = std::max(max_v, edges[e][ve]);
    }
    int NV = max_v + 1;
      
    incidences[0].resize(NV);
    incidences[1].resize(NE);

    for( int e = 0;  e < NE; ++e) {
      incidences[1][e].resize(dimension()+1);
      incidences[1][e][0].resize(2);
      incidences[1][e][0][0] = edges[e][0];
      incidences[1][e][0][1] = edges[e][1];
    }    
    archetype_handle a = add_archetype(archetype_type(pointcloud(2)));
    cell_archetype.resize(NumOfCells());
    for(unsigned c = 0; c < NumOfCells(); ++c)
      cell_archetype[c] = a;

    // vertex incidences 
    for(int v = 0; v < NV; ++v) {
      incidences[0][v].resize(dimension() +1);
    }
    // edge-on-vertex
    for(int e = 0;  e < NE; ++e) {
      for(int ve = 0; ve < 2; ++ve) {
	int v = incidences[1][e][0][ve];
	incidences[0][v][1].push_back(e);
      }
    }

    //vertex-on-vertex
    for(int v = 0;  v < NV; ++v) {
      for(int vv = 0; vv < (int)incidences[0][v][1].size(); ++vv) {
	int e = incidences[0][v][1][vv];
	incidences[0][v][0].push_back
	  (v == incidences[1][e][0][0] ? incidences[1][e][0][1] : incidences[1][e][0][0]);
      }
    }

    // edge-on-edge 
    bool manifold_grid = true;
    for(int e = 0; e < NE; ++e) {
      for(int ve = 0; ve < 2; ++ve) {
	int v = incidences[1][e][0][ve];
	if(incidences[0][v][1].size() > 2 ) { // non-manifold
	  manifold_grid = false;
	  break;
	}
	incidences[1][e][1].resize(2);
	// is this the Right Thing?
	if(incidences[0][v][1].size() == 1) 
	  incidences[1][e][1][ve] = outer_cell_handle();
	else 
	  incidences[1][e][1][ve] 
	    = (incidences[0][v][1][ve] == e ? incidences[0][v][1][1-ve] : incidences[0][v][1][ve]);
      }
      if(! manifold_grid)
	break;
    }
    // if this is not a manifold grid, cell-cell-incidences are not well defined.
    if(! manifold_grid)
      for(int e = 0; e < NE; ++e) 
	incidences[0][e][1].clear();

    // calculate_incidences();
  }




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
  inline typename ComplexND<D>::VertexIterator ComplexND<D>::FirstVertex() const { return VertexIterator(*this);}
  template<int D>
  inline typename ComplexND<D>::EdgeIterator   ComplexND<D>::FirstEdge()   const { return EdgeIterator(*this);}
  template<int D>
  inline typename ComplexND<D>::FaceIterator   ComplexND<D>::FirstFace()   const { return FaceIterator(*this);}
  template<int D>
  inline typename ComplexND<D>::FacetIterator  ComplexND<D>::FirstFacet()  const { return FacetIterator(*this);}
  template<int D>
  inline typename ComplexND<D>::CellIterator   ComplexND<D>::FirstCell()   const { return CellIterator(*this);}

  template<int D>
  inline 
  typename ComplexND<D>::AnyElementIterator
  ComplexND<D>::FirstElement(unsigned d) const { return AnyElementIterator(*this, d);}

  template<int D>
  inline 
  typename ComplexND<D>::AnyElementIterator
  ComplexND<D>::EndElement(unsigned d) const { return AnyElementIterator(*this, d, typename gt::any_element_handle(NumOfElements(d)));}


  template<int D>
  typename ComplexND<D>::archetype_type const& ComplexND<D>::ArchetypeOf(typename ComplexND<D>::Cell const& c) const 
  { return archetypes[cell_archetype[c.handle()]]; }
  


} // namespace complexnd

  



//--------------------------------------------------------------------------------------------
//----------------------------- global namespace specializations -----------------------------
//--------------------------------------------------------------------------------------------


//-----------------------------  grid types  -----------------------------  

/*! \brief Specialization of grid_types primary template

    \ingroup complexndstuff
 */
template<int D>
struct grid_types<complexnd::ComplexND<D> > 
  : public grid_types_base<complexnd::grid_types_ComplexND<D> > 
{};

//-----------------------------  element traits  -----------------------------  

// vertex
template<int D>
struct element_traits<complexnd::element_t<complexnd::ComplexND<D>, 0, D> > 
  : public element_traits_vertex_base<complexnd::ComplexND<D> >
{
  typedef element_traits_vertex_base<complexnd::ComplexND<D> >  base;
  typedef consecutive_integer_tag<0>                            consecutive_tag;
  typedef typename base::hasher_type_elem_base                  hasher_type;
};

// edge
template<int D>
struct element_traits<complexnd::element_t<complexnd::ComplexND<D>, 1, D-1> > 
  : public element_traits_edge_base<complexnd::ComplexND<D> >
{
  typedef element_traits_edge_base<complexnd::ComplexND<D> >  base;
  typedef consecutive_integer_tag<0>                          consecutive_tag;
  typedef typename base::hasher_type_elem_base                hasher_type;
};

// face
template<int D>
struct element_traits<complexnd::element_t<complexnd::ComplexND<D>, 2, D-2> > 
  : public element_traits_face_base<complexnd::ComplexND<D> >
{
  typedef element_traits_face_base<complexnd::ComplexND<D> >  base;
  typedef consecutive_integer_tag<0>                          consecutive_tag;
  typedef typename base::hasher_type_elem_base                hasher_type;
};

// facet
template<int D>
struct element_traits<complexnd::element_t<complexnd::ComplexND<D>, D-1, 1> > 
  : public element_traits_facet_base<complexnd::ComplexND<D> >
{
  typedef element_traits_facet_base<complexnd::ComplexND<D> >  base;
  typedef consecutive_integer_tag<0>                           consecutive_tag;
  typedef typename base::hasher_type_elem_base                 hasher_type;
};


// cell
template<int D>
struct element_traits<complexnd::element_t<complexnd::ComplexND<D>, D, 0> > 
  : public element_traits_cell_base<complexnd::ComplexND<D> >
{
  typedef element_traits_cell_base<complexnd::ComplexND<D> >  base;
  typedef consecutive_integer_tag<0>                          consecutive_tag;
  typedef typename base::hasher_type_elem_base                hasher_type;
};

// disambiguate for 0D: vertex <-> cell 
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<0>, 0, 0> > 
  : public element_traits_vertex_base<complexnd::ComplexND<0> >
{
  typedef element_traits_vertex_base<complexnd::ComplexND<0> >  base;
  typedef consecutive_integer_tag<0>                            consecutive_tag;
  typedef base::hasher_type_elem_base                           hasher_type;
};


// disambiguate for 1D: edge <-> cell
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<1>, 1, 0> > 
  : public element_traits_edge_base<complexnd::ComplexND<1> >
{
  typedef element_traits_edge_base<complexnd::ComplexND<1> >  base;
  typedef consecutive_integer_tag<0>                          consecutive_tag;
  typedef base::hasher_type_elem_base                         hasher_type;
};


// disambiguate for 1D: vertex <-> facet
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<1>, 0, 1> > 
  : public element_traits_vertex_base<complexnd::ComplexND<1> >
{
  typedef element_traits_vertex_base<complexnd::ComplexND<1> >  base;
  typedef consecutive_integer_tag<0>                            consecutive_tag;
  typedef base::hasher_type_elem_base                           hasher_type;
};



// disambiguate for 2D: edge <-> facet 
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<2>, 1, 1> > 
  : public element_traits_edge_base<complexnd::ComplexND<2> >
{
  typedef element_traits_edge_base<complexnd::ComplexND<2> >  base;
  typedef consecutive_integer_tag<0>                          consecutive_tag;
  typedef base::hasher_type_elem_base                         hasher_type;
};

// disambiguate for 2D: face <-> cell 
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<2>, 2, 0> > 
  : public element_traits_cell_base<complexnd::ComplexND<2> >
{
  typedef element_traits_cell_base<complexnd::ComplexND<2> >  base;
  typedef consecutive_integer_tag<0>                          consecutive_tag;
  typedef base::hasher_type_elem_base                         hasher_type;
};

// disambiguate for 3D: face <-> facet
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<3>, 2, 1> > 
  : public element_traits_facet_base<complexnd::ComplexND<3> >
{
  typedef element_traits_facet_base<complexnd::ComplexND<3> >  base;
  typedef consecutive_integer_tag<0>                           consecutive_tag;
  typedef base::hasher_type_elem_base                          hasher_type;
};



// specializations for ANY

// spec. vertex for ANY
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<complexnd::ANY>, 0, complexnd::ANY> > 
  : public element_traits_vertex_base<complexnd::ComplexND<complexnd::ANY> >
{
  typedef element_traits_vertex_base<complexnd::ComplexND<complexnd::ANY> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};

// spec. edge for ANY
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<complexnd::ANY>, 1, complexnd::ANY> > 
  : public element_traits_edge_base<complexnd::ComplexND<complexnd::ANY> >
{
  typedef element_traits_edge_base<complexnd::ComplexND<complexnd::ANY> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};


// spec. face for ANY
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<complexnd::ANY>, 2, complexnd::ANY> > 
  : public element_traits_face_base<complexnd::ComplexND<complexnd::ANY> >
{
  typedef element_traits_face_base<complexnd::ComplexND<complexnd::ANY> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};

// spec. facet for ANY
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<complexnd::ANY>, complexnd::ANY, 1 > >
  : public element_traits_facet_base<complexnd::ComplexND<complexnd::ANY> >
{
  typedef element_traits_edge_base<complexnd::ComplexND<complexnd::ANY> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};


// spec. cell for ANY
template<>
struct element_traits<complexnd::element_t<complexnd::ComplexND<complexnd::ANY>, complexnd::ANY, 0 > >
  : public element_traits_cell_base<complexnd::ComplexND<complexnd::ANY> >
{
  typedef element_traits_cell_base<complexnd::ComplexND<complexnd::ANY> >  base;
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef base::hasher_type_elem_base                hasher_type;
};




//------------------------------- total grid functions --------------------------------

/*! \defgroup complexndgf Total grid functions for ComplexND
    \ingroup complexndstuff
 */


/*! \brief Specialization of the grid_function primary template for 
           \c complexnd::ComplexND<D> k-dimensional element type

    \ingroup complexndgf
 */
template<int D, int K, class T>
class grid_function<complexnd::element_t<complexnd::ComplexND<D>, K, D-K>, T>
  : public grid_function_vector<complexnd::element_t<complexnd::ComplexND<D>, K, D-K>, T> 
{
public:
  typedef complexnd::ComplexND<D>                 grid_type;
  typedef complexnd::element_t<grid_type, K, D-K> element_type;
private:
  typedef grid_function_vector<element_type, T>   base;

public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};




} // namespace GrAL 




namespace GrAL {  namespace complexnd {
  /*
  template<>
  template<>
  element_t<ComplexND<1>,0,1> ComplexND<1>::switched_element(element_t<ComplexND<1>,-1,2>  const& e_k_1,
							     element_t<ComplexND<1>, 0,1>  const& e_k,
							     element_t<ComplexND<1>, 1,0>  const& e_k1) const
  { return switched_vertex(e_k, e_k1);}

  template<>
  template<>
  element_t<ComplexND<1>,1,0> ComplexND<1>::switched_element(element_t<ComplexND<1>,0, 1>  const& e_k_1,
							     element_t<ComplexND<1>,1, 0>  const& e_k,
							     element_t<ComplexND<1>,2,-1>  const& e_k1) const
  { return switched_cell(e_k_1, e_k);}
  */


  // Dummy specializations for D=0 and D=1: should never be called.
  template<>
  template<int K>
  element_t<ComplexND<0>,K,1-K> ComplexND<0>::switched_element(element_t<ComplexND<0>,K-1,0-(K-1)> const& e_k_1,
							       element_t<ComplexND<0>,K  ,0- K   > const& e_k,
							       element_t<ComplexND<0>,K+1,0-(K+1)> const& e_k1) const
  { return e_k;}

  template<>
  template<int K>
  element_t<ComplexND<1>,K,1-K> ComplexND<1>::switched_element(element_t<ComplexND<1>,K-1,1-(K-1)> const& e_k_1,
							       element_t<ComplexND<1>,K  ,1- K   > const& e_k,
							       element_t<ComplexND<1>,K+1,1-(K+1)> const& e_k1) const
  { return e_k;}



  template<int D>
  template<int K>
  element_t<ComplexND<D>,K,D-K> ComplexND<D>::switched_element(element_t<ComplexND<D>,K-1,D-(K-1)> const& e_k_1,
							       element_t<ComplexND<D>,K  ,D- K   > const& e_k,
							       element_t<ComplexND<D>,K+1,D-(K+1)> const& e_k1) const
  {
    for(unsigned e_ki = 0; e_ki < e_k1.NumOfElements(K); ++e_ki) {
      element_t<self,K,dim-K> e_ks(*this, element_handle_t<self,K,dim-K>(Incidences(e_k1)[e_k.dimension()][e_ki]));
      if(e_ks != e_k && incident(e_k_1, e_ks))
	return e_ks;
    }
    ENSURE(false,"no switched element found for (e_k_1, e_k, e_k1) = ("
	   << e_k_1.handle() << "," << e_k.handle() << "," << e_k1.handle() << ")", 1);
  }

  // dummy specializations for D=0
  template<>
  ComplexND<0>::Edge 
  ComplexND<0>::switched_edge(ComplexND<0>::Vertex const& v, 
			      ComplexND<0>::Edge const& e, 
			      ComplexND<0>::Face const& f) const { return e;}
  template<>  
  ComplexND<0>::Face 
  ComplexND<0>::switched_face(ComplexND<0>::Edge const& e, 
			      ComplexND<0>::Face const& f, 
			      element_t<ComplexND<0>,3,0-3> const& c) const { return f;}
  template<>  
  ComplexND<0>::Facet 
  ComplexND<0>::switched_facet(element_t<ComplexND<0>,0-2,2> const& e, 
			       ComplexND<0>::Facet         const& f, 
			       ComplexND<0>::Cell          const& c) const { return f;}


  template<int D>
  typename ComplexND<D>::Edge 
  ComplexND<D>::switched_edge(ComplexND<D>::Vertex const& v, 
			      ComplexND<D>::Edge const& e, 
			      ComplexND<D>::Face const& f) const 
  { return switched_element(v,e,f);}

  template<int D>  
  typename ComplexND<D>::Face ComplexND<D>::switched_face(typename ComplexND<D>::Edge const& e, 
							  typename ComplexND<D>::Face const& f, 
							  element_t<self,3,dim-3> const& c) const
  { return switched_element(e,f,c); }

  template<int D>
  typename ComplexND<D>::Facet ComplexND<D>::switched_facet(element_t<self,dim-2,2> const& r, 
							    typename ComplexND<D>::Facet const& f, 
							    typename ComplexND<D>::Cell const& c) const
  { return switched_element(r,f,c);}    

    
}} // namespace GrAL {  namespace complexnd

#endif
