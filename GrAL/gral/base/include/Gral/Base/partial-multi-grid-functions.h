#ifndef GRAL_BASE_GB_PARTIAL_MULTI_GRIDFUNCTIONS_H
#define GRAL_BASE_GB_PARTIAL_MULTI_GRIDFUNCTIONS_H

// $LICENSE

#include "Config/compiler-config.h"
#include "Gral/Base/common-grid-basics.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

namespace detail {

  template<class Grid, class T, int GD, int ED>
  class partial_multi_gf_aux  : public partial_multi_gf_aux<Grid,T,GD,ED-1>
  {
  public:
    typedef Grid grid_type;
  private:
    typedef typename grid_types<Grid>::template element_d<ED>::type element_type;
    typedef partial_multi_gf_aux<Grid,T,GD,ED-1>                    base;

    // partial_grid_function<element_type, T> f;
    typedef partial_grid_function<typename grid_types<Grid>::template element_d<ED>::type, T> elem_gf_type;
    elem_gf_type f;

  public:
    typedef typename base::value_type      value_type;
    typedef typename base::reference       reference;
    typedef typename base::const_reference const_reference;
    typedef typename base::size_type       size_type;


    partial_multi_gf_aux() {}
    partial_multi_gf_aux(Grid const& g) : base(g), f(g) {}
    partial_multi_gf_aux(Grid const& g, T const& t) : base(g,t), f(g,t) {}
    void clear() { base::clear(); f.clear();}
    bool empty() const { return base::empty() && f.empty();}
    size_type  total_size() const { return base::total_size() + f.size();}

    using base::operator();
    using base::operator[];
    //  using base::ElementFunction_();
    const_reference  operator()(element_type const& e) const { return f(e);}
    reference        operator[](element_type const& e)       { return f[e];}
    using base::defined;
    using base::undefine;
    bool defined (element_type const& e) const { return f.defined(e);}
    void undefine(element_type const& e)       { f.undefine(e);}

    using base::ElementFunction_;
    using base::FirstElement_;
    using base::EndElement_;
    using base::begin_;
    using base::end_;
    elem_gf_type const&                    ElementFunction_(element_type) const { return f;}
    typename elem_gf_type::ElementIterator FirstElement_   (element_type) const { return f.FirstElement();}
    typename elem_gf_type::ElementIterator EndElement_     (element_type) const { return f.EndElement();}
    typename elem_gf_type::const_iterator  begin_          (element_type) const { return f.begin();}
    typename elem_gf_type::const_iterator  end_            (element_type) const { return f.end  ();}
    typename elem_gf_type::iterator        begin_          (element_type)       { return f.begin();}
    typename elem_gf_type::iterator        end_            (element_type)       { return f.end  ();}

    grid_type const& TheGrid() const { return f.TheGrid();} 

    void set_value_ (value_type const& t)       { base::set_value_(t);  f.set_value(t);}
    void set_default(value_type const& t)       { base::set_default(t); f.set_default(t);}
    void set_grid_(grid_type const& g)          { base::set_grid_(g);   f.set_grid(g);}

 };

  // end recursion
  template<class Grid, class T, int GD>
  class partial_multi_gf_aux<Grid, T, GD, 0> 
  {
  public:
    typedef Grid grid_type;
  private:
    typedef typename grid_types<Grid>::Vertex  element_type;
    typedef partial_grid_function<element_type, T> elem_gf_type;
    typedef typename elem_gf_type::const_iterator  const_iterator;
    elem_gf_type f;
  public:
    typedef T                                      value_type;
    typedef typename elem_gf_type::reference       reference;
    typedef typename elem_gf_type::const_reference const_reference;
    typedef typename elem_gf_type::size_type       size_type;

    partial_multi_gf_aux() {}
    partial_multi_gf_aux(Grid const& g) : f(g) {}
    partial_multi_gf_aux(Grid const& g, T const& t) :  f(g,t) {}
    void clear() { f.clear();}
    bool empty()    const { return f.empty();}

    size_type total_size() const { return f.size();}

    const_reference  operator()(element_type const& e) const { return f(e);}
    reference        operator[](element_type const& e)       { return f[e];}
    bool defined (element_type const& e) const { return f.defined(e);}
    void undefine(element_type const& e)       { f.undefine(e);}

    elem_gf_type  const&                   ElementFunction_(element_type) const { return f;}
    typename elem_gf_type::ElementIterator FirstElement_   (element_type) const { return f.FirstElement();}
    typename elem_gf_type::ElementIterator EndElement_     (element_type) const { return f.EndElement();}
    typename elem_gf_type::const_iterator  begin_          (element_type) const { return f.begin();}
    typename elem_gf_type::const_iterator  end_            (element_type) const { return f.end  ();}
    typename elem_gf_type::iterator        begin_          (element_type)       { return f.begin();}
    typename elem_gf_type::iterator        end_            (element_type)       { return f.end  ();}

    grid_type const& TheGrid() const { return f.TheGrid();} 

    void set_value_ (value_type const& t) { f.set_value(t);}
    void set_grid_(grid_type const& g)    { f.set_grid(g);}
    void set_default(value_type const& t) { f.set_default(t);}
    const_reference get_default() const { return f.get_default();}
  };
  

} // namespace detail




/*! \brief Partial grid function defined on all element types of <Grid>.
    \ingroup gridfunctions

   partial_multi_grid_function<Grid,T> defines a mapping
   \f$ V(G) \cup E(G) \cup C(G) \mapsto T \f$ ( in the 2d case)

   These classes are  fully generic and do not need any further
   specialization, because they build on \c partial_grid_function \c<Elt,T>.

   \see \ref \c multi_grid_function\c <Grid,T>
   \see Module \ref gridfunctions
   \see Tested in \ref test-multi-grid-functions.C
*/



template<class Grid, class T>
class partial_multi_grid_function : public detail::partial_multi_gf_aux<Grid, T,  
									grid_types<Grid>::dim, 
									grid_types<Grid>::dim>
{
  typedef detail::partial_multi_gf_aux<Grid, T,  
			       grid_types<Grid>::dim, 
			       grid_types<Grid>::dim> base;

public: 
  typedef Grid                           grid_type;
  typedef T                              value_type;
  typedef typename base::reference       reference;
  typedef typename base::const_reference const_reference;
  typedef typename base::size_type       size_type;

  partial_multi_grid_function() {}
  partial_multi_grid_function(const grid_type& g) :  base(g) {}
  partial_multi_grid_function(const grid_type& g, T const& t) :  base(g,t) {}

  /*! \brief Element-wise size */
  template<class E>
  size_type size() const { return ElementFunction<E>().size();}
  /*! \brief Sum of sizes of element-wise grid functions */
  size_type size() const { return total_size();}


  /*! \brief Type mapping for element-related types 

      Example usage:
      \code
      // E is an element type, mgf_type is a partial_multi_grid_function<G,T>
      typedef typename mgf_type::template element_wise<E>::element_iterator iter;
      for(iter e = mgf.template FirstElement<E>(); e != mgf.template EndElement<E>(); ++e)
           mgf[*e] = 0;
      \endcode
   */
  template<class E>
  class element_wise { 
  public:
    typedef partial_grid_function<E,T>          function;
    typedef typename function::ElementIterator  element_iterator; 
    typedef typename function::const_iterator   const_iterator;
    typedef typename function::iterator         iterator;
  };

  /*! \brief Element-wise grid function */
  template<class E>
  typename element_wise<E>::function const& ElementFunction() const { return ElementFunction_(E());}

  /*! \name Element-wise iteration */
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
  
  //! \brief Set value to t for each element
  void set_value(value_type const& t) { set_value_(t); }
  //! \brief Set default value
  void set_default(value_type const& t) { base::set_default(t);}
  //! \brief Get default value
  const_reference get_default() const { return base::get_default();}


  //! \brief Set the grid
  void set_grid(grid_type const& g)   { set_grid_(g);}
  //! \brief Set grid and values for all elements
  void set_grid(grid_type const& g, value_type const& t) { set_grid_(g); set_value(t); }
  //! \brief Set grid and values for all elements
  void init(        grid_type const& g, value_type const& t) { set_grid(g,t);}
  //! \brief Set grid and values for all elements
  void init(ref_ptr<grid_type const> g, value_type const& t) { set_grid(*g,t);}

  //! \brief Undefine the value for each element
  void clear() { base::clear(); ENSURE_ALWAYS(base::empty(), "", 1);}


};


} // namespace GrAL 

#endif
