#ifndef GRAL_GB_GEOMETRY_SIMPLEX_H
#define GRAL_GB_GEOMETRY_SIMPLEX_H

// $LICENSE_NEC_2003


#include "Geometry/point-traits.h"
//#include "Container/integer-iterator.h"

#include "Container/sequence-algorithms.h"
#include <algorithm>

namespace GrAL {

template<unsigned K, class P = int>
class simplex_base {
  typedef  simplex_base self;
public:
  typedef P                           coord_type;
  typedef point_traits<P>             pt;
  typedef typename pt::component_type scalar_type;
  typedef size_t                      size_type;
protected:
  coord_type  p[K+1];
public:
  simplex_base() {}

  template<class It>
  simplex_base(It b, It e) { ::std::copy(b,e,p);}

  simplex_base(P const& apex, simplex_base<K-1, P> base)
  { p[0] = apex; ::std::copy(base.begin(), base.end(), p+1);}

  coord_type const& operator[](int i) const { return p[i];}
  coord_type      & operator[](int i)       { return p[i];}
  coord_type const& operator()(int i) const { return p[i];}

  int dimension() const { return K;}
  int space_dimension() const { return pt::Dim(p[0]);}
  /*
  typedef integer_iterator<int, self>     VertexIterator;
  VertexIterator FirstVertex() const { return VertexIterator(0);}
  VertexIterator EndVertex()   const { return VertexIterator(K+1);}

  typedef coord_type const*  geom_vertex_iterator;
  geom_vertex_iterator begin_vertex() const { return p;}
  geom_vertex_iterator end_vertex()   const { return p+K+1;}
  */


  typedef coord_type const*  const_iterator;
  typedef coord_type      *  iterator;
  const_iterator begin() const { return p;}
  const_iterator end  () const { return p+K+1;}
  iterator begin() { return p;}
  iterator end  () { return p+K+1;}
  size_type size() const { return K+1;}
};


template<unsigned K, class P = int>
class simplex : public simplex_base<K, P>
{
  typedef simplex_base<K, P> base;
public:
  simplex() {}
  template<class It>
  simplex(It b, It e) : simplex_base<K,P>(b,e) {}
  simplex(P const& P0, simplex<K-1,P> const& b) : base(P0,b) {}
};

template<class P>
class simplex<0,P> : public simplex_base<0,P>
{
  typedef simplex_base<0, P> base;
  using base::p;
public:
  simplex() {}
  simplex(P const& P0) { p[0]=P0;}
};

template<class P>
class simplex<1,P> : public simplex_base<1,P>
{
  typedef simplex_base<1, P> base;
  using base::p;
public:
  simplex() {}
  simplex(P const& P0, P const& P1) { p[0]=P0;p[1]=P1;}
  simplex(P const& P0, simplex<0,P> const& b) : base(P0, b) {}
};

template<class P>
class simplex<2,P> : public simplex_base<2,P>
{
  typedef simplex_base<2, P> base;
  using base::p;
public:
  simplex() {}
  simplex(P const& P0, P const& P1, P const& P2) { p[0]=P0;p[1]=P1;p[2]=P2;}
  simplex(P const& P0, simplex<1,P> const& b) : base(P0, b) {}
};

template<class P>
class simplex<3,P> : public simplex_base<3,P>
{
  typedef simplex_base<3, P> base;
  using base::p;
public:
  simplex() {}
  simplex(P const& P0, P const& P1, P const& P2, P const& P3) { p[0]=P0;p[1]=P1;p[2]=P2;p[3]=P3;}
  simplex(P const& P0, simplex<2,P> const& b) : base(P0, b) {}
};




struct compare_ordered {
  template<unsigned K, class P>
  static bool less(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
  { 
    simplex<K,P> sorted_lhs(lhs); sequence::bubble_sort(sorted_lhs.begin(), sorted_lhs.end());
    simplex<K,P> sorted_rhs(rhs); sequence::bubble_sort(sorted_rhs.begin(), sorted_rhs.end());
    return ::std::lexicographical_compare(sorted_lhs.begin(), sorted_lhs.end(),
					sorted_rhs.begin(), sorted_rhs.end());
  }
  
  template<unsigned K, class P>
  bool operator()(simplex<K,P> const& lhs, simplex<K,P> const& rhs) const { return less(lhs,rhs);}
};


struct compare_unordered {
  template<unsigned K, class P>
  static bool less(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
  { 
    return ::std::lexicographical_compare(lhs.begin(), lhs.end(),
					rhs.begin(), rhs.end());
  }
  template<unsigned K, class P>
  bool operator()(simplex<K,P> const& lhs, simplex<K,P> const& rhs) const { return less(lhs,rhs);}
};

struct equal_ordered {
  template<unsigned K, class P>
  static bool equal(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
  { 
    simplex<K,P> sorted_lhs(lhs); sequence::bubble_sort(sorted_lhs.begin(), sorted_lhs.end());
    simplex<K,P> sorted_rhs(rhs); sequence::bubble_sort(sorted_rhs.begin(), sorted_rhs.end());
    return ::std::equal(sorted_lhs.begin(), sorted_lhs.end(),
		      sorted_rhs.begin());
  }

  template<unsigned K, class P>
  bool operator()(simplex<K,P> const& lhs, simplex<K,P> const& rhs)  const { return equal(lhs, rhs);}
};

struct equal_unordered {
  template<unsigned K, class P>
  static bool equal(simplex<K,P> const& lhs, simplex<K,P> const& rhs)
  { 
    return ::std::equal(lhs.begin(), lhs.end(),
		      rhs.begin());
  }
  template<unsigned K, class P>
  bool operator()(simplex<K,P> const& lhs, simplex<K,P> const& rhs)  const { return equal(lhs, rhs);}
};


template<unsigned K, class P>
bool operator<(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
{ return compare_ordered::less(lhs,rhs); }

template<unsigned K, class P>
bool operator>(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
{ return (rhs < lhs);}

template<unsigned K, class P>
bool operator==(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
{ return equal_ordered::equal(lhs,rhs); }

template<unsigned K, class P>
bool operator!=(simplex<K,P> const& lhs, simplex<K,P> const& rhs) 
{ return !(lhs==rhs);}



} // namespace GrAL 


#endif
