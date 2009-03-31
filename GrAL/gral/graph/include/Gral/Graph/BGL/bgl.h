#ifndef GRAL_GB_GRAPH_BGL_BGL_H
#define GRAL_GB_GRAPH_BGL_BGL_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <boost/ref.hpp>
#include <boost/config.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

#include "Gral/Graph/base.h"

/*! \file 
   
   Generic stuff for adapting GrAL graph views to BGL (Boost Graph Library).
*/

namespace boost {

  template<class DIRECTED_TAG>  struct gral2bgl {};
  template<> struct gral2bgl<GrAL::graph::  directed_graph_tag> { typedef boost::  directed_tag type;};
  template<> struct gral2bgl<GrAL::graph::undirected_graph_tag> { typedef boost::undirected_tag type;};


  // These specializations are needed because operator[] does not get called across a reference_wrapper

  template<class E, class T, class GRAPH, class TAG>
  struct reference_wrapper<GrAL::graph::vertex_property_map<E,T, GRAPH, TAG> >
  {
    typedef GrAL::graph::vertex_property_map<E,T, GRAPH, TAG> type;
    typedef typename type::value_type value_type;
    typedef typename type::key_type   key_type;
    typedef typename type::reference  reference;
    typedef typename type::const_reference  const_reference;
    
    type * t_;
    explicit reference_wrapper(type& t): t_(&t) {}

    type& get()         const { return *t_;}
    type* get_pointer() const { return  t_;}
    operator type&() const { return *t_;}

    reference       operator[](key_type k)       { return (*t_)[k];}
    const_reference operator[](key_type k) const { return (*t_)[k];}
    const_reference operator()(key_type k) const { return (*t_)(k);}
  };

  template<class E, class T, class GRAPH, class TAG>
  struct reference_wrapper<GrAL::graph::edge_property_map<E,T, GRAPH, TAG> >
  {
    typedef GrAL::graph::edge_property_map<E,T, GRAPH, TAG> type;
    typedef typename type::value_type value_type;
    typedef typename type::key_type   key_type;
    typedef typename type::reference  reference;
    typedef typename type::const_reference  const_reference;
    
    type * t_;
    explicit reference_wrapper(type& t): t_(&t) {}

    type& get()         const { return *t_;}
    type* get_pointer() const { return  t_;}
    operator type&() const { return *t_;}
    reference       operator[](key_type k)       { return (*t_)[k];}
    const_reference operator[](key_type k) const { return (*t_)[k];}
    const_reference operator()(key_type k) const { return (*t_)(k);}
  };

  // property traits specializations for vertex/edge_property_maps, + reference_wrapped versions

  template<class E, class T, class GRAPH, class TAG>
  struct property_traits<reference_wrapper<GrAL::graph::vertex_property_map<E,T, GRAPH, TAG> > >
    : public property_traits<GrAL::graph::vertex_property_map<E,T, GRAPH, TAG> > {};

  template<class E, class T, class GRAPH, class TAG>
  struct property_traits<reference_wrapper<GrAL::graph::vertex_property_map<E,T, GRAPH, TAG> const> >
    : public property_traits<GrAL::graph::vertex_property_map<E,T, GRAPH, TAG> >{};


  template<class E, class T, class GRAPH, class TAG> 
  struct property_traits<reference_wrapper<GrAL::graph::edge_property_map<E,T, GRAPH, TAG> > >
    : public property_traits<GrAL::graph::edge_property_map<E,T, GRAPH, TAG> > {};

  template<class E, class T, class GRAPH, class TAG>
  struct property_traits<reference_wrapper<GrAL::graph::edge_property_map<E,T, GRAPH, TAG> const> >
    : public property_traits<GrAL::graph::edge_property_map<E,T, GRAPH, TAG> > {};


} // namespace boost 

namespace GrAL { namespace graph {

  // put/get overloads for vertex/edge property maps

  template<class E, class T, class GRAPH, class TAG>
  T get(vertex_property_map<E,T, GRAPH, TAG>             const& pm, 
	typename vertex_property_map<E,T, GRAPH, TAG >::key_type v)
  { return pm(v);}

  template<class E, class T, class GRAPH, class TAG>
  void put(vertex_property_map<E,T, GRAPH, TAG>                    & pm,
	   typename vertex_property_map<E,T,GRAPH,TAG>::key_type v,
	                                                              T t) 
  { pm[v] = t;}

  template<class E, class T, class GRAPH, class TAG>
  T get(edge_property_map<E,T, GRAPH, TAG>             const& pm,
	typename edge_property_map<E,T, GRAPH, TAG >::key_type e) 
  { return pm(e);}

  template<class E, class T, class GRAPH, class TAG>
  void put(edge_property_map<E,T, GRAPH, TAG >                  & pm,
	   typename edge_property_map<E,T, GRAPH, TAG >::key_type e,
	   	                                                             T t) 
  { pm[e] = t;}


}} // namespace GrAL { namespace graph 

namespace boost {

  // put/get overloads for reference wrapped property maps

  template<class PM>
  typename PM::value_type 
  get(reference_wrapper<PM const> pm, typename PM::key_type k) { return get(pm.get(),k);}

  template<class PM>
  typename PM::value_type 
  get(reference_wrapper<PM      > pm, typename PM::key_type k) { return get(pm.get(),k);}

  template<class PM>
  void 
  put(reference_wrapper<PM      > pm, typename PM::key_type k, typename PM::value_type t) 
  { put(pm.get(),k,t);}



} // namespace boost 

#endif
