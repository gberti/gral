#ifndef GRAL_BASE_GB_PARTIAL_GRID_FUNCTION_HASH_H
#define GRAL_BASE_GB_PARTIAL_GRID_FUNCTION_HASH_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include <my-hash-map.h> // STL

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-hash.h"

//----------------------------------------------------------------
// 
// supply a generic base for partial grid functions.
// This template assumes that a hash function is associated with
// a element_handle type via element_traits<E>.
//
// The main difference between a partial grid function and a
// hashed total grid function is the handling of read access to
// undefined item: in the total version this is flagged as error
// (program termination!), whereas for the partial version a
// default value (which can be manipulated seperately) is returned.
//
//----------------------------------------------------------------


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

  ElementIterator_generic_pgf() {}
  ElementIterator_generic_pgf(const base_citer& i, const base_citer& e) 
    : base_citer(i), end(e)
    {}
  element_type operator*() const { return (base_citer::operator*()).first;}
  bool IsDone() const { return (end == ((const base_citer&) (*this)));}
  element_handle handle() const { return (base_citer::operator*()).first.handle();}
};


template<class EIt, class ER, class element_tag>
struct map_element_iter_name {};

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, vertex_type_tag>
{ 
  typedef EIt VertexIterator;
  VertexIterator FirstVertex() const { 
    return VertexIterator(((ER const*)(this))->FirstElement());
  }
  unsigned NumOfVertices() const {
    return (static_cast<ER const*>(this))->size();
  }
};

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, edge_type_tag>
{ 
  typedef EIt EdgeIterator;
  EdgeIterator FirstEdge() const { 
    return (static_cast<ER const*>(this))->FirstElement();
  }
  unsigned NumOfEdges() const {
    return (static_cast<ER const*>(this))->size();
  }
};

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, facet_type_tag>
{ 
  typedef EIt FacetIterator;
  FacetIterator FirstFacet() const { 
    return (static_cast<ER const*>(this))->FirstElement();
  }
  unsigned NumOfFacets() const {
    return (static_cast<ER const*>(this))->size();
  }
};

// choose edge AND facet in 2D 
// if types Edge and Facet coincide.

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, edge2d_type_tag> : 
  public map_element_iter_name<EIt, ER, edge_type_tag>,
  public map_element_iter_name<EIt, ER, facet_type_tag> {};


template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, cell_type_tag>
{ 
  typedef EIt CellIterator;
  CellIterator FirstCell() const { 
    return (static_cast<ER const*>(this))->FirstElement();
  }
  unsigned NumOfCells() const {
    return (static_cast<ER const*>(this))->size();
  }
};

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, cell2d_type_tag>
  : public map_element_iter_name<EIt, ER, cell_type_tag> {};

template<class EIt, class ER>
struct map_element_iter_name<EIt, ER, cell3d_type_tag>
  : public map_element_iter_name<EIt, ER, cell_type_tag> {};


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

public:
  partial_grid_function() 
    : base_gf(), default_value(T()) {} 
  partial_grid_function(const grid_type& gg) 
    : base_gf(gg), default_value(T()) {} 
  partial_grid_function(const grid_type& gg, const T& dflt)
    : base_gf(gg), default_value(dflt)  {}
  
  //----------------- data access operators -------------------------   

   // read access:
  const T&   operator()(const E& e) const {
    REQUIRE( (&(e.TheGrid()) == g),
             "gf(e): Grids don't match: e.TheGrid() = "
             << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    typename table_type::const_iterator i = table.find(e);
    return ( (i != table.end()) ?
             (*i).second
             : default_value);
  }

  // write access:
  T&  operator[](const E& e)       { 
    REQUIRE( (&(e.TheGrid()) == g),
             "gf[e]: Grids don't match: e.TheGrid() = "
             << &(e.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1); 
    typename table_type::iterator i = table.find(e);
    typedef typename table_type::value_type table_value;
    if(i == table.end()) 
      i = (table.insert(table_value(e,default_value))).first ;
    return ((*i).second);
    //    return (table[e]);
  }

  const T& get_default() const { return default_value;}
  void set_default(const T& t) { default_value = t;}


  typedef ElementIterator_generic_pgf<E,T> ElementIterator;
  ElementIterator FirstElement() const { return ElementIterator(table.begin(),table.end());}


};

#endif
