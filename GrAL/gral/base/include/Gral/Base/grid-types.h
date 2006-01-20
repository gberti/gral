#ifndef GRAL_BASE_GB_GRID_TYPES_H
#define GRAL_BASE_GB_GRID_TYPES_H


// $LICENSE

#include <cstddef>

#include "Gral/Base/type-tags.h"

#include "Gral/Base/internal/get-range-operations.h"
#include "Gral/Base/internal/define-grid-entities.h"
#include "Gral/Base/internal/compile-time-reflection.h"
#include "Gral/Base/internal/grid-categories.h"
#include "Gral/Base/internal/get-types-from-base.h"
#include "Gral/Base/internal/type-aliases.h"


namespace GrAL {


  /*! \defgroup gridtypes Grid Types traits

    \ingroup traits

    The traits template \c grid_types is the main vehicle to collect and transport 
    type information on a grid.
    The specializations of \c grid_types for a concrete grid type provide access to
    grid related types like
    - elements: \c Vertex, \c Edge, ....
    - element handles: \c vertex_handle, ...
    - sequence iterators: \c VertexIterator, ....
    - incidence iterators
    - archetype types: 
        - \c archetype_type, 
        - \c archetype_iterator,
        - \c archetype_handle, 
        - \c archgt
          
   See \c grid_types_base<> for advice and an example of specialization.

   \b Example for code using grid types:
   \code
    typedef grid_types<Complex2D> gt;
    for(gt::VertexIterator v = G.FirstVertex(); ...)
       do_something(*v);
   \endcode
  
   \b Dimension-independent access to sequence and incidence iterators:
   There are a number of meta-functions available to ease access to dimension-dependent
   types like elements and iterators:
 
   \code
   // same as gt::VertexIterator
   typedef typename sequence_iterator  <gt,vertex_type_tag>::type VIterator;
   typedef typename sequence_iterator_d<gt,0              >::type VIterator;

   // same as gt::VertexOnCellIterator
   typedef typename incidence_iterator  <gt,vertex_type_tag, cell_type_tag>::type VoCIterator;
   const int D = gt::dimension_tag::dim;
   typedef typename incidence_iterator_d<gt,0, D>::type VoCIterator;
   \endcode

   This is useful in context which are generic over the element category, that is,
   are intended to work uniformly for vertices, edges etc., and where for instance
   the type of an iterator must be synthetized from an element type:
   \code
   template<class ELEMENT>
   class SomeAlgorithm {
     typedef grid_types<ELEMENT> gt;
     typedef typename ELEMENT::element_type_tag         etag;
     typedef typename sequence_iterator<gt, etag>::type ElementIterator;
     typedef typename incidence_iterator<gt, etag, 
                                         cell_type_tag>::type ElementOnCellIterator;
  
     ...
    };
   \endcode
   
   As can be seen, those meta-functions come in two flavors, taking tag types and intgral dimensions:

   <table>
     <tr> <td> tagged version </td> <td> numeric version </td> </tr>
     <tr> <td>GrAL::element</td> <td>GrAL::element_d</td> </tr>
     <tr> <td>GrAL::element_handle</td> <td>GrAL::element_handle_d</td> </tr>
     <tr> <td>GrAL::sequence_iterator</td> <td>GrAL::sequence_iterator_d</td> </tr>
     <tr> <td>GrAL::incidence_iterator</td> <td>GrAL::incidence_iterator_d</td> </tr>
   </table>

   The tagged versions cover grid of dimension up to 4 (because there are 5 predefined tags:
   GrAL::vertex_type_tag (dim=1),
   GrAL::edge_type_tag (dim=2),
   GrAL::face_type_tag (dim=3),
   GrAL::facet_type_tag (codim=1),
   GrAL::cell_type_tag (codim=0),
   and for these grids the above meta functions are defined automatically.
  
   For higher-dimensional grids, the implementor of the grid type 
   should specialize the numeric versions (xxx_d)
   to deliver the correct results for all dimensions.
   See for instance the $gral-gral/cartesiannd  GrAL-CartesianND package.
*/

  /*! \brief Empty traits class for parameterized namespaces associated  with a grid.
    \ingroup gridtypes

  This has to be specialized for concrete grid types.
  */
  template<class Grid>
  struct grid_types {
    enum { is_specialized = 0};
  };

  /*!
      We use a signed type for sizes because it is safer 
      (think of G1.NumOfVertices() - G2.NumOfVertices()).
      
  */
  typedef ptrdiff_t signed_size_t;

  namespace grid_types_detail 
  {


    /*! \brief Class to be inherited by concrete grid_types base classes.
      
      \ingroup gridtypes

      A user defined grid_types base class
      should derive from this class.
   
      \see grid_types_base
    */
    struct grid_types_root {
      //! \name dummy definitions to permit using base::xxx in derived classes
      //@{
      void begin_aux() {}
      void end_aux  () {}
      void size_aux () {}
      void category () {}
      void NumOf()     {}
      void FirstElement()   {}
      void EndElement()     {}
      //@}
      typedef void uses_root_class;

      /*! \brief default size type
          This  can be overriden in derived classes
        
          \note  This should always be a \c signed integral type,
	  because it sometimes is initialized with -1 to denote an uninitialized value.
          Also, differences between sizes can be safely represented.
      */
      typedef signed_size_t  size_type; 

      enum { is_specialized = 1 };

    };
    
  } // namespace grid_types_detail 



/*! \brief Plugin-in base class for grid_types<> specializations
  
    \ingroup gridtypes

    This class should be used as direct base class for all specializations
    of grid_types<>.

   <b> Usage example </b>
   (enabling for a \c grid_types<> specialization):
   \code
   class MyGrid { ... };
   struct grid_types_mygrid 
     : public grid_types_detail::grid_types_root { ... };

   template<>
   struct grid_types<MyGrid> : public grid_types_base<grid_types_mygrid> {};
   \endcode


*/

template<class GTBASE>  
struct grid_types_base
    : public grid_types_detail::dim_dep_defs<GTBASE>
{
  typedef grid_types_base<GTBASE> self;
public:
  enum { dim = GTBASE::dimension_tag::dim };

  template<class E>
  struct categories {
    //    typedef grid_types_detail::get_grid_categories<GTBASE> cat;
    typedef grid_types_detail::get_grid_categories<self> cat;
    typedef typename cat::template categories<E>::type type;
  };

}; //  grid_types_base<GTBASE>




  template<class T, int IS_SPEC> struct grid_types_of_aux {};
  template<class T> struct grid_types_of_aux<T,1> { typedef grid_types<T> type; };
  template<class T> struct grid_types_of_aux<T,0> { typedef grid_types<typename T::grid_type> type; };

  template<class T> struct grid_types_of : public grid_types_of_aux<T, grid_types<T>::is_specialized> {};

  /*! \defgroup freefunctioniteration Iteration interface by free functions

      \ingroup traits
      \ingroup iterators

      Example 1:
      \code
       gt::VertexIterator v     = GrAL::begin<gt::Vertex>(Grid),
                          v_end = GrAL::end  <gt::Vertex>(Grid);
       gt::size_type nv = GrAL::size<gt::Vertex>(Grid);
       gt::CellOnVertexIterator cv = GrAL::begin<gt::Cell>(*v);
       gt::size_type ncv = GrAL::size<gt::Cell>(*v);
      \endcode
      
      We can also obtain a non-standard iterator:
      \code
      TracingVertexIterator v = GrAL::begin<TracingVertexIterator>(Grid);
      \endcode
      provided the necessary overloads have been made, e.g.
      \code
      template<class SeqIt>
      class TracingIterator { ... };

      template<class G, class SeqIt>
      TracingIterator<SeqIt> gral_begin(G const& g, TracingIterator<SeqIt>)
      { return TracingIterator<SeqIt>(GrAL::begin<SeqIt>(g)); }
      
      // same for end, size
      \endcode
   
      The corresponding value type of the iterator can also be specified by dimension:
       \code
       gt::VertexIterator v     = GrAL::begin<0>(Grid),
                          v_end = GrAL::end  <0>(Grid);
       gt::size_type nv  = GrAL::size<0>(Grid);
       const int D = gt::dimension_tag::dim;
       gt::CellOnVertexIterator cv = GrAL::begin<D>(*v);
       gt::size_type ncv = GrAL::size<D>(*v);
      \endcode

      Finally, the value type can often be deduced from the left hand side:
      \code
       gt::VertexIterator v     = GrAL::begin(Grid),
                          v_end = GrAL::end  (Grid);
       gt::size_type nv  = GrAL::size<0>(Grid);
       gt::CellOnVertexIterator cv = GrAL::begin(*v);
       const int D = gt::dimension_tag::dim;
       gt::size_type ncv = GrAL::size<D>(*v);
       \endcode
       Of course, this cannot work for \c size().
  */



  /*! \brief Getting the first iterator of an anchor (grid or grid element)

      \ingroup freefunctioniteration

   */
  template<class E, class A>
  typename iterator<E,A>::type 
  begin(A const& a) { return gral_begin(a, typename iterator<E,A>::type());}
  
  /*! \brief Getting the past-the-end iterator of an anchor (grid or grid element)
      
      \ingroup freefunctioniteration
   */
  template<class E, class A>
  typename iterator<E,A>::type 
  end  (A const& a) { return gral_end  (a, typename iterator<E,A>::type());}

  // If "no match for size(A)", then a missing grid_types typedef may be the reason
  /*! \brief Getting the size of an element sequence of an anchor (grid or grid element)

      \ingroup freefunctioniteration
   */
  template<class E, class A>
  typename grid_types_of<A>::type::size_type // grid_types<typename E::grid_type>::size_type 
  size (A const& a) { return gral_size (a, typename iterator<E,A>::type());}


  /*! \brief Getting the first iterator of an anchor (grid or grid element)

      \ingroup freefunctioniteration

   */
  template<class GT, class E, class A>
  typename iterator<E,A, GT>::type 
  begin_gt(A const& a) { return gral_begin(a, typename iterator<E,A, GT>::type()); }

  /*! \brief Getting the past-the-end iterator of an anchor (grid or grid element)
      
      \ingroup freefunctioniteration
   */
  template<class GT, class E, class A>
  typename iterator<E,A, GT>::type 
  end_gt  (A const& a) { return gral_end  (a, typename iterator<E,A, GT>::type()); }

 
 /*! \brief Getting the size of an element sequence of an anchor (grid or grid element)

      \ingroup freefunctioniteration
   */
  template<class GT, class E, class A>
  typename GT::size_type           
  size_gt (A const& a) { return gral_size (a, typename iterator<E,A, GT>::type()); }


  namespace grid_types_detail {

    // helper classes, for implementing
    // gt::CellIterator ci =  begin(G);
    // gt::Cell         c  = *begin(G);

    template<class A> struct begin_iter_deref {
      A const& a;
      begin_iter_deref(A const & aa) : a(aa) {}
      
      template<class E>
      operator E() const { typedef typename GrAL::iterator<E,A>::type IT; return *(GrAL::begin<IT>(a));}
    };

    template<class A> struct end_iter_deref {
      A const& a;
      end_iter_deref(A const & aa) : a(aa) {}
      
      template<class E>
      operator E() const { typedef typename GrAL::iterator<E,A>::type IT; return *(GrAL::end<IT>(a));}
    };

    template<class A>  struct begin_iter {
      A const& a;
      begin_iter(A const & aa) : a(aa) {}
      
      template<class IT>
      operator IT() const { return GrAL::begin<IT>(a);}

      begin_iter_deref<A> operator*() const { return  begin_iter_deref<A>(a);}
    };

    template<class A>  struct end_iter {
      A const& a;
      end_iter(A const & aa) : a(aa) {}
      
      template<class IT>
      operator IT() const { return GrAL::end<IT>(a);}

      end_iter_deref<A> operator*() const { return  end_iter_deref<A>(a);}
    };

  } // namespace grid_types_detail 


  /*! \brief Getting the first iterator of an anchor (grid or grid element)

      \ingroup freefunctioniteration

   */
  template<class A>
  grid_types_detail::begin_iter<A> begin(A const& a) { return  grid_types_detail::begin_iter<A>(a);}

  /*! \brief Getting the past-the-end iterator of an anchor (grid or grid element)
      
      \ingroup freefunctioniteration
   */
  template<class A>
  grid_types_detail::end_iter  <A> end  (A const& a) { return  grid_types_detail::end_iter  <A>(a);}

   /*! \brief Getting the first iterator of an anchor (grid or grid element)
      
      \ingroup freefunctioniteration
   */
  template<int D, class A> 
  typename iterator_d<D,A>::type begin(A const& a) { return begin<typename  iterator_d<D,A>::type>(a);}
 
  /*! \brief Getting the past-the-end iterator of an anchor (grid or grid element)
      
      \ingroup freefunctioniteration
   */
  template<int D, class A> 
  typename iterator_d<D,A>::type end  (A const& a) { return end  <typename  iterator_d<D,A>::type>(a);}

 /*! \brief Getting the size of an element sequence of an anchor (grid or grid element)

      \ingroup freefunctioniteration
   */
  template<int D, class A> 
  typename grid_types_of<A>::type::size_type  size(A const& a) 
  { return size<typename element_d<typename grid_types_of<A>::type,D>::type>(a);}



  /* for instance:
  template<class Min, class Maj, class A>
  closure_iterator<Min,Maj> begin(A const&, closure_iterator<Min,Maj>) { return closure_iterator<Min,Maj>(begin(a,Maj()));}

  ??
  template<class A>
  A begin(A const& a, A) { return a;}

  No: begin(Cell a, Cell) -> CellOnCellIterator
  */


  template<class E, 
	   class GT = grid_types<typename E::grid_type>, 
	   class ARCHGT = typename GT::archgt>
  struct grid2archetype {
    enum { edim = grid_types_detail::element_dim_of<GT,E>::value };

    typedef typename element_d       <ARCHGT,edim>::type element_type;
    typedef typename element_handle_d<ARCHGT,edim>::type element_handle;
  };


  template<class XOnCell>
  typename grid2archetype<typename XOnCell::value_type>::element_type
  inline archetype_element(XOnCell  it) 
  { 
    typedef typename grid2archetype<typename XOnCell::value_type>::element_type arch_element_type;
    return arch_element_type(it.TheGrid().ArchetypeOf(it.TheAnchor()),
			     it.local_handle());
  }


} // namespace GrAL 

#include "Gral/Iterators/dummy-iterator.h"


#endif
