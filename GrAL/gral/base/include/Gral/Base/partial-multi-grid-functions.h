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
    partial_grid_function<typename grid_types<Grid>::template element_d<ED>::type, T> f;

  public:
    typedef typename base::value_type      value_type;
    typedef typename base::reference       reference;
    typedef typename base::const_reference const_reference;

    partial_multi_gf_aux() {}
    partial_multi_gf_aux(Grid const& g) : base(g), f(g) {}
    partial_multi_gf_aux(Grid const& g, T const& t) : base(g,t), f(g,t) {}
    void clear() { base::clear(); f.clear();}
    bool empty() const { return base::empty() && f.empty();}
    unsigned size() const { return base::size() + f.size();}

    using base::operator();
    using base::operator[];
    //  using base::ElementFunction_();
    const_reference  operator()(element_type const& e) const { return f(e);}
    reference        operator[](element_type const& e)       { return f[e];}
    using base::defined;
    using base::undefine;
    bool defined (element_type const& e) const { return f.defined(e);}
    void undefine(element_type const& e)       { f.undefine(e);}

    grid_function<element_type, T> const& ElementFunction_(element_type const&) const { return f;}
    grid_type const& TheGrid() const { return f.TheGrid();} 

    void set_value_ (value_type const& t) { base::set_value_(t);  f.set_value(t);}
    void set_default(value_type const& t) { base::set_default(t); f.set_default(t);}
    void set_grid_(grid_type const& g)    { base::set_grid_(g);   f.set_grid(g);}

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

    elem_gf_type f;
  public:
    typedef T                                      value_type;
    typedef typename elem_gf_type::reference       reference;
    typedef typename elem_gf_type::const_reference const_reference;

    partial_multi_gf_aux() {}
    partial_multi_gf_aux(Grid const& g) : f(g) {}
    partial_multi_gf_aux(Grid const& g, T const& t) :  f(g,t) {}
    void clear() { f.clear();}
    bool empty()    const { return f.empty();}
    unsigned size() const { return f.size();}

    const_reference  operator()(element_type const& e) const { return f(e);}
    reference        operator[](element_type const& e)       { return f[e];}
    bool defined (element_type const& e) const { return f.defined(e);}
    void undefine(element_type const& e)       { f.undefine(e);}

    grid_function<element_type, T> const& ElementFunction_(element_type const&) const { return f;}
    grid_type const& TheGrid() const { return f.TheGrid();} 

    void set_value_ (value_type const& t) { f.set_value(t);}
    void set_default(value_type const& t) { f.set_default(t);}
    void set_grid_(grid_type const& g)    { f.set_grid(g);}

    const_reference get_default() { return f.get_default();}
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
*/
//----------------------------------------------------------------


template<class Grid, class T>
class partial_multi_grid_function : public detail::partial_multi_gf_aux<Grid, T,  
									grid_types<Grid>::dim, 
									grid_types<Grid>::dim>
{
  typedef detail::partial_multi_gf_aux<Grid, T,  
			       grid_types<Grid>::dim, 
			       grid_types<Grid>::dim> base;

public: 
  typedef Grid grid_type;
  typedef T    value_type;

  partial_multi_grid_function() {}
  partial_multi_grid_function(const grid_type& g) :  base(g) {}
  partial_multi_grid_function(const grid_type& g, T const& t) :  base(g,t) {}

  template<class E>
  grid_function<E,T> const& ElementFunction() const { return ElementFunction_(E());}

  template<class E>
  unsigned size() const { return ElementFunction<E>().size();}
  
  void set_value(value_type const& t) { set_value_(t);}
  void set_grid(grid_type const& g)   { set_grid_(g);}
  void set_grid(grid_type const& g, value_type const& t) { set_grid_(g); set_value(t); }

  void clear() { base::clear(); ENSURE_ALWAYS(empty(), "", 1);}
};


} // namespace GrAL 

#endif
