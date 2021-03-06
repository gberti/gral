#ifndef GRAL_BASE_GB_GRID_FUNCTION_VECTOR_H
#define GRAL_BASE_GB_GRID_FUNCTION_VECTOR_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <vector> // STL

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

//----------------------------------------------------------------
/*! \brief Implements a grid_function on vector-basis.
  \ingroup gridfunctions 

  Model of $GrAL TotalGridFunction.

  This can be used if
  elements of type E are numbered consecutively. 

  \b Example:
  \code
  class MyVertex; // vertex_handle is consecutive integer
  
  template<class T>
  class grid_function<MyVertex, T> 
   : public grid_function_vector<MyVertex,T> 
  {
    // repeat constructors
  };
  \endcode

  \see grid_function_hash
  \see  \ref gridfunctions

*/

template<class E, class T>
class grid_function_vector {
public:
  typedef total_grid_function_category         category;
  // container types
  typedef std::vector<T>                       table_type;
  typedef typename table_type::size_type       size_type;
  typedef typename table_type::value_type      value_type;
  typedef typename table_type::reference       reference;
  typedef typename table_type::const_reference const_reference;
  typedef typename table_type::pointer         pointer;
  typedef typename table_type::difference_type difference_type;

  // grid types
  typedef element_traits<E>            et;
  typedef E                            element_type;
  typedef typename et::ElementIterator ElementIterator;
  typedef typename et::handle_type     element_handle;
  typedef typename et::grid_type       grid_type;

  template<class EE, class TT> struct apply { typedef grid_function_vector<EE,TT> type; };

  // unary function interface types
  typedef E const& argument_type;
  //typedef T const& result_type;
  typedef const_reference result_type;
protected:
  //--  DATA  -------
  ref_ptr<grid_type const> g;
  table_type        table;  // basic container
  
public:
  //--------------------- construction  ------------------
  grid_function_vector() : g() , table() {}
  grid_function_vector(const grid_type& gg) 
    : g(gg),  table(et::size(gg)) {}
  grid_function_vector(const grid_type& gg, const T& t) 
    : g(gg), table(et::size(gg),t)  {}
  grid_function_vector(ref_ptr<grid_type const> gg) 
    : g(gg),  table(et::size(gg)) {}
  grid_function_vector(ref_ptr<grid_type const> gg, const T& t) 
    : g(gg), table(et::size(gg),t)  {}

  // copying
  typedef grid_function_vector<E,T> gfc2dv;
  grid_function_vector(const gfc2dv& rs) : g(rs.g), table(rs.table)  {}
  gfc2dv& operator=(const gfc2dv& rs) 
    { if (this != &rs) { g =rs.g; table = rs.table;} return *this;}

  /*! \brief set each value to \c t.
      
      \post <tt> gf(e)  == t </tt>  for all \c e
  */
  void set_value(value_type const& t) {  ::std::fill(table.begin(),table.end(),t); } 

  /*! \brief Copy sequence <tt> [t_begin, t_end[ </tt>

      \post <tt>  ::std::equal(begin(), end(), t_begin(), t_end()) </tt>
  */
  template<class ForwardIterator>
  void set_values(ForwardIterator t_begin, ForwardIterator t_end)
  {
     std::copy(t_begin, t_end, table.begin());
  }

  void init(ref_ptr<grid_type const> gg, const T& t) {
    REQUIRE((g == 0), "grid_function<>::init: grid must be 0!\n",1);
    g = gg;
    table.resize(et::size(*g),t);
  }
  void init(const grid_type& gg, const T& t) { init(ref_ptr<grid_type const>(gg), t);}

  void set_grid(ref_ptr<grid_type const> gg) {
    REQUIRE((g == 0), "set grid: grid must be 0!\n",1);
    g = gg;
    table = table_type(et::size(*g));
  }
  void set_grid(const grid_type& gg) { set_grid(ref_ptr<grid_type const>(gg));}

  void rebind(ref_ptr<grid_type const> gg) {
    g = gg;
    table.resize(et::size(*g));
    ENSURE( (table.size() == (size_type)et::size(*g)), "", 1);
  }
  void rebind(const grid_type& gg) { rebind(ref_ptr<grid_type const>(gg));}

  // temporary function to handle enlarge grids 'by hand'
  // (should be automatic by an observer/notifier mechanism)
  void resize(size_type n) { table.resize(n); } 

  //------------------ component access -------------------------

  // should this return ref_ptr<grid_type const> ?
  const grid_type& TheGrid() const {
    REQUIRE((g != 0), "No grid!\n",1);
    return *g;
  }

  ElementIterator FirstElement() const { return et::FirstElement(*g);}
  ElementIterator EndElement()   const { return et::EndElement  (*g);}


  //--------------  data access operators -----------------------

  const_reference  operator()(const E& e) const { cv(e); return table[et::handle(e)]; }
  reference        operator[](const E& e)       { cv(e); return table[et::handle(e)]; }
  const_reference  operator[](const E& e) const { cv(e); return table[et::handle(e)]; }

  bool valid(element_handle e) const { return bound() && (element_handle(0) <= e) && (e < element_handle(table.size()));}
  bool bound()                 const { return g != 0;}
  void cv(const E& e)          const { cv(e.handle());}
  void cv(element_handle e)    const { REQUIRE(valid(e), "grid_function: invalid access: pos = " << e,1); }
  void cb()                    const { REQUIRE(bound(), "", 1);}

  // T& operator[](const ElementIterator& e)  { cv(e);  return table[et::handle(e)];}

  //-------- not for public use - more efficient access ----

  // these access operators are intended for use in some other
  // components, e.g. restricted_grid_function
  // (should be friends, but problems with templates)

  const_reference  operator()(const element_handle& e) const { cv(e); return table[e]; }
  reference        operator[](const element_handle& e)       { cv(e); return table[e]; }


  //--------------- Container interface ----------

  unsigned size() const { return table.size();}  

  typedef typename table_type::iterator       iter;
  typedef typename table_type::const_iterator const_iter;
  iter       begin()       { return iter(table.begin());}
  iter       end()         { return iter(table.end());}
  const_iter begin() const { return const_iter(table.begin());}
  const_iter end()   const { return const_iter(table.end());}
 

  // typedefs for container categorie
  // done here because of compiler errors
  // if done earlier.
  typedef iter       iterator;
  typedef const_iter const_iterator;
};

} // namespace GrAL 

#endif
