#ifndef GRAL_GB_BASE_GRID_FUNCTION_ARRAY_ADAPTER_H
#define GRAL_GB_BASE_GRID_FUNCTION_ARRAY_ADAPTER_H

// $LICENSE_NEC

#include "Geometry/point-traits.h"
#include "Gral/Base/common-grid-basics.h"

#include <iostream>

/*! \brief Adapter for viewing arrays as grid function

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


template<class ELEMENT, class T, unsigned N = 1>
class grid_function_array_adapter 
{
  typedef grid_function_array_adapter<ELEMENT,T,N> self;
public:
  typedef ELEMENT element_type;
  typedef element_traits<element_type> et;
  typedef typename et::grid_type       grid_type;
  typedef typename et::ElementIterator ElementIterator;

  typedef size_t size_type;
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


  class value_type;
  class value_proxy {
    T* f; // start address of xyz values of a vertex
    friend class value_type;
  public:
    value_proxy(T* ff) : f(ff) {}
    inline void operator=(value_type const& frc);
    
    T const& operator()(int i) const { check_range(i);  return f[i]; }
    T const& operator[](int i) const { check_range(i);  return f[i]; }
    T      & operator[](int i)       { check_range(i);  return f[i]; }

    void check_range(int i) const {
      REQUIRE( 0 <= i && i < N, "i = " << i << " out of range!\n",1);
    }
  };

  class value_type : public array_operators<value_type, N> {
    T f[N];
  public:
    value_type() {}
    value_type(value_proxy p)            { init(p.f);}
    value_type(T const* f)               { init(f);}
    value_type& operator=(value_proxy p) { init(p.f); return *this;}


    T const& operator()(int i) const { check_range(i); return f[i];}
    T const& operator[](int i) const { check_range(i); return f[i];}
    T      & operator[](int i)       { check_range(i); return f[i];}

    void check_range(int i) const {
      REQUIRE( 0 <= i && i < (int)N, "i = " << i << " out of range!\n",1);
    }

  private:
    void init(T const* p)
      { for(int i = 0; i < (int)N; ++i) f[i] = p[i];}
  };
  typedef value_type value_t;

  value_proxy operator[](element_type const& v) 
    { return value_proxy(f + N*v.handle());}
  value_type  operator()(element_type const& v) const 
    { return value_type(f + N*v.handle());}

};


template<class ELEMENT, class T, unsigned N>
inline void
grid_function_array_adapter<ELEMENT,T,N>
::value_proxy::operator=
(typename grid_function_array_adapter<ELEMENT,T,N>::value_type const& p)
{ for(int i = 0; i < N; ++i) f[i] = p[i]; }

template<class ELEMENT, class T, unsigned N>
struct point_traits<typename grid_function_array_adapter<ELEMENT,T,N>::value_type> 
  : public point_traits_fixed_size_array
        <typename grid_function_array_adapter<ELEMENT,T,N>::value_type,T,N> {};


template<class ELEMENT, class T, unsigned N>
inline
std::ostream& operator<<
(std::ostream& out,
 typename grid_function_array_adapter<ELEMENT,T,N>::
 value_type<ELEMENT,T,N>  const &v)
{
 for(unsigned i = 0; i < N; ++i)
   out << v(i) << ' ';
 return out;
}

#endif
