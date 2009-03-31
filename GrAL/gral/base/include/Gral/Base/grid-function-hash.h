#ifndef GRAL_BASE_GB_GRID_FUNCTION_HASH_H
#define GRAL_BASE_GB_GRID_FUNCTION_HASH_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/my-hash-map.h" // STL adapted

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

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
  instead of <tt> hash_table<Element,T></tt>.
  However, in some cases it is not possible to implement equality testing
  of the underlying elements without referring to the grid.

  \see  grid_function_vector
  \see  \ref gridfunctions
*/


template<class E, class T>
class grid_function_hash_base {
public:

  typedef element_traits<E>                 et;
  typedef E                                 element_type;
  typedef typename et::hasher_type          hasher_type;
  typedef typename et::grid_type            grid_type;


  typedef STDHASH::hash_map<E,T, hasher_type>   table_type;
  typedef typename table_type::size_type        size_type;
  typedef T                                     value_type;
  //  typedef typename table_type::reference        reference;
  //  typedef typename table_type::const_reference  const_reference;
  typedef T      &  reference;
  typedef T const&  const_reference;
  typedef typename table_type::pointer         pointer;
  typedef typename table_type::difference_type difference_type;

  typedef E const&        argument_type;
  typedef const_reference result_type;

protected:
  //--- DATA ---------
  ref_ptr<grid_type  const> g;
  table_type                table;  // base container

public:
  grid_function_hash_base() :  table() {}
  grid_function_hash_base(const grid_type& gg) 
    : g(gg), table()  {}
  grid_function_hash_base(ref_ptr<grid_type const> gg) 
    : g(gg), table()  {}

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

  void set_grid(ref_ptr<grid_type const> gg) {
    // REQUIRE((g == 0), "set grid: grid must be 0!\n",1);
    g = gg;
  }
  void set_grid(const grid_type& gg) { set_grid(ref_ptr<grid_type const>(gg));}


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

  unsigned size()  const { return table.size();}  
  bool     empty() const { return table.empty();}
  void     clear()       { table.clear(); ENSURE_ALWAYS(empty(), "",1); }

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

  iter       begin()       { return iter      (table.begin());}
  iter       end()         { return iter      (table.end());}
  const_iter begin() const { return const_iter(table.begin());}
  const_iter end()   const { return const_iter(table.end());}
 

  // typedefs for container categorie
  // done here because of compiler errors
  // if done earlier.
  typedef iter       iterator;
  typedef const_iter const_iterator;
};


//------------------------------------------------------------------------------


/*!  
   \brief Implements a total grid_function on hash-basis.
   \ingroup gridfunctions
 
    Model of $GrAL TotalGridFunction.

   \see \ref gridfunctions module 
   \todo Iteration does not work when a grid function is constructed
   using only the grid (gives an empty sequence instead a full sequence of
   undefined values).
*/
template<class E, class T> 
class grid_function_hash
  : public grid_function_hash_base<E,T>
{
  typedef grid_function_hash_base<E,T> base_gf;

  using base_gf::table;
public:
  typedef total_grid_function_category         category;
  typedef element_traits<E> et1;
  typedef typename et1::ElementIterator     ElementIterator;
  typedef typename base_gf::grid_type       grid_type;
  typedef typename base_gf::reference       reference;
  typedef typename base_gf::const_reference const_reference;
  using base_gf::TheGrid;

  template<class EE, class TT> struct apply { typedef grid_function_hash<EE,TT> type; };

  //--------------------- construction ------------------------------   

  grid_function_hash() {}
  grid_function_hash(const grid_type& gg) : base_gf(gg) {}
  grid_function_hash(const grid_type& gg, const T& t) 
    : base_gf(gg) 
    {
      set_value(t);
    }
  grid_function_hash(ref_ptr<grid_type const> gg) : base_gf(gg) {}
  grid_function_hash(ref_ptr<grid_type const> gg, const T& t) 
    : base_gf(gg) 
    {
      set_value(t);
    }
  ElementIterator FirstElement() const { return et1::FirstElement(TheGrid());}
  ElementIterator EndElement()   const { return et1::EndElement  (TheGrid());}


  /*! Set all value to \c t.
   */
  void set_value(T const& t) {
    for(ElementIterator e = et1::FirstElement(TheGrid()); ! e.IsDone(); ++e)
      table[*e] = t;    
  }

  void init(ref_ptr<grid_type const> gg, const T& t) {
    set_grid(gg);
    set_value(t);
  }
  void init(const grid_type& gg, const T& t) { init(ref_ptr<grid_type const>(gg), t);}

  //----------------- data access operators -------------------------   

  /*! \brief Read-only access
   */
  const_reference operator()(const E& e) const {
    /*
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    */
    REQUIRE( defined(e), "grid_function not defined for item ",1);
    return (* (table.find(e))).second;
  }
  
  /*! \brief Write access.
     \note: if <tt> !defined(e)</tt>, a new entry is created!
      Use operator() for read-only access.
  */
  reference  operator[](const E& e)       {
    /*
    REQUIRE( (&(e.TheGrid()) == g),
	     "gf(e): Grids don't match: e.TheGrid() = "
	     << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    */
    return (table[e]);
  }

};

} // namespace GrAL 

#endif
