#ifndef NMWR_GB_OVERLAPPING_GRID_FUNCTION_H
#define NMWR_GB_OVERLAPPING_GRID_FUNCTION_H


// $LICENSE

#include <vector.h>

#include "Config/compiler-config.h"
#include "Gral/Base/grid-functions.h"

//----------------------------------------------------------------
/*! \brief A grid function that knows which parts are owned by other parts.
    \ingroup overlappinggrids
    \partof  overlappinggrids

    \templateparams
    - E element type
    - T value type
    - DGrid: overlapping grid, for example overlapping_grid

   \see module gridfunctions
 */
//----------------------------------------------------------------


template<class E, class T, class DGrid>
class overlapping_grid_function {
  typedef overlapping_grid_function<E,T,DGrid> self;
public:
  typedef element_traits<E> et;
  typedef typename et::grid_type       base_grid_type;
  typedef DGrid                        ovrlp_grid_type;
  typedef typename DGrid::overlap_type overlap_type;
 
  typedef E  element_type;
  typedef T  value_type;
  typedef typename grid_function<E,T>::size_type      size_type;
  typedef typename grid_function<E,T>::const_iterator const_iterator;
private:
  //------- DATA  -------
  const ovrlp_grid_type* the_ovrlp_grid;
  const overlap_type *   the_overlap;
  grid_function<E,T>     data;

  /*
  vector<combine_type_tag>  combine_types;

  combine_type_tag combine_type(const comm_range& cr) const 
    { return combine_types[cr.layer_type];}
  */

public:
  //------------------ construction -----------------------

  overlapping_grid_function() :  the_ovrlp_grid(0), the_overlap(0) {}

  overlapping_grid_function(const ovrlp_grid_type& og)
    : the_ovrlp_grid(& og), 
      the_overlap(& og.TheOverlap()),
      data(og.TheGrid())
    {}

  overlapping_grid_function(const ovrlp_grid_type& og, const T& t)
    : the_ovrlp_grid(& og), 
      the_overlap(& og.TheOverlap()),
      data(og.TheGrid(),t)
    {}

  void set_grid(const ovrlp_grid_type& dg)
    {
      the_ovrlp_grid = & dg;
      the_overlap    = & dg.TheOverlap();
      data.set_grid(dg.TheGrid());

    }

  //-----------------  data access  ------------------------

  T& operator[](const E& e) { return data[e];}
  const T& operator[](const E& e) const { return data(e);}
  const T& operator()(const E& e) const { return data(e);}

  //---------------- component access ----------------------

  overlap_type const& TheOverlap() const { return (*the_overlap);}

  grid_function<E,T> const& TheBaseGridFunction() const { return data;}
  grid_function<E,T>      & TheBaseGridFunction()       { return data;}

  //-------------------  ranges -----------------------------

  //  range_type_ref TheCalculationRange() const;

  size_type       size() const { return data.size();}
  const_iterator  begin() const { return data.begin();}
  const_iterator  end()   const { return data.end  ();}
};


#endif
