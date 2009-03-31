#ifndef GRAL_BASE_GB_MULTI_GRIDFUNCTIONS_H
#define GRAL_BASE_GB_MULTI_GRIDFUNCTIONS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {


namespace detail {

  template<class Grid, class T, int GD, int ED>
  class multi_gf_aux : public multi_gf_aux<Grid,T,GD,ED-1> 
  {
  public:
    typedef Grid grid_type;
  private:
    typedef typename element_d<grid_types<Grid>,ED>::type  element_type;
    typedef multi_gf_aux<Grid,T,GD,ED-1>                   base;
    typedef element_traits<element_type>                   et;

    typedef grid_function<element_type, T> elem_gf_type;
    elem_gf_type f;
  public:
    typedef typename base::value_type      value_type;
    typedef typename base::reference       reference;
    typedef typename base::const_reference const_reference;
    typedef typename base::size_type       size_type;

    multi_gf_aux() {}
    multi_gf_aux(Grid const& g) : base(g), f(g) {}
    multi_gf_aux(Grid const& g, T const& t) : base(g,t), f(g,t) {}

    using base::operator();
    using base::operator[];
    T  operator()(element_type const& e) const { return f(e);}
    T& operator[](element_type const& e)       { return f[e];}

    using base::ElementFunction_;
    using base::FirstElement_;
    using base::EndElement_;
    using base::begin_;
    using base::end_;
    elem_gf_type const&                    ElementFunction_(element_type) const { return f;}
    typename elem_gf_type::ElementIterator FirstElement_   (element_type) const { return et::FirstElement(TheGrid());}
    typename elem_gf_type::ElementIterator EndElement_     (element_type) const { return et::EndElement  (TheGrid());}
    typename elem_gf_type::const_iterator  begin_          (element_type) const { return f.begin();}
    typename elem_gf_type::const_iterator  end_            (element_type) const { return f.end  ();}
    typename elem_gf_type::iterator        begin_          (element_type)       { return f.begin();}
    typename elem_gf_type::iterator        end_            (element_type)       { return f.end  ();}

    grid_type const& TheGrid() const { return f.TheGrid();} 

    size_type total_size() const { return f.size() + base::total_size();}

    void set_value_(value_type const& t)   { base::set_value_(t);  f.set_value(t);}
    void set_grid_ (grid_type const& g)    { base::set_grid_(g);   f.set_grid(g); }
 };


  // end recursion
  template<class Grid, class T, int GD>
  class multi_gf_aux<Grid, T, GD, 0> 
  {
  public:
    typedef Grid grid_type;
  private:
    typedef typename grid_types<Grid>::Vertex  element_type;
    typedef element_traits<element_type>       et;

    typedef grid_function<element_type, T> elem_gf_type;
    elem_gf_type f;
  public:
    typedef T                                      value_type;
    typedef typename elem_gf_type::reference       reference;
    typedef typename elem_gf_type::const_reference const_reference;
    typedef typename elem_gf_type::size_type       size_type;

    multi_gf_aux() {}
    multi_gf_aux(Grid const& g) : f(g) {}
    multi_gf_aux(Grid const& g, T const& t) :  f(g,t) {}


    T  operator()(element_type const& e) const { return f(e);}
    T& operator[](element_type const& e)       { return f[e];}

    elem_gf_type const&                    ElementFunction_(element_type) const { return f;}
    typename elem_gf_type::ElementIterator FirstElement_   (element_type) const { return et::FirstElement(TheGrid());}
    typename elem_gf_type::ElementIterator EndElement_     (element_type) const { return et::EndElement  (TheGrid());}
    typename elem_gf_type::const_iterator  begin_          (element_type) const { return f.begin();}
    typename elem_gf_type::const_iterator  end_            (element_type) const { return f.end  ();}
    typename elem_gf_type::iterator        begin_          (element_type)       { return f.begin();}
    typename elem_gf_type::iterator        end_            (element_type)       { return f.end  ();}

    grid_type const& TheGrid() const { return f.TheGrid();} 
    size_type total_size() const { return f.size();}

    void set_value_(value_type const& t) { f.set_value(t);}
    void set_grid_(grid_type const& g)   { f.set_grid(g);}
  };
  

  

} // namespace detail




/*! \brief Total grid function defined on all element types of \c Grid.
    \ingroup gridfunctions

   multi_grid_function<Grid,T> defines a mapping
   \f$ V(G) \cup E(G) \cup C(G) \mapsto T \f$ ( in the 2d case)

   These classes are  fully generic and do not need any further
   specialization, because they build on <tt> grid_function<Elt,T> </tt>.

   \see \c partial_multi_grid_function
   \see Module \ref gridfunctions
   \see Tested in \ref test-multi-grid-functions.C
*/

template<class Grid, class T>
class multi_grid_function : public detail::multi_gf_aux<Grid, T,  
							grid_types<Grid>::dim, 
							grid_types<Grid>::dim>
{
  typedef detail::multi_gf_aux<Grid, T,  
			       grid_types<Grid>::dim, 
			       grid_types<Grid>::dim> base;

public: 
  typedef Grid grid_type;
  typedef T    value_type;
  typedef typename base::size_type size_type;

  multi_grid_function() {}
  multi_grid_function(const grid_type& g) :  base(g) {}
  multi_grid_function(const grid_type& g, T const& t) :  base(g,t) {}
 
  /*! \brief Type mapping for element-related types 
   */
  template<class E>
  struct element_wise { 
    typedef grid_function<E,T>                  function;
    typedef typename function::ElementIterator  element_iterator;
    typedef typename function::const_iterator   const_iterator;
    typedef typename function::iterator         iterator;
  };

  /*! \brief Element-wise grid function */
  template<class E>
  typename element_wise<E>::function const& ElementFunction() const { return ElementFunction_(E());}

  /*! \name  Element-wise iterator */
  //@{
  template<class E>
  typename element_wise<E>::element_iterator FirstElement() const { return FirstElement_(E());}
  template<class E>
  typename element_wise<E>::element_iterator EndElement()   const { return EndElement_(E());}

  template<class E>
  typename element_wise<E>::const_iterator begin() const { return begin_(E());}
  template<class E>
  typename element_wise<E>::const_iterator end  () const { return end_  (E());}
  template<class E>
  typename element_wise<E>::iterator       begin()       { return begin_(E());}
  template<class E>
  typename element_wise<E>::iterator       end  ()       { return end_  (E());}
  //@}


  /*! \brief Element-wise size */
  template<class E>
  size_type size() const { return ElementFunction<E>().size();}
  /*! \brief Sum of sizes of element-wise grid functions */
  size_type size() const { return base::total_size();}

  //! \brief Set value to t for each element
  void set_value(value_type const& t) { set_value_(t);}
  //! \brief Set the grid
  void set_grid(grid_type const& g)   { set_grid_(g);}
  //! \brief Set grid and values for all elements
  void set_grid(grid_type const& g, value_type const& t) { set_grid_(g); set_value(t); }
  //! \brief Set grid and values for all elements
  void init(        grid_type const& g, value_type const& t) { set_grid(g,t);}
  //! \brief Set grid and values for all elements
  void init(ref_ptr<grid_type const> g, value_type const& t) { set_grid(*g,t);}

};


} // namespace GrAL 

#endif
