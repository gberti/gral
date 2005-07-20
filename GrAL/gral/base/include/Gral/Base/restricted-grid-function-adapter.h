#ifndef NMWR_GB_RESTRICTED_GRID_FUNCTION_ADAPTER_H
#define NMWR_GB_RESTRICTED_GRID_FUNCTION_ADAPTER_H



// $LICENSE


#include "Gral/Base/common-grid-basics.h"

#include <boost/type_traits.hpp>

namespace GrAL {

/*! \defgroup restrictedgridfunctions Restrictions of grid functions
   \ingroup gridfunctions  
*/

template<class GF, class E_iter>
class restr_gf_iterator_adapter;

template<class GF, class E_iter>
inline bool operator==(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs);
template<class GF, class E_iter>
inline bool operator!=(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs);
template<class GF, class E_iter>
inline bool operator< (restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs);



//----------------------------------------------------------------
//----------------------------------------------------------------

  namespace detail {

    template<class GF, bool IsConst> 
    struct deref_aux {
      typedef typename GF::const_reference reference;
      
      template<class E>
      reference operator()(GF & f, E const& e) const { return f(e);}
    };

    template<class GF>
    struct deref_aux<GF, false> {
      typedef typename GF::reference reference;
      
      template<class E>
      reference operator()(GF & f, E const& e) const { return f[e];}
    };


    template<class GF>
    struct deref : public deref_aux<GF, boost::is_const<GF>::value> {};
      
  } // detail

/*! \internal
    \brief Wrapper class for E_iter type: Has value-type of GF   instead of E_iter
    \ingroup restrictedgridfunctions
 
   This class is used in restricted_grid_function_adapter to implement the
  container-level (STL-compliant) iterators over the stored items.
*/

template<class GF, class E_iter>
class restr_gf_iterator_adapter : public detail::deref<GF> {
  typedef restr_gf_iterator_adapter<GF,E_iter> self;
  typedef detail::deref<GF> base;
public:
  typedef typename base::reference     reference;
  typedef std::forward_iterator_tag    iterator_category;
  typedef typename GF::value_type      value_type;
  typedef typename GF::pointer         pointer;
  typedef typename GF::difference_type difference_type;
 

  //------------------ construction -------------------

  restr_gf_iterator_adapter() {}
  restr_gf_iterator_adapter(E_iter start,  GF& f) 
    : it(start), gf(&f) {}

  //------------------ iteration --- -------------------


  self&     operator++() { ++it; return *this;}
  reference operator*()  { return base::operator()(*gf, *it);}

  //------------------ comparision ---------------------

  friend bool operator== <> (const self& ls, const self& rs);
  friend bool operator!= <> (const self& ls, const self& rs);
  friend bool operator<  <> (const self& ls, const self& rs);
private:
  //------- DATA ---------
  E_iter it;
  GF *   gf;
};


template<class GF, class E_iter>
inline bool operator==(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs)
 { return (ls.it == rs.it);}

template<class GF, class E_iter>
inline bool operator!=(restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs)
 { return (ls.it != rs.it);}

template<class GF, class E_iter>
inline bool operator< (restr_gf_iterator_adapter<GF,E_iter> const& ls,
		       restr_gf_iterator_adapter<GF,E_iter> const& rs)
 { return (ls.it <  rs.it);}


//----------------------------------------------------------------
/*! \brief Restricts a grid function of type GF to the range of an 
    element-iterator
   \ingroup restrictedgridfunctions  
 
   For example, a grid function may be the ElementIterator of a subrange.
   There is no copying involved, this class has reference semantics
   with respect to the underlying grid function.
*/
//----------------------------------------------------------------


template<class GF, class E_iter>
class restricted_grid_function_adapter {
public:
  typedef unsigned size_type;
  restricted_grid_function_adapter(GF& f,
				   E_iter b, E_iter e, // [b,e)
				   size_type s)  // s = distance(b,e)
    : gf(&f), start(b), stop(e), sz(s) {}

  typedef restr_gf_iterator_adapter<GF,E_iter> iterator;
  //  class const_iterator;

  iterator begin() const { return iterator(start,*gf); } // !! non-const iterator !!
  iterator end()   const { return iterator(stop ,*gf); }
  size_type size() const { return sz;}
private:
  GF*       gf;
  E_iter    start;
  E_iter    stop;
  size_type sz;
};

/*! \brief Creator function for restricted_grid_function_adapter.

   \ingroup restrictedgridfunctions  
*/

template<class GF, class ERange>
inline 
restricted_grid_function_adapter<GF, typename ERange::const_iterator>
Restriction(GF& gf, const ERange& R)
{ 
  typedef typename ERange::const_iterator e_iter;
  return restricted_grid_function_adapter<GF,e_iter>(gf,R.begin(),R.end(),R.size());
}


  template<class E, class T, class GF = grid_function<typename E::base_element_type, T> >
  class grid_function_view;

  namespace detail {

    /*    
    template<class GF, class EIT>
    struct gf_view_stl_aux_base {
    private:
      typedef EIT ElementIterator;
      GF const& to_derived() const { return static_cast<GF const&>(*this);}
    public:
      typedef restr_gf_iterator_adapter<GF const, ElementIterator> const_iterator;
      const_iterator begin() const { return const_iterator(to_derived().FirstElement(), to_derived());}
      const_iterator end()   const { return const_iterator(to_derived().EndElement(),   to_derived());}
    };
    */

    template<class GF, class EIT, bool IsConst>  
    struct gf_view_stl_aux  {// : public gf_view_stl_aux_base<GF, EIT> {};
    private:
      typedef EIT ElementIterator;
      GF const& to_derived() const { return static_cast<GF const&>(*this);}
    public:
      typedef restr_gf_iterator_adapter<GF const, ElementIterator> const_iterator;
      const_iterator begin() const { return const_iterator(to_derived().FirstElement(), to_derived());}
      const_iterator end()   const { return const_iterator(to_derived().EndElement(),   to_derived());}

    };

    template<class GF, class EIT>
    struct gf_view_stl_aux<GF, EIT, false> { // : public gf_view_stl_aux_base<GF, EIT> {
    private:
      typedef EIT ElementIterator;
      GF const& to_derived() const { return static_cast<GF const&>(*this);}
      GF      & to_derived()       { return static_cast<GF &>(*this);}
    public:
      typedef restr_gf_iterator_adapter<GF const, ElementIterator> const_iterator;
      const_iterator begin() const { return const_iterator(to_derived().FirstElement(), to_derived());}
      const_iterator end()   const { return const_iterator(to_derived().EndElement(),   to_derived());}

      typedef restr_gf_iterator_adapter<GF, ElementIterator> iterator;
      iterator begin()  { return iterator(to_derived().FirstElement(), to_derived());}
      iterator end()    { return iterator(to_derived().EndElement(),   to_derived());}
    };

    /*
    template<class E, class T, class BASEGF> class gf_view_stl 
      : public gf_view_stl_aux<grid_function_view<E,T,BASEGF>, 
			       typename element_traits<E>::ElementIterator,
    	       boost::is_const<BASEGF>::value> {};
    */  
  }



  /*! \brief Wrapper for grid functions of base grid
      
       \ingroup restrictedgridfunctions
       \see Module \ref restrictedgridfunctions

       In many contexts, one might use the underlying grid function on the base grid as well.
       However, these do not provide correct size() and TheGrid() functions.

       Example:
       \code
        my_grid_t G;
	typedef grid_function<grid_types<my_grid_t>::Vertex, int> my_gf_t;
	my_gf_t gf(G, 0);
        ...
	namespace rgv = restricted_grid_view;
	typedef in_circle_pred pred_t; // define some predicate on the cells of G
	typedef rgv::grid_view<my_grid_t, pred_t> view_t;
	typedef grid_types<view_t> vgt;

	pred_t  in_circle(origin,radius);
	view_t  ball(G, in_circle);

        // type of GF parameter is deduced
	rgv::grid_function_view<vgt::Vertex, int> ballgf(ball,gf);

	my_algorithm(ball, ballgf);
       \endcode

       \todo Provide iterators
  */

  template<class E, class T, class GF> //  = grid_function<typename E::base_element_type, T> >
  class grid_function_view 
    : public   detail::gf_view_stl_aux<grid_function_view<E,T,GF>,
				       typename element_traits<E>::ElementIterator,
				       boost::is_const<GF>::value
                                      > 
  { 
    typedef grid_function_view<E,T,GF> self;
  public:
    typedef E                             element_type;     //!< Element type this grid function is defined on
    typedef element_traits<E>             et;
    typedef typename et::grid_type        grid_type;        //!< The underlying grid view type
    typedef typename et::ElementIterator  ElementIterator;  //!< Iterator over the domain of this gf

    typedef GF                                 base_gf_type;
    typedef typename GF::element_type          base_element_type;
    typedef typename GF::value_type            value_type;      //!< value type  of the grid function 
    typedef typename GF::reference             reference;       //!< reference to value type, type of operator[]
    typedef typename GF::const_reference       const_reference; //!< const reference to value type, type of operator()
    typedef typename GF::size_type             size_type;       //!< type returned by size()
    typedef typename GF::pointer               pointer;
    typedef typename GF::difference_type       difference_type;

  private:
    ref_ptr<grid_type const> g;
    ref_ptr<base_gf_type>    gf;
  public:
    grid_function_view(grid_type const& gg, base_gf_type &  ggf)
      : g(gg), gf(ggf) {}
    grid_function_view(ref_ptr<grid_type const> gg, ref_ptr<base_gf_type>  ggf)
      : g(gg), gf(ggf) {}

		       
    const_reference operator()(element_type const& e) const { return (*gf)(e);}
    reference       operator[](element_type const& e)       { return (*gf)[e];}
    
    ElementIterator FirstElement() const { return et::FirstElement(*g);}
    ElementIterator EndElement()   const { return et::EndElement(*g);}

    grid_type const& TheGrid() const { return *g;}
    size_type size() const { return et::size(*g);}
    
  };



} // namespace GrAL 

#endif
