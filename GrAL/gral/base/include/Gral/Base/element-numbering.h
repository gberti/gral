#ifndef GRAL_GB_BASE_ELEMENT_NUMBERING_H
#define GRAL_GB_BASE_ELEMENT_NUMBERING_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"


template<class E, class consecutive_tag>
class element_numbering_aux {};



template<class E, int N>
class element_numbering_aux<E, consecutive_integer_tag<N> > 
{

public:
  typedef E element_type;
  typedef element_traits<E> et;
  typedef typename et::grid_type grid_type;
private:
  grid_type const* g;
  int   offset_;
public:
  element_numbering_aux(grid_type const& gg, int offset = 0) 
    : g(&gg), offset_(offset) {}

  int operator()(element_type const& e) const 
    { return (e.handle() - N + offset_);}

  grid_type const& TheGrid() const { return *g;}

};


template<class E>
class element_numbering_aux<E, non_consecutive_tag > 
{

public:
  typedef E element_type;
  typedef element_traits<E> et;
  typedef typename et::grid_type grid_type;
private:
  grid_function<element_type, int> num;
  int   offset_;
public:
  element_numbering_aux(grid_type const& gg, int offset = 0) 
    : num(gg), offset_(offset) 
    {
      int n = offset_;
      for(typename et::ElementIterator e(num.TheGrid()); ! e.IsDone(); ++e)
	num[*e] = n++;
    }

  int operator()(element_type const& e) const 
    { return num(e);}

  grid_type const& TheGrid() const { return num.TheGrid();}

};


/*! Provide a consecutive numbering of grid elements
 
    This class has an important specialization in the 
    case that the corresponding elements are already numbered
    in a consecutive way. Then, we can compute the enumeration
    on the fly, by simply adding the offset. Else, we must use
    a grid function.

 */

template<class E>
class element_numbering :
  public element_numbering_aux<E, typename element_traits<E>::consecutive_tag>
{
  typedef element_numbering_aux<E, typename element_traits<E>::consecutive_tag> base;
public:
  element_numbering(typename base::grid_type const& g,
		    int offset = 0) : base(g, offset) {}
};

#endif
