#ifndef GRAL_BASE_GB_MULTI_GRIDFUNCTIONS_H
#define GRAL_BASE_GB_MULTI_GRIDFUNCTIONS_H

// $LICENSE

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
    typedef typename grid_types<Grid>::template element_d<ED>::type element_type;
    typedef multi_gf_aux<Grid,T,GD,ED-1>                            base;

    grid_function<element_type, T> f;
  public:
    typedef typename base::value_type      value_type;
    typedef typename base::reference       reference;
    typedef typename base::const_reference const_reference;

    multi_gf_aux() {}
    multi_gf_aux(Grid const& g) : base(g), f(g) {}
    multi_gf_aux(Grid const& g, T const& t) : base(g,t), f(g,t) {}

    using base::operator();
    using base::operator[];
    //  using base::ElementFunction_();
    T  operator()(element_type const& e) const { return f(e);}
    T& operator[](element_type const& e)       { return f[e];}

    grid_function<element_type, T> const& ElementFunction_(element_type const&) const { return f;}
    grid_type const& TheGrid() const { return f.TheGrid();} 

    void set_value_(value_type const& t) { base::set_value_(t);  f.set_value(t);}
    void set_grid_ (grid_type const& g)    { base::set_grid_(g);   f.set_grid(g);}
 };

  // end recursion
  template<class Grid, class T, int GD>
  class multi_gf_aux<Grid, T, GD, 0> 
  {
  public:
    typedef Grid grid_type;
  private:
    typedef typename grid_types<Grid>::Vertex  element_type;

    grid_function<element_type, T> f;
  public:
    typedef T                                                        value_type;
    typedef typename grid_function<element_type, T>::reference       reference;
    typedef typename grid_function<element_type, T>::const_reference const_reference;

    multi_gf_aux() {}
    multi_gf_aux(Grid const& g) : f(g) {}
    multi_gf_aux(Grid const& g, T const& t) :  f(g,t) {}


    T  operator()(element_type const& e) const { return f(e);}
    T& operator[](element_type const& e)       { return f[e];}

    grid_function<element_type, T> const& ElementFunction_(element_type const&) const { return f;}
    grid_type const& TheGrid() const { return f.TheGrid();} 

    void set_value_(value_type const& t) { f.set_value(t);}
    void set_grid_(grid_type const& g)   { f.set_grid(g);}
  };
  

  

} // namespace detail




/*! \brief Total grid function defined on all element types of <Grid>.
    \ingroup gridfunctions

   multi_grid_function<Grid,T> defines a mapping
   \f$ V(G) \cup E(G) \cup C(G) \mapsto T \f$ ( in the 2d case)

   These classes are  fully generic and do not need any further
   specialization, because they build on grid_function<Elt,T>.

   \see \c partial_multi_grid_function
   \see Module \ref gridfunctions
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

  multi_grid_function() {}
  multi_grid_function(const grid_type& g) :  base(g) {}
  multi_grid_function(const grid_type& g, T const& t) :  base(g,t) {}

  template<class E>
  grid_function<E,T> const& ElementFunction() const { return ElementFunction_(E());}

  template<class E>
  unsigned size() const { return ElementFunction<E>().size();}

  void set_value(value_type const& t) { set_value_(t);}
  void set_grid(grid_type const& g)   { set_grid_(g);}
  void set_grid(grid_type const& g, value_type const& t) { set_grid_(g); set_value(t); }
};


} // namespace GrAL 

#endif
