#ifndef GRAL_BASE_GB_PARTIAL_GRID_FUNCTION_HASH_H
#define GRAL_BASE_GB_PARTIAL_GRID_FUNCTION_HASH_H


// $LICENSE

#include "Container/my-hash-map.h" // STL adapted
#include "Utility/pre-post-conditions.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Base/map-element-iter-names.h"

namespace GrAL {

/*!  \class partial_grid_function
   \brief Implements a partial grid function using hash tables.
   \ingroup gridfunctions
   \see \ref gridfunctions module
  
   This class template supplies a generic implementation
   of the $GrAL PartialGridFunction concept 
   - no partial specializations to the element type \c E are required.


   \templateparams
   \c element_traits<E>::hasher_type
   is a hash function for
   \c element_traits<E>::element_handle.


*/


/* Generic element iterator for partial grid functions 
   Could be a nested class of partial_grid_function
*/
template<class E, class T>
class ElementIterator_generic_pgf 
 : public grid_function_hash_base<E,T>::base_cit 
{
   typedef typename grid_function_hash_base<E,T>::base_cit base_citer;
private:
    base_citer end;
public:
  typedef E element_type;
  typedef element_traits<E> et;
  typedef typename et::handle_type element_handle;
  typedef typename et::grid_type   grid_type;

  ElementIterator_generic_pgf() {}
  ElementIterator_generic_pgf(const base_citer& i, const base_citer& e) 
    : base_citer(i), end(e)
    {}
  element_type operator*() const   { return element();}
  bool IsDone()  const { return (end == ((const base_citer&) (*this)));}
  element_handle       handle()  const { return element().handle();}

  // this does work only if element() is valid, i.e. !IsDone().
  grid_type     const& TheGrid() const { return element().TheGrid();}
private:
  element_type const& element() const { return (base_citer::operator*()).first;}
};



template<class E, class T>
class partial_grid_function 
  : 
  public grid_function_hash_base<E,T>,
  //  public map_element_iter_name<ElementIterator_generic_pgf<E,T>, 
  //                             typename element_traits<E>::element_type_tag>
  public map_element_iter_name<ElementIterator_generic_pgf<E,T>,
                               partial_grid_function<E,T>,
                               typename element_traits<E>::element_type_tag>
{
  typedef grid_function_hash_base<E,T> base_gf;
  
private:
  T default_value;

  using base_gf::table;
public:
  typedef typename base_gf::grid_type  grid_type;
  typedef typename base_gf::table_type table_type;

  partial_grid_function() 
    : base_gf(), default_value(T()) {} 
  partial_grid_function(const grid_type& gg) 
    : base_gf(gg), default_value(T()) {} 
  partial_grid_function(const grid_type& gg, const T& dflt)
    : base_gf(gg), default_value(dflt)  {}
  partial_grid_function(ref_ptr<grid_type const> gg) 
    : base_gf(gg), default_value(T()) {} 
  partial_grid_function(ref_ptr<grid_type const> gg, const T& dflt)
    : base_gf(gg), default_value(dflt)  {}


  void init(grid_type const& gg, T const& t) { init(ref_ptr<grid_type const>(gg), t);}
  void init(ref_ptr<grid_type const> gg, const T& t) {
    set_grid(gg);
    set_default(t);
  }
  
  //----------------- data access operators -------------------------   

   // read access:
  const T&   operator()(const E& e) const {
    /*
    REQUIRE( (&(e.TheGrid()) == g),
             "gf(e): Grids don't match: e.TheGrid() = "
             << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    */
    typename table_type::const_iterator i = table.find(e);
    return ( (i != table.end()) ?
             (*i).second
             : default_value);
  }

  // write access:
  T&  operator[](const E& e)       { 
    /*
    REQUIRE( (&(e.TheGrid()) == g),
             "gf[e]: Grids don't match: e.TheGrid() = "
             << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    */
    typename table_type::iterator i = table.find(e);
    typedef typename table_type::value_type table_value;
    if(i == table.end()) 
      i = (table.insert(table_value(e,default_value))).first ;
    return ((*i).second);
    //    return (table[e]);
  }

  /*! \brief set each value to \c t.
      
      \post <tt> gf(e)  == t </tt>  for all \c e
   */
  void set_value(T const& t) 
  {
    set_default(t);
    for(typename table_type::iterator i = table.begin(); i != table.end(); ++i)
      (*i).second = t;
  }

  const T& get_default() const { return default_value;}
  void set_default(const T& t) { default_value = t;}


  typedef ElementIterator_generic_pgf<E,T> ElementIterator;
  ElementIterator FirstElement() const { return ElementIterator(table.begin(),table.end());}


};

} // namespace GrAL 

#endif
