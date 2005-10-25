#ifndef GRAL_BASE_GB_ENUMERATED_SUBRANGE_H
#define GRAL_BASE_GB_ENUMERATED_SUBRANGE_H


// $LICENSE

/*! \file
 */

#include <vector>   // STL

#include "Config/compiler-config.h"
#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Base/mapped-iterators.h"
#include "Gral/Base/map-element-iter-names.h"
#include "Gral/Base/grid-functors.h"
#include "Gral/Iterators/facet-iterator-of-cell-set.h"
#include "Gral/Iterators/edge-iterator-of-cell-set.h"
#include "Gral/Base/element-handle.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/element-numbering.h"

namespace GrAL {

//----------------------------------------------------------------
/*! \defgroup enumsubranges Enumerated Grid Subranges
    \ingroup subranges 

    \brief Subranges defined by the sequence of their members 

 \contents
  - Classes with reference semantics: <BR>
    References to (enumerated) element ranges.
    HIt is an (random access) iterator with value type 
    element_handle, as is provided e.g. by classes [2] - [2c]
    The range is referenced by the half-open range [b,e) (b,e : HIt)
    -  template<class E, class HIt> class element_range_ref;
    -  template<class Grid, class HIt> class vertex_range_ref;
    -  template<class Grid, class HIt> class edge_range_ref;
    -  template<class Grid, class HIt> class facet_range_ref;
    -  template<class Grid, class HIt> class cell_range_ref;
    -  template<class Grid, class vit, class cit>
       class enumerated_subrange_ref;
  - Classes with value semantics: <BR>
    classes that store sequences of element handles and give
    iteration access to the corresponding elements.
     -  template<class E> class enumerated_element_range;
     -  template<class Grid> class enumerated_vertex_range;
     -  template<class Grid> class enumerated_edge_range;
     -  template<class Grid> class enumerated_facet_range;
     -  template<class Grid> class enumerated_cell_range;
     - template<class Grid> class enumerated_subrange;
   - Algorithms
     - template<class Grid, class CellIt>
       void \ref ConstructSubrangeFromCells;


 Aliases 'Element' and 'ElementIterator' are provided to ease the
 definition of template functions that are independent of the
 concrete element category (i.e. Vertex, Edge etc.)

    \see     \ref subranges module 
    \see Tests in \ref test-enumerated-subrange.C

  \todo the ranges do not allow \c ElementIterator(Range)
        constructor.
*/
//----------------------------------------------------------------


template<class E> class enumerated_element_range;
template<class Grid> class enumerated_vertex_range;
template<class Grid> class enumerated_edge_range;
template<class Grid> class enumerated_facet_range;
template<class Grid> class enumerated_cell_range;



//----------------------------------------------------------------
//----------------------------------------------------------------
//
//             Classes  with REFERENCE semantics
//
//----------------------------------------------------------------
//----------------------------------------------------------------

// helper class to access ElementIterator type
template<class E, class R>
class element_range_ref_aux {
public:
  typedef R                            range_type;
  typedef handle2element_map<E>        handle_map;

  typedef mapped_element_ra_seq_iterator<range_type, handle_map > ElementIterator;

};

//----------------------------------------------------------------
//                       element_range_ref
/*! \brief reference to a (possibly subset of) element range
   \ingroup enumsubranges

   The dimension of the element remains unspecified.
   See vertex_range_ref, facet_range_ref, cell_range_ref
   for specializations.

 */
//----------------------------------------------------------------

template<class E, class R>
class element_range_ref 
  : public map_element_iter_name<typename  element_range_ref_aux<E,R>::ElementIterator,
                                 element_range_ref<E,R>, 
                                 typename element_traits<E>::element_type_tag>
{
  typedef element_range_ref<E,R>       self;
public:
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename et::handle_type     elt_handle;
  typedef E                            Element;

  typedef R                                   range_type;
  typedef typename range_type::const_iterator const_iterator;
  typedef handle2element_map<E>        handle_map;

  typedef mapped_element_ra_seq_iterator<range_type, handle_map > ElementIterator;
  typedef range_type sequence_type;

  //  typedef HIt const_iterator; // iterator over handles
private:
  //---- DATA -----
  int ebeg, eend;
  // HIt ebeg;
  // HIt eend;
  ref_ptr<const range_type> the_range;
  ref_ptr<const grid_type>  the_grid;
public:
  //---------------------- construction -------------------------

  //element_range_ref(HIt b, HIt e, const grid_type& g) : ebeg(b), eend(e), the_grid(&g) {}
  element_range_ref() : ebeg(0), eend(0) {}

  // should this be the empty range or the whole range ??
  element_range_ref(const grid_type& g) 
    : ebeg(0), eend(0), the_grid(g) {}
  element_range_ref(ref_ptr<const grid_type> g) 
    : ebeg(0), eend(0), the_grid(g) {}

  element_range_ref(const range_type& r, const grid_type& g) 
    : ebeg(0), eend(0), the_range(r), the_grid(g) {}
  element_range_ref(ref_ptr<const range_type> r, ref_ptr<const grid_type> g) 
    : ebeg(0), eend(0), the_range(r), the_grid(g) {}

  element_range_ref(int b, int e, const range_type& r,  const grid_type& g) 
    : ebeg(b), eend(e), the_range(r), the_grid(g) {}
  element_range_ref(int b, int e, ref_ptr<const range_type> r, ref_ptr<const grid_type> g) 
    : ebeg(b), eend(e), the_range(r), the_grid(g) {}


  element_range_ref(const enumerated_element_range<E>& er);

  //---------------------- component access ---------------------

  //  const grid_type& TheGrid() const { return the_range->TheGrid();}
  grid_type const& TheGrid() const { return (*the_grid);}

  //---------------------- size queries -------------------------

  unsigned NumOfElements() const { return size();}

  // STL-like
  unsigned size()  const { return (eend - ebeg);} // random access!
  bool     empty() const { return (eend == ebeg);}

  //---------------------- iteration ----------------------------

  ElementIterator FirstElement() const 
    { return ElementIterator(ebeg,eend,
			     *the_range,handle_map(TheGrid()));}
  ElementIterator EndElement() const 
    { return ElementIterator(eend,eend,
			     *the_range,handle_map(TheGrid()));}

  //! allow for ElementIterator(Range) constructor.
  operator ElementIterator() const { return FirstElement();}

  // STL-like iteration over handles
  // WARNING: THESE ITERATORS GET INVALID IF THE RANGE IS EXTENDED !!
  const_iterator begin() const { return (the_range->begin() + ebeg);}
  const_iterator end()   const { return (the_range->begin() + eend);}

};


//----------------------------------------------------------------
/*! \brief Reference to a (possibly subset of) vertex range.
    \ingroup enumsubranges
 */
//----------------------------------------------------------------

template<class Grid, class R>
class vertex_range_ref
  : public element_range_ref<typename grid_types<Grid>::Vertex, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Vertex, R> base;
public:
  typedef typename base::Element         Vertex;
  typedef typename base::ElementIterator VertexIterator;   
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;

  vertex_range_ref() {}
  vertex_range_ref(const grid_type& g) : base(g) {}
  vertex_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  vertex_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  vertex_range_ref(const base& b) : base(b) {}

  
  unsigned       NumOfVertices() const { return size();}
  VertexIterator FirstVertex()   const { return FirstElement();}
  VertexIterator EndVertex  ()   const { return EndElement();}

};




//----------------------------------------------------------------
/*! \brief Reference to a (possibly subset of) edge range.
    \ingroup enumsubranges
 */
//----------------------------------------------------------------
template<class Grid, class R>
class edge_range_ref
  : public element_range_ref<typename grid_types<Grid>::Edge, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Edge, R> base;
public:
  typedef typename base::Element         Edge;
  typedef typename base::ElementIterator EdgeIterator;
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;

  edge_range_ref() {}
  edge_range_ref(const grid_type& g) : base(g) {}
  edge_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  edge_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  edge_range_ref(const base& b) : base(b) {}
  
  unsigned      NumOfEdges() const { return size();}
  EdgeIterator  FirstEdge()  const { return FirstElement();}
  EdgeIterator  EndEdge()    const { return EndElement();}

};


//----------------------------------------------------------------
/*! \brief Reference to a (possibly subset of) facet range.
    \ingroup enumsubranges
 */
//----------------------------------------------------------------
template<class Grid, class R>
class facet_range_ref
  : public element_range_ref<typename grid_types<Grid>::Facet, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Facet, R> base;
public:
  typedef typename base::Element         Facet;
  typedef typename base::ElementIterator FacetIterator;
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;

  facet_range_ref() {}
  facet_range_ref(const grid_type& g) : base(g) {}
  facet_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  facet_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  facet_range_ref(const base& b) : base(b) {}
  
  unsigned       NumOfFacets() const { return size();}
  FacetIterator  FirstFacet()  const { return FirstElement();}
  FacetIterator  EndFacet()    const { return EndElement();}

};

//----------------------------------------------------------------
/*! \brief Reference to a (possibly subset of) cell range.
    \ingroup enumsubranges
 */
//----------------------------------------------------------------
template<class Grid, class R>
class cell_range_ref
  : public element_range_ref<typename grid_types<Grid>::Cell, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Cell, R> base;
public:
  typedef typename base::Element         Cell;
  typedef typename base::ElementIterator CellIterator;
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;


  cell_range_ref() {}
  cell_range_ref(const grid_type& g) : base(g) {}
  cell_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  cell_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  cell_range_ref(const base& b) : base(b) {}

  
  unsigned       NumOfCells() const { return size();}
  CellIterator   FirstCell()  const { return FirstElement();}
  CellIterator   EndCell()    const { return EndElement();}

};


//----------------------------------------------------------------
//----------------------------------------------------------------
//
//             Classes  with VALUE semantics
//
//----------------------------------------------------------------
//----------------------------------------------------------------

// helper class to access ElementIterator type
template<class E>
class  enumerated_element_range_aux {
  typedef element_traits<E>            et;
  typedef typename et::handle_type     elt_handle;
  typedef  ::std::vector<elt_handle>      container_type;
  typedef handle2element_map<E>        handle_map;
public:
  typedef mapped_element_ra_seq_iterator<container_type,
                                         handle_map > ElementIterator;
};


//----------------------------------------------------------------
//              enumerated_element_range (value semantics)
/*!  \brief Subrange of a grid element range, 
     defined by listing its members.

     \ingroup enumsubranges
     \see Module enumsubranges
     \see enumerated_vertex_range, enumerated_edge_range, 
     enumerated_facet_range, enumerated_cell_range,
*/
//----------------------------------------------------------------


template<class E>
class enumerated_element_range 
  : public map_element_iter_name<typename enumerated_element_range_aux<E>::ElementIterator,
                                 enumerated_element_range<E>, 
                                 typename element_traits<E>::element_type_tag>{
  typedef enumerated_element_range<E> self;
public:
  //------- referenced types ------------------
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename et::handle_type     element_handle;
  typedef E                            Element;

  //---------- own types ----------------------

  typedef std::vector<element_handle>           elt_sequence;
  typedef elt_sequence                          container_type;
  typedef typename elt_sequence::const_iterator seq_iterator;
  typedef typename elt_sequence::const_iterator const_iterator;
  typedef typename elt_sequence::iterator       iterator;

  typedef handle2element_map<E>        handle_map;
  typedef mapped_element_ra_seq_iterator<container_type,
                                         handle_map > ElementIterator;


protected:
  ref_ptr<grid_type const> the_grid;
  elt_sequence     elements;

public:
  //-------------------- construction --------------------------

  enumerated_element_range() {}
  enumerated_element_range(const grid_type& g) : the_grid(g) {}
  enumerated_element_range(ref_ptr<grid_type const> g) : the_grid(g) {}

  void set_grid(ref_ptr<grid_type const> g) {
    REQUIRE(elements.empty(), "set_grid(): range is not empty!",1);
    the_grid = g;
  }
  void set_grid(const grid_type& g) {
    set_grid(const_ref_to_ref_ptr(g));
  }

  void append(element_handle v) { elements.push_back(v);}
  void append(Element v)    { elements.push_back(v.handle());}
  void push_back(element_handle v) { elements.push_back(v);}
  void push_back(Element v)    { elements.push_back(v.handle());}
  void clear() { elements.clear();}

  template<class RANGE>
  void copy(RANGE const& R)
  {
    typedef grid_types<RANGE> rgt;
    typedef typename rgt::template sequence_iterator<typename et::element_type_tag> RangeElementIterator;
    for(RangeElementIterator e(R);!e.IsDone(); ++e)
      push_back(*e);
  }

  //-------------------- component access ----------------------

  const grid_type&    TheGrid() const {
    REQUIRE((the_grid!=0),"no grid!\n",1);
    return *the_grid;
  }

  const elt_sequence& TheContainer() const { return elements;}
  elt_sequence&       TheContainer()       { return elements;}

  //-------------------- size queries --------------------------

  unsigned NumOfElements() const { return elements.size();}

  bool     empty() const { return elements.empty();}
  unsigned size()  const { return elements.size();}

  //-------------------- iteration  ----------------------------

  ElementIterator FirstElement() const 
    { return ElementIterator(0,elements.size(),elements, handle_map(*the_grid));}
  ElementIterator EndElement() const 
    { return ElementIterator(elements.size(),elements.size(),
			     elements, handle_map(*the_grid));}

  //! allow for ElementIterator(Range) constructor.
  operator ElementIterator() const { return FirstElement();}

  const_iterator begin() const { return elements.begin();}
  const_iterator end  () const { return elements.end  ();}
  iterator       begin()       { return elements.begin();}
  iterator       end  ()       { return elements.end  ();}
};


//----------------------------------------------------------------
//     vertex_range / facet_range / cell_range (value semantics)
//----------------------------------------------------------------

/*! \brief Vertex subrange of a grid, defined by listing the member vertices
    \ingroup enumsubranges
 */        
template<class Grid>
class enumerated_vertex_range 
  : public enumerated_element_range<typename grid_types<Grid>::Vertex>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Vertex> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Vertex;
  typedef typename base::ElementIterator VertexIterator;
  typedef typename base::element_handle  vertex_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;

  typedef vertex_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_vertex_range() {}
  enumerated_vertex_range(const grid_type& g) : base(g) {}
  enumerated_vertex_range(const base& b) : base(b) {}
  template<class RANGE>
  explicit enumerated_vertex_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }


  
  unsigned NumOfVertices()     const { return size();}
  VertexIterator FirstVertex() const { return FirstElement();}
  VertexIterator EndVertex()   const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}
};


/*! \brief Edge subrange of a grid, defined by listing the member edges
    \ingroup enumsubranges
 */        
template<class Grid>
class enumerated_edge_range 
  : public enumerated_element_range<typename grid_types<Grid>::Edge>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Edge> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Edge;
  typedef typename base::ElementIterator EdgeIterator;
  typedef typename base::element_handle  edge_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  
  typedef edge_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_edge_range() {}
  enumerated_edge_range(const grid_type& g) : base(g) {}
  enumerated_edge_range(const base& b) : base(b) {}
  template<class RANGE>
  explicit enumerated_edge_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }
  
  unsigned NumOfEdges()     const { return size();}
  EdgeIterator FirstEdge() const { return FirstElement();}
  EdgeIterator EndEdge()   const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}

};


/*! \brief Facet subrange of a grid, defined by listing the member facets
    \ingroup enumsubranges
 */        
template<class Grid>
class enumerated_facet_range 
  : public enumerated_element_range<typename grid_types<Grid>::Facet>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Facet> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Facet;
  typedef typename base::ElementIterator FacetIterator;
  typedef typename base::element_handle  facet_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  
  typedef facet_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_facet_range() {}
  enumerated_facet_range(const grid_type& g) : base(g) {}
  enumerated_facet_range(const base& b) : base(b) {}
  template<class RANGE>
  explicit enumerated_facet_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }

  unsigned NumOfFacets()     const { return size();}
  FacetIterator FirstFacet() const { return FirstElement();}
  FacetIterator EndFacet()   const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}

};


/*! \brief cell subrange of a grid, defined by listing the member cells
    \ingroup enumsubranges

 */        
template<class Grid>
class enumerated_cell_range 
  : public enumerated_element_range<typename grid_types<Grid>::Cell>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Cell> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Cell;
  typedef typename base::ElementIterator CellIterator;
  typedef typename base::element_handle  cell_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;

  typedef cell_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_cell_range() {}
  enumerated_cell_range(const grid_type& g) : base(g) {}
  enumerated_cell_range(const base& b) : base(b) {}
  
  template<class RANGE>
  explicit enumerated_cell_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }
  
  unsigned NumOfCells()     const { return size();}
  CellIterator FirstCell()  const { return FirstElement();}
  CellIterator EndCell()    const { return EndElement();}

  range_type_ref range() const 
    { return range_type_ref(0,size(),elements,*the_grid);}
};


//----------------------------------------------------------------
//
/*!  A grid subrange that is a simple enumeration of its elements.
   \ingroup enumsubranges
   \see enumsubranges

  Model of $GrAL GridVertex Range, $GrAL CellRange.<BR>

  This class has value-semantics, i.e. really stores
  the enumerated sequence.

  If vertices and/or cells are added in a piecemeal fashion by
  using \c push_back or  \c append, it is not guaranteed that
  the subrange is <em>closed</em>, i.e. that the sequence of vertices is exactly 
  the set of vertices incident to the sequence of cells.

  So either only vertices or only cells should be inserted.

  \todo Introduce own types for Vertex, Cell, handles. This would also allow to 
   have efficient vector-based total grid functions on the subrange.
    
  \todo Make the closure property a compile time or runtime policy.
  It is not consistent to allow both vertex and cell insertion if mixed insertion
  will fail. It is better to use a vertex range if only vertices are wanted
  (In some cases, algorithms formally expect also CellIterator types. Either these
  algorithms should be redesigned, or formal definitions of empty CellIterator types
   must be added to element sequences.)
*/

template<class Grid>
class enumerated_subrange : public archetypes_from_base<enumerated_subrange<Grid>, Grid>  {
  typedef enumerated_subrange<Grid> self;
public:
  using archetypes_from_base<enumerated_subrange<Grid>, Grid>::handle;

  //------- referenced types ------------------
  typedef self grid_type;
  typedef Grid base_grid_type;
  typedef grid_types<Grid> gt;

  // these could be different types
  typedef typename gt::vertex_handle        vertex_handle;
  typedef typename gt::cell_handle          cell_handle;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  typedef tp<Vertex> tpV;
  typedef tp<Cell>   tpC;

  //---------- own types ----------------------
  typedef std::vector<vertex_handle> vertex_sequence;
  typedef std::vector<cell_handle>   cell_sequence;
  typedef typename vertex_sequence::const_iterator v_iterator;
  typedef typename cell_sequence  ::const_iterator c_iterator;

  typedef vertex_range_ref<base_grid_type, vertex_sequence>  vertex_range_type;
  typedef cell_range_ref  <base_grid_type, cell_sequence  >  cell_range_type;

private:
  ref_ptr<const base_grid_type> the_grid;

  vertex_sequence vertices;
  //  edge_sequence   edges;
  cell_sequence   cells;
  // note : consistency in the sence that vertices = V(cells)
  // (set of adj. vertices of cells) cannot be ensured here!
  // It is ensured by calling init() before each access to vertices
  mutable bool initialized;
  mutable int  num_of_edges;
  mutable int  num_of_facets;
public:
  //-------------------- construction --------------------------
  /*! \brief  Default constructor, results in unbound range.
   */
  enumerated_subrange()                           :              initialized(false) { init_counts(); }
  /*! \brief Construct empty subrange
   */
  enumerated_subrange(const base_grid_type& g)         : the_grid(g), initialized(false) { init_counts(); }
  enumerated_subrange(ref_ptr<const base_grid_type> g) : the_grid(g), initialized(false) { init_counts(); }

  //! \brief delayed constructor, constructs empty subrange
  void init(ref_ptr<const base_grid_type> g) { clear(); the_grid = g; initialized = false; init_counts(); }
  //! \brief delayed constructor, constructs empty subrange
  void init(const base_grid_type& g) { init(const_ref_to_ref_ptr(g));}

  //! make this an empty range. 
  void clear() { 
    vertices.clear();
    cells   .clear();
    initialized = false;
  }

  /*! \brief Copy from cell range \c CR  
 
   */

  template<class CELLRANGE>
  enumerated_subrange(ref_ptr<const base_grid_type> g, 
		      ref_ptr<CELLRANGE> CR) :  
    the_grid(g), initialized(false) 
  { 
    construct(CR->FirstCell());
    init_counts();
  }
  
  template<class CELLRANGE>
  enumerated_subrange(const base_grid_type& g, CELLRANGE const& CR) :  
    the_grid(g), initialized(false) 
  { 
    construct(CR.FirstCell());
    init_counts();
  }


  template<class CELLIT>
  void construct(CELLIT c);

  void init_counts() const { num_of_edges = -1; num_of_facets = -1;}

  void append_vertex(vertex_handle v) { vertices.push_back(v);}
  void append_cell  (cell_handle   c) { cells.push_back(c); initialized = false;}
  void append_vertex(Vertex const& v) { append_vertex(v.handle());}
  void append_cell  (Cell  const&  c) { append_cell(c.handle());}

  void push_back(vertex_handle v) { append_vertex(v);}
  void push_back(cell_handle   c) { append_cell  (c);}
  void push_back(Vertex const& v) { append_vertex(v);}
  void push_back(Cell   const& c) { append_cell  (c);}


  //-------------------- iteration  ----------------------------

  typedef mapped_element_ra_seq_iterator<vertex_sequence,
                                      handle2element_map<Vertex> > VertexIterator;
  typedef mapped_element_ra_seq_iterator<cell_sequence,   
                                      handle2element_map<Cell> >   CellIterator;
  typedef facet_iterator_of_cell_set<CellIterator>                 FacetIterator;
  typedef edge_iterator_of_cell_set <CellIterator>                 EdgeIterator;

  unsigned NumOfCells()    const { init(); return cells.size();}
  unsigned NumOfVertices() const { init(); return vertices.size();}
  unsigned NumOfEdges() const { 
    init();
    if(num_of_edges == -1) {
      num_of_edges = 0;
      for(EdgeIterator e(FirstEdge()); !e.IsDone(); ++e)
	++num_of_edges;
    }
    return num_of_edges;
  }
  unsigned NumOfFacets() const { 
    init();
    if(num_of_facets == -1) {
      num_of_facets = 0;
      for(FacetIterator f(FirstFacet()); !f.IsDone(); ++f)
	++num_of_facets;
    }
    return num_of_facets;
  }


  CellIterator   FirstCell()   const 
    { 
      init();
      return CellIterator(0,cells.size(),
			  cells,
			  handle2element_map<Cell>(*the_grid));
    }
  CellIterator   EndCell()   const 
    { 
      init();
      return CellIterator(cells.size(),cells.size(),
			  cells,
			  handle2element_map<Cell>(*the_grid));
    }
  VertexIterator FirstVertex() const 
    {
      init();
      return VertexIterator(0, vertices.size(),
			    vertices,
			    handle2element_map<Vertex>(*the_grid));
    }
  VertexIterator EndVertex() const 
    {
      init();
      return VertexIterator(vertices.size(), vertices.size(),
			    vertices,
			    handle2element_map<Vertex>(*the_grid));
    }
  EdgeIterator   FirstEdge()  const { return EdgeIterator (FirstCell());}  
  EdgeIterator   EndEdge()    const { return EdgeIterator (EndCell());}  
  FacetIterator  FirstFacet() const { return FacetIterator(FirstCell());}  
  FacetIterator  EndFacet()   const { return FacetIterator(EndCell());}  


  //! allow for VertexIterator(Range) constructor.
  operator VertexIterator() const { return FirstVertex();}
  //! allow for EdgeIterator(Range) constructor.
  operator EdgeIterator() const { return FirstEdge();}
  //! allow for FacetIterator(Range) constructor.
  operator FacetIterator() const { return FirstFacet();}
  //! allow for CellIterator(Range) constructor.
  operator CellIterator() const { return FirstCell();}

  vertex_range_type range(tpV) const 
    { init(); return vertex_range_type(0, vertices.size(), vertices, *the_grid);}
  vertex_range_type vertex_range() const 
    { init(); return vertex_range_type(0, vertices.size(), vertices, *the_grid);}
  vertex_range_type Vertices() const 
    { init(); return vertex_range_type(0, vertices.size(), vertices, *the_grid);}

  cell_range_type   range(tpC) const 
    { init(); return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}
  cell_range_type   cell_range() const 
    { init(); return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}
  cell_range_type   Cells() const 
    { init(); return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}

  const base_grid_type& TheGrid()  const { return *the_grid;}
  const base_grid_type& BaseGrid() const { return *the_grid;}
  bool bound() const { return the_grid != 0;}
  bool empty() const { return NumOfCells() == 0 && NumOfVertices() == 0;}

  cell_handle   handle(const Cell& C)   const { return TheGrid().handle(C);}
  vertex_handle handle(const Vertex& V) const { return TheGrid().handle(V);}

private:
  // compute the closure of the cells.
  // NOTE: vertices must be empty
   void init() const;		     
};
  
/*
template<class E, class RANGE, class HANDLE>
class wrap_element_t {
  typedef E       base_element_type;
  typedef RANGE   range_type;
  typedef HANDLE  handle_type;  

  handle_type               h;
  ref_ptr<range_type const> rge;
public:
  wrap_element_t() {}
  wrap_element_t(range_type const& r) : rge(r), h(0) {}
  wrap_element_t(range_type const& r, handle_type hh) : rge(r), h(hh) {}
  
  handle_type handle() const { return h;}

  operator base_element_type() const 
  { return TheRange()->BaseElement<base_element_type>(TheRange()->BaseGrid(),h);}

};
*/

/*! \brief Wrap an underlying vertex type to create a new type
  
   \ingroup enumsubranges
   \see enumsubranges

   Model of $GrAL Vertex, $GrAL VertexIterator <BR>
   This type is convertible to the underlying vertex type.
   It is <em> not </em> constructible from the underlying type,
   because that would imply searching (and is not defined for all objects of underlying type)! 
*/
template<class RANGE>
class wrap_vertex_t {

  typedef wrap_vertex_t<RANGE> self;

 public:
  typedef typename  RANGE::base_vertex_type    base_vertex_type;
  typedef RANGE                                range_type;
  typedef RANGE                                grid_type;
  typedef typename RANGE::vertex_handle        handle_type;  
 private:
  ref_ptr<range_type const> rge;
  handle_type               h;
public:
  wrap_vertex_t() {}
  wrap_vertex_t(range_type const& r) : rge(r), h(0) {}
  wrap_vertex_t(range_type const& r, handle_type hh) : rge(r), h(hh) {}

  self& operator++() { cv(); ++h; return *this;}
  self const& operator*() const { cv();  return *this;}
  bool IsDone() const { cb(); return h >= (int)TheRange()->NumOfVertices();}

  handle_type handle() const { cv(); return h;}

  ref_ptr<range_type const>  TheRange () const { cb(); return rge;}
  ref_ptr<range_type const>  TheAnchor() const { cb(); return rge;}

  operator base_vertex_type() const   { cv(); return TheRange()->BaseVertex(h);}

  bool bound() const { return rge != 0;}
  bool valid() const { return bound() && h < (int)TheRange()->NumOfVertices();}
  void cb() const { REQUIRE(bound(),"",1);}
  void cv() const { REQUIRE(valid(),"",1);} 
};


/*! \brief Wrap an underlying cell type to create a new type
  
   \ingroup enumsubranges
   \see enumsubranges
   

   Model of $GrAL Cell, $GrAL CellIterator. <BR>
   This type is convertible to the underlying cell type.
*/
template<class RANGE>
class wrap_cell_t {

  typedef wrap_cell_t<RANGE> self;
public:
  typedef typename  RANGE::base_cell_type    base_cell_type;
  typedef typename  RANGE::base_vertex_type  base_vertex_type;
  typedef RANGE                          range_type;
  typedef RANGE                        grid_type;
  typedef typename RANGE::base_grid_type base_grid_type;
  typedef typename RANGE::cell_handle  handle_type;  
  typedef grid_types<base_grid_type>   bgt;
  typedef typename bgt::archgt         barchgt;
  typedef typename barchgt::Vertex     arch_vertex_type;

private:
  ref_ptr<range_type const> rge;
  handle_type               h;
public:
  wrap_cell_t() {}
  wrap_cell_t(range_type const& r) : rge(r), h(0) {}
  wrap_cell_t(range_type const& r, handle_type hh) : rge(r), h(hh) {}

  self& operator++() { cv(); ++h; return *this;}
  self const& operator*() const { cv();  return *this;}
  bool IsDone() const { cb(); return h >= (int)TheRange()->NumOfCells();}

  handle_type handle() const { cv(); return h;}

  ref_ptr<range_type const>  TheRange () const { cb(); return rge;}
  ref_ptr<range_type const>  TheAnchor() const { cb(); return rge;}

  temporary<base_cell_type> BaseCell() const { cv(); return temporary<base_cell_type>(TheRange()->BaseCell(h));}
  operator base_cell_type() const { cv(); return TheRange()->BaseCell(h);}

  base_vertex_type V(arch_vertex_type const& av) const { return BaseCell()->V(av);}

  bool bound() const { return rge != 0;}
  bool valid() const { return bound() && h < (int)TheRange()->NumOfCells();}
  void cb() const { REQUIRE(bound(),"",1);}
  void cv() const { REQUIRE(valid(),"",1);} 
};


//----------------------------------------------------------------
//
/*! \brief  A grid subrange  view that is a simple enumeration of its elements.
   \ingroup enumsubranges
   \see enumsubranges

  Model of $Gral GridVertex Range, $GrAL CellRange.<BR>

  This class has reference-semantics.

  There is no check if the different element ranges are consistent,
  i.e. if the sequence of vertices is exactly the set of adjacent
  vertices to the sequence of cells. This has to be ensured by
  the underlying grid range.

*/
template<class Grid>
class enumerated_subrange_ref : public archetypes_from_base<enumerated_subrange_ref<Grid>, Grid>  {
  typedef enumerated_subrange_ref<Grid> self;
public:
  using archetypes_from_base<enumerated_subrange_ref<Grid>, Grid>::handle;

  typedef self grid_type;
  typedef Grid base_grid_type;
  typedef grid_types<Grid> gt;

  //---------- own types ----------------------

  typedef enumerated_subrange<Grid>              range_type;
  typedef typename range_type::vertex_range_type vertex_range_ref_t;
  typedef typename range_type::cell_range_type   cell_range_ref_t;

  typedef enumerated_vertex_range<base_grid_type> vertex_range_t;
  typedef enumerated_cell_range<base_grid_type>   cell_range_t;

  //------- wrapped referenced types ------------------

  typedef typename gt::Vertex           base_vertex_type;
  typedef typename gt::Cell             base_cell_type;
  typedef vertex_handle_int<self>       vertex_handle;
  typedef wrap_vertex_t<self>           Vertex;
  typedef Vertex                        VertexIterator;
  typedef cell_handle_int<self>         cell_handle;
  typedef wrap_cell_t<self>             Cell;
  typedef Cell                          CellIterator;


  base_vertex_type BaseVertex(vertex_handle h) const { return base_vertex_type(BaseGrid(), *(vertices.begin() +h));}
  base_cell_type   BaseCell  (cell_handle h)   const { return base_cell_type  (BaseGrid(), *(cells   .begin() +h));}

  //typedef typename gt::vertex_handle vertex_handle;

  //  typedef  mapped_element_ra_seq_iterator<typename vertex_range_ref_t::sequence_type, 
  //handle2element_map<Vertex> >
  //VertexIterator;

  //  typedef typename gt::cell_handle   cell_handle;
  //  typedef typename gt::Cell                 Cell;

  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
  typedef tp<Vertex> tpV;
  typedef tp<Cell>   tpC;


private:
  //---- DATA ---------
  vertex_range_ref_t vertices;
  //  edge_range_ref   edges;
  cell_range_ref_t   cells;
  // note : consistency in the sence that vertices = V(cells)
  // (set of incident vertices of cells) cannot be ensured here!

public:
  //-------------------- construction --------------------------

  enumerated_subrange_ref()  {}
  enumerated_subrange_ref(const range_type& R)
    : vertices(R.vertex_range()), cells(R.cell_range()) {}
  enumerated_subrange_ref(const vertex_range_ref_t& V,
			  const cell_range_ref_t  & C)
    : vertices(V), cells(C) {}

  enumerated_subrange_ref(const vertex_range_t& V,
			  const cell_range_t  & C)
    : vertices(V.range()),
      cells   (C.range())
  {}



  //-------------------- iteration  ----------------------------

  typedef facet_iterator_of_cell_set<CellIterator>     FacetIterator;
  typedef edge_iterator_of_cell_set<CellIterator>      EdgeIterator;

  unsigned NumOfCells()    const { return cells.size();}
  unsigned NumOfVertices() const { return vertices.size();}
  // valid only if homeomorphic to a disk!
  unsigned NumOfEdges()    const 
    { return NumOfCells() + NumOfVertices() - 2 + 1; /* 1 == NumOfBoundaryComponents */}

  // CellIterator   FirstCell()   const  { return cells.FirstCell();} 
  //  VertexIterator FirstVertex() const  { return vertices.FirstVertex();}
  VertexIterator FirstVertex() const { return VertexIterator(*this);}
  CellIterator   FirstCell()   const { return CellIterator(*this);}

  FacetIterator  FirstFacet()  const  { return FacetIterator(FirstCell());}  
  FacetIterator  FirstEdge()   const  { return FacetIterator(FirstCell());}  

  //  CellIterator   EndCell()   const  { return cells.EndCell();} 
  //  VertexIterator EndVertex() const  { return vertices.EndVertex();}
  CellIterator   EndCell()   const { return CellIterator(*this, NumOfCells());}
  VertexIterator EndVertex() const { return VertexIterator(*this, NumOfVertices());}

  FacetIterator  EndFacet()  const  { return FacetIterator(EndCell());}  
  FacetIterator  EndEdge()   const  { return FacetIterator(EndCell());}  
  
  vertex_range_ref_t range(tpV) const { return vertices; }
  cell_range_ref_t   range(tpC) const { return cells; }

  base_grid_type const& TheGrid () const { return vertices.TheGrid();}
  base_grid_type const& BaseGrid() const { return vertices.TheGrid();}

  cell_handle   handle(const Cell& C)   const { return TheGrid().handle(C);}
  vertex_handle handle(const Vertex& V) const { return TheGrid().handle(V);}
};
  


template<class Grid>
struct element_traits<wrap_vertex_t<enumerated_subrange_ref<Grid> > >
  : public element_traits_vertex_base<enumerated_subrange_ref<Grid> > 
{
private:
  typedef element_traits_vertex_base<enumerated_subrange_ref<Grid> >  base;
public:
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef typename base::hasher_type_elem_base       hasher_type;
};

template<class Grid>
struct element_traits<wrap_cell_t<enumerated_subrange_ref<Grid> > >
  : public element_traits_cell_base<enumerated_subrange_ref<Grid> > 
{
private:
  typedef element_traits_cell_base<enumerated_subrange_ref<Grid> >  base;
public:
  typedef consecutive_integer_tag<0>                 consecutive_tag;
  typedef typename base::hasher_type_elem_base       hasher_type;
};


template<class Grid, class T>
class grid_function<wrap_vertex_t<enumerated_subrange_ref<Grid> >, T>
  : public grid_function_vector<wrap_vertex_t<enumerated_subrange_ref<Grid> >, T>
{
  typedef grid_function_vector<wrap_vertex_t<enumerated_subrange_ref<Grid> >, T> base;
  typedef enumerated_subrange_ref<Grid>                                          grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


template<class Grid, class T>
class grid_function<wrap_cell_t<enumerated_subrange_ref<Grid> >, T>
  : public grid_function_vector<wrap_cell_t<enumerated_subrange_ref<Grid> >, T>
{
  typedef grid_function_vector<wrap_cell_t<enumerated_subrange_ref<Grid> >, T> base;
  typedef enumerated_subrange_ref<Grid>                                          grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};



namespace detail {

template<class Grid, class GT  = grid_types<Grid> >
struct grid_types_esr : public GT {
  typedef enumerated_subrange<Grid> grid_type;

  typedef enumerated_subrange<Grid>           range_type;
  typedef enumerated_subrange<Grid>           vertex_range_type;
  typedef enumerated_subrange<Grid>           edge_range_type;
  typedef enumerated_subrange<Grid>           facet_range_type;
  typedef enumerated_subrange<Grid>           cell_range_type;

  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename range_type::EdgeIterator   EdgeIterator;
  typedef typename range_type::FacetIterator  FacetIterator;
  typedef typename range_type::CellIterator   CellIterator;
};



template<class Grid, class GT  = grid_types<Grid> >
struct grid_types_esr_ref : public GT  {
  typedef grid_types<Grid> bgt; 
  typedef enumerated_subrange_ref<Grid>       grid_type;
  typedef enumerated_subrange_ref<Grid>       range_type;
  //  typedef Grid                                grid_type;


  typedef typename range_type::Vertex         Vertex;
  typedef typename range_type::vertex_handle  vertex_handle;
  typedef typename range_type::Cell           Cell;
  typedef typename range_type::cell_handle    cell_handle;

  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename range_type::EdgeIterator   EdgeIterator;
  typedef typename range_type::FacetIterator  FacetIterator;
  typedef typename range_type::CellIterator   CellIterator;

};


template<class Grid>
struct grid_types_evr { 
  typedef Grid                                grid_type;
  typedef enumerated_vertex_range<Grid>       range_type;
  typedef grid_types<grid_type>               gt;
  typedef typename gt::dimension_tag          dimension_tag;

  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename range_type::Vertex         Vertex;
  typedef typename range_type::vertex_handle  vertex_handle;
};


template<class Grid, class R>
struct grid_types_evr_ref { 
  typedef Grid                                grid_type;
  typedef vertex_range_ref<Grid,R>            range_type;
  typedef grid_types<grid_type>               gt;
  typedef typename gt::dimension_tag          dimension_tag;

  typedef typename range_type::VertexIterator VertexIterator;
  typedef typename range_type::Vertex         Vertex;
  typedef typename range_type::vertex_handle  vertex_handle;
};



template<class Grid>
struct grid_types_eer { 
  typedef Grid                              grid_type;
  typedef enumerated_edge_range<Grid>       range_type;
  typedef grid_types<grid_type>             gt;
  typedef typename gt::dimension_tag        dimension_tag;

  typedef typename range_type::EdgeIterator EdgeIterator;
  typedef typename range_type::Edge         Edge;
  typedef typename range_type::edge_handle  edge_handle;
};

template<class Grid, class R>
struct grid_types_eer_ref { 
  typedef Grid                              grid_type;
  typedef edge_range_ref<Grid,R>            range_type;
  typedef grid_types<grid_type>             gt;
  typedef typename gt::dimension_tag        dimension_tag;

  typedef typename range_type::EdgeIterator EdgeIterator;
  typedef typename range_type::Edge         Edge;
  typedef typename range_type::edge_handle  edge_handle;
};



template<class Grid>
struct grid_types_efr { 
  typedef Grid                               grid_type;
  typedef enumerated_facet_range<Grid>       range_type;
  typedef grid_types<grid_type>              gt;
  typedef typename gt::dimension_tag         dimension_tag;

  typedef typename range_type::FacetIterator FacetIterator;
  typedef typename range_type::Facet         Facet;
  typedef typename range_type::facet_handle  facet_handle;
};

template<class Grid, class R>
struct grid_types_efr_ref { 
  typedef Grid                               grid_type;
  typedef facet_range_ref<Grid,R>            range_type;
  typedef grid_types<grid_type>              gt;
  typedef typename gt::dimension_tag         dimension_tag;

  typedef typename range_type::FacetIterator FacetIterator;
  typedef typename range_type::Facet         Facet;
  typedef typename range_type::facet_handle  facet_handle;
};



template<class Grid>
struct grid_types_ecr   {
  typedef Grid                              grid_type;
  typedef enumerated_cell_range<Grid>       range_type;
  typedef grid_types<grid_type>              gt;
  typedef typename gt::dimension_tag         dimension_tag;


  typedef typename range_type::CellIterator CellIterator;
  typedef typename range_type::Cell         Cell;
  typedef typename range_type::cell_handle  cell_handle;
};

template<class Grid, class R>
struct grid_types_ecr_ref  {
  typedef Grid                              grid_type;
  typedef cell_range_ref<Grid,R>            range_type;
  typedef grid_types<grid_type>              gt;
  typedef typename gt::dimension_tag         dimension_tag;

  typedef typename range_type::CellIterator CellIterator;
  typedef typename range_type::Cell         Cell;
  typedef typename range_type::cell_handle  cell_handle;
};


} // namespace detail


/*! Partial specializaton of grid_types traits for enumerated_subrange<Grid>.
    \ingroup traits enumsubranges

    \see Module traits
 */
template<class Grid>
struct grid_types<enumerated_subrange<Grid> >
  :  public grid_types_base<detail::grid_types_esr<Grid> > {};

/*! Partial specializaton of grid_types traits for enumerated_subrange_ref<Grid>
    \ingroup traits enumsubranges

    \see Module traits
 */
template<class Grid>
struct grid_types<enumerated_subrange_ref<Grid> >
  :  public grid_types_base<detail::grid_types_esr_ref<Grid> > {};




template<class Grid>
struct grid_types<enumerated_vertex_range<Grid> > 
  : public grid_types_base<detail::grid_types_evr<Grid> >
{};

template<class Grid, class R>
struct grid_types<vertex_range_ref<Grid,R> > 
  : public grid_types_base<detail::grid_types_evr_ref<Grid,R> >
{};

template<class Grid>
struct grid_types<enumerated_edge_range<Grid> > 
  : public grid_types_base<detail::grid_types_eer<Grid> >
{};


template<class Grid, class R>
struct grid_types<edge_range_ref<Grid,R> > 
  : public grid_types_base<detail::grid_types_eer_ref<Grid,R> >
{};

template<class Grid>
struct grid_types<enumerated_facet_range<Grid> > 
  : public grid_types_base<detail::grid_types_efr<Grid> >
{};


template<class Grid, class R>
struct grid_types<facet_range_ref<Grid,R> > 
  : public grid_types_base<detail::grid_types_efr_ref<Grid,R> >
{};


template<class Grid>
struct grid_types<enumerated_cell_range<Grid> > 
  : public grid_types_base<detail::grid_types_ecr<Grid> >
{};


template<class Grid, class R>
struct grid_types<cell_range_ref<Grid,R> > 
  : public grid_types_base<detail::grid_types_ecr_ref<Grid,R> >
{};





/*! \brief  Specialisation of element_numbering<>
    \ingroup  enumsubranges
    
    \todo This function accepts arguments of the base vertex type,
     by maintaining a partial grid function on the vertices of the base grid.
     This is done in order to make the following code possible:
     \code
     typedef grid_types<enumerated_subrange_ref<Grid> > rgt;
     enumerated_subrange_ref<Grid> Range(G); 
     // ...
     element_numbering<rgt::Vertex, rgt> number(Range);
     for(rgt::CellIterator c(Range); !c.IsDone(); ++c)
        for(rgt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	   int num_vc = number(*vc); // *vc is of base vertex type!!
     \endcode
     Note that \c VertexOnCellIterator is just an alias to the base grid's
     \c VertexOnCellIterator. This implies that \c *vc is of base vertex type,
     which is <em not  </em> convertible to \c rgt::Vertex.
     
*/
template<class Grid>
class element_numbering<wrap_vertex_t<enumerated_subrange_ref<Grid> >, 
			grid_types<enumerated_subrange_ref<Grid> > >
{
  typedef enumerated_subrange_ref<Grid>                 range_type;
  //typedef wrap_vertex_t<enumerated_subrange_ref<Grid> > element_type;
  typedef typename range_type::base_vertex_type         base_vertex_type;
  typedef grid_types<range_type>                        rgegt;
  typedef base_vertex_type                              element_type;

  ref_ptr<range_type const>        rge;
  partial_grid_function<base_vertex_type, int> num;
  int                              offset_;
public:
  element_numbering(range_type const& r, int offset = 0) 
    : rge(r), num(r.BaseGrid()), offset_(offset) 
    {
      int n = offset_;
      for(typename rgegt::VertexIterator v(*rge); ! v.IsDone(); ++v)
	num[*v] = n++;
    }

  int operator()(element_type const& e) const 
    { return num(e);}
};


/*! \brief  Specialisation of element_numbering<>
    \ingroup  enumsubranges
    
    \todo This function accepts arguments of the base cell type,
     by maintaining a partial grid function on the cells of the base grid.

*/
template<class Grid>
class element_numbering<wrap_cell_t<enumerated_subrange_ref<Grid> >, 
			grid_types <enumerated_subrange_ref<Grid> > >
{
  typedef enumerated_subrange_ref<Grid>                 range_type;
  //typedef wrap_cell_t<enumerated_subrange_ref<Grid> > element_type;
  typedef typename range_type::base_cell_type         base_cell_type;
  typedef grid_types<range_type>                        rgegt;
  typedef base_cell_type                              element_type;

  ref_ptr<range_type const>        rge;
  partial_grid_function<base_cell_type, int> num;
  int                              offset_;
public:
  element_numbering(range_type const& r, int offset = 0) 
    : rge(r), num(r.BaseGrid()), offset_(offset) 
    {
      int n = offset_;
      for(typename rgegt::CellIterator c(*rge); ! c.IsDone(); ++c)
	num[*c] = n++;
    }

  int operator()(element_type const& e) const 
    { return num(e);}
};



//----------------------------------------------------------------
/*! \brief Construct an enumerated subrange from  a set of cells
   \ingroup enumsubranges   

   THIS FUNCTION IS OBSOLETE!

   \templateparams
    - Range: (a possible type is  enumerated_subrange)
       - Model of $GrAL CellRange
       - \c Range::append_cell(cell_handle) 
       - \c Range::append_vertex(vertex_handle)
    - CellIt: $GrAL CellIterator

    \param R [OUT] 
        is the subrange to be constructed 
    \param Cit [IN] 
        range of \c Cit is the cell subrange
        to be copied

   \pre
     - R is empty before
     - return type of \c CellIt::operator*
       is convertible to  \c Range::Cell
   
   \post
     - \c R contains all cells in the range of \c Cit
     - \c R contains all vertices incident to cell in \c R
 */
//----------------------------------------------------------------


  
template<class Range, class CellIt>
void ConstructSubrangeFromCells
 (Range  & R,     // OUT: the subrange to be constructed
  CellIt   Cit);  // IN : range(Cit) is the cell subrange 


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Subranges/enumerated-subrange.C"
#endif


#endif
