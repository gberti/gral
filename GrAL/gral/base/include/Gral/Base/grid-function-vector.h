#ifndef GRAL_BASE_GB_GRID_FUNCTION_VECTOR_H
#define GRAL_BASE_GB_GRID_FUNCTION_VECTOR_H


// $LICENSE

#include <vector.h> // STL

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
/*! \brief Implements a grid_function on vector-basis.
  \ingroup gridfunctions 
 
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
//----------------------------------------------------------------


template<class E, class T>
class grid_function_vector {
public:
  // container types
  typedef vector<T>                      table_type;
  typedef typename table_type::size_type size_type;

  // grid types
  typedef element_traits<E>            et;
  typedef E                            element_type;
  typedef typename et::ElementIterator ElementIterator;
  typedef typename et::handle_type     element_handle;
  typedef typename et::grid_type       grid_type;

  // unary function interface types
  typedef E const& argument_type;
  typedef T const& result_type;
protected:
  //--  DATA  -------
  grid_type  const* g;
  table_type        table;  // basic container
  
public:
  //--------------------- construction  ------------------
  grid_function_vector() : g((grid_type*)0) , table() {}
  grid_function_vector(const grid_type& gg) 
    : g(&gg),  table(et::size(gg)) {}
  grid_function_vector(const grid_type& gg, const T& t) 
    : g(&gg), table(et::size(gg),t)  {}

  // copying
  typedef grid_function_vector<E,T> gfc2dv;
  grid_function_vector(const gfc2dv& rs) : g(rs.g), table(rs.table)  {}
  gfc2dv& operator=(const gfc2dv& rs) 
    { if (this != &rs) { g =rs.g; table = rs.table;} return *this;}

  /*  void operator=(const T& t) { fill(table.begin(),table.end(),t); } */

  void set_grid(const grid_type& gg) {
    REQUIRE((g == 0), "set grid: grid must be 0!\n",1);
    g = &gg;
    table = table_type(et::size(gg));
  }

  void rebind(const grid_type& gg) {
    g = &gg;
    size_type old_size = table.size();
    size_type new_size = et::size(gg);
    if(old_size > new_size)
      table.erase(table.begin()+et::size(gg),table.end());
    else if (old_size < new_size) {
      table.reserve(new_size);
      for(unsigned i = 0; i < new_size - old_size; ++i)
        table.push_back(T());
    }
    ENSURE( (table.size() == et::size(*g)), "", 1);
  }


  // temporary function to handle enlarge grids 'by hand'
  // (should be automatic by an observer/notifier mechanism)
  void resize(size_type n) { table.resize(n); } 

  //------------------ component access -------------------------

  const grid_type& TheGrid() const {
    REQUIRE((g != 0), "No grid!\n",1);
    return *g;
  }

  ElementIterator FirstElement() const { return et::FirstElement(*g);}


  //--------------  data access operators -----------------------

  const T&   operator()(const E& e) const {
    REQUIRE((0 <= et::handle(e) && et::handle(e) < (int)table.size()), 
	    "grid_function: invalid access: pos = " << et::handle(e),1);
    return table[et::handle(e)];
  }
  
  T&  operator[](const E& e)  {
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    REQUIRE(((0 <= et::handle(e)) && et::handle(e) < (int)table.size()), 
	    "grid_function: invalid access: pos = " << et::handle(e),1);
    return table[et::handle(e)];
  }
  const T&  operator[](const E& e) const  {
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    REQUIRE(((0 <= et::handle(e)) && et::handle(e) < (int)table.size()), 
	    "grid_function: invalid access: pos = " << et::handle(e),1);
    return table[et::handle(e)];
  }

  /*
  T& operator[](const ElementIterator& e)  { 
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    REQUIRE(((0 <= et::handle(e)) && et::handle(e) < (int)table.size()), 
	    "grid_function: invalid access: pos = " << et::handle(e),1);
    return table[et::handle(e)];
  }
  */

  //-------- not for public use - more efficient access ----

  // these access operators are intended for use in some other
  // components, e.g. restricted_grid_function
  // (should be friends, but problems with templates)

  const T&   operator()(const element_handle& e) const {
    REQUIRE(( 0 <= e  && e < (int)table.size()), 
	    "grid_function: invalid access: pos = " << e,1);
    return table[e];
  }
  
  T&  operator[](const element_handle& e)       {
    REQUIRE(( 0 <= e  && e < (int)table.size()), 
	    "grid_function: invalid access: pos = " << e,1);
    return table[e];
  }


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
  typedef T          value_type;
};

#endif
