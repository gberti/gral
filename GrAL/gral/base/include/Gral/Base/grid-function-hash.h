#ifndef GRAL_BASE_GB_GRID_FUNCTION_HASH_H
#define GRAL_BASE_GB_GRID_FUNCTION_HASH_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Container/my-hash-map.h" // STL adapted

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
/*!  
   \brief Implements  a grid_function on hash-basis.
   \ingroup gridfunctions

  This can be used if arrays are not possible,
  or for partial grid functions.

  Access operations have slightly different semantics for
  total and partial grid-functions, so corresponding derivations
  grid_function_hash  and
  partial_grid_function_hash 
  are provided.

  The main difference between a partial grid function and a
  hashed total grid function is the handling of read access to
  undefined item: in the total version this is an error
  (program termination in debug mode!), whereas for the partial version a
  default value (which can be manipulated seperately) is returned.

 \todo
  \b Optimization:
  Could use  <tt> hash_table<element_handle,T> </tt>
  instead of <tt> hash_table<Element,T> </tt>

  \see  grid_function_vector
  \see  \ref gridfunctions
*/
//----------------------------------------------------------------

template<class E, class T>
class grid_function_hash_base {
public:

  typedef element_traits<E>                 et;
  typedef E                                 element_type;
  typedef typename et::hasher_type          hasher_type;
  typedef typename et::grid_type            grid_type;


  typedef hash_map<E,T, hasher_type, equal_to<E> > table_type;
  typedef typename table_type::size_type           size_type;

  typedef E const& argument_type;
  typedef T const& result_type;

protected:
  //--- DATA ---------
  grid_type  const* g;
  table_type        table;  // base container

public:
  grid_function_hash_base() : g((grid_type*)0) , table() {}
  grid_function_hash_base(const grid_type& gg) 
    : g(&gg), table()  {}

  // copying
  typedef grid_function_hash_base<E,T> gfc2dh;
  grid_function_hash_base(const gfc2dh& rs) : g(rs.g), table(rs.table)  {}
  gfc2dh& operator=(const gfc2dh& rs) 
    { 
      if (this != &rs) {
	 g =rs.g; 
	 table = rs.table;
      } 
      return *this;
    }

  /* 
  void operator=(const T& t) {
    for(ElementIterator e = et::FirstElement(TheGrid()); ! e.IsDone(); ++e)
      table[*e] = t;
  }
  */

  void set_grid(const grid_type& gg) {
    REQUIRE((g == 0), "set grid: grid must be 0!\n",1);
    g = &gg;
  }

  const grid_type& TheGrid() const {
    REQUIRE((g != 0), "No grid!\n",1);
    return *g;
  }


  bool defined(const E& e)  const { return (table.find(e) != table.end());}
  void undefine(const E& e) { 
    table.erase(e);
    ENSURE( (! defined(e)), "undefine(e), but e is still defined!", 1);
  }

  //------------- Container interface ------------------------

  unsigned size() const { return table.size();}  

  typedef typename table_type::iterator base_it;
  class iter : public base_it {
  public:
    iter() {}
    iter(const base_it& i) : base_it(i) {}
    T& operator*() const { return (base_it::operator*()).second;}
  };

  typedef typename table_type::const_iterator base_cit;
  class const_iter : public base_cit {
  private:
  public:
    const_iter() {}
    const_iter(const iter& i) : base_cit(i) {}
    const_iter(const base_cit& i) : base_cit(i) {}
    const T& operator*() const { return (base_cit::operator*()).second;}
  };

  iter begin() { return iter(table.begin());}
  iter end()   { return iter(table.end());}
  const_iter begin() const {return const_iter(table.begin());}
  const_iter end()   const {return const_iter(table.end());}
 

  // typedefs for container categorie
  // done here because of compiler errors
  // if done earlier.
  typedef iter       iterator;
  typedef const_iter const_iterator;
  typedef T          value_type;
};


//------------------------------------------------------------------------------


/*!  
   \brief Implements a total grid_function on hash-basis.
   \ingroup gridfunctions
*/
template<class E, class T> 
class grid_function_hash
  : public grid_function_hash_base<E,T>
{
  typedef grid_function_hash_base<E,T> base_gf;
public:
  typedef element_traits<E> et1;
  typedef typename et1::ElementIterator ElementIterator;

  //--------------------- construction ------------------------------   

  grid_function_hash() {}
  grid_function_hash(const grid_type& gg) : base_gf(gg) {}
  grid_function_hash(const grid_type& gg, const T& t) 
    : base_gf(gg) // g(&gg), table(et1::size(gg))  
    {
      for(ElementIterator e = et1::FirstElement(TheGrid()); ! e.IsDone(); ++e)
	table[*e] = t;
    }

  //----------------- data access operators -------------------------   

  const T&   operator()(const E& e) const {
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    REQUIRE( defined(e), "grid_function not defined for item ",1);
    return (* (table.find(e))).second;
  }
  
  // NOTE: if(!defined(e)) => a new entry is created!
  // use operator() for read access.
  T&  operator[](const E& e)       {
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    return (table[e]);
  }

};

#endif
