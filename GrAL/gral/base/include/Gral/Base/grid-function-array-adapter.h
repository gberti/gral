#ifndef GRAL_GB_BASE_GRID_FUNCTION_ARRAY_ADAPTER_H
#define GRAL_GB_BASE_GRID_FUNCTION_ARRAY_ADAPTER_H

// $LICENSE_NEC

#include "Geometry/point-traits.h"
#include "Gral/Base/common-grid-basics.h"

#include <iostream>

namespace GrAL {

/*!\internal
    \brief Adapter for viewing arrays as grid function

    \ingroup gridfunctions
  
   When adapting existing grid data structures to the GrAL
   interface, we often have to deal with array-based grid functions,
   for example vertex coordinates as an array of 3*nv doubles,
   where nv is the number of vertices. 
   A difficulty here is the definition of the value_type.
   If we try to use the same type as \c value_type, return type of read access
   (\c operator()) and write access (\c operator[]), we run into trouble
   (see the paper "A generic toolbox for the grid craftsman").

   The solution choosen here is to provide different types for write access
   on the one hand, and for value_type and read access, on the other hand.

   \templateparams
    - ELEMENT: an element type
    - T: the value type of the array 
    - the number of values of type T associated to each element
 */

namespace gf_array_adapter {

  template<class T, unsigned N>
  class value_type;

  template<class T, unsigned N>
  class value_proxy {
    T* f; // start address of xyz values of a vertex
    friend class value_type<T,N>;

    value_proxy<T,N> & operator=(value_proxy<T,N>  const&);
  public:
    explicit value_proxy(T* ff) : f(ff) {}
    inline void operator=(value_type<T,N> const& frc);
    
    T const& operator()(int i) const { check_range(i);  return f[i]; }
    T const& operator[](int i) const { check_range(i);  return f[i]; }
    T      & operator[](int i)       { check_range(i);  return f[i]; }

    void check_range(int i) const {
      REQUIRE( 0 <= i && i < (int)N, "i = " << i << " out of range!\n",1);
    }
  };

  template<class T, unsigned N>
  class value_type : public array_operators<value_type<T,N>, T, N> {
    T f[N];
  public:
    value_type() { init(T(0)); }
    value_type(value_proxy<T,N> p)  { init(p.f);}
    value_type(T const* f)          { REQUIRE(f!=0, "", 1); init(f);}
    value_type(T t)                 { init(t);}
    value_type& operator=(value_proxy<T,N> p) { init(p.f); return *this;}


    T const& operator()(int i) const { check_range(i); return f[i];}
    T const& operator[](int i) const { check_range(i); return f[i];}
    T      & operator[](int i)       { check_range(i); return f[i];}

    void check_range(int i) const {
      REQUIRE( 0 <= i && i < (int)N, "i = " << i << " out of range!\n",1);
    }

  private:
    void init(T const* p)
      { for(int i = 0; i < (int)N; ++i) f[i] = p[i];}
    void init(T t) 
    { for(int i = 0; i < (int)N; ++i)   f[i] = t;}
  };

  // specialization for N=1
  template<class T> 
  class value_type<T,1> : public array_operators<value_type<T,1>, T, 1> {
    T f[1];
  public:
    value_type() {}
    value_type(value_proxy<T,1> p)    { init(p.f);}
    value_type(T const* ff)           { init(ff);}
    value_type(T t)                   { f[0] = t;}

    value_type& operator=(value_proxy<T,1> p) { init(p.f); return *this;}


    T const& operator()(int i) const { check_range(i); return f[i];}
    T const& operator[](int i) const { check_range(i); return f[i];}
    T      & operator[](int i)       { check_range(i); return f[i];}
    operator T const&() const { return f[0];}
    operator T      &()       { return f[0];}

    void check_range(int i) const {
      REQUIRE( 0 <= i && i < 1, "i = " << i << " out of range!\n",1);
    }

  private:
    void init(T const* p)
      { for(int i = 0; i < 1; ++i) f[i] = p[i];}
  };



  template<class T, unsigned N>
  inline void value_proxy<T,N>::operator=(value_type<T,N> const& p)
  { for(int i = 0; i < (int)N; ++i) f[i] = p[i]; }

  template<class T, unsigned N>
  inline
  std::ostream& operator<<(std::ostream& out, value_type<T,N> const&v)
  {
    for(unsigned i = 0; i < N; ++i)
      out << v(i) << ' ';
    return out;
  }

} // namespace gf_array_adapter

//! specialization of point_traits<>
template<class T, unsigned N>
struct point_traits<gf_array_adapter::value_type<T,N> > 
  : public point_traits_fixed_size_array<gf_array_adapter::value_type<T,N>, T, N>
{};

  /*! specialization of point_traits<>
      \note This promises more than can be kept, for example
      construction.
   */
template<class T, unsigned N>
struct point_traits<gf_array_adapter::value_proxy<T,N> > 
  : public point_traits_fixed_size_array<gf_array_adapter::value_proxy<T,N>, T, N>
{};
        

/*! \brief Adapter for plain arrays to grid function interface
    \ingroup gridfunctions
   
    The array is assumed to have the following layout:
    \f$ t_{0,1}, t_{0,2}, \ldots t_{0,N}, \ldots t_{M-1,1}, \ldots t_{M-1,N} \f$
    where \f$ M \f$ is the number of elements of the underlying grid of type \ELEMENT,
    and  \f$ t_{i-1,1}, t_{i-1,2}, \ldots t_{i-1,N} \f$ is the vector of \f$ N \f$ items
    of type \c T associated to element \f$ i \f$.

    Example:
    \code
    Grid g; // adapter to user grid DS
    double * x   = new double[  num_of_vertices]; // user grid function on vertices
    double * geo = new double[2*num_of_vertices]; // 2D user geometry
    grid_function_array_adapter Gf1<Grid,double,1>(g,x);
    grid_function_array_adapter Geo<Grid,double,2>(g,geo);
    \endcode

    \see Tested in \ref test-grid-function-array-adapter.C

*/
template<class ELEMENT, class T, unsigned N = 1>
class grid_function_array_adapter 
{
  typedef grid_function_array_adapter<ELEMENT,T,N> self;
public:
  typedef ELEMENT                      element_type;
  typedef element_traits<element_type> et;
  typedef typename et::grid_type       grid_type;
  typedef typename et::ElementIterator ElementIterator;

  typedef gf_array_adapter::value_type<T,N>  value_type;
  typedef gf_array_adapter::value_proxy<T,N> value_proxy;
  typedef value_proxy                        reference;
  typedef const  value_type                  const_reference;

  typedef size_t                             size_type;

  typedef value_type                         result_type;
  typedef element_type                       argument_type;
private: 
  grid_type const* g;

  T * f;
  bool owned;
public:
  grid_function_array_adapter() : g(0), f(0), owned(false) {}
  grid_function_array_adapter(grid_type const& gg)
    : g(&gg), f(new T[et::size(gg)]), owned(true) {}
  grid_function_array_adapter(grid_type const& gg,
			      T * ff)
    : g(&gg), f(ff), owned(false) {}

  ~grid_function_array_adapter() { clear();}
 
  void clear() {
    if(owned) delete [] f;
    g = 0;
    f = 0;
    owned = false;
  }

  grid_type const& TheGrid() const { return *g;}
  size_type size() const { return et::size(*g);}

  ElementIterator FirstElement() const { return et::FirstElement(*g);}


  value_proxy operator[](element_type const& v) 
  { cv(); return value_proxy(f + N*v.handle());}
  const value_type operator()(element_type const& v) const 
  { cv(); return value_type(f + N*v.handle());}

  void cv() const { REQUIRE(valid(), "", 1);}
  bool valid() const { return g != 0 && f != 0;}

  template<class TT, class VALUETYPE, class VALUETYPE_RET>
  class iterator_t {
    typedef iterator_t<TT,VALUETYPE,VALUETYPE_RET>  self;
    T    * f;
    int    i;
  public:
    typedef VALUETYPE                                           value_type;
    typedef VALUETYPE_RET                                       reference;
    typedef typename std::iterator_traits<T *>::difference_type difference_type;
    typedef typename std::iterator_traits<T *>::pointer         pointer;

    explicit iterator_t(T * ff = 0, int ii = 0) : f(ff), i(ii) {}
    
    self& operator++() { ++i; return *this; }
    self  operator++(int) { self tmp(*this); ++i; return tmp;}
    self  operator+(difference_type k) const { return self(f,i+k); }

    reference  operator*() const { return reference(f+N*i);} 
    bool operator==(self const& rhs) const { 
      REQUIRE(f == rhs.f, "", 1);
      return (i == rhs.i);
    }
    bool operator!=(self const& rhs) const { return ! (rhs  == *this);}
    bool operator< (self const& rhs) const { 
      REQUIRE(f == rhs.f, "", 1);
      return (i <  rhs.i);
    }
  };

  typedef iterator_t<T const, value_type, value_type const&>  const_iterator;
  typedef iterator_t<T,       value_type, value_proxy>        iterator;
  typedef typename iterator::difference_type                  difference_type;
  typedef typename iterator::pointer                          pointer;

  const_iterator begin() const { cv(); return const_iterator(f,0);}
  const_iterator end  () const { cv(); return const_iterator(f,size());}
  iterator       begin()       { cv(); return       iterator(f,0);}
  iterator       end  ()       { cv(); return       iterator(f,size());}


};

} // namespace GrAL 

#endif
